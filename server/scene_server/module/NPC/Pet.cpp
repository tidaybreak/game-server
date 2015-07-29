/*
 * Pet.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Pet.h"
#include "Move_Scene.h"
//#include "Map_Manager.h"
#include "NPC_Manager.h"
//#include "Map_Player.h"
//#include "Map_Pool_Repository.h"

Pet::Pet(void) :
pet_die_(false),
is_register_timer_(false),
is_sync_(false)
{

}

Pet::~Pet(void) { }

//Pet_Base_Detail const &Pet::pet_base_detail(void) {
//	return pet_base_detail_;
//}

int Pet::init(Move_Scene *scene) {
//	if (!scene || this->dead_status())
//		return -1;
//
//	Map_Player *player = this->master();
//	if (!player)
//		return -2;
//
//	set_pet_camps(player->fighter_camps());
//	set_pet_country(player->country());
//	set_pet_team_id(player->team_id());
//	set_pet_faction_id(player->fighter_detail().faction_id);
//	set_pet_fight_mode(player->fighter_mode());
//	set_layer_id(player->mover_detail().layer_id);
//
//	npc_detail_.npc_id = pet_base_detail_.pet_role_id;
//	npc_detail_.direct_master = pet_base_detail_.master_role_id;
//	status_detail_.role_id = fighter_detail_.role_id;
//	fighter_detail_.speed_map.insert(std::make_pair(0, player->speed_value()));
//
//	Grid_Coord player_coord = player->mover_detail().grid_coord;
//	scene->collect_available_coord(mover_detail().layer_id, player_coord, pet_base_detail_.coord, 1, 2);
//	mover_set(pet_base_detail_.pet_role_id, 0, pet_base_detail_.coord, scene, MOVER_PET);
//	for (Moveable_Value::iterator iter = player->moveable_value().begin();
//			iter != player->moveable_value().end(); ++iter) {
//		add_moveable_value(*iter);
//	}
//
//	pet_die_ = false;
//	tick_time_ = Time_Value::gettimeofday();
//	is_sync_ = false;
//
//	register_timer();
//	skill_insert_status();

	return 0;
}

int Pet::reset(void) {
	NPC::reset();
//	pet_base_detail_.reset();
	pet_die_ = false;
	tick_time_ = Time_Value::gettimeofday();
	is_register_timer_ = false;
	is_sync_ = false;

	return 0;
}

//void Pet::make_pet_appear_info(MSG_800006 &msg) {
//	msg.reset();
//
//	msg.pet_info.role_id = pet_base_detail_.pet_role_id;
//	msg.pet_info.master_id = pet_base_detail_.master_role_id;
//	msg.pet_info.master_name = pet_base_detail_.master_role_name;
//	msg.pet_info.name = pet_base_detail_.name;
//	msg.pet_info.pet_id = pet_base_detail_.pet_id;
//	msg.pet_info.pet_cid = pet_base_detail_.pet_cid;
//	msg.pet_info.px = mover_detail_.grid_coord.x;
//	msg.pet_info.py = mover_detail_.grid_coord.y;
//	msg.pet_info.level = fighter_detail_.level;
//	msg.pet_info.hp = fighter_detail_.fetch_property(Fighter_Detail::HP);
//	msg.pet_info.hp_max = fighter_detail_.fetch_property(Fighter_Detail::HP_MAX);
//	msg.pet_info.growth = pet_base_detail_.growth;
//	msg.pet_info.quality = pet_base_detail_.quality;
//	if (pet_base_detail_.genius > 0) {
//		//获得天赋的主动技能id
//		int active_id = CONFIG_INSTANCE->fighter_skill(pet_base_detail_.genius)["active_id"].asInt();
//		msg.pet_info.skill_vector.push_back(active_id);
//
//		int remain_sec = 0;
//		const Skill_Config_Cache *cache = CONFIG_CACHE_INSTANCE->find_skill_config_cache(active_id);
//		Map_Player *player = this->master();
//		if (cache && player) {
//			if (!player->fighter_detail().temp_skill_db_info_map.count(active_id)) {
//				Skill_DB_Info info;
//				info.skill_id = active_id;
//				player->fighter_detail().temp_skill_db_info_map.insert(std::make_pair(info.skill_id, info));
//			} else {
//				Skill_Running_Info_Map::const_iterator iter = player->fighter_detail().skill_running_info_map.find(active_id);
//				if (iter != player->fighter_detail().skill_running_info_map.end()) {
//					remain_sec = Utility_Func::get_skill_remain_sec(*cache, Time_Value::gettimeofday(), iter->second.last_use_timestamp);
//				}
//			}
//		}
//		msg.pet_info.skill_vector.push_back(remain_sec);
//	}
//	msg.pet_info.camps = fighter_detail_.camps;
//	if (msg.pet_info.camps == 0) {
//		Map_Player *player = this->master();
//		if (player) {
//			msg.pet_info.camps = player->fighter_camps();
//		}
//	}
//
//	msg.pet_info.fight_mode = fighter_detail_.fight_mode;
//}

int Pet::make_appear_info(Block_Buffer &buf) {
//	size_t wr_begin_idx = buf.get_write_idx();
//	size_t wr_end_idx = 0;
//	uint16_t len = 0;
//
//	/// fill head
//	buf.write_uint16(0);						/// len
//	buf.write_uint32(ACTIVE_PET_APPEAR);		/// msg_id
//	buf.write_uint32(0);						/// status
//
//	MSG_800006 msg;
//	make_pet_appear_info(msg);
//	msg.serialize(buf);
//
//	/// refill len in head
//	wr_end_idx = buf.get_write_idx();
//	len = wr_end_idx - wr_begin_idx - sizeof(uint16_t);
//	buf.set_write_idx(wr_begin_idx);
//	buf.write_uint16(len);
//	buf.set_write_idx(wr_end_idx);

	return 0;
}

int Pet::pet_recycle(int type) {
//	sync_pet_fighter_detail_to_logic();
//
//	if (this->validate_mover_detail()) {
//		move_disappear();
//	}
//
//	if (type == CALL_BACK) {
//		skill_erase_status();
//		sync_pet_call_back_to_logic();
//	} else if (type == DEAD) {
//		skill_erase_status();
//		sync_pet_die_to_logic();
//	}
//
//	NPC_MANAGER->pet_map().erase(pet_base_detail_.pet_role_id);
//	MAP_MANAGER_INSTANCE->unbind_master_id_pet(pet_base_detail_.master_role_id);
//
//	reset();
//	MAP_MANAGER_INSTANCE->pool_repo().push_pet(this);
	return 0;
}

int Pet::register_timer(void) {
	is_register_timer_ = true;
	return 0;
}

int Pet::unregister_timer(void) {
	is_register_timer_ = false;
	return 0;
}

int Pet::time_up(Time_Value &now) {
//	if (! is_register_timer_)
//		return 0;
//
//	Status::time_up(now);
//
//	if (pet_die_) {
//		pet_recycle(Pet::DEAD);
//		pet_die_ = false;
//	}
//
//	if (now - tick_time_ > Time_Value(60)) {
//		tick_time_ = now;
//
//		if (is_sync_) {
//			is_sync_ = false;
//			sync_pet_info_to_logic();
//		}
//
//		/**
//		 * 此处不用MAP_MANAGER_INSTANCE->find_role_id_player是因为:
//		 * 主人可能在该地图进程里面，但是不在该场景宠物附近，这个时候宠物也是找不到主人，要收回宠物
//		 */
//		Fighter *fighter = this->find_fighter_in_same_scene(pet_base_detail_.master_role_id);
//		if (! fighter) {
//			pet_recycle(CALL_BACK);
//		} else {
//			//主人存在，宠物阵营为0时候，将宠物阵营设为人的阵营
//			if (fighter_detail_.camps == 0) {
//				fighter_detail_.camps = fighter->fighter_camps();
//			}
//		}
//	}
	return 0;
}

