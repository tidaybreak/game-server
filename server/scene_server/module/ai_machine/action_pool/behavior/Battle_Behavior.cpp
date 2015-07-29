/*
 * Battle_Behavior.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Battle_Behavior.h"
#include "NPC/NPC_Manager.h"
//#include "Map_Manager.h"
//#include "Map_Player.h"

Bev_Running_Status Be_Battle_Mode_A::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	// 战斗状态时触发第二步
//	if (monster->monster_status() == Monster::FIGHT_STATUS && monster->skill_next_step() == 0)
//		return k_BRS_SUCCESS;
//
//	Time_Value now = Time_Value::gettimeofday();
//
//	role_id_t role_id = monster->most_hatred_role().role_id;
//	if (role_id == 0) return k_BRS_Failure;
//
//	Fighter *attack_obj = monster->find_fighter_in_same_scene(role_id);
//
//	if (attack_obj == 0) {
//		monster->del_from_hatred(role_id);
//		monster->walk_path().reset();
//		return k_BRS_Failure;
//	}
//
//	Coord &current_coord = attack_obj->current_coord();
//	if (attack_obj->dead_status()) {
//		monster->del_from_hatred(role_id);
//		monster->walk_path().reset();
//		return k_BRS_Failure;
//	}
//
//	if (attack_obj->is_invincible_status()) {
//		monster->del_from_hatred(role_id);
//		monster->walk_path().reset();
//		return k_BRS_Failure;
//	}
//
//	if (monster->check_ai_skill_range(monster->monster_detail().current_skill->config_cache->target_select_info, current_coord)) {
//		///  是否追击
//		if (npc->ai_config().distance_of_fight == 0) {
//			monster->del_from_hatred(role_id);
//			monster->walk_path().reset();
//			return k_BRS_Failure;
//		} else {
//			/// 转换到追击状态,清空walk_path,暂停500毫秒后开始追击
//			if (monster->monster_status() != Monster::CHASE_STATUS) {
//				monster->walk_path().reset();
//				monster->set_monster_status(Monster::CHASE_STATUS);
//				monster->push_watcher_tick(0, 300000);
//				return k_BRS_SUCCESS;
//			}
//
//			if (!monster->walk_path().has_walk_path()) {
//				monster->walk_path().walk_path.push_back(current_coord);
//			}
//
//			/// 开始追击
//			if (monster->walk_path().has_walk_coord()) {
//				Grid_Coord coord;
//
//				/// 如果目标移动就清空当前路线
//				monster->walk_path().get_next_coord(coord);
//				if (calculate_grid_distance(monster->walk_path().walk_path.front(), current_coord) >= 3) {
//					monster->walk_path().reset();
//					monster->push_watcher_tick(0, 100000);
//					return k_BRS_SUCCESS;
//				}
//
//				monster->move_watcher_tick(coord);
//				if (monster->move_inner(coord.x, coord.y) != 0) {
//					monster->walk_path().curr_coord_index++;
//				}
//			} else {
//				Grid_Coord target_coord;
//				if (monster->walk_path().get_next_path(target_coord) != 0) {
//					monster->walk_path().reset();
//					return k_BRS_SUCCESS;
//				}
//
//				/// 随机找可攻击坐标
//				monster->scene()->collect_available_coord(monster->mover_detail().layer_id, target_coord, target_coord, 0, 5);
//				monster->path_find(monster->mover_detail().grid_coord, target_coord, monster->walk_path().walk_coord, attack_obj->mover_detail().is_able_arrive);
//
//				if (monster->walk_path().walk_coord.empty()) {
//					MSG_DEBUG("AI: monster fight path find error npc_type:%d, scene:%d  now=> x:%d  y:%d target=> x:%d  y:%d",
//							monster->npc_type_id(), monster->scene_id(), monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, target_coord.x, target_coord.y);
//					monster->push_watcher_tick(1);
//					monster->walk_path().reset();
//					return k_BRS_Failure;
//				}
//
//				monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
//				if (monster->walk_path().curr_coord_index > 0) {
//					Grid_Coord coord;
//					monster->walk_path().get_next_coord(coord);
//					monster->move_watcher_tick(coord);
//					if (monster->move_inner(coord.x, coord.y) != 0) {
//						monster->walk_path().curr_coord_index++;
//					}
//				}
//			}
//		}
//	} else {
//		/// 转换到战斗状态,清空walk_path,暂停400毫秒后开始攻击
//		if (monster->monster_status() != Monster::FIGHT_STATUS) {
//			/// 有重叠并且被攻击周围有可走点且在可攻击范围内时移动
//			Grid_Coord available_coord;
//			if (monster->scene()->grid_mover_count(monster->current_coord(), monster->mover_detail().layer_id) > 1 &&
//					monster->scene()->collect_available_coord(monster->mover_detail().layer_id, current_coord, available_coord, 1, 4) == 0 &&
//					npc->ai_config().distance_of_fight > 0) {
//
//				// 目标点必须在可打范围
//				if (monster->check_ai_skill_range(monster->monster_detail().current_skill->config_cache->target_select_info, current_coord)) {
//
//					// 目标点必须直线可走
//					Coord_Vec walk_coord;
//					if (monster->scene()->path_find_sight_line(monster->mover_detail().layer_id, monster->current_coord(),
//							available_coord, walk_coord, monster->mover_detail().moveable_value, false, true) == 0) {
//						monster->tick_walk() = walk_coord;
//						monster->push_watcher_tick(0, 10);
//						return k_BRS_SUCCESS;
//					}
//				}
//			}
//
//			monster->attack_args().step = 1;
//			monster->walk_path().reset();
//			if (monster->monster_detail().current_skill->config_cache->first_hit_delay != Time_Value::zero)
//				monster->push_watcher_tick(monster->monster_detail().current_skill->config_cache->first_hit_delay);
//			monster->set_monster_status(Monster::FIGHT_STATUS);
//			return k_BRS_SUCCESS;
//		}
//
//		// 控制攻击频率
//		if (monster->check_skill_useable(monster->monster_detail().current_skill, now))
//			return k_BRS_SUCCESS;
//
//		// 技能触发间隔
//		if (monster->check_skill_last_use(now))
//			return k_BRS_SUCCESS;
//
//		// 单步||多步攻击
//		monster->attack_args().step = 1;
//		monster->use_skill_fight(monster->attack_args());
//
//		monster->set_ai_tick(now);
//	}

	return k_BRS_SUCCESS;
}

void Be_Battle_Mode_A::on_terminate(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	bool back = false;
//
//	if (!back) {
//		if (npc->ai_config().distance_of_fight != 0) {
//			int distance = calculate_pixels_distance(npc->mover_detail().grid_coord, npc->birth_coord());
//			if (distance > npc->ai_config().distance_of_fight) {
//				back = true;
//			}
//		}
//	}
//
//	if (!back) {
//		if (npc->ai_config().fight_timeout != Time_Value::zero) {
//			Time_Value now = Time_Value::gettimeofday();
//			if (npc->is_in_combat(now) && now - npc->last_be_attack_timestamp() > npc->ai_config().fight_timeout) {
//				back = true;
//			}
//		}
//	}
//
//	if (back) {
//		monster->set_monster_status(Monster::BACK_STATUS);
//		monster->del_all_hatred();
//
//		monster->walk_path().reset();
//		monster->tick_walk().clear();
//		monster->path_find(monster->mover_detail().grid_coord, monster->birth_coord(), monster->tick_walk(), false);
//
//		if (!monster->tick_walk().empty()) {
//			Grid_Coord coord = monster->tick_walk().back();
//			monster->move_inner(coord.x, coord.y);
//			monster->tick_walk().pop_back();
//			monster->pop_watcher_tick();
//			monster->push_watcher_tick(0, 1003);
//		}
//
//		Status_Info_Detail status_info;
//		if ((get_status(Status_Info_Detail::MONSTER_BACK_BIRTH_BUFF, monster->fighter_id(), 1, status_info)) == 0) {
//			monster->insert_status(status_info);
//		}
//		if (npc->ai_config().exit_combat_buf) {
//			status_info.reset();
//			if ((get_status(npc->ai_config().exit_combat_buf, monster->fighter_id(), 1, status_info)) == 0) {
//				monster->insert_status(status_info);
//			}
//		}
//	}
//
//	if (npc->hatred_map().empty()) {
//		if (monster->monster_status() == Monster::FIGHT_STATUS || monster->monster_status() == Monster::CHASE_STATUS)
//			monster->set_monster_status(Monster::NORMAL_STATUS);
//	}
//
//	// 保持tick
//	monster->push_watcher_tick(0, 500000);
}


Bev_Running_Status Be_Battle_Mode_B::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	// 战斗状态时触发第二步
//	if (monster->skill_next_step() == 0)
//		return k_BRS_SUCCESS;
//
//	Time_Value now = Time_Value::gettimeofday();
//
//	/// 控制攻击频率
//	if (monster->check_skill_useable(monster->monster_detail().current_skill, now))
//		return k_BRS_SUCCESS;
//
//	role_id_t role_id = monster->most_hatred_role().role_id;
//	monster->attack_args().target_id.push_back(role_id);
//	monster->attack_args().skill_coord = monster->mover_detail().grid_coord;
//
//	/// 单步||多步攻击
//	monster->attack_args().step = 1;
//	monster->use_skill_fight(monster->attack_args());
//
//	monster->set_ai_tick(now);

	return k_BRS_SUCCESS;
}

/*

Bev_Running_Status Be_Battle_Mode_A::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();

	Time_Value now = Time_Value::gettimeofday();

	role_id_t role_id = monster->most_hatred_role().role_id;
	if (role_id == 0) return k_BRS_SUCCESS;

	Fighter *attack_obj = 0;
	if (role_id > monster_role_id_range[0] && role_id <= plot_npc_id_range[1]) { /// 怪物
		attack_obj = NPC_MANAGER->find_monster(role_id);
	} else if (get_player_type(role_id) == Pet_Type) {
		attack_obj = MAP_MANAGER_INSTANCE->find_role_id_pet(role_id);
	} else if (get_player_type(role_id) == Person_Type) {
		attack_obj = MAP_MANAGER_INSTANCE->find_role_id_player(role_id);
	} else {
		MSG_USER("AI: error role id:%d", role_id);
		return k_BRS_Failure;
	}

	if (attack_obj == 0) {
		monster->del_from_hatred(role_id);
		monster->walk_path().reset();
		return k_BRS_Failure;
	}

	Grid_Coord &current_coord = attack_obj->current_coord();
	if (attack_obj->dead_status() ||
		!monster->validate_movable(current_coord.grid_x, current_coord.grid_y)) {
		//monster->mover_detail().scene_id != attack_obj->mover_detail().scene_id) {

		monster->del_from_hatred(role_id);
		monster->walk_path().reset();
		return k_BRS_Failure;
	}

	int distance = calculate_pixels_distance(monster->mover_detail().grid_coord, current_coord);
	if (!monster->monster_detail().is_in_attack_distance(distance)) {
		/// 转换到追击状态,清空walk_path,暂停500毫秒后开始追击
		if (monster->monster_status() != Monster::CHASE_STATUS) {
			monster->walk_path().reset();
			monster->set_monster_status(Monster::CHASE_STATUS);
			monster->push_watcher_tick(0, 300000);
			return k_BRS_SUCCESS;
		}

		if (!monster->walk_path().has_walk_path()) {
			monster->walk_path().walk_path.push_back(current_coord);
		}

		/// 开始追击
		if (monster->walk_path().has_walk_coord()) {
			Grid_Coord coord;

			/// 如果目标移动就清空当前路线
			monster->walk_path().get_next_coord(coord);
			if (//monster->scene()->grid_mover_count(coord) > 0 ||
					calculate_grid_distance(monster->walk_path().walk_path.front(), current_coord) >= 3) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->move_watcher_tick(coord);
			monster->move(coord.grid_x, coord.grid_y);
		} else {
			Grid_Coord target_coord;
			if (monster->walk_path().get_next_path(target_coord) != 0) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			/// 目标是否被包围(是则不可走)
			//if (monster->scene()->collect_available_coord(target_coord, available_coord, monster->monster_detail().skill_config.dis, monster->monster_detail().skill_config.dis) != 0) {
			//	monster->push_watcher_tick(1);
			//	monster->walk_path().reset();
			//	return k_BRS_SUCCESS;
			//}
			//if (monster->scene()->collect_available_coord(target_coord, target_coord, 1, 8) == 0) {
			//	printf("x:%d  y:%d \n", target_coord.grid_x, target_coord.grid_y);
			//}

			std::vector<Grid_Coord> grid_path;
			monster->path_find(target_coord, monster->mover_detail().grid_coord, monster->walk_path().walk_coord);

			if (monster->walk_path().walk_coord.empty()) {
				MSG_DEBUG("AI: monster fight path find error npc_type:%d, scene:%d  now=> x:%d  y:%d target=> x:%d  y:%d",
						monster->npc_type_id(), monster->scene_id(), monster->mover_detail().grid_coord.grid_x, monster->mover_detail().grid_coord.grid_y, target_coord.grid_x, target_coord.grid_y);
				monster->push_watcher_tick(1);
				monster->walk_path().reset();
				return k_BRS_Failure;
			}

			monster->walk_path().walk_coord.push_back(target_coord);
			std::reverse(monster->walk_path().walk_coord.begin(), monster->walk_path().walk_coord.end());
			monster->walk_path().walk_coord.pop_back();

			monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
			if (monster->walk_path().curr_coord_index > 0) {
				Grid_Coord coord;
				monster->walk_path().get_next_coord(coord);
				monster->move(coord.grid_x, coord.grid_y);
				monster->push_watcher_tick(0, 0);
			}
		}
	} else {
		/// 转换到战斗状态,清空walk_path,暂停500毫秒后开始攻击
		if (monster->monster_status() != Monster::FIGHT_STATUS) {
			/// 有重叠并且被攻击周围有可走点时处理
			Grid_Coord available_coord;
			if (monster->scene()->grid_mover_count(monster->current_coord()) > 1 &&
					monster->scene()->collect_available_coord(current_coord, available_coord, 2, monster->monster_detail().skill_config.dis) == 0) {
				monster->move(available_coord.grid_x, available_coord.grid_y);
			}

			monster->walk_path().reset();
			monster->push_watcher_tick(0, 500000);
			monster->set_monster_status(Monster::FIGHT_STATUS);
			return k_BRS_SUCCESS;
		}

		/// 控制攻击频率
		if (!monster->monster_detail().is_attack_tick(monster->get_ai_tick()))
			return k_BRS_SUCCESS;

		/// 技能选择
		if (monster->targger_condition(Condition::TARGET_HP_LT, attack_obj) == 0) {
			monster->condition_tick();
		}

		monster->monster_attack_args().attack_args.target_id.push_back(role_id);
		monster->monster_attack_args().attack_args.skill_coord = current_coord;
		monster->monster_attack_args().attack_args.step = 1;
		monster->monster_skill_fight(monster->monster_attack_args());
		monster->monster_attack_args().attack_args.step = 2;

		monster->set_ai_tick(now);
		monster->push_watcher_tick(monster->monster_detail().skill_tick);
	}

	return k_BRS_SUCCESS;
}

*/
