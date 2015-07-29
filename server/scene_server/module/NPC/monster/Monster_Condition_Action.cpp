/*
 * Monster_Condition_Action.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Monster.h"
#include "NPC/NPC_Manager.h"
#include "ai_machine/AI_Machine.h"
#include "Scene_Manager.h"
#include "NPC/NPC_Def.h"


int Monster::condition_action(Time_Value &now, AI_Action_Running &ai_action_running) {
//	// 暂停需要记住当前运行到第几个
//	if (ai_action_running.condition_action_vec.size() <= ai_action_running.condition_action_step_now)
//		ai_action_running.condition_action_step_now = 0;
//	uint condition_action_step_tmp = 0;
//	for (Condition_Action_Vec::iterator it = ai_action_running.condition_action_vec.begin(); it != ai_action_running.condition_action_vec.end(); ++it, ++condition_action_step_tmp) {
//		// 是否被挂起
//		if ((*it).pending_time > now) continue;
//
//		// 条件cd,前提是没死亡
//		if (!npc_dead_state() && !dead_status() && condition_action_step_tmp < ai_action_running.condition_action_step_now) continue;
//
//		// 次数控制
//		if ((*it).action_config->do_action_times) {
//			if ((*it).has_do_action_times >= (*it).action_config->do_action_times) {
//				ai_action_running.condition_action_step_now++;
//				continue;
//			}
//		}
//
//		// 动作冷却时间控制
//		if (now - (*it).last_do_action_time < (*it).action_config->do_action_cool) {
//			ai_action_running.condition_action_step_now++;
//			continue;
//		}
//
//		// 条件冷却时间控制
//		if (now - (*it).last_do_condition_time < (*it).action_config->do_condition_cool) {
//			ai_action_running.condition_action_step_now++;
//			continue;
//		}
//
//
//		// 暂停需要记住当前运行到第几步
//		if ((*it).action_config->action.size() <= (*it).action_step_now)
//			(*it).action_step_now = 0;
//
//		// 当条件达到时所有动作都要执行,即使条件可能已经不成立
//		if ((*it).action_step_now == 0) {
//			bool do_action = true;
//			for (std::vector<Condition>::const_iterator con_it = (*it).action_config->condition.begin(); con_it != (*it).action_config->condition.end(); ++con_it) {
//				switch ((*con_it).condition_type) {
//				case CONDITION_BIRTH: {
//					Time_Value now = Time_Value::gettimeofday();
//					if (now - npc_detail().birth_time < (*con_it).t_val1) {
//						do_action = false;
//					}
//					break;
//				}
//				case CONDITION_SELF_DEATH: {
//					if (!dead_status() && !npc_dead_state()) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_DIE_BY_MONSTER: {
//					if (!npc_dead_state()) {
//						do_action = false;
//					} else {
//						if (get_player_type(killer_by_) == Monster_Type) {
//							do_action = true;
//						} else {
//							do_action = false;
//						}
//					}
//					break;
//				}
//				case CONDITION_DIE_BY_PLAYER: {
//					if (!npc_dead_state()) {
//						do_action = false;
//					} else {
//						if (get_player_type(killer_by_) == Person_Type) {
//							do_action = true;
//						} else {
//							do_action = false;
//						}
//					}
//					break;
//				}
//				case CONDITION_CURRENT_HP_GTE_FIXED: {
//					if (this->fighter_detail_.current_hp() <= (*con_it).d_val1) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_CURRENT_HP_LTE_FIXED: {
//					if (this->fighter_detail_.current_hp() >= (*con_it).d_val1) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_CURRENT_HP_GTE_PRECENT: {
//					double current_hp_max_precent = this->fighter_detail_.current_hp()/this->max_hp();
//					if (current_hp_max_precent < (*con_it).d_val1) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_CURRENT_HP_LTE_PRECENT: {
//					double current_hp_max_precent = this->fighter_detail_.current_hp()/this->max_hp();
//					if (current_hp_max_precent > (*con_it).d_val1) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_CURRENT_HP_GT_LT_PRECENT: {
//					double current_hp_max_precent = this->fighter_detail_.current_hp()/this->max_hp();
//					if (current_hp_max_precent <= (*con_it).d_val1 || current_hp_max_precent >= (*con_it).d_val2) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_BY_DISTANCE: {
//					do_action = false;
//					int buf_size = (*con_it).iv_val1.size();
//					if (buf_size <= 0) break;
//
//					Target_Select_Info info;
//					info.select_type = (*con_it).i_val1;
//					info.range = (*con_it).i_val2;
//					info.target_nums = (*con_it).i_val4;
//					Fighter_Vec fighter_vec;
//
//					std::stringstream target_stream;
//					target_stream << (*con_it).i_val3;
//					int target_type = Utility_Func::target_type(target_stream.str());
//					find_castable_target(target_type, info, gird_coord(), fighter_vec);
//
//					for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//						for(Int_Vec::const_iterator iter_i = (*con_it).iv_val1.begin(); iter_i != (*con_it).iv_val1.end(); ++iter_i) {
//							if((**it_fighter).is_monster() && (**it_fighter).npc_self()->npc_type_id() == *iter_i) {
//								do_action = true;
//								break;
//							}
//						}
//						if(do_action)
//							break;
//					}
//
//					break;
//				}
//				case CONDITION_FIGHTING: {
//					if (enter_fight_timestamp_ == Time_Value::zero) {
//						do_action = false;
//					} else {
//						if (Time_Value::gettimeofday() - enter_fight_timestamp_ < (*con_it).t_val1) {
//							do_action = false;
//						}
//					}
//					break;
//				}
//				case CONDITION_PROBABILITY: {
//					if (100%random() > (*con_it).d_val1 * 100) {
//						do_action = false;
//					}
//					break;
//				}
//				case CONDITION_COORD_X_FRONT: {
//					role_id_t target_id = this->most_hatred_role().role_id;
//					Fighter *target_fighter = this->find_fighter_in_same_scene(target_id);
//					if (target_fighter) {
//						if (target_fighter->gird_coord().x > gird_coord().x) {
//							do_action = false;
//							break;
//						}
//					} else {
//						do_action = false;
//					}
//					break;
//				}
//				case CONDITION_COORD_X_BEHIND: {
//					role_id_t target_id = this->most_hatred_role().role_id;
//					Fighter *target_fighter = this->find_fighter_in_same_scene(target_id);
//					if (target_fighter) {
//						if (target_fighter->gird_coord().x < gird_coord().x) {
//							do_action = false;
//							break;
//						}
//					} else {
//						do_action = false;
//					}
//					break;
//				}
//				case CONDITION_COORD_EQUAL: {
//					do_action = false;
//					if(gird_coord().x == (*con_it).i_val1 && gird_coord().y == (*con_it).i_val2) {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_MASTER_IN_SCENE: {
//					Fighter *master_fighter = find_fighter_in_same_scene(master_id());
//					if (master_fighter) {
//						do_action = false;
//					}
//
//					break;
//				}
//				case CONDITION_DIE_BY_DISTANCE_TARGET: {
//					Fighter *fighter = 0;
//					for (Role_Sort_Set::iterator it = vision_mover_.begin(); it != vision_mover_.end(); ++it) {
//						fighter = find_fighter_in_same_scene(*it);
//						if (fighter && verify_attackable(*fighter)) {
//							if (calculate_grid_distance(mover_detail_.grid_coord, fighter->gird_coord()) <= (*con_it).i_val1) {
//								goto loop;
//								break;
//							}
//						}
//					}
//
//					do_action = false;
//
//					loop:break;
//					break;
//				}
//				case CONDITION_DIE_BY_DISTANCE_TARGET_SPEC: {
//					do_action = false;
//					Fighter *fighter = 0;
//					for (Role_Sort_Set::iterator it = vision_mover_.begin(); it != vision_mover_.end(); ++it) {
//						fighter = find_fighter_in_same_scene(*it);
//						if (fighter && verify_attackable(*fighter)) {
//							if(fighter->is_player() && fighter->role_id() == this->most_hatred_role().role_id) {
//								if (calculate_grid_distance(mover_detail_.grid_coord, fighter->gird_coord()) <= (*con_it).i_val1) {
//									do_action = true;
//									break;
//								}
//							}
//						}
//					}
//
//					break;
//				}
//				case CONDITION_IN_RANGE_X: {
//					Fighter *fighter = 0;
//					bool has_player = false;
//					for (Role_Sort_Set::iterator it = vision_mover_.begin(); it != vision_mover_.end(); ++it) {
//						fighter = find_fighter_in_same_scene(*it);
//						if (fighter && fighter->is_player()) {
//							if (calculate_grid_distance(mover_detail_.grid_coord, fighter->gird_coord()) <= (*con_it).i_val1) {
//								has_player = true;
//								break;
//							}
//						}
//					}
//					if (!has_player)
//						do_action = false;
//
//					break;
//				}
//				case CONDITION_NO_MOVE: {
//					do_action = false;
//					if (now.sec() - this->last_walk_time_ > (*con_it).i_val1) {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_SPECIAL_BE_ATTACK_TIME: {
//					if (Time_Value::gettimeofday() - last_be_attack_timestamp() > (*con_it).t_val1) {
//						do_action = false;
//					}
//					break;
//				}
//				case CONDITION_SPECIAL_TRANSFIGURATION: {
//					role_id_t hater_id = this->most_hatred_role().role_id;
//
//					Fighter *hater_obj = find_fighter_in_same_scene(hater_id);
//					if (!hater_obj) {
//						do_action = false;
//						break;
//					}
//
//					if (hater_obj->has_status_type(10004000)) {
//						do_action = true;
//					} else {
//						do_action = false;
//					}
//
//					break;
//				}
//				case CONDITION_SPECIAL_NOT_TRANSFIGURATION: {
//					role_id_t hater_id = this->most_hatred_role().role_id;
//
//					Fighter *hater_obj = find_fighter_in_same_scene(hater_id);
//					if (!hater_obj) {
//						do_action = false;
//						break;
//					}
//
//					if (hater_obj->has_status_type(10004000)) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//
//					break;
//				}
//				case CONDITION_SPECIAL_HAS_PRIVILEGE: {
//					role_id_t killer_id = killer_by_;
//					Fighter *fighter = this->find_fighter_in_same_scene(killer_id);
//					if (fighter && fighter->is_player() && fighter->player_self()->has_privilege_type((*con_it).i_val1)) {
//						do_action = true;
//					} else {
//						do_action = false;
//					}
//					break;
//				}
//				case CONDITION_SPECIAL_NOT_HAS_PRIVILEGE: {
//					role_id_t killer_id = killer_by_;
//					Fighter *fighter = this->find_fighter_in_same_scene(killer_id);
//					if (fighter && fighter->is_player() && fighter->player_self()->has_privilege_type((*con_it).i_val1)) {
//						do_action = false;
//					} else {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_SCENE_NPC_LIMIT: {
//					Scene::Scene_Layer *scene_layer = this->scene_s()->scene_layer(mover_detail_.layer_id);
//					if (!scene_layer) break;
//					int npc_amount = 0;
//					for (Scene::Mover_Map::iterator it = scene_layer->mover_map_.begin(); it != scene_layer->mover_map_.end(); ++it) {
//						if (it->second->is_monster()) {
//							if (it->second->npc_self()->npc_type_id() == (*con_it).i_val1) {
//								if ((*con_it).i_val3 && it->second->fighter()->master_id() != npc_id()) {
//									continue;
//								}
//
//								npc_amount++;
//								if (npc_amount > (*con_it).i_val2) {
//									do_action = false;
//									break;
//								}
//							}
//						}
//					}
//					break;
//				}
//				case CONDITION_SCENE_NPC_LIMIT_2: {
//					Scene::Scene_Layer *scene_layer = this->scene_s()->scene_layer(mover_detail_.layer_id);
//					if (!scene_layer) break;
//					int npc_amount = 0;
//					for (Scene::Mover_Map::iterator it = scene_layer->mover_map_.begin(); it != scene_layer->mover_map_.end(); ++it) {
//						if (it->second->is_monster()) {
//							if (it->second->npc_self()->npc_type_id() == (*con_it).i_val1 && it->second->role_id() != role_id()) {
//								if ((*con_it).i_val3 && it->second->npc_self()->master_id() != this->master_id()) {
//									continue;
//								}
//								npc_amount++;
//								if (npc_amount > (*con_it).i_val2) {
//									do_action = false;
//									break;
//								}
//							}
//						}
//					}
//					break;
//				}
//				case CONDITION_CHILDREN_EXISTS: {
//					role_id_t master_id = this->role_id();
//					bool has_children = false;
//					Scene::Scene_Layer *scene_layer = this->scene_s()->scene_layer(mover_detail_.layer_id);
//					if (!scene_layer) break;
//					for (Scene::Mover_Map::iterator it = scene_layer->mover_map_.begin(); it != scene_layer->mover_map_.end(); ++it) {
//						if (it->second->is_monster() && it->second->role_id() != master_id) {
//							if (it->second->npc_self()->master_id() == master_id) {
//								has_children = true;
//								break;
//							}
//						}
//					}
//					if (has_children)
//						do_action = true;
//					else
//						do_action = false;
//					break;
//				}
//				case CONDITION_HAS_HATRED: {
//					do_action = false;
//					if(0 == (*con_it).i_val1) {
//						role_id_t hater_id = this->most_hatred_role().role_id;
//
//						Fighter *hater_obj = find_fighter_in_same_scene(hater_id);
//						if (!hater_obj) {
//							do_action = true;
//							break;
//						}
//					} else {
//						for(Hatred_Map::iterator iter = this->hatred_map().begin(); iter != this->hatred_map().end(); ++iter) {
//							role_id_t hater_id = iter->second.role_id;
//
//							Fighter *hater_obj = find_fighter_in_same_scene(hater_id);
//							if (!hater_obj) {
//								do_action = true;
//								break;
//							}
//						}
//					}
//
//					break;
//				}
//				case CONDITION_ARRIVE_TARGET_COORD: {
//					do_action = false;
//					if ((*it).grid_coord_set.empty() || this->gird_coord() == *(*it).grid_coord_set.begin()) {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_ARRIVE_COORD_1_2: {
//					do_action = false;
//					if ((*con_it).i_val1 && (*con_it).i_val2 && (*con_it).i_val1 == this->gird_coord().x && (*con_it).i_val2 == this->gird_coord().y) {
//						do_action = true;
//					}
//					break;
//				}
//				case CONDITION_ATTACH_PLAYER: {
//					do_action = false;
//
//					Pixel_Rect_Range rect;
//					rect.coordA.pixel_x = get_pixel_width_by_coord(this->gird_coord().x - (*con_it).i_val1);
//					rect.coordA.pixel_y = get_pixel_width_by_coord(this->gird_coord().y + (*con_it).i_val1);
//					rect.coordB.pixel_x = get_pixel_width_by_coord(this->gird_coord().x + (*con_it).i_val1);
//					rect.coordB.pixel_y = get_pixel_width_by_coord(this->gird_coord().y + (*con_it).i_val1);
//					rect.coordC.pixel_x = get_pixel_width_by_coord(this->gird_coord().x + (*con_it).i_val1);
//					rect.coordC.pixel_y = get_pixel_width_by_coord(this->gird_coord().y - (*con_it).i_val1);
//					rect.coordD.pixel_x = get_pixel_width_by_coord(this->gird_coord().x - (*con_it).i_val1);
//					rect.coordD.pixel_y = get_pixel_width_by_coord(this->gird_coord().y - (*con_it).i_val1);
//
//					Mover_Hash_Set mover_set;
//					this->scene()->collect_square_mover(this->mover_detail().layer_id, rect, mover_set);
//
//					(*it).mover_set.clear();
//					for (Mover_Hash_Set::const_iterator iter = mover_set.begin(); iter != mover_set.end(); ++iter) {
//						if ((*iter)->is_player()) {
//							(*it).mover_set.insert(*iter);
//						}
//					}
//
//					if (!(*it).mover_set.empty())
//						do_action = true;
//					break;
//				}
//				case CONDITION_MOVE_TO_TARGET_IN_VEC: {
//					if ((*con_it).grid_coord_vec.empty()) {
//						do_action = false;
//						break;
//					}
//
//					if ((*it).grid_coord == Grid_Coord(0, 0)) {	// 初始化
//						(*it).i_val1 = 0;
//						(*it).grid_coord = (*con_it).grid_coord_vec[(*it).i_val1];
//						tick_walk_.clear();
//						path_find(gird_coord(), (*it).grid_coord, tick_walk_, false);
//					}
//
//					if (this->gird_coord() == (*it).grid_coord) {
//						// 防止重复进入，因可能在同一点一直触发
//						int cur_index = 0;	// 当前的坐标位于第几个目标点
//						for (Grid_Coord_Vec::const_iterator iter_gc = (*con_it).grid_coord_vec.begin();
//								iter_gc != (*con_it).grid_coord_vec.end(); ++iter_gc, ++cur_index) {
//							if ((*it).grid_coord == *iter_gc) {
//								break;
//							}
//						}
//						if (cur_index < (*it).i_val1) {
//							do_action = false;
//							break;
//						}
//
//						if (!(*con_it).i_val1 && (*it).i_val1 == 0) {	// 首次不动作
//							do_action = false;
//						} else if (!(*con_it).i_val2 && (*it).i_val1+1 == (int)(*con_it).grid_coord_vec.size()) {	// 最后一次不动作
//							do_action = false;
//						} else {
//							do_action = true;
//						}
//						// 寻路前进
//						if (++(*it).i_val1 < (int)(*con_it).grid_coord_vec.size()) {
//							(*it).grid_coord = (*con_it).grid_coord_vec[(*it).i_val1];
//							tick_walk_.clear();
//							path_find(gird_coord(), (*it).grid_coord, tick_walk_, false);
//						}
//					} else {
//						do_action = false;
//					}
//					break;
//				}
//				default: {
//					break;
//				}
//				}
//
//				if (!do_action) break;
//			}
//
//			if (!do_action) {
//				ai_action_running.condition_action_step_now++;
//				(*it).last_do_condition_time = now;
//				continue;
//			}
//
//			// 有概率不执行动作
//			if ((*it).action_step_now == 0 && 100%(random() + 1) < (*it).action_config->not_do_action_probability) {
//				ai_action_running.condition_action_step_now++;
//				(*it).last_do_condition_time = now;
//
//				(*it).has_do_action_times++;
//				(*it).last_do_action_time = now;
//				continue;
//			}
//		}
//
//		uint action_step_tmp = 0;
//		bool is_need_return_after_action = false;
//		for (std::vector<Action>::const_iterator act_it = (*it).action_config->action.begin(); act_it != (*it).action_config->action.end(); ++act_it, ++action_step_tmp) {
//			if (action_step_tmp < (*it).action_step_now) continue;
//
//			// 死亡时要做完没完成的动作,如条件是死亡
//			if (this->npc_dead_state()) {
//				if ((*it).action_step_now > 0 && (*it).action_step_now < (*it).action_config->action.size()) {
//					no_bt_action_ = true;
//				}
//			}
//
//			switch ((*act_it).action_type) {
//			case ACTION_PAUSE_PENDING: {
//				no_bt_action_ = true;
//				this->pop_watcher_tick();
//				this->push_watcher_tick((*act_it).t_val1);
//
//				is_need_return_after_action = true;
//
//				break;
//			}
//			case ACTION_PAUSE_PENDING_RAND: {
//				Time_Value tmp_time = (*act_it).t_val2 - (*act_it).t_val1;
//				int rand_msec = random()%(tmp_time.sec() * 1000 + tmp_time.usec()/1000);
//				Time_Value pause_time = (*act_it).t_val1 + Time_Value(rand_msec/1000, rand_msec%1000 * 1000);
//				if (pause_time <= Time_Value::zero) {
//					MSG_USER("pause rand error, %ld  %ld", (*act_it).t_val1.sec(), (*act_it).t_val2.sec());
//					break;
//				}
//
//				no_bt_action_ = true;
//				this->pop_watcher_tick();
//				this->push_watcher_tick(pause_time);
//
//				is_need_return_after_action = true;
//
//				break;
//			}
//			case ACTION_PAUSE_NO_PENDING: {
//				(*it).pending_time = now + (*act_it).t_val1;
//
//				is_need_return_after_action = true;
//
//				break;
//			}
//			case ACTION_PAUSE_NO_PENDING_RAND: {
//				Time_Value tmp_time = (*act_it).t_val2 - (*act_it).t_val1;
//				int rand_msec = random()%(tmp_time.sec() * 1000 + tmp_time.usec()/1000);
//				Time_Value pause_time = (*act_it).t_val1 + Time_Value(rand_msec/1000, rand_msec%1000 * 1000);
//				if (pause_time <= Time_Value::zero) {
//					MSG_USER("pause rand error, %ld  %ld", (*act_it).t_val1.sec(), (*act_it).t_val2.sec());
//					break;
//				}
//
//				(*it).pending_time = now + pause_time;
//
//				is_need_return_after_action = true;
//
//				break;
//			}
//			case ACTION_TALK: {
//				MSG_800010 msg;
//				msg.npc_id = npc_id();
//				msg.show_type = (*act_it).i_val2;
//				msg.time = (*act_it).t_val1.sec();
//				if ((*act_it).i_val1)
//					CONFIG_CACHE_INSTANCE->find_npc_talk_config_cache((*act_it).i_val1, msg.content);
//				else
//					msg.content = (*act_it).s_val1;
//				Block_Buffer buf;
//				buf.make_message(ACTIVE_NPC_TALK);
//				msg.serialize(buf);
//				buf.finish_message();
//				talk(buf);
//				break;
//			}
//			case ACTION_DISPLACEMENT_TO_EXISTS_COORD: {
//				if ((*it).grid_coord_set.size() > 0) {
//					Coord des_coord = *((*it).grid_coord_set.begin());
//					displacement(gird_coord(), des_coord, 400015001, 0, 0, false);
//				}
//				break;
//			}
//			case ACTION_USE_SKILL_IN_EXISTS_COORD:
//			case ACTION_USE_SKILL: {
//				if(this->is_silence())
//					break;
//				int skill_id = (*act_it).i_val1 ? (*act_it).i_val1 : monster_detail_.default_skill->skill_id;
//				if (attack_args_.skill_id == skill_id) {
//					// 是否可使用(如cd内)
//					if (!attack_args_.step) {
//						Skill_Running_Info *skill_running_info = find_skill_running_info(skill_id);
//						if (skill_running_info) {
//							Time_Value now = Time_Value::gettimeofday();
//							if (!check_skill_useable(skill_running_info, now)) {
//								attack_args_.step = 1;
//
//								// 如果已经死亡,要做完这个技能
//								if (dead_status() || npc_dead_state()) {
//									no_bt_action_ = true;
//								}
//								return 0;
//								break;
//							}
//						}
//					}
//
//					if (attack_args_.step) {
//						if ((*act_it).action_type == ACTION_USE_SKILL_IN_EXISTS_COORD && (*it).grid_coord_set.size() > 0) {
//							attack_args_.skill_coord = *((*it).grid_coord_set.begin());
//						}
//
//						// 触发技能
//						use_skill_fight(attack_args_);
//
//						// 是否执行完技能
//						if (attack_args_.step) {
//
//							// 在技能内不走行为树
//							no_bt_action_ = true;
//
//							return 0;
//						}
//						break;
//					}
//				} else {
//					// 如果还在释放其它技能就跳出,放完再进来
//					if (attack_args_.step) {
//						return 0;
//					}
//					// 有些技能在死亡时触发
//					if (dead_status() || npc_dead_state() || !attack_args_.step) {
//						Skill_Running_Info *skill_running_info = find_skill_running_info(skill_id);
//						if (skill_running_info) {
//							Time_Value now = Time_Value::gettimeofday();
//							if (check_skill_useable(skill_running_info, now) == 0) {
//								change_current_skill(skill_running_info);
//								attack_args_.step = 1;
//								push_watcher_tick(0, 1002);
//								no_bt_action_ = true;
//								return 0;
//								break;
//							}
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_BUFF_TO_SELF: {
//				if(this->is_silence())
//					break;
//				Status_Info_Detail status_info;
//				status_info.reset();
//				if ((get_status((*act_it).i_val1, mover_id(), 1, status_info)) == 0) {
//					insert_status(status_info);
//				}
//				break;
//			}
//			case ACTION_BUFF_TO_MASTER: {
//				if(this->is_silence())
//					break;
//				Fighter *master_fighter = find_fighter_in_same_scene(master_id());
//				if (!master_fighter) {
//					master_fighter = NPC_MANAGER->find_monster(master_id());
//				}
//				if (master_fighter) {
//					Status_Info_Detail status_info;
//					status_info.reset();
//					if ((get_status((*act_it).i_val1, mover_id(), 1, status_info)) == 0) {
//						master_fighter->insert_status(status_info);
//					}
//				}
//				break;
//			}
//			case ACTION_BUFF_TO_MASTER_SELF: {
//				if(this->is_silence())
//					break;
//				Fighter *master_fighter = find_fighter_in_same_scene(master_id());
//				if (!master_fighter) {
//					master_fighter = NPC_MANAGER->find_monster(master_id());
//				}
//				if (master_fighter) {
//					Status_Info_Detail status_info;
//					status_info.reset();
//					if ((get_status((*act_it).i_val1, mover_id(), 1, status_info)) == 0) {
//						master_fighter->insert_status(status_info);
//					}
//
//					status_info.reset();
//					if ((get_status((*act_it).i_val2, master_id(), 1, status_info)) == 0) {
//						insert_status(status_info);
//					}
//				}
//				break;
//			}
//			case ACTION_BUFF_TO_HATRED_LIST: {
//				if(this->is_silence())
//					break;
//				Status_Info_Detail status_info;
//				status_info.reset();
//				if ((get_status((*act_it).i_val1, mover_id(), 1, status_info)) == 0) {
//					for (Hatred_Map::iterator it = hatred_map_.begin(); it != hatred_map_.end(); ++it) {
//						Fighter *fighter = this->find_fighter_in_same_scene(it->second.role_id);
//						if (fighter) {
//							fighter->insert_status(status_info);
//						}
//					}
//				}
//				break;
//			}
//			case ACTION_BUFF_TO_VISION_ATTACKABLE: {
//				if(this->is_silence())
//					break;
//				Status_Info_Detail status_info;
//				status_info.reset();
//				if ((get_status((*act_it).i_val1, mover_id(), 1, status_info)) == 0) {
//					for (Role_Sort_Set::iterator it = vision_mover_.begin(); it != vision_mover_.end(); ++it) {
//						Fighter *fighter = this->find_fighter_in_same_scene(*it);
//						if (fighter && verify_attackable(*fighter)) {
//							fighter->insert_status(status_info);
//						}
//					}
//				}
//				break;
//			}
//			case ACTION_BUFF_RAND_TO_KILLER_CAMP: {
//				if(this->is_silence())
//					break;
//				int buf_size = (*act_it).iv_val1.size();
//				if (buf_size <= 0) break;
//
//				Fighter *killer = find_fighter_in_same_scene(killer_by_);
//				if (!killer) break;
//
//				int camps = killer->fighter_camps();
//				Scene::Scene_Layer *scene_layer = this->scene()->scene_layer(mover_detail().layer_id);
//				if (scene_layer) {
//					Scene::Mover_Map &mover_map = scene_layer->mover_map_;
//					for (Scene::Mover_Map::iterator iter = mover_map.begin(); iter != mover_map.end(); ++iter) {
//						if (iter->second->fighter()->fighter_camps() == camps) {
//							int status_id = (*act_it).iv_val1[random()%buf_size];
//							iter->second->fighter()->insert_status_all_by_cfg(status_id, fighter_id());
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_BUFF_LINE_TO_SELF_TARGET: {
//				if(this->is_silence())
//					break;
//				role_id_t hater_id = this->most_hatred_role().role_id;
//				int status_type = get_status_type((*act_it).i_val1);
//				if (has_status_type(status_type)) {
//					Status_Info_Detail *status_info = find_status(status_type);
//					if (status_info) {
//						if (status_info->caster_role_id == hater_id) {
//							break;
//						} else {
//							erase_status_type(status_type);
//							Fighter *old_obj = find_fighter_in_same_scene(status_info->caster_role_id);
//							if (old_obj) {
//								old_obj->erase_status_type(status_type);
//							}
//
//							insert_status_all_by_cfg((*act_it).i_val1, hater_id);
//							Fighter *new_obj = find_fighter_in_same_scene(hater_id);
//							if (new_obj) {
//								new_obj->insert_status_all_by_cfg((*act_it).i_val1, fighter_id());
//
//							}
//						}
//					}
//				} else {
//					insert_status_all_by_cfg((*act_it).i_val1, hater_id);
//					Fighter *new_obj = find_fighter_in_same_scene(hater_id);
//					if (new_obj) {
//						new_obj->insert_status_all_by_cfg((*act_it).i_val1, fighter_id());
//
//					}
//				}
//
//
//				break;
//			}
//			case ACTION_BUFF_LINE_TO_SELF_TARGET_VERSION: {
//				if(this->is_silence())
//					break;
//				if(0 == (*act_it).i_val2) {
//					role_id_t hater_id = this->most_hatred_role().role_id;
//					int status_type = get_status_type((*act_it).i_val1);
//					if (has_status_type(status_type)) {
//						Status_Info_Detail *status_info = find_status(status_type);
//						if (status_info) {
//							if (status_info->caster_role_id == hater_id) {
//								break;
//							} else {
//								Fighter *old_obj = find_fighter_in_same_scene(status_info->caster_role_id);
//								if (old_obj) {
//									old_obj->erase_status_type(status_type);
//								}
//								erase_status_type(status_type);
//
//								Fighter *new_obj = find_fighter_in_same_scene(hater_id);
//								if (new_obj) {
//									insert_status_all_by_cfg((*act_it).i_val1, hater_id);
//									new_obj->insert_status_all_by_cfg((*act_it).i_val1, fighter_id());
//								}
//							}
//						}
//					} else {
//						Fighter *new_obj = find_fighter_in_same_scene(hater_id);
//						if (new_obj) {
//							insert_status_all_by_cfg((*act_it).i_val1, hater_id);
//							new_obj->insert_status_all_by_cfg((*act_it).i_val1, fighter_id());
//						}
//					}
//				} else {
//					Target_Select_Info info;
//					info.reset();
//					int status_type = get_status_type((*act_it).i_val1);
//					info.select_type = (*act_it).i_val4;
//					info.range = (*act_it).i_val5;
//					info.target_nums = (*act_it).i_val6;
//					Fighter_Vec fighter_vec;
//
//					std::stringstream target_stream;
//					target_stream << (*act_it).i_val3;
//					int target_type = Utility_Func::target_type(target_stream.str());
//					find_castable_target(target_type, info, gird_coord(), fighter_vec);
//
//					for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//						if (!has_status_type(status_type, (*it_fighter)->role_id())) {
//							Fighter *new_obj = find_fighter_in_same_scene((*it_fighter)->role_id());
//							if (new_obj) {
//								insert_status_all_by_cfg((*act_it).i_val1, (*it_fighter)->role_id());
//								new_obj->insert_status_all_by_cfg((*act_it).i_val1, fighter_id());
//							}
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_STOP_AI: {
//				this->stop_ai();
//				break;
//			}
//			case ACTION_START_MASTER_AI: {
//				Monster *master = NPC_MANAGER->find_monster(this->master_id());
//				if (master && !master->ai_status()) {
//					master->start_ai();
//				}
//				break;
//			}
//			case ACTION_STOP_MOVING: {
//				if (this->tick_walk().empty())
//					break;
//				this->grid_coord_vec() = this->tick_walk();
//				this->tick_walk().clear();
//				break;
//			}
//			case ACTION_START_MASTER_MOVING: {
//				Monster *master = NPC_MANAGER->find_monster(this->master_id());
//				if (master && master->tick_walk().empty() && !master->grid_coord_vec().empty()) {
//					master->tick_walk() = master->grid_coord_vec();
//					master->grid_coord_vec().clear();
//				}
//				break;
//			}
//			case ACTION_BUFF_REMOVE_SCENE: {
//				Scene::Scene_Layer &scene_layer = this->scene()->first_scene_layer();
//				for (Scene::Mover_Map::iterator it = scene_layer.mover_map_.begin(); it != scene_layer.mover_map_.end(); ++it) {
//					if (it->second->fighter()->has_status_type((*act_it).i_val1)) {
//						it->second->fighter()->erase_status_type((*act_it).i_val1);
//					}
//				}
//				break;
//			}
//			case ACTION_BUFF_REMOVE_SELF: {
//				if (has_status_type((*act_it).i_val1)) {
//					erase_status_type((*act_it).i_val1);
//				}
//				break;
//			}
//			case ACTION_BIRTH_NPC_BY_HATER_TOWARD : {
//				if(this->is_silence())
//					break;
//				role_id_t hater_id = this->most_hatred_role().role_id;
//
//				Fighter *hater_obj = find_fighter_in_same_scene(hater_id);
//				if (!hater_obj) break;
//
//				Grid_Coord_Vec path;
//				path_find(gird_coord(), hater_obj->gird_coord(), path, false);
//
//				if (path.empty()) {
//					return 0;
//					break;
//				}
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.birth_coord = path[path.size() -1];
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				npc_addition_info.default_skill = (*act_it).i_val3;
//				if ((*act_it).i_val5) {
//					npc_addition_info.property = fighter_detail().property;
//				}
//				Special_Monster *monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene_s(), npc_addition_info);
//
//				if ((*act_it).i_val4 && monster) {
//					monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//				}
//				break;
//			}
//			case ACTION_BIRTH_NPC_BY_DISTANCE_1: {
//				if(this->is_silence())
//					break;
//				Grid_Coord coord = gird_coord();
//				Special_Monster *monster;
//				int range_grid = (*act_it).i_val3 / 40;
//				range_grid = range_grid <= 0 ? 1 : range_grid;
//				Grid_Coord birth_grid;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				if ((*act_it).d_val1 > 0.0) {
//					get_prop_by_percent(fighter_detail().property, (*act_it).d_val1, npc_addition_info.property);
//				}
//
//				for (int i = 0; i < (*act_it).i_val2; ++i) {
//					birth_grid.x = coord.x - range_grid/2 + random() % range_grid;
//					birth_grid.y = coord.y - range_grid/2 + random() % range_grid;
//
//					if (!validate_movable_by_coord(birth_grid.x, birth_grid.y))
//						birth_grid = coord;
//
//					if ((*act_it).i_val5 && scene_s()->grid_mover_count(birth_grid, 0, 0))
//						continue;
//
//					npc_addition_info.birth_coord = birth_grid;
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//
//					if ((*act_it).i_val4 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_BY_DISTANCE_1_SPEED: {
//				if(this->is_silence())
//					break;
//				Grid_Coord coord = gird_coord();
//				Special_Monster *monster;
//				int range_grid = (*act_it).i_val3 / 40;
//				range_grid = range_grid <= 0 ? 1 : range_grid;
//				Grid_Coord birth_grid;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				if ((*act_it).d_val1 > 0.0) {
//					get_prop_by_percent(fighter_detail().property, (*act_it).d_val1, npc_addition_info.property);
//				}
//
//				for (int i = 0; i < (*act_it).i_val2; ++i) {
//					birth_grid.x = coord.x - range_grid/2 + random() % range_grid;
//					birth_grid.y = coord.y - range_grid/2 + random() % range_grid;
//
//					if (!validate_movable_by_coord(birth_grid.x, birth_grid.y))
//						birth_grid = coord;
//
//					if ((*act_it).i_val5 && scene_s()->grid_mover_count(birth_grid, 0, 0))
//						continue;
//
//					npc_addition_info.birth_coord = birth_grid;
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//
//					if(monster) {
//						for (Int_Vec::const_iterator iv_it = (*act_it).iv_val1.begin(); iv_it != (*act_it).iv_val1.end(); ++iv_it) {
//							this->fighter_detail().no_hatred_mover.insert(*iv_it);
//						}
//					}
//
//					if ((*act_it).i_val6) {
//						this->del_all_hatred();
//					}
//
//					if ((*act_it).i_val4 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//						role_id_t target_id = this->most_hatred_role().role_id;
//						Fighter *target_fighter = this->find_fighter_in_same_scene(target_id);
//						if (target_fighter) {
//							int set_speed = target_fighter->speed_value() * (*act_it).d_val2;
//							if (monster->speed_value() != set_speed) {
//								monster->speed_additional(0, set_speed);
//							}
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_GET_POINT_BY_FIXED_GRID: {
//				Grid_Coord coord = gird_coord();
//				int range_grid = (*act_it).i_val1 <= 0 ? 1 : (*act_it).i_val1;
//				Grid_Coord rand_grid;
//
//				(*it).grid_coord_set.clear();
//				uint coord_amount = (*act_it).i_val2;
//				for (int i = 0; i < (*act_it).i_val2 * 10; ++i) {
//					rand_grid.x = coord.x - range_grid + random() % (range_grid * 2);
//					rand_grid.y = coord.y - range_grid + random() % (range_grid * 2);
//
//					if (!validate_movable_by_coord(rand_grid.x, rand_grid.y))
//						continue;
//
//					if ((*it).grid_coord_set.count(rand_grid))
//						continue;
//
//					bool distance_check = false;
//					for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//						if (calculate_grid_distance(rand_grid, *coord_it) <= (*act_it).i_val3) {
//							distance_check = true;
//							break;
//						}
//					}
//					if (distance_check) continue;
//
//					(*it).grid_coord_set.insert(rand_grid);
//
//					if ((*it).grid_coord_set.size() >= coord_amount)
//						break;
//				}
//				break;
//			}
//			case ACTION_GET_POINT_BY_TARGET_GRID: {
//				(*it).grid_coord_set.clear();
//				role_id_t target_id = this->most_hatred_role().role_id;
//				Fighter *target_fighter = this->find_fighter_in_same_scene(target_id);
//				if (target_fighter) {
//					(*it).grid_coord_set.insert(target_fighter->gird_coord());
//				}
//
//				Grid_Coord coord = gird_coord();
//				int range_grid = (*act_it).i_val1 <= 0 ? 1 : (*act_it).i_val1;
//				Grid_Coord rand_grid;
//
//				uint coord_amount = (*act_it).i_val2;
//				for (int i = 0; i < (*act_it).i_val2 * 10; ++i) {
//					rand_grid.x = coord.x - range_grid + random() % (range_grid * 2);
//					rand_grid.y = coord.y - range_grid + random() % (range_grid * 2);
//
//					if (!validate_movable_by_coord(rand_grid.x, rand_grid.y))
//						continue;
//
//					if ((*it).grid_coord_set.count(rand_grid))
//						continue;
//
//					bool distance_check = false;
//					for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//						if (calculate_grid_distance(rand_grid, *coord_it) <= (*act_it).i_val3) {
//							distance_check = true;
//							break;
//						}
//					}
//					if (distance_check) continue;
//
//					(*it).grid_coord_set.insert(rand_grid);
//
//					if ((*it).grid_coord_set.size() >= coord_amount)
//						break;
//				}
//				break;
//			}
//			case ACTION_GET_POINT_BY_CONFIG: {
//				(*it).grid_coord_set.clear();
//				for (Grid_Coord_Vec::const_iterator act_it_iter = (*act_it).grid_coord_vec.begin(); act_it_iter != (*act_it).grid_coord_vec.end(); ++act_it_iter) {
//					(*it).grid_coord_set.insert((*act_it_iter));
//				}
//				break;
//			}
//			case ACTION_GET_POINT_WITH_ONE_PLAYER: {
//				if (scene()) {
//					std::vector<Map_Player> player_vec;
//					scene()->collect_scene_player(player_vec);
//					if (player_vec.size() > 0) {
//						size_t rand_index = rand() % player_vec.size();
//
//						if (rand_index < 0 || rand_index >= player_vec.size())
//							break;
//						(*it).grid_coord_set.clear();
//						(*it).grid_coord_set.insert(player_vec[rand_index].mover_detail().grid_coord);
//					}
//				}
//				break;
//			}
//			case ACTION_BIRTH_NPC_BY_DISTANCE_2: {
//				if(this->is_silence())
//					break;
//				Grid_Coord coord = gird_coord();
//				Special_Monster *monster;
//				int range_grid = (*act_it).i_val3 / 40;
//				range_grid = range_grid <= 0 ? 1 : range_grid;
//				Grid_Coord birth_grid;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				if ((*act_it).d_val1 > 0.0) {
//					double new_blood = this->max_hp() * (*act_it).d_val1;
//					const NPC_Config_Cache *config_cache = CONFIG_CACHE_INSTANCE->find_npc_config_cache((*act_it).i_val1);
//					if (!config_cache) {
//						MSG_USER("find npc config:%d error", (*act_it).i_val1);
//						break;
//					}
//					npc_addition_info.property = config_cache->property;
//					npc_addition_info.property[Fighter_Detail::HP][Fighter_Detail::BASIC].fixed_value = new_blood;
//					npc_addition_info.property[Fighter_Detail::HP_MAX][Fighter_Detail::BASIC].fixed_value = new_blood;
//				}
//
//				for (int i = 0; i < (*act_it).i_val2; ++i) {
//					birth_grid.x = coord.x - range_grid/2 + random() % range_grid;
//					birth_grid.y = coord.y - range_grid/2 + random() % range_grid;
//
//					if (!validate_movable_by_coord(birth_grid.x, birth_grid.y))
//						birth_grid = coord;
//
//					if ((*act_it).i_val5 && scene_s()->grid_mover_count(birth_grid, 0, 0))
//						continue;
//
//					npc_addition_info.birth_coord = birth_grid;
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//
//					if ((*act_it).i_val4 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_BY_DISTANCE_3: {
//				if(this->is_silence())
//					break;
//				Grid_Coord coord = gird_coord();
//				Special_Monster *monster;
//				int range_grid = (*act_it).i_val3 / 40;
//				range_grid = range_grid <= 0 ? 1 : range_grid;
//				Grid_Coord birth_grid;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				if ((*act_it).d_val1 > 0.0) {
//					get_prop_by_percent(fighter_detail().property, (*act_it).d_val1, npc_addition_info.property);
//				}
//
//				for (Int_Vec::const_iterator it = (*act_it).iv_val1.begin(); it != (*act_it).iv_val1.end(); ++it) {
//					birth_grid.x = coord.x - range_grid/2 + random() % range_grid;
//					birth_grid.y = coord.y - range_grid/2 + random() % range_grid;
//
//					if (!validate_movable_by_coord(birth_grid.x, birth_grid.y))
//						birth_grid = coord;
//
//					if ((*act_it).i_val5 && scene_s()->grid_mover_count(birth_grid, 0, 0))
//						continue;
//
//					npc_addition_info.birth_coord = birth_grid;
//					monster = NPC_MANAGER->create_special_monster(*it, scene(), npc_addition_info);
//
//					if ((*act_it).i_val4 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//				}
//
//				break;
//			}
//			case ACTION_PROP_BLOOD_FIXED: {
//				map_set_prop((*act_it).d_val1, Fighter_Detail::HP, Fighter_Detail::BASIC);
//				break;
//			}
//			case ACTION_PROP_PRE_ATTACK: {
//				double physical_attack = fighter_detail_.fetch_property(Fighter_Detail::PHYSICAL_ATTACK) * (*act_it).d_val1;
//				double spell_attack = fighter_detail_.fetch_property(Fighter_Detail::SPELL_ATTACK) * (*act_it).d_val1;
//				map_set_prop(physical_attack, Fighter_Detail::PHYSICAL_ATTACK, Fighter_Detail::BASIC);
//				map_set_prop(spell_attack, Fighter_Detail::SPELL_ATTACK, Fighter_Detail::BASIC);
//				break;
//			}
//			case ACTION_PROP_BASE: {
//				Fighter_Detail::Property_Type prop_type = Fighter_Detail::Property_Type((*act_it).i_val1);
//				double prop_value = fighter_detail_.fetch_property(prop_type, Fighter_Detail::ALL_DURABLE_BIT) * (*act_it).d_val1 + (*act_it).d_val2;
//				prop_value = prop_value < 0.0 ? 0.0 : prop_value;
//				prop_value = prop_value > INT_MAX ? INT_MAX : prop_value;
//				map_set_prop(prop_value, prop_type, Fighter_Detail::BASIC);
//				break;
//			}
//			case ACTION_MODIFY_PROP_BASE: {
//				Fighter_Detail::Property_Type prop_type = Fighter_Detail::Property_Type((*act_it).i_val1);
//				double prop_value = fighter_detail_.fetch_property(prop_type, Fighter_Detail::ALL_DURABLE_BIT) * (*act_it).d_val1 + (*act_it).d_val2;
//				prop_value = prop_value > INT_MAX ? INT_MAX : prop_value;
//				map_modify_prop(prop_value, prop_type, Fighter_Detail::BASIC);
//				break;
//			}
//			case ACTION_MODIFY_PROP_BASE_IN_DISTANCE: {
//				int buf_size = (*act_it).iv_val1.size();
//				if (buf_size <= 0) break;
//
//				Target_Select_Info info;
//				info.select_type = (*act_it).i_val1;
//				info.range = (*act_it).i_val2;
//				info.target_nums = (*act_it).i_val4;
//				Fighter_Vec fighter_vec;
//
//				std::stringstream target_stream;
//				target_stream << (*act_it).i_val3;
//				int target_type = Utility_Func::target_type(target_stream.str());
//				find_castable_target(target_type, info, gird_coord(), fighter_vec);
//
//				bool do_ac;
//				if((*it).i_val1) {//-
//					do_ac = true;
//					for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//						for(Int_Vec::const_iterator iter_i = (*act_it).iv_val1.begin(); iter_i != (*act_it).iv_val1.end(); ++iter_i) {
//							if((**it_fighter).is_monster() && (**it_fighter).npc_self()->npc_type_id() == *iter_i) {
//								do_ac = false;
//								break;
//							}
//						}
//						if(!do_ac)
//							break;
//					}
//					if(do_ac) {
//						(*it).i_val1 = 0;
//						Fighter_Detail::Property_Type prop_type = Fighter_Detail::Property_Type((*act_it).i_val5);
//						double val = monster_detail_.config_cache->property[prop_type][Fighter_Detail::BASIC].fixed_value;
//						double prop_value =  val* (*act_it).d_val1 + (*act_it).d_val2;
//						prop_value = prop_value > INT_MAX ? INT_MAX : prop_value;
//						prop_value = -prop_value;
//						map_modify_prop(prop_value, prop_type, Fighter_Detail::BASIC, UPDATE_PROPERTY_VISIBLE);
//						if((*act_it).i_val6 && (*act_it).i_val7) {
//							Status_Info_Detail status_info;
//							status_info.reset();
//							if ((get_status((*act_it).i_val6, mover_id(), 1, status_info)) == 0) {
//								erase_status_type(status_info.status_type);
//							}
//						}
//					}
//				} else {//+
//					do_ac = false;
//					Fighter_Vec::iterator it_fighter = fighter_vec.begin();
//					for (; it_fighter != fighter_vec.end(); ++it_fighter) {
//						for(Int_Vec::const_iterator iter_i = (*act_it).iv_val1.begin(); iter_i != (*act_it).iv_val1.end(); ++iter_i) {
//							if((**it_fighter).is_monster() && (**it_fighter).npc_self()->npc_type_id() == *iter_i) {
//								do_ac = true;
//								break;
//							}
//						}
//						if(do_ac)
//							break;
//					}
//					if(do_ac)
//					{
//						(*it).i_val1 = 1;
//						Fighter_Detail::Property_Type prop_type = Fighter_Detail::Property_Type((*act_it).i_val5);
//						double val = monster_detail_.config_cache->property[prop_type][Fighter_Detail::BASIC].fixed_value;
//						double prop_value =  val * (*act_it).d_val1 + (*act_it).d_val2;
//						prop_value = prop_value > INT_MAX ? INT_MAX : prop_value;
//						map_modify_prop(prop_value, prop_type, Fighter_Detail::BASIC, UPDATE_PROPERTY_VISIBLE);
//					}
//				}
//
//				do_ac = true;
//				for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//					for(Int_Vec::const_iterator iter_i = (*act_it).iv_val1.begin(); iter_i != (*act_it).iv_val1.end(); ++iter_i) {
//						if((**it_fighter).is_monster() && (**it_fighter).npc_self()->npc_type_id() == *iter_i) {
//							do_ac = false;
//							break;
//						}
//					}
//					if(!do_ac)
//						break;
//				}
//				if(do_ac) {
//					if((*act_it).i_val6 && (*act_it).i_val7) {
//						Status_Info_Detail status_info;
//						status_info.reset();
//						if ((get_status((*act_it).i_val6, mover_id(), 1, status_info)) == 0) {
//							erase_status_type(status_info.status_type);
//						}
//					}
//				}
//
//				do_ac = false;
//				Fighter_Vec fighters;
//				fighters.clear();
//				Fighter_Vec::iterator it_fighter = fighter_vec.begin();
//				for (; it_fighter != fighter_vec.end(); ++it_fighter) {
//					for(Int_Vec::const_iterator iter_i = (*act_it).iv_val1.begin(); iter_i != (*act_it).iv_val1.end(); ++iter_i) {
//						if((**it_fighter).is_monster() && (**it_fighter).npc_self()->npc_type_id() == *iter_i) {
//							do_ac = true;
//							fighters.push_back(*it_fighter);
//						}
//					}
//				}
//				if(do_ac)
//				{
//					if((*act_it).i_val6 && (*act_it).i_val7) {
//						Status_Info_Detail status_info;
//						for (Fighter_Vec::iterator it_fighter = fighters.begin(); it_fighter != fighters.end(); ++it_fighter) {
//							status_info.reset();
//							if ((get_status((*act_it).i_val6, mover_id(), 1, status_info)) == 0) {
//								(*it_fighter)->insert_status(status_info);
//							}
//
//							status_info.reset();
//							if ((get_status((*act_it).i_val7, (*it_fighter)->mover_id(), 1, status_info)) == 0) {
//								insert_status(status_info);
//							}
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_PROP_BLOOD_TO_MASTER: {
//				Fighter *master_fighter = find_fighter_in_same_scene(master_id());
//				if (!master_fighter) {
//					master_fighter = NPC_MANAGER->find_monster(master_id());
//				}
//				if (master_fighter) {
//					double blood = this->current_hp() * (*act_it).d_val1;
//					map_modify_prop(-blood, Fighter_Detail::HP, Fighter_Detail::BASIC);
//					master_fighter->map_modify_prop(blood, Fighter_Detail::HP, Fighter_Detail::BASIC);
//				}
//
//				break;
//			}
//			case ACTION_SPECIAL_DIE: {
//				this->normal_die_self();
//
//				break;
//			}
//			case ACTION_DIE_BY_MASTER: {
//				if (npc_status() == RECOVER_STATUS || npc_status() == DEAD_STATUS) return -1;
//
//				Attack_Info info;
//				info.attacker = this->master_id();
//
//				Fighter *killer = MAP_MANAGER_INSTANCE->find_role_id_player(info.attacker);
//				if (killer) {
//					die_process(info, *killer);
//				} else {
//					die_process(info, *this);
//				}
//				break;
//			}
//			case ACTION_RECOVER_SELF: {
//				this->recover_self();
//
//				break;
//			}
//			case ACTION_SPECIAL_KEEP_FOLLOW: {
//				role_id_t target_id = this->most_hatred_role().role_id;
//				Fighter *target_fighter = this->find_fighter_in_same_scene(target_id);
//				if (target_fighter) {
//					if (target_fighter->gird_coord().x < gird_coord().x) {
//						int set_speed = target_fighter->speed_value() * 2;
//						if (this->speed_value() != set_speed) {
//							this->speed_additional(0, set_speed);
//						}
//					} else {
//						int distance = calculate_pixels_distance(gird_coord(), target_fighter->gird_coord());
//						if (distance > (*act_it).i_val1) {
//							int basic_speed = this->speed_value();
//							this->speed_additional(0, basic_speed + (*act_it).i_val2);
//						}
//
//						if (distance < (*act_it).i_val3) {
//							if (this->speed_value() != monster_detail_.config_cache->speed)
//								this->speed_additional(0, monster_detail_.config_cache->speed);
//						}
//					}
//				}
//				break;
//			}
//			case ACTION_SPECIAL_SET_SPEED: {
//				role_id_t target_id = this->most_hatred_role().role_id;
//				Fighter *target_fighter = this->find_fighter_in_same_scene(target_id);
//				if (target_fighter) {
//					int set_speed = target_fighter->speed_value() * (*act_it).d_val1;
//					if (this->speed_value() != set_speed) {
//						this->speed_additional(0, set_speed);
//					}
//				}
//				break;
//			}
//			case ACTION_SPECIAL_HATRED_BY_INDEX: {
//				if (hatred_map().empty() && validate_mover_detail()) {
//					Scene::Scene_Layer &scene_layer = scene_s()->first_scene_layer();
//					bool find_mover = false;
//					for (Int_Vec::const_iterator iv_it = (*act_it).iv_val1.begin(); iv_it != (*act_it).iv_val1.end(); ++iv_it) {
//						for (Scene::Mover_Map::iterator mover_it = scene_layer.mover_map_.begin(); mover_it != scene_layer.mover_map_.end(); ++mover_it) {
//							if (*iv_it == mover_it->second->mover_type()) {
//								if (!mover_it->second->fighter()->dead_status()) {
//									this->fighter_detail_.no_hatred_mover.erase(*iv_it);
//									this->add_to_hatred(*mover_it->second->fighter(), 100);
//									find_mover = true;
//									break;
//								}
//							}
//						}
//
//						if (find_mover) break;
//					}
//				}
//				break;
//			}
//			case ACTION_SPECIAL_FILTER_HATRED: {
//				for (Int_Vec::const_iterator iv_it = (*act_it).iv_val1.begin(); iv_it != (*act_it).iv_val1.end(); ++iv_it) {
//					this->fighter_detail_.no_hatred_mover.insert(*iv_it);
//				}
//				break;
//			}
//			case ACTION_SPECIAL_GOTO_BIRTH_COORD: {
//				if (gird_coord() != birth_coord()) {
//					tick_walk_.clear();
//					path_find(gird_coord(), birth_coord(), tick_walk_, false);
//					if (tick_walk_.empty()) {
//						MSG_USER("find path error scene:%d %d %d %d %d". scene_id(), gird_coord().x, gird_coord().y, birth_coord().x, birth_coord().y);
//					} else {
//						is_need_return_after_action = true;
//					}
//				}
//
//				break;
//			}
//			case ACTION_SPECIAL_APPEAR: {
//				if (!scene_s()->find_mover_with_layer(mover_detail().layer_id, role_id())) {
//					if (this->fighter()->current_hp() < 1.0) {
//						this->fighter()->map_set_prop(1.0, Fighter_Detail::HP, Fighter_Detail::BASIC);
//					}
//					this->appear();
//				} else {
//					MSG_USER("NPC can find mover, npc type = %d", this->npc_type_id());
//				}
//				break;
//			}
//			case ACTION_SPECIAL_DISAPPEAR: {
//				this->move_disappear();
//				break;
//			}
//			case ACTION_SPECIAL_REALIVE: {
//				this->set_npc_state(ALIVE_STATUS);
//				break;
//			}
//			case ACTION_SPECIAL_SET_INVINCIBLE: {
//				set_invincible_attrib((*act_it).i_val1);
//				break;
//			}
//			case ACTION_SPECIAL_DROP: {
//				NPC_MANAGER->drop_by_amount((*act_it).i_val1, this->gird_coord(), this->scene(), 1);
//				break;
//			}
//			case ACTION_SPECIAL_DROP_DIE: {
//				NPC_MANAGER->drop_by_monster((*act_it).i_val1, this, killer_by_, monster_detail_.config_cache->die_drop_type);
//				break;
//			}
//			case ACTION_SPECIAL_DIE_DROP: {
//				die_drop_ = (*act_it).i_val1;
//				break;
//			}
//			case ACTION_SPECIAL_GOTO_COORD: {
//				Grid_Coord target_coord = Grid_Coord((*act_it).i_val1, (*act_it).i_val2);
//				if (gird_coord() != target_coord) {
//					tick_walk_.clear();
//					path_find(gird_coord(), target_coord, tick_walk_, false);
//					if (tick_walk_.empty()) {
//						MSG_USER("find path error scene:%d %d %d %d %d". scene_id(), gird_coord().x, gird_coord().y, target_coord.x, target_coord.y);
//					}
//				}
//
//				break;
//			}
//			case ACTION_SPECIAL_RAND_MOVE: {
//				Grid_Coord target_coord;
//				int rand_dis = (*act_it).i_val1 + random()%((*act_it).i_val2 - (*act_it).i_val1);
//				int ret = scene()->collect_available_coord(mover_detail_.layer_id, this->gird_coord(), target_coord, rand_dis, (*act_it).i_val2);
//				if (ret != 0) {
//					scene()->collect_available_coord(mover_detail_.layer_id, this->gird_coord(), target_coord, (*act_it).i_val1, (*act_it).i_val2);
//				}
//				if (validate_movable_by_coord(target_coord.x, target_coord.y) && gird_coord() != target_coord) {
//					tick_walk_.clear();
//					path_find(gird_coord(), target_coord, tick_walk_, false);
//					if (tick_walk_.empty()) {
//						MSG_USER("find path error scene:%d %d %d %d %d". scene_id(), gird_coord().x, gird_coord().y, target_coord.x, target_coord.y);
//					}
//				}
//				break;
//			}
//			case ACTION_MOVE_RAND_FROM_BIRTH: {
//				Grid_Coord target_coord;
//				int rand_dis = (*act_it).i_val1 + random()%((*act_it).i_val2 - (*act_it).i_val1);
//				int ret = scene()->collect_available_coord(mover_detail_.layer_id, this->npc_detail().birth_coord, target_coord, rand_dis, (*act_it).i_val2);
//				if (ret != 0) {
//					scene()->collect_available_coord(mover_detail_.layer_id, this->npc_detail().birth_coord, target_coord, (*act_it).i_val1, (*act_it).i_val2);
//				}
//				if (validate_movable_by_coord(target_coord.x, target_coord.y) && gird_coord() != target_coord) {
//					tick_walk_.clear();
//					path_find(gird_coord(), target_coord, tick_walk_, false);
//					if (tick_walk_.empty()) {
//						MSG_USER("find path error scene:%d %d %d %d %d". scene_id(), gird_coord().x, gird_coord().y, target_coord.x, target_coord.y);
//					}
//				}
//				break;
//			}
//			case ACTION_FAIL_GAHTER_MONSTER: {
//				this->gather_monster_retrun(1, false, NULL, true);
//				//
//				Role_Info_Addition addit;
//				addit.reset();
//				addit.type = RAT_GATHER_MONSTER;
//				get_addition(addit);
//				addit.int64_vec.clear();
//				set_addition(addit);
//				break;
//			}
//			case ACTION_SPECIAL_FLY_CONTROL: {
//				int fly_type = (*act_it).i_val1 == 1 ? ADDITION_FLY_UP : ADDITION_FLY_DOWN;
//
//				if ((*act_it).i_val2 == 1) {
//					Role_Info_Addition addition;
//					addition.type = fly_type;
//					addition.int_value1 = (*act_it).t_val1.sec() * 1000 + (*act_it).t_val1.usec();
//					this->set_addition(addition);
//					if (fly_type == ADDITION_FLY_UP) {
//						mover_detail().grid_coord.coord_z = 10;
//					} else {
//						mover_detail().grid_coord.coord_z = 0;
//					}
//
//					set_invincible_attrib(true);
//					set_outof_control_attrib(true);
//
//					no_bt_action_ = true;
//					this->pop_watcher_tick();
//					this->push_watcher_tick((*act_it).t_val1);
//					is_need_return_after_action = true;
//				} else {
//					this->del_addition(fly_type);
//
//					if (fly_type == ADDITION_FLY_DOWN) {
//						set_invincible_attrib(false);
//						set_outof_control_attrib(false);
//					}
//				}
//				break;
//			}
//			case ACTION_SPECIAL_ADDITION_INFO_40: {
//				if ((*act_it).t_val1 != Time_Value::zero) {
//					Time_Value life_left = (*act_it).t_val1 - (Time_Value::gettimeofday() - npc_detail().birth_time);
//					if (life_left > Time_Value::zero) {
//						Role_Info_Addition addition;
//						addition.type = ADDITION_COUNTDOWN;
//						addition.int_value1 = life_left.sec() * 1000 + life_left.usec()/1000;
//						this->set_addition(addition);
//					} else {
//						this->del_addition(ADDITION_COUNTDOWN);
//					}
//				}
//				break;
//			}
//			case ACTION_SPECIAL_ARROW_TOWARD: {
//				MSG_805700 msg;
//				msg.toward = (*act_it).i_val1;
//				Block_Buffer buf;
//				buf.make_message(INNER_ARROW_TOWARD);
//				msg.serialize(buf);
//				this->scene()->broadcast_all(mover_detail_.layer_id, buf);
//				break;
//			}
//			case ACTION_SPECIAL_NAME_TO_MASTER: {
//				if (master_id()) {
//					Player_Type player_type = get_player_type(master_id());
//					if (player_type == Monster_Type) {
//						Monster *master = NPC_MANAGER->find_monster(master_id());
//						if (master) {
//							monster_detail().name = master->monster_detail().name + "的" + monster_detail().name;
//							monster_detail().master_name = master->monster_detail().name;
//							appear();
//						}
//					} else if (player_type == Person_Type) {
//						Map_Player *master = MAP_MANAGER_INSTANCE->find_role_id_player(master_id());
//						if (master) {
//							monster_detail().name = master->base_detail().role_name + "的" + monster_detail().name;
//							monster_detail().master_name = master->base_detail().role_name;
//							appear();
//						}
//					}
//				}
//				break;
//			}
//			case ACTION_SPECIAL_NAME_TO_MASTER_MASTER: {
//				if (master_id()) {
//					Monster *monster = NPC_MANAGER->find_monster(master_id());
//					if (monster && monster->monster_detail().master_name != "") {
//						monster_detail().name = monster->monster_detail().master_name + "的" + monster_detail().name;
//						appear();
//					}
//				}
//				break;
//			}
//			case ACTION_BIRTH_NPC_BY_TARGET_X_Y: {
//				if(this->is_silence())
//					break;
//				role_id_t target_id = this->most_hatred_role().role_id;
//				Fighter *target_fighter = this->find_fighter_in_same_scene(target_id);
//				if (target_fighter) {
//					Grid_Coord coord = target_fighter->gird_coord();
//					coord.x += (*act_it).i_val6;
//					coord.y += (*act_it).i_val7;
//
//					Special_Monster *monster;
//					Grid_Coord birth_grid;
//
//					NPC_Addition_Info npc_addition_info;
//					npc_addition_info.layer_id = mover_detail().layer_id;
//					npc_addition_info.identify = this->get_friend_foe_identify();
//					if ((*act_it).i_val5) {
//						npc_addition_info.property = fighter_detail().property;
//					}
//					Int_Int_Map birth_coord_filter;
//					int loop_max = 0;
//					for (int i = 0; i < (*act_it).i_val2; ++loop_max) {
//						if (loop_max > 100) break;
//
//						if ((*act_it).i_val8) {
//							birth_grid.x = coord.x + random() % (*act_it).i_val8;
//						} else {
//							birth_grid.x = coord.x;
//						}
//
//						if ((*act_it).i_val9) {
//							birth_grid.y = coord.y + random() % (*act_it).i_val9;
//						} else {
//							birth_grid.y = coord.y;
//						}
//
//						//if (birth_coord_filter[birth_grid.x] == birth_grid.y) continue;
//
//						++i;
//						birth_coord_filter[birth_grid.x] = birth_grid.y;
//
//						if (scene_s()->grid_mover_count(birth_grid, 0, 0)) continue;
//						npc_addition_info.birth_coord = birth_grid;
//						monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//
//						if ((*act_it).i_val4 && monster) {
//							monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//						}
//
//						if ((*act_it).i_val3 && monster) {
//							monster->push_walk_path(gird_coord());
//						}
//
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_BY_FIXED_COORD: {
//				if(this->is_silence())
//					break;
//				Grid_Coord coord((*act_it).i_val3, (*act_it).i_val4);
//				Special_Monster *monster;
//				Grid_Coord birth_grid;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				if ((*act_it).d_val1 > 0.0) {
//					get_prop_by_percent(fighter_detail().property, (*act_it).d_val1, npc_addition_info.property);
//				}
//
//				Int_Int_Map birth_coord_filter;
//				int loop_max = 0;
//				for (int i = 0; i < (*act_it).i_val2; ++loop_max) {
//					if (loop_max > 100) break;
//
//					birth_grid.x = coord.x - (*act_it).i_val5 + random() % ((*act_it).i_val5 * 2);
//					birth_grid.y = coord.y - (*act_it).i_val6 + random() % ((*act_it).i_val6 * 2);
//
//					if (birth_coord_filter[birth_grid.x] == birth_grid.y) continue;
//
//					++i;
//					birth_coord_filter[birth_grid.x] = birth_grid.y;
//
//					if (scene_s()->grid_mover_count(birth_grid, 0, 0)) continue;
//					npc_addition_info.birth_coord = birth_grid;
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//
//					if ((*act_it).i_val7 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_IN_RANGE: {
//				if(this->is_silence())
//					break;
//				Special_Monster *monster;
//				Grid_Coord birth_grid;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//
//				Int_Int_Map birth_coord_filter;
//				int loop_max = 0;
//				for (int i = 0; i < (*act_it).i_val5; ++loop_max) {
//					if (loop_max > 100) break;
//
//					int is_right = rand() % 2;
//					int is_up = rand() % 2;
//					int x_delta = (*act_it).i_val2 ? random() % (*act_it).i_val2 : 0;
//					int y_delta = (*act_it).i_val4 ? random() % (*act_it).i_val4 : 0;
//					if (is_right) {
//						birth_grid.x = this->gird_coord().x + (*act_it).i_val1 + x_delta;
//					} else {
//						birth_grid.x = this->gird_coord().x - (*act_it).i_val1 - x_delta;
//					}
//
//					if (is_up) {
//						birth_grid.y = this->gird_coord().y + (*act_it).i_val3 + y_delta;
//					} else {
//						birth_grid.y = this->gird_coord().y - (*act_it).i_val3 - y_delta;
//					}
//
//					if (birth_coord_filter[birth_grid.x] == birth_grid.y) continue;
//
//					++i;
//					birth_coord_filter[birth_grid.x] = birth_grid.y;
//
//					if (scene_s()->grid_mover_count(birth_grid, 0, 0)) continue;
//					npc_addition_info.birth_coord = birth_grid;
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val6, scene(), npc_addition_info);
//
//					if ((*act_it).i_val7 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_IN_RANGE_LIMIT: {
//				if(this->is_silence())
//					break;
//				Special_Monster *monster;
//				Grid_Coord birth_grid;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//
//				Int_Int_Map birth_coord_filter;
//				int loop_max = 0;
//				for (int i = 0; i < (*act_it).i_val5; ++loop_max) {
//					if (loop_max > 100) break;
//
//					int is_right = rand() % 2;
//					int is_up = rand() % 2;
//					int x_delta = (*act_it).i_val2 ? random() % (*act_it).i_val2 : 0;
//					int y_delta = (*act_it).i_val4 ? random() % (*act_it).i_val4 : 0;
//					if (is_right) {
//						birth_grid.x = this->gird_coord().x + (*act_it).i_val1 + x_delta;
//					} else {
//						birth_grid.x = this->gird_coord().x - (*act_it).i_val1 - x_delta;
//					}
//
//					if (is_up) {
//						birth_grid.y = this->gird_coord().y + (*act_it).i_val3 + y_delta;
//					} else {
//						birth_grid.y = this->gird_coord().y - (*act_it).i_val3 - y_delta;
//					}
//
//					if (birth_coord_filter[birth_grid.x] == birth_grid.y) continue;
//
//					// 判断寻路路径长度是否超过指定值{i_val8}
//					Grid_Coord_Vec path_vec;
//					path_find(gird_coord(), birth_grid, path_vec, false);
//					if (path_vec.size() > uint32_t((*act_it).i_val8))
//						continue;
//
//					++i;
//					birth_coord_filter[birth_grid.x] = birth_grid.y;
//
//					if (scene_s()->grid_mover_count(birth_grid, 0, 0)) continue;
//					npc_addition_info.birth_coord = birth_grid;
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val6, scene(), npc_addition_info);
//
//					if ((*act_it).i_val7 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_AT_NPC_BEHIND: {
//				if(this->is_silence())
//					break;
//				Pixel_Rect_Range rect;
//				int reverse_toward = mover_detail().toward > 4 ? mover_detail().toward - 4 : mover_detail().toward + 4;
//				get_rect(gird_coord(), gird_coord(), (*act_it).i_val3, (*act_it).i_val4, rect, reverse_toward);
//				Grid_Coord coord;
//				Grid_Coord_Vec grid_coord_vec;
//				int offset_distance = (*act_it).i_val3 + (*act_it).i_val4;
//				for (int i = -offset_distance; i < offset_distance; ++i) {
//					for (int j = -offset_distance; j < offset_distance; ++j) {
//						coord.x = gird_coord().x + i;
//						coord.y = gird_coord().y + j;
//						if (!validate_movable_by_coord(coord.x, coord.y)) {
//							continue;
//						}
//						if (is_in_square(rect, coord)) {
//							grid_coord_vec.push_back(coord);
//						}
//					}
//				}
//
//				if (!grid_coord_vec.empty()) {
//					NPC_Addition_Info npc_addition_info;
//					npc_addition_info.layer_id = mover_detail().layer_id;
//					npc_addition_info.identify = this->get_friend_foe_identify();
//					for (int i = 0; i < (*act_it).i_val2; ++i) {
//						int coord_index = random()%grid_coord_vec.size();
//						npc_addition_info.birth_coord = grid_coord_vec[coord_index];
//						NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_AT_NPC_BEHIND_MOST_HATRED: {
//				if(this->is_silence())
//					break;
//				Special_Monster *monster;
//				Pixel_Rect_Range rect;
//				int reverse_toward = mover_detail().toward > 4 ? mover_detail().toward - 4 : mover_detail().toward + 4;
//				get_rect(gird_coord(), gird_coord(), (*act_it).i_val3, (*act_it).i_val4, rect, reverse_toward);
//				Grid_Coord coord;
//				Grid_Coord_Vec grid_coord_vec;
//				int offset_distance = (*act_it).i_val3 + (*act_it).i_val4;
//				for (int i = -offset_distance; i < offset_distance; ++i) {
//					for (int j = -offset_distance; j < offset_distance; ++j) {
//						coord.x = gird_coord().x + i;
//						coord.y = gird_coord().y + j;
//						if (!validate_movable_by_coord(coord.x, coord.y)) {
//							continue;
//						}
//						if (is_in_square(rect, coord)) {
//							grid_coord_vec.push_back(coord);
//						}
//					}
//				}
//
//				if (!grid_coord_vec.empty()) {
//					NPC_Addition_Info npc_addition_info;
//					npc_addition_info.layer_id = mover_detail().layer_id;
//					npc_addition_info.identify = this->get_friend_foe_identify();
//					for (int i = 0; i < (*act_it).i_val2; ++i) {
//						int coord_index = random()%grid_coord_vec.size();
//						npc_addition_info.birth_coord = grid_coord_vec[coord_index];
//						monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//						if (monster) {
//							if (!this->hatred_map().empty()) {
//								uint32_t size = this->hatred_map().size();
//								Hatred_Map::const_iterator first = this->hatred_map().begin();
//								uint32_t rand = std::rand() % size;
//								std::advance(first, rand);		// 随机抽中玩家
//								if( first != this->hatred_map().end()) {
//									monster->add_to_hatred(first->first, monster->max_hp());
//									hatred_lock_timestamp_ = Time_Value(50000);
//								}
//							}
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_AT_GRID_COORD: {
//				if(this->is_silence())
//					break;
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				for (std::vector<Grid_Coord>::const_iterator it = (*act_it).grid_coord_vec.begin(); it != (*act_it).grid_coord_vec.end(); ++it) {
//					npc_addition_info.birth_coord = *it;
//					if (scene_s()->grid_mover_count(npc_addition_info.birth_coord, 0, 0)) continue;
//					Monster *monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//					if (monster && (*act_it).i_val2) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//				}
//
//				break;
//			}
//			case ACTION_BIRTH_NPC_AS_SQUARE: {
//				if(this->is_silence())
//					break;
//				Grid_Coord coord;
//				Grid_Coord_Vec grid_coord_vec;
//				for (int i = -1; i <= 1; i++) {
//					for (int j = -1; j <= 1; j++) {
//						if (i == 0 || j == 0) continue;
//						coord.x = gird_coord().x + (*act_it).i_val5 * i;
//						coord.y = gird_coord().y + (*act_it).i_val6 * j;
//						if (this->validate_movable_by_coord(coord.x, coord.y))
//							grid_coord_vec.push_back(coord);
//					}
//				}
//				int npc_amount = 4 - grid_coord_vec.size();
//				int distance = std::max((*act_it).i_val5, (*act_it).i_val6);
//				for (int i = 0; i < npc_amount * 10; ++npc_amount) {
//					int ret = scene_s()->collect_available_coord(mover_detail().layer_id, gird_coord(), coord, distance + 1, distance + 3);
//					if (!ret && coord != gird_coord()) {
//						grid_coord_vec.push_back(coord);
//					}
//					if (grid_coord_vec.size() == 4) break;
//				}
//
//				Special_Monster *monster;
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.identify = this->get_friend_foe_identify();
//				if ((*act_it).d_val1 > 0.0) {
//					get_prop_by_percent(fighter_detail().property, (*act_it).d_val1, npc_addition_info.property);
//				}
//
//				for (Grid_Coord_Vec::iterator it = grid_coord_vec.begin(); it != grid_coord_vec.end(); ++it) {
//					npc_addition_info.birth_coord = *it;
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val1, scene(), npc_addition_info);
//
//					if ((*act_it).i_val2 && monster) {
//						monster->add_to_hatred(this->most_hatred_role().role_id, 100);
//					}
//
//					if ((*act_it).i_val4 && monster) {
//						monster->push_walk_path(gird_coord());
//					}
//				}
//
//				break;
//			}
//			case ACTION_EFFECT_BY_X_Y_OFFSET: {
//				MSG_800512 msg;
//				msg.use_type = 20;
//				msg.effect_id = (*act_it).i_val1;
//				Coord coord;
//				uint effect_amount = 0;
//				if ((*act_it).i_val3 > (*act_it).i_val2) {
//					effect_amount = (*act_it).i_val2 + random()%((*act_it).i_val3 - (*act_it).i_val2);
//				} else {
//					effect_amount = (*act_it).i_val2;
//				}
//				Int_Int_Map filter_map;
//				for (uint i = 0; i < effect_amount * 5; ++i) {
//					coord.x = gird_coord().x - (*act_it).i_val6 + random()%((*act_it).i_val6 * 2);
//					coord.y = gird_coord().y - (*act_it).i_val7 + random()%((*act_it).i_val7 * 2);
//
//					if (filter_map[coord.x] == coord.y) continue;
//
//					filter_map[coord.x] = coord.y;
//					msg.start_coord.push_back(coord);
//
//					if (msg.start_coord.size() == effect_amount) break;
//				}
//				Block_Buffer buf;
//				buf.make_message(ACTIVE_NOTICE_CLIENT_EFFECT);
//				msg.serialize(buf);
//				buf.finish_message();
//				broadcast_around(buf);
//				break;
//			}
//			case ACTION_EFFECT_ON_SELF: {
//				MSG_800512 msg;
//				msg.use_type = 20;
//				msg.toward = (*act_it).i_val2 == 9 ? mover_detail_.toward : (*act_it).i_val2;
//				msg.effect_id = (*act_it).i_val1;
//				msg.attack_id.push_back(fighter_id());
//				msg.start_coord.push_back(gird_coord());
//				Block_Buffer buf;
//				buf.make_message(ACTIVE_NOTICE_CLIENT_EFFECT);
//				msg.serialize(buf);
//				buf.finish_message();
//				broadcast_around(buf);
//				break;
//			}
//			case ACTION_EFFECT_ON_SELF_ACTION: {
//				MSG_800502 msg;
//				msg.attack_id = fighter_id();
//				msg.skill_id = (*act_it).i_val1 ? (*act_it).i_val1 : 400001001;
//				Grid_Coord map_coord = gird_coord();
//				msg.px = (*act_it).i_val4 ? (*act_it).i_val4 : map_coord.x;
//				msg.py = (*act_it).i_val5 ? (*act_it).i_val5 : map_coord.y;
//				msg.step = (*act_it).i_val2;
//				msg.action = (*act_it).i_val3;
//				Block_Buffer buf;
//				buf.make_message(ACTIVE_FIGHT_ACTION_BROADCAST);
//				msg.serialize(buf);
//				buf.finish_message();
//				broadcast_around(buf);
//				break;
//			}
//			case ACTION_EFFECT_ON_FIXED_COORD: {
//				MSG_800512 msg;
//				msg.use_type = 20;
//				msg.effect_id = (*act_it).i_val1;
//				Coord coord;
//				for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//					coord.x = (*coord_it).x;
//					coord.y = (*coord_it).y;
//					msg.start_coord.push_back(coord);
//				}
//				Block_Buffer buf;
//				buf.make_message(ACTIVE_NOTICE_CLIENT_EFFECT);
//				msg.serialize(buf);
//				buf.finish_message();
//				broadcast_around(buf);
//				break;
//			}
//			case ACTION_EFFECT_LINE_SELF_COORD_CACHE: {
//				MSG_800512 msg;
//				msg.use_type = 40;
//				msg.effect_id = (*act_it).i_val1;
//				msg.speed = (*act_it).i_val2;
//				Coord coord;
//				msg.attack_id.push_back(fighter_id());
//				Grid_Coord map_coord;
//				get_map_coord(map_coord);
//				for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//					coord.x = (*coord_it).x;
//					coord.y = (*coord_it).y;
//					msg.end_coord.push_back(coord);
//					int distance = calculate_pixels_distance(map_coord, (*coord_it));
//					int usec = distance/(*act_it).i_val2 * 1000;
//					Time_Value action_time = Time_Value(usec/1000, usec%1000);
//					AI_Event *ai_event = create_ai_event(action_time, (*act_it).i_val3);
//					if (ai_event)
//						ai_event->set_cache_coord((*coord_it));
//				}
//				Block_Buffer buf;
//				buf.make_message(ACTIVE_NOTICE_CLIENT_EFFECT);
//				msg.serialize(buf);
//				buf.finish_message();
//				broadcast_around(buf);
//				break;
//			}
//			case ACTION_EFFECT_LINE_SELF_COORD_COORD_CACHE: {
//				MSG_800512 msg;
//				msg.use_type = 40;
//				msg.effect_id = (*act_it).i_val1;
//				msg.speed = (*act_it).i_val2;
//				Coord coord, start_coord;
//				Grid_Coord map_coord = gird_coord();
//				map_coord.x += (*act_it).i_val4;
//				map_coord.y += (*act_it).i_val5;
//				map_coord.x = map_coord.x < 0 ? 0 : map_coord.x;
//				map_coord.y = map_coord.y < 0 ? 0 : map_coord.y;
//				start_coord.x = map_coord.x;
//				start_coord.y = map_coord.y;
//				msg.start_coord.push_back(start_coord);
//				for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//					coord.x = (*coord_it).x;
//					coord.y = (*coord_it).y;
//					msg.end_coord.push_back(coord);
//					double distance = calculate_pixels_distance(map_coord, (*coord_it));
//					int usec = distance/(*act_it).i_val2 * 1000;
//					Time_Value action_time = Time_Value(usec/1000, usec%1000 * 1000);
//					AI_Event *ai_event = create_ai_event(action_time, (*act_it).i_val3);
//					if (ai_event)
//						ai_event->set_cache_coord((*coord_it));
//				}
//				Block_Buffer buf;
//				buf.make_message(ACTIVE_NOTICE_CLIENT_EFFECT);
//				msg.serialize(buf);
//				buf.finish_message();
//				broadcast_around(buf);
//				break;
//			}
//			case ACTION_HURT_BASE_BY_DISTANCE: {
//				if(this->is_silence())
//					break;
//				Target_Select_Info info;
//				info.select_type = (*act_it).i_val1;
//				info.range = (*act_it).i_val2;
//				Fighter_Vec fighter_vec;
//
//				std::stringstream target_stream;
//				target_stream << (*act_it).i_val3;
//				int target_type = Utility_Func::target_type(target_stream.str());
//				find_castable_target(target_type, info, gird_coord(), fighter_vec);
//
//				Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, (*act_it).i_val5, (*act_it).d_val1, (*act_it).d_val2);
//				attack_param.effect_id = 10090201;
//				Attack_Result attack_result;
//				int result = 0;
//				bool is_attack_hit_any = false;
//				for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//					attack_result.reset();
//					result = attack_fighter(attack_param, **it_fighter, attack_result, get_attack_by_param);
//					if (ATTACK_NOT_HIT != result) {
//						is_attack_hit_any = true;
//					}
//				}
//				break;
//			}
//			case ACTION_HURT_BASE_BY_FIXED_COORD: {
//				if(this->is_silence())
//					break;
//				for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//					Target_Select_Info info;
//					info.select_type = (*act_it).i_val1;
//					info.range = (*act_it).i_val2;
//					info.target_nums = (*act_it).i_val4;
//					Fighter_Vec fighter_vec;
//
//					std::stringstream target_stream;
//					target_stream << (*act_it).i_val3;
//					int target_type = Utility_Func::target_type(target_stream.str());
//					find_castable_target(target_type, info, *coord_it, fighter_vec);
//
//					Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, (*act_it).i_val5, (*act_it).d_val1, (*act_it).d_val2);
//					attack_param.effect_id = 10090201;
//					Attack_Result attack_result;
//					int result = 0;
//					bool is_attack_hit_any = false;
//					for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//						attack_result.reset();
//						result = attack_fighter(attack_param, **it_fighter, attack_result, get_attack_by_param);
//						if (ATTACK_NOT_HIT != result) {
//							is_attack_hit_any = true;
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_HURT_MAXHP_PERCENT_BY_FIXED_COORD: {
//				if(this->is_silence())
//					break;
//				for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//					Target_Select_Info info;
//					info.select_type = (*act_it).i_val1;
//					info.range = (*act_it).i_val2;
//					info.target_nums = (*act_it).i_val4;
//					Fighter_Vec fighter_vec;
//
//					std::stringstream target_stream;
//					target_stream << (*act_it).i_val3;
//					int target_type = Utility_Func::target_type(target_stream.str());
//					find_castable_target(target_type, info, *coord_it, fighter_vec);
//
//					Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, (*act_it).i_val5, (*act_it).d_val1, (*act_it).d_val2);
//					attack_param.effect_id = 10090201;
//					Attack_Result attack_result;
//					int result = 0;
//					bool is_attack_hit_any = false;
//					for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//						attack_result.reset();
//						result = attack_fighter(attack_param, **it_fighter, attack_result, get_attack_by_peerfighter_maxhp);
//						if (ATTACK_NOT_HIT != result) {
//							is_attack_hit_any = true;
//						}
//					}
//				}
//
//				break;
//			}
//			case ACTION_HURT_BASE_BY_FIXED_COORD_LINE: {
//				if(this->is_silence())
//					break;
//				Pixel_Rect_Range rect;
//				int dis = 0, range = 0, toward = 0, select_type = 0, target_nums =0;
//				select_type = (*act_it).i_val1;
//				range = (*act_it).i_val2;
//				target_nums = (*act_it).i_val4;
//				dis = (*act_it).i_val6;
//				toward = ((*act_it).i_val7 > 9 || (*act_it).i_val7 < 1) ? 6 : (*act_it).i_val7;
//				Grid_Coord coord;
//				coord.x = (*act_it).i_val8;
//				coord.y = (*act_it).i_val9;
//
//				std::stringstream target_stream;
//				target_stream << (*act_it).i_val3;
//				int target_type = Utility_Func::target_type(target_stream.str());
//
//				Target_Select_Info info;
//				info.select_type = select_type;
//				info.range = range;
//				info.target_nums = target_nums;
//				info.param1 = dis;
//				info.param2 = toward;
//				//info.
//
//				Fighter_Vec fighter_vec;
//				find_castable_target(target_type, info, coord, fighter_vec);
//
//				Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, (*act_it).i_val5, (*act_it).d_val1, (*act_it).d_val2);
//				attack_param.effect_id = 10090201;
//				Attack_Result attack_result;
//				int result = 0;
//				bool is_attack_hit_any = false;
//				for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//					attack_result.reset();
//					result = attack_fighter(attack_param, **it_fighter, attack_result, get_attack_by_param);
//					if (ATTACK_NOT_HIT != result) {
//						is_attack_hit_any = true;
//					}
//				}
//
//				break;
//			}
//			case ACTION_HURT_MAXHP_PERCENT_BY_FIXED_COORD_LINE: {
//				if(this->is_silence())
//					break;
//				Pixel_Rect_Range rect;
//				int dis = 0, range = 0, toward = 0, select_type = 0, target_nums =0;
//				select_type = (*act_it).i_val1;
//				range = (*act_it).i_val2;
//				target_nums = (*act_it).i_val4;
//				dis = (*act_it).i_val6;
//				toward = ((*act_it).i_val7 > 9 || (*act_it).i_val7 < 1) ? 6 : (*act_it).i_val7;
//				Grid_Coord coord;
//				coord.x = (*act_it).i_val8;
//				coord.y = (*act_it).i_val9;
//
//				std::stringstream target_stream;
//				target_stream << (*act_it).i_val3;
//				int target_type = Utility_Func::target_type(target_stream.str());
//
//				Target_Select_Info info;
//				info.select_type = select_type;
//				info.range = range;
//				info.target_nums = target_nums;
//				info.param1 = dis;
//				info.param2 = toward;
//				//info.
//
//				Fighter_Vec fighter_vec;
//				find_castable_target(target_type, info, coord, fighter_vec);
//
//				Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, (*act_it).i_val5, (*act_it).d_val1, (*act_it).d_val2);
//				attack_param.effect_id = 10090201;
//				Attack_Result attack_result;
//				int result = 0;
//				bool is_attack_hit_any = false;
//				for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//					attack_result.reset();
//					result = attack_fighter(attack_param, **it_fighter, attack_result, get_attack_by_peerfighter_maxhp);
//					if (ATTACK_NOT_HIT != result) {
//						is_attack_hit_any = true;
//					}
//				}
//
//				break;
//			}
//			case ACTION_HURT_SHARE_BY_DISTANCE: {
//				if(this->is_silence())
//					break;
//				Target_Select_Info info;
//				info.select_type = (*act_it).i_val1;
//				info.range = (*act_it).i_val2;
//				Fighter_Vec fighter_vec;
//
//				std::stringstream target_stream;
//				target_stream << (*act_it).i_val3;
//				int target_type = Utility_Func::target_type(target_stream.str());
//				find_castable_target(target_type, info, gird_coord(), fighter_vec);
//
//				int peer_amount = fighter_vec.size();
//				if (!peer_amount) break;
//				Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, (*act_it).i_val5, (*act_it).d_val1/peer_amount, (*act_it).d_val2/peer_amount);
//				attack_param.effect_id = 10090201;
//				Attack_Result attack_result;
//				int result = 0;
//				bool is_attack_hit_any = false;
//				for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//					attack_result.reset();
//					result = attack_fighter(attack_param, **it_fighter, attack_result, get_attack_by_param);
//					if (ATTACK_NOT_HIT != result) {
//						is_attack_hit_any = true;
//					}
//				}
//				break;
//			}
//			case ACTION_HURT_SHARE_BY_DISTANCE_WITH_EFFECT: {
//				if(this->is_silence())
//					break;
//				Target_Select_Info info;
//				info.select_type = (*act_it).i_val1;
//				info.range = (*act_it).i_val2;
//				Fighter_Vec fighter_vec;
//
//				std::stringstream target_stream;
//				target_stream << (*act_it).i_val3;
//				int target_type = Utility_Func::target_type(target_stream.str());
//				find_castable_target(target_type, info, gird_coord(), fighter_vec);
//
//				int peer_amount = fighter_vec.size();
//				if (!peer_amount) break;
//				Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, (*act_it).i_val5, (*act_it).d_val1/peer_amount, (*act_it).d_val2/peer_amount);
//				attack_param.effect_id = 10090201;
//				Attack_Result attack_result;
//				int result = 0;
//				bool is_attack_hit_any = false;
//				for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//					attack_result.reset();
//					result = attack_fighter(attack_param, **it_fighter, attack_result, get_attack_by_param);
//					if (ATTACK_NOT_HIT != result) {
//						is_attack_hit_any = true;
//
//						Status_Info_Detail status_info;
//						status_info.reset();
//						if ((get_status((*act_it).i_val6, mover_id(), 1, status_info)) == 0) {
//							(*it_fighter)->insert_status(status_info);
//						}
//					}
//				}
//				break;
//			}
//			case ACTION_BUFF_TO_DISTANCE: {
//				if(this->is_silence())
//					break;
//				Target_Select_Info info;
//				info.select_type = (*act_it).i_val1;
//				info.range = (*act_it).i_val2;
//				Fighter_Vec fighter_vec;
//
//				std::stringstream target_stream;
//				target_stream << (*act_it).i_val3;
//				int target_type = Utility_Func::target_type(target_stream.str());
//				find_castable_target(target_type, info, gird_coord(), fighter_vec);
//
//				Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, 0, (*act_it).d_val1, (*act_it).d_val2);
//				Attack_Result attack_result;
//				for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//					for (Int_Vec::const_iterator it = (*act_it).iv_val1.begin(); it != (*act_it).iv_val1.end(); ++it) {
//						(*it_fighter)->insert_status_all_by_cfg(*it, fighter_id(), 1, master_id());
//					}
//				}
//				break;
//			}
//			case ACTION_BUFF_TO_FIXED_COORD: {
//				if(this->is_silence())
//					break;
//				for (Grid_Coord_Set::iterator coord_it = (*it).grid_coord_set.begin(); coord_it != (*it).grid_coord_set.end(); ++coord_it) {
//					Target_Select_Info info;
//					info.select_type = (*act_it).i_val1;
//					info.range = (*act_it).i_val2;
//					Fighter_Vec fighter_vec;
//
//					std::stringstream target_stream;
//					target_stream << (*act_it).i_val3;
//					int target_type = Utility_Func::target_type(target_stream.str());
//					find_castable_target(target_type, info, *coord_it, fighter_vec);
//
//					Attack_Param attack_param(this->monster_detail_.current_skill->skill_id, 0, (*act_it).d_val1, (*act_it).d_val2);
//					Attack_Result attack_result;
//					for (Fighter_Vec::iterator it_fighter = fighter_vec.begin(); it_fighter != fighter_vec.end(); ++it_fighter) {
//						for (Int_Vec::const_iterator it = (*act_it).iv_val1.begin(); it != (*act_it).iv_val1.end(); ++it) {
//							(*it_fighter)->insert_status_all_by_cfg(*it, fighter_id(), 1, master_id());
//						}
//					}
//				}
//				break;
//			}
//			case ACTION_N_TIMES_AND_DIE: {
//				if ((*it).has_do_action_times >= (*act_it).i_val1) {
//					this->normal_die_self();
//				}
//				break;
//			}
//			case ACTION_CLEAN_NPC_SELF: {
//				if (scene()) {
//					std::vector<Mover> mover_vec;
//					scene()->collect_npc_children(this->role_id(), mover_vec);
//					for (std::vector<Mover>::iterator iter_c = mover_vec.begin(); iter_c != mover_vec.end(); ++iter_c) {
//						if (iter_c->is_npc() && iter_c->npc_self()) {
//							iter_c->npc_self()->normal_die_self();
//						}
//					}
//				}
//				break;
//			}
//			case ACTION_BIRTH_NPC_RAND: {
//				if(this->is_silence())
//					break;
//				bool do_must = false;
//				if ((*act_it).i_val4) {
//					role_id_t killer_id = killer_by_;
//					Fighter *fighter = this->find_fighter_in_same_scene(killer_id);
//					if (fighter && fighter->is_player() && fighter->player_self()->has_status_type((*act_it).i_val4)) {
//						do_must = true;
//					}
//				}
//
//				if (!do_must) {
//					int rand_value = rand() % 100;
//					if (rand_value > (*act_it).i_val1)
//						break;
//				}
//
//				Special_Monster *monster;
//				Grid_Coord birth_grid = this->gird_coord();
//
//				NPC_Addition_Info npc_addition_info;
//				npc_addition_info.layer_id = mover_detail().layer_id;
//				npc_addition_info.birth_coord = birth_grid;
//				npc_addition_info.identify.master_id = this->role_id();
//				npc_addition_info.identify.final_master = this->master_id();
//
//				Int_Int_Map birth_coord_filter;
//				for (int i = 0; i < (*act_it).i_val3; ++i) {
//					monster = NPC_MANAGER->create_special_monster((*act_it).i_val2, scene(), npc_addition_info);
//				}
//				break;
//			}
//			case ACTION_FIND_PATH_AND_GO: {
//				if (!(*act_it).grid_coord_vec.empty()) {
//					Grid_Coord_Vec coord_vec_tmp;
//					for (Grid_Coord_Vec::const_iterator iter_gc = (*act_it).grid_coord_vec.begin();
//							iter_gc != (*act_it).grid_coord_vec.end(); ++iter_gc) {
//						if (*iter_gc != this->gird_coord()) {
//							coord_vec_tmp.push_back(*iter_gc);
//						}
//					}
//
//					if (!coord_vec_tmp.empty()) {
//						Grid_Coord des_coord = coord_vec_tmp[rand() % coord_vec_tmp.size()];
//
//						tick_walk_.clear();
//						path_find(gird_coord(), des_coord, tick_walk_, false);
//
//						(*it).grid_coord_set.clear();
//						(*it).grid_coord_set.insert(des_coord);
//					}
//				}
//				break;
//			}
//			case ACTION_SKILL_TO_ARROUND: {
//				if ((*it).mover_set.empty())
//					break;
//
//
//				break;
//			}
//			case ACTION_DISAPPEAR_MONSTER: {
//				Fighter *master_fighter = NPC_MANAGER->find_monster(master_id());
//				if (master_fighter) {
//					master_fighter->npc_self()->normal_die_self();
//				}
//				//this->normal_die_self();
//				break;
//			}
//			case ACTION_ADD_BUF_TO_RAND_PLAYER: {
//				if(this->is_silence())
//					break;
//				Role_Vec role_vec;
//				role_vec.clear();
//				scene()->collect_scene_player(role_vec);
//
//				if (role_vec.empty())
//					break;
//
//				size_t rand_index = rand() % role_vec.size();
//				if (rand_index < 0 || rand_index >= role_vec.size())
//					break;
//
//				Map_Player *player = MAP_MANAGER_INSTANCE->find_role_id_player(role_vec[rand_index]);
//				player->insert_status_all_by_cfg((*act_it).i_val1, mover_id(), 1, mover_id());
//				break;
//			}
//			case ACTION_CHANGER_MASTER_MOST_HATED: {
//				Fighter *master_fighter = NPC_MANAGER->find_monster(master_id());
//				if (master_fighter && this->most_hatred_role().role_id) {
//					master_fighter->del_all_hatred();
//					master_fighter->add_to_hatred(this->most_hatred_role().role_id, 100);
//				}
//				break;
//			}
//			case ACTION_HATE_MASTER: {
//				Fighter *master_fighter = NPC_MANAGER->find_monster(master_id());
//				if (master_fighter) {
//					this->del_all_hatred();
//					this->add_to_hatred(master_fighter->role_id(), 100);
//				}
//				break;
//			}
//			case ACTION_CHOOSE_TARGET_POINT_TO_RUN: {
//				if ((*it).i_val1 < 0 || (*it).i_val1 >= (int)(*act_it).grid_coord_vec.size())
//					break;
//
//				(*it).grid_coord = (*act_it).grid_coord_vec[(*it).i_val1++];
//				tick_walk_.clear();
//				path_find(gird_coord(), (*it).grid_coord, tick_walk_, false);
//				if (tick_walk_.empty()) {
//					MSG_USER("find path error scene:%d %d %d %d %d". scene_id(), gird_coord().x, gird_coord().y, birth_coord().x, birth_coord().y);
//				}
//				break;
//			}
//			case ACTION_CHOOSE_TARGET_POINT_TO_JUDGE: {
//				if ((*it).i_val1 < 0 || (*it).i_val1 >= (int)(*act_it).grid_coord_vec.size())
//					break;
//
//				(*it).grid_coord = (*act_it).grid_coord_vec[(*it).i_val1++];
//				break;
//			}
//			case ACTION_MOVE_TO_FIX_COORD: {
//				tick_walk_.clear();
//				Grid_Coord target_coord((*act_it).i_val1, (*act_it).i_val2);
//				path_find(gird_coord(), target_coord, tick_walk_, false);
//				if (tick_walk_.empty()) {
//					MSG_USER("find path error scene:%d %d %d %d %d". scene_id(), gird_coord().x, gird_coord().y, birth_coord().x, birth_coord().y);
//				}
//				break;
//			}
//			default: {
//				break;
//			}
//			}
//
//			(*it).action_step_now++;
//
//			// 返回前要判断是否完成动作
//			if (is_need_return_after_action) {
//				if ((*it).action_step_now == (*it).action_config->action.size()) {
//					(*it).has_do_action_times++;
//					(*it).last_do_action_time = now;
//				}
//				return 0;
//			}
//		}
//
//		// 记录执行次数和时间
//		(*it).has_do_action_times++;
//		(*it).last_do_action_time = now;
//
//	}

	return 0;
}
