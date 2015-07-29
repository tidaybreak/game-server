/*
 * Status.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: ti
 */

#include "Config_Cache_Buff.h"
#include "Err_Code.h"
#include "Game_Typedef.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Pool_Manager.h"
#include "Status_Func.h"
#include "Status_Struct.h"
#include "Status.h"
#include "Fighter_Detail.h"
#include "fighter/Fighter.h"
#include "fighter/Fighter_Func.h"
#include "fighter/Fighter_Struct.h"
#include "Scene_Player.h"
#include "Scene_Monitor.h"
#include "Battle_Scene.h"
#include "Scene_Public.h"
#include "NPC/hero/Hero.h"
#include "Utility_Func.h"
#include "Config_Cache_Scene.h"
#include "Move_Scene.h"
#include "Base_Detail.h"
#include "NPC/NPC_Manager.h"

Status::Status(void) {
	status_detail_ = 0;
	buff_insert_handle_switch_ = false;
}

Status::~Status(void) {}

void Status::load_detail(void) {
	if (status_detail_)
		return;
	status_detail_ = POOL_MANAGER->pop_status_detail_pool();
}

int Status::sign_in(void) {
	Time_Value now = Time_Value::gettimeofday();
	time_up(now);

	return 0;
}

int Status::sign_out(void) {
	// do nothing
	return 0;
}

void Status::reset(void) {
	if (0 != status_detail_) {
		POOL_MANAGER->push_status_detail_pool(status_detail_);
	}
	status_detail_ = 0;
	buff_insert_handle_switch_ = false;
}

int Status::client_scene_complete(void) {
	//check
	MSG_81000071 msg_fight;
	msg_fight.type = SFT_FIGHT;
	MSG_81000071 msg_scene;
	msg_scene.type = SFT_SCENE;

	int scene_id = fighter()->move_scene_id();
	Status_Info_Msg msg_info;
	Status_Info* status_info = 0;
	for (Status_Detail::Status_Map::iterator status_it = status_detail_->status_map.begin();
			status_it != status_detail_->status_map.end(); ++status_it) {
		status_info = status_it->second;
		if (!status_info->scene_visible.empty() && !check_int_in_vec(scene_id, status_info->scene_visible)) {
			continue;
		}

		msg_info.reset();
		msg_info.status_owner = role_id();
		msg_info.status_id = status_info->status_id_;
		msg_info.overlay = status_info->overlay_;
		msg_info.last_time_ms = status_info->last_time_ms;
		msg_info.remain_time = status_info->last_end_time.sec() - Time_Value::gettimeofday().sec();
		if (msg_info.remain_time < 0) {
			msg_info.remain_time = 0;
		}
		make_buff_msg(status_info, msg_info);
		if (status_info->is_fight_buff == SFT_FIGHT) {
			msg_fight.buff_detail.push_back(msg_info);
		} else {
			msg_scene.buff_detail.push_back(msg_info);
		}
	}
	if (!msg_fight.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_fight, buf);
		call_broadcast_around(buf);
	}
	if (!msg_scene.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_scene, buf);
		call_broadcast_around(buf);
	}
	return 0;
}

int Status::daily_refresh(void) {
//	const All_Status_Config_Cache &cache = CONFIG_CACHE_BUFF->all_status_config();
//	this->erase_status_type_in_set(cache.daily_erase_set);
	return 0;
}

const Status_Detail *Status::status_detail(void) const {
	return status_detail_;
}

int Status::time_up(const Time_Value &now) {
	if (!status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
		return -1;
	}
	// make all time out status to update
	int ret = 0;

	while (! status_detail_->timepri_list.empty()) {
		Status_Effect_Detail *status_effect = status_detail_->timepri_list.front();

		if (status_effect->next_opt > now) break;
		int status_id = status_effect->status_id;
		status_detail_->timepri_list.pop_front();

		if (status_effect->interval == Time_Value::zero) {
			continue;//
		} else {
			if (status_effect->is_hit && !this->lottery_success()) {
				if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
					return -1;
				}
				insert_status_and_sort_timepri(status_effect, status_detail_->timepri_list);
				continue;
			}

			//判断是否延迟生效
			if (Time_Value::zero != status_effect->delay_time_tick) {
				if (Time_Value::zero == status_effect->delay_time_ms) {//初始延迟生效的开始时间
					status_effect->delay_time_ms = now + status_effect->delay_time_tick;
				}
				++status_effect->delay_times;
			} else {
				ret = this->opt_status_effect_handle(*status_effect, now);
				if (0 != ret || !status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
					return -1;
				}
				if (!has_status_id(status_id)) {
					continue;
				}
			}

			int count = 1;
			int ms_interval = status_effect->ms_interval();
			if (ms_interval > 0) {
				int now_msec = now.msec();
				int next_msec = status_effect->next_opt.msec();
				count = (now_msec - next_msec) / ms_interval + 1;
			}
			long ms_to_add = ms_interval * count;
			if (ms_to_add < 0)
				ms_to_add = 1800000;
			if (ms_to_add > 1080000000) {
				erase_status_type(status_effect->status_id);
				continue;
			}
			long sec_to_add = ms_to_add / 1000;
			long usec_to_add = ms_to_add % 1000  * 1000;
			Time_Value time_to_add(sec_to_add, usec_to_add);
			status_effect->next_opt = status_effect->next_opt + time_to_add;

			// 延迟生效处理
			if (Time_Value::zero != status_effect->delay_time_tick
					&& now >= status_effect->delay_time_ms) {
				ret = this->opt_status_effect_handle(*status_effect, now);
				if (0 != ret || !status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
					return -1;
				}
				if (!has_status_id(status_id)) {
					continue;
				}
				{
					int count = 1;
					int ms_interval = status_effect->delay_ms_interval();
					if (ms_interval > 0) {
						int now_msec = now.msec();
						int next_msec = status_effect->delay_time_ms.msec();
						count = (now_msec - next_msec) / ms_interval + 1;
					}
					long ms_to_add = ms_interval * count;
					if (ms_to_add < 0)
						ms_to_add = 1800000;
					if (ms_to_add > 1080000000) {
						erase_status_type(status_effect->status_id);
						MSG_USER("ERROR:delay time_tick");
						continue;
					}
					long sec_to_add = ms_to_add / 1000;
					long usec_to_add = ms_to_add % 1000  * 1000;
					Time_Value time_to_add(sec_to_add, usec_to_add);
					status_effect->delay_time_ms = status_effect->delay_time_ms + time_to_add;
				}
				--status_effect->delay_times;
			}
		}

		if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
			return -1;
		}
		if (!has_status_id(status_id)) {
			continue;
		}
		insert_status_and_sort_timepri(status_effect, status_detail_->timepri_list);
	}

	while(! status_detail_->once_timepri_list.empty()) {
		//		bool is_remove = false;
		Status_Effect_Detail *status_effect = status_detail_->once_timepri_list.front();

		if (status_effect->delay_time_ms > now) break;
		int status_id = status_effect->status_id;
		status_detail_->once_timepri_list.pop_front();

		this->insert_status_effect_handle(*status_effect);
		if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
			return -1;
		}
		if (!has_status_id(status_id)) {
			continue;
		}
		insert_status_and_sort_once_timepri(status_effect, status_detail_->once_timepri_list);
	}

	MSG_81000071 msg;
	msg.type = SFT_FIGHT;
	Status_Info_Msg msg_info;
	while (! status_detail_->status_timepri_list.empty()) {
		Status_Info *status_info = status_detail_->status_timepri_list.front();
		if (status_info->last_end_time > now) break;
		status_detail_->status_timepri_list.pop_front();

		bool is_switch = false;
		for (Status_Info::Status_Info_Map::const_iterator it_sim = status_info->status_info_map.begin();
				it_sim != status_info->status_info_map.end(); ++it_sim) {
			for (Status_Info_Detail::Effect_Vec::const_iterator it_em = it_sim->second.effect_map.begin();
					it_em != it_sim->second.effect_map.end(); ++it_em) {
				if (it_em->effect_type == Status_Def::STATUS_ET_SWITCH) {
					is_switch = true;
					break;
				}
			}
			if (is_switch) {
				break;
			}
		}

		msg_info.reset();
		msg_info.status_owner = role_id();
		msg_info.status_id = status_info->status_id_;
		msg_info.overlay = 0;//status_info->overlay_;
		if (is_switch) {
			msg_info.source_id = status_info->other_id;
		}
		msg.buff_detail.push_back(msg_info);
		this->erase_status_type(status_info->status_id_);
	}
	if (!msg.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		call_broadcast_around(buf);
	}

	msg.reset();
	msg.type = SFT_SCENE;
	while (! status_detail_->out_status_timepri_list.empty()) {
		Status_Info *status_info = status_detail_->out_status_timepri_list.front();
		if (status_info->last_end_time > now) break;
		status_detail_->out_status_timepri_list.pop_front();

		msg_info.reset();
		msg_info.status_owner = role_id();
		msg_info.status_id = status_info->status_id_;
		msg_info.overlay = 0;//status_info->overlay_;
		msg.buff_detail.push_back(msg_info);
		this->erase_status_type(status_info->status_id_);
	}
	if (!msg.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		call_broadcast_around(buf);

		if (this->hero_self() && this->hero_self()->master()) {
			Scene_Player *sp = this->hero_self()->master();
			if (sp) {
				Block_Buffer buf;
				buf.make_message(INNER_SYNC_STATUS_HERO);
				buf.write_int8(0); // del status
				buf.write_uint16(msg.buff_detail.size());
				for (std::vector<Status_Info_Msg>::const_iterator it = msg.buff_detail.begin();
						it != msg.buff_detail.end(); ++it) {
					buf.write_int32(it->status_id);
				}
				buf.finish_message();
				sp->send_to_logic(buf);
			}
		}
	}

	return 0;
}

