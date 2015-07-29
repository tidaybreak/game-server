/*
 * machine.cpp
 *
 *  Created on: 2014年5月5日
 *      Author: linyisheng
 */



#include "Machine.h"
#include "Move_Scene.h"
#include "NPC_Manager.h"
#include "ai_machine/AI_Machine.h"
#include "Msg_Active_Struct.h"
#include "Config_Cache_NPC.h"
#include "Err_Code.h"
#include "Utility_Func.h"
#include "Msg_Inner_Struct.h"
#include "Scene_Player.h"
#include "Msg_NPC_Struct.h"
#include "status/Status.h"
#include "status/Status_Struct.h"
#include "team/Scene_Team_Manager.h"
#include "Scene_Public.h"



Machine::Machine(void):gather_status(0), plot_step_(0),gather_player_role_id_(0),
	 acc_role_id_(0), check_out_range_(false),lift_return_(false),tick_count_(0) {

}

Machine::~Machine(void) { }

int Machine::birth(role_id_t npc_id, int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info) {
	if (npc_addition_info.birth_coord.x == 0 && npc_addition_info.birth_coord.y == 0) {
		MSG_USER("machine:%d birth error coord:%d %d error", npc_type_id, npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
		return -1;
	}

//	if (scene->mpt_value_by_coord(npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y) < Move_Scene::MOVE_ABLE_FROM) {
//		MSG_USER("machine:%d birth error coord:%d %d error", npc_type_id, npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y);
//		return -1;
//	}

	npc_detail_.npc_id = npc_id;
	int ret = this->init(npc_type_id, scene, point, npc_addition_info);
	if (ret != 0) {
		MSG_USER("machine birth error.");
		return -1;
	}

	/// ai init
	//this->init_bt();
	//create accessory
	if(point && point->config.acc_id != 0){
		NPC_Addition_Info acc_add_info;
		acc_add_info.birth_coord = point->config.acc_coord;
		this->create_accessory(point->config.acc_id, scene, NULL, acc_add_info);
		//

	}
	enter_move_scene(scene);
	move_appear();

	return 0;
}

int Machine::init(int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info) {
	const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(npc_type_id);
	if (config_cache == 0) {
		MSG_USER("find npc config:%d error", npc_type_id);
		return -1;
	}

	machine_detail_.config_cache = config_cache;

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
	machine_detail_.jid = npc_addition_info.jid ? npc_addition_info.jid : config_cache->jid;
	machine_detail_.name = (npc_addition_info.name != "") ? npc_addition_info.name : config_cache->name;
	if(point){
		set_npc_operation_type(point->config.type);
	}else{
		set_npc_operation_type(OPERATE_NONE);
	}
	npc_detail_.fighter_detail_.level = config_cache->level;
	return 0;
}

int Machine::fini(void) {
	NPC::fini();

	reset();

	return 0;
}

int Machine::reset(void) {
	NPC::reset();
	machine_detail_.reset();
	gather_status = 0;
	plot_step_ = 0;
	plot_player_vec_.clear();
	acc_role_id_  = 0;
	take_lift_player_map_.clear();
	check_out_range_ = false;
	lift_return_ = false;
	tick_count_ = 0;
	return 0;
}

int Machine::move_appear_info(Block_Buffer &buf) {
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	MSG_81000005 msg;
	msg.npc_info.role_id = role_id();
	msg.npc_info.master_direct = this->acc_role_id_;
	msg.npc_info.type = npc_operation_type();
	msg.npc_info.state = npc_operation_state();
	msg.npc_info.npc_type = npc_detail().npc_type_id;
	msg.npc_info.npc_jid = machine_detail_.jid;
	msg.npc_info.role_name = machine_detail_.name;
	msg.npc_info.title_id = npc_detail().title_id;
	msg.npc_info.coord = this->pixel_coord();
	msg.npc_info.hp_curr = 0;
	msg.npc_info.hp_max = 0;
	msg.npc_info.speed = 0;
	msg.npc_info.add_id = npc_detail().add_id;
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

//
//int Machine::die_process(const Attack_Info &info, Fighter &killer) {
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

int Machine::notice_logic_die(role_id_t attacker) {
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

bool Machine::is_machine(void) {
	return true;
}

Machine *Machine::machine_self(void) {
	return this;
}
int Machine::tick(Time_Value &now){
	if(tick_count_ > 0){
		--tick_count_;
		return 0;
	}
	tick_count_ = 3;
	if(this->plot_step_ == PLOT_STEP_1){
		if(check_player_in_plot()){
			return 0;
		}
		this->plot_step_ = 0;
		plot_player_vec_.clear();
		//operate gather
		Mover* mover = this->move_scene()->find_mover_with_layer(0, gather_player_role_id_);
		if(mover && mover->player_self()){
			this->operate_start_gather(mover->player_self(), false);
		}else{
			this->del_gather_status();
		}
		return 0;
	}
	if(this->plot_step_ == PLOT_STEP_2){
		if(check_player_in_plot()){
			return 0;
		}
		this->plot_step_ = 0;
		plot_player_vec_.clear();
		//operate gather
		Mover* mover = this->move_scene()->find_mover_with_layer(0, gather_player_role_id_);
		if(mover && mover->player_self()){
			this->operate_end_gather(mover->player_self(), true);
		}else{
			this->del_gather_status();
		}
		return 0;
	}

	if(npc_operation_type() == OPERATE_AUTO){
		check_event_trigger(false);
		return 0;
	}else if(npc_operation_type() == OPERATE_AUTO_SECTOR){
		check_event_trigger(true);
		return 0;
	}else if(npc_operation_type() == OPERATE_AUTO_TEAM){
		check_event_team_trigger(false);
		return 0;
	}else if(npc_operation_type() == OPERATE_AUTO_SECTOR_TEAM){
		check_event_team_trigger(true);
		return 0;
	}else if(npc_operation_type() == OPERATE_GAHTER){
		check_gathering_player(now);
		return 0;
	}
	return 0;
}

void Machine::check_event_team_trigger(bool sector/* = false*/){
	if(move_scene() == NULL || npc_record_ == NULL){
		return;
	}
	//check cd
	Time_Value now = Time_Value::gettimeofday();
	if(this->npc_record()->check_gt_cd(now) == false){
		return;
	}
	std::vector<Scene_Player*> target_vec;
	Scene_Player_Set player_set;
	if(sector){
		Coord ard_mid;
		ard_mid.x = this->grid_coord().x+npc_record_->config.view_range/32;
		ard_mid.y = this->grid_coord().y+npc_record_->config.move_range/32;
		int degree = npc_record_->config.chase_range;
		if(check_out_range_){
			degree += 10;
		}
		move_scene()->collect_sector_fight_player(mover_detail().layer_id, mover_detail().grid_coord,
				ard_mid.grid_coord(), degree, player_set, check_out_range_);
	}else{
		int range = npc_record_->config.view_range;
		if(check_out_range_){
			range += 32;
		}
		move_scene()->collect_round_fight_player(mover_detail().layer_id, mover_detail().grid_coord,
			player_set, range);
	}
	target_vec.assign(player_set.begin(), player_set.end());
	if(check_out_range_){
		if(target_vec.empty()){
			check_out_range_ = false;
		}else{
			role_id_t team_id = target_vec[0]->teamer_detail().team_id;
			Map_Team *team = SCENE_TEAM_MANAGER->get_team(team_id);
			if(!team){
				return;
			}
			int target_count = target_vec.size();
			int team_num = team->get_all_team_member_nums();
			if(target_count < team_num){
				check_out_range_ = false;
			}
		}
		return;
	}
	if(target_vec.empty() == false){
		role_id_t team_id = target_vec[0]->teamer_detail().team_id;
		Map_Team *team = SCENE_TEAM_MANAGER->get_team(team_id);
		if(!team){
			return;
		}
		int target_count = target_vec.size();
		int team_num = team->get_all_team_member_nums();
		if(target_count == team_num){
			std::vector<Scene_Player*> trigger_vec;
			trigger_vec.push_back(target_vec[0]);
			event_trigger(trigger_vec);
		}
	}
}

void Machine::check_event_trigger(bool sector/* = false*/){
	if(move_scene() == NULL || npc_record_ == NULL){
		return;
	}
	//check cd
	Time_Value now = Time_Value::gettimeofday();
	if(this->npc_record()->check_gt_cd(now) == false){
		return;
	}
	std::vector<Scene_Player*> target_vec;
	Scene_Player_Set player_set;
	if(sector){
		Coord ard_mid;
		ard_mid.x = this->grid_coord().x+npc_record_->config.view_range/32;
		ard_mid.y = this->grid_coord().y+npc_record_->config.move_range/32;
		int degree = npc_record_->config.chase_range;
		if(check_out_range_){
			degree += 10;
		}
		move_scene()->collect_sector_fight_player(mover_detail().layer_id, mover_detail().grid_coord,
				ard_mid.grid_coord(), degree, player_set, check_out_range_);
	}else{
		int range = npc_record_->config.view_range;
		if(check_out_range_){
			range += 32;
		}
		move_scene()->collect_round_fight_player(mover_detail().layer_id, mover_detail().grid_coord,
			player_set, range);
	}
	target_vec.assign(player_set.begin(), player_set.end());
	if(check_out_range_){
		if(target_vec.empty()){
			check_out_range_ = false;
		}
		return;
	}
	if(target_vec.empty() == false){
		event_trigger(target_vec);
	}
}

int Machine::event_trigger(std::vector<Scene_Player*>& target_vec){
	if(this->npc_record() == NULL){
		return -1;
	}
	//check times
	if(npc_record()->config.gt_times != 0 &&
			npc_record()->cur_gt_times >= abs(npc_record()->config.gt_times)){// 采集次数用完
		return -1;
	}
	//check cd
	Time_Value now = Time_Value::gettimeofday();
	if(this->npc_record()->check_gt_cd(now) == false){
		return ERROR_START_GATHER_BE_CD;
	}
	//execute event
	std::vector<Scene_Player*>::iterator it;
	for(it = target_vec.begin(); it != target_vec.end(); ++it){
		event_execute(this->machine_event_id(), (*it));
	}
	//set cd
	this->npc_record()->set_cur_gt_cd(now);
	//times
	if(this->npc_record()->config.gt_times != 0){
		this->npc_record()->cur_gt_times += 1;
		if(this->npc_record()->config.gt_times > 0 &&
				this->npc_record()->cur_gt_times >= this->npc_record()->config.gt_times){
			//recover
			if(acc_role_id_ != 0){
				this->destory_accessory(acc_role_id_);
			}else{
				const Machine_Event_Config *event;
				event = CONFIG_CACHE_NPC->find_machine_event_config(this->machine_event_id());
				if(event && event->type == MACHINE_EVENT_MOVEABLE){
					if(this->move_scene()){
						Block_Buffer buf;
						MSG_81000014 msg;
						msg.role_id = role_id();
						MAKE_MSG_BUF(msg, buf);
						this->scene_broad_around(buf, SCENE_MOVE);
					}
				}
			}
			this->npc_record()->clear_point();
		}
	}
	return 0;
}

int Machine::event_execute(int id, Scene_Player* target){
	const Machine_Event_Config *event = CONFIG_CACHE_NPC->find_machine_event_config(id);
	if (event == 0) {
		MSG_USER("find machine event config:%d error", id);
		return -1;
	}
	switch(event->type){
		case MACHINE_EVENT_MOVEABLE:{
			if(this->move_scene() == NULL){
				return -1;
			}
			if(event->val_2 != -1){
				int mp_value = event->val_1;
				bool moveable = event->val_2==0?false:true;
				this->move_scene()->change_move_value(mp_value, moveable);
			}
			if(target){
				if(event->words != ""){
					MSG_81003000 msg;
					msg.text = event->words;
					msg.type = 0;
					OBJ_SEND_TO_CLIENT(msg, (*target));
				}
			}
			break;
		}
		case MACHINE_EVENT_REF_MONSTER:{
			Move_Scene* move_scene = target->move_scene();
			if(move_scene){
				NPC_Record* point = move_scene->get_npc_record(event->val_1);
				if(point){
					NPC_MANAGER->record_refresh(point);
				}
			}
			break;
		}
		case MACHINE_EVENT_BEAK_BACK:{
			Move_Scene* move_scene = target->move_scene();
			if(move_scene){
				Coord point_a = this->mover_detail().grid_coord;
//				Coord point_b = target->mover_detail().grid_coord;
				int distance = event->val_1;
//				Coord point_c = point_b;
//				move_scene->calc_point_in_line_by_b(*target, point_a, point_b,
//						distance, point_c, this->mover_detail().toward, true, true);
//				move_scene->mover_change_coord(target, point_c, false);
				//broadcast to player
				MSG_81000024 msg;
				msg.pos_x = point_a.pixel_coord().x;
				msg.pos_y = point_a.pixel_coord().y;
				msg.distance = distance;
				OBJ_SEND_TO_CLIENT(msg, (*target));
				if(event->words != ""){
					MSG_81003000 msg;
					msg.text = event->words;
					msg.type = 0;
					OBJ_SEND_TO_CLIENT(msg, (*target));
				}
			}
			break;
		}
		case MACHINE_EVENT_BUFFER:{
			Status_Effect_Argv argv;
			argv.caster_id = this->role_id();
			argv.status_id = event->val_1;
			argv.last_time_ms = event->val_2;
			Effect_Argv_Prop prop;
			prop.i_effect_val1 = event->val_3;
			prop.d_effect_val1 = event->val_4;
			prop.d_effect_val2 = event->val_5;
			argv.data.push_back(prop);
			target->insert_status_out_fight(argv);
			if(event->words != ""){
				MSG_81003000 msg;
				msg.text = event->words;
				msg.type = 0;
				OBJ_SEND_TO_CLIENT(msg, (*target));
			}
			break;
		}
		case MACHINE_EVENT_TAKE_LIFT:{
			std::vector<Scene_Player*> player_vec;
			if(event->val_1 == 0){//单人电梯
				player_vec.push_back(target);
			}else{//组队电梯
				role_id_t team_id = target->teamer_detail().team_id;
				Map_Team *team = SCENE_TEAM_MANAGER->get_team(team_id);
				if(!team){
					return ERROR_TAKE_LIFT_NO_TEAM;
				}
				int offline_num = team->get_offline_team_member_nums();
				if(offline_num > 0){
					return ERROR_TAKE_LIFT_TEAMER_OFFLINE;
				}

				Role_Hash_Set team_set;
				team->team_all_member_set(team_set);
				Role_Hash_Set::iterator it;
				for(it = team_set.begin(); it != team_set.end(); ++it){
					Mover* mover = move_scene()->find_mover_with_layer(0, *it);
					if(!mover || mover->player_self() == NULL){
						return ERROR_TAKE_LIFT_TEAM_NOT_COMPLETE;
					}
					player_vec.push_back(mover->player_self());
				}
			}
			// check
			int view_range = this->npc_record_->config.view_range;
			Mover* acc_mover = NULL;
			if(npc_operation_type() == OPERATE_CLICK || npc_operation_type() == OPERATE_GAHTER){
				if(acc_role_id_ == 0 ){
					return -1;
				}
				acc_mover = move_scene()->find_mover_with_layer(0, acc_role_id_);
				if(acc_mover == NULL || acc_mover->machine_self() == NULL){
					return -1;
				}
				if(acc_mover->machine_self()->npc_operation_type() == OPERATE_AUTO_SECTOR ||
						acc_mover->machine_self()->npc_operation_type() == OPERATE_AUTO_SECTOR_TEAM){
					Coord arc_mid;
					arc_mid.x = acc_mover->grid_coord().x+npc_record_->config.view_range/32;
					arc_mid.y = acc_mover->grid_coord().y+npc_record_->config.move_range/32;
					int degree = npc_record_->config.chase_range;
					int player_count = player_vec.size();
					for(int i=0; i<player_count; ++i){
						if(is_in_sector(acc_mover->grid_coord(), arc_mid, player_vec[i]->grid_coord(), degree, false) == false){
							return ERROR_TAKE_LIFT_TEAMER_OUT_RANGE;
						}
					}
				}else{
					int player_count = player_vec.size();
					for(int i=0; i<player_count; ++i){
						if(calculate_pixels_distance(player_vec[i]->grid_coord(), this->grid_coord()) > view_range){
							return ERROR_TAKE_LIFT_TEAMER_OUT_RANGE;
						}
					}
				}

			}

			int lift_distance = event->val_2/32*32;
			if(lift_return_){
				lift_distance = -lift_distance;
			}
			MSG_81000027 msg;
			if (lift_distance < 0) {
				msg.out_x = event->coord_1.x;
				msg.out_y = event->coord_1.y;
			} else {
				msg.out_x = event->coord_2.x;
				msg.out_y = event->coord_2.y;
			}
			msg.distance = lift_distance;
			msg.lift_id = this->role_id();
			int player_count = player_vec.size();
			for(int i=0; i<player_count; ++i){
				msg.player_vec.push_back(player_vec[i]->role_id());
				Hero *hero = SCENE_PUBLIC->find_mover_hero(player_vec[i]->role_id());
				if(hero){
					msg.player_vec.push_back(hero->role_id());
				}
			}
			if(npc_record_){
				msg.arc_mid_x = npc_record_->config.view_range;
				msg.arc_mid_y = npc_record_->config.move_range;
				msg.degree = npc_record_->config.chase_range;
			}
			for(int i=0; i<player_count; ++i){
				OBJ_SEND_TO_CLIENT(msg, (*player_vec[i]));
				player_vec[i]->set_take_lift_begin();
				take_lift_player_map_.insert(std::make_pair(player_vec[i]->role_id(), false));
			}

			//设置位置
			Coord target_pos;
			for(int i=0; i<player_count; ++i){
				target_pos = player_vec[i]->mover_detail().grid_coord;
				target_pos.y += lift_distance/32;
				if(move_scene()){
					move_scene()->mover_change_coord(player_vec[i], target_pos, false);
				}
			}
			target_pos = this->mover_detail().grid_coord;
			target_pos.y += lift_distance/32;
			if(this->move_scene()){
				this->move_scene()->mover_change_coord(this, target_pos, false);
			}
			if(acc_role_id_ != 0){
				Mover* mover_acc = move_scene()->find_mover_with_layer(0, acc_role_id_);
				if(mover_acc){
					target_pos = mover_acc->mover_detail().grid_coord;
					target_pos.y += lift_distance/32;
					this->move_scene()->mover_change_coord(mover_acc, target_pos, false);
				}
			}
			// 需要出去检测范围后，检测范围才生效
			check_out_range_ = true;
			lift_return_ = !lift_return_;
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Machine::event_execute(int id, role_id_t role_id){
	if(this->move_scene() == NULL){
		return -1;
	}
	Mover* mover = this->move_scene()->find_mover_with_layer(0, role_id);
	if(mover && mover->player_self()){
		return event_execute(id, mover->player_self());
	}
	return -1;
}

int Machine::open_machine(Scene_Player* player){
	std::vector<Scene_Player*> target_vec;
	target_vec.push_back(player);
	int result = event_trigger(target_vec);
	return result;
}

int Machine::start_gather(Scene_Player* player){
	int result = 0;
	if(this->recover_state()){
		result = -1;//ERROR_GATHER_ID_NONEXIST;
	}
	if(gather_status >0){
		result =  ERROR_GATHER_MULT_LIMIT;
	}
	//check times
	if(npc_record() && npc_record()->config.gt_times != 0 &&
			npc_record()->cur_gt_times >= abs(npc_record()->config.gt_times)){// 采集次数用完
		result = -1;
	}
	if(result != 0){
		//send gather fail to client
		MSG_50210100 msg;
		msg.material_role_id = this->role_id();
		msg.result = 0;
		OBJ_SEND_TO_CLIENT(msg, (*player));
		return -1;
	}
	std::string plot_id = "";
	if(this->npc_record() != NULL){
		plot_id = this->npc_record()->config.start_plot;
		//check cd
		Time_Value now = Time_Value::gettimeofday();
		if(this->npc_record()->check_gt_cd(now) == false){
			return ERROR_START_GATHER_BE_CD;
		}
	}
	if(plot_id != ""){
		this->operate_plot_step1(player, plot_id);
	}else{
		this->operate_start_gather(player, true);
	}
	return result;
}

int Machine::stop_gather(role_id_t role_id){
	if(this->recover_state()){
		return -1;
		//return ERROR_STOP_GATHER_ID_NONEXIST;
	}
	if(this->plot_step_ != 0){
		return ERROR_STOP_GATHER_NONGATHER;
	}
	Gather_Player_Map::iterator it;
	it = gather_player_map.find(role_id);
	if(it == gather_player_map.end()){
		return ERROR_STOP_GATHER_NONGATHER;
	}
	gather_player_map.erase(role_id);
	del_gather_status();
	return 0;
}

int Machine::gather_time(void){
	if(this->npc_record()){
		return this->npc_record()->config.gather_time;
	}
	return 0;
}

int Machine::machine_event_id(void){
	if(npc_record() && npc_record()->config.events_vec.empty() == false){
		int count = npc_record()->config.events_vec.size();
		int index = random()%count;
		return npc_record()->config.events_vec[index];
	}
	return 0;
}

bool Machine::check_player_in_plot(void){
	bool result = false;
	std::vector<role_id_t>::iterator it;
	for(it = plot_player_vec_.begin(); it != plot_player_vec_.end(); ++it){
		Mover* mover = this->move_scene()->find_mover_with_layer(0, (*it));
		if(mover && mover->player_self()){
			if(mover->player_self()->is_in_plot()){
				result = true;
				break;
			}
		}
	}
	return result;
}

void Machine::check_gathering_player(const Time_Value& now){
	Gather_Player_Map::iterator it;
	for(it = gather_player_map.begin() ; it != gather_player_map.end(); ){
		if(now > it->second){
			Mover* mover = this->move_scene()->find_mover_with_layer(0, it->first);
			if(mover && mover->player_self()){
				std::string plot_id = "";
				if(this->npc_record()){
					plot_id = this->npc_record()->config.end_plot;
				}
				if(plot_id != ""){
					operate_plot_step2(mover->player_self(), plot_id);
				}else{
					this->operate_end_gather(mover->player_self(), true);
				}
			}else{
				del_gather_status();
			}
			it = gather_player_map.erase(it);
		}else{
			++it;
		}
	}
}

int Machine::operate_start_gather(Scene_Player* player, bool add_gather_status/* = true*/){
	Time_Value end_time = Time_Value::gettimeofday() +
			Time_Value(this->gather_time(), 0);
	gather_player_map.insert(std::make_pair(player->role_id(), end_time));
	MSG_50210100 msg;
	msg.material_role_id = this->role_id();
	msg.gather_time = this->gather_time();
	msg.result = 1;
	OBJ_SEND_TO_CLIENT(msg, (*player));
	if(add_gather_status){
		this->add_gather_status();
	}
	return 0;
}

int Machine::operate_end_gather(Scene_Player* player, bool del_gather_status/* = true*/){

	{
		MSG_81000110 msg;
		msg.material_role_id = this->role_id();
		msg.result = 1;
		OBJ_SEND_TO_CLIENT(msg, (*player));
		hook_gather_act();
	}
	if(del_gather_status){
		this->del_gather_status();
	}
	std::vector<Scene_Player*> target_vec;
	target_vec.push_back(player);
	this->event_trigger(target_vec);
	return 0;
}

int Machine::operate_plot_step1(Scene_Player* player, std::string plot_id){
	this->plot_step_ = PLOT_STEP_1;
	this->gather_player_role_id_ = player->role_id();
	this->plot_player_vec_.push_back(player->role_id());
	this->add_gather_status();
	player->play_plot(plot_id, 0, true);
	return 0;
}

int Machine::operate_plot_step2(Scene_Player* player, std::string plot_id){
	this->plot_step_ = PLOT_STEP_2;
	this->gather_player_role_id_ = player->role_id();
	this->plot_player_vec_.push_back(player->role_id());
	player->play_plot(plot_id, 0, true);
	return 0;
}

int Machine::create_accessory(int npc_type_id, Move_Scene *scene, NPC_Record* point, NPC_Addition_Info &npc_addition_info){
	Machine* accessory = NPC_MANAGER->create_machine(npc_type_id, scene, point, npc_addition_info);
	if(!accessory){
		return -1;
	}
	this->acc_role_id_ = accessory->role_id();
	return 0;
}

int Machine::destory_accessory(role_id_t role_id){
	Machine* accessory = NPC_MANAGER->find_machine_npc(role_id);
	if(!accessory){
		return -1;
	}

	if(this->move_scene()){
		Block_Buffer buf;
		MSG_81000014 msg;
		msg.role_id = accessory->role_id();
		MAKE_MSG_BUF(msg, buf);
		this->scene_broad_around(buf, SCENE_MOVE);
	}
	accessory->exit_move_scene();
	return 0;
}
int Machine::beat_back(Scene_Player* player, const Coord &player_coord){
	int id = this->machine_event_id();
	const Machine_Event_Config *event = CONFIG_CACHE_NPC->find_machine_event_config(id);
	if(!event){
		return -1;
	}
	Move_Scene* move_scene = player->move_scene();
	if(move_scene){
		Coord point_a = this->mover_detail().grid_coord;
		Coord point_b = player_coord;
		int distance = event->val_1;
		Coord point_c = point_b;
		move_scene->calc_point_in_line_by_b(*player, point_a, point_b,
	   distance, point_c, this->mover_detail().toward, true, true);
	   move_scene->mover_change_coord(player, point_c, false);
		//broadcast to player
		Block_Buffer buf;
		MSG_81000023 msg;
		msg.role_id = player->role_id();
		msg.pos_x = point_c.pixel_coord().x;
		msg.pos_y = point_c.pixel_coord().y;
		MAKE_MSG_BUF(msg, buf);
		this->scene_broad_around(buf, SCENE_MOVE);
	}
	return 0;
}

int Machine::take_lift_finish(Scene_Player* player){
	Take_Lift_Player_Map::iterator it = take_lift_player_map_.find(player->role_id());
	if(it != take_lift_player_map_.end()){
		it->second = true;
	}
	if(all_take_lift_finish()){
		Take_Lift_Player_Map::iterator it;
		for(it = take_lift_player_map_.begin(); it != take_lift_player_map_.end(); ++it){
			Mover* mover = this->move_scene()->find_mover_with_layer(0, it->first);
			if(mover && mover->player_self()){
				mover->player_self()->set_take_lift_finish();
				mover->move_disappear();
				mover->move_appear();
				MSG_81000028 msg;
				msg.lift_id = this->role_id();
				OBJ_SEND_TO_CLIENT(msg, (*mover));
			}
		}
		take_lift_player_map_.clear();
	}
	return 0;
}

bool Machine::all_take_lift_finish(void){
	bool result = true;
	Take_Lift_Player_Map::iterator it;
	for(it = take_lift_player_map_.begin(); it != take_lift_player_map_.end(); ++it){
		if(it->second == false){
			Mover* mover = this->move_scene()->find_mover_with_layer(0, it->first);
			if(mover && mover->player_self() && mover->player_self()->is_take_lift()){
				result = false;
				break;
			}else{
				it->second = true;
			}
		}
	}
	return result;
}

bool Machine::is_lift(void){
	const Machine_Event_Config *event;
	event = CONFIG_CACHE_NPC->find_machine_event_config(this->machine_event_id());
	if(event){
		if(event->type == MACHINE_EVENT_TAKE_LIFT){
			return true;
		}
	}
	return false;
}

void Machine::lift_reset(void){
	if(this->lift_return_ == false){
		return;
	}
	const Machine_Event_Config *event;
	event = CONFIG_CACHE_NPC->find_machine_event_config(this->machine_event_id());
	if(!event){
		return;
	}
	if(event->type != MACHINE_EVENT_TAKE_LIFT){
		return;
	}
	//设置位置
	int lift_distance = event->val_2;
	Coord target_pos;
	target_pos = this->mover_detail().grid_coord;
	target_pos.y -= lift_distance/32;
	if(this->move_scene()){
		this->move_scene()->mover_change_coord(this, target_pos, false);
	}
	if(acc_role_id_ != 0){
		Mover* mover_acc = move_scene()->find_mover_with_layer(0, acc_role_id_);
		if(mover_acc){
			target_pos = mover_acc->mover_detail().grid_coord;
			target_pos.y -= lift_distance/32;
			this->move_scene()->mover_change_coord(mover_acc, target_pos, false);
		}
	}
	// 需要出去检测范围后，检测范围才生效
	check_out_range_ = false;
	lift_return_ = false;
}
