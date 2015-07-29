/*
 * Scene_Player.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#include "Scene_Player.h"
#include "Scene_Module_Head.h"
#include "Move_Scene.h"
#include "Battle_Scene.h"
#include "Scene_Player_Detail.h"
#include "Global_Scene_Manager.h"
#include "NPC/NPC_Manager.h"
#include "NPC/NPC_Struct.h"
#include "world_boss/Gang_Boss_Manager.h"
#include "world_boss/World_Boss_Manager.h"
#include "Err_Code.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Inner_Struct.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Msg_Fight_Struct.h"
#include "Msg_Role_Scene_Struct.h"
#include "fighter/Fighter_Struct.h"
#include "Config_Cache.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Public.h"
#include "Scene_Public.h"
#include "team/Scene_Team_Manager.h"
#include "NPC/monster/Monster_Def.h"
#include "Config_Cache_Team.h"
#include "team/Team_Func.h"
#include "announce/Scene_Announce_Manager.h"
#include "dragon/Scene_Dragon_Vale_Manager.h"
#include "gang/Scene_Gang_Global_Manager.h"
#include "expedition/Scene_Expeditioner.h"

Scene_Player::Scene_Player() {
	// TODO Auto-generated constructor stub
	monitor_ = SCENE_MONITOR;
	detail_ptr_ = 0;
	base_detail_ = 0;
	reset();
}

void Scene_Player::reset(void) {
	Scene_Common::reset();
	Fighter::reset();
	Scene_Campfire::reset();
	Scene_Warer::reset();
	Scene_Tasker::reset();
	Scene_Teamer::reset();
	Scene_Ganger::reset();
	Scene_Expeditioner::reset();

	if (detail_ptr_) {
		POOL_MANAGER->push_scene_player_data_pool(detail_ptr_);
	}
	detail_ptr_ = 0;

	base_detail_ = 0;
	monitor_link_.reset();

	move_scene_completed_ = false;
	battle_scene_completed_ = false;
	player_has_init_ = false;
	is_no_profit_ = false;
	is_hosting_ = false;
	zone_.clear();
}

Scene_Player::~Scene_Player() {
	// TODO Auto-generated destructor stub
}

int Scene_Player::send_to_gate(Block_Buffer &buf, bool use_swap) {
	if (!player_has_init_)
		return 0;

	return monitor_->send_to_monitor(monitor_link_.gate_muid, buf, use_swap);
}

int Scene_Player::send_to_logic(Block_Buffer &buf, bool use_swap) {
	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.logic_muid, buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.logic_muid, tmp_buf, true);
	}

	return ret;
}

int Scene_Player::send_to_local_chat(Block_Buffer &buf, bool use_swap) {
	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.local_chat_muid, buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(buf.get_msg_id(), role_id());
		ret = monitor_->send_to_monitor(monitor_link_.local_chat_muid, tmp_buf, true);
	}

	return ret;
}

int Scene_Player::send_to_client(Block_Buffer &buf, bool use_swap) {
	if (!player_has_init_)
		return 0;

	CHECK_BUFFER_LEN(buf);

	int ret = 0;
	if (buf.is_read_begin()) {
		buf.make_head(0, role_id());
		ret = send_to_gate(buf, use_swap);
		buf.set_read_begin();
	} else {
		Block_Buffer tmp_buf;
		tmp_buf.copy(&buf);
		tmp_buf.make_head(0, role_id());
		ret = send_to_gate(tmp_buf, true);
	}

	return ret;
}

int Scene_Player::send_err_to_client(int error_id, int source) {
	Block_Buffer buf;
	buf.make_message(80000001);
	buf.write_int32(error_id);
	buf.write_int32(source);
	buf.write_string("");
	buf.finish_message();
	return this->send_to_client(buf);
}

int Scene_Player::time_up(const Time_Value &now) {
	// 有移动场景才tick
	if (!validate_mover_scene())
		return 0;

	Fighter::time_up(now);
	Scene_Common::time_up(now);
	Scene_Warer::time_up(now);
	Scene_Ganger::time_up(now);
	Scene_Expeditioner::time_up(now);
	return 0;
}

int Scene_Player::sign_in(void) {
	move_appear();

	// 通知逻辑服场景加载完成
	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_NOTICE_SIGN_SCENE);
	inner_buf.finish_message();
	send_to_logic(inner_buf);

	is_hosting_ = false;

	return 0;
}

int Scene_Player::get_role_info(void) {
	MSG_81000100 msg;
	msg.scene_order_now = move_scene_order();
	msg.scene_order_total = move_scene()->scene_config()->order_total;

	msg.role_name = role_name();
	msg.gender = base_detail().gender;
	msg.gang_id = base_detail().gang_id;
	msg.gang_name = base_detail().gang_name;
	msg.property.push_back(Property(PT_LEVEL, level()));
	msg.property.push_back(Property(PT_CAREER, career()));
	msg.property.push_back(Property(PT_FORCE, fighter_detail().force));

	msg.property.push_back(Property(PT_BLOOD_MAX, blood_max()));
	msg.property.push_back(Property(PT_BLOOD_CURRENT, blood_current()));
	msg.property.push_back(Property(PT_EXP_CURRENT, fighter_detail().experience));
	msg.property.push_back(Property(PT_EXP_LEVEL_UP, CONFIG_CACHE_ROLE->level_up_need_exp_map(level())));
	msg.property.push_back(Property(PT_STRENGTH_MAX, fighter_detail().strength_max.total()));
	msg.property.push_back(Property(PT_STRENGTH_CURRENT, fighter_detail().strength_current.total()));
	msg.property.push_back(Property(PT_BLOOD_POOL_MAX, blood_pool_max()));
	msg.property.push_back(Property(PT_BLOOD_POOL_CURRENT, blood_pool_current()));
	msg.property.push_back(Property(PT_SKILL_POINT, fighter_detail().ttl_skill_points_));

	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Scene_Player::sign_out(bool quiet, bool offline_exit) {
	//todo 各模块退出处理(退出场景等)
	Scene_Ganger::sign_out();
	Scene_Common::sign_out();

	// 同步数据到逻辑
	if (! quiet) {
		sync_mover_detail_to_logic();
	}

	// 没有发场景加载完成就退出不需要exit_battle_scene
	if (is_in_battle()){
		exit_battle_scene(13);
		notify_gate_exit_battle(13);
	}

	// 退出移动场景
	if (this->validate_mover_scene()) {
		exit_move_scene(offline_exit);
	} else {
		MSG_DEBUG("not in move scene , role_id = %ld layer_id:%d", base_detail_->role_id, mover_detail().layer_id);
	}

	// 同步数据到逻辑
	if (! quiet) {
		sync_fighter_detail_to_logic();
	}

	move_scene_completed_ = false;

	return 0;
}

int Scene_Player::load_detail(Block_Buffer &buf) {
	Scene_Player_Detail *detail = POOL_MANAGER->pop_scene_player_data_pool();
	if (! detail) {
		MSG_TRACE_ABORT("");
		return -1;
	}

	detail_ptr_ = detail;
	base_detail_ = &detail->base_detail;

	Fighter::set_detail_prt(detail->fighter_detail);
	Mover::set_detail_prt(detail->mover_detail);

	monitor_link_.deserialize(buf);
	base_detail_->deserialize(buf);
	Fighter::load_detail(buf);

	Status::load_status_info(buf);
	Scene_Ganger::gang_load_detail(buf);

	Scene_Teamer::syn_teamer_info(buf);
	buf.read_string(zone_);
	Scene_Expeditioner::load_detail(buf);
	uint16_t tmp_energy = 0;
	buf.read_uint16(tmp_energy);
	set_energy(tmp_energy);
	int life_skill_id = 0;
	buf.read_int32(life_skill_id);
	set_life_skill(life_skill_id);

	set_toward(base_detail_->location.toward);
	set_layer_id(base_detail_->location.scene_layer);

	Coord grid_coord = base_detail_->location.coord.grid_coord();
	set_grid_coord(grid_coord.x, grid_coord.y, true);

	Coord pixel_coord = base_detail_->location.coord.pixel_coord();
	set_pixel_coord(pixel_coord.x, pixel_coord.y);

	return 0;
}

int Scene_Player::module_init(void) {
	monitor()->server_muid_build(monitor_link_.gate_muid);
	player_has_init_ = true;

	scene_init();

	//加载篝火信息
	req_campfire_player_info();

	return 0;
}

int Scene_Player::scene_init(void) {
	Monitor_Unique_ID scene_muid = monitor_link_.scene_muid;
	scene_muid.uuid = CONFIG_CACHE->server_flag();

	Scene_Manager *scene_manager = GLOBAL_SCENE_MANAGER->get_scene_manager(scene_muid);
	if (! scene_manager) {
		MSG_TRACE_ABORT("scene_manager null, role:%ld, uuid:%ld, type:%d id:%d, order:%d", role_id(), monitor_link_.scene_muid.uuid,
				monitor_link_.scene_muid.type, monitor_link_.scene_muid.id, monitor_link_.scene_muid.order);
		return -1;
	}

	Move_Scene *scene = scene_manager->find_scene(scene_muid, role_id(), base_detail_->create_scene_args_);

	if (! scene) {
		MSG_TRACE_ABORT("scene_manager null, role:%ld, uuid:%ld, type:%d id:%d, order:%d", role_id(), monitor_link_.scene_muid.uuid,
				monitor_link_.scene_muid.type, monitor_link_.scene_muid.id, monitor_link_.scene_muid.order);
		return -1;
	}

	enter_move_scene(scene);

	return 0;
}

/*
 * des_scene
 * 用于组队副本刷新：
 * 条件: 1.在组队副本
 *      2.组队副本已经完成
 * 动作: 返回组队房间*/