void Status::fight_ation_tick(Role_Vec &vec) {
	if (!status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
		return;

	Time_Value now = Time_Value::gettimeofday();
	// check and debuff
	for (Status_Detail::Status_Effect_List::iterator it = this->status_detail_->action_list.begin();
			it != this->status_detail_->action_list.end(); ++it) {
		int status_id = (**it).status_id;
		if ((**it).action_tick
				<= (**it).action_times) { //action
			if (!this->lottery_success() && (**it).is_hit) {
				(**it).action_times = 0;
				continue;
			}
			if (0 != (**it).delay_action_tick) {
				++(**it).delay_times;
			} else {
				{
					Role_Vec role_vec;
					for (Role_Vec::iterator pos_it = vec.begin(); pos_it != vec.end(); ++pos_it) {
						if (fighter()->battle_scene()) {
							Mover *mover = fighter()->battle_scene() ->find_mover_with_role_id(*pos_it);
							if (!mover || !mover->fighter() || mover->fighter()->dead_status()) continue;
							role_vec.push_back(mover->role_id());
						}
					}
					opt_status_effect_handle(**it, now, 0, role_vec);
					if (status_detail_ == 0 || status_detail_->status_map.empty() || this->fighter()->dead_status())
						return;
					if (!has_status_id(status_id)) {
						break;
					}
				}
			}
			(**it).action_times = 0;
		} else {
			(**it).action_times ++;
		}

		/* 延迟生效
		 * 不另外一个列表，直接当前列表
		 * */
		if (0 != (**it).delay_action_tick) {
			if ((**it).delay_action_tick <= (**it).delay_action_times + 1) { // +1 有算当前次
				--(**it).delay_times;
				{
					Role_Vec role_vec;
					for (Role_Vec::iterator pos_it = vec.begin(); pos_it != vec.end(); ++pos_it) {
						if (fighter()->battle_scene()) {
							Mover *mover = fighter()->battle_scene() ->find_mover_with_role_id(*pos_it);
							if (!mover || !mover->fighter() || mover->fighter()->dead_status()) continue;
							role_vec.push_back(mover->role_id());
						}
					}
					opt_status_effect_handle(**it, now, 0, role_vec);
					if (status_detail_ == 0 || status_detail_->status_map.empty() || this->fighter()->dead_status())
						return;
					if (!has_status_id(status_id)) {
						break;
					}
				}
			} else {
				++(**it).delay_action_times;
			}
		}
	}

	MSG_81000071 msg;//理论上场景buff不会配到这边来
	msg.type = SFT_FIGHT;
	Status_Info_Msg msg_info;
	Status_Info *status_info = NULL;
	Status_Detail::Status_Info_List erase_list;
	for (Status_Detail::Status_Info_List::iterator it = status_detail_->status_action_list.begin();
			it != status_detail_->status_action_list.end(); ++it) {
		status_info = *it;
		++status_info->action_times;
		if (status_info->cfg_action_times <= status_info->action_times) {
			erase_list.push_back(*it);
		}
	}
	for (Status_Detail::Status_Info_List::iterator it = erase_list.begin();
			it != erase_list.end(); ++it) {
		msg_info.reset();
		msg_info.status_id = (*it)->status_id_;
		msg_info.overlay = 0;
		msg_info.last_time_ms = (*it)->last_time_ms;
		msg_info.status_owner = role_id();
		msg.buff_detail.push_back(msg_info);
		status_detail_->status_action_list.remove(*it);
		this->erase_status_type((*it)->status_id_);
	}
	if (!msg.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		call_broadcast_around(buf);
	}

	return ;
}

void Status::be_hit_tick(const Attack_Param &attack_param) {
	if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
		return ;
	}

	Time_Value now = Time_Value::gettimeofday();
	// check and debuff
	for (Status_Detail::Status_Effect_List::iterator it = this->status_detail_->passive_list.begin();
			it != this->status_detail_->passive_list.end(); ++it) {
		int status_id = (**it).status_id;
		if ((**it).passive_tick
				<= (**it).passive_times) { //action
			if (!this->lottery_success() && (**it).is_hit) {
				(**it).passive_times = 0;
				continue;
			}
			if (0 != (**it).delay_passive_tick) {
				++(**it).delay_times;
			} else {
				Role_Vec role_vec;
				if (attack_param.attacker > 0) {
					role_vec.push_back(attack_param.attacker);
				}
				opt_status_effect_handle(**it, now, attack_param.skill_id, role_vec);
				if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
					return;
				if (!has_status_id(status_id)) {
					break;
				}
			}
			(**it).passive_times = 0;
		} else {
			(**it).passive_times ++;
		}
		/* 延迟生效
		 * 不另外一个列表，直接当前列表
		 * */
		if (0 != (**it).delay_passive_tick) {
			if ((**it).delay_passive_tick <= (**it).delay_passive_times + 1) { // +1 有算当前次
				--(**it).delay_times;
				Role_Vec role_vec;
				if (attack_param.attacker > 0) {
					role_vec.push_back(attack_param.attacker);
				}
				opt_status_effect_handle(**it, now, attack_param.skill_id, role_vec);
				if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status())
					return;
				if (!has_status_id(status_id)) {
					break;
				}
			} else {
				++(**it).delay_passive_times;
			}
		}
	}

	MSG_81000071 msg;//理论上场景buff不会配到这边来
	msg.type = SFT_FIGHT;
	Status_Info_Msg msg_info;
	Status_Info *status_info = NULL;
	Status_Detail::Status_Info_List erase_list;
	for (Status_Detail::Status_Info_List::iterator it = status_detail_->status_passive_list.begin();
			it != status_detail_->status_passive_list.end(); ++it) {
		status_info = *it;
		++status_info->trigger_times;
		if (status_info->cfg_trigger_times <= status_info->trigger_times) {
			erase_list.push_back(*it);
		}
	}
	for (Status_Detail::Status_Info_List::iterator it = erase_list.begin();
			it != erase_list.end(); ++it) {
		msg_info.reset();
		msg_info.status_id = (*it)->status_id_;
		msg_info.overlay = 0;
		msg_info.last_time_ms = (*it)->last_time_ms;
		msg_info.status_owner = role_id();
		msg.buff_detail.push_back(msg_info);
		status_detail_->status_passive_list.remove(*it);
		this->erase_status_type((*it)->status_id_);
	}
	if (!msg.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		call_broadcast_around(buf);
	}
	return ;
}

int Status::insert_status_out_fight(Status_Effect_Argv &argv) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(argv.status_id);
	if (NULL == status_config) {
		MSG_USER_TRACE("status id %d configure file error.", argv.status_id);
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (status_config->is_fight_buff) { // 战斗buff不通过此接口插入
		return -1;
	}
	int ret = this->insert_status(argv);
	if (0 == ret && this->hero_self() && this->hero_self()->master()) {
		Scene_Player *sp = this->hero_self()->master();
		if (sp) {
			Block_Buffer buf;
			buf.make_message(INNER_SYNC_STATUS_HERO);
			buf.write_int8(1); // add status
			buf.write_uint16(1);
			argv.serialize(buf);
			buf.finish_message();
			sp->send_to_logic(buf);
		}
	}
	return ret;
}

int Status::insert_status_from_fight(Status_Effect_Detail &effect) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(effect.status_id);
	if (NULL == status_config) {
		MSG_USER_TRACE("status id %d configure file error.", effect.status_id);
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (!status_config->is_fight_buff) { // 战斗buff引发的buff通过此接口插入
		return -1;
	}
	effect.trigger_buff.caster_id = role_id();
	return this->insert_status_from_buff(effect.trigger_buff);
}

int Status::insert_status_from_buff(Status_Effect_Argv &argv) {
	argv.caster_id = role_id();
	return this->insert_status(argv);
}

int Status::insert_status(Status_Effect_Argv &argv) {
//	MSG_USER("1:role_id(%ld), argv.status_id(%d), status_detail_(%ld).", role_id(), argv.status_id, status_detail_);
	if (0 == this->status_detail_
			|| 0 == argv.status_id) {
		return 0;
	}
//	MSG_USER("2:role_id(%ld).", role_id());
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(argv.status_id);
	if (NULL == status_config) {
		MSG_USER_TRACE("status id %d configure file error.", argv.status_id);
		return ERROR_CONFIG_NOT_EXIST;
	}

//	MSG_USER("3:role_id(%ld).", role_id());
	argv.dispath();

	if (argv.data.size() != status_config->effect_map.size()
			|| !argv.check_last_time()) {
		MSG_USER("ERROR status_id:%d; argv.data.size: status_config.effect_map.size = %d : %d;", argv.status_id, argv.data.size(), status_config->effect_map.size());
		return 0;
	}

//	MSG_USER("4:role_id(%ld).", role_id());
	attribute_set_prop(argv.data);

	int ret = this->check_param(argv.status_id);
	if (0 != ret) {
		return ret;
	}

//	MSG_USER("5:role_id(%ld).", role_id());
	Status_Info detail;
	detail.other_id = argv.other_id;
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

	ret = this->insert_status(detail);
	if (0 == ret) {
		if (status_config->buff_charactor == TYPE_CHARACTER_BAD) {
			if (player_self()) {
				be_skill_trigger_active_skill(*player_self(), TYPE_NEG_BUFF);
			} else if (npc_self()) {
				be_skill_trigger_active_skill(*npc_self(), TYPE_NEG_BUFF);
			}
		}
//		MSG_USER("success: role_id(%ld).", role_id());
		this->sync_status_info();
	}
	return ret;
}

int Status::insert_status(Status_Info &detail) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(detail.status_id_);
	if (NULL == status_config) {
		MSG_USER_TRACE("status id %d configure file error.", detail.status_id_);
		return ERROR_CONFIG_NOT_EXIST;
	}

	//check
	MSG_81000071 msg;
	msg.type = (int)detail.is_fight_buff;
