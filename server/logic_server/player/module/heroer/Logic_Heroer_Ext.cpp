/*
 * Logic_Heroer_Ext.cpp
 *
 *  Created on: 2014年11月13日
 *      Author: Linqiyou
 */

#include "Logic_Player.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Hero.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Role.h"
#include "pack/Packer.h"
#include "pack/Pack_Func.h"
#include "equiper/Equiper_Def.h"
#include "equiper/Equiper.h"
#include "equiper/Equiper_Struct.h"
#include "equiper/Equiper_Func.h"
#include "Logic_Heroer_Def.h"
#include "Logic_Heroer.h"
#include "Logic_Heroer_Struct.h"
#include "Logic_Heroer_Func.h"
#include "Msg_Packer_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Hero_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Fighter_Detail.h"
#include "NPC/hero/Hero_Struct.h"
#include "task/Task_Struct.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Equip.h"
#include "Msg_Rune_Stone_Struct.h"
#include "rune_stone/Rune_Stone_Def.h"
#include "Config_Cache_Gang.h"
#include "gang/Gang_Struct.h"
#include "rune/Rune_Struct.h"
#include "public/Public_Def.h"
#include "public/Public_Struct.h"
#include <set>
#include "Config_Cache_Buff.h"
#include "status/Logic_Status_Struct.h"
#include "status/Status_Func.h"

int Logic_Heroer::hero_status_load_init(void) {
	for (Logic_Status_Detail::Status_Map::iterator status_it = heroer_detail_->logic_status_detail_.status_map.begin();
			status_it != heroer_detail_->logic_status_detail_.status_map.end(); ++status_it) {
		if (is_timepri_remove(status_it->first)) {
			insert_status_and_sort(&(status_it->second), heroer_detail_->out_status_timepri_list);
		}

		for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_it->second.status_info_map.begin();
				status_info_map_it != status_it->second.status_info_map.end(); ++status_info_map_it) {
			status_info_map_it->second.overlay = status_it->second.overlay_;
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				(*it).overlay = status_it->second.overlay_;
				if (((*it).heat_type.count(EFFECT_TYPE_TIME) && (*it).delay_time_tick > Time_Value::zero && Time_Value::zero == (*it).interval)
									|| (*it).delay_action_tick > 0 || (*it).delay_passive_tick > 0 || (*it).delay_time_tick > Time_Value::zero) {
				} else {
					hero_insert_status_effect_handle(status_info_map_it->second, (*it));
				}
			}
		}

		if (!status_it->second.buff_scene.empty()) {
			heroer_detail_->buff_scene_status.insert(status_it->second.status_id_);
		}
	}
	return 0;
}
//int Logic_Heroer::hero_insert_status(Block_Buffer &buf) {
//	return 0;
//}
int Logic_Heroer::handle_hero_status_info_from_scene(Block_Buffer &buf) {
	int8_t type = 0;
	buf.read_int8(type);
	uint16_t len = 0;
	buf.read_uint16(len);
	if (type == 0) { //del
		int tmp = 0;
		for (uint16_t i = 0; i < len; ++i) {
			buf.read_int32(tmp);
			hero_erase_status_type(tmp, true);
		}
	} else { //add
		Status_Effect_Argv argv;
		for (uint16_t i = 0; i < len; ++i) {
			argv.reset();
			argv.deserialize(buf);
			hero_insert_status(argv, true);
		}
	}
//	if (len > 0) {
//		heroer_prop_refresh(player_self()->heroer_detail().fighter_index, false, true);
//	}
	return 0;
}
int Logic_Heroer::hero_insert_status(Status_Effect_Argv &argv, const bool from_scene) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(argv.status_id);
	if (NULL == status_config) {
		MSG_USER_TRACE("status id %d configure file error.", argv.status_id);
		return ERROR_CONFIG_NOT_EXIST;
	}

	argv.dispath();

	if (argv.data.size() != status_config->effect_map.size()
			|| !argv.check_last_time()) {
		MSG_USER("ERROR status_id:%d; argv.data.size: status_config.effect_map.size = %d : %d;", argv.status_id, argv.data.size(), status_config->effect_map.size());
		return 0;
	}

	attribute_set_prop(argv.data);

