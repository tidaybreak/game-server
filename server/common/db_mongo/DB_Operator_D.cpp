/*
 * DB_Operator_D.cpp
 *
 *  Created on: 2014年11月14日
 *      Author: Linqiyou
 */

#include "DB_Operator.h"
#include "DB_Fields.h"
#include "Configurator.h"
#include "Record_Client.h"
#include "Pool_Manager.h"
#include "Server_Struct.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "mongo/client/dbclient.h"
#include "gang/Chat_Gang_Struct.h"
#include "DB_Def.h"
#include "Msg_Inner_Struct.h"
#include "answer/Answer_Struct_Scene.h"
#include "Msg_Struct.h"
#include "activity/Activity_Struct.h"
#include "expedition/Expedition_Struct.h"
#include "r2_invite/R2_Invite_Struct.h"
#include "expedition_scene/Expedition_Scene_Struct.h"
#include "activities_stores/Activities_Stores_Struct.h"
#include "active/Active_Struct.h"
#include "oper_activity/Oper_Activity_Struct.h"

using namespace mongo;

#include "DB_Manager.h"

int DB_Operator::load_heroer_status_detail(Heroer_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Heroer_Fields::STATUS_COLLECTION, QUERY(Heroer_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	BSONObjIterator it_part(res.getObjectField(Status_Fields::STATUS_MAP.c_str()));
	Status_Info status_info;
	while (it_part.more()) {
		BSONObj obj_st = it_part.next().embeddedObject();
		status_info.reset();
		status_info.status_id_ = obj_st[Status_Fields::Status_Map::STATUS_ID].numberInt();
		status_info.overlay_ = obj_st[Status_Fields::Status_Map::OVERLAY].numberInt();
		status_info.action_times = obj_st[Status_Fields::Status_Map::ACTION_TIMES].numberInt();
		status_info.trigger_times = obj_st[Status_Fields::Status_Map::TRIGGER_TIMES].numberInt();
		status_info.cfg_action_times = obj_st[Status_Fields::Status_Map::CFG_ACTION_TIMES].numberInt();
		status_info.cfg_trigger_times = obj_st[Status_Fields::Status_Map::CFG_TRIGGER_TIMES].numberInt();
		status_info.last_status_from_ = obj_st[Status_Fields::Status_Map::LAST_STATUS_FROM].numberInt();
		status_info.last_caster_role_id_ = obj_st[Status_Fields::Status_Map::LAST_CASTER_ROLE_ID].numberLong();
		status_info.last_caster_master_id_ = obj_st[Status_Fields::Status_Map::LAST_CASTER_MASTER_ID].numberLong();
//		if (obj_st.hasField(Status_Fields::Status_Map::IS_FIGHT_BUFF.c_str())) {
			status_info.is_fight_buff = obj_st[Status_Fields::Status_Map::IS_FIGHT_BUFF].booleanSafe();
//		}
		status_info.last_time_ms = obj_st[Status_Fields::Status_Map::LAST_TIME_MS].numberLong();

		BSONObjIterator it_bs(obj_st.getObjectField(Status_Fields::Status_Map::BUFF_SCENE.c_str()));
		int scene_id = 0;
		while (it_bs.more()) {
			scene_id = it_bs.next().numberInt();
			status_info.buff_scene.push_back(scene_id);
		}

		BSONObjIterator it_sv(obj_st.getObjectField(Status_Fields::Status_Map::SCENE_VISIBLE.c_str()));
		while (it_sv.more()) {
			scene_id = it_sv.next().numberInt();
			status_info.scene_visible.push_back(scene_id);
		}

		if (obj_st.hasField(Status_Fields::Status_Map::LAST_START_TIME.c_str())) {
			BSONObj obj = obj_st[Status_Fields::Status_Map::LAST_START_TIME].embeddedObject();
			status_info.last_start_time.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
		}
		if (obj_st.hasField(Status_Fields::Status_Map::LAST_END_TIME.c_str())) {
			BSONObj obj = obj_st[Status_Fields::Status_Map::LAST_END_TIME].embeddedObject();
			status_info.last_end_time.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
		}
		BSONObjIterator it_fifo(obj_st.getObjectField(Status_Fields::Status_Map::STATUS_FIFO.c_str()));
		role_id_t role_id = 0;
		while (it_fifo.more()) {
			role_id = it_fifo.next().numberLong();
			status_info.status_fifo.push_back(role_id);
		}
		BSONObjIterator it_info(obj_st.getObjectField(Status_Fields::Status_Map::STATUS_INFO_MAP.c_str()));
		Status_Info_Detail status_info_detail;
		while (it_info.more()) {
			BSONObj obj_info = it_info.next().embeddedObject();
			role_id = obj_info[Status_Fields::ROLE_ID].numberLong();
			status_info_detail.reset();
			status_info_detail.status_id = obj_info[Status_Fields::Status_Map::Status_Info_Map::STATUS_ID].numberInt();
			status_info_detail.status_from = obj_info[Status_Fields::Status_Map::Status_Info_Map::STATUS_FROM].numberInt();
			status_info_detail.caster_role_id = obj_info[Status_Fields::Status_Map::Status_Info_Map::CASTER_ROLE_ID].numberLong();
			status_info_detail.caster_master_id = obj_info[Status_Fields::Status_Map::Status_Info_Map::CASTER_MASTER_ID].numberLong();
			status_info_detail.overlay = obj_info[Status_Fields::Status_Map::Status_Info_Map::OVERLAY].numberInt();

			BSONObjIterator it_em(obj_info.getObjectField(Status_Fields::Status_Map::Status_Info_Map::EFFECT_MAP.c_str()));
			Status_Effect_Detail status_effect_detail;
			int ht = 0;
			while (it_em.more()) {
				BSONObj obj_em = it_em.next().embeddedObject();
				status_effect_detail.reset();
				BSONObjIterator it_ht(obj_em.getObjectField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::HEAT_TYPE.c_str()));
				while (it_ht.more()) {
					ht = it_ht.next().numberInt();
					status_effect_detail.heat_type.insert(ht);
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_MS.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_MS].embeddedObject();
					status_effect_detail.delay_time_ms.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::NEXT_OPT.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::NEXT_OPT].embeddedObject();
					status_effect_detail.next_opt.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::INTERVAL.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::INTERVAL].embeddedObject();
					status_effect_detail.interval.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_TICK.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_TICK].embeddedObject();
					status_effect_detail.delay_time_tick.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}

				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::TRIGGER_BUFF.c_str())) {
					BSONObj obj_tb = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::TRIGGER_BUFF].embeddedObject();
					BSONObjIterator it_dt(obj_tb.getObjectField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DATA.c_str()));
					Effect_Argv_Prop prop;
					while (it_dt.more()) {
						BSONObj obj_dt = it_dt.next().embeddedObject();
						prop.reset();
						prop.effect_value = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::EFFECT_VALUE].numberDouble();
						prop.i_effect_val1 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL1].numberInt();
						prop.i_effect_val2 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL2].numberInt();
						prop.i_effect_val3 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL3].numberInt();
						prop.d_effect_val1 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL1].numberDouble();
						prop.d_effect_val2 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL2].numberDouble();
						prop.d_effect_val3 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL3].numberDouble();
						prop.d_effect_val4 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL4].numberDouble();
						prop.d_effect_val5 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL5].numberDouble();
						prop.d_effect_val6 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL6].numberDouble();
						prop.d_effect_val7 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL7].numberDouble();
						prop.d_effect_val8 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL8].numberDouble();
						prop.d_effect_val9 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL9].numberDouble();
						status_effect_detail.trigger_buff.data.push_back(prop);
					}
					BSONObjIterator it_dg(obj_tb.getObjectField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DISAPPEAR_GAP.c_str()));
					while (it_dg.more()) {
						ht = it_dg.next().numberInt();
						status_effect_detail.trigger_buff.disappear_gap.push_back(ht);
					}
					status_effect_detail.trigger_buff.status_id = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_ID].numberInt();
					status_effect_detail.trigger_buff.last_time_ms = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::LAST_TIME_MS].numberLong();
					status_effect_detail.trigger_buff.cfg_action_gap = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_ACTION_GAP].numberInt();
					status_effect_detail.trigger_buff.cfg_trigger_gap = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_TRIGGER_GAP].numberInt();
					status_effect_detail.trigger_buff.status_from = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_FROM].numberInt();
					status_effect_detail.trigger_buff.caster_id = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CASTER_ID].numberLong();
					status_effect_detail.trigger_buff.overlay = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::OVERLAY].numberInt();
					status_effect_detail.trigger_buff.is_fight_buff = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::IS_FIGHT_BUFF].boolean();
				}
				status_effect_detail.effect_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_ID].numberInt();
				status_effect_detail.effect_value = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_VALUE].numberInt();
				status_effect_detail.i_effect_value1 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE1].numberInt();
				status_effect_detail.i_effect_value2 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE2].numberInt();
				status_effect_detail.i_effect_value3 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE3].numberInt();
				status_effect_detail.d_effect_value1 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE1].numberDouble();
				status_effect_detail.d_effect_value2 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE2].numberDouble();
				status_effect_detail.d_effect_value3 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE3].numberDouble();
				status_effect_detail.d_effect_value4 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE4].numberDouble();
				status_effect_detail.d_effect_value5 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE5].numberDouble();
				status_effect_detail.d_effect_value6 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE6].numberDouble();
				status_effect_detail.d_effect_value7 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE7].numberDouble();
				status_effect_detail.d_effect_value8 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE8].numberDouble();
				status_effect_detail.d_effect_value9 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE9].numberDouble();
				status_effect_detail.effect_type = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TYPE].numberInt();
				status_effect_detail.status_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::STATUS_ID].numberInt();
				status_effect_detail.effect_times = 0;//obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TIMES].numberInt();
				status_effect_detail.overlay = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::OVERLAY].numberInt();
				status_effect_detail.is_overlay = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_OVERLAY].Bool();
				status_effect_detail.is_hit = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_HIT].Bool();
				status_effect_detail.skill_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::SKILL_ID].numberInt();
				status_effect_detail.buff_chance = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::BUFF_CHANCE].numberInt();
				status_effect_detail.caster_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::CASTER_ID].numberLong();
				status_effect_detail.action_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TIMES].numberInt();
				status_effect_detail.action_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TICK].numberInt();
				status_effect_detail.passive_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TIMES].numberInt();
				status_effect_detail.passive_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TICK].numberInt();
				status_effect_detail.is_delay = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_DELAY].Bool();
				status_effect_detail.delay_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIMES].numberInt();
				status_effect_detail.delay_action_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TIMES].numberInt();
				status_effect_detail.delay_passive_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TIMES].numberInt();
				status_effect_detail.delay_action_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TICK].numberInt();
				status_effect_detail.delay_passive_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TICK].numberInt();
				status_effect_detail.last_cure_value = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_VALUE].numberDouble();
				status_effect_detail.last_cure_rate = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_RATE].numberDouble();

				status_effect_detail.buff_scene = status_info.buff_scene;
				status_info_detail.effect_map.push_back(status_effect_detail);
			}
			status_info.status_info_map.insert(std::make_pair(role_id, status_info_detail));
		}
		detail.logic_status_detail_.status_map.insert(std::make_pair(status_info.status_id_, status_info));
	}
	return 0;
}

