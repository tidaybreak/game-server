/*
 * Title.cpp
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#include "Logic_Player_Def.h"
#include "Msg_Title_Struct.h"
#include "Msg_Active_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Err_Code.h"
#include "Title_Def.h"
#include "Title.h"
#include "Title_Struct.h"
#include "Title_Func.h"
#include "Config_Cache_Title.h"
#include "public/Public_Struct.h"
#include "gang/Gang_Struct.h"
#include "Logic_Player.h"
#include "equiper/Equiper_Func.h"
#include "mail/Mail_Struct.h"
#include "title/Title_Manager.h"
#include "ranking/Ranking_Manager.h"
#include "announce/Logic_Announce.h"
#include "Config_Cache_Item.h"
#include "team/Team_Manager.h"
#include "dungeon/Single_Dungeon.h"
#include "achievement/Achievement_Struct.h"
#include "Config_Cache_Mail.h"


Title::Title(void) { reset(); }

Title::~Title(void) { }

void Title::reset(void) {
	title_detail_ = 0;
	title_map_recharge.clear();
	title_map_exploit.clear();
	title_map_contribute.clear();
	title_map_war_scene_win.clear();
	title_map_war_scene_kill.clear();
	title_map_arena_win.clear();
	title_map_cumulate_equip.clear();
	title_map_level.clear();
	title_map_boss.clear();
	title_map_ancient_instance.clear();
	equip_title = 0;
}

int Title::create_init(Title_Detail &detail) {
	return 0;
}

void Title::load_detail(Title_Detail &detail) {
	title_detail_ = &detail;
	return ;
}

void Title::save_detail(Title_Detail &detail) {
	if(title_detail().is_change) {
		detail = title_detail();
		title_detail_->is_change = false;
	}
	return ;
}
const Title_Detail &Title::title_detail(void) const {
	return *title_detail_;
}
void Title::module_init(void) {
	if (!title_detail_) {
		return ;
	}
	for (Title_Detail_Info::iterator iter = title_detail_->title_set.begin();
			iter != title_detail_->title_set.end(); ) {
		const Title_Cfg_Detail * const cfg = CONFIG_CACHE_TITLE->find_title(iter->first);
		if (!cfg || !TITLE_MANAGER->handle_title(role_id(), iter->first)) {
			title_detail_->title_set.erase(iter++);
		} else {
			++iter;
		}
	}
	for (Title_Detail_Info::iterator iter = title_detail_->title_set.begin();
			iter != title_detail_->title_set.end(); ++iter) {
		insert_title_and_sort(&(iter->second), title_detail_->title_time_list);
	}
	equip_title = title_detail_->equip_title;
	if (title_detail_->title_set.count(title_detail_->equip_title) == 0) {
		title_detail_->equip_title = 0;
		player_self()->set_title(title_detail_->equip_title);
		title_auto_equip_title();
	}
	this->add_rank_title();
	if (title_detail_->title_set.count(equip_title) > 0
			&& title_detail_->equip_title == 0) {
		title_detail_->equip_title = equip_title;
		player_self()->set_title(title_detail_->equip_title);
	}
	this->title_property_refresh(false);
	title_construct();
}
void Title::sign_in(void) {
	this->create_event(Logic_Enum::EVENT_LOGIC_TITLE_TIME, &Logic_Player::time_up, Time_Value(SAVE_DETAIL_INTERVAL));
}
void Title::sign_in_repeat(void) {
//	this->create_event(Logic_Enum::EVENT_LOGIC_TITLE_TIME, &Logic_Player::time_up, Time_Value(SAVE_DETAIL_INTERVAL));
}
void Title::sign_out(void) {
}
void Title::client_ready_now(void) {
	//const Title_Cfg_Detail *cfg = CONFIG_CACHE_TITLE->find_title(equip_title);
	//if (cfg && cfg->ann_sign_in) {
	//	player_self()->create_ann_title_sign_in(player_self(), cfg->ann_sign_in);
	//}
	for (Title_Detail_Info::iterator it = title_detail_->title_set.begin(); it != title_detail_->title_set.end(); ++it) {
		const Title_Cfg_Detail *cfg = CONFIG_CACHE_TITLE->find_title(it->second.id);
		if (cfg && cfg->ann_sign_in) {
			player_self()->create_ann_title_sign_in(player_self(), cfg->ann_sign_in);
		}
	}
}
// 场景加载完成
int Title::move_scene_completed(void) {
	return 0;
}
//托管中下线调用
void Title::trusteeship_sign_out(void) {
}
void Title::time_up(void) {
	Time_Value now = Time_Value::gettimeofday();
	while(! title_detail_->title_time_list.empty()) {
		//		bool is_remove = false;
		Title_Detail_Info_Detail *info = title_detail_->title_time_list.front();

		if (info->last_end_time > now) break;
		int32_t id = info->id;
		title_detail_->title_time_list.pop_front();

		this->erase_title(id, false);
	}
	add_rank_title();
	player_self()->hero_time_up(now);
}

int Title::req_fetch_title_info(void) {
	MSG_50100900 res_msg;
	for (Title_Detail_Info::iterator iter = title_detail_->title_set.begin();
			iter != title_detail_->title_set.end(); ++iter) {
		Int_Int tt;
		tt.val_1 = iter->first;
		if (0 != iter->second.last_time) {
			tt.val_2 = iter->second.last_end_time.sec();
		}
		res_msg.title_vector.push_back(tt);
	}
	res_msg.equip_title = title_detail_->equip_title;

	int8_t task_complete = 1;
	for (Int_Hash_Set::const_iterator it = CONFIG_CACHE_TITLE->honour_task().begin(); it != CONFIG_CACHE_TITLE->honour_task().end(); ++it) {
		if (!player_self()->is_task_submited(*it)) {
			task_complete = 0;
		}
	}
	res_msg.task_complete = task_complete;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Title::req_title_on_off(const int32_t title_id, const int8_t op) {
	if (title_detail_->title_set.count(title_id) == 0)
		return ERROR_CLIENT_PARAM;

//	int title_type = get_title_type(msg.title_id);
//	if (title_type == TEMP_TITLE || title_type == INNER_TITLE || title_type == XIAN_TITLE)
//		return this->respond_error_result(RES_TITLE_ON_OFF, ERROR_TITLE_SELF_NOT_ON_OFF);

	if (op == 1) {			//穿称号
		title_detail_->equip_title = title_id;
	} else if (op == 2) {			//脱称号
		title_detail_->equip_title = 0;
	} else {
		return ERROR_CLIENT_PARAM;
	}
	save_tick();

	this->sync_title_info_to_map();
	this->req_fetch_title_info();

	MSG_50100901 res_msg;
	res_msg.title_id = title_id;
	res_msg.op = op;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Title::add_title(int title_id, const bool is_module_init) {
	if (title_id <= 0 || has_title(title_id))
		return -1;

	const Title_Cfg_Detail * const cfg = CONFIG_CACHE_TITLE->find_title(title_id);
	if (!cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	Title_Detail_Info_Detail info;
	info.id = title_id;
	info.start_time = Time_Value::gettimeofday();
	info.last_time = cfg->validity;
	info.last_end_time = info.start_time + Time_Value(info.last_time);
	title_detail_->title_set.insert(std::make_pair(info.id, info));
	Title_Detail_Info::iterator it = title_detail_->title_set.find(info.id);
	if (it != title_detail_->title_set.end()) {
		insert_title_and_sort(&(it->second), title_detail_->title_time_list);
	}
	title_auto_equip_title();

	save_tick();

	this->title_property_refresh();
	this->sync_title_info_to_map();
//	player_self()->task_listen_title(title_id);

	MSG_81000900 acv_msg;
	acv_msg.title_id = title_id;
	if (0 != cfg->validity) {
		acv_msg.time = info.last_end_time.sec();
	}
	THIS_SEND_TO_CLIENT(acv_msg);

	if (cfg->announ && false == is_module_init && cfg->quality >= 3) {
		title_send_mail(title_id);
	}
	return 0;
}

int Title::add_rank_title(int title_id) {
	if (title_id <= 0 || has_title(title_id))
		return -1;

	const Title_Cfg_Detail * const cfg = CONFIG_CACHE_TITLE->find_title(title_id);
	if (!cfg) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	Title_Detail_Info_Detail info;
	info.id = title_id;
	info.start_time = Time_Value::gettimeofday();
	info.last_time = cfg->validity;
	info.last_end_time = info.start_time + Time_Value(info.last_time);
	title_detail_->title_set.erase(info.id);
	title_detail_->title_set.insert(std::make_pair(info.id, info));
	Title_Detail_Info::iterator it = title_detail_->title_set.find(info.id);
	if (it != title_detail_->title_set.end()) {
		insert_title_and_sort(&(it->second), title_detail_->title_time_list);
	}
	title_auto_equip_title();

	save_tick();

	this->title_property_refresh();
	this->sync_title_info_to_map();
//	player_self()->task_listen_title(title_id);

	MSG_81000900 acv_msg;
	acv_msg.title_id = title_id;
	if (0 != cfg->validity) {
		acv_msg.time = info.last_end_time.sec();
	}
	THIS_SEND_TO_CLIENT(acv_msg);

	player_self()->title_send_mail(title_id);
	return 0;
}

//int Title::replace_title(int title_id, bool sync) {
//	if (title_id <= 0 || has_title(title_id))
//		return -1;
//
////	int title_type = get_title_type(title_id);
////	for (std::vector<int>::iterator iter = title_detail_.equip_title.begin();
////			iter != title_detail_.equip_title.end(); ++iter) {
////		if (get_title_type(*iter) == title_type) {
////			title_detail_.equip_title.erase(iter);
////			break;
////		}
////	}
////	for (std::set<int>::iterator iter = title_detail_.title_set.begin();
////			iter != title_detail_.title_set.end(); ++iter) {
////		if (get_title_type(*iter) == title_type) {
////			title_detail_.title_set.erase(iter);
////			break;
////		}
////	}
////	title_detail_.equip_title.push_back(title_id);
////	title_detail_.title_set.insert(title_id);
////	title_detail_.save_tick();
////
////	if (sync) {
////		this->title_property_refresh();
////		this->sync_title_info_to_map();
////	}
////
////	player_self()->task_listen_title(title_id);
//	return 0;
//}

int Title::erase_title(const int title_id, const bool need_erase_list) {
	if (!title_detail_->title_set.count(title_id)) return 0;

	if (title_id <= 0 || (!has_title(title_id)))
		return -1;

	if (need_erase_list) {
		Title_Detail_Info_List::iterator iter = this->title_detail_->title_time_list.begin();
		for (; iter != this->title_detail_->title_time_list.end(); ++iter) {
			if ((**iter).id == title_id) {
				this->title_detail_->title_time_list.erase(iter);
				break;
			}
		}
	}
	title_detail_->title_set.erase(title_id);
	if (title_detail_->equip_title == title_id) {
		title_detail_->equip_title = 0;
		title_auto_equip_title();
	}

	save_tick();

	this->title_property_refresh();
	this->sync_title_info_to_map();

	MSG_81000901 acv_msg;
	acv_msg.title_id = title_id;
	THIS_SEND_TO_CLIENT(acv_msg);
	return 0;
}

bool Title::has_title(int title_id) {
	int count = title_detail_->title_set.count(title_id);
	if (count > 0)
		return true;
	return false;
}

int Title::get_title_info(int &equip_title) {
	equip_title = title_detail_->equip_title;
	return 0;
}

int Title::sync_title_info_to_map(void) {
	player_self()->set_title(title_detail_->equip_title);
	MSG_81000902 res_msg;
	res_msg.role_id = role_id();
	res_msg.equip_title = title_detail_->equip_title;

	THIS_SEND_TO_CLIENT(res_msg);

	Block_Buffer sync_buff;
	sync_buff.make_message(INNER_SYNC_TITLE_INFO);
	sync_buff.write_int32(this->title_detail_->equip_title);
	sync_buff.finish_message();
	this->send_to_scene(sync_buff);
	return 0;
}

int Title::title_listen_task_finish(void) { // 待优化
	for (Int_Hash_Set::const_iterator it = CONFIG_CACHE_TITLE->honour_task().begin(); it != CONFIG_CACHE_TITLE->honour_task().end(); ++it) {
		if (!player_self()->is_task_submited(*it)) {
			return -1;
		}
	}
	req_fetch_title_info();
	this->title_property_refresh();
	return 0;
}
int Title::title_property_refresh(bool sync) {

	for (Int_Hash_Set::const_iterator it = CONFIG_CACHE_TITLE->honour_task().begin(); it != CONFIG_CACHE_TITLE->honour_task().end(); ++it) {
		if (!player_self()->is_task_submited(*it)) {
			return -1;
		}
	}

	Addition_Type addi_type = AT_TITLE;
//	E_FUNC::reset_fight_prop(*player_self(), addi_type);

	std::map<int, double> pro_val_map;

	for(Title_Detail_Info::iterator it = this->title_detail_->title_set.begin(); it != this->title_detail_->title_set.end(); ++it) {
		const Title_Cfg_Detail *cfg = CONFIG_CACHE_TITLE->find_title(it->first);
		if (!cfg) {
			continue;
		}
		for (Int_Double_Map::const_iterator it_prop = cfg->prop.begin(); it_prop != cfg->prop.end(); ++it_prop) {
			pro_val_map[it_prop->first] += it_prop->second;
		}
	}

	// 人物属性加成
	if (! pro_val_map.empty()) {
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		for (std::map<int, double>::iterator iter = pro_val_map.begin(); iter != pro_val_map.end(); ++iter) {
			if (iter->first && iter->second) {
				setter.reset();
				setter.prop_type = (Property_Type)iter->first;
				setter.operate = O_SET;
				setter.addi_type = addi_type;
				setter.basic = 0.0;
				setter.fixed = iter->second;
				setter.percent = 0.0;
				setter_vec.push_back(setter);
			}
		}
		if (! setter_vec.empty()) {
			player_self()->fight_prop_modify(setter_vec, 221);
		}
	}

	return 0;
}

int Title::get_title_type(int title_id) {
//	const Json::Value &title_info = CONFIG_INSTANCE->title_info(title_id);
//	if (title_info == Json::Value::null)
//		return 0;
//
//	return title_info["type"].asInt();
	return 0;
}

int Title::add_rank_title(void) {
	std::vector<int> charts_type_vector;
	charts_type_vector.clear();
	charts_type_vector.push_back(RankingDef::LOCAL_PERSONAL_FORCE_RANKING);	//第一个为个人战力排行榜，特殊处理
	charts_type_vector.push_back(RankingDef::LOCAL_PERSONAL_LEVEL_RANKING);
	charts_type_vector.push_back(RankingDef::LOCAL_PERSONAL_CHARM_RANKING);
	charts_type_vector.push_back(RankingDef::LOCAL_PERSONAL_FRIEND_VALUE_RANKING);
	charts_type_vector.push_back(RankingDef::LOCAL_ARENA_SIGLE_RANKING);
	charts_type_vector.push_back(RankingDef::LOCAL_ARENA_TEAM_RANKING);

	int career = player_self()->base_detail().career;
	for (uint i = 0; i < charts_type_vector.size(); ++i) {
		if (i > 0) {
			career = 0;
		}
//		int rank = RANKING_MANAGER->find_rank((RankingDef::Ranking_Type)charts_type_vector[i], title_detail_->role_id);
//		if (rank > 0) {
//			int title_id = 0;
//			TITLE_MANAGER->get_charts_title_id(charts_type_vector[i], rank, career, title_id);
//			if (title_id > 0) {
//				this->add_title(title_id, true);
//			}
//		}
		Title_Record_Detail::Charts_Title_Map::iterator iter = TITLE_MANAGER->title_record_detail().charts_title_map.find((RankingDef::Ranking_Type)charts_type_vector[i]);
		if (iter != TITLE_MANAGER->title_record_detail().charts_title_map.end()) {
			//判断是否已有相同的称号,有相同称号的玩家,role_id置0
			for (Title_Record_Detail::Charts_Info_Vector::iterator iter_old = iter->second.begin();
					iter_old != iter->second.end(); ++iter_old) {
				if (iter_old->role_id == role_id()) {
					this->add_title(iter_old->title_id, true);
					//break;
				}
			}
		}
	}
	if (player_self()->role_id() == TITLE_MANAGER->title_record_detail().gang_war_first_leader) {
		this->add_title(TITLE_GANG_WAR_FIRST, true);
	}
	return 0;
}

int Title::title_listen_recharge(void) {
//	this->title_detail_->recharge_his += val;
	title_construct_recharge();
	save_tick();
	return 0;
}

int Title::title_listen_exploit(void) {
	title_construct_exploit();
	save_tick();
	return 0;
}

int Title::title_listen_contribute(void) {
	title_construct_contribute();
	save_tick();
	return 0;
}

int Title::title_listen_war_scene_win(void) {
	title_construct_war_scene_win();
	save_tick();
	return 0;
}

int Title::title_listen_war_scene_kill(void) {
	title_construct_war_scene_kill();
	save_tick();
	return 0;
}

int Title::title_listen_arena_win(void) {
	title_construct_arena_win();
	save_tick();
	return 0;
}

int Title::title_listen_ancient_instance(void) {
	title_construct_ancient_instance();
	save_tick();
	return 0;
}

int Title::title_listen_cumulate_equip(void) {
	title_construct_cumulate_equip();
	save_tick();
	return 0;
}
int Title::title_listen_level(void) {
	title_construct_level();
	save_tick();
	return 0;
}
int Title::title_listen_boss(void) {
	title_construct_boss();
	save_tick();
	return 0;
}

int Title::title_auto_equip_title(void) {
	return 0;
	int quality = 0;
	Int_Vec highly_title_ids;
	for (Title_Detail_Info::iterator iter = title_detail_->title_set.begin();
			iter != title_detail_->title_set.end(); ++iter) {
		const Title_Cfg_Detail * const cfg = CONFIG_CACHE_TITLE->find_title(iter->first);
		if (!cfg || quality > cfg->quality) {
			continue;
		}
		if (cfg->quality > quality) {
			quality = cfg->quality;
			highly_title_ids.clear();
		}
		highly_title_ids.push_back(iter->first);
	}
	if (!highly_title_ids.empty()) {
		if (this->title_detail_->equip_title > 0) {
			const Title_Cfg_Detail * const cfg = CONFIG_CACHE_TITLE->find_title(this->title_detail_->equip_title);
			if (!cfg) {
				this->title_detail_->equip_title = 0;
			} else {
				if (quality <= cfg->quality) {
					return -1;
				}
			}
		}
		int rand_index = random() % highly_title_ids.size();
		this->title_detail_->equip_title = highly_title_ids[rand_index];

		save_tick();

		this->sync_title_info_to_map();
		this->req_fetch_title_info();

		MSG_50100901 res_msg;
		res_msg.title_id = highly_title_ids[rand_index];
		res_msg.op = 1;
		THIS_SEND_TO_CLIENT(res_msg);
	}
	return 0;
}

bool Title::title_check_cumulate_equip(void) {
	int suit_idx_begin = PACK_T_EQUIP_INDEX + EQUIP_SUIT_BEGIN;
	int suit_idx_end = PACK_T_EQUIP_INDEX + EQUIP_SUIT_END;
	const Item_Detail *equip = 0;
	for (int idx = suit_idx_begin; idx < suit_idx_end; ++idx) {
		equip = 0;
		equip = player_self()->pack_get_const_item(idx);
		if (0 == equip) return false;
		const Item_Detail_Config* cfg = CONFIG_CACHE_ITEM->find_item(equip->id_);
		if (0 == cfg) return false;
		if (CONFIG_CACHE_TITLE->handbook().count(equip->id_) == 0) {
			return false;
		}
	}
	return true;
}

int Title::title_construct(void) {
	for (Title_Cfg::const_iterator cfg = CONFIG_CACHE_TITLE->title_cfg().begin();
			cfg != CONFIG_CACHE_TITLE->title_cfg().end(); ++cfg) {
		if (cfg->second.condition.from_table == "recharge") {
			title_map_recharge.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.condition.from_table == "exploit") {
			title_map_exploit.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.condition.from_table == "gang_contribute") {
			title_map_contribute.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.condition.from_table == "war_scene_win") {
			title_map_war_scene_win.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.condition.from_table == "war_scene_kill") {
			title_map_war_scene_kill.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.condition.from_table == "arena_win") {
			title_map_arena_win.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.cond_type == 1) {
			title_map_cumulate_equip.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.cond_type == 2) {
			title_map_level.insert(std::make_pair(cfg->second.cond, cfg->first));
		} else if (cfg->second.cond_type == 3) {
			title_map_boss.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		} else if (cfg->second.condition.from_table == "ancient_instance") {
			title_map_ancient_instance.insert(std::make_pair(cfg->second.condition.cond, cfg->first));
		}
	}
	title_construct_recharge();
	title_construct_exploit();
	title_construct_contribute();
	title_construct_war_scene_win();
	title_construct_war_scene_kill();
	title_construct_arena_win();
	title_construct_cumulate_equip();
	title_construct_level();
	title_construct_ancient_instance();
	return 0;
}

int Title::title_construct_recharge(void) {
	for (Int_Int_Map::iterator it = title_map_recharge.begin(); it != title_map_recharge.end(); ++it) {
		if (player_self()->public_detail().recharge_money_his >= (uint32_t)it->first) {
			this->add_title(it->second);
		} else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_exploit(void) {
	for (Int_Int_Map::iterator it = title_map_exploit.begin(); it != title_map_exploit.end(); ++it) {
		if (player_self()->public_detail().exploit_lvl >= it->first) {
			this->add_title(it->second);
		}else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_contribute(void) {
	for (Int_Int_Map::iterator it = title_map_contribute.begin(); it != title_map_contribute.end(); ++it) {
		if (player_self()->ganger_detail().total_contribute >= static_cast<uint32_t>(it->first)) {
			this->add_title(it->second);
		}else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_war_scene_win(void) {
	for (Int_Int_Map::iterator it = title_map_war_scene_win.begin(); it != title_map_war_scene_win.end(); ++it) {
		if (player_self()->achievement_detail().war_light_win_nums_his + player_self()->achievement_detail().war_dark_win_nums_his >= it->first) {
			this->add_title(it->second);
		}else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_war_scene_kill(void) {
	for (Int_Int_Map::iterator it = title_map_war_scene_kill.begin(); it != title_map_war_scene_kill.end(); ++it) {
		if (player_self()->achievement_detail().war_kill_nums_his >= it->first) {
			this->add_title(it->second);
		}else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_arena_win(void) {
	for (Int_Int_Map::iterator it = title_map_arena_win.begin(); it != title_map_arena_win.end(); ++it) {
		if (player_self()->achievement_detail().arena_win_nums_his >= it->first) {
			this->add_title(it->second);
		}else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_ancient_instance(void) {
	for (Int_Int_Map::iterator it = title_map_ancient_instance.begin(); it != title_map_ancient_instance.end(); ++it) {
		if (player_self()->get_shanggu_complete_num() >= it->first) {
			this->add_title(it->second);
		}else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_cumulate_equip(void) {
	// 称号：收集八件，只有一个称号，特殊处理
	for (Int_Int_Map::iterator it = title_map_cumulate_equip.begin(); it != title_map_cumulate_equip.end(); ++it) {
		if (title_check_cumulate_equip()) {
			this->add_title(it->second);
		}/*else {
			this->erase_title(it->second);
		}*/
	}
	return 0;
}