//	int ret = this->check_param(argv.status_id);  // 检测是否可以插入(霸体)
//	if (0 != ret) {
//		return ret;
//	}

	Status_Info detail;
	detail.status_id_ = argv.status_id;
	detail.last_caster_master_id_ = 0;
	detail.last_caster_role_id_ = argv.caster_id;
	detail.last_status_from_ = argv.status_from;
	detail.cfg_action_times = argv.cfg_action_gap;
	detail.cfg_trigger_times = argv.cfg_trigger_gap;
	detail.last_time_ms = argv.last_time_ms;
	detail.last_start_time = Time_Value::gettimeofday();
	detail.last_end_time = detail.last_start_time + Time_Value(argv.last_time_ms / 1000, (argv.last_time_ms % 1000) * 1000);
	detail.is_fight_buff = status_config->is_fight_buff;//argv.is_out_buff;
	detail.buff_scene.assign(status_config->buff_scene.begin(), status_config->buff_scene.end());
	detail.scene_visible.assign(status_config->scene_visible.begin(), status_config->scene_visible.end());

	Status_Info_Detail status_info_detail;
	status_info_detail.caster_master_id = detail.last_caster_master_id_;
	status_info_detail.caster_role_id = detail.last_caster_role_id_;
	status_info_detail.status_from = detail.last_status_from_;
	status_info_detail.status_id = detail.status_id_;
	Status_Effect_Detail status_effect_detail;
	size_t i = 0;
	for (Buff_Effect_Vec::const_iterator it = status_config->effect_map.begin();
				it != status_config->effect_map.end(); ++it) {
		status_effect_detail.reset();
		status_effect_detail.effect_id = (*it).buff_effect_id;
		status_effect_detail.effect_type = (*it).buff_effect_type;
		status_effect_detail.i_effect_value1 = argv.data[i].i_effect_val1;
		status_effect_detail.i_effect_value2 = argv.data[i].i_effect_val2;
		status_effect_detail.i_effect_value3 = argv.data[i].i_effect_val3;
		status_effect_detail.d_effect_value1 = argv.data[i].d_effect_val1;
		status_effect_detail.d_effect_value2 = argv.data[i].d_effect_val2;
		status_effect_detail.d_effect_value3 = argv.data[i].d_effect_val3;
		status_effect_detail.d_effect_value4 = argv.data[i].d_effect_val4;
		status_effect_detail.d_effect_value5 = argv.data[i].d_effect_val5;
		status_effect_detail.d_effect_value6 = argv.data[i].d_effect_val6;
		status_effect_detail.d_effect_value7 = argv.data[i].d_effect_val7;
		status_effect_detail.d_effect_value8 = argv.data[i].d_effect_val8;
		status_effect_detail.d_effect_value9 = argv.data[i].d_effect_val9;
		status_effect_detail.effect_value = argv.data[i].effect_value;

		status_effect_detail.action_tick = (*it).action_gap;
		status_effect_detail.passive_tick = (*it).passive_gap;
		status_effect_detail.set_time((*it).time_gap);

		status_effect_detail.caster_id = status_info_detail.caster_role_id;
		status_effect_detail.status_id = argv.status_id;

		status_effect_detail.is_hit = (*it).buff_hit;
		status_effect_detail.skill_id = (*it).buff_trigger_skill;
		status_effect_detail.trigger_buff = (*it).trigger_buff;
		status_effect_detail.buff_chance = (*it).buff_chance;
		for (Int_Hash_Set::const_iterator it_bt = (*it).beat_type_set.begin(); it_bt != (*it).beat_type_set.end(); ++it_bt) {
			status_effect_detail.heat_type.insert(*it_bt);
		}
		status_effect_detail.is_overlay = (*it).buff_overlay;

		if (!(*it).buff_delay.empty()) {
			status_effect_detail.is_delay = true;
			status_effect_detail.set_delay_time((*it).delay_time_gap);
			status_effect_detail.delay_action_tick = (*it).delay_action_gap;
			status_effect_detail.delay_passive_tick = (*it).delay_passive_gap;
		}

		status_effect_detail.buff_scene = detail.buff_scene;
		status_info_detail.effect_map.push_back(status_effect_detail);
		++i;
	}
	detail.status_info_map.insert(std::make_pair(status_info_detail.caster_role_id, status_info_detail));
	int ret = hero_insert_status(detail);
	if (0 == ret) {
		heroer_prop_refresh(player_self()->heroer_detail().fighter_index, false, true);
		if (!from_scene) {
			this->hero_sync_status_add(argv);
		}
	}
	return ret;
}
//int Logic_Heroer::hero_insert_status(std::vector<Status_Effect_Argv> &argv_vec) {
//	/*int Logic_Logic_Heroer::logic_insert_status_hero(const std::vector<Status_Effect_Argv> sea_vec) {
//	for (std::vector<Status_Effect_Argv>::const_iterator it = sea_vec.begin(); it != sea_vec.end(); ++it) {
//		const Btl_Buff_Cfg *cfg = CONFIG_CACHE_BUFF->btl_buff(it->status_id);
//		if (!cfg) {
//			return ERROR_STATUS_NOT_EXIST;
//		}
//	}
//	Block_Buffer buf;
//	buf.make_message(INNER_SYNC_STATUS_HERO_INSERT);
//	buf.write_uint16(sea_vec.size());
//	for (std::vector<Status_Effect_Argv>::const_iterator it = sea_vec.begin(); it != sea_vec.end(); ++it) {
//		it->serialize(buf);
//	}
//	buf.finish_message();
//	this->send_to_scene(buf);
//	return 0;
//}*/
//	return 0;
//}
int Logic_Heroer::hero_insert_status(Status_Info &detail) {
	if (detail.is_fight_buff) {
		return -1;
	}
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(detail.status_id_);
	if (NULL == status_config) {
		MSG_USER_TRACE("status id %d configure file error.", detail.status_id_);
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (is_action_remove(detail.status_id_) || is_passive_remove(detail.status_id_)) {
		return -1;
	}

	//check
//	MSG_81000071 msg;
//	msg.type = (int)detail.is_fight_buff;
//	Status_Info_Msg msginfo;
	bool need_pop = false; // 是否需要移除新旧状态队列
	Time_Value now = Time_Value::gettimeofday();

	Logic_Status_Detail::Status_Map::iterator status_it = heroer_detail_->logic_status_detail_.status_map.find(detail.status_id_);
	if (status_it == heroer_detail_->logic_status_detail_.status_map.end()) {
		detail.overlay_ = 1; // 最开始
		if (!heroer_detail_->logic_status_detail_.status_map.insert(std::make_pair(detail.status_id_, detail)).second) {
			return -1;
		}
		status_it = heroer_detail_->logic_status_detail_.status_map.find(detail.status_id_);
		if (status_it == heroer_detail_->logic_status_detail_.status_map.end()) {
			return ERROR_INNER_SERVER;
		}

		if (is_timepri_remove(detail.status_id_)) {
			insert_status_and_sort(&(status_it->second), heroer_detail_->out_status_timepri_list);
		}

		for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_it->second.status_info_map.begin();
				status_info_map_it != status_it->second.status_info_map.end(); ++status_info_map_it) {
			status_info_map_it->second.overlay = status_it->second.overlay_;
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				(*it).overlay = status_it->second.overlay_;
				if (((*it).heat_type.count(EFFECT_TYPE_TIME) && (*it).delay_time_tick > Time_Value::zero && Time_Value::zero == (*it).interval)
									|| (*it).delay_action_tick > 0 || (*it).delay_passive_tick > 0 || (*it).delay_time_tick > Time_Value::zero) {
				} else {
					hero_insert_status_effect_handle(status_info_map_it->second, (*it));
				}
			}
		}
	} else {
		if (status_config->overlay > 1) {// 可叠加且叠加数大于1
			if (status_config->overlay == status_it->second.overlay_) {
				need_pop = true;
			}
			int ret = hero_merge_status_to_status(status_it->second, detail, status_config);
			if (0 != ret) {
				return ret;
			}
		} else {// 不可叠加
			need_pop = true;
			int ret = hero_cover_status_to_status(status_it->second, detail);
			if (0 != ret) {
				return ret;
			}
		}
	}
	status_it = heroer_detail_->logic_status_detail_.status_map.find(detail.status_id_);
	if (status_it == heroer_detail_->logic_status_detail_.status_map.end()) {
		return -1;
	}
	if (!detail.buff_scene.empty()) {
		heroer_detail_->buff_scene_status.insert(detail.status_id_);
	}

	if (need_pop) {
		status_it->second.status_fifo.pop_front();
	}
	status_it->second.status_fifo.push_back(detail.last_caster_role_id_);

