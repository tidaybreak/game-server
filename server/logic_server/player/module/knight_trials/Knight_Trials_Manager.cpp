/*
 * Knight_Trials_Manager.cpp
 *
 *  Created on: May 12, 2014
 *      Author: xiaoliang
 */

#include "Knight_Trials_Manager.h"
#include "knight_trials/Knight_Trials_Def.h"
#include "Config_Cache_Knight_Trials.h"
#include "Config_Cache.h"
#include "Logic_Player.h"
#include "Msg_Inner_Enum.h"
#include "Logic_Monitor.h"
#include "DB_Def.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "heroer/Logic_Heroer_Func.h"

Knight_Trials_Manager::Knight_Trials_Manager(void) {
	reset();
}

Knight_Trials_Manager::~Knight_Trials_Manager(void) { }

void Knight_Trials_Manager::reset(void) {
	force_role_vec_map.clear();
	force_role_vec.clear();
	lmake_time = Time_Value::gettimeofday();
}

void Knight_Trials_Manager::tick(const Time_Value &now) {
	if(lmake_time.sec() <= now.sec()) {
		make_force_role_vec_form_map();
		lmake_time.sec(now.sec() + Time_Value::ONE_MINUTE_IN_SECS * 30);
	}
}

int Knight_Trials_Manager::logic_knight_trials_match_player(Block_Buffer& buf) {
	uint8_t type = 0;
	buf.read_uint8(type);
	role_id_t role_id = 0;
	buf.read_int64(role_id);
	if(!type || !role_id) {
		return 0;
	}
	int64_t from_uuid = 0;
	buf.read_int64(from_uuid);
	switch(type) {
		case KNIGHT_TRIALS_SIX_MATCH: {
			int ttl_force = 0;
			buf.read_int32(ttl_force);
			uint8_t size = 0;
			buf.read_uint8(size);
			UInt8_Vec rvec;
			rvec.clear();
			for(; size > 0; --size) {
				uint8_t v = 0;
				buf.read_uint8(v);
				rvec.push_back(v);
			}
			Role_Id_Set role_id_set;
			match_player_role_id_by_rankvec(rvec, role_id_set, ttl_force, 0);
			if(rvec.size() > role_id_set.size()) {
				//todo 匹配玩家不足,有问题待优化
			}
			for(Role_Id_Set::iterator it = role_id_set.begin(); it != role_id_set.end(); ++it) {
				//match_player_callback(*it, from_uuid, role_id);
			}
			break;
		}
		case KNIGHT_TRIALS_ONE_MATCH: {
			break;
		}
		case KNIGHT_TRIALS_SIX_MATCH_BACK: {
			//其实判断不判断应该没啥关系;如果这个判断失败就绝对是出问题的;由自己逻辑服发出去;回来自己逻辑服的来源uuid竟然不是自己;肿么可能
			if(from_uuid == CONFIG_CACHE->server_flag()) {
				Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
				if(player) {
					Knight_Trials_Player kplayer;
					kplayer.reset();
					buf.read_int64(kplayer.uuid);
					buf.read_string(kplayer.server_name);
					kplayer.base.deserialize(buf);
					kplayer.fighter.deserialize(buf);
					kplayer.heroer.deserialize_fighter(buf);
					//调用回调接口插入玩家
					player->insert_knight_trials_player_fighter_info_vec_callback(kplayer);
				} else {
					//返回的时候;玩家不在线就不管了
				}
			}
			break;
		}
		case KNIGHT_TRIALS_ONE_MATCH_BACK: {
			break;
		}
	}
	return 0;
}

int Knight_Trials_Manager::match_player_local(role_id_t role_id, Knight_Trials_Player &kplayer, role_id_t from_role_id, int64_t match_day, bool clear) {
	//2014年12月3日 11:28:04修改不管玩家是否在线都从数据库取数据
////	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
////	if (player) {//玩家在线啊
////		kplayer.reset();
////		kplayer.uuid = CONFIG_CACHE->server_flag();
////		kplayer.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
////		kplayer.base = player->base_detail();
////		kplayer.fighter = player->fighter_detail();
//		/*********ttl_force***********/
//		kplayer.fighter.ttl_force = player->fighter_detail().force + player->get_fight_hero_force();
//		/*******************/
////		kplayer.heroer = player->heroer_detail();
////	} else { //玩家不在线;数据库异步取数据
//new
//		Block_Buffer buf;
//		buf.make_message(MONGO_KNIGHT_TRIALS_MATCH_PLAYER);
//		buf.write_int64(0);//本地
//		buf.write_int64(from_role_id);
//		buf.write_int64(role_id);
//		buf.write_int64(match_day);
//		buf.write_bool(clear);
//		buf.finish_message();
//		DB_MANAGER->push_data_block(buf, 0);
//new
		return -1;
////	}
////	return 0;
}

