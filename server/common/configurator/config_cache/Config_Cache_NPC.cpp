/*
 * Task_Config.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: Linqiyou
 */

#include "Config_Cache_NPC.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Fighter.h"

const int NPC_PROPS[] = { PT_PHY_ATTACK, PT_SPELL_ATTACK, PT_PHY_DEFENSE,
		PT_SPELL_DEFENSE, PT_HIT, PT_AVOID, PT_BLOOD_MAX, PT_CRIT, PT_CRIT_DEF,
		PT_CRIT_HURT, PT_CURE_RATE, PT_BE_CURE_RATE,};

const NPC_Config_Cache* Config_Cache_NPC::find_npc_config_cache(
		const role_id_t id) const {
	NPC_Config_Cache_Map::const_iterator it = npc_config_map_.find(id);
	if (it != npc_config_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

Monster_Section_Config* Config_Cache_NPC::get_monster_section_config(
		int scene_key) {
	Monster_Section_Config_Map::iterator it = monster_section_config_map_.find(
			scene_key);
	if (it != monster_section_config_map_.end()) {
		return &(it->second);
	} else {
		Monster_Section_Config section;
		monster_section_config_map_.insert(std::make_pair(scene_key, section));
		Monster_Section_Config_Map::iterator it =
				monster_section_config_map_.find(scene_key);
		if (it != monster_section_config_map_.end()) {
			return &(it->second);
		}
	}
	return NULL;
}

void Config_Cache_NPC::refresh_drop_config(void) {
	const Json::Value& config_list =
	CONFIG_INSTANCE->config_json()["npc"]["drop"];
	Drop_Config cache;
	for (Json::Value::const_iterator it = config_list.begin();
			it != config_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}
		if (config_json.isMember("jid")) {
			cache.jid = config_json["jid"].asInt();
		}
		if (config_json.isMember("name")) {
			cache.name = config_json["name"].asString();
		}
		if (config_json.isMember("rate_type")) {
			cache.rate_type = config_json["rate_type"].asInt();
		}

		//box_rate
		if (config_json.isMember("drop_box_rate")) {
			for (Json::Value::iterator iter =
					config_json["drop_box_rate"].begin();
					iter != config_json["drop_box_rate"].end(); ++iter) {
				cache.box_rate_vec.push_back((*iter).asDouble());
			}
		}
		//box
		if (config_json.isMember("drop_box")) {
			for (Json::Value::iterator iter = config_json["drop_box"].begin();
					iter != config_json["drop_box"].end(); ++iter) {
				Drop_Box_Config box;
				const Json::Value &box_json = *iter;

				if (box_json.isMember("rate_type")) {
					box.rate_type = box_json["rate_type"].asInt();
				}
				if (box_json.isMember("item")) {
					for (Json::Value::iterator iter = box_json["item"].begin();
							iter != box_json["item"].end(); ++iter) {
						if((*iter).isArray()){
							if((*iter).size() != 3){
								LOG_ABORT("drop_config error(item): %d", cache.id);
							}
							box.item_id_vec1.push_back((*iter)[0u].asInt());
							box.item_id_vec2.push_back((*iter)[1u].asInt());
							box.item_id_vec3.push_back((*iter)[2u].asInt());
						}else{
							int item_id_value = (*iter).asInt();
							box.item_id_vec1.push_back(item_id_value);
							box.item_id_vec2.push_back(item_id_value);
							box.item_id_vec3.push_back(item_id_value);
						}
					}
				}
				if (box_json.isMember("rate")) {
					for (Json::Value::iterator iter = box_json["rate"].begin();
							iter != box_json["rate"].end(); ++iter) {
						box.item_rate_vec.push_back((*iter).asDouble());
					}
				}
				if (box_json.isMember("amount")) {
					for (Json::Value::iterator iter =
							box_json["amount"].begin();
							iter != box_json["amount"].end(); ++iter) {
						box.item_amount_vec.push_back((*iter).asInt());
					}
				}
				if (box_json.isMember("bind")) {
					for (Json::Value::iterator iter = box_json["bind"].begin();
							iter != box_json["bind"].end(); ++iter) {
						box.item_bind_vec.push_back((*iter).asInt());
					}
				}
				cache.box_vec.push_back(box);
			}
		}

		if (config_json.isMember("amount")) {
			cache.cfg_max_gt_times = config_json["amount"].asInt();
			if (cache.cfg_max_gt_times == 0) {
				cache.cfg_max_gt_times = INT_MAX;
			} else if (cache.cfg_max_gt_times < 0) {
				MSG_TRACE_ABORT("drops gather times(%d) less than 0", cache.cfg_max_gt_times);
				cache.cfg_max_gt_times = 1;
			}
		} else {
			cache.cfg_max_gt_times = 1;
		}

		set_map_second_value_by_key(cache.id, drop_config_map_, cache);
	}
	check_drop();
	check_drop_box();
}

const Drop_Config* Config_Cache_NPC::find_drop_config_cache(int id) const {
	Drop_Config_Map::const_iterator it = drop_config_map_.find(id);
	if (it != drop_config_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

void Config_Cache_NPC::refresh_material_config(void) {
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["material"];
	Material_Section_Config* section_config = NULL;
	Material_Point_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("scene_key")) {
			cache.scene_key = config_json["scene_key"].asInt();
		}
		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}

		if (config_json.isMember("material_id")) {
			cache.material_id = config_json["material_id"].asInt();
		}

		if (config_json.isMember("type")) {
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("lv_limit")) {
			cache.lv_limit = config_json["lv_limit"].asInt();
		}
		if (config_json.isMember("gather_time")) {
			cache.gather_time = config_json["gather_time"].asInt();
		}
		if (config_json.isMember("gather_times")) {
			cache.gather_times = config_json["gather_times"].asInt();
		}
		if (config_json.isMember("refresh_time")) {
			cache.refresh_time = config_json["refresh_time"].asInt();
		}
		if (config_json.isMember("refresh_times")) {
			cache.refresh_times = config_json["refresh_times"].asInt();
		}
		//coord
		if (config_json.isMember("coord")) {
			Json_Utility::assign_coord(config_json["coord"], cache.coord);
		}

		if (config_json.isMember("rate")) {
			cache.rate = config_json["rate"].asDouble();
		}
		//drops
		if (config_json.isMember("drops")) {
			for (Json::Value::iterator iter = config_json["drops"].begin();
					iter != config_json["drops"].end(); ++iter) {
				cache.drops_vec.push_back((*iter).asInt());
			}
		}
		//tasks
		if (config_json.isMember("tasks")) {
			for (Json::Value::iterator iter = config_json["tasks"].begin();
					iter != config_json["tasks"].end(); ++iter) {
				cache.task_vec.push_back((*iter).asInt());
			}
		}
		//events
		if (config_json.isMember("events")) {
			for (Json::Value::iterator iter = config_json["events"].begin();
					iter != config_json["events"].end(); ++iter) {
				cache.event_vec.push_back((*iter).asInt());
			}
		}

		section_config = get_material_section_config(cache.scene_key);
		set_map_second_value_by_key(cache.id,
				section_config->material_point_map, cache);
	}
}