int Scene_Player::login_scene_exist(int &des_scene) {
	des_scene = 0;
	Monitor_Unique_ID scene_muid = monitor_link_.scene_muid;
	scene_muid.uuid = CONFIG_CACHE->server_flag();
	Scene_Manager *scene_manager = GLOBAL_SCENE_MANAGER->get_scene_manager(scene_muid);
	if (! scene_manager) {
		MSG_TRACE_ABORT("scene_manager null, role:%ld, uuid:%ld, type:%d id:%d, order:%d", role_id(), monitor_link_.scene_muid.uuid,
				monitor_link_.scene_muid.type, monitor_link_.scene_muid.id, monitor_link_.scene_muid.order);
		return -1;
	}

	Move_Scene *scene = scene_manager->find_scene_simple(scene_muid, role_id(), base_detail_->create_scene_args_);

	// 登录不存在要创建的场景处理
	if (!scene) {
		const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_muid.id);
		if (scene_config) {
			if (scene_config->login_create) {
				scene = scene_manager->find_scene(scene_muid, role_id(), base_detail_->create_scene_args_);
			}
		}
	}

	if (! scene) {
		return 0;
	} else {
		/*
		 * 用于组队副本刷新：
		 * 条件: 1.在组队副本
		 *      2.组队副本已经完成
		 * 动作: 返回组队房间*/
		const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_muid.id);
		if (scene_config) {
			if ((Scene_Team == scene_config->manager
					|| Scene_Team_Defend == scene_config->manager)
					&& scene->is_scene_pass()) {
				int8_t type = 0;
				const int* scene_id_ptr = CONFIG_CACHE_TEAM->find_scene(type);
				if (scene_id_ptr) {
					des_scene = *scene_id_ptr;
					return 0;
				}
			} else if (Scene_Gang_War == scene_config->manager) {
				const Gang_War_Matched_Gang_Info *info = SCENE_GANG_GLOBAL_MANAGER->find_matched_gang_info(this->ganger_detail().gang_id);
				if (info) {
					this->set_create_scene_args(0, Time_Value::gettimeofday(), info->scene_index, 0, 0, 0, true);
				} else {
					return 0;
				}
			}
		}
	}

	return 1;
}