//void Knight_Trials_Manager::match_player_callback(role_id_t role_id, int64_t from_uuid, role_id_t from_role_id) {
//	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
//	if (player) {//玩家在线啊
//
//		//player->set_knight_trials_uuid(CONFIG_CACHE->server_flag());
//		//player->set_knight_trials_server_name(CONFIG_CACHE->server_maintainer_cache().game_server_flag);
//
//		Block_Buffer buf;
//		buf.make_message(INNER_KNIGHT_TRIALS_MATCH_PLAYER);
//		buf.write_uint8(KNIGHT_TRIALS_SIX_MATCH_BACK);
//		buf.write_int64(from_role_id);
//		buf.write_int64(from_uuid);//请求来的服务器
//
//		buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器
//		buf.write_string(CONFIG_CACHE->server_maintainer_cache().game_server_flag);
////		/*********ttl_force***********/
////		kplayer.fighter.ttl_force = player->fighter_detail().force + player->get_fight_hero_force();
////		/*******************/
//		//人物基本信息
//		player->base_detail().serialize(buf);
//		// 人物战斗信息
//		player->fighter_detail().serialize(buf);
//		//英雄信息
//		player->heroer_detail().serialize_fighter(buf);
//
//		buf.finish_message();
//		LOGIC_MONITOR->send_to_scene(KNIGHT_TRIALS_INNER_SCENE_ID, 0, buf);
//	} else {//玩家不在线啊
//		Block_Buffer buf;
//		buf.make_message(MONGO_KNIGHT_TRIALS_MATCH_PLAYER);
//		buf.write_int64(from_uuid);// 跨服
//		buf.write_int64(from_role_id);
//		buf.write_int64(role_id);
//		buf.finish_message();
//		DB_MANAGER->push_data_block(buf, 0);
//	}
//}

void Knight_Trials_Manager::process_load_fighter_player_data(Knight_Trials_Player *kplayer) {
	if(!kplayer) {
		MSG_DEBUG("logic_player_detail is 0");
		return;
	}
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(kplayer->to_role_id);
	if(player) {
		player->local_callback_knight_trals_enter_fighter(*kplayer);
	} else {
		MSG_DEBUG("process_load_fighter_player_data player not online, role_id:%d", kplayer->to_role_id);
	}
}

void Knight_Trials_Manager::process_load_player_data(Logic_Player_Detail* logic_player_detail) {
	if(!logic_player_detail) {
		MSG_DEBUG("logic_player_detail is 0");
		return;
	}

	if(logic_player_detail->status != Logic_Player_Detail::SUCCESS_LOADED) {
		POOL_MANAGER->push_player_data_pool(logic_player_detail, 17);
		return;
	}
	Knight_Trials_Player kplayer;
	kplayer.reset();
	kplayer.base = logic_player_detail->base_detail;
	kplayer.fighter = logic_player_detail->fighter_detail;
	kplayer.heroer = logic_player_detail->heroer_detail;
	refresh_Heros_skill(kplayer.heroer);

	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(kplayer.base.role_id);
	if (iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		//时装 start
		kplayer.base.avatar_vec = iter->second->avatar_vec;
		//时装 end
	} else {
		MSG_USER("player(role_id=%ld) is not in db_view", kplayer.base.role_id);
	}


	if(logic_player_detail->knight_trials_detail.to_uuid) {//需要发往跨服
		Block_Buffer buf;
		/* head start*/
		buf.make_message(INNER_KNIGHT_TRIALS_MATCH_PLAYER);
		buf.write_uint8(KNIGHT_TRIALS_SIX_MATCH_BACK);
		buf.write_int64(logic_player_detail->knight_trials_detail.to_role_id);
		buf.write_int64(logic_player_detail->knight_trials_detail.to_uuid);//请求来的服务器,要发回那个服务器
		/* head end*/
		/* body start*/
		buf.write_int64(CONFIG_CACHE->server_flag());//自己的服务器
		buf.write_string(CONFIG_CACHE->server_maintainer_cache().game_server_flag);
		//人物基本信息
		kplayer.base.serialize(buf);
		// 人物战斗信息
		kplayer.fighter.serialize(buf);
		//英雄信息
		kplayer.heroer.serialize_fighter(buf);
		/* body end*/
		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(KNIGHT_TRIALS_INNER_SCENE_ID, 0, buf);
	} else {//就是本地匹配的
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(logic_player_detail->knight_trials_detail.to_role_id);
		if(player) {
			kplayer.uuid = CONFIG_CACHE->server_flag();
			kplayer.server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
			player->insert_knight_trials_player_fighter_info_vec(kplayer);
		} else {
			//返回的时候;玩家不在线就不管了
		}
	}
	POOL_MANAGER->push_player_data_pool(logic_player_detail, 18);
}