//	if (status_it->second->scene_visible.empty() || check_int_in_vec(fighter()->move_scene_id(), status_it->second->scene_visible)) {
//		msginfo.reset();
//		msginfo.status_owner = role_id();
//		msginfo.status_id = detail.status_id_;
//		msginfo.overlay = status_it->second->overlay_;
//		msginfo.last_time_ms = status_it->second->last_time_ms;
//		msginfo.remain_time = status_it->second->last_end_time.sec() - now.sec();
//		if (msginfo.remain_time < 0) {
//			msginfo.remain_time = 0;
//		}
//		make_buff_msg(status_it->second, msginfo.prop);
//		msg.buff_detail.push_back(msginfo);
//
//		Block_Buffer buf;
//		MAKE_MSG_BUF(msg, buf);
//		call_broadcast_around(buf);
//	}
	return 0;
}
int Logic_Heroer::hero_erase_status_type(const int status_id, const bool from_scene) {
	Logic_Status_Detail::Status_Map::iterator it = heroer_detail_->logic_status_detail_.status_map.find(status_id);
	if (it != heroer_detail_->logic_status_detail_.status_map.end()) {
		Status_Info *status_info = &(it->second);
		for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info->status_info_map.begin();
				status_info_map_it != status_info->status_info_map.end(); ++status_info_map_it) {
			for (Status_Info_Detail::Effect_Vec::iterator effect_map_it = status_info_map_it->second.effect_map.begin();
					effect_map_it != status_info_map_it->second.effect_map.end(); ++effect_map_it) {
				this->hero_erase_status_effect_handle((*effect_map_it));
			}
		}
		heroer_detail_->out_status_timepri_list.remove(status_info);
		heroer_detail_->logic_status_detail_.status_map.erase(status_id);
		if (!from_scene) {
			Int_Vec iv;
			iv.push_back(status_id);
			this->hero_sync_status_erase(Int_Vec(iv));
		}
		heroer_prop_refresh(player_self()->heroer_detail().fighter_index, false, true);
		return 0;
	}

	return -1;
}
int Logic_Heroer::hero_sync_status_add(Status_Effect_Argv &argv) {
	if (!heroer_detail_->fighter_index) {
		return -1;
	}
	const Btl_Buff_Cfg *cfg = CONFIG_CACHE_BUFF->btl_buff(argv.status_id);
	if (!cfg) {
		return ERROR_STATUS_NOT_EXIST;
	}
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_STATUS_HERO_INSERT);
	buf.write_int8(1); // add status
	buf.write_uint16(1);
	argv.serialize(buf);
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}
int Logic_Heroer::hero_sync_status_erase(const Int_Vec iv) {
	if (!heroer_detail_->fighter_index) {
		return -1;
	}
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_STATUS_HERO_INSERT);
	buf.write_int8(0); // del status
	buf.write_uint16(iv.size());
	for (Int_Vec::const_iterator it = iv.begin(); it != iv.end(); ++it) {
		buf.write_int32(*it);
	}
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}
int Logic_Heroer::hero_merge_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from,const Btl_Buff_Cfg *status_config) {
	if (!status_config) {
		return -1;
	}
	if (status_info_to.status_info_map.size() != status_info_from.status_info_map.size()) {
		return ERROR_CLIENT_OPERATE;
	}
	if (is_action_remove(status_info_to.status_id_) || is_passive_remove(status_info_to.status_id_)) {
		return -1;
	}
//	status_info_to.cfg_action_times = status_info_from.cfg_action_times;
//	status_info_to.cfg_trigger_times = status_info_from.cfg_trigger_times;
	status_info_to.last_caster_master_id_ = status_info_from.last_caster_master_id_;
	status_info_to.last_caster_role_id_ = status_info_from.last_caster_role_id_;
	status_info_to.last_end_time = status_info_from.last_end_time;
	status_info_to.last_start_time = status_info_from.last_start_time;
	status_info_to.last_time_ms = status_info_from.last_time_ms;
	status_info_to.last_status_from_ = status_info_from.last_status_from_;
	status_info_to.status_id_ = status_info_from.status_id_;

	if (is_timepri_remove(status_info_to.status_id_)) {
		replace_status_and_sort(&status_info_to, heroer_detail_->out_status_timepri_list);
	}

	if (status_info_to.status_info_map.empty()) {
		if (!status_info_from.status_info_map.empty()) {
			MSG_DEBUG("ERROR_IMPOSSIBLE!");
			status_info_to.overlay_ = 1;
			status_info_to.status_info_map = status_info_from.status_info_map;
			Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
			status_info_map_it->second.overlay = status_info_to.overlay_;
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				(*it).overlay = status_info_to.overlay_;
				this->hero_insert_status_effect_handle(status_info_map_it->second, (*it));
			}
		}
		return 0;
	}

	++status_info_to.overlay_;
	bool has_overlay = false; // 是否有超过,有则处理掉旧的
	if (status_config->overlay < status_info_to.overlay_) {
		status_info_to.overlay_ = status_config->overlay;
		has_overlay = true;
	}

	return hero_merge_status_to_status_overlay(status_info_to, status_info_from, has_overlay);
}
int Logic_Heroer::hero_merge_status_to_status_overlay(Status_Info &status_info_to, Status_Info &status_info_from, bool has_overlay) {
	//被插入，只有一个
	Status_Info::Status_Info_Map::iterator status_info_map_it_from = status_info_from.status_info_map.begin();
	if (status_info_map_it_from == status_info_from.status_info_map.end()) {
		return -1;
	}

	Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.find(status_info_map_it_from->first);
	if (has_overlay) {
		std::deque<role_id_t>::const_iterator status_fifo_it = status_info_to.status_fifo.begin();
		if (status_fifo_it != status_info_to.status_fifo.end()) {
			if (*status_fifo_it == status_info_map_it_from->first) { // 已存在,且是要被替换的，不替换
				return 0; ///
			}
			Status_Info::Status_Info_Map::iterator it_pop = status_info_to.status_info_map.find(*status_fifo_it);
			if (it_pop != status_info_to.status_info_map.end()) {
				Status_Info_Detail status_info_detail_old = it_pop->second;
				--it_pop->second.overlay;
				if (it_pop->second.effect_map.size() == status_info_detail_old.effect_map.size()) {
					uint16_t len = 0;
					for (Status_Info_Detail::Effect_Vec::iterator effect_it = it_pop->second.effect_map.begin();
							effect_it != it_pop->second.effect_map.end(); ++effect_it) {
						this->hero_update_status_effect_handle(it_pop->second, status_info_detail_old,
								(*effect_it), status_info_detail_old.effect_map[len]);
						++len;
					}
				}
				//根据层数处理
				if (it_pop->second.overlay <= 0) {
					status_info_to.status_info_map.erase(it_pop);
				}
			} else {
				MSG_USER("ERROR_IMPOSSIBLE");
			}
		} else {
			MSG_USER("ERROR_IMPOSSIBLE: someone modify progress!");
		}
	}

	if (status_info_map_it == status_info_to.status_info_map.end()) { // 当前人物还未插入
		if (status_info_to.status_info_map.insert(*status_info_map_it_from).second) {
			status_info_map_it = status_info_to.status_info_map.find(status_info_map_it_from->first);
			if (status_info_map_it != status_info_to.status_info_map.end()) {
				status_info_map_it->second.overlay = status_info_to.overlay_;
				for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
						it != status_info_map_it->second.effect_map.end(); ++it) {
					(*it).overlay = status_info_to.overlay_;
					this->hero_insert_status_effect_handle(status_info_map_it->second, (*it));
				}
			}
		}
	} else {
		/*
		 * 已插入：处理buff,effect层数效果,持续时间
		 * */
		Status_Info_Detail status_info_detail_old = status_info_map_it->second;
		status_info_map_it->second.overlay = status_info_to.overlay_;
		uint16_t len = 0;
		for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
				it != status_info_map_it->second.effect_map.end(); ++it) {
			if ((*it).is_overlay) {
				(*it).overlay = status_info_to.overlay_;
				this->hero_update_status_effect_handle(status_info_map_it->second, status_info_detail_old,
						(*it), status_info_detail_old.effect_map[len]);
			}
			++len;
		}
	}
	return 0;
}
int Logic_Heroer::hero_cover_status_to_status(Status_Info &status_info_to, Status_Info &status_info_from) {
	if (is_action_remove(status_info_to.status_id_)
			|| is_passive_remove(status_info_to.status_id_)) {
		return -1;
	}
	status_info_to.last_caster_master_id_ = status_info_from.last_caster_master_id_;
	status_info_to.last_caster_role_id_ = status_info_from.last_caster_role_id_;
	status_info_to.last_end_time = status_info_from.last_end_time;
	status_info_to.last_start_time = status_info_from.last_start_time;
	status_info_to.last_time_ms = status_info_from.last_time_ms;
	status_info_to.last_status_from_ = status_info_from.last_status_from_;
	status_info_to.overlay_ = 1; // 因为是覆盖，只存在一个
	status_info_to.status_id_ = status_info_from.status_id_;

	if (is_timepri_remove(status_info_to.status_id_)) {
		replace_status_and_sort(&status_info_to, this->heroer_detail_->out_status_timepri_list);
	}

	// 因为是覆盖，只存在一个
	if (status_info_to.status_info_map.empty()) {
		if (!status_info_from.status_info_map.empty()) {
			MSG_DEBUG("ERROR_IMPOSSIBLE!");
			status_info_to.status_info_map = status_info_from.status_info_map;
			Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
			status_info_map_it->second.overlay = status_info_to.overlay_;
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				(*it).overlay = status_info_to.overlay_;
				this->hero_insert_status_effect_handle(status_info_map_it->second, (*it));
			}
		}
		return 0;
	}

	Status_Info::Status_Info_Map status_info_map_old = status_info_to.status_info_map;
	// from获取已经拥有的action_times,next_opt
	if (!status_info_from.status_info_map.empty()) {
		Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
		Status_Info::Status_Info_Map::iterator status_info_map_from_it = status_info_from.status_info_map.begin();
		if (status_info_map_it->second.effect_map.size() == status_info_map_it->second.effect_map.size()) {
			uint16_t len = 0;
			for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				status_info_map_from_it->second.effect_map[len].action_times = (*it).action_times;
				status_info_map_from_it->second.effect_map[len].next_opt = (*it).next_opt;
				++len;
			}
		} else {
			MSG_USER("ERROR_IMPOSSIBLE:maybe config : %d", status_info_from.status_id_);
		}
	}
	status_info_to.status_info_map = status_info_from.status_info_map;

	// 因为是覆盖，只存在一个
	if (!status_info_map_old.empty() && !status_info_to.status_info_map.empty()) {
		Status_Info::Status_Info_Map::iterator status_info_map_old_it = status_info_map_old.begin();
		Status_Info::Status_Info_Map::iterator status_info_map_it = status_info_to.status_info_map.begin();
		if (status_info_map_old_it != status_info_map_old.end()
				&& status_info_map_it != status_info_to.status_info_map.end()) {
			status_info_map_it->second.overlay = status_info_to.overlay_;
			if (status_info_map_it->second.effect_map.size() == status_info_map_old_it->second.effect_map.size()) {
				uint16_t len = 0;
				for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
						it != status_info_map_it->second.effect_map.end(); ++it) {
					(*it).overlay = status_info_to.overlay_;
					this->hero_update_status_effect_handle(status_info_map_it->second, status_info_map_old_it->second,
							(*it), status_info_map_old_it->second.effect_map[len]);
					++len;
				}
			}
		}
	}

	return 0;
}
int Logic_Heroer::hero_insert_status_effect_handle(Status_Info_Detail &status_info, Status_Effect_Detail &effect) {
	if (0 == effect.overlay
			|| !Utility_Func::lottery_success(effect.buff_chance)) {
		return 0;
	}
	if (!hero_check_status_effect(effect)) {
		return 0;
	}

	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
		this->hero_add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		this->hero_add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		this->hero_add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
//	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(heroer_detail_->logic_status_detail_.running_effect.exp_map.count(id) != 0);
//		effect.i_effect_value1 = id;
//		heroer_detail_->logic_status_detail_.running_effect.exp_map[effect.i_effect_value1] = effect.d_effect_value1;
//		break;
//	}
	default:
		break;
	}