bool Scene_Player::is_in_mover_scene(void){
	return this->move_scene() && move_scene_completed_;
}

bool Scene_Player::can_fight(void){
	if(is_in_mover_scene() && (plot_status()== false) && (is_in_battle() == false)){
		return true;
	}
	return false;
}

int Scene_Player::move_able(void){
	int ret = Fighter::move_able();
	if (0 == ret) {
		if(this->is_take_lift_){
			return -1;
		}
	}
	return ret;
}

int Scene_Player::move_appear_info(Block_Buffer &buf) {
	// size_t rd_idx = buf.get_read_idx();
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	Block_Buffer &appear_buf = appear_buff();
	if (appear_buf.readable_bytes()) {
		appear_buf.set_write_idx(16 + 8 + 8);
		mover_detail().pixel_coord.serialize(appear_buf);
	}

	if (!appear_buf.readable_bytes() || appear_buf.get_ext_flag()) {
		appear_buf.reset();
		appear_buf.set_ext_flag(true);

		appear_buf.make_message(ACTIVE_PLAYER_APPEAR);

		MSG_81000001 msg;
		msg.role_info.role_id = base_detail_->role_id;
		msg.role_info.head_id = base_detail_->head_id;
		msg.role_info.role_name = role_name();
		msg.role_info.coord = mover_detail().pixel_coord;
		msg.role_info.career = career();
		msg.role_info.gender = base_detail().gender;
		msg.role_info.level = level();
		msg.role_info.addition_prop = addition_prop();
		msg.role_info.title_id = base_detail().title_id;
		msg.role_info.vip = (uint8_t)vip();
		msg.role_info.gang_name = base_detail().gang_name;
		msg.role_info.server_name = zone_;
		msg.role_info.speed = fighter_detail().speed.total();
		if (0 != this->battle_scene()) {
			msg.role_info.state = 1;
		}else{
			if(war_info()->revive == 1
					|| gang_war_info().revive == 1
					|| is_revive()
					|| expedition_scene_info().revive == 1 ){//战场复活中
				msg.role_info.state = 2;
			}
		}
		const Scene_Config *cfg = CONFIG_CACHE_SCENE->scene_config_cache(move_scene_id());
		if (cfg) {
			switch(cfg->manager) {
			case Scene_War: {
				msg.role_info.tmp_camp = war_info()->league;
				break;
			}
			case Scene_Gang_War: {
				msg.role_info.tmp_camp = gang_war_info().league;
				break;
			}
			default:
				break;
			}
		}
		msg.role_info.avatar.assign(base_detail().avatar_vec.begin(), base_detail().avatar_vec.end());
		// avatar
		if(base_detail().ride != 0){
			int int_value;
			int_value = base_detail().mount_model_id;
			msg.role_info.avatar.push_back(int_value);
		}

		msg.serialize(appear_buf);
	}

	buf.copy(&appear_buf);

	// refill len in head
	wr_end_idx = buf.get_write_idx();
	len = wr_end_idx - wr_begin_idx - sizeof(uint32_t);
	buf.set_write_idx(wr_begin_idx);
	buf.write_uint32(len);
	buf.set_write_idx(wr_end_idx);

	//
	return 0;
}