void Config_Cache_NPC::refresh_machine_config(void) {
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["machine"];
	Machine_Section_Config* section_config = NULL;
	Machine_Point_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("scene_key")) {
			cache.scene_key = config_json["scene_key"].asInt();
		}
		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}

		if (config_json.isMember("machine_id")) {
			cache.machine_id = config_json["machine_id"].asInt();
		}

		if (config_json.isMember("type")) {
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("view_range")) {
			cache.view_range = config_json["view_range"].asInt();
		}
		if (config_json.isMember("trigger_times")) {
			cache.trigger_times = config_json["trigger_times"].asInt();
		}
		if (config_json.isMember("trigger_cd")) {
			cache.trigger_cd = config_json["trigger_cd"].asInt();
		}
		if (config_json.isMember("gather_time")) {
			cache.gather_time = config_json["gather_time"].asInt();
		}
		if (config_json.isMember("refresh_time")) {
			cache.refresh_time = config_json["refresh_time"].asInt();
		}
		if (config_json.isMember("refresh_times")) {
			cache.refresh_times = config_json["refresh_times"].asInt();
		}
		//coord
		if (config_json.isMember("coord")) {
			Json_Utility::assign_coord(config_json["coord"], cache.coord);
		}
		//events
		if (config_json.isMember("events")) {
			for (Json::Value::iterator iter = config_json["events"].begin();
					iter != config_json["events"].end(); ++iter) {
				cache.event_vec.push_back((*iter).asInt());
			}
		}
		if (config_json.isMember("start_plot")) {
			if (config_json["start_plot"].type() == Json::intValue) {
				std::stringstream ss;
				int tmp_value = config_json["start_plot"].asInt();
				ss << tmp_value;
				cache.start_plot = ss.str();
			} else {
				cache.start_plot = config_json["start_plot"].asString();
			}
		}
		if (config_json.isMember("end_plot")) {
			if (config_json["end_plot"].type() == Json::intValue) {
				std::stringstream ss;
				int tmp_value = config_json["end_plot"].asInt();
				ss << tmp_value;
				cache.end_plot = ss.str();
			} else {
				cache.end_plot = config_json["end_plot"].asString();
			}
		}

		if (config_json.isMember("acc_id")) {
			cache.acc_id = config_json["acc_id"].asInt();
		}
		//coord
		if (config_json.isMember("acc_coord")) {
			Json_Utility::assign_coord(config_json["acc_coord"],
					cache.acc_coord);
		}
		section_config = get_machine_section_config(cache.scene_key);
		set_map_second_value_by_key(cache.id, section_config->point_map, cache);
	}
}

void Config_Cache_NPC::refresh_machine_event_config(void) {
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["machine_event"];
	Machine_Event_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}
		if (config_json.isMember("type")) {
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("val_1")) {
			cache.val_1 = config_json["val_1"].asInt();
		}
		if (config_json.isMember("val_2")) {
			cache.val_2 = config_json["val_2"].asInt();
		}
		if (config_json.isMember("val_3")) {
			cache.val_3 = config_json["val_3"].asInt();
		}
		if (config_json.isMember("val_4")) {
			cache.val_4 = config_json["val_4"].asInt();
		}
		if (config_json.isMember("val_5")) {
			cache.val_4 = config_json["val_5"].asInt();
		}
		if (config_json.isMember("words")) {
			cache.words = config_json["words"].asString();
		}
		if (config_json.isMember("coord_1")) {
			Json_Utility::assign_coord(config_json["coord_1"], cache.coord_1);
		}
		if (config_json.isMember("coord_2")) {
			Json_Utility::assign_coord(config_json["coord_2"], cache.coord_2);
		}

		set_map_second_value_by_key(cache.id, machine_event_map_, cache);
	}
}

Material_Section_Config* Config_Cache_NPC::get_material_section_config(
		int scene_key) {
	Material_Section_Config_Map::iterator it =
			material_section_config_map_.find(scene_key);
	if (it != material_section_config_map_.end()) {
		return &(it->second);
	} else {
		Material_Section_Config section;
		material_section_config_map_.insert(std::make_pair(scene_key, section));
		Material_Section_Config_Map::iterator it =
				material_section_config_map_.find(scene_key);
		if (it != material_section_config_map_.end()) {
			return &(it->second);
		}
	}
	return NULL;
}

Machine_Section_Config* Config_Cache_NPC::get_machine_section_config(
		int scene_key) {
	Machine_Section_Config_Map::iterator it = machine_section_config_map_.find(
			scene_key);
	if (it != machine_section_config_map_.end()) {
		return &(it->second);
	} else {
		Machine_Section_Config section;
		machine_section_config_map_.insert(std::make_pair(scene_key, section));
		Machine_Section_Config_Map::iterator it =
				machine_section_config_map_.find(scene_key);
		if (it != machine_section_config_map_.end()) {
			return &(it->second);
		}
	}
	return NULL;
}