//	this->buff_use_skill_passive(effect);
//	this->insert_status_from_fight(effect);
	return 0;
}
int Logic_Heroer::hero_opt_status_effect_handle_by_scene_change(Status_Info_Detail &status_info, Status_Effect_Detail &effect, const int des_scene) {
	if (0 == effect.overlay
			|| !Utility_Func::lottery_success(effect.buff_chance)) {
		return 0;
	}
	if (!hero_check_status_effect(effect, des_scene)) {
		if (effect.effect_times > 0) {
			hero_erase_status_effect_handle(effect);
		}
		return 0;
	}
	if (effect.effect_times > 0) {
		return 0;
	}

	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
		this->hero_add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		this->hero_add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		this->hero_add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
//	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(heroer_detail_->logic_status_detail_.running_effect.exp_map.count(id) != 0);
//		effect.i_effect_value1 = id;
//		heroer_detail_->logic_status_detail_.running_effect.exp_map[effect.i_effect_value1] = effect.d_effect_value1;
//		break;
//	}
	default:
		break;
	}
//	this->buff_use_skill_passive(effect);
//	this->insert_status_from_fight(effect);
	return 0;
}
int Logic_Heroer::hero_update_status_effect_handle(Status_Info_Detail &status_info, Status_Info_Detail &status_info_from, Status_Effect_Detail &new_effect,
		Status_Effect_Detail &old_effect) {
	if (0 == old_effect.overlay
			 || !Utility_Func::lottery_success(new_effect.buff_chance)) {
		return this->hero_insert_status_effect_handle(status_info, new_effect);
	}
	if (0 == new_effect.overlay) {
//		return this->erase_status_effect_handle(status_info_from, old_effect);
		return this->hero_erase_status_effect_handle(old_effect);
	}
	if (!hero_check_status_effect(new_effect)) {
		return 0;
	}
	int effect_type = new_effect.effect_type;
	switch (effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR: {
		if (new_effect.i_effect_value1 == old_effect.i_effect_value1
				&& new_effect.d_effect_value1 == old_effect.d_effect_value1
				&& new_effect.overlay == old_effect.overlay
				&& (new_effect.d_effect_value2 == old_effect.d_effect_value2
						|| new_effect.i_effect_value1 == Property_Type::PT_MORALE_CURRENT)) {
			new_effect.effect_times = old_effect.effect_times;
			break;
		}
		//// 策划保证不会有心跳加[上限值]的东西 effect_times不需要计算
		this->hero_cover_status_prop(new_effect, old_effect);
		if (new_effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& new_effect.i_effect_value1 != PT_MORALE_CURRENT
				&& new_effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& new_effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			new_effect.effect_times = old_effect.effect_times + (new_effect.overlay - old_effect.overlay);
		}
		break;
	}
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		if (new_effect.i_effect_value1 == old_effect.i_effect_value1
				&& new_effect.d_effect_value1 == old_effect.d_effect_value1
				&& new_effect.overlay == old_effect.overlay
				&& (new_effect.d_effect_value2 == old_effect.d_effect_value2
						|| new_effect.i_effect_value1 == Property_Type::PT_MORALE_CURRENT)) {
			new_effect.effect_times = old_effect.effect_times;
			break;
		}
		//// 策划保证不会有心跳加[上限值]的东西 effect_times不需要计算
		this->hero_cover_status_prop(new_effect, old_effect);
		if (new_effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& new_effect.i_effect_value1 != PT_MORALE_CURRENT
				&& new_effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& new_effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			new_effect.effect_times = old_effect.effect_times + (new_effect.overlay - old_effect.overlay);
		}
		break;
//	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(status_detail_->running_effect.exp_map.count(id) != 0);
//		new_effect.i_effect_value1 = id;
//		status_detail_->running_effect.exp_map[new_effect.i_effect_value1] = new_effect.d_effect_value1;
//		status_detail_->running_effect.exp_map.erase(old_effect.i_effect_value1);
//		break;
//	}
	default:
		break;
	}
	return 0;
}
int Logic_Heroer::hero_erase_status_effect_handle(Status_Effect_Detail &effect) {
	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			this->hero_sub_status_prop(effect);
		}
		break;
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			this->hero_sub_status_prop(effect);
		}
		break;