int DB_Operator::save_heroer_status_detail(Heroer_Detail &detail) {
	std::vector<BSONObj> status_map;
	std::vector<BSONObj> status_fifo;
	std::vector<BSONObj> status_info_map;
	std::vector<BSONObj> effect_map;
	std::vector<BSONObj> trigger_buff;
	std::vector<BSONObj> data;
	std::vector<int> heat_type;
	for (Logic_Status_Detail::Status_Map::const_iterator it = detail.logic_status_detail_.status_map.begin();
			it != detail.logic_status_detail_.status_map.end(); ++it) {
		status_fifo.clear();
		status_info_map.clear();
		Status_Info si = it->second;
		for (std::deque<role_id_t>::const_iterator it_sim = it->second.status_fifo.begin();
				it_sim != it->second.status_fifo.end(); ++it_sim) {
			status_fifo.push_back(BSON(Status_Fields::ROLE_ID << (long long int)*it_sim));
		}
		for (Status_Info::Status_Info_Map::const_iterator it_sim = it->second.status_info_map.begin();
				it_sim != it->second.status_info_map.end(); ++it_sim) {
			effect_map.clear();
			for (Status_Info_Detail::Effect_Vec::const_iterator it_em = it_sim->second.effect_map.begin();
					it_em != it_sim->second.effect_map.end(); ++it_em) {
				heat_type.clear();
				trigger_buff.clear();
				data.clear();
				heat_type.assign((*it_em).heat_type.begin(), (*it_em).heat_type.end());
				for (Effect_Argv_Prop_Vec::const_iterator it_data = (*it_em).trigger_buff.data.begin(); it_data != (*it_em).trigger_buff.data.end(); ++it_data) {
					data.push_back(BSON(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::EFFECT_VALUE << it_data->effect_value
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL1 << it_data->i_effect_val1
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL2 << it_data->i_effect_val2
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL3 << it_data->i_effect_val3
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL1 << it_data->d_effect_val1
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL2 << it_data->d_effect_val2
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL3 << it_data->d_effect_val3
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL4 << it_data->d_effect_val4
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL5 << it_data->d_effect_val5
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL6 << it_data->d_effect_val6
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL7 << it_data->d_effect_val7
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL8 << it_data->d_effect_val8
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL9 << it_data->d_effect_val9));
				}
				trigger_buff.push_back(BSON(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_ID << (*it_em).trigger_buff.status_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::LAST_TIME_MS << (long long int)(*it_em).trigger_buff.last_time_ms
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_ACTION_GAP << (*it_em).trigger_buff.cfg_action_gap
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_TRIGGER_GAP << (*it_em).trigger_buff.cfg_trigger_gap
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_FROM << (*it_em).trigger_buff.status_from
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CASTER_ID << (long long int)(*it_em).trigger_buff.caster_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::OVERLAY << (*it_em).trigger_buff.overlay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DISAPPEAR_GAP << (*it_em).trigger_buff.disappear_gap
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DATA << data
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::IS_FIGHT_BUFF << (*it_em).trigger_buff.is_fight_buff));
				effect_map.push_back(BSON(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_ID << (*it_em).effect_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_VALUE << (*it_em).effect_value
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE1 << (*it_em).i_effect_value1
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE2 << (*it_em).i_effect_value2
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE3 << (*it_em).i_effect_value3
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE1 << (*it_em).d_effect_value1
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE2 << (*it_em).d_effect_value2
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE3 << (*it_em).d_effect_value3
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE4 << (*it_em).d_effect_value4
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE5 << (*it_em).d_effect_value5
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE6 << (*it_em).d_effect_value6
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE7 << (*it_em).d_effect_value7
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE8 << (*it_em).d_effect_value8
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE9 << (*it_em).d_effect_value9
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TYPE << (*it_em).effect_type
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::STATUS_ID << (*it_em).status_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::HEAT_TYPE << heat_type
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TIMES << (*it_em).effect_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::OVERLAY << (*it_em).overlay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_OVERLAY << (*it_em).is_overlay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_HIT << (*it_em).is_hit
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::SKILL_ID << (*it_em).skill_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::TRIGGER_BUFF << trigger_buff
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::BUFF_CHANCE << (*it_em).buff_chance
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::CASTER_ID << (long long int)(*it_em).caster_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TIMES << (*it_em).action_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TICK << (*it_em).action_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TIMES << (*it_em).passive_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TICK << (*it_em).passive_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::NEXT_OPT << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).next_opt.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).next_opt.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::INTERVAL << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).interval.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).interval.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_DELAY << (*it_em).is_delay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIMES << (*it_em).delay_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_MS << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).delay_time_ms.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).delay_time_ms.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TIMES << (*it_em).delay_action_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TIMES << (*it_em).delay_passive_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_TICK << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).delay_time_tick.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).delay_time_tick.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TICK << (*it_em).delay_action_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TICK << (*it_em).delay_passive_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_VALUE << (*it_em).last_cure_value
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_RATE << (*it_em).last_cure_rate));
			}
			status_info_map.push_back(BSON(Status_Fields::ROLE_ID << (long long int)it_sim->first
					<< Status_Fields::Status_Map::Status_Info_Map::STATUS_ID << it_sim->second.status_id
					<< Status_Fields::Status_Map::Status_Info_Map::STATUS_FROM << it_sim->second.status_from
					<< Status_Fields::Status_Map::Status_Info_Map::CASTER_ROLE_ID << (long long int)it_sim->second.caster_role_id
					<< Status_Fields::Status_Map::Status_Info_Map::CASTER_MASTER_ID << (long long int)it_sim->second.caster_master_id
					<< Status_Fields::Status_Map::Status_Info_Map::OVERLAY << it_sim->second.overlay
					<< Status_Fields::Status_Map::Status_Info_Map::EFFECT_MAP << effect_map));
		}

		status_map.push_back(BSON(Status_Fields::Status_Map::STATUS_ID << it->first
				<< Status_Fields::Status_Map::OVERLAY << it->second.overlay_
				<< Status_Fields::Status_Map::ACTION_TIMES << it->second.action_times
				<< Status_Fields::Status_Map::TRIGGER_TIMES << it->second.trigger_times
				<< Status_Fields::Status_Map::CFG_ACTION_TIMES << it->second.cfg_action_times
				<< Status_Fields::Status_Map::CFG_TRIGGER_TIMES << it->second.trigger_times
				<< Status_Fields::Status_Map::LAST_STATUS_FROM << it->second.last_status_from_
				<< Status_Fields::Status_Map::LAST_CASTER_ROLE_ID << (long long int)it->second.last_caster_role_id_
				<< Status_Fields::Status_Map::LAST_CASTER_MASTER_ID << (long long int)it->second.last_caster_master_id_
				<< Status_Fields::Status_Map::IS_FIGHT_BUFF << it->second.is_fight_buff
				<< Status_Fields::Status_Map::LAST_TIME_MS << (long long int)it->second.last_time_ms
				<< Status_Fields::Status_Map::LAST_START_TIME << BSON(Time_Value_Fields::SEC << (long long int)it->second.last_start_time.sec()
						<< Time_Value_Fields::USEC << (long long int)it->second.last_start_time.usec())
				<< Status_Fields::Status_Map::LAST_END_TIME << BSON(Time_Value_Fields::SEC << (long long int)it->second.last_end_time.sec()
						<< Time_Value_Fields::USEC << (long long int)it->second.last_end_time.usec())
				<< Status_Fields::Status_Map::STATUS_FIFO << status_fifo
				<< Status_Fields::Status_Map::STATUS_INFO_MAP << status_info_map
				<< Status_Fields::Status_Map::BUFF_SCENE << it->second.buff_scene
				<< Status_Fields::Status_Map::SCENE_VISIBLE << it->second.scene_visible
				));
	}

	BSONObjBuilder builder;
	builder << Heroer_Fields::ROLE_ID << (long long int)detail.role_id
			<< Status_Fields::STATUS_MAP << status_map;

	MONGO_CONNECTION.update(Heroer_Fields::STATUS_COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::save_answer_info_detail(const Answer_All_Data_Info &answer_info) {
	std::vector<BSONObj> role_sorce_vec;
	for (RoleID_Data_Map::const_iterator it = answer_info.role_sorce_map.begin();
			it != answer_info.role_sorce_map.end(); ++it) {
		role_sorce_vec.push_back(BSON(Answer_Info_Fields::Rank_Role_Data::ROLE_ID << (long long int)it->second.role_id
				<< Answer_Info_Fields::Rank_Role_Data::NAME << it->second.name
				<< Answer_Info_Fields::Rank_Role_Data::LEVEL << it->second.level
				<< 	Answer_Info_Fields::Rank_Role_Data::SORCE << it->second.sorce
				<< 	Answer_Info_Fields::Rank_Role_Data::TRUE_NUM << it->second.true_num
				<< 	Answer_Info_Fields::Rank_Role_Data::EXP << it->second.exp
		));
	}

	std::vector<BSONObj> role_data_vec;
	for (RoleID_Answer_Role_Data_Info_Map::const_iterator it = answer_info.role_data_info_map.begin();
			it != answer_info.role_data_info_map.end(); ++it) {
		BSONObj obj = BSON( Answer_Info_Fields::Role_Info_Data::FREE_NUM << it->second.use_free_num
				<< Answer_Info_Fields::Role_Info_Data::DOUBLE_NUM << it->second.use_double_num
				<< 	Answer_Info_Fields::Role_Info_Data::CUR_FREE_QID << it->second.cur_free_qid
				<< 	Answer_Info_Fields::Role_Info_Data::CUR_DOUBLE_QID << it->second.cur_double_qid
				<< 	Answer_Info_Fields::KEY << (long long int)it->first
		);
		role_data_vec.push_back(obj);
	}

	BSONObj obj =  BSON(
			Answer_Info_Fields::EXT_NUM << answer_info.ext_num <<
			Answer_Info_Fields::CUR_QNO << answer_info.cur_qno <<
			Answer_Info_Fields::QID_LIST << answer_info.question_id_list <<
			Answer_Info_Fields::LAST_TIME << (long long int)answer_info.last_start_time.sec() <<
			Answer_Info_Fields::FORCE_RANK << answer_info.force_rank <<
			Answer_Info_Fields::ARENA_RANK << answer_info.arena_rank <<
			Answer_Info_Fields::GANG_RANK << answer_info.gang_rank <<
			Answer_Info_Fields::ROLE_SORCE_VEC << role_sorce_vec <<
			Answer_Info_Fields::ROLE_DATA_VEC << role_data_vec);

	BSONObjBuilder builder;
	builder << Common_Fields::TYPE << DB_COMMON_TYPE_ANSWER
			<< Common_Fields::VALUE << obj;

	MONGO_CONNECTION.update(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_ANSWER),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_answer_info_detail(Answer_All_Data_Info &answer_info) {
	typedef Answer_Info_Fields FIELDS;
	BSONObj res = MONGO_CONNECTION.findOne(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_ANSWER));
	if (res.isEmpty()) return -1;

	BSONObj obj = res.getObjectField(Common_Fields::VALUE.c_str());
	if (obj.isEmpty()) return -1;

	answer_info.ext_num = obj[FIELDS::EXT_NUM].numberInt();
	answer_info.cur_qno = obj[FIELDS::CUR_QNO].numberInt();
	answer_info.last_start_time.sec(obj[FIELDS::LAST_TIME].numberLong());
	{
		BSONObjIterator iter(res.getObjectField(FIELDS::QID_LIST.c_str()));
		while (iter.more())	{
			answer_info.question_id_list.insert(iter.next().numberInt());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(FIELDS::FORCE_RANK.c_str()));
		while (iter.more())	{
			answer_info.force_rank.push_back(iter.next().valuestrsafe());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(FIELDS::ARENA_RANK.c_str()));
		while (iter.more())	{
			answer_info.arena_rank.push_back(iter.next().valuestrsafe());
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(FIELDS::GANG_RANK.c_str()));
		while (iter.more())	{
			answer_info.gang_rank.push_back(iter.next().valuestrsafe());
		}
	}
	{
		BSONObj rsv_obj = obj.getObjectField(FIELDS::ROLE_SORCE_VEC.c_str());
		BSONObjIterator iter(rsv_obj);
		Answer_Rank_Role_Data data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			data.role_id  = log_obj[FIELDS::Rank_Role_Data::ROLE_ID].numberLong();
			data.name = log_obj[FIELDS::Rank_Role_Data::NAME].valuestrsafe();
			data.exp = log_obj[FIELDS::Rank_Role_Data::EXP].numberInt();
			data.level = log_obj[FIELDS::Rank_Role_Data::LEVEL].numberInt();
			data.true_num = log_obj[FIELDS::Rank_Role_Data::TRUE_NUM].numberInt();
			data.sorce = log_obj[FIELDS::Rank_Role_Data::SORCE].numberInt();
			answer_info.role_sorce_map.insert(std::make_pair(data.role_id, data));
		}
	}
	{
		BSONObj rdv_obj = obj.getObjectField(FIELDS::ROLE_DATA_VEC.c_str());
		BSONObjIterator iter(rdv_obj);
		Answer_Role_Data_Info data;
		while(iter.more()) {
			data.reset();
			BSONObj log_obj = iter.next().embeddedObject();
			role_id_t role_id  = log_obj[FIELDS::KEY].numberLong();

			data.use_free_num = log_obj[FIELDS::Role_Info_Data::FREE_NUM].numberInt();
			data.use_double_num = log_obj[FIELDS::Role_Info_Data::DOUBLE_NUM].numberInt();
			data.cur_free_qid = log_obj[FIELDS::Role_Info_Data::CUR_FREE_QID].numberInt();
			data.cur_double_qid = log_obj[FIELDS::Role_Info_Data::CUR_DOUBLE_QID].numberInt();
			answer_info.role_data_info_map.insert(std::make_pair(role_id, data));
		}
	}
	return 0;
}

int DB_Operator::save_two_recharge_info(Two_Recharge_Info &info) {
	BSONObj obj =  BSON(
			Two_Recharge_Info_Fields::REC_NUM << info.rec_num <<
			Two_Recharge_Info_Fields::LAST_TIME << info.last_start_time );

	BSONObjBuilder builder;
	builder << Common_Fields::TYPE << DB_COMMON_TYPE_TWO_RECHARGE
			<< Common_Fields::VALUE << obj;

	MONGO_CONNECTION.update(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_TWO_RECHARGE),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_two_recharge_info(Two_Recharge_Info &info) {
	info.reset();
	BSONObj res = MONGO_CONNECTION.findOne(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_TWO_RECHARGE));
	if (res.isEmpty()) {
		return -1;
	}
	info.rec_num = res[Two_Recharge_Info_Fields::REC_NUM].numberInt();
	info.last_start_time = res[Two_Recharge_Info_Fields::LAST_TIME].numberInt();
	return 0;
}

int DB_Operator::save_questionnaire_info( role_id_t role_id, std::vector<Questionnaire_Answer_Info> &ans_info) {
	for(std::vector<Questionnaire_Answer_Info>::iterator it = ans_info.begin(); it != ans_info.end(); ++it) {
		BSONObjBuilder builder,builder_obj;
		builder_obj<< Questionnaite_Info_Fields::List::QNO << it->qno;
		if(it->radio) {
			builder_obj<< Questionnaite_Info_Fields::List::RADIO << it->radio;
		}
		if(!it->mult.empty()) {
			builder_obj<< Questionnaite_Info_Fields::List::MULT << it->mult;
		}
		if(!it->contents.empty()) {
			builder_obj<< Questionnaite_Info_Fields::List::CONTENTS << it->contents;
		}
		builder.append(Questionnaite_Info_Fields::LIST, builder_obj.obj());
		MONGO_CONNECTION.update(Questionnaite_Info_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id),
				BSON("$push" << builder.obj() ), true);
	}
	return 0;
}