//	bool need_broadcast = true; // 是否需要广播给客户端
	bool need_pop = false; // 是否需要移除新旧状态队列
	bool need_hook_overlay = false;// 是否需要监听层数变化
	Time_Value now = Time_Value::gettimeofday();

	Status_Detail::Status_Map::iterator status_it = status_detail_->status_map.find(detail.status_id_);
	if (status_it == status_detail_->status_map.end()) {
		int ret = insert_new_status(detail, status_config);
		if (0 != ret || 0 == this->status_detail_) {
			return 0;
		}
		status_it = status_detail_->status_map.find(detail.status_id_);
		if (status_it == status_detail_->status_map.end()) {
			return ERROR_INNER_SERVER;
		}
		need_hook_overlay = true;
	} else {
		Status_Info *status_info = status_it->second;
		if (status_config->overlay > 1) {// 可叠加且叠加数大于1
			if (status_config->overlay == status_info->overlay_) {
				need_pop = true;
			}
			int ret = merge_status_to_status(*status_info, detail, status_config);
			if (0 != ret) {
				return ret;
			}
			need_hook_overlay = true;
		} else {// 不可叠加
//			need_broadcast = false;
			need_pop = true;
			int ret = cover_status_to_status(*status_info, detail);
			if (0 != ret) {
				return ret;
			}
		}
	}
	status_it = status_detail_->status_map.find(detail.status_id_);
	if (status_it == status_detail_->status_map.end()) {
		return -1;
	}
	if (need_hook_overlay) {
		this->hook_status_overlay(status_it->second->status_id_, status_it->second->overlay_);
	}

	if (need_pop) {
		status_it->second->status_fifo.pop_front();
	}
	status_it->second->status_fifo.push_back(detail.last_caster_role_id_);

	if (status_it->second->scene_visible.empty() || check_int_in_vec(fighter()->move_scene_id(), status_it->second->scene_visible)) {
		Status_Info_Msg msginfo;
		msginfo.status_owner = role_id();
		msginfo.status_id = detail.status_id_;
		msginfo.overlay = status_it->second->overlay_;
		msginfo.last_time_ms = status_it->second->last_time_ms;
		msginfo.remain_time = status_it->second->last_end_time.sec() - now.sec();
		if (msginfo.remain_time < 0) {
			msginfo.remain_time = 0;
		}
		{
			for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_it->second->status_info_map.begin();
						status_info_map_it != status_it->second->status_info_map.end(); ++status_info_map_it) {
				for (Status_Info_Detail::Effect_Vec::iterator it = status_info_map_it->second.effect_map.begin();
						it != status_info_map_it->second.effect_map.end(); ++it) {
					if (it->effect_type == Status_Def::STATUS_ET_CHANGE_FASHION) {
						msginfo.source_id = it->i_effect_value1;
						break;
					}
				}
			}
		}
		make_buff_msg(status_it->second, msginfo);
		msg.buff_detail.push_back(msginfo);

		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		call_broadcast_around(buf);
	}
	return 0;
}

int Status::insert_status_to_hero(Status_Effect_Argv &argv) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_STATUS_HERO);
	buf.write_int8(1); // add status
	buf.write_uint16(1);
	argv.serialize(buf);
	buf.finish_message();
	send_to_logic(buf);
	return 0;
}

int Status::del_status_to_hero(Int_Hash_Set &ihs) {
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_STATUS_HERO);
	buf.write_int8(0); // del status
	buf.write_uint16(ihs.size());
	for (Int_Hash_Set::const_iterator it = ihs.begin();
			it != ihs.end(); ++it) {
		buf.write_int32(*it);
	}
	buf.finish_message();
	send_to_logic(buf);
	return 0;
}

int Status::del_status_to_hero(const int id) {
	Int_Hash_Set ihs;
	ihs.insert(id);
	del_status_to_hero(ihs);
	return 0;
}

int Status::make_buff_msg(const Status_Info *status_info, Status_Info_Msg &msg_info) {
	if (!status_info) {
		return -1;
	}
	for (Status_Info::Status_Info_Map::const_iterator status_info_it = status_info->status_info_map.begin();
			status_info_it != status_info->status_info_map.end(); ++status_info_it) {
		const Status_Info_Detail &info = status_info_it->second;
		size_t i = 0;
		for (Status_Info_Detail::Effect_Vec::const_iterator effect_it = info.effect_map.begin();
				effect_it != info.effect_map.end(); ++effect_it) {
			const Status_Effect_Detail &effect = *effect_it;
			switch (effect.effect_type) {
			case Status_Def::STATUS_EFFECT_TYPE_ATTR:
				if (effect.i_effect_value1 == Property_Type::PT_MORALE_CURRENT
						|| effect.i_effect_value1 == Property_Type::PT_SPEED) {
					if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
						msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
					} else {
						msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
					}
				} else {
					if (effect.d_effect_value1 != 0) {
						if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
							msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
						} else {
							msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
						}
//						MSG_USER("role(%ld): effect_id(%d) effect.d_effect_value1(%lf) * effect.overlay(%d), prop[i].value(%lf)", role_id(), effect.status_id, effect.d_effect_value1 , effect.overlay, prop[i].value);
					} else {
						if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
							msg_info.prop[i].value += effect.d_effect_value2 * effect.overlay;
						} else {
							msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value2 * effect.overlay));
						}
					}
				}
				break;
			case Status_Def::STATUS_ET_ATTR_WITH_HERO:
				if (effect.i_effect_value1 == Property_Type::PT_MORALE_CURRENT
						|| effect.i_effect_value1 == Property_Type::PT_SPEED) {
					if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
						msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
					} else {
						msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
					}
				} else {
					if (effect.d_effect_value1 != 0) {
						if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
							msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
						} else {
							msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
						}
					} else {
						if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
							msg_info.prop[i].value += effect.d_effect_value2 * effect.overlay;
						} else {
							msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value2 * effect.overlay));
						}
					}
				}
				break;
			case Status_Def::STATUS_EFFECT_TYPE_POISON:
				if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
					msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
				} else {
					msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
				}
				break;
			case Status_Def::STATUS_EFFECT_TYPE_T_ATTR: {
				if (effect.d_effect_value1 != 0) {
					if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
						msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
					} else {
						msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
					}
				} else {
					if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
						msg_info.prop[i].value += effect.d_effect_value2 * effect.overlay;
					} else {
						msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value2 * effect.overlay));
					}
				}
				break;
			}
			case Status_Def::STATUS_EFFECT_TYPE_CURE_BY_PROP: {
				double cure_rate_self = fighter()->fighter_detail().fetch_fight_property(PT_BE_CURE_RATE);
				if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
					msg_info.prop[i].value += effect.last_cure_value * effect.overlay * (1 + effect.last_cure_rate + cure_rate_self);
				} else {
					msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.last_cure_value * effect.overlay * (1 + effect.last_cure_rate + cure_rate_self)));
				}
				break;
			}
			case Status_Def::STATUS_EFFECT_TYPE_SHIELD: {
				double shield_cfg = fighter()->fighter_detail().fetch_fight_property((Property_Type)effect.i_effect_value1);
				shield_cfg *= effect.d_effect_value2;
				shield_cfg += effect.d_effect_value4;
				if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
					msg_info.prop[i].value += shield_cfg;
				} else {
					msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, shield_cfg));
				}
				break;
			}
			case Status_Def::STATUS_EFFECT_TYPE_EXP:
			case Status_Def::STATUS_ET_EXP_TWO:
			case Status_Def::STATUS_ET_EXP_ACTIVITY: {
				if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
					msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
				} else {
					msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
				}
				break;
			}
			case Status_Def::STATUS_EFFECT_TYPE_BLOOD_BY_PER: {
				if (effect.d_effect_value1 != 0) {
					if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
						msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
					} else {
						msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
					}
				} else {
					if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
						msg_info.prop[i].value += effect.d_effect_value2 * effect.overlay;
					} else {
						msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value2 * effect.overlay));
					}
				}
				break;
			}
			case Status_Def::STATUS_ET_REBOUND: {
				if (!msg_info.prop.empty() && msg_info.prop.size() > i) {
					msg_info.prop[i].value += effect.d_effect_value1 * effect.overlay;
				} else {
					msg_info.prop.push_back(Property((Property_Type)effect.i_effect_value1, effect.d_effect_value1 * effect.overlay));
				}
				break;
			}
			case Status_Def::STATUS_ET_SWITCH: {
				msg_info.source_id = status_info->other_id;
				break;
			}
			default:
				break;
			}
			++i;
		}
		break;
	}
	return 0;
}

int Status::status_hook_enter_battle_scene(void) {
	for (Status_Detail::Status_Map::iterator status_it = status_detail_->status_map.begin();
			status_it != status_detail_->status_map.end(); ++status_it) {
		if (!status_it->second->is_fight_buff) {
			Status_Info *status_info = status_it->second;
			for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info->status_info_map.begin();
					status_info_map_it != status_info->status_info_map.end(); ++status_info_map_it) {
				for (Status_Info_Detail::Effect_Vec::iterator effect_map_it = status_info_map_it->second.effect_map.begin();
						effect_map_it != status_info_map_it->second.effect_map.end(); ++effect_map_it) {
					if (effect_map_it->trigger_buff.status_id > 0 && effect_map_it->trigger_buff.is_fight_buff) {
						this->insert_status_from_buff(effect_map_it->trigger_buff);
					}
				}
			}
		}
	}
	special_status_handle();
	return 0;
}

