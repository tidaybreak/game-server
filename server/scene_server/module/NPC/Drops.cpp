/*
 * Drops.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Drops.h"
#include "Move_Scene.h"
#include "ai_machine/AI_Machine.h"
#include "NPC_Manager.h"
#include "Config_Cache.h"
#include "Msg_Active_Struct.h"
#include "Msg_NPC_Struct.h"
#include "Err_Code.h"
#include "Scene_Player.h"
#include "Config_Cache_Dungeon.h"
#include "Config_Cache_NPC.h"
#include "expedition_scene/Expedition_Scene.h"


Drops::Drops(void){

}

Drops::~Drops(void) { }

int Drops::birth(role_id_t &npc_id, Move_Scene *scene, NPC_Record *point, Coord &birth_coord, int ai, int ai_config) {
	if (this->init(npc_id, scene, point, birth_coord, ai, ai_config) != 0) {
		MSG_USER("drops type id:%d configure file error.", npc_id);
		return -1;
	}
	this->enter_move_scene(scene);
	this->move_appear();

	return 0;
}

int Drops::init(role_id_t &npc_id, Move_Scene *scene, NPC_Record *point, Coord &birth_coord, int ai, int ai_config) {
//	if (drops_detail_.drops_map.empty()) return -1;
//
//	npc_detail_.ai_config = CONFIG_CACHE_INSTANCE->find_ai_config_cache(ai_config);
//	if (!npc_detail_.ai_config) {
//		MSG_USER("can not find ai config:%d ai:%d", npc_id, ai_config);
//		return -1;
//	}
//
	/// npc base init
	this->npc_record_ = point;
	npc_detail_.npc_id = npc_id;
	npc_detail_.npc_status = NORMAL_STATUS;
	npc_detail_.birth_time = Time_Value::gettimeofday();
	npc_detail_.ai_id = ai;
	npc_detail_.is_ai_start = true;
	npc_detail_.birth_coord = birth_coord;
	//npc_detail_.life_time = (npc_detail_.ai_config->life_time == Time_Value::zero) ? Time_Value(INT_MAX, 0) : Time_Value::gettimeofday() + npc_detail_.ai_config->life_time;
	if(!point){
		npc_detail_.life_time = Time_Value::gettimeofday() + Time_Value(600,0);
	}
	if (scene > 0 && Expedition_Type == get_scene_type(scene->scene_id())) {
		Appear_Addition aa;
		aa.type = MOVER_APPEAR_ADD_EXPE_DROPS_OR_MATERIAL;
		aa.array.push_back(cur_gt_times_);
		aa.array.push_back(cfg_max_gt_times_);
		set_addition(aa);
	}

	/// drops mover init
	Mover::set_detail_prt(npc_detail_.mover_detail_);
	this->set_grid_coord(npc_detail().birth_coord.grid_coord().x, npc_detail().birth_coord.grid_coord().y, true);
	this->set_pixel_coord(npc_detail().birth_coord.pixel_coord().x,npc_detail().birth_coord.pixel_coord().y);
	this->retify_gauge_grid_coord();
	npc_detail_.mover_detail_.mover_type = MOVER_T_DROP;
	this->set_layer_id(0);


//	fighter_detail_.role_id = npc_id;


	return 0;
}

int Drops::fini(void) {
	NPC::fini();

	reset();

	return 0;
}

int Drops::reset(void) {
	NPC::reset();

	drops_detail_.reset();

	cur_gt_times_ = 0;
	cfg_max_gt_times_ = 0;
	return 0;
}

int Drops::tick(Time_Value &now) {
	if(recover_state())
	{
		return 0;
	}
	if(this->npc_record() == NULL){
		if (now > npc_detail().life_time) {
			// 死亡自动拾取
			if (drops_detail_.auto_owner && drops_detail_.role_drop_map.count(drops_detail_.auto_owner)) {
				die_auto_get();
			}
			recover_self();
			return 0;
		}
	}
	if (cur_gt_times_ >= cfg_max_gt_times_) {
		recover_self();
		return 0;
	}

	//NPC::tick(now);
	return 0;
}

int Drops::active_npc(void) {
	if (move_scene() == NULL || npc_id() == 0) {
		MSG_USER("ERROR_9:npc id:%ld", npc_id());
		return -1;
	}

	move_appear();

	return 0;
}

int Drops::move_appear_info(Block_Buffer &buf) {
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	MSG_81000005 msg;
	msg.npc_info.role_id = role_id();
	msg.npc_info.npc_type = npc_detail().npc_type_id;
	msg.npc_info.title_id  = npc_detail().title_id;
	msg.npc_info.npc_jid = drops_detail().jid;
	if(this->drops_detail_.drop_type == Drops::DROP_TYPE_BOX){
//		if(this->drops_detail().open){
//			msg.npc_info.type = 2;
//		}else{
//			msg.npc_info.type = 0;
//		}
	}else{
		msg.npc_info.type = 1;
	}
	if(this->drops_detail().drop_type == DROP_TYPE_SCATTER){
		msg.npc_info.npc_type = 60820000;
		msg.npc_info.npc_jid = 60820000;
		int ava;
		ava = npc_detail().npc_type_id;
		msg.npc_info.avatar.push_back(ava);
	}
	msg.npc_info.add_id = npc_detail().add_id;
	msg.npc_info.role_name = drops_detail_.name;
	msg.npc_info.coord = this->pixel_coord();
	msg.npc_info.hp_curr = 0;
	msg.npc_info.hp_max = 0;
	msg.npc_info.speed = 0;
	msg.npc_info.addition_prop = addition_prop();
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

int Drops::die_auto_get(void) {
//	Map_Player *player = MAP_MANAGER_INSTANCE->find_role_id_player(drops_detail_.auto_owner);
//	if (player) {
//		MSG_140015 msg;
//		msg.role_id = npc_id();
//		Drops_Detail::Drop_Map::iterator begin = drops_detail_.drops_map.begin();
//		Drops_Detail::Drop_Map::iterator end = drops_detail_.drops_map.end();
//		for (Drops_Detail::Drop_Map::iterator it = begin; it != end; ++it) {
//			msg.item_index_vec.push_back(it->first);
//		}
//
//		return player->get_drops_goods(msg);
//	}

	return -1;
}
//
//int Drops::die_process(const Attack_Info &info, Fighter &killer) {
//	if (npc_status() == RECOVER_STATUS || npc_status() == DEAD_STATUS) return -1;
//	NPC::die_process(info, killer);
//
//	return Fighter::die_process(info, killer);
//}

int Drops::fetch_drops_list(Scene_Player* player, Block_Buffer &res_buf) {
	if (!player) {
		return -1;
	}
	if (had_collect(player->role_id())) {
		return -1;
	}
	if(npc_record_){
		if(player && player->move_scene()){
			if(player->move_scene()->scene_type() == Team_Fb_Scene_Type
					|| player->move_scene()->scene_type() == HERO_DREAM_Scene_Type
					|| player->move_scene()->scene_type() == TEAM_DEFEND_Scene_Type){
				if(player->is_use_profile() == false){
					return ERROR_GET_BOX_NO_PROFIT;
				}
			}else{
				if(player->is_no_profit()){
					return ERROR_GET_BOX_NO_PROFIT;
				}
			}
		}
		if(npc_record_->config.type == TREASURE_BOX_SCENE_PASS){
			if(move_scene() && move_scene()->is_scene_pass() == false){
				return ERROR_GET_BOX_NO_PASS_SCENE;
			}
		}else if(npc_record_->config.type == TREASURE_BOX_VIP){
			if(player->vip() <= 0){
				return ERROR_GATHER_BOX_VIP_LIMIT;
			}
		}
	}
	MSG_50210000 msg;
	msg.role_id = this->role_id();
	Role_Drop_Map::iterator it_drops = drops_detail().role_drop_map.find(player->role_id());
	if (it_drops == drops_detail().role_drop_map.end()) {
		const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(npc_detail().npc_type_id);
		if (drop_config == NULL) return -1;
		Drop_Role_Detail drop_role_detail;
		NPC_MANAGER->process_drop_mode(player->career(), drop_role_detail.drops_vec, drop_config, 1, 1, 1, 1);
		drops_detail().role_drop_map.insert(std::make_pair(player->role_id(), drop_role_detail));

		it_drops = drops_detail().role_drop_map.find(player->role_id());
		if (it_drops == drops_detail().role_drop_map.end()) {
			MSG_TRACE_ABORT("ERROR: cant inert role_drops_map.");
		}
	}

	int count = it_drops->second.drops_vec.size();
	for(int i=0; i<count; ++i)
	{
		Drops_Item drops_item;
		drops_item.item_id = it_drops->second.drops_vec[i].item;
		drops_item.item_amount = it_drops->second.drops_vec[i].amount;
		drops_item.bind = it_drops->second.drops_vec[i].bind;
		msg.item_vec.push_back(drops_item);
	}
	res_buf.make_message(msg.msg_id);
	msg.serialize(res_buf);
	res_buf.finish_message();
	it_drops->second.open = true;
	return 0;
}
//
//int Drops::check_get_goods(MSG_140015 &msg, role_id_t role_id) {
//	if (drops_detail_.get_limit == 1 && drops_detail_.get_roles.count(role_id) > 0)
//		return ERROR_HAVE_GET_DROPS;
//
//	if (drops_detail_.get_limit == 2 && !FACTION_BOSS_MANAGER->is_get_drops(role_id))
//		return ERROR_HAVE_GET_DROPS;
//
//	//有限制时候，只返回一个掉落物，否则全部拾取
//	if (drops_detail_.get_limit > 0) {
//		int drop_first = msg.item_index_vec[0];
//		msg.item_index_vec.clear();
//		msg.item_index_vec.push_back(drop_first);
//		return 0;
//	}
//
//	for (std::vector<int32_t>::iterator it = msg.item_index_vec.begin(); it != msg.item_index_vec.end(); ++it) {
//		Drops_Detail::Drop_Map::iterator find_it = drops_detail_.drops_map.find(*it);
//		if (find_it == drops_detail_.drops_map.end()) {
//			// 提示非法操作,先屏蔽 以后再查
//			//return 1;
//			return ERROR_DROP_HAS_BELONG;
//		}
//
//		if (find_it->second.drops_base.belong != 0 && find_it->second.drops_base.belong != role_id) return ERROR_DROPS_OWNER;
//	}
//
//	return 0;
//}

bool Drops::is_drops(void) {
	return true;
}

Drops *Drops::drops_self(void) {
	return this;
}


int Drops::erase_drops_item(int item_index, role_id_t role_id, Fighter* player) {
	if(item_index == -2){
		//add item fail
		drops_detail_.drops_status = Drops_Detail::DROPS_STATUS_NORMAL;
		return 0;
	}
	Role_Drop_Map::iterator it_drops = drops_detail().role_drop_map.find(role_id);
	if (it_drops == drops_detail().role_drop_map.end()) {
		return -1;
	}
	Drop_Vec &drops_vec = it_drops->second.drops_vec;
	int count = drops_vec.size();
	if(item_index >= count ){
		return -1;
	}
	drops_detail_.drops_status = Drops_Detail::DROPS_STATUS_NORMAL;

	Int_Hash_Set need_broadcast;

	if( item_index == -1){
		//
		if(player && player->player_self() && player->move_scene()){
			Int_Int_Map scene_reward_map;
			int count = drops_vec.size();
			for(int i = 0; i < count; ++i){
				//vip
				if(drops_vec[i].item == PT_EXP_CURRENT){
					int vip_level = player->player_self()->vip();
					int vip_rate = CONFIG_CACHE_DUNGEON->get_single_copy_vip_rate(vip_level);
					int base_exp = drops_vec[i].amount;
					int prop_exp = base_exp*player->player_self()->exp_sum();
					int vip_exp = base_exp*(double)vip_rate/100;
					int exp = base_exp + prop_exp + vip_exp;
					scene_reward_map[drops_vec[i].item] += exp;
				}else{
					scene_reward_map[drops_vec[i].item] += drops_vec[i].amount;
					need_broadcast.insert(drops_vec[i].item);
				}
			}
			player->move_scene()->hook_award_player(player->player_self(), scene_reward_map, 0);
		}
		drops_vec.clear();
	}else{
		std::vector<Drop_One>::iterator it = drops_vec.begin();
		int index = 0;
		for(it = drops_vec.begin(); it != drops_vec.end(); ++it, ++index){
			if(index == item_index){
				//
				if(player && player->player_self() && player->move_scene()){
					Int_Int_Map scene_reward_map;
					//vip
					if(it->item == PT_EXP_CURRENT){
						int vip_level = player->player_self()->vip();
						int vip_rate = CONFIG_CACHE_DUNGEON->get_single_copy_vip_rate(vip_level);
						int base_exp = it->amount;
						int prop_exp = base_exp*player->player_self()->exp_sum();
						int vip_exp = base_exp*(double)vip_rate/100;
						int exp = base_exp + prop_exp + vip_exp;
						scene_reward_map[it->item] += exp;
					}else{
						scene_reward_map[it->item] += it->amount;
					}
					player->move_scene()->hook_award_player(player->player_self(), scene_reward_map, 0);
				}
				need_broadcast.insert(it->item);
				drops_vec.erase(it);
				break;
			}
		}
	}

	if (npc_record() && !npc_record()->config.npc_vec.empty()
			&& player && player->player_self() && Expedition_Type == get_scene_type(move_scene_id())) {
		Expedition_Scene *es = (Expedition_Scene*)this->move_scene();
		es->sync_refresh_open_box_to_logic(this->npc_record()->config.npc_vec[0].val_1, need_broadcast, player->player_self());
	}

	if(player){
		notice_get_drops(player, item_index);
	}
	// 精英副本通关，玩家捡完宝箱后才出传送阵
	if(npc_record_ && npc_record_->config.type == TREASURE_BOX_SCENE_PASS){
		if(move_scene()){
			move_scene()->reset_delay_show_ts_point_time();
			move_scene()->set_pass_status(Move_Scene::SCENE_PASS_STATUS_COMPELTE_RECEIVE, true);
		}
	}
	if(drops_vec.empty()){
		drops_detail().role_drop_map.erase(role_id);

		++cur_gt_times_;
		add_addition(role_id);

		if (cur_gt_times_ >= cfg_max_gt_times_) {
			this->recover_self();
		}
	}
	return 0;
}

int Drops::notice_get_drops(Fighter* player, int8_t item_index) {
	if(this->drops_detail().drop_type  == DROP_TYPE_SCATTER){
		Block_Buffer res_buf;
		MSG_50210002 msg;
		msg.result = 1;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}else{
		Block_Buffer res_buf;
		MSG_50210001 msg;
		msg.role_id = role_id();
		msg.item_index = item_index;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}

	return 0;
}

void Drops::add_addition(const role_id_t role_id) {
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
	addition.array.push_back(cur_gt_times_);
	addition.array.push_back(cfg_max_gt_times_);
	addition.int64_vec.push_back(role_id);
	set_addition(addition);

	if (this->npc_record()) {
		Expedition_Scene *es = (Expedition_Scene*)this->move_scene();
		es->add_role_collected(this->npc_record()->config.id, role_id);
	}
}

int Drops::recover_self(void) {
	if (npc_detail_.npc_status == RECOVER_STATUS) {
		MSG_DEBUG("npc is recover status");
		return 0;
	}
	//refresh
	if(this->npc_record()){
		this->npc_record()->clear_point();
	} else {
		disappear_time_ = Time_Value::gettimeofday();
		if(this->drops_detail().drop_type  == DROP_TYPE_SCATTER){
			if(this->move_scene()){
				Block_Buffer buf;
				MSG_81000014 msg;
				msg.role_id = role_id();
				MAKE_MSG_BUF(msg, buf);
				this->scene_broad_around(buf, SCENE_MOVE);
			}
		}
		exit_move_scene();
	}
	return 0;
}

int Drops::get_drops_goods_by_index(Scene_Player *sp,const int8_t index, std::vector<Drops_Item>& item_vec){
	if (!sp) {
		return -1;
	}
	Role_Drop_Map::iterator it_drops = drops_detail().role_drop_map.find(sp->role_id());
	if (it_drops == drops_detail().role_drop_map.end()) {
		const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(npc_detail().npc_type_id);
		Drop_Role_Detail drop_role_detail;
		if (drop_config != NULL) {
			NPC_MANAGER->process_drop_mode(sp->career(), drop_role_detail.drops_vec, drop_config, 1, 1, 1, 1);
		} else {
			drop_role_detail.drops_vec = this->drops_detail_.drops_vec;
		}
		drops_detail().role_drop_map.insert(std::make_pair(sp->role_id(), drop_role_detail));

		it_drops = drops_detail().role_drop_map.find(sp->role_id());
		if (it_drops == drops_detail().role_drop_map.end()) {
			return -1;
		}
	}

	Drop_Vec &drops_vec = it_drops->second.drops_vec;
	if(index == -1){
		int count = drops_vec.size();
		for(int i=0; i<count; ++i){
			Drops_Item item;
			item.item_id = it_drops->second.drops_vec[i].item;
			item.item_amount = it_drops->second.drops_vec[i].amount;
			item.bind = it_drops->second.drops_vec[i].bind;
			item_vec.push_back(item);
		}
	} else {
		int count = it_drops->second.drops_vec.size();
		if(index>count){
				return -1;
		}
		Drops_Item item;
		item.item_id = it_drops->second.drops_vec[index].item;
		item.item_amount = it_drops->second.drops_vec[index].amount;
		item.bind = it_drops->second.drops_vec[index].bind;
		item_vec.push_back(item);

	}

	return 0;
}

void Drops::set_box_open(bool value){
	this->drops_detail().open = value;
}

void Drops::hook_scene_pass(int scene_id){
	if(npc_record_ && npc_record_->config.type == TREASURE_BOX_SCENE_PASS){
		set_box_open(true);
	}
}

bool Drops::is_scene_pass_box(void){
	if(npc_record_ && npc_record_->config.type == TREASURE_BOX_SCENE_PASS){
		return true;
	}
	return false;
}