const Machine_Event_Config* Config_Cache_NPC::find_machine_event_config(
		int id) {
	Machine_Event_Config_Map::const_iterator it = machine_event_map_.find(id);
	if (it != machine_event_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

void Config_Cache_NPC::refresh_npc_record(void) {
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["monster_section"];
	Monster_Section_Config* section_config = NULL;
	Monster_Point_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}
		if (config_json.isMember("scene_key")) {
			cache.scene_key = config_json["scene_key"].asInt();
		}
		if (config_json.isMember("point_type")) {
			cache.point_type = config_json["point_type"].asInt();
		}
		if (config_json.isMember("status")) {
			cache.state_type = config_json["status"].asInt();
		}
		if (config_json.isMember("mult_type")) {
			cache.mult_type = config_json["mult_type"].asInt();
		}
		if (config_json.isMember("type")) {
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("refresh_times")) {
			cache.refresh_times = config_json["refresh_times"].asInt();
		}
		if (config_json.isMember("refresh_time")) {
			cache.refresh_time = config_json["refresh_time"].asInt();
		}

		if (config_json.isMember("refresh_con")) {
			cache.refresh_con = config_json["refresh_con"].asInt();
		}
		if (config_json.isMember("con_time")) {
			cache.con_time = config_json["con_time"].asInt();
		}
		if (config_json.isMember("after_time")) {
			cache.after_time = config_json["after_time"].asInt();
		}
		if (config_json.isMember("after_point")) {
			cache.after_point = config_json["after_point"].asInt();
		}

		if (config_json.isMember("birth_coord")) {
			Json_Utility::assign_coord(config_json["birth_coord"],
					cache.birth_coord);
		}
		if (config_json.isMember("view_range")) {
			cache.view_range = config_json["view_range"].asInt();
		}
		if (config_json.isMember("move_range")) {
			cache.move_range = config_json["move_range"].asInt();
		}
		if (config_json.isMember("chase_range")) {
			cache.chase_range = config_json["chase_range"].asInt();
		}
		if (config_json.isMember("speed")) {
			if (config_json["speed"].size() >= 3) {
				cache.speed[0] = config_json["speed"][0u].asInt();
				cache.speed[1] = config_json["speed"][1u].asInt();
				cache.speed[2] = config_json["speed"][2u].asInt();
			}
		}

		if (config_json.isMember("monster")) {
			Json_Utility::assign_int_int_vec(config_json["monster"],
					cache.npc_vec);
		}
		if (config_json.isMember("additional_coord")) {
			for (Json::Value::iterator iter =
					config_json["additional_coord"].begin();
					iter != config_json["additional_coord"].end(); ++iter) {
				Coord coord;
				Json_Utility::assign_coord(*iter, coord);
				cache.add_coord_vec.push_back(coord);
			}
		}
		if (config_json.isMember("start_plot")) {
			if (config_json["start_plot"].type() == Json::intValue) {
				std::stringstream ss;
				int tmp_value = config_json["start_plot"].asInt();
				ss << tmp_value;
				cache.start_plot = ss.str();
			} else {
				cache.start_plot = config_json["start_plot"].asString();
			}
		}
		if (config_json.isMember("start_plot_time")) {
			cache.start_plot_time = config_json["start_plot_time"].asInt();
		}
		if (config_json.isMember("end_plot")) {
			if (config_json["end_plot"].type() == Json::intValue) {
				std::stringstream ss;
				int tmp_value = config_json["end_plot"].asInt();
				ss << tmp_value;
				cache.end_plot = ss.str();
			} else {
				cache.end_plot = config_json["end_plot"].asString();
			}
		}
		if (config_json.isMember("end_plot_time")) {
			cache.end_plot_time = config_json["end_plot_time"].asInt();
		}
		if (config_json.isMember("talk")) {
			std::vector<Int_Int> m_talk_vec;
			for (Json::Value::iterator iter = config_json["talk"].begin();
					iter != config_json["talk"].end(); ++iter) {
				m_talk_vec.clear();
				const Json::Value &m_talk_json = (*iter);
				for (Json::Value::iterator t_iter = m_talk_json.begin();
						t_iter != m_talk_json.end(); ++t_iter) {
					Int_Int a_talk;
					a_talk.val_1 = (*t_iter).asInt();
					++t_iter;
					if (t_iter == m_talk_json.end()) {
						LOG_ABORT(
								"monster_config error(talk):sceen_id:%d, point_id:%d",
								cache.scene_key, cache.id);
					} else {
						a_talk.val_2 = (*t_iter).asInt();
					}
					m_talk_vec.push_back(a_talk);
				}
				cache.talk_vec.push_back(m_talk_vec);
			}
		}
		if (config_json.isMember("chat")) {
			std::vector<Int_Int> m_talk_vec;
			for (Json::Value::iterator iter = config_json["chat"].begin();
					iter != config_json["chat"].end(); ++iter) {
				m_talk_vec.clear();
				const Json::Value &m_talk_json = (*iter);
				for (Json::Value::iterator t_iter = m_talk_json.begin();
						t_iter != m_talk_json.end(); ++t_iter) {
					Int_Int a_talk;
					a_talk.val_1 = (*t_iter).asInt();
					++t_iter;
					if (t_iter == m_talk_json.end()) {
						LOG_ABORT(
								"monster_config error(chat):sceen_id:%d, point_id:%d",
								cache.scene_key, cache.id);
					} else {
						a_talk.val_2 = (*t_iter).asInt();
					}
					m_talk_vec.push_back(a_talk);
				}
				cache.chat_vec.push_back(m_talk_vec);
			}
		}
		if (config_json.isMember("group")) {
			Json_Utility::assign_int_int_vec(config_json["group"],
					cache.group_vec, 1000);
		}
		if (config_json.isMember("walk_path")) {
			for (Json::Value::iterator iter = config_json["walk_path"].begin();
					iter != config_json["walk_path"].end(); ++iter) {
				Coord walk_coord;
				Json_Utility::assign_coord((*iter), walk_coord);
				cache.walk_path.push_back(walk_coord.grid_coord());
			}
		}
		if (config_json.isMember("ia")) {
			cache.ia = config_json["ia"].asInt();
		}
		if (config_json.isMember("rate")) {
			cache.rate = config_json["rate"].asInt();
		}
		if (config_json.isMember("gather_time")) {
			cache.gather_time = config_json["gather_time"].asInt();
		}
		if (config_json.isMember("gt_cd")) {
			cache.gt_cd = config_json["gt_cd"].asInt();
		}
		if (config_json.isMember("gt_times")) {
			cache.gt_times = config_json["gt_times"].asInt();
		}
		if (config_json.isMember("tasks")) {
			Json_Utility::assign_int_vec(config_json["tasks"], cache.tasks_vec);
		}
		if (config_json.isMember("events")) {
			Json_Utility::assign_int_vec(config_json["events"],
					cache.events_vec);
		}
		if (config_json.isMember("acc_id")) {
			cache.acc_id = config_json["acc_id"].asInt();
		}
		if (config_json.isMember("acc_coord")) {
			Json_Utility::assign_coord(config_json["acc_coord"],
					cache.acc_coord);
		}

		if (config_json.isMember("drop_type")) {
			cache.drop_type = config_json["drop_type"].asInt();
		}
		if (config_json.isMember("drop_mult_mode")) {
			cache.drop_mult_mode = config_json["drop_mult_mode"].asInt();
		}
		if (config_json.isMember("drop_mult_coe")) {
			cache.drop_mult_coe = config_json["drop_mult_coe"].asDouble();
		}
		if (config_json.isMember("drop_lv_limit")) {
			cache.drop_lv_limit = config_json["drop_lv_limit"].asInt();
		}
		if (config_json.isMember("drop_lv_coe")) {
			cache.drop_lv_coe = config_json["drop_lv_coe"].asInt();
		}
		if (config_json.isMember("drops")) {
			Json_Utility::assign_int_vec(config_json["drops"], cache.drops);
		}
		if (config_json.isMember("drops_coord")) {
			int array_size = config_json["drops_coord"].size();
			for (int i = 0; i < array_size; ++i) {
				Coord d_coord;
				Json_Utility::assign_coord(config_json["drops_coord"][i],
						d_coord);
				cache.drops_coord.push_back(d_coord);
			}
		}
		if (config_json.isMember("fightMap_key")) {
			cache.fightMap_key = config_json["fightMap_key"].asInt();
		}
		if (config_json.isMember("monster_chance")) {
			cache.monster_chance = config_json["monster_chance"].asInt();
		}
				section_config = get_monster_section_config(cache.scene_key);
		set_map_second_value_by_key(cache.id, section_config->point_map, cache);
	}
}

void Config_Cache_NPC::refresh_npc_group(void) {
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["monster_group"];
	Monster_Section_Config* section_config;
	Monster_Group_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();
		if (config_json.isMember("scene_key")) {
			cache.scene_key = config_json["scene_key"].asInt();
		}
		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}
		if (config_json.isMember("monster")) {
			Json_Utility::assign_int_vec(config_json["monster"], cache.monster);
		}
		if (config_json.isMember("wave")) {
			Json_Utility::assign_int_vec(config_json["wave"], cache.wave);
		}
		if (config_json.isMember("pos")) {
			Json_Utility::assign_int_vec(config_json["pos"], cache.pos);
		}
		if(config_json.isMember("chest_ID")){
			Json_Utility::assign_int_int_vec(config_json["chest_ID"], cache.chest_id);
		}
		section_config = get_monster_section_config(cache.scene_key);
		set_map_second_value_by_key(cache.id, section_config->group_map, cache);
	}
}

void Config_Cache_NPC::refresh_npc_path_guide(void) {
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["scene_path_guide"];
	Monster_Section_Config* section_config;
	std::vector<int> path_guide_vec;
	int scene_key;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		path_guide_vec.clear();
		scene_key = 0;
		if (config_json.isMember("scene_key")) {
			scene_key = config_json["scene_key"].asInt();
		}
		if (config_json.isMember("path_guide")) {
			Json_Utility::assign_int_vec(config_json["path_guide"],
					path_guide_vec);
		}
		section_config = get_monster_section_config(scene_key);
		section_config->path_guide_vec = path_guide_vec;
	}
}