void Status::erase_status_type_for_msg(const int status_id) {
	MSG_81000071 msg_fight;
	msg_fight.type = SFT_FIGHT;
	MSG_81000071 msg_scene;
	msg_scene.type = SFT_SCENE;
	Status_Info_Msg msg_info;

	Status_Detail::Status_Map::iterator it = status_detail_->status_map.find(status_id);
	if (it != status_detail_->status_map.end()) {
		if (it->second->is_fight_buff == SFT_FIGHT) {
			bool is_switch = false;
			for (Status_Info::Status_Info_Map::const_iterator it_sim = (*it).second->status_info_map.begin();
					it_sim != (*it).second->status_info_map.end(); ++it_sim) {
				for (Status_Info_Detail::Effect_Vec::const_iterator it_em = it_sim->second.effect_map.begin();
						it_em != it_sim->second.effect_map.end(); ++it_em) {
					if (it_em->effect_type == Status_Def::STATUS_ET_SWITCH) {
						is_switch = true;
						break;
					}
				}
				if (is_switch) {
					break;
				}
			}

			msg_info.reset();
			msg_info.status_id = it->first;
			msg_info.overlay = 0;
			msg_info.last_time_ms = it->second->last_time_ms;
			msg_info.status_owner = role_id();
			if (is_switch) {
				msg_info.source_id = (*it).second->other_id;
			}
			msg_fight.buff_detail.push_back(msg_info);
		} else {
			msg_info.reset();
			msg_info.status_id = it->first;
			msg_info.overlay = 0;
			msg_info.last_time_ms = it->second->last_time_ms;
			msg_info.status_owner = role_id();
			msg_scene.buff_detail.push_back(msg_info);
		}
		this->erase_status_type(status_id);
	}

	if (!msg_fight.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_fight, buf);
		call_broadcast_around(buf);
	}
	if (!msg_scene.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_scene, buf);
		call_broadcast_around(buf);
	}
}

int Status::erase_status_type(int status_id) {
	Status_Detail::Status_Map::iterator it = status_detail_->status_map.find(status_id);
	if (it != this->status_detail_->status_map.end()) {
		Status_Info *status_info = it->second;
		for (Status_Info::Status_Info_Map::iterator status_info_map_it = status_info->status_info_map.begin();
				status_info_map_it != status_info->status_info_map.end(); ++status_info_map_it) {
			for (Status_Info_Detail::Effect_Vec::iterator effect_map_it = status_info_map_it->second.effect_map.begin();
					effect_map_it != status_info_map_it->second.effect_map.end(); ++effect_map_it) {
//				this->erase_status_effect_handle(status_info_map_it->second, (*effect_map_it));
				this->erase_status_effect_handle((*effect_map_it));
				status_detail_->timepri_list.remove(&(*effect_map_it));
				status_detail_->action_list.remove(&(*effect_map_it));
				status_detail_->passive_list.remove(&(*effect_map_it));
				status_detail_->once_timepri_list.remove(&(*effect_map_it));
			}
		}
		status_detail_->status_timepri_list.remove(status_info);
		status_detail_->status_action_list.remove(status_info);
		status_detail_->status_passive_list.remove(status_info);
		status_detail_->out_status_timepri_list.remove(status_info);
		status_detail_->out_status_action_list.remove(status_info);
		status_detail_->out_status_passive_list.remove(status_info);
		status_detail_->status_dispel_set.erase(status_id);
		status_detail_->status_pure_set.erase(status_id);
		status_detail_->status_stoic_set.erase(status_id);
		status_detail_->status_shield_set.erase(status_id);
		status_detail_->status_evil_set.erase(status_id);
		status_detail_->status_map.erase(status_id);
		POOL_MANAGER->push_status_info_pool(status_info);
		this->sync_status_info();
		return 0;
	}

	return -1;
}

void Status::erase_status_in_set(const Int_Hash_Set &set) {
	MSG_81000071 msg_fight;
	msg_fight.type = SFT_FIGHT;
	MSG_81000071 msg_scene;
	msg_scene.type = SFT_SCENE;
	Status_Info_Msg msg_info;

	Int_Vec erase_status;
	for (Status_Detail::Status_Map::iterator it = status_detail_->status_map.begin(); it != status_detail_->status_map.end(); ++it) {
		if (set.count(it->first) != 0) {
			if (it->second->is_fight_buff == SFT_FIGHT) {
				bool is_switch = false;
				for (Status_Info::Status_Info_Map::const_iterator it_sim = (*it).second->status_info_map.begin();
						it_sim != (*it).second->status_info_map.end(); ++it_sim) {
					for (Status_Info_Detail::Effect_Vec::const_iterator it_em = it_sim->second.effect_map.begin();
							it_em != it_sim->second.effect_map.end(); ++it_em) {
						if (it_em->effect_type == Status_Def::STATUS_ET_SWITCH) {
							is_switch = true;
							break;
						}
					}
					if (is_switch) {
						break;
					}
				}

				msg_info.reset();
				msg_info.status_id = it->first;
				msg_info.overlay = 0;
				msg_info.last_time_ms = it->second->last_time_ms;
				msg_info.status_owner = role_id();
				if (is_switch) {
					msg_info.source_id = (*it).second->other_id;
				}
				msg_fight.buff_detail.push_back(msg_info);
			} else {
				msg_info.reset();
				msg_info.status_id = it->first;
				msg_info.overlay = 0;
				msg_info.last_time_ms = it->second->last_time_ms;
				msg_info.status_owner = role_id();
				msg_scene.buff_detail.push_back(msg_info);
			}
			erase_status.push_back(it->first);
		}
	}

	for (Int_Vec::const_iterator it = erase_status.begin(); it != erase_status.end(); ++it) {
		this->erase_status_type(*it);
	}

	if (!msg_fight.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_fight, buf);
		call_broadcast_around(buf);
	}
	if (!msg_scene.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_scene, buf);
		call_broadcast_around(buf);
	}
}

void Status::clear_all_fighter_status(const bool is_from_die) {
	if (!status_detail_)
		return;

	MSG_81000071 msg;
	msg.type = SFT_FIGHT;
	msg.is_die = is_from_die;
	Status_Info_Msg msg_info;
	Int_Vec erase_types;
	for (Status_Detail::Status_Map::iterator it = status_detail_->status_map.begin();
			it != status_detail_->status_map.end(); ++it) {
		if (it->second->is_fight_buff) {
			bool is_switch = false;
			for (Status_Info::Status_Info_Map::const_iterator it_sim = (*it).second->status_info_map.begin();
					it_sim != (*it).second->status_info_map.end(); ++it_sim) {
				for (Status_Info_Detail::Effect_Vec::const_iterator it_em = it_sim->second.effect_map.begin();
						it_em != it_sim->second.effect_map.end(); ++it_em) {
					if (it_em->effect_type == Status_Def::STATUS_ET_SWITCH) {
						is_switch = true;
						break;
					}
				}
				if (is_switch) {
					break;
				}
			}
//			bool is_shape_sheep = false;
//			for (Int_Int_Map::iterator it_re = status_detail_->running_effect.shape_sheep_map.begin();
//					it_re != status_detail_->running_effect.shape_sheep_map.end(); ++it_re) {
//				if (it_re->second == it->first) {
//					is_shape_sheep = true;
//					break;
//				}
//			}
//			if (!is_shape_sheep) {
				msg_info.reset();
				msg_info.status_id = it->first;
				msg_info.overlay = 0;
				msg_info.last_time_ms = (*it).second->last_time_ms;
				msg_info.status_owner = role_id();
				if (is_switch) {
					msg_info.source_id = (*it).second->other_id;
				}
				msg.buff_detail.push_back(msg_info);
//			}
			erase_types.push_back(it->first);
		}
	}
	erase_types.push_back(CONFIG_CACHE_BUFF->special_level_buff().status_id);
	for (Int_Vec::iterator it = erase_types.begin(); it != erase_types.end(); ++it) {
		this->erase_status_type(*it);
	}
	if (!msg.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		call_broadcast_around(buf);
	}
}
void Status::erase_evil_status_by_rand(const int num) {
	if (num <= 0) {
		return ;
	}
	MSG_81000071 msg_fight;
	msg_fight.type = SFT_FIGHT;
	for (int i = 0; i < num; ++i) {
		if (status_detail_->status_evil_set.empty()) {
			break;
		}
		Int_Hash_Set::iterator it = status_detail_->status_evil_set.begin();
		if (it != status_detail_->status_evil_set.end()) {
			Status_Info_Msg msg_info;
			msg_info.status_id = *it;
			msg_info.overlay = 0;
			bool is_switch = false;
			Status_Detail::Status_Map::iterator it_sm = status_detail_->status_map.find(*it);
			if (it_sm != status_detail_->status_map.end()) {
				msg_info.last_time_ms = it_sm->second->last_time_ms;
				msg_info.status_owner = role_id();

				for (Status_Info::Status_Info_Map::const_iterator it_sim = it_sm->second->status_info_map.begin();
						it_sim != it_sm->second->status_info_map.end(); ++it_sim) {
					for (Status_Info_Detail::Effect_Vec::const_iterator it_em = it_sim->second.effect_map.begin();
							it_em != it_sim->second.effect_map.end(); ++it_em) {
						if (it_em->effect_type == Status_Def::STATUS_ET_SWITCH) {
							is_switch = true;
							break;
						}
					}
					if (is_switch) {
						break;
					}
				}
				if (is_switch) {
					msg_info.source_id = (*it_sm).second->other_id;
				}
			}
			msg_fight.buff_detail.push_back(msg_info);
			this->erase_status_type(*it);
		} else {
			break;
		}
	}
	if (!msg_fight.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_fight, buf);
		call_broadcast_around(buf);
	}
}