int Knight_Trials_Manager::match_player_role_id_by_rankvec(const UInt8_Vec &rank, Role_Id_Set &role_id_set, int ttl_force, role_id_t self_role_id) {
	if(force_role_vec.empty()) {
		MSG_DEBUG("knight match force_role_vec is empty my_role_id:%ld",self_role_id);
		return -1;
	}
	for(UInt8_Vec::const_iterator rit = rank.begin(); rit != rank.end(); ++rit) {
		const Knight_Trials_Points_Conf *conf = CONFIG_CACHE_KNIGHT_TRIALS->get_points_conf_by_points(*rit);
		if(conf){
			//随机计算开始和结束战力
			int start = (ttl_force * conf->spro) / 100;
			int end = (ttl_force * conf->epro) / 100;
			srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
			int rstart = random() % (end-start+1);
			srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
			int rend = random() % (end-start+1);
			if(rstart < rend) {
				start = start+rstart;
				end = end+rend;
			} else {
				start = start+rend;
				end = end+rstart;
			}
			//查找在这区间里的人
			int index = force_role_vec.size()/2;
			int ui = force_role_vec.size(); //上限
			int di = 0;	//下限
			int tforce = force_role_vec[index].force;
			while((tforce < start || tforce > end) && (ui != index && di != index)) {
				if(tforce < start) {
					di = index;
					index = index + (ui-index)/2;
				} else if(tforce > end) {
					ui = index;
					index = di + (index - di)/2;
				}
				tforce = force_role_vec[index].force;
			}
			//找出角色ID
			role_id_t role_id = 0;
			do{
				for(uint32_t i = 0; i < force_role_vec[index].role_vec.size(); ++i) {
					role_id = force_role_vec[index].role_vec[i];
					if(role_id_set.find(role_id) == role_id_set.end() && role_id != self_role_id) {
						break;
					} else {
						role_id = 0;
					}
				}
				if(!role_id) {
					--index;
				} else {
					role_id_set.insert(role_id);
					MSG_DEBUG("start knight match the my_role_id:%ld rank:%d role_id:%ld force:%d",self_role_id, *rit, role_id, tforce);
					break;
				}
			} while(role_id == 0 && index >= 0);
		}//end if
	}
	return 0;
}

void Knight_Trials_Manager::insert_force_role_id_match(int force, role_id_t role_id) {
	Force_Role_Set_Map::iterator it = force_role_vec_map.find(force);
	if(it != force_role_vec_map.end()) {
		it->second.insert(role_id);
	} else {
		Role_Id_Set role_set;
		role_set.clear();
		role_set.insert(role_id);
		force_role_vec_map.insert(std::make_pair(force, role_set));
	}
}

void Knight_Trials_Manager::make_force_role_vec_form_map(void) {
	force_role_vec.clear();
	Force_Role_Struct v;
	for(Force_Role_Set_Map::iterator it = force_role_vec_map.begin(); it != force_role_vec_map.end(); ++it) {
		v.reset();
		v.force = it->first;
		for(Role_Id_Set::iterator sit = it->second.begin(); sit != it->second.end(); ++sit) {
			v.role_vec.push_back(*sit);
		}
		force_role_vec.push_back(v);
	}
}

void Knight_Trials_Manager::login_out_update_map(int old_force, int new_force, role_id_t role_id) {
	Force_Role_Set_Map::iterator it = force_role_vec_map.find(old_force);
	if(it != force_role_vec_map.end()) {
		Role_Id_Set::iterator rit = it->second.find(role_id);
		if(rit != it->second.end()) {
			it->second.erase(rit);
		} else {
			MSG_DEBUG("No role_id %ld record in force_role_vec_map.Role_Id_Set (login_out_update_map) old_force:%d", role_id, old_force);
		}
		if(!it->second.size()) {
			force_role_vec_map.erase(it);
		}
	} else {
		MSG_DEBUG("No old force %d record in force_role_vec_map (login_out_update_map)", old_force);
	}
	insert_force_role_id_match(new_force, role_id);
}