void Config_Cache_NPC::refresh_config_cache(void) {
	PERF_MON("Config_Cache_NPC");

	refresh_npc_config();
	refresh_npc_record();
	refresh_npc_group();
	refresh_npc_path_guide();
	refresh_drop_config();
	refresh_machine_event_config();
	refresh_monster_ai_config();
	refresh_ai_config();
	refresh_outpost_trigger();
	refresh_vagrant_item();
	refresh_tbay_gold_npc();
	refresh_monster_chest_config();
	refresh_chest_lv_config();

}

void Config_Cache_NPC::refresh_npc_config(void) {
	Int_Set monster_set;
	monster_set.clear();
	std::stringstream repeat_ids;
	bool is_repeat_id = false;
	const Json::Value& config_list =
	CONFIG_INSTANCE->config_json()["npc"]["monster"];
	NPC_Config_Cache cache;
	for (Json::Value::const_iterator it = config_list.begin();
			it != config_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}

		if (config_json.isMember("name")) {
			cache.name = config_json["name"].asString();
		}
		if (config_json.isMember("level")) {
			cache.level = config_json["level"].asInt();
		}
		if (config_json.isMember("title_id")) {
					cache.title_id = config_json["title_id"].asInt();
		}
		if (config_json.isMember("jid")) {
			cache.jid = config_json["jid"].asInt();
		}
		if (cache.jid == 0) {
			LOG_ABORT("monster_config error (jid), monster_id: %d", cache.id);
		}
		if (config_json.isMember("type")) {
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("m_type")) {
			cache.m_type = config_json["m_type"].asInt();
		}
		if (config_json.isMember("race")) {
			if (config_json["race"].isArray()) {
				Json_Utility::assign_int_vec(config_json["race"], cache.race);
			}
		}

		//prop
		for (size_t i = 0; i < ARRAY_SIZE(NPC_PROPS); ++i) {
			Int_Double prop_value;
			prop_value.id = NPC_PROPS[i];

			std::ostringstream prop_stream;
			prop_stream << prop_value.id;

			if (config_json[prop_stream.str()] != Json::Value::null) {
				prop_value.value = config_json[prop_stream.str()].asDouble();
				cache.prop_value_vec.push_back(prop_value);
			}
		}
		//passive_skills
		if (config_json.isMember("passive_skills")) {
			for (Json::Value::iterator iter = config_json["passive_skills"].begin();
					iter != config_json["passive_skills"].end(); ++iter) {
				cache.passive_skills.push_back((*iter).asInt());
			}
		}
		//skills
		if (config_json.isMember("skills")) {
			for (Json::Value::iterator iter = config_json["skills"].begin();
					iter != config_json["skills"].end(); ++iter) {
				cache.skill_vec.push_back((*iter).asInt());
			}
		}
		//check skill
		if (cache.type / 100 == 3 && cache.skill_vec.empty()) {
			LOG_ABORT("monster_config error (has not skill):%d", cache.id);
		}
		//battle_ai
		if (config_json.isMember("battle_ai")) {
			for (Json::Value::iterator iter = config_json["battle_ai"].begin();
					iter != config_json["battle_ai"].end(); ++iter) {
				cache.battle_ai_vec.push_back((*iter).asInt());
			}
		}
		if (config_json.isMember("drop_type")) {
			cache.drop_type = config_json["drop_type"].asInt();
		}
		if (config_json.isMember("drop_mult_mode")) {
			cache.drop_mult_mode = config_json["drop_mult_mode"].asInt();
		}
		if (config_json.isMember("drop_mult_coe")) {
			cache.drop_mult_coe = config_json["drop_mult_coe"].asDouble();
		}
		if (config_json.isMember("drop_lv_limit")) {
			cache.drop_lv_limit = config_json["drop_lv_limit"].asInt();
		}
		if (config_json.isMember("drop_lv_coe")) {
			cache.drop_lv_coe = config_json["drop_lv_coe"].asDouble();
		}

		//drops
		if (config_json.isMember("drops")) {
			Json_Utility::assign_int_vec(config_json["drops"], cache.drops_vec);
		}
		if (config_json.isMember("money")) {
			cache.money = config_json["money"].asInt();
		}
		if (config_json.isMember("exp")) {
			cache.exp = config_json["exp"].asInt();
		}
		if (config_json.isMember("soul")) {
			cache.soul = config_json["soul"].asInt();
		}
		if (config_json.isMember("res_mult_coe")) {
			cache.res_mult_coe = config_json["res_mult_coe"].asDouble();
		}
		if (config_json.isMember("res_lv_limit")) {
			cache.res_lv_limit = config_json["res_lv_limit"].asInt();
		}
		if (config_json.isMember("res_lv_coe")) {
			cache.res_lv_coe = config_json["res_lv_coe"].asDouble();
		}
		if (config_json.isMember("chat")) {
			Json_Utility::assign_int_vec(config_json["chat"], cache.chat_vec);
		}
		// check id
		if(monster_set.count(cache.id) > 0){
			repeat_ids << cache.id;
			repeat_ids << " ";
			is_repeat_id = true;
		}
		monster_set.insert(cache.id);
		set_map_second_value_by_key(cache.id, npc_config_map_, cache);
	}
	if(is_repeat_id){
		std::string repeat_error;
		repeat_error = repeat_ids.str();
		LOG_ABORT("monster_config error, monster_id repeat: %s", repeat_error.c_str());
	}
}

void Config_Cache_NPC::refresh_ai_config(void) {
	const Json::Value& ai_info_list =
	CONFIG_INSTANCE->config_json()["ai"]["ai_info"];
	AI_Info cache;
	for (Json::Value::const_iterator it = ai_info_list.begin();
			it != ai_info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();
		cache.id = atoi(it.key().asCString());

		if (config_json.isMember("life_time")) {
			double life_time = config_json["life_time"].asDouble();
			long int sec = int(life_time);
			long int usec = (life_time - sec) * 1000000;
			cache.life_time.set(sec, usec);
		}

		if (config_json.isMember("tick")) {
			cache.tick_time.set(config_json["tick"].asInt() / 1000,
					config_json["tick"].asInt() % 1000 * 1000);
		}
		if (cache.tick_time <= Time_Value::zero) {
			cache.tick_time.set(3, 0);
			LOG_USER("error tick time ai:%d");
		}

		if (config_json.isMember("vision_type"))
			cache.vision_type = config_json["vision_type"].asInt();

		if (config_json.isMember("vision_area"))
			cache.vision_area = config_json["vision_area"].asInt();

		if (config_json.isMember("vision_always"))
			cache.vision_always = config_json["vision_always"].asInt();

		if (config_json.isMember("vision_tick"))
			cache.vision_tick.set(config_json["vision_tick"].asInt() / 1000,
					config_json["vision_tick"].asInt() % 1000 * 1000);

		if (config_json.isMember("patrol_tick"))
			cache.patrol_tick.set(config_json["patrol_tick"].asInt() / 1000,
					config_json["patrol_tick"].asInt() % 1000 * 1000);

		if (config_json.isMember("distance_of_patrol"))
			cache.distance_of_patrol =
					config_json["distance_of_patrol"].asInt();

		if (config_json.isMember("fight_timeout"))
			cache.fight_timeout.set(config_json["fight_timeout"].asInt() / 1000,
					config_json["fight_timeout"].asInt() % 1000 * 1000);

		if (config_json.isMember("distance_of_fight"))
			cache.distance_of_fight = config_json["distance_of_fight"].asInt();

		if (config_json.isMember("exit_combat_buf"))
			cache.exit_combat_buf = config_json["exit_combat_buf"].asInt();

		if (config_json.isMember("walk_path")) {
			for (Json::Value::iterator iter = config_json["walk_path"].begin();
					iter != config_json["walk_path"].end(); ++iter) {
				Coord coord((*iter)[0u].asInt(), (*iter)[1u].asInt());
				cache.walk_path.push_back(coord);
			}
		}

		if (config_json.isMember("die_share")) {
			cache.die_share =
					config_json["die_share"].asInt() > 0 ? true : false;
		}

		if (config_json.isMember("ignore_scene")) {
			cache.ignore_scene =
					config_json["ignore_scene"].asInt() > 0 ? true : false;
		}

		if (config_json.isMember("date_appear")) {
			cache.date_appear.no_set = false;

			if (config_json["date_appear"]["date_loop"] != Json::Value::null
					&& config_json["date_appear"]["date_loop"].asInt() >= 0) {
				cache.date_appear.date_loop =
						config_json["date_appear"]["date_loop"].asInt();
			}

			if (config_json["date_appear"]["date_week"] != Json::Value::null) {
				for (Json::Value::iterator iter =
						config_json["date_appear"]["date_week"].begin();
						iter != config_json["date_appear"]["date_week"].end();
						++iter) {
					cache.date_appear.date_week.push_back((*iter).asInt());
				}
			}

			if (config_json["date_appear"]["date_time"] != Json::Value::null) {
				AI_Time_Data::Hour_Min hour_min;
				for (Json::Value::iterator iter =
						config_json["date_appear"]["date_time"].begin();
						iter != config_json["date_appear"]["date_time"].end();
						++iter) {
					hour_min.hour = (*iter)[0u].asInt();
					hour_min.min = (*iter)[1u].asInt();
					cache.date_appear.date_time.push_back(hour_min);
				}
			}
		}
		set_map_second_value_by_key(cache.id, ai_info_map_, cache);
	}
}