int Status::del_status_overlay(const int status_id, const int num) {
	MSG_81000071 msg_fight;
	msg_fight.type = SFT_FIGHT;
	MSG_81000071 msg_scene;
	msg_scene.type = SFT_SCENE;
	Status_Info_Msg msg_info;
	msg_info.status_id = status_id;

	int overlay = num;
	Status_Detail::Status_Map::iterator it = status_detail_->status_map.find(status_id);
	if (it != status_detail_->status_map.end()) {
		Status_Info *status_info = it->second;
		msg_info.last_time_ms = it->second->last_time_ms;
		msg_info.status_owner = role_id();
		if (status_info->overlay_ <= num) {
			msg_info.overlay = 0;
			if (status_info->is_fight_buff == SFT_FIGHT) {
				msg_fight.buff_detail.push_back(msg_info);
			} else {
				msg_scene.buff_detail.push_back(msg_info);
			}
			this->erase_status_type(it->first);
		} else {
			while (overlay > 0) {
				std::deque<role_id_t>::const_iterator status_fifo_it = status_info->status_fifo.begin();
				if (status_fifo_it != status_info->status_fifo.end()) {
					Status_Info::Status_Info_Map::iterator it_pop = status_info->status_info_map.find(*status_fifo_it);
					if (it_pop != status_info->status_info_map.end()) {
						Status_Info_Detail status_info_detail_old = it_pop->second;
						--it_pop->second.overlay;
						for (Status_Info_Detail::Effect_Vec::iterator effect_it = it_pop->second.effect_map.begin();
								effect_it != it_pop->second.effect_map.end(); ++effect_it) {
							this->update_status_effect_handle(it_pop->second, status_info_detail_old,
									*effect_it, status_info_detail_old.effect_map[(*effect_it).effect_id]);
							if (status_detail_ == 0 || status_detail_->status_map.empty() || this->fighter()->dead_status())
								return 0;
						}
						//根据层数处理
						if (it_pop->second.overlay <= 0) {
							status_info->status_info_map.erase(it_pop);
							if (status_info->status_info_map.empty()) {
								this->erase_status_type(it->first);
							}
						}
					}
					--overlay;
				} else {
					break;
				}
			}
			msg_info.overlay = status_info->overlay_ - num;
			if (status_info->is_fight_buff == SFT_FIGHT) {
				msg_fight.buff_detail.push_back(msg_info);
			} else {
				msg_scene.buff_detail.push_back(msg_info);
			}
		}
	}

	if (!msg_fight.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_fight, buf);
		call_broadcast_around(buf);
	}
	if (!msg_scene.buff_detail.empty()) {
		Block_Buffer buf;
		MAKE_MSG_BUF(msg_scene, buf);
		call_broadcast_around(buf);
	}
	return 0;
}