int Pet::pet_pull_back(void) {
//	if ((! move_scene()) || this->dead_status()) {
//		return -1;
//	}
//
//	/**
//	 * 此处不用MAP_MANAGER_INSTANCE->find_role_id_player是因为:
//	 * 有可能此时主人玩家还未发 场景加载完成 消息, 此处会调用pet的appear(),
//	 * 加载完成 消息哪里也会调用pet的appear, 会两次进入地图.
//	 */
//
//	Fighter *fighter = this->find_fighter_in_same_scene(pet_base_detail_.master_role_id);
//	if (! fighter)
//		return -1;
//
//	Mover_Detail &detail = fighter->mover_detail();
//
//	move_disappear();
//	Grid_Coord player_coord;
//	player_coord.x = detail.grid_coord.x;
//	player_coord.y = detail.grid_coord.y;
//	move_scene()->collect_available_coord(mover_detail().layer_id, player_coord, pet_base_detail_.coord, 1, 2);
//	reset_coord(pet_base_detail_.coord);
//	appear();

	return 0;
}
//
//int Pet::refresh_pet_base_detail(Pet_Base_Detail &detail) {
//	role_id_t pet_role_id = pet_base_detail_.pet_role_id;
//	pet_base_detail_ = detail;
//	//宠物属性改变时候，同步到地图服，更新宠物基本信息，此时Pet_Base_Detail中宠物role_id为0，不能直接覆盖
//	if (pet_base_detail_.pet_role_id <= 0) {
//		pet_base_detail_.pet_role_id = pet_role_id;
//	}
//
//	return 0;
//}