//	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
//		if (0 == status_detail_->running_effect.exp_map.erase(effect.i_effect_value1)) {
//			MSG_USER("exp_map has no %d", effect.i_effect_value1);
//		}
//		break;
//	}
	default:
		break;
	}
//	this->buff_unuse_skill_passive(effect);
	return 0;
}
bool Logic_Heroer::hero_check_status_effect(Status_Effect_Detail &effect) {
	if (effect.buff_scene.empty()) {
		return true;
	}

	const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->base_detail().location.scene_id);
	if (sc) {
		for (Int_Vec::const_iterator it = effect.buff_scene.begin(); it != effect.buff_scene.end(); ++it) {
			if (*it == sc->type) {
				return true;
			}
		}
	}
	return false;
}

bool Logic_Heroer::hero_check_status_effect(Status_Effect_Detail &effect, const int scene) {
	if (effect.buff_scene.empty()) {
		return true;
	}
	const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(scene);
	if (sc) {
		for (Int_Vec::const_iterator it = effect.buff_scene.begin(); it != effect.buff_scene.end(); ++it) {
			if (*it == sc->type) {
				return true;
			}
		}
	}
	return false;
}

int Logic_Heroer::hero_add_status_prop(Status_Effect_Detail & fix, int skill_id) {
	if (fix.i_effect_value1 < PT_FIGHT_PROP_START || fix.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;
	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(fix.status_id);
	if (!bbc) {
		return -1;
	}

	Prop_Setter prop;
	prop.prop_type = (Property_Type)fix.i_effect_value1;
	if (fix.i_effect_value1 == Property_Type::PT_MORALE_CURRENT) {
		prop.fixed = 0.0;
		prop.percent = 0.0;
		prop.basic = fix.d_effect_value1 * fix.overlay;
		prop.addi_type = Addition_Type::AT_BASIC;
	} else if (fix.i_effect_value1 == Property_Type::PT_SPEED) {
		prop.fixed = fix.d_effect_value1 * fix.overlay;
		prop.percent = 0.0;
		prop.basic = 0.0;
		prop.addi_type = Addition_Type::AT_STATUS;
	} else if (fix.i_effect_value1 == PT_BLOOD_CURRENT) {
		prop.fixed = fix.d_effect_value1 * fix.overlay;
		prop.percent = fix.d_effect_value2 * fix.overlay;
		prop.addi_type = Addition_Type::AT_STATUS;
	} else {
		prop.fixed = fix.d_effect_value1 * fix.overlay;
		prop.percent = fix.d_effect_value2 * fix.overlay;
		prop.addi_type = Addition_Type::AT_STATUS;
	}

	if (prop.fixed > 0.000000001) {
		heroer_detail_->status_pro_val.insert(std::make_pair(fix.i_effect_value1, prop.fixed));
	}
	if (prop.percent > 0.000000001) {
		heroer_detail_->status_pro_val_per.insert(std::make_pair(fix.i_effect_value1, prop.percent));
	}

	return 0;
}

int Logic_Heroer::hero_add_status_prop_by_per(Status_Effect_Detail & fix, int skill_id) {
//	if (fix.i_effect_value1 != PT_BLOOD_CURRENT)
//		return 0;
//	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(fix.status_id);
//	if (!bbc) {
//		return -1;
//	}
//
//	Prop_Setter_Vec prop_vec;
//	Prop_Setter prop;
//	prop.prop_type = (Property_Type)fix.i_effect_value1;
//	if (fix.d_effect_value1 * fix.overlay != 0) {
//		double mx = fighter()->blood_max();
//		prop.basic = fix.d_effect_value1 * fix.overlay * mx;
//		prop.addi_type = Addition_Type::AT_BASIC;
//		prop_vec.push_back(prop);
//	} else {
//		return 0;
//	}
//	prop_vec.push_back(prop);

	return 0;
}
int Logic_Heroer::hero_hook_scene_change(const int src_scene, const int des_scene) {
	if (src_scene == des_scene) {
		return -1;
	}
	Int_Vec need_erase_status;
	for (Int_Hash_Set::const_iterator it = heroer_detail_->buff_scene_status.begin();
			it != heroer_detail_->buff_scene_status.end(); ++it) {
		Logic_Status_Detail::Status_Map::iterator status_it = heroer_detail_->logic_status_detail_.status_map.find(*it);
		if (status_it != heroer_detail_->logic_status_detail_.status_map.end()) {
			for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_it->second.status_info_map.begin();
					status_info_map_it != status_it->second.status_info_map.end(); ++status_info_map_it) {
//				status_info_map_it->second.overlay = status_it->second.overlay_;
				for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
						it != status_info_map_it->second.effect_map.end(); ++it) {
//					(*it).overlay = status_it->second.overlay_;
					if (((*it).heat_type.count(EFFECT_TYPE_TIME) && (*it).delay_time_tick > Time_Value::zero && Time_Value::zero == (*it).interval)
										|| (*it).delay_action_tick > 0 || (*it).delay_passive_tick > 0 || (*it).delay_time_tick > Time_Value::zero) {
					} else {
						hero_opt_status_effect_handle_by_scene_change(status_info_map_it->second, (*it), des_scene);
					}
				}
			}
		} else {
			need_erase_status.push_back(*it);
		}
	}
	for (Int_Vec::iterator it = need_erase_status.begin(); it != need_erase_status.end(); ++it) {
		heroer_detail_->buff_scene_status.erase(*it);
	}
	return 0;
}