int Status::insert_status_effect_handle(Status_Info_Detail &status_info, Status_Effect_Detail &effect) {
	if (0 == effect.overlay
			|| !Utility_Func::lottery_success(effect.buff_chance)) {
		return 0;
	}
	if (!check_status_effect(effect)) {
		return 0;
	}

	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				//&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		if (this->player_self()) {
			Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
			if (hero) {
				hero->add_status_prop(effect);
			}
		} else if (this->hero_self() && this->hero_self()->master()) {
			Scene_Player *player = this->hero_self()->master();
			player->add_status_prop(effect);
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_POISON:
		handle_poison_trigger(*fighter(), effect);
		break;
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_DIZZ: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.stun_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.stun_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_STOIC: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.stoic_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.stoic_map[effect.i_effect_value1] = effect.status_id;
		this->handle_stoic();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_BY_PROP: {
		this->cure_status_prop(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_HURT_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.hurt_link_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.hurt_link_map[effect.i_effect_value1] = effect.status_id;
		this->add_hurt_link(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_TAUNT: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.taunt_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.taunt_map[effect.i_effect_value1] = effect.status_id;
		this->handle_taunt();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_LOCK: {
		hand_lock(effect.status_id);
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.lock_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.lock_map[effect.i_effect_value1] = effect.status_id;
		if (fighter() && fighter()->battle_scene()) {
			fighter()->battle_scene()->clear_enemy_simulate_target(*fighter());
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHIELD: {
		this->add_shield_prop(effect);
		this->status_detail_->status_shield_set.insert(effect.status_id);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_INVINCIBLE: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.invincible_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.invincible_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.cure_link_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.cure_link_map[effect.i_effect_value1] = effect.status_id;
		this->add_cure_link(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SILENT: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.silent_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.silent_map[effect.i_effect_value1] = effect.status_id;
		if (fighter()) {
			fighter()->break_use_skill();
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CONFUSION: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.confusion_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.confusion_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(status_detail_->running_effect.exp_map.count(id) != 0);
//		effect.i_effect_value1 = id;
		status_detail_->running_effect.exp_map[effect.status_id] = effect.d_effect_value1;
		if (status_detail_->running_effect.exp_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_BLOOD_BY_PER: {
		this->add_status_prop_by_per(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHAPE_SHEEP: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.shape_sheep_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.shape_sheep_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_ET_REBOUND: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.rebound_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.rebound_map[effect.i_effect_value1] = effect.d_effect_value1;
		break;
	}
	case Status_Def::STATUS_ET_POISON_BY_EXPRESSION:
		handle_poison_by_expression_trigger(*fighter(), effect);
		break;
	case Status_Def::STATUS_ET_CHANGE_FASHION: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.change_fashion_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.change_fashion_map[effect.status_id] = effect.i_effect_value1;
		break;
	}
	case Status_Def::STATUS_ET_EXP_TWO: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(status_detail_->running_effect.exp_two_map.count(id) != 0);
//		effect.i_effect_value1 = id;
		status_detail_->running_effect.exp_two_map[effect.status_id] = effect.d_effect_value1;
		if (status_detail_->running_effect.exp_two_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_ET_DIRECT_DIE: {
		// interface
		if (0 != fighter())
			fighter()->manual_set_fighter_dead();
		break;
	}
	case Status_Def::STATUS_ET_EXP_ACTIVITY: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(status_detail_->running_effect.exp_two_map.count(id) != 0);
//		effect.i_effect_value1 = id;
		status_detail_->running_effect.exp_activity_map[effect.status_id] = effect.d_effect_value1;
//		if (status_detail_->running_effect.exp_activity_map.size() > 1) {
//			MSG_TRACE_ABORT("error:status more than two exp_activity_map(%d)", effect.status_id);
//		}
		break;
	}
	default:
		break;
	}
	this->buff_use_skill_passive(effect);
	this->insert_status_from_fight(effect);
	return 0;
}

int Status::insert_status_effect_handle(Status_Effect_Detail &effect) {
	if (0 == effect.overlay
			|| !Utility_Func::lottery_success(effect.buff_chance)) {
		return 0;
	}
	if (!check_status_effect(effect)) {
		return 0;
	}

	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				//&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		if (this->player_self()) {
			Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
			if (hero) {
				hero->add_status_prop(effect);
			}
		} else if (this->hero_self() && this->hero_self()->master()) {
			Scene_Player *player = this->hero_self()->master();
			player->add_status_prop(effect);
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_POISON:
		handle_poison_trigger(*fighter(), effect);
		break;
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_DIZZ: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.stun_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.stun_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_STOIC: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.stoic_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.stoic_map[effect.i_effect_value1] = effect.status_id;
		this->handle_stoic();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_BY_PROP: {
		this->cure_status_prop(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_HURT_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.hurt_link_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.hurt_link_map[effect.i_effect_value1] = effect.status_id;
		this->add_hurt_link(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_TAUNT: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.taunt_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.taunt_map[effect.i_effect_value1] = effect.status_id;
		this->handle_taunt();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_LOCK: {
		hand_lock(effect.status_id);
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.lock_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.lock_map[effect.i_effect_value1] = effect.status_id;
		if (fighter() && fighter()->battle_scene()) {
			fighter()->battle_scene()->clear_enemy_simulate_target(*fighter());
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHIELD: {
		this->add_shield_prop(effect);
		this->status_detail_->status_shield_set.insert(effect.status_id);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_INVINCIBLE: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.invincible_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.invincible_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.cure_link_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.cure_link_map[effect.i_effect_value1] = effect.status_id;
		this->add_cure_link(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SILENT: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.silent_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.silent_map[effect.i_effect_value1] = effect.status_id;
		player_self()->break_use_skill();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CONFUSION: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.confusion_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.confusion_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.exp_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.exp_map[effect.status_id] = effect.d_effect_value1;
		if (status_detail_->running_effect.exp_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_BLOOD_BY_PER: {
		this->add_status_prop_by_per(effect);
		break;
	}
	case Status_Def::STATUS_ET_REBOUND: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.rebound_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.rebound_map[effect.i_effect_value1] = effect.d_effect_value1;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHAPE_SHEEP: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.shape_sheep_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.shape_sheep_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_ET_POISON_BY_EXPRESSION:
		handle_poison_trigger(*fighter(), effect);
		break;
	case Status_Def::STATUS_ET_CHANGE_FASHION: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.change_fashion_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.change_fashion_map[effect.status_id] = effect.i_effect_value1;
		break;
	}
	case Status_Def::STATUS_ET_EXP_TWO: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.exp_two_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.exp_two_map[effect.status_id] = effect.d_effect_value1;
		if (status_detail_->running_effect.exp_two_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_ET_DIRECT_DIE: {
		// interface
		if (0 != fighter())
			fighter()->manual_set_fighter_dead();
		break;
	}
	case Status_Def::STATUS_ET_EXP_ACTIVITY: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.exp_two_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.exp_activity_map[effect.status_id] = effect.d_effect_value1;
//		if (status_detail_->running_effect.exp_activity_map.size() > 1) {
//			MSG_TRACE_ABORT("error:status more than two exp_activity(%d)", effect.status_id);
//		}
		break;
	}
	default:
		break;
	}
	this->buff_use_skill_passive(effect);
	this->insert_status_from_fight(effect);
	return 0;
}

int Status::insert_status_effect_handle(Status_Effect_Detail &effect, const int scene) {
	if (0 == effect.overlay
			|| !Utility_Func::lottery_success(effect.buff_chance)) {
		return 0;
	}
	if (!check_status_effect(effect, scene)) {
		return 0;
	}

	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				//&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		if (this->player_self()) {
			Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
			if (hero) {
				hero->add_status_prop(effect);
			}
		} else if (this->hero_self() && this->hero_self()->master()) {
			Scene_Player *player = this->hero_self()->master();
			player->add_status_prop(effect);
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_POISON:
		handle_poison_trigger(*fighter(), effect);
		break;
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_DIZZ: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.stun_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.stun_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_STOIC: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.stoic_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.stoic_map[effect.i_effect_value1] = effect.status_id;
		this->handle_stoic();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_BY_PROP: {
		this->cure_status_prop(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_HURT_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.hurt_link_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.hurt_link_map[effect.i_effect_value1] = effect.status_id;
		this->add_hurt_link(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_TAUNT: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.taunt_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.taunt_map[effect.i_effect_value1] = effect.status_id;
		this->handle_taunt();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_LOCK: {
		hand_lock(effect.status_id);
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.lock_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.lock_map[effect.i_effect_value1] = effect.status_id;
		if (fighter() && fighter()->battle_scene()) {
			fighter()->battle_scene()->clear_enemy_simulate_target(*fighter());
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHIELD: {
		this->add_shield_prop(effect);
		this->status_detail_->status_shield_set.insert(effect.status_id);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_INVINCIBLE: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.invincible_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.invincible_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.cure_link_map.count(id) != 0);
		effect.i_effect_value1 = id;
		status_detail_->running_effect.cure_link_map[effect.i_effect_value1] = effect.status_id;
		this->add_cure_link(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SILENT: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.silent_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.silent_map[effect.i_effect_value1] = effect.status_id;
		player_self()->break_use_skill();
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CONFUSION: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.confusion_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.confusion_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.exp_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.exp_map[effect.status_id] = effect.d_effect_value1;
		if (status_detail_->running_effect.exp_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_BLOOD_BY_PER: {
		this->add_status_prop_by_per(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHAPE_SHEEP: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.shape_sheep_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.shape_sheep_map[effect.i_effect_value1] = effect.status_id;
		break;
	}
	case Status_Def::STATUS_ET_REBOUND: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.rebound_map.count(stun_id) != 0);
		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.rebound_map[effect.i_effect_value1] = effect.d_effect_value1;
		break;
	}
	case Status_Def::STATUS_ET_POISON_BY_EXPRESSION:
		handle_poison_trigger(*fighter(), effect);
		break;
	case Status_Def::STATUS_ET_CHANGE_FASHION: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.change_fashion_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.change_fashion_map[effect.status_id] = effect.i_effect_value1;
		break;
	}
	case Status_Def::STATUS_ET_EXP_TWO: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.exp_two_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.exp_two_map[effect.status_id] = effect.d_effect_value1;
		if (status_detail_->running_effect.exp_two_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_ET_DIRECT_DIE: {
		// interface
		if (0 != fighter())
			fighter()->manual_set_fighter_dead();
		break;
	}
	case Status_Def::STATUS_ET_EXP_ACTIVITY: {
//		uint16_t stun_id = 0;
//		do {
//			++stun_id;
//		} while(status_detail_->running_effect.exp_two_map.count(stun_id) != 0);
//		effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.exp_activity_map[effect.status_id] = effect.d_effect_value1;
//		if (status_detail_->running_effect.exp_activity_map.size() > 1) {
//			MSG_TRACE_ABORT("error:status more than two exp_activity(%d)", effect.status_id);
//		}
		break;
	}
	default:
		break;
	}
	this->buff_use_skill_passive(effect);
	this->insert_status_from_fight(effect);
	return 0;
}

int Status::update_status_effect_handle(Status_Info_Detail &status_info, Status_Info_Detail &status_info_from, Status_Effect_Detail &new_effect,
		Status_Effect_Detail &old_effect) {
	if (0 == old_effect.overlay
			 || !Utility_Func::lottery_success(new_effect.buff_chance)) {
		return this->insert_status_effect_handle(status_info, new_effect);
	}
	if (0 == new_effect.overlay) {
//		return this->erase_status_effect_handle(status_info_from, old_effect);
		return this->erase_status_effect_handle(old_effect);
	}
	if (!check_status_effect(new_effect)) {
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
		this->cover_status_prop(new_effect, old_effect);
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
		this->cover_status_prop(new_effect, old_effect);
		if (new_effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& new_effect.i_effect_value1 != PT_MORALE_CURRENT
				&& new_effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& new_effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			new_effect.effect_times = old_effect.effect_times + (new_effect.overlay - old_effect.overlay);
		}
		if (this->player_self()) {
			Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
			if (hero) {
				hero->cover_status_prop(new_effect, old_effect);
			}
		} else if (this->hero_self() && this->hero_self()->master()) {
			Scene_Player *player = this->hero_self()->master();
			player->cover_status_prop(new_effect, old_effect);
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_POISON:
		break;
	case Status_Def::STATUS_EFFECT_TYPE_DIZZ: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.stun_map.count(stun_id) != 0);
		new_effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.stun_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.stun_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_STOIC: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.stoic_map.count(id) != 0);
		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.stoic_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.stoic_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_HURT_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.hurt_link_map.count(id) != 0);
		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.hurt_link_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.hurt_link_map.erase(old_effect.i_effect_value1);
		this->cover_hurt_link(new_effect, old_effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_TAUNT: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.taunt_map.count(stun_id) != 0);
		new_effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.taunt_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.taunt_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_LOCK: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.lock_map.count(stun_id) != 0);
		new_effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.lock_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.lock_map.erase(old_effect.i_effect_value1);
		if (fighter() && fighter()->battle_scene()) {
			fighter()->battle_scene()->clear_enemy_simulate_target(*fighter());
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHIELD: {
		this->cover_shield_prop(new_effect, old_effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_INVINCIBLE: {
		uint16_t stun_id = 0;
		do {
			++stun_id;
		} while(status_detail_->running_effect.invincible_map.count(stun_id) != 0);
		new_effect.i_effect_value1 = stun_id;
		status_detail_->running_effect.invincible_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.invincible_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_LINK: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.cure_link_map.count(id) != 0);
		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.cure_link_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.cure_link_map.erase(old_effect.i_effect_value1);
		this->cover_cure_link(new_effect, old_effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SILENT: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.silent_map.count(id) != 0);
		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.silent_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.silent_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CONFUSION: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.confusion_map.count(id) != 0);
		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.confusion_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.confusion_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
		status_detail_->running_effect.exp_map.erase(old_effect.status_id);
		status_detail_->running_effect.exp_map[new_effect.status_id] = new_effect.d_effect_value1;
		if (status_detail_->running_effect.exp_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", new_effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHAPE_SHEEP: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.shape_sheep_map.count(id) != 0);
		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.shape_sheep_map[new_effect.i_effect_value1] = new_effect.status_id;
		status_detail_->running_effect.shape_sheep_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_ET_REBOUND: {
		uint16_t id = 0;
		do {
			++id;
		} while(status_detail_->running_effect.rebound_map.count(id) != 0);
		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.rebound_map[new_effect.i_effect_value1] = new_effect.d_effect_value1;
		status_detail_->running_effect.rebound_map.erase(old_effect.i_effect_value1);
		break;
	}
	case Status_Def::STATUS_ET_CHANGE_FASHION: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(status_detail_->running_effect.change_fashion_map.count(id) != 0);
//		new_effect.i_effect_value1 = id;
		status_detail_->running_effect.change_fashion_map.erase(old_effect.status_id);
		status_detail_->running_effect.change_fashion_map[new_effect.status_id] = new_effect.i_effect_value1;
		break;
	}
	case Status_Def::STATUS_ET_EXP_TWO: {
//		uint16_t id = 0;
//		do {
//			++id;
//		} while(status_detail_->running_effect.exp_two_map.count(id) != 0);
//		new_effect.i_effect_value1 = id;
//		status_detail_->running_effect.exp_two_map[new_effect.i_effect_value1] = new_effect.d_effect_value1;
//		status_detail_->running_effect.exp_two_map.erase(old_effect.i_effect_value1);

		status_detail_->running_effect.exp_two_map.erase(old_effect.status_id);
		status_detail_->running_effect.exp_two_map[new_effect.status_id] = new_effect.d_effect_value1;
		if (status_detail_->running_effect.exp_two_map.size() > 1) {
			MSG_TRACE_ABORT("error:status more than two exp_two(%d)", new_effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_ET_EXP_ACTIVITY: {
		status_detail_->running_effect.exp_activity_map.erase(old_effect.status_id);
		status_detail_->running_effect.exp_activity_map[new_effect.status_id] = new_effect.d_effect_value1;
//		if (status_detail_->running_effect.exp_activity_map.size() > 1) {
//			MSG_TRACE_ABORT("error:status more than two exp_activity_map(%d)", new_effect.status_id);
//		}
		break;
	}
	default:
		break;
	}
	return 0;
}

int Status::opt_status_effect_handle(Status_Effect_Detail &effect, const Time_Value &now_time, int skill_id, Role_Vec role_vec) {
	if (0 == effect.overlay
			 || !Utility_Func::lottery_success(effect.buff_chance)) {
		return 0;
	}
	if (!check_status_effect(effect)) {
		return 0;
	}

	if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
		return -1;
	}
	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR: {
		this->add_status_prop(effect, skill_id);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				//&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	}
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		this->add_status_prop(effect, skill_id);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		if (this->player_self()) {
			Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
			if (hero) {
				hero->add_status_prop(effect, skill_id);
			}
		} else if (this->hero_self() && this->hero_self()->master()) {
			Scene_Player *player = this->hero_self()->master();
			player->add_status_prop(effect, skill_id);
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_POISON:
		handle_poison_trigger(*fighter(), effect, skill_id);
		break;
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		this->add_status_prop(effect);
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			effect.effect_times += effect.overlay;
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_CURE_BY_PROP: {
		this->cure_status_prop(effect, skill_id);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_BLOOD_BY_PER: {
		this->add_status_prop_by_per(effect);
		break;
	}
	case Status_Def::STATUS_ET_POISON_BY_EXPRESSION:
		handle_poison_by_expression_trigger(*fighter(), effect, skill_id);
		break;
	case Status_Def::STATUS_ET_DIRECT_DIE: {
		// interface
		if (0 != fighter())
			fighter()->manual_set_fighter_dead();
		break;
	}
	default:
		break;
	}
	if (0 == status_detail_ || status_detail_->status_map.empty() || this->fighter()->dead_status()) {
		return -1;
	}
	const Btl_Buff_Effect *cfg = CONFIG_CACHE_BUFF->btl_effect(effect.effect_id);
	if (!cfg) return 0;

//	if (cfg->buff_direct == 4) {
//		role_vec.clear();
//		role_vec.push_back(effect.caster_id);
//	}
	this->buff_use_skill(effect, role_vec);
	this->insert_status_from_fight(effect);
	return 0;
}

int Status::erase_status_effect_handle(Status_Effect_Detail &effect) {
	switch (effect.effect_type) {
	case Status_Def::STATUS_EFFECT_TYPE_ATTR:
	case Status_Def::STATUS_EFFECT_TYPE_T_ATTR:
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			this->sub_status_prop(effect);
		}
		break;
	case Status_Def::STATUS_ET_ATTR_WITH_HERO:
		if (effect.i_effect_value1 != PT_BLOOD_CURRENT
				&& effect.i_effect_value1 != PT_MORALE_CURRENT
				&& effect.i_effect_value1 != PT_STRENGTH_CURRENT
				&& effect.i_effect_value1 != PT_BLOOD_POOL_CURRENT) {
			this->sub_status_prop(effect);
			if (this->player_self()) {
				Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
				if (hero) {
					hero->sub_status_prop(effect);
				}
			}
		}
		break;
	case Status_Def::STATUS_EFFECT_TYPE_DIZZ: {
		if (0 == status_detail_->running_effect.stun_map.erase(effect.i_effect_value1)) {
			MSG_USER("stun_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_STOIC: {
		if (0 == status_detail_->running_effect.stoic_map.erase(effect.i_effect_value1)) {
			MSG_USER("stoic_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_HURT_LINK: {
		if (0 == status_detail_->running_effect.hurt_link_map.erase(effect.i_effect_value1)) {
			MSG_USER("hurt_link_map has no %d", effect.i_effect_value1);
		} else {
			this->sub_hurt_link(effect);
		}
		if (status_detail_->running_effect.hurt_link_map.empty()) {
			this->status_detail_->hurt_link.clear();
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_TAUNT: {
		if (0 == status_detail_->running_effect.taunt_map.erase(effect.i_effect_value1)) {
			MSG_USER("stun_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_LOCK: {
		if (0 == status_detail_->running_effect.lock_map.erase(effect.i_effect_value1)) {
			MSG_USER("stun_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHIELD: {
		this->sub_shield_prop(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_INVINCIBLE: {
		if (0 == status_detail_->running_effect.invincible_map.erase(effect.i_effect_value1)) {
			MSG_USER("stun_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CURE_LINK: {
		if (0 == status_detail_->running_effect.cure_link_map.erase(effect.i_effect_value1)) {
			MSG_USER("cure_link_map has no %d", effect.i_effect_value1);
		} else {
			this->sub_cure_link(effect);
		}
		if (status_detail_->running_effect.cure_link_map.empty()) {
			this->status_detail_->cure_link.clear();
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SILENT: {
		if (0 == status_detail_->running_effect.silent_map.erase(effect.i_effect_value1)) {
			MSG_USER("silent_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_CONFUSION: {
		if (0 == status_detail_->running_effect.confusion_map.erase(effect.i_effect_value1)) {
			MSG_USER("confusion_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_EXP: {
		if (0 == status_detail_->running_effect.exp_map.erase(effect.status_id)) {
			MSG_USER("exp_map has no %d", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_BLOOD_BY_PER: {
		this->add_status_prop_by_per(effect);
		break;
	}
	case Status_Def::STATUS_EFFECT_TYPE_SHAPE_SHEEP: {
		if (0 == status_detail_->running_effect.shape_sheep_map.erase(effect.i_effect_value1)) {
			MSG_USER("shape_sheep_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_ET_REBOUND: {
		if (0 == status_detail_->running_effect.rebound_map.erase(effect.i_effect_value1)) {
			MSG_USER("exp_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_ET_CHANGE_FASHION: {
		if (0 == status_detail_->running_effect.change_fashion_map.erase(effect.status_id)) {
			MSG_USER("change_fashion_map has no %d", effect.i_effect_value1);
		}
		break;
	}
	case Status_Def::STATUS_ET_EXP_TWO: {
		if (0 == status_detail_->running_effect.exp_two_map.erase(effect.status_id)) {
			MSG_USER("exp_two_map has no %d", effect.status_id);
		}
		break;
	}
	case Status_Def::STATUS_ET_EXP_ACTIVITY: {
		if (0 == status_detail_->running_effect.exp_activity_map.erase(effect.status_id)) {
			MSG_USER("exp_activity_map has no %d", effect.status_id);
		}
		break;
	}
	default:
		break;
	}
	this->buff_unuse_skill_passive(effect);
	return 0;
}

bool Status::check_status_effect(Status_Effect_Detail &effect) {
	if (effect.buff_scene.empty()) {
		return true;
	}
	if (!move_scene()) {
//		if (fighter()->mover_detail().is_hero()){
		if (this->hero_self()) {
			Scene_Player *player = this->hero_self()->master();
			if (NULL != player) {
				const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player->move_scene()->scene_id());
				if (sc) {
					for (Int_Vec::const_iterator it = effect.buff_scene.begin(); it != effect.buff_scene.end(); ++it) {
						if (*it == sc->type) {
							return true;
						}
					}
				}
			}
		} else if (this->player_self()) {
			Scene_Player *player = this->player_self();
			if (NULL != player) {
				const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(player->base_detail().location.scene_id);
				if (sc) {
					for (Int_Vec::const_iterator it = effect.buff_scene.begin(); it != effect.buff_scene.end(); ++it) {
						if (*it == sc->type) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(move_scene()->scene_id());
	if (sc) {
		for (Int_Vec::const_iterator it = effect.buff_scene.begin(); it != effect.buff_scene.end(); ++it) {
			if (*it == sc->type) {
				return true;
			}
		}
	}
	return false;
}

bool Status::check_status_effect(Status_Effect_Detail &effect, const int scene) {
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

int Status::hook_status_overlay(const int status_id, const int num){
	if(this->player_self() && this->fighter() &&
			this->player_self()->battle_scene()){
		this->player_self()->battle_scene()->hook_status_overlay(this->fighter(), status_id, num);
	}
	return 0;
}

int Status::buff_use_skill(Status_Effect_Detail &effect, const Role_Vec &role_vec) {
	if (fighter()->is_be_skill(effect.skill_id)) {
		return 0;
	}
	if (effect.skill_id > 0) {
		int lvl = fighter()->get_learned_skill_lv(effect.skill_id);
		if (0 == lvl) {
			lvl = 1;
		}
		fighter()->handle_inner_call_skill_action(role_vec, effect.skill_id, lvl);
	}
	return 0;
}

int Status::buff_use_skill_passive(Status_Effect_Detail &effect) {
	if (!fighter()->is_be_skill(effect.skill_id)) {
		return 0;
	}
	if (effect.skill_id > 0) {
		int lvl = fighter()->get_learned_skill_lv(effect.skill_id);
		if (0 == lvl) {
			lvl = 1;
		}
		fighter()->fight_add_be_skill(effect.skill_id, lvl);
	}
	return 0;
}

int Status::buff_unuse_skill_passive(Status_Effect_Detail &effect) {
	if (!fighter()->is_be_skill(effect.skill_id)) {
		return 0;
	}
	fighter()->fight_remove_be_skill(effect.skill_id);
	return 0;
}

bool Status::has_status_id(const int status_id) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return (this->status_detail_->status_map.count(status_id) == 0)? false : true;
}

bool Status::has_status_evil(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !this->status_detail_->status_evil_set.empty();
}

bool Status::hurt_link_exist(const int status_id) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return (this->status_detail_->hurt_link.count(status_id) == 0)? false : true;
}

bool Status::cure_link_exist(const int status_id) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return (this->status_detail_->cure_link.count(status_id) == 0)? false : true;
}

int Status::status_num(const int status_id) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return 0;
	}
	Status_Detail::Status_Map::const_iterator it = this->status_detail_->status_map.find(status_id);
	if (it != this->status_detail_->status_map.end()) {
		return it->second->overlay_;
	}
	return 0;
}

double Status::exp_sum(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return 0.0;
	}
	return this->status_detail_->running_effect.exp_multi_sum();
}

double Status::rebound_sum(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return 0.0;
	}
	return this->status_detail_->running_effect.rebound_sum();
}

bool Status::control_judge(void) {
	bool can_control = true;
//	for (Int_Int_Map::iterator it = status_detail_->running_effect.immune_control_map.begin();
//			it != status_detail_->running_effect.immune_control_map.end(); ++it) {
//		it->second--;
//		if (it->second >= 0) {
//			can_control = false;
//			break;
//		} else {
//			can_control = true;
//			continue;
//		}
//	}

	return can_control;
}

bool Status::is_hurt_link(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.hurt_link_map.empty();
}

bool Status::is_dizz(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.stun_map.empty();
}

bool Status::is_stoic(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.stoic_map.empty();
}

bool Status::is_taunt(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.taunt_map.empty();
}

bool Status::is_lock(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.lock_map.empty();
}

bool Status::is_slow(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return status_detail_->running_effect.speed_sum() < 0;
}

bool Status::is_invincible(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.invincible_map.empty();
}

bool Status::is_cure_link(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.cure_link_map.empty();
}

bool Status::is_silent(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.silent_map.empty();
}

bool Status::is_confusion(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return false;
	}
	return !status_detail_->running_effect.confusion_map.empty();
}

int Status::check_param(int status_id) {
	const Btl_Buff_Cfg* buff_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL== buff_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	if (buff_config->is_stoic && this->is_stoic()) {
		MSG_81000061 res_msg;
		res_msg.result = Status_Def::STATUS_EFFECT_TYPE_STOIC;
		res_msg.role_id = role_id();
		THIS_SEND_TO_CLIENT(res_msg);
		return ERROR_IS_STOIC;
	}

	return 0;
}

bool Status::lottery_success(void) {
	int hit = fighter()->fighter_detail().fetch_fight_property(Property_Type::PT_STATUS_HIT_RATE);
	int num = rand()%100;
	if (1 == hit) {
		return true;
	} else if (num >= hit * 100) {
		return true;
	}
	return false;
}

void Status::handle_dispel(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return ;
	}
	this->erase_status_in_set(status_detail_->status_dispel_set);
}

void Status::handle_pure(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return ;
	}
	this->erase_status_in_set(status_detail_->status_pure_set);
}

void Status::handle_stoic(void) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return ;
	}
	MSG_81000061 res_msg;
	res_msg.result = Status_Def::STATUS_EFFECT_TYPE_STOIC;
	res_msg.role_id = role_id();
	Int_Hash_Set status_stoic_set(status_detail_->status_stoic_set);
	this->erase_status_in_set(status_detail_->status_stoic_set);
	if (!status_stoic_set.empty()) {
		THIS_SEND_TO_CLIENT(res_msg);
	}
}
void Status::handle_taunt(void) {
//	if (player_self()) {
//
//	}
	if (0 == fighter() || 0 == fighter()->battle_scene()) return;
	fighter()->battle_scene()->clear_enemy_simulate_target(*fighter());
}

double Status::handle_shield(double hurt, role_id_t attack_id) {
	if (!this->status_detail_ || this->status_detail_->status_map.empty()) {
		return hurt;
	}
	MSG_81000061 res_msg;
	res_msg.role_id = role_id();
	res_msg.attack_id = attack_id;
	if (this->is_invincible()) { // 无敌：免疫伤害
		Int_Int_Map::const_iterator it_invincible = status_detail_->running_effect.invincible_map.begin();
		if (it_invincible != status_detail_->running_effect.invincible_map.end()) {
			res_msg.status_id = it_invincible->second;
		}
		res_msg.result = Status_Def::STATUS_EFFECT_TYPE_INVINCIBLE;
		res_msg.extra_value = hurt;
		Block_Buffer buf;
		MAKE_MSG_BUF(res_msg, buf);
		call_broadcast_around(buf);
//		THIS_SEND_TO_CLIENT(res_msg);
		return 0;
	}
	Int_Hash_Set::const_iterator it_shield = this->status_detail_->status_shield_set.begin();
	if (it_shield != this->status_detail_->status_shield_set.end()) {
		res_msg.status_id = *it_shield;
	}
	res_msg.result = Status_Def::STATUS_EFFECT_TYPE_SHIELD;
	double result = hurt;
	if (0 == this->status_detail_->shield_cfg) {
		return result;
	}

	if (this->status_detail_->shield_hurt > this->status_detail_->shield_cfg) {
		this->status_detail_->shield_hurt = this->status_detail_->shield_cfg;
	}

	this->status_detail_->shield_hurt += hurt;
	bool shield_break = false;
	if (this->status_detail_->shield_hurt >= this->status_detail_->shield_cfg) {
		result = this->status_detail_->shield_hurt - this->status_detail_->shield_cfg; //盾破
		this->erase_status_in_set(this->status_detail_->status_shield_set);
		shield_break = true;
	} else {
		result = 0.0;
	}
	res_msg.extra_value = floor(hurt - result);
	if (res_msg.extra_value > 0 && !shield_break) {
		Block_Buffer buf;
		MAKE_MSG_BUF(res_msg, buf);
		call_broadcast_around(buf);
//		if (hero_self()) {
//			Scene_Player *sp = hero_self()->master();
//			if (sp) {
//				OBJ_SEND_TO_CLIENT(res_msg, (*sp));
//			}
//		}
	}
	return result;
}

double Status::handle_hurt_link(const Attack_Param &attack_param, double hurt_rate, Fighter &attacker) {
	if (this->is_hurt_link()) {
		//队友 delay

		Attack_Param param = attack_param;
		param.attack_result = -1; // 不继续伤害链
		//hero
		if (fighter()->mover_detail().is_player()){
			Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
			if (hero && hero->is_hurt_link()) {
				double dis = 0.0;
				for (Status_Link::iterator status_link_it = this->status_detail_->hurt_link.begin();
						status_link_it != this->status_detail_->hurt_link.end(); ++status_link_it) {
					if (hero->has_status_id(status_link_it->first) && hero->hurt_link_exist(status_link_it->first)) {
						dis += status_link_it->second.dis;
					}
				}
				hero->actual_sub_blood_for_buff(param, 0, 0, dis, Status_Def::STATUS_EFFECT_TYPE_HURT_LINK);
			}
		} else if (fighter()->mover_detail().is_hero()){
			Scene_Player *player = this->hero_self()->master();
			if (NULL != player && player->is_hurt_link()) {
				double dis = 0.0;
				for (Status_Link::iterator status_link_it = this->status_detail_->hurt_link.begin();
						status_link_it != this->status_detail_->hurt_link.end(); ++status_link_it) {
					if (player->has_status_id(status_link_it->first) && player->hurt_link_exist(status_link_it->first)) {
						dis += status_link_it->second.dis;
					}
				}
				player->actual_sub_blood_for_buff(param, 0, 0, dis, Status_Def::STATUS_EFFECT_TYPE_HURT_LINK);
			}
		} else {
			if (fighter()->battle_scene()) {
				for (Battle_Scene::Mover_Map::const_iterator it = fighter()->battle_scene()->mover_map().begin();
						it != fighter()->battle_scene()->mover_map().end(); ++it) {
					if (it->second) {
						Mover *mover = it->second;
						if (!mover || !mover->fighter() || mover->fighter()->dead_status()) continue;
						if (it->second->battle_position() != fighter()->battle_position()
								&& it->second->battle_position().pos == fighter()->battle_position().pos && it->second->fighter()->is_hurt_link()) {
							double dis = 0.0;
							for (Status_Link::iterator status_link_it = this->status_detail_->hurt_link.begin();
									status_link_it != this->status_detail_->hurt_link.end(); ++status_link_it) {
								if (it->second->fighter()->has_status_id(status_link_it->first) && it->second->fighter()->hurt_link_exist(status_link_it->first)) {
									dis += status_link_it->second.dis;
								}
							}
							it->second->fighter()->actual_sub_blood_for_buff(param, 0, 0, dis, Status_Def::STATUS_EFFECT_TYPE_HURT_LINK);
						}
					}
				}
			}
		}
		for (Status_Link::iterator status_link_it = this->status_detail_->hurt_link.begin();
				status_link_it != this->status_detail_->hurt_link.end(); ++status_link_it) {
			hurt_rate += status_link_it->second.main;
		}
	}
	return hurt_rate;
}

double Status::handle_cure_link(Fighter &attacker, double attack_cure, double cure_fix) {
	if (this->is_hurt_link()) {
		//队友 delay

		double cure_blood = 0.0;
		//hero
		if (fighter()->mover_detail().is_player()){
			Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
			if (hero && hero->is_hurt_link()) {
				double dis = 0.0;
				for (Status_Link::iterator status_link_it = this->status_detail_->cure_link.begin();
						status_link_it != this->status_detail_->cure_link.end(); ++status_link_it) {
					if (hero->has_status_id(status_link_it->first) && hero->cure_link_exist(status_link_it->first)) {
						dis += status_link_it->second.dis;
					}
				}
				cure_blood = attack_cure * (cure_fix + dis);
				hero->actual_cure(attacker, cure_blood);
			}
		} else if (fighter()->mover_detail().is_hero()){
			Scene_Player *player = this->hero_self()->master();
			if (NULL != player && player->is_hurt_link()) {
				double dis = 0.0;
				for (Status_Link::iterator status_link_it = this->status_detail_->cure_link.begin();
						status_link_it != this->status_detail_->cure_link.end(); ++status_link_it) {
					if (player->has_status_id(status_link_it->first) && player->cure_link_exist(status_link_it->first)) {
						dis += status_link_it->second.dis;
					}
				}
				cure_blood = attack_cure * (cure_fix + dis);
				player->actual_cure(attacker, cure_blood);
			}
		}
		cure_fix += this->status_detail_->cure_link_main;
	}
	return cure_fix;
}

void Status::hand_lock(const int status_id) {
	if (fighter() && fighter()->battle_scene() && fighter()->mover_detail_ptr()) {
		Int_Hash_Set status_ids;
		for (Battle_Scene::Mover_Map::const_iterator it = fighter()->battle_scene()->mover_map().begin();
			it != fighter()->battle_scene()->mover_map().end(); ++it) {
			if (it->first.pos != fighter()->mover_detail().battle_position.pos
					|| it->first.idx == fighter()->mover_detail().battle_position.idx
					|| !it->second->fighter() || !it->second->fighter()->is_lock()) continue;
			status_ids.clear();
			for (Int_Int_Map::const_iterator it_re = it->second->fighter()->status_detail()->running_effect.lock_map.begin();
					it_re != it->second->fighter()->status_detail()->running_effect.lock_map.end(); ++it_re) {
				status_ids.insert(it_re->second);
			}
			status_ids.erase(status_id);
			for (Int_Hash_Set::const_iterator it_si = status_ids.begin(); it_si != status_ids.end(); ++it_si) {
				it->second->fighter()->erase_status_type(*it_si);
			}
		}
	}
}