int Pet::refresh_pet_fighter_detail(Fighter_Detail &detail) {
//	fighter_detail_ = detail;
//	fighter_detail_.role_id = pet_base_detail_.pet_role_id;
//	//宠物属性变化同步宠物信息到地图服时候，地图服宠物要重新设置阵营战斗模式
//	Map_Player *player = this->master();
//	if (player) {
//		set_pet_camps(player->fighter_camps());
//		set_pet_country(player->country());
//		set_pet_team_id(player->team_id());
//		set_pet_faction_id(player->fighter_detail().faction_id);
//		set_pet_fight_mode(player->fighter_mode());
//	}
//
//	//添加宠物技能
//	add_pet_skill();
//
//	//宠物死亡后满血复活
//	if (fighter_detail_.fetch_property(Fighter_Detail::HP) < 1) {
//		double hp = max_hp();
//		fighter_detail_.modify_property(Fighter_Detail::HP, Fighter_Detail::BASIC,
//						Fighter_Detail::SET, hp);
//	}
//
//	if (move_scene()) {
//		pet_notify_fight_info();
//	}

	return 0;
}

int Pet::sync_pet_fighter_detail_to_logic(void) {
//	Block_Buffer sync_buff;
//	sync_buff.make_message(INNER_SYNC_PET_FIGHTER_DETAIL);
//	fighter_detail_.serialize(sync_buff);
//	sync_buff.write_int64(pet_base_detail_.master_role_id);
//	sync_buff.write_int64(pet_base_detail_.pet_id);
//	sync_buff.finish_message();
//	return send_to_logic(sync_buff);
	return 0;
}

int Pet::sync_pet_die_to_logic(void) {
//	Block_Buffer sync_buff;
//	sync_buff.make_message(INNER_PET_DIE_TO_LOGIC);
//	sync_buff.write_int64(pet_base_detail_.master_role_id);
//	sync_buff.finish_message();
//	return send_to_logic(sync_buff);
	return 0;
}

int Pet::sync_pet_call_back_to_logic() {
//	Block_Buffer sync_buff;
//	sync_buff.make_message(INNER_SYNC_PET_FIGHT);
//	sync_buff.write_int64(pet_base_detail_.master_role_id);
//	sync_buff.finish_message();
//	return send_to_logic(sync_buff);
	return 0;
}