int Logic_Heroer::hero_sub_status_prop(Status_Effect_Detail& fix) {
	if (fix.i_effect_value1 < PT_FIGHT_PROP_START || fix.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;

	if (fix.i_effect_value1 == Property_Type::PT_MORALE_CURRENT
			|| fix.i_effect_value1 == Property_Type::PT_BLOOD_POOL_CURRENT)
		return 0;
	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(fix.status_id);
	if (!bbc) {
		return -1;
	}

	int effect_times = fix.effect_times;
//	if (0 == effect_times) {
//		effect_times = 1;
//	}
//	effect_times *=  fix.overlay;

	fix.effect_times = 0;

	std::map<int, double>::iterator it_pro = heroer_detail_->status_pro_val.find(fix.i_effect_value1);
	if (it_pro != heroer_detail_->status_pro_val.end()) {
		it_pro->second -= fix.d_effect_value1 * effect_times;
		if (it_pro->second <= 0.000000001) {
			heroer_detail_->status_pro_val.erase(it_pro);
		}
	}
	std::map<int, double>::iterator it_pro_per = heroer_detail_->status_pro_val_per.find(fix.i_effect_value1);
	if (it_pro_per != heroer_detail_->status_pro_val_per.end()) {
		it_pro_per->second -= fix.d_effect_value2 * effect_times;
		if (it_pro_per->second <= 0.000000001) {
			heroer_detail_->status_pro_val_per.erase(it_pro_per);
		}
	}
	return 0;
}

int Logic_Heroer::hero_cover_status_prop(Status_Effect_Detail & fix, Status_Effect_Detail & fix_from) {
	if (fix.i_effect_value1 < PT_FIGHT_PROP_START || fix.i_effect_value1 > PT_FIGHT_PROP_END)
		return 0;
	const Btl_Buff_Cfg *bbc = CONFIG_CACHE_BUFF->btl_buff(fix.status_id);
	if (!bbc) {
		return -1;
	}

	Prop_Setter_Vec prop_vec;
	Prop_Setter prop;
	prop.prop_type = (Property_Type)fix.i_effect_value1;
	if (fix.i_effect_value1 == Property_Type::PT_MORALE_CURRENT) {
		prop.fixed = 0.0;
		prop.percent = 0.0;
		prop.basic = fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay;
		prop.addi_type = Addition_Type::AT_BASIC;
	} else if (fix.i_effect_value1 == Property_Type::PT_SPEED) {
		prop.fixed = fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay;
		prop.percent = 0.0;
		prop.basic = 0.0;
		prop.addi_type = Addition_Type::AT_STATUS;
	} else {
		prop.fixed = fix.d_effect_value1 * fix.overlay - fix_from.d_effect_value1 * fix_from.overlay;
		prop.percent = fix.d_effect_value2 * fix.overlay - fix_from.d_effect_value2 * fix_from.overlay;
		prop.addi_type = Addition_Type::AT_STATUS;
	}

	if (prop.fixed > 0.000000001) {
		heroer_detail_->status_pro_val[fix.i_effect_value1] += prop.fixed;//.insert(std::make_pair(fix.i_effect_value1, prop.fixed));
	}
	if (prop.percent > 0.000000001) {
		heroer_detail_->status_pro_val_per[fix.i_effect_value1] += prop.percent;//.insert(std::make_pair(fix.i_effect_value1, prop.percent));
	}

	return 0;
}

void Logic_Heroer::heroer_assist_skill_refresh(
		const Hero_Config &cfg, Hero_Info_Detail &hero) {

	hero.assist_skill.reset();
	if (cfg.assist_skill_id_vec.empty()) return;
	int skill_id = *cfg.assist_skill_id_vec.begin();
	if (0 == skill_id) return;

	const Skill_Config *cfg_skill = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (0 == cfg_skill) return;
	for (Skill_Detail_Map::const_iterator it = cfg_skill->skill_level_map.begin();
			it != cfg_skill->skill_level_map.end(); ++it) {
		if (hero.level <  it->second.lv_lim) continue;
		if (hero.intimacy_lv < it->second.intimacy_lv) continue;
		if (it->second.skill_level <= hero.assist_skill.val_2) continue;
		hero.assist_skill.val_1 = it->second.skill_id;
		if (hero.assist_skill.val_2 > it->second.skill_level) continue;
		hero.assist_skill.val_2 = it->second.skill_level;
	}
}

void Logic_Heroer::update_hero_assist_info(Hero_Info_Detail &hero) {

	MSG_50102031 msg;

	msg.hero_id = hero.hero_id;
	msg.hero_idx = hero.hero_index;
	get_hero_assist_info(hero, msg.hero_assist_info) ;

	THIS_SEND_TO_CLIENT(msg);
}

int Logic_Heroer::handle_hero_assist(
		int type, int hero_idx, int loc) {

	Int_Double_Map props;
	Int_Int_Vec old_skills;
	pre_refresh_hero_assist(props, old_skills);
	int result = 0;
	if (type == 1) {
		result = hero_assist_on(hero_idx, loc);
	} else if (type == 2){
		result = hero_assist_off(hero_idx, loc);
	} else {
		result = ERROR_CLIENT_PARAM;
	}

	if ( 0 != result ) return result;

	MSG_50102032 msg;
	msg.result = 0;
	Int_Int int_val;
	for (Int_Int_Map::iterator it = heroer_detail_->hero_assist_map.begin();
			it != heroer_detail_->hero_assist_map.end(); ++it) {
		int_val.reset();
		int_val.val_1 = it->first;
		int_val.val_2 = it->second;
		msg.hero_assist_vec.push_back(int_val);
	}

	THIS_SEND_TO_CLIENT(msg);

	pos_refresh_hero_assist(props, old_skills);
	fight_heroer_prop_refresh();

	return 0;
}

int Logic_Heroer::hero_assist_on(int hero_idx, int loc) {


	// judge role's level
	if (loc > MAX_ASSIST_HERO || loc <= 0) return ERROR_CLIENT_PARAM;
	int role_lv1 = CONFIG_CACHE_HERO->find_intimacy_common(3);
	int role_lv2 = CONFIG_CACHE_HERO->find_intimacy_common(4);
	int hero_lv = CONFIG_CACHE_HERO->find_intimacy_common(5);
	int role_lv = level();
	if (1 == loc && role_lv < role_lv1) return ERROR_ASSIST_LV_LIMIT;
	if (2 == loc && role_lv < role_lv2) return ERROR_ASSIST_LV_LIMIT;

	Heroer_Detail::Hero_Map::iterator find_it = heroer_detail_->hero_map.find(hero_idx);
	if (find_it == heroer_detail_->hero_map.end()) return ERROR_INNER_SERVER;
	if (find_it->second.level < hero_lv) return ERROR_ASSIST_HERO_LV_LIMIT;

	for (Int_Int_Map::iterator it = heroer_detail_->hero_assist_map.begin();
			it != heroer_detail_->hero_assist_map.end(); ++it) {
		if (it->second == hero_idx) return ERROR_HERO_ASSISTED;
	}

	const Hero_Config* cfg_hero = CONFIG_CACHE_HERO->find_hero(find_it->second.hero_id);
	if (0 == cfg_hero) return ERROR_CONFIG_NOT_EXIST;
	if (find_it->second.intimacy_lv < cfg_hero->hero_intimacy.assist_lv_limit)
		return ERROR_INTIMACY_LV_LIMIT;

	if (find_it->second.hero_index == heroer_detail_->fighter_index) return ERROR_HERO_FIGHTING;

	Int_Int_Map::iterator find_it_assist = heroer_detail_->hero_assist_map.find(loc);
	if (find_it_assist != heroer_detail_->hero_assist_map.end()) {
		hero_assist_off(find_it_assist->second, loc);
	}

	heroer_detail_->hero_assist_map[loc] = hero_idx;

	return 0;
}

int Logic_Heroer::hero_assist_off(int hero_idx, int loc) {

	if (loc > MAX_ASSIST_HERO || loc <= 0) return ERROR_CLIENT_PARAM;
	Int_Int_Map::iterator find_it = heroer_detail_->hero_assist_map.find(loc);
	if (find_it == heroer_detail_->hero_assist_map.end()) return ERROR_CLIENT_PARAM;
	if (hero_idx != find_it->second) return ERROR_CLIENT_PARAM;

	heroer_detail_->hero_assist_map.erase(find_it);

	return 0;
}

int Logic_Heroer::handle_hero_community(int id, int answer, int hero_idx) {

	if (0 == heroer_detail_) return ERROR_INNER_SERVER;
	Heroer_Detail::Hero_Map::iterator find_it = heroer_detail_->hero_map.find(hero_idx);
	if (find_it == heroer_detail_->hero_map.end()) return ERROR_INNER_SERVER;
	int max_community_times = CONFIG_CACHE_HERO->find_intimacy_common(1);
	if (find_it->second.community_times >= max_community_times) return
			ERROR_NO_COMMUNITY_TIMES;

	const Intimacy_Community *cfg_community = CONFIG_CACHE_HERO->find_intimacy_community(id);
	if (0 == cfg_community) return ERROR_CONFIG_NOT_EXIST;
	if (cfg_community->hero_id != find_it->second.hero_id) return ERROR_CLIENT_PARAM;

	const Hero_Config* cfg_hero = CONFIG_CACHE_HERO->find_hero(find_it->second.hero_id);
	if (0 == cfg_hero) return ERROR_CONFIG_NOT_EXIST;

	if (find_it->second.intimacy_lv < cfg_hero->hero_intimacy.lvl_limit) {

		double val = 0;
		if (answer == 1) val = cfg_community->val.val_1;
		if (answer == 2) val = cfg_community->val.val_2;

		int add_val = static_cast<int>(val * cfg_hero->hero_intimacy.convert_fix);
		find_it->second.intimacy_exp += add_val;

		hero_intimacy_upgrade(find_it->second, *cfg_hero);
	}

	++find_it->second.community_times;
	MSG_50102033 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	update_hero_assist_info(find_it->second);
	return 0;
}

int Logic_Heroer::hero_intimacy_upgrade(Hero_Info_Detail &hero, const Hero_Config &cfg_hero) {

	Int_Double_Map props;
	Int_Int_Vec old_skills;
	pre_refresh_hero_assist(props, old_skills);

	int ret_val = 0;
	while(hero.intimacy_exp > 0) {

		if (hero.intimacy_lv >= cfg_hero.hero_intimacy.lvl_limit) {
			hero.exp = 0;
			break;
		}
		const Intimacy_Base *cfg_base = CONFIG_CACHE_HERO->find_intimacy_base(hero.intimacy_lv);
		if (0 == cfg_base) break;

		int upgrade_exp = static_cast<int>(cfg_base->exp * cfg_hero.hero_intimacy.exp_fix);
		if (hero.intimacy_exp < upgrade_exp) break;

		hero.intimacy_exp -= upgrade_exp;
		hero.intimacy_lv += 1;
		ret_val = 1;
	}

	if (1 == ret_val) {
		heroer_assist_skill_refresh(cfg_hero, hero);
		pos_refresh_hero_assist(props, old_skills);
		fight_heroer_prop_refresh();
	}

	return ret_val;
}

int Logic_Heroer::handle_gift_hero(int item_id, int hero_idx) {

	if (0 == heroer_detail_) return ERROR_INNER_SERVER;
	Heroer_Detail::Hero_Map::iterator find_it = heroer_detail_->hero_map.find(hero_idx);
	if (find_it == heroer_detail_->hero_map.end()) return ERROR_INNER_SERVER;
	int max_gift_times = CONFIG_CACHE_HERO->find_intimacy_common(2);
	if (find_it->second.gift_times >= max_gift_times) return ERROR_NO_GIFT_TIMES;

	const Hero_Config* cfg_hero = CONFIG_CACHE_HERO->find_hero(find_it->second.hero_id);
	if (0 == cfg_hero) return ERROR_CONFIG_NOT_EXIST;
	if (find_it->second.intimacy_lv >= cfg_hero->hero_intimacy.lvl_limit) return ERROR_MAX_INTIMACY_LV;

	const Hero_Intimacy_Gift *cfg_gift = CONFIG_CACHE_HERO->find_intimacy_gift(item_id);
	if (0 == cfg_gift) return ERROR_CONFIG_NOT_EXIST;

	Id_Amount item(item_id, 1);
	int ret_val = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item,
			Lose_Item_DM_Info(ITEM_LOSE_INTIMACY_HERO));
	if (0 != ret_val) return ret_val;

	int add_exp = cfg_gift->gift_intimacty;
	if (cfg_gift->type == cfg_hero->hero_intimacy.favor_gift_type) {
		int times_val = CONFIG_CACHE_HERO->find_intimacy_common(6);
		add_exp = add_exp * times_val / 1000;
	} else if (cfg_gift->type == cfg_hero->hero_intimacy.disfavor_gift_type) {
		int times_val = CONFIG_CACHE_HERO->find_intimacy_common(7);
		add_exp = add_exp * times_val / 1000;
	}

	find_it->second.intimacy_exp += add_exp;
	hero_intimacy_upgrade(find_it->second, *cfg_hero);
	++find_it->second.gift_times;

	MSG_50102034 msg;
	msg.result = 0;
	THIS_SEND_TO_CLIENT(msg);

	update_hero_assist_info(find_it->second);
	return 0;
}

