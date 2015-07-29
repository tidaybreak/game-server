/*
 * Logic_Status.cpp
 *
 *  Created on: 2014年6月12日
 *      Author: Linqiyou
 */

#include "Msg_Inner_Enum.h"
#include "Err_Code.h"
#include "Logic_Status_Struct.h"
#include "status/Status_Struct.h"
#include "Logic_Status.h"
#include "Config_Cache_Buff.h"
#include "Logic_Player.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "Msg_Active_Struct.h"
#include "status/Status_Func.h"
#include "Config_Cache_Scene.h"

Logic_Status::Logic_Status() {
	reset();
}

Logic_Status::~Logic_Status() {
	// TODO Auto-generated destructor stub
}

int Logic_Status::create_init(Logic_Status_Detail &detail) {
	return 0;
}

void Logic_Status::load_detail(Logic_Status_Detail &detail) {
	logic_status_detail_ = &detail;
}

void Logic_Status::save_detail(Logic_Status_Detail &detail) {
	if(logic_status_detail().is_change) {
		detail = logic_status_detail();
		logic_status_detail_->is_change = false;
	}
	return ;
}

Logic_Status_Detail const &Logic_Status::logic_status_detail(void) const{
	return *logic_status_detail_;
}

void Logic_Status::module_init(void) {
}

void Logic_Status::sign_in(void) {
    return ;
}

void Logic_Status::sign_out(void)
{
    return ;
}

int Logic_Status::client_ready_now(void) {
	special_status_handle();
	return 0;
}
int Logic_Status::move_scene_completed(void) {
	MSG_81000071 msg_fight;
	msg_fight.type = SFT_FIGHT;
	MSG_81000071 msg_scene;
	msg_scene.type = SFT_SCENE;

	Status_Info_Msg msg_info;
	for (Int_Hash_Set::const_iterator it_pre = pre_send_to_client_.begin();
			it_pre != pre_send_to_client_.end(); ++it_pre) {
		const Btl_Buff_Cfg *buff_cfg = CONFIG_CACHE_BUFF->btl_buff(*it_pre);
		if (!buff_cfg) {
			continue;
		}
		msg_info.reset();
		msg_info.status_owner = role_id();
		msg_info.status_id = *it_pre;
		msg_info.overlay = 0;
		msg_info.last_time_ms = 0;
		msg_info.remain_time = 0;
		if (buff_cfg->is_fight_buff == SFT_FIGHT) {
			msg_fight.buff_detail.push_back(msg_info);
		} else {
			msg_scene.buff_detail.push_back(msg_info);
		}
	}
	int scene_id = player_self()->base_detail().location.scene_id;
	for (boost::unordered_map<int32_t, Status_Info>::iterator status_it = logic_status_detail_->status_map.begin();
			status_it != logic_status_detail_->status_map.end(); ++status_it) {
		if (status_it->second.scene_visible.empty() || !check_int_in_vec(scene_id, status_it->second.scene_visible)) {
			continue;
		}

		pre_send_to_client_.insert(status_it->second.status_id_);
	}
	const Int_Hash_Set *sv_cfg = CONFIG_CACHE_BUFF->btl_scene_visible(scene_id);
	if (sv_cfg) {
		for (Int_Hash_Set::const_iterator it_sv = sv_cfg->begin(); it_sv != sv_cfg->end(); ++it_sv) {
			const Btl_Buff_Cfg *buff_cfg = CONFIG_CACHE_BUFF->btl_buff(*it_sv);
			if (!buff_cfg) {
				continue;
			}
			msg_info.reset();
			msg_info.status_owner = role_id();
			msg_info.status_id = *it_sv;
			msg_info.overlay = 1;
			msg_info.last_time_ms = 0;
			msg_info.remain_time = 0;
			if (buff_cfg->is_fight_buff == SFT_FIGHT) {
				msg_fight.buff_detail.push_back(msg_info);
			} else {
				msg_scene.buff_detail.push_back(msg_info);
			}
			pre_send_to_client_.insert(*it_sv);
		}
	}
	if (!msg_fight.buff_detail.empty()) {
		THIS_SEND_TO_CLIENT(msg_fight);
	}
	if (!msg_scene.buff_detail.empty()) {
		THIS_SEND_TO_CLIENT(msg_scene);
	}
	return 0;
}

void Logic_Status::reset(void) {
	logic_status_detail_ = 0;
	pre_send_to_client_.clear();
}

void Logic_Status::save_tick(void) {
	if (logic_status_detail_) {
		logic_status_detail_->save_tick();
	}
}