int Pet::sync_pet_info_to_logic(void) {
//	Block_Buffer sync_buff;
//	sync_buff.make_message(INNER_SYNC_PET_INFO);
//	MSG_700205 sync_msg;
//	sync_msg.master_role_id = pet_base_detail_.master_role_id;
//	sync_msg.pet_id = pet_base_detail_.pet_id;
//	sync_msg.experience = fighter_detail_.experience;
//	sync_msg.hp = fighter_detail_.fetch_property(Fighter_Detail::HP);
//	sync_msg.serialize(sync_buff);
//	sync_buff.finish_message();
//	return send_to_logic(sync_buff);
	return 0;
}

int Pet::send_to_logic(Block_Buffer &buf) {
//	Map_Player *player = MAP_MANAGER_INSTANCE->find_role_id_player(pet_base_detail_.master_role_id);
//	if (player) {
//		return player->send_to_logic(buf);
//	}
//	return MAP_MANAGER_INSTANCE->send_to_logic_server(buf);
	return 0;
}

int Pet::skill_insert_status(void) {
//	for (uint i = 0; i < pet_base_detail_.skill_vector.size(); ++i) {
//		if (pet_base_detail_.skill_vector[i] > 0) {
//			this->pet_skill_status_insert(pet_base_detail_.skill_vector[i]);
//		}
//	}
	return 0;
}

int Pet::skill_erase_status(void) {
//	for (uint i = 0; i < pet_base_detail_.skill_vector.size(); ++i) {
//		if (pet_base_detail_.skill_vector[i] > 0) {
//			this->pet_skill_status_erase(pet_base_detail_.skill_vector[i]);
//		}
//	}
	return 0;
}

int Pet::add_pet_skill(void) {
//	for (uint i = 0; i < pet_base_detail_.skill_vector.size(); ++i) {
//		if (pet_base_detail_.skill_vector[i] > 0) {
//			this->add_new_skill(pet_base_detail_.skill_vector[i]);
//		}
//	}
	return 0;
}

int Pet::modify_pet_experience(double value, int master_level) {
//	if (value <= 0)
//		return -1;
//
//	if (this->fighter_detail_.experience + value >= LONG_MAX) {
//		this->fighter_detail_.experience = LONG_MAX;
//	} else {
//		this->fighter_detail_.experience += value;
//	}
//
//	double exp_up = CONFIG_CACHE_INSTANCE->pet_level_up_exp(fighter_detail_.level);
//	if (exp_up < 1.0)
//		return -1;
//
//	int level_up = 0;
//	while (this->fighter_detail_.experience >= exp_up) {
//		this->fighter_detail_.experience -= exp_up;
//		this->fighter_detail_.level++;
//		level_up++;
//
//		exp_up = CONFIG_CACHE_INSTANCE->pet_level_up_exp(fighter_detail_.level);
//	}
//
//	//宠物等级大于等于人物等级时，宠物不获得经验
//	if (fighter_detail_.level >= master_level) {
//		fighter_detail_.level = master_level;
//		fighter_detail_.experience = 0;
//	}
//
//	if (level_up > 0) {		//宠物升级马上将宠物信息同步回逻辑服
//		sync_pet_fighter_detail_to_logic();
//	} else {
//		is_sync_ = true;
//	}
//
//	return this->notify_fight_info(UPDATE_PRO_PET_EXP, this->fighter_detail_.experience);
	return 0;
}

int Pet::pet_notify_fight_info(void) {
//	MSG_800501 res_msg;
//	res_msg.role_id = fighter_detail_.role_id;
//	res_msg.flag = 0;
//
//	Property_Info info;
//	info.reset();
//	info.cur_value = fighter_detail_.level;
//	info.type = UPDATE_PRO_PET_LEVEL;
//	res_msg.info.push_back(info);
//
//	info.reset();
//	info.cur_value = fighter_detail_.fetch_property(Fighter_Detail::HP);
//	info.type = Fighter_Detail::HP;
//	res_msg.info.push_back(info);
//
//	info.reset();
//	info.cur_value = fighter_detail_.fetch_property(Fighter_Detail::HP_MAX);
//	info.type = Fighter_Detail::HP_MAX;
//	res_msg.info.push_back(info);
//
//	return this->notify_fight_info(res_msg, true);
	return 0;
}