void Config_Cache_NPC::refresh_outpost_trigger(void){
	std::set<Outpost_Trigger_Key> key_set;
	key_set.clear();
	std::stringstream repeat_ids;
	bool is_repeat_id = false;

	const Json::Value& info_list = CONFIG_INSTANCE->config_json()["npc"]["organ"];
	Outpost_Trigger_Config cache;
	for(Json::Value::const_iterator it = info_list.begin(); it != info_list.end(); ++it){
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("sceneId")) {
			cache.scene_id = config_json["sceneId"].asInt();
		}
		if (config_json.isMember("monster_id")) {
			cache.monster_id = config_json["monster_id"].asInt();
		}
		if (config_json.isMember("chance")) {
			cache.chance = config_json["chance"].asInt();
		}
		if (config_json.isMember("type")) {
			cache.type = config_json["type"].asInt();
		}
		if (config_json.isMember("scene_id")) {
			cache.target_scene_id = config_json["scene_id"].asInt();
		}
		if (config_json.isMember("scene_item")) {
			cache.target_scene_time = config_json["scene_item"].asInt();
		}
		if (config_json.isMember("scene_type")) {
			cache.target_scene_type = config_json["scene_type"].asInt();
		}
		if (config_json.isMember("id")) {
			Json_Utility::assign_int_vec(config_json["id"], cache.points);
		}
		Outpost_Trigger_Key key;
		key.val_1 = cache.scene_id;
		key.val_2 = cache.type;
		// check id
		{
			if(key_set.count(key) > 0){
				repeat_ids << cache.scene_id;
				repeat_ids << " ";
				is_repeat_id = true;
			}
			key_set.insert(key);
		}
		set_map_second_value_by_key(key, outpost_trigger_map_, cache);
	}

	if(is_repeat_id){
		std::string repeat_error;
		repeat_error = repeat_ids.str();
		LOG_ABORT("ogran_config error, scene_id repeat: %s", repeat_error.c_str());
	}
}

void Config_Cache_NPC::refresh_vagrant_item(void){
	Int_Set key_set;
	key_set.clear();
	std::stringstream repeat_ids;
	bool is_repeat_id = false;

	const Json::Value& info_list = CONFIG_INSTANCE->config_json()["npc"]["businessman"];
	Vagrant_Item_Config cache;
	for(Json::Value::const_iterator it = info_list.begin(); it != info_list.end(); ++it){
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("key")) {
			cache.key = config_json["key"].asInt();
		}
		if (config_json.isMember("id")) {
			cache.item_id = config_json["id"].asInt();
		}
		if (config_json.isMember("sys_sell_type")) {
			cache.sys_sell_type = config_json["sys_sell_type"].asInt();
		}
		if (config_json.isMember("limit_num")) {
			cache.limit_num = config_json["limit_num"].asInt();
		}
		if (config_json.isMember("num")) {
			cache.num = config_json["num"].asInt();
		}
		if (config_json.isMember("cost")) {
			cache.cost = config_json["cost"].asInt();
		}
		if (config_json.isMember("monster_id")) {
			Json_Utility::assign_int_vec(config_json["monster_id"], cache.monsters);
		}
		set_map_second_value_by_key(cache.key, vagrant_item_map_, cache);
		// check id
		{
			if(key_set.count(cache.key) > 0){
				repeat_ids << cache.key;
				repeat_ids << " ";
				is_repeat_id = true;
			}
			key_set.insert(cache.key);
		}
	}

	if(is_repeat_id){
		std::string repeat_error;
		repeat_error = repeat_ids.str();
		LOG_ABORT("businessman_config error, key repeat: %s", repeat_error.c_str());
	}
}

void Config_Cache_NPC::refresh_tbay_gold_npc(void){

	int bless_index = 0;
	const Json::Value& info_list = CONFIG_INSTANCE->config_json()["npc"]["gold_npc"];
	Tbay_Gold_Npc_Config cache;
	for(Json::Value::const_iterator it = info_list.begin(); it != info_list.end(); ++it){
		cache.reset();
		const Json::Value &config_json = (*it);
		bless_index += 1;
		if (config_json.isMember("cost")) {
			cache.cost = config_json["cost"].asInt();
		}
		if (config_json.isMember("sys_sell_type")) {
			cache.sys_sell_type = config_json["sys_sell_type"].asInt();
		}
		if(config_json.isMember("name")){
			cache.name = config_json["name"].asString();
		}

		if(config_json.isMember("buff_id")){
			int count = 0;
			if(config_json["buff_id"].size() < 2){
				LOG_ABORT("gold npc error(buff_id)");
			}
			for (Json::Value::iterator iter = config_json["buff_id"].begin();
					iter != config_json["buff_id"].end(); ++iter) {
				if(count == 0){// id
					cache.buff_argv.status_id = (*iter).asInt();
					cache.buff_argv.status_from = TYPE_SOURCE_MECHINE;
				}else if(count == 1 ){// time[1000]
					if((*iter).isArray()){
						Json_Utility::assign_int_vec((*iter), cache.buff_argv.disappear_gap);
					}
				}else{// attr[530321,20], []
					if(!(*iter).isNull() && (*iter).isArray()){
						Skill_Effect_Val_Inner inner;
						int id = (*iter)[0u].asInt();
						double value = (*iter)[1u].asDouble();
						CONFIG_CACHE_FIGHTER->get_attr_change_data(id, inner, value);
						if (0 == inner.val1) { //伤害链\经验加成
							inner.val1 = id;
							inner.val4 = value;
						}
						if (3 == (*iter).size()) {
							double value2 = (*iter)[2u].asDouble();
							inner.val7 = value2;//目前使用作治疗的下限值
						}

						Effect_Argv_Prop prop;
						prop.i_effect_val1 = inner.val1;
						prop.d_effect_val1 = inner.val4;
						prop.d_effect_val2 = inner.val5;
						prop.d_effect_val3 = inner.val6;
						prop.d_effect_val4 = inner.val7;
						cache.buff_argv.data.push_back(prop);
					}
				}
				++count;
			}
		}

		set_map_second_value_by_key(bless_index, tbay_gold_npc_map_, cache);
	}

}