int Scene_Player::make_move_info(Block_Buffer &buf, uint8_t toward, uint16_t x, uint16_t y) {
	MSG_81000020 msg;
	msg.role_id = role_id();

	msg.coord.push_back(Coord(x, y));
	buf.make_message(ACTIVE_MOVER_MOVE);
	msg.serialize(buf);
	buf.finish_message();

	return 0;
}

int Scene_Player::move_appear(void) {
	Mover::move_appear();

	Hero *hero = SCENE_PUBLIC->find_mover_hero(role_id());
	if (hero && hero->move_scene()) {
		hero->move_appear();
	}

	return 0;
}

int Scene_Player::move_disappear(void) {
	Hero *hero = SCENE_PUBLIC->find_mover_hero(role_id());
	if (hero && hero->move_scene() && hero->move_scene()->find_mover_with_layer(hero->mover_detail().layer_id, hero->role_id())) {
		hero->move_disappear();
	}

	// 剩余广播
	if (validate_mover_scene()) {
		this->move_scene()->force_flush_tick(mover_detail().layer_id, mover_detail().block_index);
	}

	Mover::move_disappear();

	return 0;
}

int Scene_Player::enter_move_scene(Move_Scene *scene) {
	Mover::enter_move_scene(scene);
	return 0;
}

int Scene_Player::exit_move_scene(bool offline_exit) {
	// 英雄先退
	Hero *hero = SCENE_PUBLIC->find_mover_hero(role_id());
	if (hero) {
		hero->exit_move_scene(offline_exit);
	}

	// 剩余广播
	if (validate_mover_scene()) {
		this->move_scene()->force_flush_tick(mover_detail().layer_id, mover_detail().block_index);
	}

	Mover::exit_move_scene(offline_exit);

	this->is_take_lift_ = false;
	notify_end_plot();
	//
	SCENE_PUBLIC->recycle_all_hero(role_id());
	return 0;
}

int Scene_Player::battle_appear_info(Block_Buffer &buf) {
	// size_t rd_idx = buf.get_read_idx();
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	// fill head
	buf.make_message(ACTIVE_FIGHTER_APPEAR);

	int state = 0;
	if (0 != fighter() && Battle_Status::DIE_OVER == fighter()->get_battle_status()) {
		state = 1;
	}

	MSG_81000008 msg;
	msg.fighter_info.role_id = role_id();
	msg.fighter_info.role_name = role_name();
	msg.fighter_info.career = career();
	msg.fighter_info.gender = base_detail().gender;
	msg.fighter_info.position.val_1 = mover_detail().battle_position.pos;
	msg.fighter_info.position.val_2 = mover_detail().battle_position.idx;
	msg.fighter_info.hp_curr = blood_current();
	msg.fighter_info.hp_max = blood_max();
	msg.fighter_info.fight_state = state;
	msg.fighter_info.avatar = base_detail().avatar_vec;

	boost::unordered_set<Property_Type> property_set;
	property_set.insert(PT_MORALE_CAPS);
	property_set.insert(PT_MORALE_CURRENT);

	for (boost::unordered_set<Property_Type>::iterator it = property_set.begin(); it != property_set.end(); ++it) {
		msg.fighter_info.init_props.push_back(Prop_Change(*it));
	}

	for (std::vector<Prop_Change>::iterator pc_it = msg.fighter_info.init_props.begin();
			pc_it != msg.fighter_info.init_props.end(); ++pc_it) {
		(*pc_it).current_val = fighter_detail().fetch_fight_property(Property_Type((*pc_it).prop_type));
	}

	msg.serialize(buf);

	// refill len in head
	wr_end_idx = buf.get_write_idx();
	len = wr_end_idx - wr_begin_idx - sizeof(uint32_t);
	buf.set_write_idx(wr_begin_idx);
	buf.write_uint32(len);
	buf.set_write_idx(wr_end_idx);

	return 0;
}

int Scene_Player::battle_appear(void) {
	Mover::battle_appear();

	return 0;
}

int Scene_Player::battle_disappear(void) {
	Mover::battle_disappear();

	return 0;
}