int Logic_Heroer::pre_refresh_hero_assist(Int_Double_Map &props, Int_Int_Vec &old_skills) {

	if (0 == heroer_detail_) return -1;
	for (Int_Int_Map::iterator it = heroer_detail_->hero_assist_map.begin();
			it != heroer_detail_->hero_assist_map.end(); ++it) {

		Heroer_Detail::Hero_Map::iterator find_it = heroer_detail_->hero_map.find(it->second);
		if (find_it == heroer_detail_->hero_map.end()) continue;

		figure_out_assist_hero_add_prop(find_it->second, props);
		old_skills.push_back(find_it->second.assist_skill);
	}

	return 0;
}

int Logic_Heroer::pos_refresh_hero_assist(Int_Double_Map &props, Int_Int_Vec &old_skills) {

	if (0 == heroer_detail_) return -1;
	for (Int_Double_Map::iterator it = props.begin(); it != props.end(); ++it) {
		it->second = -it->second;
	}

	Int_Int_Vec new_skills;
	for (Int_Int_Map::iterator it = heroer_detail_->hero_assist_map.begin();
			it != heroer_detail_->hero_assist_map.end(); ++it) {

		Heroer_Detail::Hero_Map::iterator find_it = heroer_detail_->hero_map.find(it->second);
		if (find_it == heroer_detail_->hero_map.end()) continue;

		figure_out_assist_hero_add_prop(find_it->second, props);
		new_skills.push_back(find_it->second.assist_skill);
	}

	Prop_Setter_Vec vec;
	Prop_Setter setter;
	for (Int_Double_Map::iterator it = props.begin(); it != props.end(); ++it) {

		if (!it->second > 0.0 && !it->second < -0.0) continue;
		setter.reset();
		setter.addi_type = Addition_Type::AT_HERO_ASSIST;
		setter.operate = it->second >= 0 ? O_ADD : O_SUB;
		setter.prop_type = Property_Type(it->first);
		setter.basic = 0.0;
		setter.fixed = abs(it->second);
		setter.percent = 0;
		vec.push_back(setter);
	}

	std::vector<Skill_Info> skill_vec;
	Skill_Info skill_info;
	if (!old_skills.empty()) {
		skill_vec.clear();
		for (Int_Int_Vec::iterator it = old_skills.begin(); it != old_skills.end(); ++it) {
			skill_info.reset();
			skill_info.skill_id = it->val_1;
			skill_info.skill_lv = it->val_2;
			skill_vec.push_back(skill_info);
		}
		player_self()->remove_be_skills(skill_vec);
	}
	old_skills.clear();

	if (!new_skills.empty()) {
		skill_vec.clear();
		for (Int_Int_Vec::iterator it = new_skills.begin(); it != new_skills.end(); ++it) {
			skill_info.reset();
			skill_info.skill_id = it->val_1;
			skill_info.skill_lv = it->val_2;
			skill_vec.push_back(skill_info);
			old_skills.push_back(*it);
		}
		player_self()->add_be_skills(skill_vec);
	}

	if (!vec.empty()) {
		player_self()->fight_prop_modify(vec, 551);
	}

	return 0;
}

void Logic_Heroer::refresh_outer_fight_hero_assist_prop(Hero_Info_Detail &hero) {

	Int_Double_Map props;
	Int_Int_Vec skills;
	pre_refresh_hero_assist(props, skills);

	for (Int_Double_Map::iterator it = props.begin(); it != props.end(); ++it) {

		if (!it->second > 0.0 && !it->second < -0.0) continue;
		hero.hero_fighter_detail.modify_fight_property(Property_Type(it->first),
				Addition_Type::AT_HERO_ASSIST, O_SET, 0.0,  it->second, 0.0);
	}

//	if (!skills.empty()) {
//		for (Int_Int_Vec::iterator it = skills.begin(); it != skills.end(); ++it) {
//			Skill_DB_Info skill;
//			skill.skill_id = it->val_1;
//			skill.skill_level = it->val_2;
//			hero.hero_fighter_detail.extra_skill_vec.push_back(skill);
//		}
//	}
}