void Config_Cache_NPC::refresh_monster_chest_config(void){
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["monster_chest"];
	Monster_Chest_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("chestID")) {
			cache.id = config_json["chestID"].asInt();
		}
		if (config_json.isMember("chest_name")) {
			cache.chest_name = config_json["chest_name"].asString();
		}
		if (config_json.isMember("chest_priority")) {
			cache.chest_priority = config_json["chest_priority"].asInt();
		}
		if (config_json.isMember("dropid")) {
			Json_Utility::assign_int_vec(config_json["dropid"], cache.dropid);
		}
		if (config_json.isMember("drop_type")) {
			cache.drop_type = config_json["drop_type"].asInt();
		}
		if (config_json.isMember("soul_max")) {
			cache.soul_max = config_json["soul_max"].asInt();
		}
		if (config_json.isMember("jta")) {
			cache.jta = config_json["jta"].asInt();
		}
		if (config_json.isMember("act_time")) {
			if(config_json["act_time"].size() == 2){
				int time_begin = config_json["act_time"][0u].asInt();
				int time_last = config_json["act_time"][0u].asInt();
				int month = time_begin/100;
				int day = time_begin%100;
				Time_Value now(Time_Value::gettimeofday());
				Date_Time date_now(now);
				Date_Time date_des(date_now);
				date_des.month(month);
				date_des.day(day);
				date_des.hour(0);
				date_des.minute(0);
				date_des.second(0);
				cache.begin_time = date_des.time_sec();
				cache.finish_time = cache.begin_time + time_last*Time_Value::ONE_HOUR_IN_SECS;
			}
		}
		set_map_second_value_by_key(cache.id, monster_chest_map_, cache);
	}
}

void Config_Cache_NPC::refresh_chest_lv_config(void){
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["lvl_correction"];
	Chest_Lv_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		cache.reset();
		const Json::Value &config_json = (*it);

		if (config_json.isMember("lvl")) {
			cache.lvl = config_json["lvl"].asInt();
		}
		if (config_json.isMember("base_correction")) {
			cache.base_correction = config_json["base_correction"].asInt();
		}
		if (config_json.isMember("item")) {
			cache.item = config_json["item"].asInt();
		}
		if (config_json.isMember("monster_id")) {
			cache.monster_id = config_json["monster_id"].asInt();
		}
		set_map_second_value_by_key(cache.lvl, chest_lv_map_, cache);
	}
}

const AI_Info * Config_Cache_NPC::find_ai_config_cache(const int id) const {
	AI_Info_Map::const_iterator it = ai_info_map_.find(id);
	if (it != ai_info_map_.end()) {
		return &(it->second);
	}
	return NULL;
}

Vagrant_Item_Config_Map* Config_Cache_NPC::get_vagrant_item_config_map(void){
	return &vagrant_item_map_;
}

int Config_Cache_NPC::vagrant_item_trigger(const int scene_id, const int point){
	Outpost_Trigger_Key key;
	key.val_1 = scene_id;
	key.val_2 = 0;
	Outpost_Trigger_Config_Map::iterator it = outpost_trigger_map_.find(key);
	if(it != outpost_trigger_map_.end()){
		for(Int_Vec::iterator s_it = it->second.points.begin(); s_it != it->second.points.end(); ++s_it){
			if(*s_it == point){
				int rd_value = random()%1000;
				if(it->second.chance > rd_value){
					return it->second.monster_id;
				}else{
					return 0;
				}

			}
		}
	}
	return 0;
}

int Config_Cache_NPC::tbay_trigger(const int scene_id, const int point, int& target_scene_id, int& target_scene_time, int& target_scene_type){
	Outpost_Trigger_Key key;
	key.val_1 = scene_id;
	key.val_2 = 1;
	Outpost_Trigger_Config_Map::iterator it = outpost_trigger_map_.find(key);
	if(it != outpost_trigger_map_.end()){
		for(Int_Vec::iterator s_it = it->second.points.begin(); s_it != it->second.points.end(); ++s_it){
			if(*s_it == point){
				int rd_value = random()%1000;
				if(it->second.chance > rd_value){
					target_scene_id = it->second.target_scene_id;
					target_scene_time = it->second.target_scene_time;
					target_scene_type = it->second.target_scene_type;
					return it->second.monster_id;
				}else{
					return 0;
				}

			}
		}
	}
	return 0;
}

Tbay_Gold_Npc_Config* Config_Cache_NPC::get_tbay_gold_npc_config(const int8_t bless_index){
	Tbay_Gold_Npc_Config_Map::iterator it = tbay_gold_npc_map_.find(bless_index);
	if(it != tbay_gold_npc_map_.end()){
		return &(it->second);
	}
	return NULL;
}

const Monster_AI_Config* Config_Cache_NPC::find_monster_ai_config(
		const int id) const {
	return get_map_second_pointer_by_key(id, monster_ai_config_map_);
}

const Monster_Chest_Config* Config_Cache_NPC::find_monster_chest_config(const int id){
	return get_map_second_pointer_by_key(id, monster_chest_map_);
}

const Chest_Lv_Config* Config_Cache_NPC::find_chest_lv_config(const int lv){
	return get_map_second_pointer_by_key(lv, chest_lv_map_);
}

int Config_Cache_NPC::get_monster_chest_id(std::vector<Int_Int> int_int_vec){
	std::vector<int> result_vec;
	std::vector<Int_Int>::iterator it;
	for(it = int_int_vec.begin(); it != int_int_vec.end(); ++it){
		if(it->val_2 > random()%1000 ){
			result_vec.push_back(it->val_1);
		}
	}
	int now_sec = Time_Value::gettimeofday().sec();
	int chest_id = 0;
	int chest_pri = 0;
	int count = result_vec.size();
	for(int i = 0; i < count; ++i){
		const Monster_Chest_Config* cfg = find_monster_chest_config(result_vec[i]);
		if(!cfg){
			continue;
		}
		if(cfg->is_valid_time(now_sec) == false){
			continue;
		}
		if(cfg->chest_priority >= chest_pri){
			chest_id = cfg->id;
			chest_pri = cfg->chest_priority;
		}
	}
	return chest_id;
}

int Config_Cache_NPC::get_monster_chest_pos(std::vector<int8_t> int_vec){
	std::vector<int> temp_vec;
	for(int i = 0; i <= 9; ++i){
		temp_vec.push_back(i);
	}
	int count = int_vec.size();
	for(int i = 0; i < count; ++i){
		if(int_vec[i] < 1 || int_vec[i] > 9){
			continue;
		}
		temp_vec[int_vec[i]] = 0;
	}
	std::vector<int> result_vec;
	int tmp_count = temp_vec.size();
	for(int i = 0; i < tmp_count; ++i){
		if(temp_vec[i] == 0){
			continue;
		}
		result_vec.push_back(temp_vec[i]);
	}
	int result_count = result_vec.size();
	if(result_count > 0){
		int index = random()%result_count;
		return result_vec[index];
	}
	return 0;
}

