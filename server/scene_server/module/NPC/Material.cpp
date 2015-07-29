/*
 * Material.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Material.h"
#include "Move_Scene.h"
#include "NPC_Manager.h"
#include "ai_machine/AI_Machine.h"
#include "Msg_Active_Struct.h"
#include "Config_Cache_NPC.h"
#include "Err_Code.h"
#include "Utility_Func.h"
#include "Msg_Inner_Struct.h"
#include "Scene_Player.h"
#include "Config_Cache_Scene.h"
#include "Scene_Monitor.h"
#include "gang/Scene_Gang_Global_Manager.h"
#include "Global_Scene_Manager.h"
#include "Msg_NPC_Struct.h"
#include "pack/Pack_Struct.h"
#include "campfire/Campfire_Data_Manager.h"
#include "expedition_scene/Expedition_Scene.h"

Material::Material(void):gather_status_(0), camp_(0), camp_bak_(0), camp_last_(0) { }

Material::~Material(void) { }

int Material::birth(role_id_t npc_id, int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info) {
	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
		MSG_USER("Material:%d birth error coord:%d %d error", npc_type_id, npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
		return -1;
	}

	if (scene->mpt_value_by_coord(npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y) < Move_Scene::MOVE_ABLE_FROM) {
		MSG_USER("Material:%d birth error coord:%d %d error", npc_type_id, npc_addition_info.birth_coord.grid_coord().x, npc_addition_info.birth_coord.grid_coord().y);
		return -1;
	}

	npc_detail_.npc_id = npc_id;
	int ret = this->init(npc_type_id, scene, point, npc_addition_info);
	if (ret != 0) {
		MSG_USER("material birth error.");
		return -1;
	}

	/// ai init
	//this->init_bt();

	enter_move_scene(scene);
	move_appear();

	return 0;
}

int Material::init(int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info) {
	const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(npc_type_id);
	if (config_cache == 0) {
		MSG_USER("find npc config:%d error", npc_type_id);
		return -1;
	}

	/// npc base init
	npc_record_ = point;
	//npc_detail_.direct_master = npc_addition_info.identify.master_id;
	npc_detail_.title_id = config_cache->title_id;
	npc_detail_.birth_by_skill = npc_addition_info.birth_by_skill;
	npc_detail_.npc_status = NORMAL_STATUS;
	npc_detail_.birth_time = Time_Value::gettimeofday();
	npc_detail_.npc_type_id = npc_type_id;
	npc_detail_.ai_id = 0;
	npc_detail_.is_ai_start = false;
	npc_detail_.birth_coord = npc_addition_info.birth_coord;
	if(point){
		if(point->birth_by != 0){
			npc_detail_.add_id.push_back(point->birth_by);
		}
		npc_detail_.state_type = point->config.state_type;
	}
	if (scene > 0 && Expedition_Type == get_scene_type(scene->scene_id())) {
		Appear_Addition aa;
		aa.type = MOVER_APPEAR_ADD_EXPE_DROPS_OR_MATERIAL;
		aa.array.push_back(this->npc_record()->cur_gt_times);
		aa.array.push_back(abs(this->npc_record()->config.gt_times));
		set_addition(aa);
	}

	/// material mover init
	Mover::set_detail_prt(npc_detail_.mover_detail_);
	int toward = npc_addition_info.toward > 0 ? npc_addition_info.toward : Move_Scene::toward_up;
	this->set_toward(toward);
	this->set_grid_coord(npc_detail().birth_coord.grid_coord().x, npc_detail().birth_coord.grid_coord().y, true);
	this->set_pixel_coord(npc_detail().birth_coord.pixel_coord().x,npc_detail().birth_coord.pixel_coord().y);
	this->retify_gauge_grid_coord();
	npc_detail_.mover_detail_.mover_type = (Mover_Type)config_cache->type;
	//mover_detail_.layer_id = record->layer_id;

	/// material fighter init
	npc_detail_.fighter_detail_.role_id = npc_id();
	//npc_detail_.fighter_detail_.camps = npc_addition_info.identify.camps ? npc_addition_info.identify.camps : config_cache->camps;
	npc_detail_.fighter_detail_.level = npc_addition_info.level ? npc_addition_info.level : config_cache->level;

	/// material base init
	material_detail_.jid = npc_addition_info.jid ? npc_addition_info.jid : config_cache->jid;
	material_detail_.name = (npc_addition_info.name != "") ? npc_addition_info.name : config_cache->name;
	material_detail_.type = point->config.type;
	material_detail_.lv_limit = config_cache->drop_lv_limit;
	material_detail_.gather_time = point->config.gather_time;
	material_detail_.tasks_vec = point->config.tasks_vec;
	int drop_size = config_cache->drops_vec.size();
	for(int i=0; i<drop_size; ++i){
		if(i == 0){
			material_detail_.rate = config_cache->drops_vec[i];
		}else{
			material_detail_.drops_vec.push_back(config_cache->drops_vec[i]);
		}
	}
	npc_detail_.fighter_detail_.level = config_cache->level;
	return 0;
}

int Material::fini(void) {
	NPC::fini();

	reset();

	return 0;
}

int Material::reset(void) {
	NPC::reset();
	material_detail_.reset();
	gather_player_map_.clear();
	success_player_map_.clear();
	gather_status_ = 0;
	camp_ = 0;
	camp_bak_ = 0;
	camp_last_ = 0;
//	gang_war_all_gather_player_.clear();
	return 0;
}

int Material::move_appear_info(Block_Buffer &buf) {
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	MSG_81000005 msg;
	msg.npc_info.role_id = role_id();
	msg.npc_info.type = npc_operation_type();
	msg.npc_info.state = npc_operation_state();
	msg.npc_info.npc_type = npc_detail().npc_type_id;
	msg.npc_info.npc_jid = material_detail_.jid;
	msg.npc_info.role_name = material_detail_.name;
	msg.npc_info.title_id = npc_detail().title_id;
	msg.npc_info.coord = this->pixel_coord();
	msg.npc_info.hp_curr = 0;
	msg.npc_info.hp_max = 0;
	msg.npc_info.speed = 0;
	msg.npc_info.add_id = npc_detail().add_id;

	if (MOVER_T_MATERIAL_GANG_WAR_TOWER == npc_detail_.mover_detail_.mover_type) {
		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_GANG_WAR;
		int sec = 0;
		// 300: 公会战array [已走时间、总时间、人数、所属(0未被占领 1红色 2蓝色)]
		Gather_Player_Map::iterator it = gather_player_map_.begin();
		if (it != gather_player_map_.end()) {
			Time_Value now = Time_Value::gettimeofday();
			if (it->second > now) {
				Time_Value remain_time = it->second - now;
				int remain_time_ms = remain_time.msec() * gather_player_map_.size();
				sec = material_detail_.gather_time - remain_time_ms / 1000;
			}
			if (sec < 0) {
				sec = 0;
			}
		}
		app_add.array.push_back(sec);
		app_add.array.push_back(material_detail_.gather_time);
		app_add.array.push_back(gather_player_map_.size());
		app_add.array.push_back(camp_);
		app_add.array.push_back(this->is_gathering_camp());
		msg.npc_info.addition_prop.push_back(app_add);
	}else{
		msg.npc_info.addition_prop = addition_prop();
	}
//	Int_Int ava;
//	ava.val_1 = material_detail_.jid;
//	if(this->drops_detail_.drop_type == 0){
//		ava.val_2 = 0;
//	}else{
//		ava.val_2 = 1;
//	}
//	msg.npc_info.avatar.push_back(ava);
	buf.make_message(msg.msg_id);
	msg.serialize(buf);

	// refill len in head
	wr_end_idx = buf.get_write_idx();
	len = wr_end_idx - wr_begin_idx - sizeof(uint32_t);
	buf.set_write_idx(wr_begin_idx);
	buf.write_uint32(len);
	buf.set_write_idx(wr_end_idx);

	return 0;
}

void Material::set_role_name(const std::string& name){
	material_detail_.name = name;
}

std::string Material::role_name(void)
{
	return material_detail_.name;
}
//
//int Material::die_process(const Attack_Info &info, Fighter &killer) {
//	if (npc_status() == RECOVER_STATUS || npc_status() == DEAD_STATUS) return -1;
//	NPC::die_process(info, killer);
//
//	// notice logic server
//	notice_logic_die(info.attacker);
//
//	if (material_detail_.material_buff) {
//		Map_Player *player = MAP_MANAGER_INSTANCE->find_role_id_player(info.attacker);
//		if (player)
//			player->insert_status_all_by_cfg(material_detail_.material_buff, info.attacker);
//	}
//
//	material_drop(info.attacker);
//
//	if (material_detail_.collect_times > 0) {
//		material_detail_.collect_times--;
//		if (material_detail_.collect_times == 0) {
//			this->set_gather_state(false);
//		}
//	}
//
//	if (material_detail_.config_cache->collect_jid) {
//		if (material_detail_.config_cache->collect_jid != material_detail_.jid) {
//			this->change_self(material_detail_.config_cache->collect_jid);
//		}
//	}
//
//	Fighter::die_process(info, killer);
//
//	return 0;
//}

int Material::notice_logic_die(role_id_t attacker) {
//	if (mover_type() != MOVER_MATERIAL) {
//		return 0;
//	}
//
//	role_id_t killer_id = find_direct_master_id(attacker);
//	if (killer_id == 0) return -1;
//
//	MSG_700015 inner_msg;
//	inner_msg.role_id = killer_id;
//	inner_msg.npc_type_id = npc_self()->npc_type_id();
//	inner_msg.scene = npc_self()->mover_detail().scene_id;
//	if (npc_self()->ai_config().die_share) {
//		for (Hatred_Map::iterator iter = hatred_map_.begin(); iter != hatred_map_.end(); ++iter) {
//			killer_id = find_direct_master_id(iter->first);
//			if (killer_id == 0) continue;
//			Role_Vec::iterator find_it = std::find(inner_msg.kill_roles.begin(), inner_msg.kill_roles.end(), killer_id);
//			if (find_it != inner_msg.kill_roles.end()) continue;
//			inner_msg.kill_roles.push_back(killer_id);
//		}
//	} else {
//		inner_msg.kill_roles.push_back(inner_msg.role_id);
//	}
//	Block_Buffer inner_buf;
//	inner_buf.make_message(INNER_KILL_NPC);
//	inner_msg.serialize(inner_buf);
//	inner_buf.finish_message();
//	MAP_MANAGER_INSTANCE->send_to_logic_server(inner_buf);

	return 0;
}

bool Material::is_material(void) {
	return true;
}

Material *Material::material_self(void) {
	return this;
}

int Material::change_self(int jid) {
//	material_detail_.jid = jid;
//	notify_fight_info(UPDATE_NPC_JID, jid, true);

	return 0;
}

void Material::material_drop(role_id_t attacker) {
//	if (!material_detail_.config_cache->die_drops.empty()) {
//		if (!this->move_scene()) return ;
//
//		attacker = find_direct_master_id(attacker);
//		if (attacker == 0) return ;
//		for (Int_Set::iterator it = material_detail_.config_cache->die_drops.begin(); it != material_detail_.config_cache->die_drops.end(); ++it) {
//			NPC_MANAGER->drop_by_amount((*it), grid_coord(), this->move_scene(), 1, 0, attacker, 0);
//		}
//	}
}

int Material::tick(Time_Value &now){
	if(recover_state())
	{
		return 0;
	}
	Gather_Player_Map::iterator it;
	for( it = gather_player_map_.begin(); it != gather_player_map_.end(); ){
		if(now > it->second){
			Mover* mover = this->move_scene()->find_mover_with_layer(0, it->first);
			if(!mover || !mover->player_self()){
				it = gather_player_map_.erase(it);
				del_gather_status();
				continue;
			}
			int drop_result = drop_item(mover->player_self());
			if(drop_result == -1){
				Time_Value end_time = Time_Value::gettimeofday() + Time_Value(5, 0);
				success_player_map_.insert(std::make_pair(it->first, end_time));
				it = gather_player_map_.erase(it);
				gather_goods_result(mover->role_id(), 0);
			}
			else if(drop_result == 0){//

				it = gather_player_map_.erase(it);
				del_gather_status();
				if(this->npc_record()->config.gt_times != 0){
					this->npc_record_->cur_gt_times += 1;
					add_addition(mover->role_id());
					if(this->npc_record()->config.gt_times > 0 &&
							this->npc_record_->cur_gt_times >= this->npc_record_->config.gt_times){
						this->npc_record()->clear_point();
						return 0;
					}
				}
			}else{
				Time_Value end_time = Time_Value::gettimeofday() + Time_Value(5, 0);
				success_player_map_.insert(std::make_pair(it->first, end_time));
				it = gather_player_map_.erase(it);
			}

		}else{
			++it;
		}
	}
	//采集物添加到背包超时处理
	for( it = success_player_map_.begin(); it != success_player_map_.end(); ){
		if(now > it->second){
			it = success_player_map_.erase(it);
			del_gather_status();
		}else{
			++it;
		}
	}
	return 0;
}

int Material::start_gather(Scene_Player* player){
	if(!player || this->recover_state()){
		return -1;//ERROR_GATHER_ID_NONEXIST;
	}
	if (had_collect(player->role_id())) {
		return ERROR_GATHER_DONE;
	}
	if(npc_record() && npc_record()->config.gt_times != 0 &&
			npc_record()->cur_gt_times >= abs(npc_record()->config.gt_times)){
		return -1;// 采集次数用完
	}
	if(material_detail_.type == Material_Type_Single && gather_status_ >0){
		return ERROR_GATHER_MULT_LIMIT;
	}
	if(this->material_detail_.lv_limit > player->level()){
		return ERROR_GATHER_LV_LIMIT;
	}
	if(player){
		Item_Info_Vec spar_items;
		int spar_result = player->spar_gather(this->npc_detail().npc_type_id, spar_items);
		if(spar_result != 0){
			return spar_result;
		}
	}
	if(this->npc_record_){
		int task_count = this->material_detail_.tasks_vec.size();
		if(task_count){
			bool has_task = false;
			for(int i=0; i<task_count; ++i){
				if(player->is_task_exist(player, this->material_detail_.tasks_vec[i])){
					has_task = true;
					break;
				}
			}
			if(has_task == false){
				return ERROR_GATHER_TASK_LIMIT;
			}
		}
		//check cd
		Time_Value now = Time_Value::gettimeofday();
		if(this->npc_record()->check_gt_cd(now) == false){
			return ERROR_START_GATHER_BE_CD;
		}
	}
	if(mover_detail().mover_type == MOVER_T_MATERIAL_WAR_MINE ||
			mover_detail().mover_type == MOVER_T_MATERIAL_WAR_SPE_MINE){
		if(player->war_info()->is_pull_mine != 0){
			return ERROR_GATHER_PULLING_MINE;
		}
	}else if(mover_detail().mover_type == MOVER_T_MATERIAL_WAR_REVIVE_BALL){
		if(player->war_info()->revive != 1){
			return ERROR_GATHER_WAR_REVIVE_LIMIT;
		}
	}else if(mover_detail().mover_type == MOVER_T_MATERIAL_GANG_WAR_TOWER){
		int ret = gang_war_handle(player);
		return ret;
	}else if (mover_detail().mover_type == MOVER_T_MATERIAL_GANG_WAR_REVIVE_BALL) {
		if (player->gang_war_info().revive != 1) {
			return ERROR_GATHER_WAR_REVIVE_LIMIT;
		}
	}else if(mover_detail().mover_type == MOVER_T_MATERIAL_CAMPFIRE_MEAT){
		if (player->base_detail().gang_id == 0) {
			return ERROR_BONFIRE_MEAT_GANG_LIMIT;
		}
		if(npc_record_){
			int fire_pt = npc_record_->leader_id;
			Campfire_Detail* detail = CAMPFIRE_DATA_MANAGER->get_campfire_detail_from_point(fire_pt);
			if(detail && detail->gang_id != player->base_detail().gang_id){
				return ERROR_BONFIRE_MEAT_OWN_GANG_LIMIT;
			}
		}
	}

	Time_Value end_time = Time_Value::gettimeofday() +
			Time_Value(this->material_detail_.gather_time, 0);
	gather_player_map_.insert(std::make_pair(player->role_id(), end_time));
	add_gather_status();
	return 0;
}

int Material::stop_gather(role_id_t role_id){
	if(this->recover_state()){
		return -1;
		//return ERROR_STOP_GATHER_ID_NONEXIST;
	}
	Gather_Player_Map::iterator it;
	it = success_player_map_.find(role_id);
	if(it != success_player_map_.end()){
		return -1;//ERROR_STOP_GATHER_HAS_SUCCESS;
	}
	it = gather_player_map_.find(role_id);
	if(it == gather_player_map_.end()){
		return -1;//ERROR_STOP_GATHER_NONGATHER;
	}
	gather_player_map_.erase(role_id);
	Scene_Player *pl = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
	if (pl) {
		pl->gang_war_set_point_id(0);
		pl->update_mover_gang_war_info();
	}
	del_gather_status();
	Gather_Player_Map::iterator it_b = gather_player_map_.begin();
	if (it_b != gather_player_map_.end()) {
		Time_Value now = Time_Value::gettimeofday();
		Time_Value remain_time = it_b->second - now;
		int remain_time_ms = remain_time.msec() * (gather_player_map_.size() + 1);
		int end_time_ms = remain_time_ms / gather_player_map_.size();
		Time_Value end_time = now +
				Time_Value(end_time_ms / 1000, end_time_ms % 1000 * 1000);
		send_player_end_time(end_time, now);
//		MSG_USER("remain: %d, end: %d", remain_time_ms, end_time_ms);
	}
	send_appear_addition();

	if (MOVER_T_MATERIAL_GANG_WAR_TOWER == npc_detail_.mover_detail_.mover_type
		&& move_scene()
		&& pl) {
		move_scene()->hook_gather_special_handle(pl, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
	}
	return 0;
}

int Material::drop_item(Scene_Player* player){
	// 战场采矿
	if(npc_detail_.mover_detail_.mover_type ==  MOVER_T_MATERIAL_WAR_MINE ||
			npc_detail_.mover_detail_.mover_type ==  MOVER_T_MATERIAL_WAR_SPE_MINE ||
			npc_detail_.mover_detail_.mover_type == MOVER_T_MATERIAL_WAR_REVIVE_BALL
			|| npc_detail_.mover_detail_.mover_type == MOVER_T_MATERIAL_GANG_WAR_TOWER
			|| npc_detail_.mover_detail_.mover_type == MOVER_T_MATERIAL_GANG_WAR_REVIVE_BALL ||
			npc_detail_.mover_detail_.mover_type == MOVER_T_MATERIAL_CAMPFIRE_MEAT){
		MSG_81000110 msg;
		msg.material_role_id = this->role_id();
		msg.result = 1;
		OBJ_SEND_TO_CLIENT(msg, (*player));
		hook_gather_act();
		return -1;
	}
	// 生活技能
	Item_Info_Vec spar_items;
	int spar_result = player->spar_gather(npc_detail().npc_type_id, spar_items);
	if(spar_result == 0 && spar_items.empty() == false){
		{
			MSG_81000110 msg;
			msg.material_role_id = this->role_id();
			msg.result = 1;
			OBJ_SEND_TO_CLIENT(msg, (*player));
			hook_gather_act();
		}
		//to logic
		Block_Buffer buf;
		MSG_20300010 msg;
		msg.material_role_id = this->role_id();
		for(Item_Info_Vec::iterator it = spar_items.begin(); it != spar_items.end(); ++it){
			Drops_Item item;
			item.bind = it->bind;
			item.item_amount = it->amount;
			item.item_id = it->id;
			msg.item_vec.push_back(item);
		}
		buf.make_message(msg.msg_id);
		msg.serialize(buf);
		buf.finish_message();
		player->send_to_logic(buf);

		return 1;
	}else{
		if(random() % 1000 < this->material_detail_.rate){
			{
				MSG_81000110 msg;
				msg.material_role_id = this->role_id();
				msg.result = 1;
				OBJ_SEND_TO_CLIENT(msg, (*player));
				hook_gather_act();
			}
			//to logic
			Block_Buffer buf;
			MSG_20300010 msg;
			msg.material_role_id = this->role_id();
			for(std::vector<int>::iterator it = material_detail_.drops_vec.begin();
					it != material_detail_.drops_vec.end(); ++it){
				Drops_Item item;
				item.bind = Item_Detail::BIND;
				item.item_amount = 1;
				item.item_id = *it;
				msg.item_vec.push_back(item);
			}
			buf.make_message(msg.msg_id);
			msg.serialize(buf);
			buf.finish_message();
			player->send_to_logic(buf);
			return 1;
		}else{
			MSG_81000110 msg;
			msg.material_role_id = this->role_id();
			msg.result = 0;
			OBJ_SEND_TO_CLIENT(msg, (*player));
			return 0;
		}
	}
	return 0;
}

int Material::gather_goods_result(role_id_t role_id, int result){
	Gather_Player_Map::iterator it = success_player_map_.find(role_id);
	if(it == success_player_map_.end()){
		return -1;
	}
	Mover* mover = this->move_scene()->find_mover_with_layer(0, role_id);
	if(!mover || !mover->player_self()){
		success_player_map_.erase(it);
		del_gather_status();
		return -1;
	}


	if(move_scene()){
		move_scene()->hook_gather_success(mover, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
	}

	//
	if (MOVER_T_MATERIAL_GANG_WAR_TOWER == npc_detail_.mover_detail_.mover_type) {
		const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(mover->player_self()->ganger_detail().gang_id);
		if (info) {
			camp_bak_ = info->camp;
			camp_ = 0;
		}
		send_appear_addition();
		if (move_scene()) {
			move_scene()->hook_gather_special_handle(mover, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
		}
	}
	success_player_map_.erase(it);
	// 生活技能
	if(mover && mover->player_self()){
		mover->player_self()->spar_gather_cost(npc_detail().npc_type_id);
	}

	del_gather_status();

	if(this->npc_record()->config.gt_times != 0){
		this->npc_record_->cur_gt_times += 1;
		add_addition(mover->role_id());

		if(this->npc_record()->config.gt_times > 0 &&
				this->npc_record_->cur_gt_times >= this->npc_record_->config.gt_times){
			this->npc_record()->clear_point();
			return 0;
		}
	}
	return 0;
}

int8_t &Material::camp(void) {
	return this->camp_;
}
int8_t &Material::camp_bak(void) {
	return this->camp_bak_;
}
int8_t &Material::camp_last(void) {
	return this->camp_last_;
}
int8_t Material::is_gathering_camp(void) {
	for(Gather_Player_Map::iterator it = gather_player_map_.begin(); it != gather_player_map_.end(); ++it){
		Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(it->first);
		if (player) {
			const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(player->ganger_detail().gang_id);
			if (info) {
				return info->camp;
				break;
			}
		}
	}
	if (this->camp_bak_ != 0) {
		return this->camp_bak_;
	}
	return 0;
}

int Material::get_gather_players(std::vector<role_id_t>& players){
	Gather_Player_Map::iterator it;
	for(it = gather_player_map_.begin(); it != gather_player_map_.end(); ++it){
		players.push_back(it->first);
	}
	int count = gather_player_map_.size();
	return count;
}

int Material::change_gather_cd(role_id_t role_id, int total_time, int remain_time){
	Gather_Player_Map::iterator it = gather_player_map_.find(role_id);
	if(it == gather_player_map_.end()){
		return -1;
	}
	it->second = Time_Value::gettimeofday() + Time_Value(remain_time);
	Mover* mover = this->move_scene()->find_mover_with_layer(0, role_id);
	if(!mover || !mover->player_self()){
		return -1;
	}

	int player_num =  gather_player_map_.size();
	MSG_81000111 msg;
	msg.role_id = this->role_id();
	msg.total_time = total_time;
	msg.remain_time = remain_time;
	msg.gather_num = player_num;
	OBJ_SEND_TO_CLIENT(msg, (*mover));
	return 0;
}

void Material::send_appear_addition(void) {
	if (MOVER_T_MATERIAL_GANG_WAR_TOWER == npc_detail_.mover_detail_.mover_type) {
		MSG_81000002 msg;
		msg.role_id = role_id();
		Appear_Addition app_add;
		app_add.type = MOVER_APPEAR_ADDITION_GANG_WAR;
		int sec = 0;
		// 300: 公会战array [已走时间、总时间、人数、所属(0未被占领 1红色 2蓝色)]
		Gather_Player_Map::iterator it = gather_player_map_.begin();
		if (it != gather_player_map_.end()) {
			Time_Value now = Time_Value::gettimeofday();
			if (it->second > now) {
				Time_Value remain_time = it->second - now;
				int remain_time_ms = remain_time.msec() * gather_player_map_.size();
				sec = material_detail_.gather_time - round(remain_time_ms / 1000.0);
			}
			if (sec < 0) {
				sec = 0;
			}
		}
		app_add.array.push_back(sec);
		app_add.array.push_back(material_detail_.gather_time);
		app_add.array.push_back(gather_player_map_.size());
		app_add.array.push_back(camp_);
		app_add.array.push_back(this->is_gathering_camp());
		msg.add_info_vec.push_back(app_add);
		if (move_scene()) {
			Block_Buffer buf;
			buf.make_message(msg.msg_id);
			msg.serialize(buf);
			buf.finish_message();
			move_scene()->broadcast_all_within_battle(0, buf);
		}
	}
	return ;
}

void Material::send_player_end_time(const Time_Value &end_time, const Time_Value &now) {
	int end_time_ms = end_time.msec() - now.msec();
	int remain_time_ms = end_time_ms * gather_player_map_.size();
//	MSG_USER("send: remain: %d, end: %d", remain_time_ms, end_time_ms);
	MSG_81000111 acv_msg;
	acv_msg.role_id = this->role_id();
	acv_msg.total_time = this->material_detail_.gather_time;
	acv_msg.gather_num = gather_player_map_.size();
	acv_msg.remain_time = acv_msg.total_time - remain_time_ms / 1000;
	for (Gather_Player_Map::iterator it_b = gather_player_map_.begin();
			it_b != gather_player_map_.end(); ++it_b) {
		it_b->second.set( end_time);
		role_id_t role_id = it_b->first;
		Scene_Player *other_player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
		if (other_player) {
			OBJ_SEND_TO_CLIENT(acv_msg, (*other_player));
		}
	}
}

void Material::add_addition(const role_id_t role_id) {
	if (Expedition_Type != get_scene_type(move_scene_id())) {
		return;
	}
	if (role_id > 0) {
		role_collected.insert(role_id);
	}

	Appear_Addition addition;
	addition.type = MOVER_APPEAR_ADD_EXPE_DROPS_OR_MATERIAL;

	get_addition(addition);
	addition.array.clear();
	addition.array.push_back(this->npc_record()->cur_gt_times);
	addition.array.push_back(abs(this->npc_record()->config.gt_times));
	addition.int64_vec.push_back(role_id);
	set_addition(addition);

	if (this->npc_record()) {
		Expedition_Scene *es = (Expedition_Scene*)this->move_scene();
		es->add_role_collected(this->npc_record()->config.id, role_id);
	}
}

int Material::gang_war_handle(Scene_Player* player) {
	Time_Value now = Time_Value::gettimeofday();
	const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(player->ganger_detail().gang_id);
	if (info) {
		if (!gather_player_map_.empty()) {
			for (Gather_Player_Map::iterator it = gather_player_map_.begin();
				it != gather_player_map_.end(); ++it) {
				role_id_t role_id = it->first;
				Time_Value second = it->second;
				Scene_Player *other_player = SCENE_MONITOR->find_scene_player_by_role_id(role_id);
				if (other_player) {
					const Gang_War_Matched_Gang_Info *info_other = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(other_player->ganger_detail().gang_id);
					if (info_other && info_other->camp != info->camp) {
						this->stop_gather(role_id);

						{
//							Mover *mover = player->find_mover_in_move_scene(it->first);
//							if (! mover) {
//								MSG_USER("can not find scene player:%ld", it->first);
//								return ERROR_PLAYER_NOT_ONLINE;
//							}
//
//							if (! mover->player_self()) {
//								MSG_USER("is not player:%ld", it->first);
//								return ERROR_CLIENT_OPERATE;
//							}

							if(other_player->is_safe_time()){
								return -1;
							}

							if (other_player->gang_war_info().revive == 1) {
								MSG_USER("has die");
								return -1;
							}

							if (player->gang_war_info().revive == 1) {
								MSG_USER("has die");
								return -1;
							}

							if (player->is_in_battle()) {
								MSG_USER("has in battle");
								return -1;
							}

							if (other_player->is_in_battle()) {
								MSG_USER("has in battle");
								return -1;
							}

							if (player->dead_status()) {
								MSG_USER("has die");
								return -1;
							}

							if (other_player->fighter()->dead_status()) {
								MSG_USER("has die");
								return -1;
							}

							player->gang_war_set_point_id(0);
							player->update_mover_gang_war_info();

							other_player->gang_war_set_point_id(0);
							other_player->update_mover_gang_war_info();

							Battle_Scene *scene = GLOBAL_SCENE_MANAGER->create_battle_scene(player->move_scene());

							if(player->set_battle_position(0, Position_Key::LINE_TWO_FRONT, scene) == 0){
								player->enter_battle_scene(scene);
							}

							if(other_player->set_battle_position(1, Position_Key::LINE_TWO_FRONT, scene) == 0){
								other_player->enter_battle_scene(scene);
							}
						}
						// 非同一边多人
						Gather_Player_Map::iterator it_b = gather_player_map_.begin();
						if (it_b != gather_player_map_.end()) {
							if (move_scene()) {
								move_scene()->hook_gather_start_special(player, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
							}
							Time_Value remain_time = second - now;
							int remain_time_ms = remain_time.msec() * (gather_player_map_.size() + 1);
							int end_time_ms = remain_time_ms / gather_player_map_.size();
							Time_Value end_time = now +
									Time_Value(end_time_ms / 1000, end_time_ms % 1000 * 1000);
//							add_gather_status();
							send_appear_addition();
							send_player_end_time(end_time, now);
							if (move_scene()) {
								move_scene()->hook_gather_special_handle(0, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
							}
						}
						return -1;
					} else {
						break;
					}
				}
			}

			{
				MSG_50210100 res_msg;
				res_msg.material_role_id = role_id();
				res_msg.gather_time = material_detail().gather_time;
				res_msg.result = 1;
				OBJ_SEND_TO_CLIENT(res_msg, (*player));
			}
			// 同一边多人
			Gather_Player_Map::iterator it = gather_player_map_.begin();
			if (it != gather_player_map_.end()) {
				if (move_scene()) {
					move_scene()->hook_gather_start_special(player, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
				}
				Time_Value remain_time = it->second - now;
				int remain_time_ms = remain_time.msec() * gather_player_map_.size();
				int end_time_ms = remain_time_ms / (gather_player_map_.size() + 1);
				Time_Value end_time = now +
						Time_Value(end_time_ms / 1000, end_time_ms % 1000 * 1000);
				gather_player_map_.insert(std::make_pair(player->role_id(), end_time));
				add_gather_status();
				send_player_end_time(end_time, now);
				send_appear_addition();
				if (move_scene()) {
					move_scene()->hook_gather_special_handle(0, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
				}
			}
			return 1000;
		} else if (0 != camp_bak_ || 0 != camp_) {
			if ((0 != camp_bak_ && info->camp != camp_bak_) || (0 != camp_ && info->camp != camp_)) {
				{
					MSG_50210100 res_msg;
					res_msg.material_role_id = role_id();
					res_msg.gather_time = material_detail().gather_time;
					res_msg.result = 1;
					OBJ_SEND_TO_CLIENT(res_msg, (*player));
				}
				if (move_scene()) {
					move_scene()->hook_gather_start_special(player, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
				}
				Time_Value end_time = Time_Value::gettimeofday() +
						Time_Value(this->material_detail_.gather_time / (gather_player_map_.size() + 1), 0);
				gather_player_map_.insert(std::make_pair(player->role_id(), end_time));
				add_gather_status();
				send_appear_addition();
				if (move_scene()) {
					move_scene()->hook_gather_special_handle(0, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
				}

				return 1000;
			}
			return ERROR_GANG_WAR_TOWER_HOLD;
		}
		{
			MSG_50210100 res_msg;
			res_msg.material_role_id = role_id();
			res_msg.gather_time = material_detail().gather_time;
			res_msg.result = 1;
			OBJ_SEND_TO_CLIENT(res_msg, (*player));
		}
		if (move_scene()) {
			move_scene()->hook_gather_start_special(player, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
		}
		Time_Value end_time = Time_Value::gettimeofday() +
				Time_Value(this->material_detail_.gather_time / (gather_player_map_.size() + 1), 0);
		gather_player_map_.insert(std::make_pair(player->role_id(), end_time));
		add_gather_status();
		send_appear_addition();
		if (move_scene()) {
			move_scene()->hook_gather_special_handle(0, npc_record_->config.id, npc_detail_.mover_detail_.mover_type);
		}
	} else {
		return -1;
	}
	return 1000;
}