int Scene_Player::enter_battle_scene(Battle_Scene *scene) {
	if(scene == NULL){
		return -1;
	}

	if (this->battle_scene()) {
		MSG_TRACE_ABORT("has battle scene, role:%ld battle_id:%d, b_id:%d", role_id(), battle_scene_id(), scene->battle_id());
		return -1;
	}

	Fighter::enter_battle_scene(scene);

	MSG_DEBUG("enter_battle_scene, battle_id: %ld, role: %ld, pos:%d  %d, talent:%d", this->battle_scene_id(), role_id(), this->battle_position().pos, this->battle_position().idx, fighter_detail().cur_used_talent_);

	hook_player_enter_battle();

	if (this->validate_battle_scene()) {
		// 通知网关进入战斗
		Block_Buffer inner_buf;
		inner_buf.make_message(SCENE_GATE_BATTLE_STATUS);
		inner_buf.write_int64(role_id());
		inner_buf.write_int32(1);
		inner_buf.finish_message();
		this->send_to_gate(inner_buf);

		// 通知玩家进入战斗
		MSG_81000050 msg;
		msg.map_id = (int32_t)scene->get_map_id();
		msg.position = mover_detail().battle_position.pos;
		msg.wave_cur = battle_scene()->get_cur_monster_wave();
		msg.wave_max = battle_scene()->get_max_monster_wave();
		msg.battle_type = battle_scene()->get_battle_type();
		THIS_SEND_TO_CLIENT(msg);
		battle_scene_completed_ = false;


		scene->add_player_on_battle(role_id());
	}

	move_scene_completed_ = false;

	return 0;
}

void Scene_Player::player_pos_key_fit_hero(Position_Key &player_pos_key, Position_Key &hero_pos_key) {
	// 英雄前排 hero_formation == 5
	if (hero_pos_key.idx == 5) {
		switch (player_pos_key.idx) {
		case 4:
			player_pos_key.idx = 7;
			hero_pos_key.idx = 4;
			break;
		case 5:
			player_pos_key.idx = 8;
			break;
		case 6:
			player_pos_key.idx = 9;
			hero_pos_key.idx = 6;
			break;
		case 7:
			hero_pos_key.idx = 4;
			break;
		//case 8:
		case 9:
			hero_pos_key.idx = 6;
			break;
		default:
			break;
		}

	// 英雄后排 hero_formation == 8
	} else {
		switch (player_pos_key.idx) {
		case 4:
			hero_pos_key.idx = 7;
			break;
		//case 5:
		case 6:
			hero_pos_key.idx = 9;
			break;
		case 7:
			player_pos_key.idx = 4;
			hero_pos_key.idx = 7;
			break;
		case 8:
			player_pos_key.idx = 5;
			hero_pos_key.idx = 8;
			break;
		case 9:
			player_pos_key.idx = 6;
			hero_pos_key.idx = 9;
			break;
		default:
			break;
		}
	}
}

int Scene_Player::hero_battle_birth(void) {
	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id());
	if (hero) {
		const Coord &player_coord = mover_detail().grid_coord;
		if (0 != hero->battle_birth(hero->hero_const_base_detail().hero_role_id, battle_scene(), player_coord)) {
			hero->fight_hero_recycle();
			return -1;
		}
	}
	return 0;
}