void Config_Cache_NPC::refresh_monster_ai_config(void) {
	const Json::Value& info_list =
	CONFIG_INSTANCE->config_json()["npc"]["battle_ai"];
	Monster_AI_Config cache;
	for (Json::Value::const_iterator it = info_list.begin();
			it != info_list.end(); ++it) {
		const Json::Value &config_json = (*it);
		cache.reset();

		if (config_json.isMember("id")) {
			cache.id = config_json["id"].asInt();
		}
		if (config_json.isMember("pri")) {
			cache.pri = config_json["pri"].asInt();
		}
		if (config_json.isMember("times")) {
			cache.times = config_json["times"].asInt();
		}
		if (config_json.isMember("cd")) {
			cache.cd = config_json["cd"].asInt();
		}
		if (config_json.isMember("rate")) {
			cache.rate = config_json["rate"].asInt();
		}
		if (config_json.isMember("stop_sing")) {
			cache.stop_sing = config_json["stop_sing"].asInt();
		}
		if (config_json.isMember("c_type")) {
			cache.c_type = config_json["c_type"].asInt();
		}
		if (config_json.isMember("battle_over")) {
			cache.battle_over = config_json["battle_over"].asInt();
		}
		if(config_json.isMember("foreshow")){
			cache.foreshow = config_json["foreshow"].asInt();
		}
		if (config_json.isMember("c_val")) {

			switch (cache.c_type) {
			case MONSTER_AI_C_NULL: {
				break;
			}
			case MONSTER_AI_C_HP:
			case MONSTER_AI_C_MP: {
				if (config_json["c_val"].size() == 3) {
					cache.c_val1 = config_json["c_val"][0u].asInt();
					cache.c_val2 = config_json["c_val"][1u].asInt();
					cache.c_val3 = config_json["c_val"][2u].asInt();
				}else{
					LOG_ABORT("monster_ai error(c_val):[%d]", cache.id);
				}
				break;
			}
			case MONSTER_AI_C_DIE:
			case MONSTER_AI_C_SKILL:
			case MONSTER_AI_C_PLOT:
			case MONSTER_AI_C_TALK:
			case MONSTER_AI_C_AI:{
				cache.c_val1 = config_json["c_val"].asInt();
				break;
			}
			case MONSTER_AI_C_TIMES:
			case MONSTER_AI_C_TIME: {
				if (config_json["c_val"].size() == 2) {
					cache.c_val1 = config_json["c_val"][0u].asInt();
					cache.c_val2 = config_json["c_val"][1u].asInt();
				}else{
					LOG_ABORT("monster_ai error(c_val):[%d]", cache.id);
				}
				break;
			}
			default:{
				break;
			}
			}
		}

		if (config_json.isMember("a_type")) {
			cache.a_type = config_json["a_type"].asInt();
		}
		if (config_json.isMember("a_val")) {
			switch (cache.a_type) {
			case MONSTER_AI_A_LOOP_SKILLS: {
				//[1,2]
				for (Json::Value::iterator iter = config_json["a_val"].begin();
						iter != config_json["a_val"].end(); ++iter) {
					cache.skill_vec.push_back((*iter).asInt());
					cache.skill_rate_vec.push_back(1000);
				}
				break;
			}
			case MONSTER_AI_A_RAN_SKILLS: {
				//[[100,1000],[100,1000]]
				for (Json::Value::iterator iter = config_json["a_val"].begin();
						iter != config_json["a_val"].end(); ++iter) {
					if ((*iter).size() == 2) {
						cache.skill_vec.push_back((*iter)[0u].asInt());
						cache.skill_rate_vec.push_back((*iter)[1u].asInt());
					}else{
						LOG_ABORT("monster_ai error(a_val):[%d]", cache.id);
					}
				}
				break;
			}
			case MONSTER_AI_A_ONE_SKILL: {
				cache.a_val1 = config_json["a_val"].asInt();
				cache.skill_vec.push_back(cache.a_val1);
				break;
			}
			case MONSTER_AI_A_ONE_SKILL_BB: {
				cache.a_val1 = config_json["a_val"].asInt();
				cache.a_val2 = 1;
				cache.skill_vec.push_back(cache.a_val1);
				break;
			}
			case MONSTER_AI_A_ONE_SKILL_DL: {
				if (config_json["a_val"].size() == 2) {
					cache.a_val1 = config_json["a_val"][0u].asInt();
					cache.a_val2 = 1;
					cache.a_val3 = config_json["a_val"][1u].asInt();
					cache.skill_vec.push_back(cache.a_val1);
				}else{
					LOG_ABORT("monster_ai error(a_val):[%d]", cache.id);
				}
				break;
			}
			case MONSTER_AI_A_CLEAN_MP:
			case MONSTER_AI_A_CLEAN_BUF:
			case MONSTER_AI_A_REFRESH_HERO: {
				cache.a_val1 = config_json["a_val"].asInt();
				break;
			}
			case MONSTER_AI_A_PLOT:
			case MONSTER_AI_A_TALK:
			case MONSTER_AI_A_TRANSFORM:{
				if (config_json["a_val"].size() == 2) {
					cache.a_val1 = config_json["a_val"][0u].asInt();
					cache.a_val2 = config_json["a_val"][1u].asInt();
				}else{
					LOG_ABORT("monster_ai error(a_val):[%d]", cache.id);
				}
				break;
			}
			case MONSTER_AI_A_ADD_BUF:{
				int count = 0;
				if(config_json["a_val"].size() < 2){
					LOG_ABORT("monster_ai error(a_val):[%d]", cache.id);
				}
				for (Json::Value::iterator iter = config_json["a_val"].begin();
						iter != config_json["a_val"].end(); ++iter) {
					if(count == 0){// id
						cache.buf_argv.status_id = (*iter).asInt();
						cache.buf_argv.status_from = TYPE_SOURCE_MECHINE;
					}else if(count == 1 ){// time[1000]
						if((*iter).isArray()){
							Json_Utility::assign_int_vec((*iter), cache.buf_argv.disappear_gap);
						}
					}else{// attr[530321,20], []
						if(!(*iter).isNull() && (*iter).isArray()){
							Skill_Effect_Val_Inner inner;
							int id = (*iter)[0u].asInt();
							double value = (*iter)[1u].asDouble();
							CONFIG_CACHE_FIGHTER->get_attr_change_data(id, inner, value);
							if (0 == inner.val1) { //伤害链\经验加成
								inner.val1 = id;
								inner.val4 = value;
							}
							if (3 == (*iter).size()) {
								double value2 = (*iter)[2u].asDouble();
								inner.val7 = value2;//目前使用作治疗的下限值
							}

							Effect_Argv_Prop prop;
							prop.i_effect_val1 = inner.val1;
							prop.d_effect_val1 = inner.val4;
							prop.d_effect_val2 = inner.val5;
							prop.d_effect_val3 = inner.val6;
							prop.d_effect_val4 = inner.val7;
							cache.buf_argv.data.push_back(prop);
						}
					}
					++count;
				}
				break;
			}
			default:{
				break;
			}
			}
		}
		set_map_second_value_by_key(cache.id, monster_ai_config_map_, cache);
	}
}