role_id_t Pet::master_id(void) {
//	return pet_base_detail_.master_role_id;
	return 0;
}

void Pet::set_pet_camps(int camps) {
//	fighter_detail_.camps = camps;
}

void Pet::set_pet_country(int country) {
//	fighter_detail_.country = country;
}

void Pet::set_pet_team_id(int team_id) {
//	fighter_detail_.team_id = team_id;
}

void Pet::set_pet_faction_id(faction_id_t faction_id) {
//	fighter_detail_.faction_id = faction_id;
}

//void Pet::set_pet_fight_mode(Fighter_Detail::Fight_Mode fight_mode) {
////	fighter_detail_.fight_mode = fight_mode;
//}

void Pet::set_pet_name(std::string name) {
//	pet_base_detail_.name = name;
}

void Pet::set_pet_cid(int pet_cid) {
//	pet_base_detail_.pet_cid = pet_cid;
}

Pet* Pet::pet_self(void) {
	return this;
}

void Pet::displacement_only_vision(const Coord &coord_from, Coord &coord_to, int skill_id, int step, int action) {
//	if (coord_from == coord_to) {
//		return;
//	}
//
//	if (pet_base_detail_.pet_role_id <= 0 || pet_base_detail_.master_role_id <= 0) {
//		return ;
//	}
//
//	/// construct move msg
//	MSG_800502 move_msg;
//	move_msg.reset();
//	move_msg.attack_id = fighter_detail_.role_id;
//	move_msg.target_id.push_back(fighter_detail_.role_id);
//	move_msg.skill_id = skill_id;
//	move_msg.action = action;
//	move_msg.step = step;
//	move_msg.px = coord_to.x;
//	move_msg.py = coord_to.y;
//
//	Block_Buffer move_buf;
//	move_buf.make_message(ACTIVE_FIGHT_ACTION_BROADCAST);
//	move_msg.serialize(move_buf);
//	move_buf.finish_message();
//
//	/// construct disappear msg
//	MSG_800001 disappear_msg;
//	disappear_msg.role_id = mover_detail_.mover_id;
//
//	disappear_msg.info.push_back(1); /// [1] type = 1
//	disappear_msg.info.push_back(skill_id); /// [2] skill_id
//	disappear_msg.info.push_back(gird_coord().x); /// [3] start px
//	disappear_msg.info.push_back(gird_coord().y); /// [4] start py
//	disappear_msg.info.push_back(coord_to.x); /// [5] end px
//	disappear_msg.info.push_back(coord_to.y); /// [6] end py
//
//	Block_Buffer disappear_buf;
//	disappear_buf.make_message(ACTIVE_MOVER_DISAPPEAR);
//	disappear_msg.serialize(disappear_buf);
//	disappear_buf.finish_message();
//
//	/// construct appear msg
//	MSG_800006 pet_appear_msg;
//	this->make_pet_appear_info(pet_appear_msg);
//
//	pet_appear_msg.pet_info.additional_info.push_back(1);
//	pet_appear_msg.pet_info.additional_info.push_back(skill_id);
//	pet_appear_msg.pet_info.additional_info.push_back(coord_from.x);
//	pet_appear_msg.pet_info.additional_info.push_back(coord_from.y);
//	pet_appear_msg.pet_info.additional_info.push_back(coord_to.x);
//	pet_appear_msg.pet_info.additional_info.push_back(coord_to.y);
//	pet_appear_msg.pet_info.px = coord_to.x;
//	pet_appear_msg.pet_info.py = coord_to.y;
//
//	Block_Buffer appear_buf;
//	appear_buf.make_message(ACTIVE_PET_APPEAR);
//	pet_appear_msg.serialize(appear_buf);
//	appear_buf.finish_message();
//
//	if (move_scene())
//		move_scene()->broadcast_mover_change_coord(this, coord_to, &disappear_buf, &move_buf, &appear_buf);
}