int Scene_Player::enter_battle_scene_team_member(Battle_Scene *scene) {
	// 组队战斗(目前用于组队场景)
	if (this->mover_detail().is_player()) {
		const Scene_Config * scene_config = CONFIG_CACHE_SCENE->scene_config_cache(this->move_scene_id());
		if (scene_config && Scene_Config::Dungeon_Config_Detail::CAN_TEAM == scene_config->dungeon.can_team) {
			Map_Team* team = SCENE_TEAM_MANAGER->get_team(this->player_self()->get_team_id());
			if (NULL != team) {
				int pos = -1;
				for (Battle_Scene::Debug_Mover_Map::const_iterator it = player_self()->battle_scene()->pos_debug_map().begin();
						it != player_self()->battle_scene()->pos_debug_map().end(); ++it) {
					if (it->second != role_id()) continue;
					pos = it->first.pos;
				}
				if (pos == -1) {
					return -1;
				}
				Int_Hash_Set idx_set;
				for (Battle_Scene::Debug_Mover_Map::const_iterator it = player_self()->battle_scene()->pos_debug_map().begin();
						it != player_self()->battle_scene()->pos_debug_map().end(); ++it) {
					if (it->first.pos != pos) continue;
					idx_set.insert(it->first.idx);
				}
				int ret = ensure_battle_pos_unique(idx_set);
				if (0 != ret) {
					return ret;
				}
				Role_Hash_Set members;
				team->team_all_member_set(members);
				for(Role_Hash_Set::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
					role_id_t id = *iter;
					if (id != role_id()) {
						Scene_Player *player = SCENE_MONITOR->find_scene_player_by_role_id(id);
						if (player && !player->is_in_battle()
								&& (is_in_range(player->mover_detail().grid_coord, this->mover_detail().grid_coord, FIGHT_TEAM_DISTANCE) || 1 == player->follow())) {

							int idx = player->team_position().idx;
							int y = get_other_idx(idx);
							if (0 == y) {
								idx = Position_Key::LINE_ONE_FRONT;
								y = Position_Key::LINE_ONE_BEHIND;
							}
							get_empty_pos_check_idx(idx_set, idx);
							if (idx == -1) {
								return ERROR_FIGHT_NOT_EMPTY;
							}
							if(player->set_battle_position(pos, idx, scene) == 0){
								player->enter_battle_scene(scene);
								idx_set.insert(idx);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

int Scene_Player::exit_battle_scene(int exit_from) {
	MSG_DEBUG("exit_battle_scene, role:%ld, battle id:%d, exit_from:%d", role_id(), battle_scene_id(), exit_from);

	// 以下发现情况：当玩家被exit_scene必须让英雄exit_battle_scene
	Hero *hero = SCENE_PUBLIC->find_fight_hero(role_id());
	if (hero && hero->battle_scene()) {
		MSG_USER("master id:%ld hero:%ld, b_id:%d", role_id(), hero->role_id(), hero->battle_scene_id());
		hero->exit_battle_scene();
	}

	Fighter::exit_battle_scene();

	// 通知玩家退出战斗
	MSG_81000055 msg;
	if (12 == exit_from) {
		msg.type = 1;
	}
	THIS_SEND_TO_CLIENT(msg);

	// 更新人物在场景的状态
	MSG_81000026 state_msg;
	state_msg.role_id = role_id();
	state_msg.state = 0;
	Block_Buffer buf;
	MAKE_MSG_BUF(state_msg, buf);
	scene_broad_around(buf);

	// 死亡复活
	die_revive();

	// 战场不用血池回血
	if(move_scene() &&
			(move_scene()->scene_type() != War_Scene_Type &&
					move_scene()->scene_type() != GANG_WAR_TYPE &&
					move_scene()->scene_type() != World_Boss_Scene_Type
					&& move_scene()->scene_type() != Expedition_Type
			)
		){
		this->blood_pool_add_blood();
	}

	battle_scene_completed_ = false;

	return 0;
}

int Scene_Player::set_scene_muid(Monitor_Unique_ID &muid) {
	hook_scene_change(monitor_link_.scene_muid.id, muid.id);

	monitor_link_.scene_muid = muid;
	return 0;
}

int Scene_Player::set_create_scene_args(int type, Time_Value time, role_id_t id, int v1, int v2, int v3, bool v4) {
	base_detail_->create_scene_args_.type = type;
	base_detail_->create_scene_args_.time = time;
	base_detail_->create_scene_args_.id = id;
	base_detail_->create_scene_args_.sort = v1;
	base_detail_->create_scene_args_.sub_sort = v2;
	base_detail_->create_scene_args_.int_value = v3;
	base_detail_->create_scene_args_.slave = v4;

	return 0;
}

role_id_t Scene_Player::role_id(void) {
	return base_detail_->role_id;
}

int Scene_Player::level(void) {
	return fighter_detail().level;
}

std::string Scene_Player::role_name(void) {
	return base_detail_->role_name;
}

int Scene_Player::career(void) {
	return base_detail_->career;
}

int Scene_Player::vip(void) {
	Time_Value now = Time_Value::gettimeofday();
	if(base_detail_->vip_over_time <= now)
		return 0;
	return base_detail_->vip;
}

int Scene_Player::move_scene_completed(void) {
	MSG_DEBUG("role:%ld, scene:%d, force:%d", role_id(), this->move_scene_id(), this->fighter_detail().force);

	// 可能发生顶号
	Hero *hero = SCENE_PUBLIC->find_mover_hero(role_id());
	if (hero && move_scene()->find_mover_with_layer(mover_detail().layer_id, hero->role_id())) {
		hero->move_disappear();
	}
	if (move_scene()->find_mover_with_layer(mover_detail().layer_id, role_id())) {
		MSG_DEBUG("repeat login, role_id = %ld layer_id:%d", base_detail_->role_id, mover_detail().layer_id);
		move_disappear();
	}

	sign_in();

	move_scene()->client_scene_complete(*this);

	Status::client_scene_complete();
	Scene_Warer::client_scene_complete();
	Scene_Ganger::client_scene_complete();

	// 战斗中通知进入战斗
	if (battle_scene()) {
		MSG_81000050 msg;
		msg.position = mover_detail().battle_position.pos;
		msg.wave_cur = battle_scene()->get_cur_monster_wave();
		msg.wave_max = battle_scene()->get_max_monster_wave();
		msg.battle_type = battle_scene()->get_battle_type();
		msg.map_id = (int32_t)battle_scene()->get_map_id();
		THIS_SEND_TO_CLIENT(msg);
		battle_scene_completed_ = false;
	}else{
		move_scene_completed_ = true;
	}

	return 0;
}

int Scene_Player::battle_scene_completed(bool completed) {
	// MSG_DEBUG("scene completed, role:%ld, b_id:%d", role_id(), battle_scene_id());
	battle_scene_completed_ = completed;

	if (! this->validate_battle_scene()) {
		MSG_USER("scene null, role_id = %ld layer_id:%d", base_detail_->role_id, mover_detail().layer_id);
		return ERROR_INNER_SERVER;
	}

	// 可能发生顶号
	if (battle_scene()->find_mover_with_position(mover_detail().battle_position)) {
		MSG_DEBUG("repeat login, role_id = %ld layer_id:%d", base_detail_->role_id, mover_detail().layer_id);
		Mover::battle_disappear();
		continue_battle_after_refresh();

		battle_scene()->sync_give_up_data_when_client_scene_completed(*this);
	} else {
		if (battle_scene()->check_battle_over_before_appear()) {
			MSG_USER("battle over, role_id=%ld, battle_type=%d, battle_state=%d",
					role_id(), battle_scene()->get_battle_type(), battle_scene()->battle_status());
			exit_battle_scene(12);
			notify_gate_exit_battle(6);

			return ERROR_BATTLE_WAS_OVER;
		}

		// 英雄出战
		hero_battle_birth();
		Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
		if (0 != hero) {
			if (0 == hero->use_skill()) {
				hero->set_battle_status(BATTLING);
			}
		}
	}

	MSG_50200105 msg;
	msg.count = static_cast<int>(battle_scene()->mover_map().size()+1);
	THIS_SEND_TO_CLIENT(msg);

	battle_appear();
	battle_scene()->client_scene_complete(*this);
	battle_scene()->del_player_on_battle(role_id());

	send_init_data_to_client();
	Hero *hero = SCENE_PUBLIC->find_battle_hero(role_id());
	if (0 != hero) {
		hero->send_init_data_to_client();
	}

	return 0;
}

int Scene_Player::inner_transmit(int des_scene, int scene_order, int64_t arg3, std::string arg4, Coord coord) {
	MSG_DEBUG("inner transmit start, role:%ld. src scene:%d, des scene:%d %d", role_id(), this->move_scene_id(), des_scene, scene_order);

	MSG_10400005 msg;
	msg.scene_id = des_scene;
	msg.scene_order = scene_order;
	msg.role_id = arg3;
	Block_Buffer buf;
	make_message(buf, INNER_INNER_ASK_GATE_TRAN);
	msg.serialize(buf);
	coord.serialize(buf);
	buf.finish_message();
	return this->send_to_gate(buf);
}

void Scene_Player::sync_mover_detail_to_logic(void) {
	MSG_20200010 msg;
	msg.location.scene_id = move_scene_id();
	msg.location.coord = mover_detail().grid_coord;
	msg.location.toward = mover_detail().toward;
	msg.location.scene_order = move_scene_order();
	msg.location.scene_layer = mover_detail().layer_id;
	THIS_SEND_TO_LOGIC(msg);
}

void Scene_Player::sync_fighter_detail_to_logic(void) {
	MSG_20200013 msg;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	fighter_detail().serialize(buf);
	buf.finish_message();
	send_to_logic(buf, true);

	is_hosting_ = true;
}

int Scene_Player::notify_gate_exit_battle(int from){
	//
	{
		//通知逻辑服退出战斗
		MSG_20200202 msg;
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		this->send_to_logic(buf);
	}

	{
		// 通知网关退出战斗
		Block_Buffer inner_buf;
		inner_buf.make_message(SCENE_GATE_BATTLE_STATUS);
		inner_buf.write_int64(role_id());
		inner_buf.write_int32(0);
		inner_buf.finish_message();
		this->send_to_gate(inner_buf);
	}


	MSG_DEBUG("role:%ld from:%d", role_id(), from);
	return 0;
}

int Scene_Player::hook_player_enter_battle(void){
	if(move_scene()){
		move_scene()->hook_player_enter_battle(this);
	}
	return 0;
}

void Scene_Player::back_to_revive_point(void){
	// 如果死亡回到复活点
	if (dead_status() && move_scene()) {
		//单人副本复活处理，指定等级后弹复活框
		if(move_scene()->scene_type() == FuBen_Scene_Type){
			int revive_valid_lv = CONFIG_CACHE_PUBLIC->get_public_default_value(24);
			if(level() >= revive_valid_lv){
				int param1 = 0;
				int param2 = 0;
				int param3 = 0;
				move_scene()->get_scene_param(param1, param2, param3);
				param1 = 1;
				move_scene()->set_scene_param(param1, param2, param3, true);
				set_revive_info(true, Time_Value::max);
			}
		}
		Coord revive_point;
		move_scene()->get_nearest_revive_point(revive_point, grid_coord());
		if (validate_move_value(revive_point.x, revive_point.y)) {
			this->move_disappear();
			this->set_grid_coord(pixel_x_to_grid(revive_point.x), pixel_y_to_grid(revive_point.y), true);
			this->set_pixel_coord(revive_point.x, revive_point.y);
			//reset_coord();
		}
	}
}

bool Scene_Player::is_no_profit(void){
	return this->is_no_profit_;
}

void Scene_Player::set_no_profit(bool value){
	this->is_no_profit_ = value;
}

int Scene_Player::set_role_name(const std::string &new_name) {
	base_detail_->role_name = new_name;
	return 0;
}

int Scene_Player::modify_gender(const uint8_t gender) {
	base_detail_->gender = gender;
	return 0;
}

int Scene_Player::set_gang_id(const role_id_t gang_id) {
	base_detail_->gang_id = gang_id;
	return 0;
}

int Scene_Player::set_gang_name(const std::string &gang_name) {
	base_detail_->gang_name = gang_name;
	return 0;
}

void Scene_Player::set_mount_ride(const int ride){
	if(base_detail_){
		base_detail_->ride = ride;
	}
}
void Scene_Player::set_mount_horse_model_id(const int id){
	if(base_detail_){
		base_detail_->mount_model_id = id;
	}
}

void Scene_Player::set_rela_scene_id(const int id) {
	if (0 == this->base_detail_) return;
	this->base_detail_->rela_scene_id = id;
}

int Scene_Player::set_title(const int title_id) {
	base_detail_->title_id = title_id;

	MSG_81000902 res_msg;
	res_msg.role_id = base_detail_->role_id;
	res_msg.equip_title = title_id;

	Block_Buffer msg_buf;
	MAKE_MSG_BUF(res_msg, msg_buf);
	scene_broad_around(msg_buf);
	return 0;
}

int Scene_Player::set_vip_info_change(uint32_t vip_level, int64_t vip_over_time) {
	base_detail_->vip = vip_level;
	base_detail_->vip_over_time.sec(vip_over_time);

	MSG_81000060 res_msg;
	res_msg.role_id = base_detail_->role_id;
	Prop_Change p;
	p.reset();
	p.prop_type = PT_VIP_LEVEL;
	p.current_val = vip();
	res_msg.change_info.push_back(p);

	Block_Buffer msg_buf;
	MAKE_MSG_BUF(res_msg, msg_buf);

	scene_broad_around(msg_buf);
	WORLD_BOSS_MANAGER->listen_vip_info_change(this);
	GANG_BOSS_MANAGER->listen_vip_info_change(this);
	return 0;
}

role_id_t Scene_Player::gang_id(void) {
	return base_detail().gang_id;
}

std::string Scene_Player::zone(void) {
	return zone_;
}

void Scene_Player::add_charm(uint32_t num) {
	base_detail_->charm += num;
}

void Scene_Player::set_head_id(uint32_t id) {
	base_detail_->head_id = id;
}

void Scene_Player::set_avatar(Int_Vec &avatar_vec) {
	base_detail_->avatar_vec.clear();
	base_detail_->avatar_vec.assign(avatar_vec.begin(), avatar_vec.end());
	broadcast_avatar_to_client();
}

int Scene_Player::hook_scene_change(int src_scene, int des_scene) {
	const Scene_Config * s_scene = CONFIG_CACHE_SCENE->scene_config_cache(
			src_scene);
	const Scene_Config * d_scene = CONFIG_CACHE_SCENE->scene_config_cache(
			des_scene);
	if (!s_scene || !d_scene) {
		return 0;
	}

	Status::hook_scene_change(src_scene, des_scene);
	return 0;
}

void Scene_Player::force_end_battle_scene(void) {
	if (!validate_battle_scene()) return;
	battle_scene()->force_to_recycle_battle_scene();
}

void Scene_Player::force_finish_battle_scene(void){
	if (validate_battle_scene()){
		battle_scene()->force_finish_battle();
	}
}

void Scene_Player::call_ann_battle_in_bonfire_scene(Role_Vec& loser_vec, int scene_id) {

	if (scene_id != 80214001) return;
	if (loser_vec.size() != 1) return;
	Scene_Player *loser = SCENE_MONITOR->find_scene_player_by_role_id(loser_vec[0]);
	if (0 == loser) return;

	SCENE_ANNOUNCE_MANAGER->create_ann_battle_in_bonfire_scene(this, loser);
}

bool Scene_Player::is_system_hosting(void) {

	return is_hosting_;
}

void Scene_Player::notice_enter_scene(void){
	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_NOTICE_ENTER_SCENE);
	inner_buf.finish_message();
	send_to_logic(inner_buf);
}