void Config_Cache_NPC::check_monster_position(void) {
	std::vector<Int_Int> invalid_vec;
	Monster_Section_Config_Map::const_iterator it;
	for (it = monster_section_config_map_.begin();
			it != monster_section_config_map_.end(); ++it) {
		Monster_Group_Config_Map::const_iterator s_it;
		for (s_it = it->second.group_map.begin();
				s_it != it->second.group_map.end(); ++s_it) {
			bool valid = check_monster_position_valid(s_it->second.monster,
					s_it->second.wave, s_it->second.pos);
			if (!valid) {
				Int_Int invalid_value;
				invalid_value.val_1 = it->first;
				invalid_value.val_2 = s_it->first;
				invalid_vec.push_back(invalid_value);
			}
		}
	}
	if(invalid_vec.empty() == false){
		std::stringstream ss;
		int count = invalid_vec.size();
		for(int i = 0; i < count; ++i){
			ss<<"["<<invalid_vec[i].val_1<<","<<invalid_vec[i].val_2<<"] ";
		}
		std::string err_out = ss.str();
		LOG_ABORT("monster_config error(group):%s", err_out.c_str());
	}
}

bool Config_Cache_NPC::check_monster_position_valid(
		const std::vector<int>& monster_vec, const std::vector<int>& wave_vec,
		const std::vector<int>& pos_vec) {
	int m_count = monster_vec.size();
	int w_count = wave_vec.size();
	int p_count = pos_vec.size();
	if (m_count != w_count || w_count != p_count) {
		return false;
	}
	boost::unordered_set<int> pos_set;
	int wave_index = 0;
	for (int i = 0; i < w_count; ++i) {
		if (pos_vec[i] < 1 || pos_vec[i] > 12) {
			return false;
		}
		if (wave_index != wave_vec[i]) {
			wave_index = wave_vec[i];
			pos_set.clear();
		}
		if (pos_set.count(pos_vec[i]) > 0) {
			return false;
		}
		pos_set.insert(pos_vec[i]);
	}
	return true;
}

void Config_Cache_NPC::check_monster_coord(void){
	std::vector<Int_Int> birth_invalid;
	std::vector<Int_Int> walk_invalid;
	Monster_Section_Config_Map::iterator it;
	for (it = monster_section_config_map_.begin();
			it != monster_section_config_map_.end(); ++it) {
		Monster_Point_Config_Map::iterator s_it;
		for (s_it = it->second.point_map.begin();
				s_it != it->second.point_map.end(); ++s_it) {
			if(s_it->second.point_type != 0){
				continue;
			}
			int map_id = CONFIG_CACHE_SCENE->get_map_id_from_scene_id(s_it->second.scene_key);
			bool birth_valid = check_monster_birth_coord(
					s_it->second.birth_coord,s_it->second.add_coord_vec, map_id);
			if (!birth_valid) {
				Int_Int invalid_value;
				invalid_value.val_1 = it->first;
				invalid_value.val_2 = s_it->first;
				birth_invalid.push_back(invalid_value);
			}

			bool walk_valid = check_monster_walk_coord(s_it->second.walk_path, map_id);
			if (!walk_valid) {
				Int_Int invalid_value;
				invalid_value.val_1 = it->first;
				invalid_value.val_2 = s_it->first;
				walk_invalid.push_back(invalid_value);
			}
		}
	}
	if(birth_invalid.empty() == false){
		std::stringstream ss;
		int count = birth_invalid.size();
		for(int i = 0; i < count; ++i){
			ss<<"["<<birth_invalid[i].val_1<<","<<birth_invalid[i].val_2<<"] ";
		}
		std::string err_out = ss.str();
		LOG_ABORT("monster_config error(birth_coord):%s", err_out.c_str());
	}
	if(walk_invalid.empty() == false){
		std::stringstream ss;
		int count = walk_invalid.size();
		for(int i = 0; i < count; ++i){
			ss<<"["<<walk_invalid[i].val_1<<","<<walk_invalid[i].val_2<<"] ";
		}
		std::string err_out = ss.str();
		LOG_ABORT("monster_config error(walk_coord):%s", err_out.c_str());
	}
}

bool Config_Cache_NPC::check_monster_birth_coord(Coord& birth_coord, std::vector<Coord>& add_coord, int map_id){
	if(add_coord.empty()){
		int map_value = CONFIG_CACHE_SCENE->get_map_value(map_id,birth_coord);
		if(map_value <= 0){
			return false;
		}
	}else{
		int count = add_coord.size();
		for(int i = 0; i < count; ++i){
			int map_value = CONFIG_CACHE_SCENE->get_map_value(map_id,add_coord[i]);
			if(map_value <= 0){
				return false;
			}
		}
	}
	return true;
}

bool Config_Cache_NPC::check_monster_walk_coord(std::vector<Coord>& walk_coord, int map_id){
	int count = walk_coord.size();
	for(int i = 0; i < count; ++i){
		int map_value = CONFIG_CACHE_SCENE->get_map_value(map_id,walk_coord[i]);
		if(map_value <= 0){
			return false;
		}
	}
	return true;
}

bool Config_Cache_NPC::check_drop(void){
	std::vector<int> invalid_vec;
	Drop_Config_Map::const_iterator it;
	for(it = drop_config_map_.begin(); it != drop_config_map_.end(); ++it){
		if(it->second.box_rate_vec.size() != it->second.box_vec.size()){
			invalid_vec.push_back(it->second.id);
		}
	}
	std::stringstream ss;
	if(invalid_vec.empty() == false){
		int count = invalid_vec.size();
		for(int i = 0; i < count; ++i){
			ss<<invalid_vec[i]<<", ";
		}
		std::string err_out = ss.str();
		LOG_ABORT("drop_config error(box_num != box_rate_num ):[%s]", err_out.c_str());
	}

	return true;
}

bool Config_Cache_NPC::check_drop_box(void){
	std::vector<int> invalid_vec;
	Drop_Config_Map::const_iterator it;
	for(it = drop_config_map_.begin(); it != drop_config_map_.end(); ++it){
		int count = it->second.box_vec.size();
		for(int i = 0; i < count; ++i){
			if(it->second.box_vec[i].item_id_vec1.size() != it->second.box_vec[i].item_id_vec2.size() ||
					it->second.box_vec[i].item_id_vec2.size() != it->second.box_vec[i].item_id_vec3.size() ||
					it->second.box_vec[i].item_id_vec3.size() != it->second.box_vec[i].item_rate_vec.size() ||
					it->second.box_vec[i].item_rate_vec.size() != it->second.box_vec[i].item_amount_vec.size() ||
					it->second.box_vec[i].item_amount_vec.size() != it->second.box_vec[i].item_bind_vec.size()){
				MSG_USER("it->second.box_vec[i].item_id_vec1(%d),it->second.box_vec[i].item_id_vec2(%d),it->second.box_vec[i].item_id_vec3(%d),",
						it->second.box_vec[i].item_id_vec1.size(), it->second.box_vec[i].item_id_vec2.size(), it->second.box_vec[i].item_id_vec3.size());
				MSG_USER("it->second.box_vec[i].item_rate_vec(%d).", it->second.box_vec[i].item_rate_vec.size());
				MSG_USER("it->second.box_vec[i].item_amount_vec(%d).", it->second.box_vec[i].item_amount_vec.size());
				MSG_USER("it->second.box_vec[i].item_bind_vec(%d).", it->second.box_vec[i].item_bind_vec.size());
				invalid_vec.push_back(it->second.id);
			}
		}
	}
	std::stringstream ss;
	if(invalid_vec.empty() == false){
		int count = invalid_vec.size();
		for(int i = 0; i < count; ++i){
			ss<<invalid_vec[i]<<", ";
		}
		std::string err_out = ss.str();
		LOG_ABORT("drop_box_config error(num error ):[%s]", err_out.c_str());
	}
	return true;
}