int DB_Operator::save_inner_check_info(MSG_20999999 msg, std::string &ip) {
	BSONObjBuilder builder;
	builder << Inner_Check_Info_Fields::CORE_NUM << msg.core_num
			<< Inner_Check_Info_Fields::UPDATE_TIME << (long long int)Time_Value::gettimeofday().sec()
			<< "ip" << ip;

	MONGO_CONNECTION.update(Inner_Check_Info_Fields::COLLECTION, QUERY(Inner_Check_Info_Fields::SERVER << msg.server),
				BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::save_inner_check_info(Block_Buffer &buf) {
	bool is_start = false;
	std::string server_flag;

	BSONObjBuilder builder;

	uint8_t type = 0;
	while (true) {
		bool break_now = false;

		if (!buf.readable_bytes())
			break;

		buf.read_uint8(type);
		switch (type) {
		case 1: {
			buf.read_string(server_flag);
			break;
		}
		case 2: {
			uint8_t core_amount = 0;
			buf.read_uint8(core_amount);
			builder << Inner_Check_Info_Fields::CORE_NUM << core_amount;
			break;
		}
		case 3: {
			std::string logic_ip_port;
			buf.read_string(logic_ip_port);
			builder << "logic_ip_port" << logic_ip_port;
			break;
		}
		case 4: {
			is_start = true;
			std::string server_ver;
			buf.read_string(server_ver);
			builder << "ver" << server_ver;
			break;
		}
		case 5: {
			uint32_t start_time = 0;
			buf.read_uint32(start_time);
			builder << "s_time" << start_time;
			break;
		}
		case 6: {
			role_id_t server_id = 0;
			buf.read_int64(server_id);
			builder << "s_id" << (long long int)server_id;
			break;
		}
		case 7: {
			int32_t player_total = 0;
			buf.read_int32(player_total);
			builder << "online" << player_total;
			break;
		}
		case 8: {
			uint8_t lan = 0;
			buf.read_uint8(lan);
			builder << "lan" << lan;
			break;
		}
		case 9: {
			std::string mongo_ip_port;
			buf.read_string(mongo_ip_port);
			builder << "mongo_ip_port" << mongo_ip_port;
			break;
		}
		case 10: {
			Time_Value time_send;
			time_send.deserialize(buf);
			Time_Value time_delay = Time_Value::gettimeofday() - time_send;
			builder << "time_delay" << (long long int)(time_delay.sec() * 1000 * 1000 + time_delay.usec());
			break;
		}
		case 11: {
			std::stringstream str_stream;
			str_stream << "mem_used.";
			int mem_used = 0;
			uint8_t server_index = 0;
			buf.read_uint8(server_index);
			str_stream << (int)server_index;
			buf.read_int32(mem_used);
			builder << str_stream.str() << mem_used;
			break;
		}
		case 12: {
			std::stringstream str_stream;
			str_stream << "cpu_per.";
			int cpu_per = 0;
			uint8_t server_index = 0;
			buf.read_uint8(server_index);
			str_stream << (int)server_index;
			buf.read_int32(cpu_per);
			builder << str_stream.str() << cpu_per;
			break;
		}
		case 13: {
			int solution_id = 0;
			buf.read_int32(solution_id);
			builder << "so_id" << solution_id;
			break;
		}
		case 14: {
			uint32_t l_l_max = 0;
			buf.read_uint32(l_l_max);
			builder << "l_l_max" << l_l_max;
			break;
		}
		case 15: {
			uint32_t r_l_max = 0;
			buf.read_uint32(r_l_max);
			builder << "r_l_max" << r_l_max;
			break;
		}
		case 16: {
			uint32_t inter_flag = 0;
			buf.read_uint32(inter_flag);
			builder << "inter" << inter_flag;
			break;
		}
		case 17: {
			uint32_t game_start = 0;
			buf.read_uint32(game_start);
			builder << "game_start" << game_start;
			break;
		}
		case 18: {
			uint32_t p_total = 0;
			buf.read_uint32(p_total);
			builder << "p_total" << p_total;
			break;
		}
		case 19: {
			uint32_t t_zone = 0;
			buf.read_uint32(t_zone);
			builder << "t_zone" << t_zone;
			break;
		}
		case 20: {
			int hi_power = 0;
			buf.read_int32(hi_power);
			builder << "h_power" << hi_power;
			break;
		}
		case 21: {
			int hi_power_avg = 0;
			buf.read_int32(hi_power_avg);
			builder << "a_power" << hi_power_avg;
			break;
		}
		case 22: {
			int m_men = 0;
			buf.read_int32(m_men);
			builder << "m_men" << m_men;
			break;
		}
		case 23: {
			int m_cpu = 0;
			buf.read_int32(m_cpu);
			builder << "m_cpu" << m_cpu;
			break;
		}

		default: {
			break_now = true;
		}
		}

		if (break_now) break;
	}

	if (server_flag == "") {
		return -1;
	}

	if (is_start) {
		std::vector<int> vec_null;
		builder << "mem_used" << vec_null;
		builder << "cpu_per" << vec_null;
	}

	builder << Inner_Check_Info_Fields::UPDATE_TIME << (long long int)Time_Value::gettimeofday().sec();

	MONGO_CONNECTION.update(Inner_Check_Info_Fields::COLLECTION, QUERY(Inner_Check_Info_Fields::SERVER << server_flag),
				BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_exchange_detail(Exchange_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Exchange_Detail_Fields::COLLECTION,
				QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));

	if (res.isEmpty()) {
		return -1;
	}

	Title_Detail_Info_Detail info;
	BSONObjIterator iter_title(res.getObjectField(Exchange_Detail_Fields::EX_DATA.c_str()));
	while(iter_title.more()) {
		BSONObj obj = iter_title.next().embeddedObject();
		int k = obj[Exchange_Detail_Fields::KEY].numberInt();
		int v = obj[Exchange_Detail_Fields::VALUE].numberInt();
		detail.ex_data.insert(std::make_pair(k, v));
	}
	{
		BSONObjIterator iter_title(res.getObjectField(Exchange_Detail_Fields::EX_DATA_EDAY.c_str()));
		while(iter_title.more()) {
			BSONObj obj = iter_title.next().embeddedObject();
			int k = obj[Exchange_Detail_Fields::KEY].numberInt();
			int v = obj[Exchange_Detail_Fields::VALUE].numberInt();
			detail.ex_data_every_day.insert(std::make_pair(k, v));
		}
	}

	return 0;
}

int DB_Operator::save_exchange_detail(Exchange_Detail &detail) {
	std::vector<BSONObj> ex_data, ex_data_eday;
	for (Int_Int_Map::const_iterator iter = detail.ex_data.begin();
			iter != detail.ex_data.end(); ++iter) {
		ex_data.push_back(BSON(Exchange_Detail_Fields::KEY << iter->first
				<< Exchange_Detail_Fields::VALUE << iter->second));
	}

	for (Int_Int_Map::const_iterator iter = detail.ex_data_every_day.begin();
			iter != detail.ex_data_every_day.end(); ++iter) {
		ex_data_eday.push_back(BSON(Exchange_Detail_Fields::KEY << iter->first
				<< Exchange_Detail_Fields::VALUE << iter->second));
	}
	BSONObjBuilder builder;
	builder << Exchange_Detail_Fields::EX_DATA << ex_data
			<< Exchange_Detail_Fields::EX_DATA_EDAY << ex_data_eday;

	MONGO_CONNECTION.update(Exchange_Detail_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_activity_detail(Activity_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Activity_Fields::COLLECTION,
				QUERY(Activity_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	detail.is_receive_37phone = res[Activity_Fields::IS_REC_37PHONE].booleanSafe();
	detail.is_rec_r2_gift = res[Activity_Fields::IS_REC_R2_LOVE_GIFT].booleanSafe();
	{
		BSONObjIterator iter_set(res.getObjectField(Activity_Fields::VIP37_RIDS.c_str()));
		while (iter_set.more()) {
			detail.vip37_receive_id.insert(iter_set.next().numberInt());
		}
	}
	return 0;
}

int DB_Operator::save_activity_detail(Activity_Detail &detail) {
	BSONObjBuilder builder;
	builder << Activity_Fields::VIP37_RIDS << detail.vip37_receive_id
			<< Activity_Fields::IS_REC_37PHONE << detail.is_receive_37phone
			<< Activity_Fields::IS_REC_R2_LOVE_GIFT << detail.is_rec_r2_gift;

	MONGO_CONNECTION.update(Activity_Fields::COLLECTION, QUERY(Activity_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::save_r2_invite_detail(const R2_Invite_Detail &detail) {
	return 0;
}

int DB_Operator::load_r2_invite_detail(R2_Invite_Detail &detail) {
	return 0;
}

int DB_Operator::save_activities_stores_detail(const Activities_Stores_Detail &detail) {
	BSONObjBuilder builder;
	std::vector<BSONObj> id_num;
	for(UInt_UInt_Map::const_iterator iter = detail.id_num_buy.begin(); iter != detail.id_num_buy.end(); ++iter) {
		id_num.push_back(BSON(Reward_Fields::KEY << iter->first
				<< Reward_Fields::VALUE << iter->second));
	}
	builder << Activities_Exchange_Stores_Detail_Fields::ID_NUM_BUY << id_num;
	MONGO_CONNECTION.update(Activities_Exchange_Stores_Detail_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_activities_stores_detail(Activities_Stores_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Activities_Exchange_Stores_Detail_Fields::COLLECTION,
				QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	BSONObjIterator iter_title(res.getObjectField(Activities_Exchange_Stores_Detail_Fields::ID_NUM_BUY.c_str()));
	while(iter_title.more()) {
		BSONObj obj = iter_title.next().embeddedObject();
		int k = obj[Reward_Fields::KEY].numberInt();
		int v = obj[Reward_Fields::VALUE].numberInt();
		detail.id_num_buy.insert(std::make_pair(k, v));
	}

	return 0;
	return 0;
}

int DB_Operator::save_active_data(Active_Data_Map &data_map) {
	std::vector<BSONObj> data_vec;
	for (Active_Data_Map::const_iterator it = data_map.begin();
			it != data_map.end(); ++it) {
		BSONObj obj = BSON( Active_Data_Fields::ID << it->second.id
				<< Active_Data_Fields::NUM << it->second.num
				<< 	Active_Data_Fields::BEGIN_TIME << (long long int)it->second.begin_time.sec()
				<< 	Active_Data_Fields::END_TIME << (long long int)it->second.end_time.sec()
		);
		data_vec.push_back(obj);
	}
	BSONObjBuilder builder;
	builder << Common_Fields::TYPE << DB_COMMON_TYPE_ACTIVE
			<< Common_Fields::VALUE << data_vec;

	MONGO_CONNECTION.update(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_ACTIVE),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_active_data(Active_Data_Map &data_map) {
	BSONObj res = MONGO_CONNECTION.findOne(Common_Fields::COLLECTION, QUERY(Common_Fields::TYPE << DB_COMMON_TYPE_ACTIVE));
	if (res.isEmpty()) return -1;
	{
		BSONObjIterator iter(res.getObjectField(Common_Fields::VALUE.c_str()));
		while (iter.more())	{
			BSONObj obj = iter.next().embeddedObject();
			uint32_t id = obj[Active_Data_Fields::ID].numberInt();
			uint32_t num = obj[Active_Data_Fields::NUM].numberInt();
			int64_t b =  obj[Active_Data_Fields::BEGIN_TIME].numberLong();
			int64_t e =  obj[Active_Data_Fields::END_TIME].numberLong();
			data_map.insert(std::make_pair(id, Active_Data_Stauts(id, num, b, e)));
		}
	}
	return 0;
}

int DB_Operator::load_activities_stores_goods(Activity_Exchange_Mall_Goods_Info_Map &goods_vec) {
	unsigned long long res_count = MONGO_CONNECTION.count(Activities_Exchange_Stores_Fields::COLLECTION);
	vector<BSONObj> res;
		BSONObjBuilder field_builder;
		field_builder 	<< Activities_Exchange_Stores_Fields::ID << 1
						<< Activities_Exchange_Stores_Fields::ITEM_ID << 1
						//<< Activities_Exchange_Stores_Fields::ITEM_NUM << 1
						<< Activities_Exchange_Stores_Fields::EXCHANGE_ID << 1
						<< Activities_Exchange_Stores_Fields::EXCHANGE_NUM << 1
						<< Activities_Exchange_Stores_Fields::LIMIT_LV << 1
						<< Activities_Exchange_Stores_Fields::LIMIT_EXCHAGNGE << 1
						<< Activities_Exchange_Stores_Fields::CLASSIFY << 1
						<< Activities_Exchange_Stores_Fields::SUBCLASS << 1
						<< Activities_Exchange_Stores_Fields::ACT_ID << 1
						<< Activities_Exchange_Stores_Fields::SORT << 1;
		BSONObj field_bson = field_builder.obj();
		vector<BSONObj> iter_record;
		iter_record.reserve(res_count);
		goods_vec.clear();//清空列表
		Activity_Exchange_Mall_Goods_Info gi;
		MONGO_CONNECTION.findN(iter_record, Activities_Exchange_Stores_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
			BSONObj &obj = *iter;
			gi.reset();
			gi.id = obj[Activities_Exchange_Stores_Fields::ID].numberInt();
			gi.item_id=obj[Activities_Exchange_Stores_Fields::ITEM_ID].numberInt();
		//	gi.item_num=obj[Activities_Exchange_Stores_Fields::ITEM_NUM].numberInt();
			gi.exchange_id=obj[Activities_Exchange_Stores_Fields::EXCHANGE_ID].numberInt();
			gi.exchange_num=obj[Activities_Exchange_Stores_Fields::EXCHANGE_NUM].numberInt();
			gi.limit_lv=obj[Activities_Exchange_Stores_Fields::LIMIT_LV].numberInt();
			gi.limit_exchange=obj[Activities_Exchange_Stores_Fields::LIMIT_EXCHAGNGE].numberInt();
			gi.classify=obj[Activities_Exchange_Stores_Fields::CLASSIFY].numberInt();
			gi.subclass=obj[Activities_Exchange_Stores_Fields::SUBCLASS].numberInt();
			gi.act_id =obj[Activities_Exchange_Stores_Fields::ACT_ID].numberInt();
			gi.sort=obj[Activities_Exchange_Stores_Fields::SORT].numberInt();
			//插入数据
			goods_vec.insert(std::make_pair(gi.id, gi));
		}
	return 0;
}

int DB_Operator::load_expedition_detail(Expedition_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Expedition_Fields::COLLECTION,
					QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	detail.coord.type = res[Role_Fields::Location::COORD_TYPE].numberInt();
	detail.coord.x = res[Role_Fields::Location::COORD_X].numberInt();
	detail.coord.y = res[Role_Fields::Location::COORD_Y].numberInt();
	detail.award_time.sec(res[Time_Value_Fields::SEC].numberLong());
	detail.award_time.usec(res[Time_Value_Fields::USEC].numberLong());
	{
		BSONObjIterator iter_set(res.getObjectField(Expedition_Fields::COLLECT_CHEST.c_str()));
		while (iter_set.more()) {
			detail.collect_chest.insert(iter_set.next().numberInt());
		}
	}
	{
		BSONObjIterator iter_set(res.getObjectField(Expedition_Fields::COLLECT_BEAST.c_str()));
		while (iter_set.more()) {
			detail.collect_beast.insert(iter_set.next().numberInt());
		}
	}
	{
		BSONObjIterator iter_set(res.getObjectField(Expedition_Fields::KILL_BEAST.c_str()));
		while (iter_set.more()) {
			BSONObj obj = iter_set.next().embeddedObject();
			int k = obj[Expedition_Fields::KEY].numberInt();
			int v = obj[Expedition_Fields::VAL].numberInt();
			detail.kill_beast.insert(std::make_pair(k, v));
		}
	}
	return 0;
}

int DB_Operator::save_expedition_detail(const Expedition_Detail &detail) {
	std::vector<BSONObj> kill_beast, box_collect;
	Int_Vec collect_chest, collect_beast;
	for (Int_Int_Map::const_iterator it = detail.kill_beast.begin();
			it != detail.kill_beast.end(); ++it) {
		kill_beast.push_back(BSON(Expedition_Fields::KEY << it->first
				<< Expedition_Fields::VAL << it->second ));
	}
	for (Int_Hash_Set::const_iterator it = detail.collect_beast.begin();
			it != detail.collect_beast.end(); ++it) {
		collect_beast.push_back(*it);
	}
	for (Int_Hash_Set::const_iterator it = detail.collect_chest.begin();
			it != detail.collect_chest.end(); ++it) {
		collect_chest.push_back(*it);
	}

	for (Int_Time_Map::const_iterator it = detail.box_collect.begin(); it != detail.box_collect.end(); ++it) {
		box_collect.push_back(BSON(Expedition_Fields::ID << it->first
				<< Time_Value_Fields::SEC << (long long int)it->second.sec()
				<< Time_Value_Fields::USEC << (long long int)it->second.usec()));
	}

	BSONObjBuilder builder;
	builder << Expedition_Fields::KILL_BEAST << kill_beast
			<< Expedition_Fields::COLLECT_CHEST << collect_chest
			<< Expedition_Fields::COLLECT_BEAST << collect_beast
			<< Role_Fields::Location::COORD_TYPE << detail.coord.type
			<< Role_Fields::Location::COORD_X << detail.coord.x
			<< Role_Fields::Location::COORD_Y << detail.coord.y
			<< Time_Value_Fields::SEC << (long long int)detail.award_time.sec()
			<< Time_Value_Fields::USEC << (long long int)detail.award_time.usec()
			<< Expedition_Fields::BOX_COLLECT << box_collect;

	MONGO_CONNECTION.update(Expedition_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_expedition_manager_detail(Expedition_Manager_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Expedition_Manager_Detail_Fields::COLLECTION,
					QUERY(Role_Fields::ROLE_ID << (long long int)0));
	if (res.isEmpty()) {
		return -1;
	}
	{
		detail.flag_time.sec(res[Expedition_Manager_Detail_Fields::FLAG_TIME].numberLong());
		detail.award_time.sec(res[Time_Value_Fields::SEC].numberLong());
		detail.award_time.usec(res[Time_Value_Fields::USEC].numberLong());
		detail.first_listen_time.sec(res[Expedition_Manager_Detail_Fields::FIRST_LISTEN_TIME].numberLong());

	}
	{
		Item_Info item_info;
		int64_t gang_id = 0;
		BSONObjIterator iter_set(res.getObjectField(Expedition_Manager_Detail_Fields::AWARD.c_str()));
		while (iter_set.more()) {
			BSONObj obj = iter_set.next().embeddedObject();
			gang_id = obj[Expedition_Manager_Detail_Fields::Gang_Data_Info_Map::LINE].numberLong();

			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Detail_Fields::AWARD.c_str()));
			while (iter_set.more()) {
				item_info.reset();
				BSONObj obj = iter_set.next().embeddedObject();
				item_info.id = obj[Package_Fields::Item::ID].numberInt();
				item_info.amount = obj[Package_Fields::Item::AMOUNT].numberInt();
				item_info.bind = obj[Package_Fields::Item::BIND].numberInt();
				detail.award[gang_id].push_back(item_info);
			}
		}
	}
	{
		Gang_Data_Info gdi;
		BSONObjIterator iter_set(res.getObjectField(Expedition_Manager_Detail_Fields::GANG_DATA_INFO_MAP.c_str()));
		while (iter_set.more()) {
			gdi.reset();
			BSONObj obj = iter_set.next().embeddedObject();
			int64_t gang_id = obj[Gang_Fields::GANG_ID].numberLong();
			gdi.line = obj[Expedition_Manager_Detail_Fields::Gang_Data_Info_Map::LINE].numberInt();
			gdi.coord.type = obj[Role_Fields::Location::COORD_TYPE].numberInt();
			gdi.coord.x = obj[Role_Fields::Location::COORD_X].numberInt();
			gdi.coord.y = obj[Role_Fields::Location::COORD_Y].numberInt();
			detail.gang_data_info_map.insert(std::make_pair(gang_id, gdi));
		}
	}
	return 0;
}

int DB_Operator::save_expedition_manager_detail(const Expedition_Manager_Detail &detail) {
	std::vector<BSONObj> gang_data_info_map, award;
	for (Expedition_Manager_Detail::Int64_Gang_Data_Info_Map::const_iterator it = detail.gang_data_info_map.begin();
			it != detail.gang_data_info_map.end(); ++it) {
		gang_data_info_map.push_back(BSON(Gang_Fields::GANG_ID << (long long int)it->first
				<< Expedition_Manager_Detail_Fields::Gang_Data_Info_Map::LINE << it->second.line
				<< Role_Fields::Location::COORD_TYPE << it->second.coord.type
				<< Role_Fields::Location::COORD_X << it->second.coord.x
				<< Role_Fields::Location::COORD_Y << it->second.coord.y));
	}

	for (Int64_Item_Info_Vec_Map::const_iterator it_award = detail.award.begin();
			it_award != detail.award.end(); ++it_award) {
		std::vector<BSONObj> item_vec;
		for (Item_Info_Vec::const_iterator it = it_award->second.begin();
				it != it_award->second.end(); ++it) {
			item_vec.push_back(BSON(Package_Fields::Item::ID << it->id
					<< Package_Fields::Item::AMOUNT << it->amount
					<< Package_Fields::Item::BIND << it->bind));
		}
		award.push_back(BSON(Expedition_Manager_Detail_Fields::Gang_Data_Info_Map::LINE << (long long int)it_award->first
				<< Expedition_Manager_Detail_Fields::AWARD << item_vec));
	}

	BSONObjBuilder builder;
	builder << Expedition_Manager_Detail_Fields::GANG_DATA_INFO_MAP << gang_data_info_map
			<< Time_Value_Fields::SEC << (long long int)detail.award_time.sec()
			<< Time_Value_Fields::USEC << (long long int)detail.award_time.usec()
			<< Expedition_Manager_Detail_Fields::AWARD << award
			<< Expedition_Manager_Detail_Fields::FLAG_TIME << (long long int)detail.flag_time.sec()
			<< Expedition_Manager_Detail_Fields::FIRST_LISTEN_TIME << (long long int)detail.first_listen_time.sec();

	MONGO_CONNECTION.update(Expedition_Manager_Detail_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)0),
			BSON("$set" << builder.obj()), true);
	{
		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(now);
		dt.day(1);
		dt.hour(0);
		dt.microsec(0);
		dt.minute(0);
		dt.second(0);
		now.sec(dt.time_sec());
		BSONObjBuilder builder;
			builder << Expedition_Manager_Detail_Fields::GANG_DATA_INFO_MAP << gang_data_info_map
					<< Time_Value_Fields::SEC << (long long int)detail.award_time.sec()
					<< Time_Value_Fields::USEC << (long long int)detail.award_time.usec()
					<< Expedition_Manager_Detail_Fields::AWARD << award
					<< Expedition_Manager_Detail_Fields::FLAG_TIME << (long long int)now.sec();
		MONGO_CONNECTION.update(Expedition_Manager_Detail_Fields::BAK_COLLECTION, QUERY(Expedition_Manager_Detail_Fields::FLAG_TIME << (long long int)now.sec()),
				BSON("$set" << builder.obj()), true);
	}
	return 0;
}

int DB_Operator::load_expedition_global_manager_data(Expedition_Global_Manager_Data &data) {
	BSONObj res = MONGO_CONNECTION.findOne(Expedition_Global_Manager_Data_Fields::COLLECTION,
					QUERY(Role_Fields::ROLE_ID << (long long int)0));
	if (res.isEmpty()) {
		return -1;
	}
	{
		BSONObjIterator iter_set(res.getObjectField(Expedition_Global_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP.c_str()));
		while (iter_set.more()) {
			BSONObj obj = iter_set.next().embeddedObject();
			int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
			int64_t tv_sec = obj[Time_Value_Fields::SEC].numberLong();
			int64_t tv_usec = obj[Time_Value_Fields::USEC].numberLong();
			data.opened_uuid_opentime_map_.insert(std::make_pair(uuid, Time_Value(tv_sec, tv_usec)));
		}
	}
	{
		BSONObjIterator iter_set(res.getObjectField(Expedition_Global_Manager_Data_Fields::UUID_INTER_MAP.c_str()));
		while (iter_set.more()) {
			BSONObj obj = iter_set.next().embeddedObject();
			int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
			int inter = obj[Expedition_Global_Manager_Data_Fields::INTER].numberInt();
			data.uuid_inter_map_.insert(std::make_pair(uuid, inter));
		}
	}
	{
		BSONObjIterator iter_set(res.getObjectField(Expedition_Global_Manager_Data_Fields::OPENTIME.c_str()));
		while (iter_set.more()) {
			BSONObj obj = iter_set.next().embeddedObject();
			int inter = obj[Expedition_Global_Manager_Data_Fields::INTER].numberInt();
			int64_t tv_sec = obj[Time_Value_Fields::SEC].numberLong();
			int64_t tv_usec = obj[Time_Value_Fields::USEC].numberLong();
			data.opentime_.insert(std::make_pair(inter, Time_Value(tv_sec, tv_usec)));
		}
	}

	return 0;
}
int DB_Operator::save_expedition_global_manager_data(const Expedition_Global_Manager_Data &data) {
	std::vector<BSONObj> opened_uuid_opentime_map, uuid_inter_map, opentime;
	for (Role_Id_Time_Map::const_iterator it = data.opened_uuid_opentime_map_.begin();
			it != data.opened_uuid_opentime_map_.end(); ++it) {
		opened_uuid_opentime_map.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->first
				<< Time_Value_Fields::SEC << (long long int)it->second.sec()
				<< Time_Value_Fields::USEC << (long long int)it->second.usec()));
	}
	for (Role_Int_Map::const_iterator it = data.uuid_inter_map_.begin();
			it != data.uuid_inter_map_.end(); ++it) {
		uuid_inter_map.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->first
				<< Expedition_Global_Manager_Data_Fields::INTER << it->second));
	}
	for (Int_Time_Map::const_iterator it = data.opentime_.begin();
			it != data.opentime_.end(); ++it) {
		opentime.push_back(BSON(Expedition_Global_Manager_Data_Fields::INTER << it->first
				<< Time_Value_Fields::SEC << (long long int)it->second.sec()
				<< Time_Value_Fields::USEC << (long long int)it->second.usec()));
	}

	BSONObjBuilder builder;
	builder << Expedition_Global_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP << opened_uuid_opentime_map
			<< Expedition_Global_Manager_Data_Fields::UUID_INTER_MAP << uuid_inter_map
			<< Expedition_Global_Manager_Data_Fields::OPENTIME << opentime;

	MONGO_CONNECTION.update(Expedition_Global_Manager_Data_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)0),
			BSON("$set" << builder.obj()), true);
	{
		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(now);
		dt.day(1);
		dt.hour(0);
		dt.microsec(0);
		dt.minute(0);
		dt.second(0);
		now.sec(dt.time_sec());

		BSONObjBuilder builder;
		builder << Expedition_Global_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP << opened_uuid_opentime_map
				<< Expedition_Global_Manager_Data_Fields::UUID_INTER_MAP << uuid_inter_map
				<< Expedition_Global_Manager_Data_Fields::OPENTIME << opentime
				<< Expedition_Manager_Detail_Fields::FLAG_TIME << (long long int)now.sec();
		MONGO_CONNECTION.update(Expedition_Global_Manager_Data_Fields::BAK_COLLECTION, QUERY(Expedition_Manager_Detail_Fields::FLAG_TIME << (long long int)now.sec()),
				BSON("$set" << builder.obj()), true);
	}
	return 0;
}

int DB_Operator::load_expedition_manager_data(Inter_ExpeditionManagerData_Map &map) {
	unsigned long long row_count = MONGO_CONNECTION.count( Expedition_Manager_Data_Fields::COLLECTION );

	vector<BSONObj> iter_record;
	iter_record.reserve( row_count );

	Expedition_Manager_Data emd;
	MONGO_CONNECTION.findN(iter_record, Expedition_Manager_Data_Fields::COLLECTION, mongo::Query(), row_count);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj& obj = *iter;
		emd.reset();
		emd.inter_ = obj[Expedition_Manager_Data_Fields::INTER].numberInt();

		emd.last_reset_time_.sec(obj[Expedition_Manager_Data_Fields::LAST_RESET_TIME_SEC].numberLong());
		emd.last_reset_time_.usec(obj[Expedition_Manager_Data_Fields::LAST_RESET_TIME_USEC].numberLong());
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
				int64_t tv_sec = obj[Time_Value_Fields::SEC].numberLong();
				int64_t tv_usec = obj[Time_Value_Fields::USEC].numberLong();
				emd.opened_uuid_opentime_map_.insert(std::make_pair(uuid, Time_Value(tv_sec, tv_usec)));
			}
		}
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_UUID_MAP.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
				int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
				emd.gang_id_uuid_map_.insert(std::make_pair(gang_id, uuid));
			}
		}
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::FIRST_GANG_RANK.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
				int rank = obj[Expedition_Manager_Data_Fields::RANK].numberInt();
				emd.first_gang_rank_.insert(std::make_pair(gang_id, rank));
			}
		}
		{
			emd.has_init_ = obj[Expedition_Manager_Data_Fields::HAS_INIT].numberInt();
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::UUID_HAS_INIT.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
				bool hi = obj[Expedition_Manager_Data_Fields::HAS_INIT].numberInt();
				emd.uuid_has_init_.insert(std::make_pair(uuid, hi));
			}
		}
		{
			Expedition_Gang_Rank_Info egri;
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_RANK_MAP.c_str()));
			while (iter_set.more()) {
				egri.reset();
				BSONObj obj = iter_set.next().embeddedObject();
				egri.gang_id = obj[Role_Fields::GANG_ID].numberLong();
				egri.rank = obj[Expedition_Manager_Data_Fields::RANK].numberInt();
				egri.uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
				egri.server_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
				egri.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();
				egri.gang_leader_id = obj[Role_Fields::ROLE_ID].numberLong();
				egri.gang_leader_name = obj[Expedition_Manager_Data_Fields::GANG_LEADER_NAME].valuestrsafe();
				egri.force = obj[Fighter_Fields::FORCE].numberInt();
				egri.num = obj[Expedition_Manager_Data_Fields::NUM].numberInt();
				egri.asset = obj[Expedition_Manager_Data_Fields::ASSET].numberInt();
				egri.level = obj[Role_Fields::LEVEL].numberInt();
				egri.create_time = obj[Expedition_Manager_Data_Fields::CREATE_TIME].numberInt();
				egri.expedition_rank = obj[Expedition_Manager_Data_Fields::EXPEDITION_RANK].numberInt();

				emd.gang_rank_map_.insert(std::make_pair(egri.gang_id, egri));
			}
		}
		{
			Expedition_Gang_Rank_Info egri;
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::UUID_GANG_RANK_MAP.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
				BSONObjIterator iter_set_2(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_RANK_MAP.c_str()));
				while (iter_set_2.more()) {
					egri.reset();
					BSONObj obj = iter_set_2.next().embeddedObject();
					egri.gang_id = obj[Role_Fields::GANG_ID].numberLong();
					egri.rank = obj[Expedition_Manager_Data_Fields::RANK].numberInt();
					egri.uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
					egri.server_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
					egri.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();
					egri.gang_leader_id = obj[Role_Fields::ROLE_ID].numberLong();
					egri.gang_leader_name = obj[Expedition_Manager_Data_Fields::GANG_LEADER_NAME].valuestrsafe();
					egri.force = obj[Fighter_Fields::FORCE].numberInt();
					egri.num = obj[Expedition_Manager_Data_Fields::NUM].numberInt();
					egri.asset = obj[Expedition_Manager_Data_Fields::ASSET].numberInt();
					egri.level = obj[Role_Fields::LEVEL].numberInt();
					egri.create_time = obj[Expedition_Manager_Data_Fields::CREATE_TIME].numberInt();
					egri.expedition_rank = obj[Expedition_Manager_Data_Fields::EXPEDITION_RANK].numberInt();

					emd.uuid_gang_rank_map_[uuid].insert(std::make_pair(egri.gang_id, egri));
				}
			}
		}
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_RANK_SYNC_SUCCESS.c_str()));
			while (iter_set.more()) {
				int64_t gang_id = iter_set.next().numberLong();
				emd.gang_rank_sync_success_.insert(gang_id);
			}
		}
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_RANK_SYNC_NEW_VEC.c_str()));
			while (iter_set.more()) {
				int64_t gang_id = iter_set.next().numberLong();
				emd.gang_rank_sync_new_vec_.push_back(gang_id);
			}
		}

		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_POINT_ID.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
				int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
				emd.gang_id_point_id_.insert(std::make_pair(gang_id, point_id));
			}
		}
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_CUR_LINE.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
				int cur_line = obj[Expedition_Manager_Data_Fields::CUR_LINE].numberInt();
				emd.gang_id_cur_line_.insert(std::make_pair(gang_id, cur_line));
			}
		}
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::UUID_GANG_ID_POINT_ID.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
				BSONObjIterator iter_set_2(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_POINT_ID.c_str()));
				while (iter_set_2.more()) {
					BSONObj obj = iter_set_2.next().embeddedObject();
					int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
					int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
					emd.uuid_gang_id_point_id_[uuid].insert(std::make_pair(gang_id, point_id));
				}
			}
		}
		{
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::UUID_GANG_ID_CUR_LINE.c_str()));
			while (iter_set.more()) {
				BSONObj obj = iter_set.next().embeddedObject();
				int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
				BSONObjIterator iter_set_2(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_CUR_LINE.c_str()));
				while (iter_set_2.more()) {
					BSONObj obj = iter_set_2.next().embeddedObject();
					int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
					int cur_line = obj[Expedition_Manager_Data_Fields::CUR_LINE].numberInt();
					emd.uuid_gang_id_cur_line_[uuid].insert(std::make_pair(gang_id, cur_line));
				}
			}
		}
		{ // line_scene_map_ --
			Expedition_Scene_Data esd;
			Stronghold sh;
			BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::LINE_SCENE_MAP.c_str()));
			while (iter_set.more()) {
				esd.reset();
				BSONObj obj = iter_set.next().embeddedObject();
				esd.cur_line_ = obj[Expedition_Manager_Data_Fields::CUR_LINE].numberInt();
				esd.map_base_point_num_ = obj[Expedition_Scene_Data_Fields::MAP_BASE_POINT_NUM].numberInt();
				esd.award_time_.sec(obj[Time_Value_Fields::SEC].numberLong());
				esd.award_time_.usec(obj[Time_Value_Fields::USEC].numberLong());
				if (obj.hasField(Expedition_Scene_Data_Fields::SETTLE_TIME.c_str())) {
					esd.settle_time_ = obj[Expedition_Scene_Data_Fields::SETTLE_TIME].numberInt();
				}

				BSONObjIterator iter_set_2(obj.getObjectField(Expedition_Scene_Data_Fields::POINT_GANG_ID.c_str()));
				while (iter_set_2.more()) {
					BSONObj obj = iter_set_2.next().embeddedObject();
					int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
					int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
					esd.point_gang_id_.insert(std::make_pair(point_id, gang_id));
				}

				BSONObjIterator iter_set_3(obj.getObjectField(Expedition_Scene_Data_Fields::MAP_DATA.c_str()));
				while (iter_set_3.more()) {
					sh.reset();
					BSONObj obj = iter_set_3.next().embeddedObject();
					sh.point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
					sh.type = (Expedition_Build_Type)obj[Stronghold_Fields::TYPE].numberInt();
					BSONObjIterator iter_set_4(obj.getObjectField(Stronghold_Info_Fields::SOLDIER_MAP.c_str()));
					while (iter_set_4.more()) {
						BSONObj obj = iter_set_4.next().embeddedObject();
						int nums = obj[Stronghold_Info_Fields::NUMS].numberInt();
						int64_t role_id = obj[Role_Fields::ROLE_ID].numberLong();
						sh.info.defend_id_map.insert(std::make_pair(role_id, nums));
					}
					sh.info.occupier.server_uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
					sh.info.occupier.server_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
					sh.info.occupier.gang_id = obj[Role_Fields::GANG_ID].numberLong();
					sh.info.occupier.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();

					esd.map_data_.insert(std::make_pair(sh.point_id, sh));
				}

				Expedition_Scene_Demage_Rank_Data esdrd;
				BSONObjIterator iter_set_4(obj.getObjectField(Expedition_Scene_Data_Fields::DEMAGE_RANK.c_str()));
				while (iter_set_4.more()) {
					esdrd.reset();
					BSONObj obj = iter_set_4.next().embeddedObject();
					esdrd.role_id = obj[Role_Fields::ROLE_ID].numberLong();
					esdrd.role_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
					esdrd.gang_id = obj[Role_Fields::GANG_ID].numberLong();
					esdrd.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();
					esdrd.uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
					esdrd.server_name = obj[Expedition_Scene_Data_Fields::SERVER_NAME].valuestrsafe();
					esdrd.force = obj[Fighter_Fields::FORCE].numberInt();
					esdrd.level = obj[Fighter_Fields::LEVEL].numberInt();
					esdrd.demage = obj[Expedition_Scene_Data_Fields::DEMAGE].numberDouble();
					esd.demage_rank_.insert(std::make_pair(esdrd.role_id, esdrd));
				}

				Expedition_Occupy_Castle_Data eocd;
				BSONObjIterator iter_set_5(obj.getObjectField(Expedition_Scene_Data_Fields::OCCUPY_CASTLE_RANK.c_str()));
				while (iter_set_5.more()) {
					eocd.reset();
					BSONObj obj = iter_set_5.next().embeddedObject();
					eocd.role_id = obj[Role_Fields::ROLE_ID].numberLong();
					eocd.role_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
					eocd.gang_id = obj[Role_Fields::GANG_ID].numberLong();
					eocd.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();
					eocd.server_uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
					eocd.server_name = obj[Expedition_Scene_Data_Fields::SERVER_NAME].valuestrsafe();
					eocd.force = obj[Fighter_Fields::FORCE].numberInt();
					eocd.level = obj[Fighter_Fields::LEVEL].numberInt();
					eocd.nums = obj[Expedition_Scene_Data_Fields::DEMAGE].numberDouble();
					eocd.get_nums = obj[Expedition_Scene_Data_Fields::GET_NUMS].numberDouble();
					esd.occupy_castle_rank_.insert(std::make_pair(eocd.role_id, eocd));
				}

				Expedition_Drop_Or_Material edom;
				BSONObjIterator iter_set_6(obj.getObjectField(Expedition_Scene_Data_Fields::DROP_OR_MATERIAL.c_str()));
				while (iter_set_6.more()) {
					BSONObj obj = iter_set_6.next().embeddedObject();
					int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
					edom.reset();
					edom.birth_time.sec(obj[Time_Value_Fields::SEC].numberLong());
					edom.birth_time.usec(obj[Time_Value_Fields::USEC].numberLong());

					BSONObjIterator iter_set_7(obj.getObjectField(Expedition_Scene_Data_Fields::HAD_COLLECTED.c_str()));
					while (iter_set_7.more()) {
						int64_t role_id = iter_set.next().numberLong();
						edom.had_collected.insert(role_id);
					}
					esd.refreshed_drop_or_material_.insert(std::make_pair(point_id, edom));
				}

				BSONObjIterator iter_set_7(obj.getObjectField(Expedition_Scene_Data_Fields::REFRESHED_MONSTER_TIMING.c_str()));
				while (iter_set_7.more()) {
					BSONObj obj = iter_set_7.next().embeddedObject();
					int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
					Time_Value tv(Time_Value::zero);
					tv.sec(obj[Time_Value_Fields::SEC].numberLong());
					tv.usec(obj[Time_Value_Fields::USEC].numberLong());

					esd.refreshed_monster_timing_.insert(std::make_pair(point_id, tv));
				}

				BSONObjIterator iter_set_8(obj.getObjectField(Expedition_Scene_Data_Fields::REFRESHED_MONSTER_KILL_GAP.c_str()));
				while (iter_set_8.more()) {
					BSONObj obj = iter_set_8.next().embeddedObject();
					int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
					Time_Value tv(Time_Value::zero);
					tv.sec(obj[Time_Value_Fields::SEC].numberLong());
					tv.usec(obj[Time_Value_Fields::USEC].numberLong());

					esd.refreshed_monster_kill_gap_.insert(std::make_pair(point_id, tv));
				}

				emd.line_scene_map_.insert(std::make_pair(esd.cur_line_, esd));
			}
		} // -- line_scene_map_
		{ // uuid_line_scene_map_ --
			Expedition_Scene_Data esd;
			Stronghold sh;
			BSONObjIterator iter_set_1(obj.getObjectField(Expedition_Manager_Data_Fields::UUID_LINE_SCENE_MAP.c_str()));
			while (iter_set_1.more()) {
				BSONObj obj = iter_set_1.next().embeddedObject();
				int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();

				BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::LINE_SCENE_MAP.c_str()));
				while (iter_set.more()) {
					esd.reset();
					BSONObj obj = iter_set.next().embeddedObject();
					esd.cur_line_ = obj[Expedition_Manager_Data_Fields::CUR_LINE].numberInt();
					esd.map_base_point_num_ = obj[Expedition_Scene_Data_Fields::MAP_BASE_POINT_NUM].numberInt();
					esd.award_time_.sec(obj[Time_Value_Fields::SEC].numberLong());
					esd.award_time_.usec(obj[Time_Value_Fields::USEC].numberLong());
					if (obj.hasField(Expedition_Scene_Data_Fields::SETTLE_TIME.c_str())) {
						esd.settle_time_ = obj[Expedition_Scene_Data_Fields::SETTLE_TIME].numberInt();
					}

					BSONObjIterator iter_set_2(obj.getObjectField(Expedition_Scene_Data_Fields::POINT_GANG_ID.c_str()));
					while (iter_set_2.more()) {
						BSONObj obj = iter_set_2.next().embeddedObject();
						int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
						int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
						esd.point_gang_id_.insert(std::make_pair(point_id, gang_id));
					}

					BSONObjIterator iter_set_3(obj.getObjectField(Expedition_Scene_Data_Fields::MAP_DATA.c_str()));
					while (iter_set_3.more()) {
						sh.reset();
						BSONObj obj = iter_set_3.next().embeddedObject();
						sh.point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
						sh.type = (Expedition_Build_Type)obj[Stronghold_Fields::TYPE].numberInt();
						BSONObjIterator iter_set_4(obj.getObjectField(Stronghold_Info_Fields::SOLDIER_MAP.c_str()));
						while (iter_set_4.more()) {
							BSONObj obj = iter_set_4.next().embeddedObject();
							int nums = obj[Stronghold_Info_Fields::NUMS].numberInt();
							int64_t role_id = obj[Role_Fields::ROLE_ID].numberLong();
							sh.info.defend_id_map.insert(std::make_pair(role_id, nums));
						}
						sh.info.occupier.server_uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
						sh.info.occupier.server_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
						sh.info.occupier.gang_id = obj[Role_Fields::GANG_ID].numberLong();
						sh.info.occupier.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();

						esd.map_data_.insert(std::make_pair(sh.point_id, sh));
					}

					Expedition_Scene_Demage_Rank_Data esdrd;
					BSONObjIterator iter_set_4(obj.getObjectField(Expedition_Scene_Data_Fields::DEMAGE_RANK.c_str()));
					while (iter_set_4.more()) {
						esdrd.reset();
						BSONObj obj = iter_set_4.next().embeddedObject();
						esdrd.role_id = obj[Role_Fields::ROLE_ID].numberLong();
						esdrd.role_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
						esdrd.gang_id = obj[Role_Fields::GANG_ID].numberLong();
						esdrd.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();
						esdrd.uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
						esdrd.server_name = obj[Expedition_Scene_Data_Fields::SERVER_NAME].valuestrsafe();
						esdrd.force = obj[Fighter_Fields::FORCE].numberInt();
						esdrd.demage = obj[Expedition_Scene_Data_Fields::DEMAGE].numberDouble();
						esd.demage_rank_.insert(std::make_pair(esdrd.role_id, esdrd));
					}

					Expedition_Occupy_Castle_Data eocd;
					BSONObjIterator iter_set_5(obj.getObjectField(Expedition_Scene_Data_Fields::OCCUPY_CASTLE_RANK.c_str()));
					while (iter_set_5.more()) {
						eocd.reset();
						BSONObj obj = iter_set_5.next().embeddedObject();
						eocd.role_id = obj[Role_Fields::ROLE_ID].numberLong();
						eocd.role_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
						eocd.gang_id = obj[Role_Fields::GANG_ID].numberLong();
						eocd.gang_name = obj[Role_Fields::GANG_NAME].valuestrsafe();
						eocd.server_uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
						eocd.server_name = obj[Expedition_Scene_Data_Fields::SERVER_NAME].valuestrsafe();
						eocd.force = obj[Fighter_Fields::FORCE].numberInt();
						eocd.level = obj[Fighter_Fields::LEVEL].numberInt();
						eocd.nums = obj[Expedition_Scene_Data_Fields::DEMAGE].numberDouble();
						esd.occupy_castle_rank_.insert(std::make_pair(eocd.role_id, eocd));
					}

					Expedition_Drop_Or_Material edom;
					BSONObjIterator iter_set_6(obj.getObjectField(Expedition_Scene_Data_Fields::DROP_OR_MATERIAL.c_str()));
					while (iter_set_6.more()) {
						BSONObj obj = iter_set_6.next().embeddedObject();
						int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
						edom.reset();
						edom.birth_time.sec(obj[Time_Value_Fields::SEC].numberLong());
						edom.birth_time.usec(obj[Time_Value_Fields::USEC].numberLong());

						BSONObjIterator iter_set_7(obj.getObjectField(Expedition_Scene_Data_Fields::HAD_COLLECTED.c_str()));
						while (iter_set_7.more()) {
							int64_t role_id = iter_set_7.next().numberLong();
							edom.had_collected.insert(role_id);
						}
						esd.refreshed_drop_or_material_.insert(std::make_pair(point_id, edom));
					}

					BSONObjIterator iter_set_7(obj.getObjectField(Expedition_Scene_Data_Fields::REFRESHED_MONSTER_TIMING.c_str()));
					while (iter_set_7.more()) {
						BSONObj obj = iter_set_7.next().embeddedObject();
						int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
						Time_Value tv(Time_Value::zero);
						tv.sec(obj[Time_Value_Fields::SEC].numberLong());
						tv.usec(obj[Time_Value_Fields::USEC].numberLong());

						esd.refreshed_monster_timing_.insert(std::make_pair(point_id, tv));
					}

					BSONObjIterator iter_set_8(obj.getObjectField(Expedition_Scene_Data_Fields::REFRESHED_MONSTER_KILL_GAP.c_str()));
					while (iter_set_8.more()) {
						BSONObj obj = iter_set_8.next().embeddedObject();
						int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
						Time_Value tv(Time_Value::zero);
						tv.sec(obj[Time_Value_Fields::SEC].numberLong());
						tv.usec(obj[Time_Value_Fields::USEC].numberLong());

						esd.refreshed_monster_kill_gap_.insert(std::make_pair(point_id, tv));
					}

					emd.uuid_line_scene_map_[uuid].insert(std::make_pair(esd.cur_line_, esd));
				}
			}
		} // -- uuid_line_scene_map_
		map.insert(std::make_pair(emd.inter_, emd));
	}

	return 0;
}
int DB_Operator::save_expedition_manager_data(const Expedition_Manager_Data &data) {
	std::vector<BSONObj> opened_uuid_opentime_map, gang_id_uuid_map, first_gang_rank,
			gang_rank_map, uuid_gang_rank_map, gang_id_point_id, gang_id_cur_line,
			uuid_gang_id_point_id, uuid_gang_id_cur_line,
			line_scene_map, uuid_line_scene_map, uuid_has_init;
	BSONArrayBuilder gang_rank_sync_success, gang_rank_sync_new_vec;

	for (Role_Id_Time_Map::const_iterator it = data.opened_uuid_opentime_map_.begin();
			it != data.opened_uuid_opentime_map_.end(); ++it) {
		opened_uuid_opentime_map.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->first
				<< Time_Value_Fields::SEC << (long long int)it->second.sec()
				<< Time_Value_Fields::USEC << (long long int)it->second.usec()));
	}

	for (RoleID_RoleID_Map::const_iterator it = data.gang_id_uuid_map_.begin();
			it != data.gang_id_uuid_map_.end(); ++it) {
		gang_id_uuid_map.push_back(BSON(Role_Fields::GANG_ID << (long long int)it->first
				<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->second));
	}

	for (Role_UInt_Map::const_iterator it = data.first_gang_rank_.begin();
			it != data.first_gang_rank_.end(); ++it) {
		first_gang_rank.push_back(BSON(Role_Fields::GANG_ID << (long long int)it->first
				<< Expedition_Manager_Data_Fields::RANK << it->second));
	}

	for (Role_Bool_Map::const_iterator it = data.uuid_has_init_.begin(); it != data.uuid_has_init_.end(); ++it) {
		uuid_has_init.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->first
				<< Expedition_Manager_Data_Fields::HAS_INIT << it->second));
	}

	for (Gang_ExpeditionGangRankInfo_Map::const_iterator it_grm = data.gang_rank_map_.begin();
				it_grm != data.gang_rank_map_.end(); ++it_grm) {
		gang_rank_map.push_back(BSON(Role_Fields::GANG_ID << (long long int)it_grm->first
					<< Expedition_Manager_Data_Fields::RANK << it_grm->second.rank
					<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_grm->second.uuid
					<< Role_Fields::ROLE_NAME << it_grm->second.server_name
					<< Role_Fields::GANG_NAME << it_grm->second.gang_name
					<< Role_Fields::ROLE_ID << (long long int)it_grm->second.gang_leader_id
					<< Expedition_Manager_Data_Fields::GANG_LEADER_NAME << it_grm->second.gang_leader_name
					<< Fighter_Fields::FORCE << it_grm->second.force
					<< Expedition_Manager_Data_Fields::NUM << it_grm->second.num
					<< Expedition_Manager_Data_Fields::ASSET << it_grm->second.asset
					<< Role_Fields::LEVEL << it_grm->second.level
					<< Expedition_Manager_Data_Fields::CREATE_TIME << it_grm->second.create_time
					<< Expedition_Manager_Data_Fields::EXPEDITION_RANK << it_grm->second.expedition_rank));
	}
	for (Role_GangExpeditionGangRankInfoMap_Map::const_iterator it = data.uuid_gang_rank_map_.begin();
			it != data.uuid_gang_rank_map_.end(); ++it) {
		std::vector<BSONObj> grm;
		for (Gang_ExpeditionGangRankInfo_Map::const_iterator it_grm = it->second.begin();
				it_grm != it->second.end(); ++it_grm) {
			grm.push_back(BSON(Role_Fields::GANG_ID << (long long int)it_grm->first
					<< Expedition_Manager_Data_Fields::RANK << it_grm->second.rank
					<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_grm->second.uuid
					<< Role_Fields::ROLE_NAME << it_grm->second.server_name
					<< Role_Fields::GANG_NAME << it_grm->second.gang_name
					<< Role_Fields::ROLE_ID << (long long int)it_grm->second.gang_leader_id
					<< Expedition_Manager_Data_Fields::GANG_LEADER_NAME << it_grm->second.gang_leader_name
					<< Fighter_Fields::FORCE << it_grm->second.force
					<< Expedition_Manager_Data_Fields::NUM << it_grm->second.num
					<< Expedition_Manager_Data_Fields::ASSET << it_grm->second.asset
					<< Role_Fields::LEVEL << it_grm->second.level
					<< Expedition_Manager_Data_Fields::CREATE_TIME << it_grm->second.create_time
					<< Expedition_Manager_Data_Fields::EXPEDITION_RANK << it_grm->second.expedition_rank));
		}
		uuid_gang_rank_map.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->first
				<< Expedition_Manager_Data_Fields::GANG_RANK_MAP << grm));
	}

	{
		for (Role_Hash_Set::const_iterator it = data.gang_rank_sync_success_.begin();
				it != data.gang_rank_sync_success_.end(); ++it) {
			gang_rank_sync_success.append((long long int)*it);
		}
	}
	{
		for (Role_Vec::const_iterator it = data.gang_rank_sync_new_vec_.begin();
				it != data.gang_rank_sync_new_vec_.end(); ++it) {
			gang_rank_sync_new_vec.append((long long int) *it);
		}
	}

	for (Role_UInt_Map::const_iterator it_gipi = data.gang_id_point_id_.begin();
			it_gipi != data.gang_id_point_id_.end(); ++it_gipi) {
		gang_id_point_id.push_back(BSON(Role_Fields::GANG_ID << (long long int)it_gipi->first
				<< Expedition_Manager_Data_Fields::POINT_ID << it_gipi->second));
	}
	for (Role_Int_Map::const_iterator it_gipi = data.gang_id_cur_line_.begin();
			it_gipi != data.gang_id_cur_line_.end(); ++it_gipi) {
		gang_id_cur_line.push_back(BSON(Role_Fields::GANG_ID << (long long int)it_gipi->first
				<< Expedition_Manager_Data_Fields::CUR_LINE << it_gipi->second));
	}
	for (Role_RoleUIntMap_Map::const_iterator it = data.uuid_gang_id_point_id_.begin();
			it != data.uuid_gang_id_point_id_.end(); ++it) {
		std::vector<BSONObj> gicl;
		for (Role_UInt_Map::const_iterator it_gipi = it->second.begin();
				it_gipi != it->second.end(); ++it_gipi) {
			gicl.push_back(BSON(Role_Fields::GANG_ID << (long long int)it_gipi->first
					<< Expedition_Manager_Data_Fields::POINT_ID << it_gipi->second));
		}
		uuid_gang_id_point_id.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->first
				<< Expedition_Manager_Data_Fields::GANG_ID_POINT_ID << gicl));
	}

	for (Role_RoleIntMap_Map::const_iterator it = data.uuid_gang_id_cur_line_.begin();
			it != data.uuid_gang_id_cur_line_.end(); ++it) {
		std::vector<BSONObj> gicl;
		for (Role_Int_Map::const_iterator it_gipi = it->second.begin();
				it_gipi != it->second.end(); ++it_gipi) {
			gicl.push_back(BSON(Role_Fields::GANG_ID << (long long int)it_gipi->first
					<< Expedition_Manager_Data_Fields::CUR_LINE << it_gipi->second));
		}
		uuid_gang_id_cur_line.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it->first
				<< Expedition_Manager_Data_Fields::GANG_ID_CUR_LINE << gicl));
	}

	for (Line_ExpeditionSceneData_Map::const_iterator it = data.line_scene_map_.begin();
			it != data.line_scene_map_.end(); ++it) {
		std::vector<BSONObj> pgi, mp, dr, occupy_castle_rank, refreshed_monster,
		refreshed_monster_timing,refreshed_monster_kill_gap;
		for (Int_Uid::const_iterator it_pgi = it->second.point_gang_id_.begin();
				it_pgi != it->second.point_gang_id_.end(); ++it_pgi) {
			pgi.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_pgi->first
					<< Role_Fields::GANG_ID << (long long int)it_pgi->second));
		}
		for (Stronghold_Map::const_iterator it_mp = it->second.map_data_.begin();
				it_mp != it->second.map_data_.end(); ++it_mp) {
			std::vector<BSONObj> sm;
			for (Role_Int_Map::const_iterator it_sm = it_mp->second.info.defend_id_map.begin();
					it_sm != it_mp->second.info.defend_id_map.end(); ++it_sm) {
				sm.push_back(BSON(Role_Fields::ROLE_ID << (long long int) it_sm->first
						<< Stronghold_Info_Fields::NUMS << it_sm->second));
			}
			mp.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_mp->first
					<< Stronghold_Fields::TYPE << it_mp->second.type
					<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_mp->second.info.occupier.server_uuid
					<< Role_Fields::ROLE_NAME << it_mp->second.info.occupier.server_name
					<< Role_Fields::GANG_ID << (long long int)it_mp->second.info.occupier.gang_id
					<< Role_Fields::GANG_NAME << it_mp->second.info.occupier.gang_name
					<< Stronghold_Info_Fields::SOLDIER_MAP << sm));
		}

		{
			for (Role_ExpeditionSceneDemageRankData_Map::const_iterator it_dr = it->second.demage_rank_.begin();
					it_dr != it->second.demage_rank_.end(); ++it_dr) {
				dr.push_back(BSON(Role_Fields::ROLE_ID << (long long int)it_dr->first
						<< Role_Fields::ROLE_NAME << it_dr->second.role_name
						<< Role_Fields::GANG_ID << (long long int)it_dr->second.gang_id
						<< Role_Fields::GANG_NAME << it_dr->second.gang_name
						<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_dr->second.uuid
						<< Expedition_Scene_Data_Fields::SERVER_NAME << it_dr->second.server_name
						<< Fighter_Fields::FORCE << it_dr->second.force
						<< Fighter_Fields::LEVEL << it_dr->second.level
						<< Expedition_Scene_Data_Fields::DEMAGE << it_dr->second.demage));
			}
		}

		for (Role_ExpeditionOccupyCastleData_Map::const_iterator it_dr = it->second.occupy_castle_rank_.begin();
				it_dr != it->second.occupy_castle_rank_.end(); ++it_dr) {
			occupy_castle_rank.push_back(BSON(Role_Fields::ROLE_ID << (long long int)it_dr->first
					<< Role_Fields::ROLE_NAME << it_dr->second.role_name
					<< Role_Fields::GANG_ID << (long long int)it_dr->second.gang_id
					<< Role_Fields::GANG_NAME << it_dr->second.gang_name
					<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_dr->second.server_uuid
					<< Expedition_Scene_Data_Fields::SERVER_NAME << it_dr->second.server_name
					<< Fighter_Fields::FORCE << it_dr->second.force
					<< Fighter_Fields::LEVEL << it_dr->second.level
					<< Expedition_Scene_Data_Fields::DEMAGE << it_dr->second.nums
					<< Expedition_Scene_Data_Fields::GET_NUMS << it_dr->second.get_nums));
		}

		{
			for (Point_ExpeditionDropOrMaterial::const_iterator it_p = it->second.refreshed_drop_or_material_.begin();
					it_p != it->second.refreshed_drop_or_material_.end(); ++it_p) {
				BSONArrayBuilder had_collected;
				for (Role_Hash_Set::const_iterator it_hc = it_p->second.had_collected.begin();
						it_hc != it_p->second.had_collected.end(); ++it_hc) {
					had_collected.append((long long int)*it_hc);
				}
				refreshed_monster.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_p->first
						<< Expedition_Scene_Data_Fields::HAD_COLLECTED << had_collected.arr()
						<< Time_Value_Fields::SEC << (long long int)it_p->second.birth_time.sec()
						<< Time_Value_Fields::USEC << (long long int)it_p->second.birth_time.usec()));
			}
		}

		{
			for (Int_Time_Map::const_iterator it_mt = it->second.refreshed_monster_timing_.begin();
					it_mt != it->second.refreshed_monster_timing_.end(); ++it_mt) {
				refreshed_monster_timing.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_mt->first
						<< Time_Value_Fields::SEC << (long long int)it_mt->second.sec()
						<< Time_Value_Fields::USEC << (long long int)it_mt->second.usec()));
			}
		}

		{
			for (Int_Time_Map::const_iterator it_mt = it->second.refreshed_monster_kill_gap_.begin();
					it_mt != it->second.refreshed_monster_kill_gap_.end(); ++it_mt) {
				refreshed_monster_kill_gap.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_mt->first
						<< Time_Value_Fields::SEC << (long long int)it_mt->second.sec()
						<< Time_Value_Fields::USEC << (long long int)it_mt->second.usec()));
			}
		}

		line_scene_map.push_back(BSON(Expedition_Manager_Data_Fields::CUR_LINE << it->first
				<< Expedition_Scene_Data_Fields::MAP_BASE_POINT_NUM << it->second.map_base_point_num_
				<< Expedition_Scene_Data_Fields::POINT_GANG_ID << pgi
				<< Expedition_Scene_Data_Fields::MAP_DATA << mp
				<< Expedition_Scene_Data_Fields::DEMAGE_RANK << dr
				<< Time_Value_Fields::SEC << (long long int)it->second.award_time_.sec()
				<< Time_Value_Fields::USEC << (long long int)it->second.award_time_.usec()
				<< Expedition_Scene_Data_Fields::SETTLE_TIME << it->second.settle_time_
				<< Expedition_Scene_Data_Fields::OCCUPY_CASTLE_RANK << occupy_castle_rank
				<< Expedition_Scene_Data_Fields::DROP_OR_MATERIAL << refreshed_monster
				<< Expedition_Scene_Data_Fields::REFRESHED_MONSTER_TIMING << refreshed_monster_timing
				<< Expedition_Scene_Data_Fields::REFRESHED_MONSTER_KILL_GAP << refreshed_monster_kill_gap));
	}

	for (UUID_LineExpeditionSceneData_Map_Map::const_iterator it_ulsm = data.uuid_line_scene_map_.begin();
			it_ulsm != data.uuid_line_scene_map_.end(); ++it_ulsm) {
		std::vector<BSONObj> lsm;
		for (Line_ExpeditionSceneData_Map::const_iterator it = it_ulsm->second.begin();
				it != it_ulsm->second.end(); ++it) {
			std::vector<BSONObj> pgi, mp, dr, occupy_castle_rank, refreshed_monster,
			refreshed_monster_timing,refreshed_monster_kill_gap;
			for (Int_Uid::const_iterator it_pgi = it->second.point_gang_id_.begin();
					it_pgi != it->second.point_gang_id_.end(); ++it_pgi) {
				pgi.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_pgi->first
						<< Role_Fields::GANG_ID << (long long int)it_pgi->second));
			}
			for (Stronghold_Map::const_iterator it_mp = it->second.map_data_.begin();
					it_mp != it->second.map_data_.end(); ++it_mp) {
				std::vector<BSONObj> sm;
				for (Role_Int_Map::const_iterator it_sm = it_mp->second.info.defend_id_map.begin();
						it_sm != it_mp->second.info.defend_id_map.end(); ++it_sm) {
					sm.push_back(BSON(Role_Fields::ROLE_ID << (long long int) it_sm->first
							<< Stronghold_Info_Fields::NUMS << it_sm->second));
				}
				mp.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_mp->first
						<< Stronghold_Fields::TYPE << it_mp->second.type
						<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_mp->second.info.occupier.server_uuid
						<< Role_Fields::ROLE_NAME << it_mp->second.info.occupier.server_name
						<< Role_Fields::GANG_ID << (long long int)it_mp->second.info.occupier.gang_id
						<< Role_Fields::GANG_NAME << it_mp->second.info.occupier.gang_name
						<< Stronghold_Info_Fields::SOLDIER_MAP << sm));
			}

			for (Role_ExpeditionSceneDemageRankData_Map::const_iterator it_dr = it->second.demage_rank_.begin();
					it_dr != it->second.demage_rank_.end(); ++it_dr) {
				dr.push_back(BSON(Role_Fields::ROLE_ID << (long long int)it_dr->first
						<< Role_Fields::ROLE_NAME << it_dr->second.role_name
						<< Role_Fields::GANG_ID << (long long int)it_dr->second.gang_id
						<< Role_Fields::GANG_NAME << it_dr->second.gang_name
						<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_dr->second.uuid
						<< Expedition_Scene_Data_Fields::SERVER_NAME << it_dr->second.server_name
						<< Fighter_Fields::FORCE << it_dr->second.force
						<< Expedition_Scene_Data_Fields::DEMAGE << it_dr->second.demage));
			}

			for (Role_ExpeditionOccupyCastleData_Map::const_iterator it_dr = it->second.occupy_castle_rank_.begin();
					it_dr != it->second.occupy_castle_rank_.end(); ++it_dr) {
				occupy_castle_rank.push_back(BSON(Role_Fields::ROLE_ID << (long long int)it_dr->first
						<< Role_Fields::ROLE_NAME << it_dr->second.role_name
						<< Role_Fields::GANG_ID << (long long int)it_dr->second.gang_id
						<< Role_Fields::GANG_NAME << it_dr->second.gang_name
						<< Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_dr->second.server_uuid
						<< Expedition_Scene_Data_Fields::SERVER_NAME << it_dr->second.server_name
						<< Fighter_Fields::FORCE << it_dr->second.force
						<< Fighter_Fields::LEVEL << it_dr->second.level
						<< Expedition_Scene_Data_Fields::DEMAGE << it_dr->second.nums
						<< Expedition_Scene_Data_Fields::GET_NUMS << it_dr->second.get_nums));
			}

			for (Point_ExpeditionDropOrMaterial::const_iterator it_p = it->second.refreshed_drop_or_material_.begin();
					it_p != it->second.refreshed_drop_or_material_.end(); ++it_p) {
				BSONArrayBuilder had_collected;
				for (Role_Hash_Set::const_iterator it_hc = it_p->second.had_collected.begin();
						it_hc != it_p->second.had_collected.end(); ++it_hc) {
					had_collected.append((long long int)*it_hc);
				}
				refreshed_monster.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_p->first
						<< Expedition_Scene_Data_Fields::HAD_COLLECTED << had_collected.arr()
						<< Time_Value_Fields::SEC << (long long int)it_p->second.birth_time.sec()
						<< Time_Value_Fields::USEC << (long long int)it_p->second.birth_time.usec()));
			}

			{
				for (Int_Time_Map::const_iterator it_mt = it->second.refreshed_monster_timing_.begin();
						it_mt != it->second.refreshed_monster_timing_.end(); ++it_mt) {
					refreshed_monster_timing.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_mt->first
							<< Time_Value_Fields::SEC << (long long int)it_mt->second.sec()
							<< Time_Value_Fields::USEC << (long long int)it_mt->second.usec()));
				}
			}

			{
				for (Int_Time_Map::const_iterator it_mt = it->second.refreshed_monster_kill_gap_.begin();
						it_mt != it->second.refreshed_monster_kill_gap_.end(); ++it_mt) {
					refreshed_monster_kill_gap.push_back(BSON(Expedition_Manager_Data_Fields::POINT_ID << it_mt->first
							<< Time_Value_Fields::SEC << (long long int)it_mt->second.sec()
							<< Time_Value_Fields::USEC << (long long int)it_mt->second.usec()));
				}
			}

			lsm.push_back(BSON(Expedition_Manager_Data_Fields::CUR_LINE << it->first
					<< Expedition_Scene_Data_Fields::MAP_BASE_POINT_NUM << it->second.map_base_point_num_
					<< Expedition_Scene_Data_Fields::POINT_GANG_ID << pgi
					<< Expedition_Scene_Data_Fields::MAP_DATA << mp
					<< Expedition_Scene_Data_Fields::DEMAGE_RANK << dr
					<< Time_Value_Fields::SEC << (long long int)it->second.award_time_.sec()
					<< Time_Value_Fields::USEC << (long long int)it->second.award_time_.usec()
					<< Expedition_Scene_Data_Fields::SETTLE_TIME << it->second.settle_time_
					<< Expedition_Scene_Data_Fields::OCCUPY_CASTLE_RANK << occupy_castle_rank
					<< Expedition_Scene_Data_Fields::DROP_OR_MATERIAL << refreshed_monster
					<< Expedition_Scene_Data_Fields::REFRESHED_MONSTER_TIMING << refreshed_monster_timing
					<< Expedition_Scene_Data_Fields::REFRESHED_MONSTER_KILL_GAP << refreshed_monster_kill_gap));
		}

		uuid_line_scene_map.push_back(BSON(Expedition_Global_Manager_Data_Fields::UUID << (long long int)it_ulsm->first
				<< Expedition_Manager_Data_Fields::LINE_SCENE_MAP << lsm));
	}

	BSONObjBuilder builder;
	builder << Expedition_Manager_Data_Fields::INTER << data.inter_
			<< Expedition_Manager_Data_Fields::LAST_RESET_TIME_SEC << (long long int) data.last_reset_time_.sec()
			<< Expedition_Manager_Data_Fields::LAST_RESET_TIME_USEC << (long long int) data.last_reset_time_.usec()
			<< Expedition_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP << opened_uuid_opentime_map
			<< Expedition_Manager_Data_Fields::GANG_ID_UUID_MAP << gang_id_uuid_map
			<< Expedition_Manager_Data_Fields::FIRST_GANG_RANK << first_gang_rank
			<< Expedition_Manager_Data_Fields::HAS_INIT << data.has_init_
			<< Expedition_Manager_Data_Fields::UUID_HAS_INIT << uuid_has_init
			<< Expedition_Manager_Data_Fields::GANG_RANK_MAP << gang_rank_map
			<< Expedition_Manager_Data_Fields::UUID_GANG_RANK_MAP << uuid_gang_rank_map
			<< Expedition_Manager_Data_Fields::GANG_RANK_SYNC_SUCCESS << gang_rank_sync_success.arr()
			<< Expedition_Manager_Data_Fields::GANG_RANK_SYNC_NEW_VEC << gang_rank_sync_new_vec.arr()
			<< Expedition_Manager_Data_Fields::GANG_ID_POINT_ID << gang_id_point_id
			<< Expedition_Manager_Data_Fields::GANG_ID_CUR_LINE << gang_id_cur_line
			<< Expedition_Manager_Data_Fields::UUID_GANG_ID_POINT_ID << uuid_gang_id_point_id
			<< Expedition_Manager_Data_Fields::UUID_GANG_ID_CUR_LINE << uuid_gang_id_cur_line
			<< Expedition_Manager_Data_Fields::LINE_SCENE_MAP << line_scene_map
			<< Expedition_Manager_Data_Fields::UUID_LINE_SCENE_MAP << uuid_line_scene_map;

	MONGO_CONNECTION.update(Expedition_Manager_Data_Fields::COLLECTION, QUERY(Expedition_Manager_Data_Fields::INTER << data.inter_),
			BSON("$set" << builder.obj()), true);