int Logic_Status::sync_status_module_info(Block_Buffer &buf) {
	logic_status_detail_->serialize(buf);
	return 0;
}

int Logic_Status::load_status_info(Block_Buffer &buf) {
	if (!logic_status_detail_) {
		return 0;
	}
	logic_status_detail_->deserialize(buf);
	this->save_tick();
	return 0;
}

int Logic_Status::sync_status_info(Block_Buffer &buf) {
//	buf.make_message(INNER_SYNC_STATUS);
	logic_status_detail_->serialize(buf);
//	buf.finish_message();
//	this->send_to_scene(buf);
	return 0;
}

double Logic_Status::exp_sum(const int scene_id){
	int scene_id_bak = scene_id;
	if (scene_id_bak == 0) {
		scene_id_bak = player_self()->location().scene_id;
	}
	const Scene_Config *sc = CONFIG_CACHE_SCENE->scene_config_cache(scene_id_bak);
	if (!sc) {
		return 0.0;
	}
	if (//FuBen_Scene_Type != sc->type
//			&&
			Team_Fb_Scene_Type != sc->type
			&& HERO_DREAM_Scene_Type != sc->type
			) {
		return 0.0;
	}
	return this->logic_status_detail_->running_effect.exp_multi_sum();
}

int Logic_Status::logic_insert_status(const Status_Effect_Argv &argv) {
	const Btl_Buff_Cfg *cfg = CONFIG_CACHE_BUFF->btl_buff(argv.status_id);
	if (!cfg) {
		return ERROR_STATUS_NOT_EXIST;
	}
	Block_Buffer buf;
	buf.make_message(INNER_STATUS_INSERT);
	argv.serialize(buf);
	buf.finish_message();
	this->send_to_scene(buf);

	return 0;
}

//int Logic_Status::logic_insert_status_hero(const Status_Effect_Argv &argv) {
//	const Btl_Buff_Cfg *cfg = CONFIG_CACHE_BUFF->btl_buff(argv.status_id);
//	if (!cfg) {
//		return ERROR_STATUS_NOT_EXIST;
//	}
//	Block_Buffer buf;
//	buf.make_message(INNER_SYNC_STATUS_HERO_INSERT);
//	buf.write_uint16(1);
//	argv.serialize(buf);
//	buf.finish_message();
//	this->send_to_scene(buf);
//
//	return 0;
//}
//int Logic_Status::logic_insert_status_hero(const std::vector<Status_Effect_Argv> sea_vec) {
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
//}

bool Logic_Status::has_status_id(const int status_id) {
	return (this->logic_status_detail_->status_map.count(status_id) == 0)? false : true;
}

double Logic_Status::find_rune_stone_smelt_status_rate(const int status_id) {
	double ret = 0.0;
	Logic_Status_Detail::Status_Map::const_iterator it_sm = this->logic_status_detail_->status_map.find(status_id);
	if (it_sm != this->logic_status_detail_->status_map.end() && it_sm->second.overlay_ > 0) {
		const Status_Info* status_info = &(it_sm->second);
		for (Status_Info::Status_Info_Map::const_iterator status_info_map_it = status_info->status_info_map.begin();
				status_info_map_it != status_info->status_info_map.end(); ++status_info_map_it) {
			if (status_info_map_it->second.overlay <= 0) {
				continue;
			}
			for (Status_Info_Detail::Effect_Vec::const_iterator it = status_info_map_it->second.effect_map.begin();
					it != status_info_map_it->second.effect_map.end(); ++it) {
				if (status_info_map_it->second.overlay <= 0) {
					continue;
				}
				if ((*it).effect_type != Status_Def::STATUS_EFFECT_TYPE_ENCHANT_RATE) {
					continue;
				}
				ret += (*it).d_effect_value1;
			}
		}
	}
	return ret;
}

void Logic_Status::logic_del_status(const int status_id) {
	const Btl_Buff_Cfg *cfg = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (!cfg) {
		return ;
	}
	Block_Buffer buf;
	buf.make_message(INNER_SYNC_STATUS_DEL);
	buf.write_int32(status_id);
	buf.finish_message();
	this->send_to_scene(buf);
}

int Logic_Status::special_status_handle(void) {
	const int lv = 10;
	int status_id = CONFIG_CACHE_BUFF->special_level_buff().status_id;
	if (level() > lv) {
		if (has_status_id(status_id)) {
			logic_del_status(status_id);
		}
		return 0;
	} else if (!has_status_id(status_id)) {
		logic_insert_status(CONFIG_CACHE_BUFF->special_level_buff());
	}
	return 0;
}