int Title::title_construct_level(void) {
	Int_Uid *iu = 0;
	switch(career()) {
	case 1: {
		iu = &(TEAM_MANAGER->first_lvl_zs_his());
		break;
	}
	case 2: {
		iu = &(TEAM_MANAGER->first_lvl_ck_his());
		break;
	}
	case 3: {
		iu = &(TEAM_MANAGER->first_lvl_yss_his());
		break;
	}
	default:
		break;
	}
	if (0 == iu) {
		return -1;
	}
	for (Int_Int_Map::iterator it = title_map_level.begin(); it != title_map_level.end(); ++it) {
		Int_Uid::const_iterator it_fl = iu->find(it->first);
		if (it_fl == iu->end()) {
			continue;
		}
		if (it_fl->second == role_id()) {
			this->add_title(it->second);
		}else {
			this->erase_title(it->second);
		}
	}
	return 0;
}

int Title::title_construct_boss(void) {
	for (Int_Int_Map::iterator it = title_map_boss.begin(); it != title_map_boss.end(); ++it) {
		this->add_title(it->second);
	}
	return 0;
}

void Title::title_send_mail(const int id) {
	const Title_Cfg_Detail * const cfg = CONFIG_CACHE_TITLE->find_title(id);
	if (!cfg) {
		return ;
	}

	int mail_id = 0;
	if (cfg->validity == 0) {
		mail_id = 1038;
	} else {
		mail_id = 1045;
	}
	const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
	Mail_Config_Map::const_iterator iter = mail_config.find(mail_id);
	if(iter == mail_config.end()) {
		return ;
	}

	Mail_Send_Info send_info;
	send_info.sender_id = 0;
	send_info.receiver_id = this->role_id();
	send_info.send_type = MAIL_TYPE_SYS;
	send_info.show_send_time = Time_Value::gettimeofday();

	send_info.sender_name = iter->second.sender_name;
	send_info.title = iter->second.mail_title;

	char mail_content[1024] = {'\0'};
	if (cfg->validity == 0) {
		snprintf(mail_content, sizeof(mail_content), iter->second.mail_content.c_str(), cfg->des.c_str(), cfg->name.c_str());
	} else {
		int sec = (cfg->validity/Time_Value::ONE_DAY_IN_SECS);
		snprintf(mail_content, sizeof(mail_content), iter->second.mail_content.c_str(), cfg->des.c_str(), cfg->name.c_str(), sec);
	}
	send_info.content.assign(mail_content);


	/*Mail_Send_Info send_info;
	send_info.sender_id = 0;
	send_info.receiver_id = this->role_id();
	send_info.send_type = MAIL_TYPE_SYS;
	send_info.show_send_time = Time_Value::gettimeofday();
	send_info.sender_name = "系统";
	send_info.title = "获得新称号";



	std::stringstream content;
	content << "恭喜你";
	if (!cfg->des.empty()) {
		content << cfg->des << "，";// << cfg->des << "，";
	}
	content << "成功获得称号：" << cfg->name << "，有效期：";
	if (cfg->validity == 0) {
		content << "永久";
	} else {
		content << ceil(cfg->validity/Time_Value::ONE_DAY_IN_SECS) << "天";
	}
	content << "。可在 角色>称号 面板切换称号。";
	send_info.content = content.str();*/
	player_self()->send_mail_to_self(send_info);

	if (cfg->announ == true){
		player_self()->create_ann_title(player_self(), cfg->des, cfg->name);
	}
}

void Title::save_tick(void) {
	title_detail_->detail_change();
}