//	{
//		Time_Value now = Time_Value::gettimeofday();
//		Date_Time dt(now);
//		dt.day(1);
//		dt.hour(0);
//		dt.microsec(0);
//		dt.minute(0);
//		dt.second(0);
//		now.sec(dt.time_sec());
//
//		BSONObjBuilder builder;
//		builder << Expedition_Global_Manager_Data_Fields::OPENED_UUID_OPENTIME_MAP << opened_uuid_opentime_map
//				<< Expedition_Global_Manager_Data_Fields::UUID_INTER_MAP << uuid_inter_map
//				<< Expedition_Global_Manager_Data_Fields::OPENTIME << opentime
//				<< Expedition_Manager_Detail_Fields::FLAG_TIME << (long long int)now.sec();
//		MONGO_CONNECTION.update(Expedition_Global_Manager_Data_Fields::BAK_COLLECTION, QUERY(Expedition_Manager_Detail_Fields::FLAG_TIME << (long long int)now.sec()),
//				BSON("$set" << builder.obj()), true);
//	}
	return 0;
}

int DB_Operator::save_oper_activity_detail(const Oper_Activity_Detail &detail) {
	BSONObjBuilder builder;

	vector<BSONObj> last_item_start_time;
	for ( UInt_Time_Map::const_iterator iter = detail.last_item_start_time.begin();iter != detail.last_item_start_time.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Oper_Activity_Data_Fields::Common::KEY << iter->first;
		tmp_builder << Oper_Activity_Data_Fields::Common::VAL << (long long int)iter->second.sec();
		last_item_start_time.push_back(tmp_builder.obj());
	}

	vector<BSONObj> actid_login_day;
	for ( UInt_UInt_Map::const_iterator iter = detail.actid_login_day.begin();iter != detail.actid_login_day.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Oper_Activity_Data_Fields::Common::KEY << iter->first;
		tmp_builder << Oper_Activity_Data_Fields::Common::VAL << iter->second;
		actid_login_day.push_back(tmp_builder.obj());
	}

	vector<BSONObj> actid_type_record_val;
	for ( UInt_UInt_Oper_Activity_Record_Val_Map::const_iterator iter = detail.actid_type_record_val.begin();iter != detail.actid_type_record_val.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Oper_Activity_Data_Fields::Common::KEY << iter->first;
		vector<BSONObj> val1;
		for(UInt_Oper_Activity_Record_Val_Map::const_iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); ++iter1) {
			BSONObjBuilder b1;
				//struct
				BSONObjBuilder bval;
				bval << Oper_Activity_Data_Fields::Record::NUM << iter1->second.num
						<< Oper_Activity_Data_Fields::Record::EXT1 << iter1->second.ext1
						<< Oper_Activity_Data_Fields::Record::EXT2 << iter1->second.ext2
						<< Oper_Activity_Data_Fields::Record::EXT3 << iter1->second.ext3;
				vector<BSONObj> val2;
				for(UInt_UInt_Map::const_iterator iter2 = iter1->second.type_num.begin(); iter2 != iter1->second.type_num.end(); ++iter2) {
					val2.push_back(BSON(
							Oper_Activity_Data_Fields::Common::KEY << iter2->first <<
							Oper_Activity_Data_Fields::Common::VAL << iter2->second
					));
				}
				bval << Oper_Activity_Data_Fields::Record::TYPE_NUM << val2;
				//end struct
			b1 << Oper_Activity_Data_Fields::Common::KEY << iter1->first;
			b1 << Oper_Activity_Data_Fields::Common::VAL << bval.obj();
			val1.push_back(b1.obj());
		}
		tmp_builder << Oper_Activity_Data_Fields::Common::VAL << val1;
		actid_type_record_val.push_back(tmp_builder.obj());
	}

	vector<BSONObj> actid_receive_id;
	for ( UInt_UInt_Set_Map::const_iterator iter = detail.actid_receive_id.begin();iter != detail.actid_receive_id.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Oper_Activity_Data_Fields::Common::KEY << iter->first;
		tmp_builder << Oper_Activity_Data_Fields::Common::VAL << iter->second;
		actid_receive_id.push_back(tmp_builder.obj());
	}

	vector<BSONObj> actid_receive_id_num;
	for ( UInt_UIntUIntMap_Map::const_iterator iter = detail.actid_receive_id_num.begin();iter != detail.actid_receive_id_num.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Oper_Activity_Data_Fields::Common::KEY << iter->first;
		vector<BSONObj> val2;
		for(UInt_UInt_Map::const_iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2) {
			val2.push_back(BSON(
					Oper_Activity_Data_Fields::Common::KEY << iter2->first <<
					Oper_Activity_Data_Fields::Common::VAL << iter2->second
			));
		}
		tmp_builder << Oper_Activity_Data_Fields::Common::VAL << val2;
		actid_receive_id_num.push_back(tmp_builder.obj());
	}

	vector<BSONObj> actid_append_effect;
	for ( UInt_UInt_Set_Map::const_iterator iter = detail.actid_append_effect.begin();iter != detail.actid_append_effect.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Oper_Activity_Data_Fields::Common::KEY << iter->first;
		tmp_builder << Oper_Activity_Data_Fields::Common::VAL << iter->second;
		actid_append_effect.push_back(tmp_builder.obj());
	}

	vector<BSONObj> actid_recid_last_refresh_time;
	for ( UInt_UInt_Time_Map::const_iterator iter = detail.actid_recid_last_refresh_time.begin();iter != detail.actid_recid_last_refresh_time.end();++iter) {
		BSONObjBuilder tmp_builder;
		tmp_builder << Oper_Activity_Data_Fields::Common::KEY << iter->first;
		vector<BSONObj> second;
		for ( UInt_Time_Map::const_iterator iter_second = iter->second.begin();iter_second != iter->second.end();++iter_second) {
			BSONObjBuilder builder_second;
			builder_second << Oper_Activity_Data_Fields::Common::KEY << iter_second->first;
			builder_second << Oper_Activity_Data_Fields::Common::VAL << (long long int)iter_second->second.sec();
			second.push_back(builder_second.obj());
		}
		tmp_builder << Oper_Activity_Data_Fields::Common::VAL << second;
		actid_recid_last_refresh_time.push_back(tmp_builder.obj());
	}

	builder
	<< Oper_Activity_Data_Fields::LAST_ITEM_START_TIME << last_item_start_time//每一项活动ID对应最后一次开始时间
	<< Oper_Activity_Data_Fields::ACTID_TYPE_RECORD_VAL<< actid_type_record_val//活动ID->类型->完成记录(次数,类型->值)
	<< Oper_Activity_Data_Fields::ACTID_RECEIVE_ID<< actid_receive_id//活动ID对应已经领取的ID集合
	<< Oper_Activity_Data_Fields::ACTID_RECEIVE_ID_NUM<< actid_receive_id_num//活动ID->(项目)领取ID->领取次数
	<< Oper_Activity_Data_Fields::ACTID_APPEND_EFFECT<< actid_append_effect//活动ID执行过的附加效果
	<< Oper_Activity_Data_Fields::ACTID_RECID_LAST_REFRESH_TIME<< actid_recid_last_refresh_time//活动ID->(项目)领取ID->最后一次刷新时间
	<< Oper_Activity_Data_Fields::ACTID_LOGIN_DAY<< actid_login_day//活动ID->login day时间
	;

	MONGO_CONNECTION.update(Oper_Activity_Data_Fields::COLLECTION,
			QUERY(Role_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_oper_activity_detail(Oper_Activity_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Oper_Activity_Data_Fields::COLLECTION,QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}
	{
		BSONObj list = res.getObjectField(Oper_Activity_Data_Fields::LAST_ITEM_START_TIME.c_str());
		BSONObjIterator iter(list);
		detail.last_item_start_time.clear();
		while(iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint32_t key = obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();
			long long int val = obj[Oper_Activity_Data_Fields::Common::VAL].numberLong();
			detail.last_item_start_time[key].sec(val);
		}
	}
	{
		BSONObj list = res.getObjectField(Oper_Activity_Data_Fields::ACTID_LOGIN_DAY.c_str());
		BSONObjIterator iter(list);
		detail.actid_login_day.clear();
		while(iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint32_t key = obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();
			uint32_t val = obj[Oper_Activity_Data_Fields::Common::VAL].numberInt();
			detail.actid_login_day[key] = val;
		}
	}
	{
		BSONObj list = res.getObjectField(Oper_Activity_Data_Fields::ACTID_TYPE_RECORD_VAL.c_str());
		BSONObjIterator iter(list);
		detail.actid_type_record_val.clear();
		while(iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint32_t key = obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();

			BSONObj tlist = obj.getObjectField(Oper_Activity_Data_Fields::Common::VAL.c_str());
			BSONObjIterator titer(tlist);
			while(titer.more()) {
				BSONObj tobj = titer.next().embeddedObject();
				uint32_t type = tobj[Oper_Activity_Data_Fields::Common::KEY].numberInt();
				uint32_t num = tobj[Oper_Activity_Data_Fields::Common::VAL][Oper_Activity_Data_Fields::Record::NUM].numberInt();
				detail.actid_type_record_val[key][type].num = num;
				detail.actid_type_record_val[key][type].ext1 = tobj[Oper_Activity_Data_Fields::Common::VAL][Oper_Activity_Data_Fields::Record::EXT1].numberInt();
				detail.actid_type_record_val[key][type].ext2 = tobj[Oper_Activity_Data_Fields::Common::VAL][Oper_Activity_Data_Fields::Record::EXT2].numberInt();
				detail.actid_type_record_val[key][type].ext3 = tobj[Oper_Activity_Data_Fields::Common::VAL][Oper_Activity_Data_Fields::Record::EXT3].numberInt();

				BSONObj tnlist = tobj[Oper_Activity_Data_Fields::Common::VAL].Obj().getObjectField(Oper_Activity_Data_Fields::Record::TYPE_NUM.c_str());
				BSONObjIterator tniter(tnlist);
				while(tniter.more()) {
					BSONObj tnobj = tniter.next().embeddedObject();
					uint32_t key = tnobj[Oper_Activity_Data_Fields::Common::KEY].numberInt();
					uint32_t val = tnobj[Oper_Activity_Data_Fields::Common::VAL].numberInt();
					detail.actid_type_record_val[key][type].type_num[key] = val;
				}
			}
		}
	}
	{
		BSONObj list = res.getObjectField(Oper_Activity_Data_Fields::ACTID_RECEIVE_ID.c_str());
		BSONObjIterator iter(list);
		detail.actid_receive_id.clear();
		while(iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint32_t key = obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();

			BSONObjIterator iter_set(obj.getObjectField(Oper_Activity_Data_Fields::Common::VAL.c_str()));
			while (iter_set.more()) {
				uint32_t rec_id = iter_set.next().numberInt();
				detail.actid_receive_id[key].insert(rec_id);
			}
		}
	}
	{
		BSONObj list = res.getObjectField(Oper_Activity_Data_Fields::ACTID_APPEND_EFFECT.c_str());
		BSONObjIterator iter(list);
		detail.actid_append_effect.clear();
		while(iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint32_t key = obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();

			BSONObjIterator iter_set(obj.getObjectField(Oper_Activity_Data_Fields::Common::VAL.c_str()));
			while (iter_set.more()) {
				uint32_t rec_id = iter_set.next().numberInt();
				detail.actid_append_effect[key].insert(rec_id);
			}
		}
	}
	{
		BSONObj list = res.getObjectField(Oper_Activity_Data_Fields::ACTID_RECEIVE_ID_NUM.c_str());
		BSONObjIterator iter(list);
		detail.actid_receive_id_num.clear();
		while(iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint32_t key = obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();

			BSONObjIterator iter_set(obj.getObjectField(Oper_Activity_Data_Fields::Common::VAL.c_str()));
			while (iter_set.more()) {
				BSONObj rec_obj = iter_set.next().embeddedObject();
				uint32_t rec_id = rec_obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();
				uint32_t num = rec_obj[Oper_Activity_Data_Fields::Common::VAL].numberInt();
				detail.actid_receive_id_num[key][rec_id] = num;
			}
		}
	}
	{
		BSONObj list = res.getObjectField(Oper_Activity_Data_Fields::ACTID_RECID_LAST_REFRESH_TIME.c_str());
		BSONObjIterator iter(list);
		detail.actid_recid_last_refresh_time.clear();
		while(iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			uint32_t key = obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();

			BSONObjIterator iter_set(obj.getObjectField(Oper_Activity_Data_Fields::Common::VAL.c_str()));
			while (iter_set.more()) {
				BSONObj rec_obj = iter_set.next().embeddedObject();
				uint32_t rec_id = rec_obj[Oper_Activity_Data_Fields::Common::KEY].numberInt();
				long long int time = rec_obj[Oper_Activity_Data_Fields::Common::VAL].numberLong();
				detail.actid_recid_last_refresh_time[key][rec_id].sec(time);
			}
		}
	}
	return 0;
}

int DB_Operator::save_pet_data(const Pet_Detail &detail) {

	vector<BSONObj> pet_skill_vec;
	for (Int_Int_Map::const_iterator it = detail.skill_map.begin(); it != detail.skill_map.end(); ++it) {
		pet_skill_vec.push_back(
				BSON(Pet_Fields::Pet_Skill::ID << it->first << Pet_Fields::Pet_Skill::LOC << it->second));
	}

	BSONObjBuilder builder;
	builder << Pet_Fields::ROLE_ID << (long long int)detail.role_id
			<< Pet_Fields::LV << detail.lv
			<< Pet_Fields::STATE << detail.state
			<< Pet_Fields::UPGRADE_VAL << detail.upgrade_val
			<< Pet_Fields::ACTIVE_ILLUSION_ID << detail.active_illusion_id
			<< Pet_Fields::SKILLS << pet_skill_vec
			<< Pet_Fields::ILLUSIONS << detail.illusion_vec;

	MONGO_CONNECTION.update(Pet_Fields::COLLECTION,
			QUERY(Pet_Fields::ROLE_ID << (long long int)(detail.role_id)),
			BSON("$set" << builder.obj()), true);

	return 0;
}
int DB_Operator::load_pet_data(Pet_Detail &detail) {

	BSONObj res = MONGO_CONNECTION.findOne(Pet_Fields::COLLECTION,
			QUERY(Pet_Fields::ROLE_ID <<(long long int)detail.role_id));
	if (res.isEmpty()) return -1;

	detail.lv = res[Pet_Fields::LV].numberInt();
	detail.state = res[Pet_Fields::STATE].numberInt();
	detail.upgrade_val = res[Pet_Fields::UPGRADE_VAL].numberInt();
	detail.active_illusion_id = res[Pet_Fields::ACTIVE_ILLUSION_ID].numberInt();

	const BSONObj &skills_obj = res.getObjectField(Pet_Fields::SKILLS.c_str());
	BSONObjIterator skills_iter(skills_obj);
	while (skills_iter.more()) {
		const BSONObj &skill_obj = skills_iter.next().embeddedObject();
		int id = skill_obj[Pet_Fields::Pet_Skill::ID].numberInt();
		int loc = skill_obj[Pet_Fields::Pet_Skill::LOC].numberInt();
		detail.skill_map[id] = loc;
	}

	const BSONObj &illusions_obj = res.getObjectField(Pet_Fields::ILLUSIONS.c_str());
	BSONObjIterator illusions_iter(illusions_obj);
	while(illusions_iter.more()) {
		detail.illusion_vec.push_back(illusions_iter.next().numberInt());
	}

	return 0;
}
