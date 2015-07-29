/*
 * Robot_Player.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#include "Robot_Player.h"
#include "Robot_Monitor.h"
#include "Config_Cache.h"
#include "Config_Cache_Robot.h"
#include "Msg_Role_Scene_Struct.h"
#include "Msg_Fight_Struct.h"
#include "Msg_Test_Struct.h"
#include "Msg_Task_Struct.h"
#include "Msg_NPC_Struct.h"
#include "Msg_Test_Enum.h"
#include "Msg_Task_Enum.h"
#include "Msg_NPC_Enum.h"
#include "Msg_Role_Scene_Enum.h"
#include "task/Task_Def.h"
#include "task/Task_Struct.h"
#include "Config_Cache_NPC.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Role.h"

Robot_Player::Robot_Player() {
	reset();
}

Robot_Player::~Robot_Player() {
	// TODO Auto-generated destructor stub
}

void Robot_Player::reset(void) {
	cid_ = 0;
	login_logic_cid_ = 0;
	monitor_ = ROBOT_MONITOR;
	base_detail_.reset();
	msg_detail_.reset();
	tick_ = Time_Value::zero;
	npc_appear_map_.clear();
}


int Robot_Player::send_to_server(Block_Buffer &buf) {
	return monitor_->send_to_gate(cid_, buf);
}


int Robot_Player::load_detail(Block_Buffer &buf) {

	return 0;
}

int Robot_Player::tick(Time_Value &now) {
//	if (tick_ == Time_Value::zero) {
//		tick_ = now;
//		//player_init();
//	}
//	Time_Value tv_diff = now - tick_;
//	if (tv_diff.sec() < 1 && tv_diff.usec() < 500000) {	//
//		return 0;
//	} else {
//		tick_ = now;
//	}

	switch (base_detail_.status) {
//	case STATUS_BEGIN: {
//		break;
//	}
	case Robot_Detail::LOGIN: {
		//tick_logining(now);
		break;
	}
//	case Robot_Detail::LOGIN: {
//		tick_logined(now);
//		break;
//	}
//	case STATUS_EXCEPTION: {
//		LOG_USER("robot %s exception", base_detail_.role_name.c_str());
//		break;
//	}
	default: {
		//LOG_USER("unknow status_ = %d", status_);
		tick_logined(now);
		break;
	}
	}
	return 0;
}

int Robot_Player::tick_logined(Time_Value &now) {
	switch (base_detail_.status) {
	case Robot_Detail::LOGOUT: {
		return 0;
	}
	case Robot_Detail::INITING: {
		++base_detail_.loop_initing;
		if (base_detail_.loop_initing < LOOP_INITING) {
			return 0;
		} else {
			base_detail_.status = Robot_Detail::IDLE;
		}
		break;
	}
	case Robot_Detail::FIGHT: {
		// 战斗
		return 0;
	}
	case Robot_Detail::WAITING_EXIT_DIFF_SPACE: {
		++base_detail_.loop_exit_diff_space;
		if (base_detail_.loop_exit_diff_space < LOOP_WAITING_EXIT_DIFF_SPACE) {
			return 0;
		} else {
			base_detail_.status = Robot_Detail::IDLE;
		}
		break;
	}
	case Robot_Detail::TURN_SCENES: {
		++base_detail_.loop_transfer_scene;
		if (base_detail_.loop_transfer_scene < LOOP_TURN_SCENES) {
			return 0;
		} else {
			base_detail_.status = Robot_Detail::IDLE;
		}
		break;
	}
	case Robot_Detail::WAYFINDING: {
		++base_detail_.loop_path_finding;
		if (base_detail_.loop_path_finding < LOOP_WAYFINDING) {
			return 0;
		} else {
			base_detail_.status = Robot_Detail::IDLE;
		}
		break;
	}
	case Robot_Detail::COLLECTING: {
		++base_detail_.loop_collecting;
		if (base_detail_.loop_collecting < LOOP_COLLECTING) {
			return 0;
		} else {
			base_detail_.status = Robot_Detail::IDLE;
		}
		break;
	}
	case Robot_Detail::FB_WAYFINDING: {
		++base_detail_.loop_fb_wayfinding;
		if (base_detail_.loop_fb_wayfinding < LOOP_FB_WAYFINDING) {
			return 0;
		} else {
			++base_detail_.loop_fb_wayfind_end;
			if (base_detail_.loop_fb_wayfind_end > LOOP_FB_WAYFIND_END) {
				base_detail_.status = Robot_Detail::IDLE;
				base_detail_.loop_fb_wayfind_end = 0;
				base_detail_.loop_fb_wayfinding = 0;

				if (!in_main_scene() && task_info_.id) {
					if (task_info_.id == 10100101) {
						const Player_Init_Cache& cfg_player_init_cache = CONFIG_CACHE_ROLE->player_init_cache();
						if (! cfg_player_init_cache.birth_scene) {
							return -1;
						}
						wayfind_info_.cur_location.scene_id = cfg_player_init_cache.safe_scene;
						wayfind_info_.cur_location.coord = cfg_player_init_cache.safe_coord;
						wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
						gm_finish_task(task_info_.id);
						exit_diff_space();
						return 0;
						//if (wayfind_info_.det_scene_id != wayfind_info_.cur_location.scene_id) {
						//	return transfer_scene();
						//}
					} else {
						gm_finish_task(task_info_.id);
						exit_diff_space();
						return 0;
					}
				}
			}
		}
		break;
	}


	case Robot_Detail::MOVE: {
		if (base_detail_.sub_status == Robot_Detail::FIGHT) {
			// 战斗
			return 0;
		}
		// 移动
		move();
		return 0;
	}
//	case Robot_Detail::FIGHT: {
//		// 战斗
//		return 0;
//	}
	default:
		break;
	}

	/// 再处理移动
	int robot_def = CONFIG_CACHE_ROBOT->robot_config().robot_def;
	switch(robot_def) {
		case 0: {
			rand_move();
			break;
		}
		case 1: {
			ai_task(now);
			break;
		}
	}

	return 0;
}

int Robot_Player::rand_move(void) {
//	int f[2] = {-1, 1};
//	Grid_Coord target_coord;
//	target_coord.grid_x = base_detail_.coord.grid_x + f[rand() % 2] * (rand() % 40);
//	target_coord.grid_y = base_detail_.coord.grid_y + f[rand() % 2] * (rand() % 40);
//	if (scene_->mpt_value_by_coord(target_coord.grid_x, target_coord.grid_y) < Scene::MOVE_ABLE_FROM)
//		return 0;
//
//	walk_path_.reset();
//	path_find(mover_detail().grid_coord, target_coord, walk_path_.walk_coord, false);
//	walk_path_.curr_coord_index = walk_path_.walk_coord.size();

	wayfind_info().det_scene_id = 80100001;
	if (wayfind_info_.det_scene_id != wayfind_info_.cur_location.scene_id) {
		return transfer_scene();
	} else {
		Coord coord = wayfind_info().cur_location.coord;
		int move_point = 32 * 5;
		switch(random()%8) {
			case 0:{
				coord.x += move_point;
				break;
			}
			case 1:{
				coord.x -= move_point;
				break;
			}
			case 2:{
				coord.y += move_point;
				break;
			}
			case 3:{
				coord.y -= move_point;
				break;
			}
			case 4:{
				coord.x += move_point;
				coord.y += move_point;
				break;
			}
			case 5:{
				coord.x -= move_point;
				coord.y += move_point;
				break;
			}
			case 6:{
				coord.x -= move_point;
				coord.y -= move_point;
				break;
			}
			case 7:{
				coord.x += move_point;
				coord.y -= move_point;
				break;
			}
		}
		wayfind_info_.det_coord = coord;

		path_find_move();
	}

	return 0;
}

int Robot_Player::ai_task(Time_Value &now) {
	if (!task_info_.id || task_info_.type != 101) {
		req_fetch_task_list();
		return -1;
	}

	if (task_info_.id == 10100101) {
//		const Player_Init_Cache& cfg_player_init_cache = CONFIG_CACHE_ROLE->player_init_cache();
//		if (! cfg_player_init_cache.birth_scene) {
//			return -1;
//		}
//		wayfind_info_.cur_location.scene_id = cfg_player_init_cache.birth_scene;
//		wayfind_info_.cur_location.coord = cfg_player_init_cache.birth_coord;
	}
	if (task_info_.id >= 10100600) {
		base_detail_.status = Robot_Detail::LOGOUT;
		return monitor()->close_link_by_cid(cid_, Link_Close(10100600, 0));
	}

	/*
	Task_Info
	value : 保存当前任务完成度

	status: 1不可接 2可接 3已接 4可交付 5已交付 6失败

	extra :

	is_auto_submit:  可直接交付的任务，在任务完成后(状态为4)置为1；其他情况下，全为0
	task_quality: 悬赏任务品质。 0:白 1:绿  2:蓝  3：紫 4:橙
	type: 任务类型。
	                101:主线任务  102: 支线任务   103: 日常任务   104:周常任务   105: 公会任务
	                106:活动任务   107: 精英任务   108:活动        109:英雄任务    110:悬赏任务
	*/
//    TASK_STATUS_VISIBLE = 1,    // 任务可见(不可接)
//    TASK_STATUS_ACCEPTABLE = 2, // 任务可接
//    TASK_STATUS_ACCEPTED = 3,   // 任务已接
//    TASK_STATUS_FINISH = 4,     // 任务已完成
//    TASK_STATUS_SUBMIT = 5,     // 任务已提交
//    TASK_STATUS_FAIL = 6,       // 限时任务失败

	switch (task_info_.status) {
	case TaskInfo::TASK_STATUS_VISIBLE:
		ai_task_visible();
		break;
	case TaskInfo::TASK_STATUS_ACCEPTABLE:
		ai_task_acceptable();
		break;
	case TaskInfo::TASK_STATUS_ACCEPTED:
		ai_task_accepted();
		break;
	case TaskInfo::TASK_STATUS_FINISH:
		ai_task_finish();
		break;
	case TaskInfo::TASK_STATUS_SUBMIT:
		ai_task_submit();
		break;
	}


	return 0;
}

void Robot_Player::set_cid(int cid) {
	cid_ = cid;
}

void Robot_Player::set_login_logic_cid(int cid) {
	login_logic_cid_ = cid;
}

Robot_Player *Robot_Player::player_self(void) {
	return this;
}

std::string Robot_Player::role_name(void) {
	return base_detail_.role_name;
}


role_id_t Robot_Player::role_id(void) {
	return base_detail_.role_id;
}

int Robot_Player::level(void) {
	return 0;
}

int Robot_Player::career(void) {
	return 0;
}

int Robot_Player::notice_logic_sign_out(int out_reason) {

	return 0;
}

int Robot_Player::verify_msg_detail(uint32_t serial_cipher, uint32_t msg_time_cipher) {

	if (! msg_detail_.is_inited) {
		//msg_detail_.hash_key = ELFHash(base_detail_.account_info.account_name.c_str(), base_detail_.account_info.account_name.length());
	}
	uint32_t serial = serial_cipher;// ^ msg_detail_.hash_key;
	uint32_t msg_time = msg_time_cipher;// ^ serial_cipher;

	Time_Value now = Time_Value::gettimeofday();
	if (! msg_detail_.is_inited) {
		msg_detail_.is_inited = true;
		msg_detail_.msg_serial = serial; /// 第一条消息序号
		msg_detail_.msg_timestamp = now; /// 第一条消息时间戳
		msg_detail_.msg_interval_count_ = 1;
		msg_detail_.msg_interval_timestamp = now;
		return 0;
	}

	/// 操作频率统计
	if (now - msg_detail_.msg_interval_timestamp < Time_Value(3, 0)) {
		if (msg_detail_.msg_interval_count_ > 90) {
			MSG_USER("msg_interval_count:%d", msg_detail_.msg_interval_count_);
			return 10000116;//ERROR_CLOSE_CLIENT_TOO_OFTEN;
		}
	} else {
		msg_detail_.msg_interval_count_ = 0;
		msg_detail_.msg_interval_timestamp = now;
	}

	/// 判断包序号
	if (serial <= msg_detail_.msg_serial || serial - msg_detail_.msg_serial > 10) {
		MSG_USER("serial = %d, msg_detail_.msg_serial = %d", serial, msg_detail_.msg_serial);
		return 10000117;//ERROR_CLOSE_CLIENT_MSG_SERIAL;
	}

	/// 判断包时间戳
	Time_Value msg_time_tv(msg_time, 0);
	if (std::abs(msg_time - now.sec()) > 10) {
		MSG_USER("msg_time = %d, msg_timestamp = %d", msg_time, msg_detail_.msg_timestamp.sec());
		return 10000118;//ERROR_CLOSE_CLIENT_MSG_TIME;
	}

	msg_detail_.msg_serial = serial;
	msg_detail_.msg_timestamp = msg_time_tv;
	++msg_detail_.msg_interval_count_;

	return 0;
}

int Robot_Player::move(void) {
	Coord_Vec::reverse_iterator rit = wayfind_info().path.rbegin();
	if(rit != wayfind_info().path.rend()) {
		MSG_10200102 move_msg;
		move_msg.role_id = base_detail().role_id;
		move_msg.path.push_back(rit->pixel_coord());

		wayfind_info().cur_location.coord = rit->pixel_coord();
		wayfind_info().path.erase(rit.base());
		uint8_t move_skip_range = CONFIG_CACHE_ROBOT->robot_config().move_skip_range;
		while(wayfind_info().path.size() > 1 && move_skip_range > 0) {
			wayfind_info().path.erase(--wayfind_info().path.end());
			--move_skip_range;
		}
/**************************/ //比较坑;第一次需要发多一个格仔
		if(wayfind_info().first_move) {
			wayfind_info().first_move = false;
			for(int i=0; i < 2; ++i) {
				Coord_Vec::reverse_iterator rit_first = wayfind_info().path.rbegin();
				if(rit_first != wayfind_info().path.rend()) {
					move_msg.path.push_back(rit_first->pixel_coord());
					wayfind_info().cur_location.coord = rit_first->pixel_coord();
					wayfind_info().path.erase(rit_first.base());
					uint8_t frist_move_skip_range = CONFIG_CACHE_ROBOT->robot_config().move_skip_range;
					while(wayfind_info().path.size() > 1 && frist_move_skip_range > 0) {
						wayfind_info().path.erase(--wayfind_info().path.end());
						--frist_move_skip_range;
					}
				}
			}
		}
/******************************/
		Block_Buffer move_buf;
		move_buf.make_client_message(10200102, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		move_msg.serialize(move_buf);
		move_buf.finish_message();
		this->send_to_server(move_buf);

		if (reach_dest_grid()) {
			wayfind_info_.path.clear();
			base_detail_.status = Robot_Detail::IDLE;
		}
	} else {
		//MSG_USER("%s >< 移动end", player->base_detail().account.c_str());
		//player->base_detail().status = Robot_Detail::TURN_SCENES;

//		path_find();
//		base_detail_.status = Robot_Detail::MOVE;
		base_detail().status = Robot_Detail::IDLE;
		//rand_move();
	}

	return 0;
}

int Robot_Player::player_init(void) {
	gm_level(50);
	req_fetch_task_list();
	gm_modify_prop();

	base_detail_.status = Robot_Detail::INITING;
	base_detail_.loop_initing = 1;

	const Player_Init_Cache& cfg_player_init_cache = CONFIG_CACHE_ROLE->player_init_cache();
	if (! cfg_player_init_cache.birth_scene) {
		return -1;
	}
	wayfind_info_.cur_location.scene_id = cfg_player_init_cache.birth_scene;
	wayfind_info_.cur_location.coord = cfg_player_init_cache.birth_coord;

	return 0;
}

void Robot_Player::gm_level(const int level) {
	char char_level[100];
	memset(char_level, 0x00, sizeof(char_level));
	snprintf(char_level, sizeof(char_level), "level %d", level);
	MSG_10299999 msg;
	msg.command = char_level;
	Block_Buffer buf;
	buf.make_client_message(REQ_SCENE_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg.serialize(buf);
	buf.finish_message();
	this->send_to_server(buf);
}

void Robot_Player::gm_modify_prop(void) {
	char _char[100];
	memset(_char, 0x00, sizeof(_char));
	int prop = PT_POWER, val = 10000;
	snprintf(_char, sizeof(_char), "prop %d %d", prop, val);
	MSG_10299999 msg;
	msg.command = _char;
	Block_Buffer buf;
	buf.make_client_message(REQ_SCENE_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg.serialize(buf);
	buf.finish_message();
	this->send_to_server(buf);

	memset(_char, 0x00, sizeof(_char));
	prop = PT_ARMOR;
	snprintf(_char, sizeof(_char), "prop %d %d", prop, val);
	msg.reset();
	msg.command = _char;
	buf.reset();
	buf.make_client_message(REQ_SCENE_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg.serialize(buf);
	buf.finish_message();
	this->send_to_server(buf);

	memset(_char, 0x00, sizeof(_char));
	prop = PT_STAMINA;
	snprintf(_char, sizeof(_char), "prop %d %d", prop, val);
	msg.reset();
	msg.command = _char;
	buf.reset();
	buf.make_client_message(REQ_SCENE_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg.serialize(buf);
	buf.finish_message();
	this->send_to_server(buf);


	// money
	memset(_char, 0x00, sizeof(_char));
	snprintf(_char, sizeof(_char), "money %d", 999999999);
	MSG_10299999 logic_msg;
	logic_msg.reset();
	logic_msg.command = _char;
	buf.reset();
	buf.make_client_message(REQ_LOGIC_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	logic_msg.serialize(buf);
	buf.finish_message();
	this->send_to_server(buf);

}

void Robot_Player::gm_finish_task(const int task_id) {
	char _char[100];
	memset(_char, 0x00, sizeof(_char));
	snprintf(_char, sizeof(_char), "task 7 %d", task_id);
	MSG_10299999 logic_msg;
	logic_msg.reset();
	logic_msg.command = _char;
	Block_Buffer buf;
	buf.make_client_message(REQ_LOGIC_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	logic_msg.serialize(buf);
	buf.finish_message();
	this->send_to_server(buf);
}


int Robot_Player::set_path_find_status(void) {
	base_detail_.status = Robot_Detail::WAYFINDING;
	base_detail_.loop_path_finding = 1;
	return 0;
}

int Robot_Player::path_find_move(void) {
	path_find();
	if (! wayfind_info().path.empty()) {
		base_detail_.status = Robot_Detail::MOVE;
	} else {
		set_path_find_status();
	}
	return 0;
}

int Robot_Player::ai_task_visible(void) {
	if (!task_info_.id || task_info_.status != TaskInfo::TASK_STATUS_VISIBLE) {
		return -1;
	}
	gm_level(60);
	return 0;
}

int Robot_Player::ai_task_acceptable(void) {
	if (task_info_.id && task_info_.status == TaskInfo::TASK_STATUS_ACCEPTABLE) {
		MSG_10110002 logic_msg;
		logic_msg.task_id = task_info_.id ;
		Block_Buffer buf;
		buf.make_client_message(REQ_TASK_ACCETP, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		logic_msg.serialize(buf);
		buf.finish_message();
		this->send_to_server(buf);
	}
	return 0;
}

int Robot_Player::ai_task_accepted(void) {
	if (!task_info_.id || task_info_.status != TaskInfo::TASK_STATUS_ACCEPTED) {
		return -1;
	}
	const Task_Mission_Detail_Config *task_cfg = CONFIG_CACHE_TASK->find_task(task_info_.id);
	if(!task_cfg || task_cfg->execute.empty()) {
		return -1;
	}

//
//		if(conf->execute.size() <= analyze_progress) {
//			//todo 完成任务了
//			return;
//		}
//		Task_Mission_Exec_Detail_Cfg exe_cfg = conf->execute[analyze_progress];
//		switch(exe_cfg.type) {
//			case 111112: {
//				do_111112(exe_cfg.id, exe_cfg.amount);
//				break;
//			}
//		}
//	}


	// 找怪
//	const Monster_Section_Config* monster_section_cfg = CONFIG_CACHE_NPC->get_monster_section_config(wayfind_info_.cur_location.scene_id);
//	if (! monster_section_cfg) {
//		return 0;
//	}
//	std::vector<Coord> vec_monster_coord;
//	for (Monster_Point_Config_Map::const_iterator it_point_map = monster_section_cfg->point_map.begin();
//			it_point_map != monster_section_cfg->point_map.end(); ++it_point_map) {
//		vec_monster_coord.push_back(it_point_map->second.birth_coord);
//	}
//	if (!vec_monster_coord.empty() && wayfind_info_.path.empty()) {
//		wayfind_info_.det_coord = coord;
//		wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
//		path_find();
//		base_detail_.status = Robot_Detail::MOVE;
//	}


	if (! task_cfg->execute.empty()) {
		switch (task_cfg->execute[0].type) {
		case TaskExecType::TASK_EXEC_COLLECT: {
			// 采集任务
			return task_exec_collect(task_cfg);

//			wayfind_info_.det_scene_id = task_cfg->execute[0].sceneid;
//			wayfind_info_.det_coord.set(task_cfg->execute[0].target_x, task_cfg->execute[0].target_y, Coord::PIXEL_TYPE);
//			//wayfind_info_.det_coord.set(2000, 1000, Coord::PIXEL_TYPE);
//
//			if (wayfind_info_.det_scene_id != wayfind_info_.cur_location.scene_id) {
//				//wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
//				transfer_scene();
//			} else {
//				bool find_out = false;
//				Coord src = wayfind_info_.cur_location.coord.grid_coord();
//				Coord det = wayfind_info_.det_coord.grid_coord();
//				//if (wayfind_info_.cur_location.coord == wayfind_info_.det_coord) {
//				if (src == det) {
//					// 采集
//					MSG_DEBUG("  %s 采集时 npc_appear_map_.size: %d.   ", base_detail_.account.c_str(), npc_appear_map_.size());
//					for (std::map<role_id_t, NPC_Appear_Info>::const_iterator it_npc_appear = npc_appear_map_.begin();
//							it_npc_appear != npc_appear_map_.end(); ++it_npc_appear) {
//						//int npc_type = it_npc_appear->second.npc_type;
//						//int talk = task_cfg->execute[0].talk;
//						if (4000000 < it_npc_appear->first && it_npc_appear->first < 5000000 &&
//								it_npc_appear->second.npc_type == (int)task_cfg->execute[0].talk) {
//
//							if (it_npc_appear->second.add_id.empty()) {
//								// 不指定归属采集物
//								find_out = true;
//								MSG_10210100 msg;
//								msg.material_role_id = it_npc_appear->first;
//								Block_Buffer buf;
//								buf.make_client_message(REQ_START_GATHER, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
//								msg.serialize(buf);
//								buf.finish_message();
//								this->send_to_server(buf);
//								break;
//
//							} else {
//								// 指定归属采集物
//								for (size_t i_add_id = 0; i_add_id < it_npc_appear->second.add_id.size(); ++i_add_id) {
//									if (base_detail_.role_id == it_npc_appear->second.add_id[i_add_id]) {
//										find_out = true;
//										MSG_10210100 msg;
//										msg.material_role_id = it_npc_appear->first;
//										Block_Buffer buf;
//										buf.make_client_message(REQ_START_GATHER, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
//										msg.serialize(buf);
//										buf.finish_message();
//										this->send_to_server(buf);
//										break;
//									}
//								}
//							}
//						}
//					}
//				} else {
////				if (! find_out) {
//					path_find();
//					base_detail_.status = Robot_Detail::MOVE;
//				}
//			}
			return 0;

		}
		case TaskExecType::TASK_EXEC_FB: {
			// 完成副本任务
			return task_exec_fb(task_cfg);
//			wayfind_info_.det_scene_id = task_cfg->execute[0].id;
//			if (wayfind_info_.det_scene_id != wayfind_info_.cur_location.scene_id) {
//				//wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
//				transfer_scene();
//			} else {
//				if (base_detail_.walked_mosters_points) {
//					if (! in_main_scene()) {
//						gm_finish_task(task_info_.id);
//						exit_diff_space();
//					}
//					return 0;
//				}
//
//				//bool find_out = false;	// TODO
////				if (! base_detail_.need_walk_monster_points.empty()) {
////					base_detail_.need_walk_monster_points.erase(base_detail_.need_walk_monster_points.begin());
////					if (base_detail_.need_walk_monster_points.empty()) {
////						base_detail_.walked_mosters_points = true;
////						return 0;
////					}
////				}
//
//				// 找怪
//				const Monster_Section_Config* monster_section_cfg = CONFIG_CACHE_NPC->get_monster_section_config(wayfind_info_.cur_location.scene_id);
//				if (!monster_section_cfg || monster_section_cfg->point_map.empty()) {
//					return 0;
//				}
//				if (base_detail_.need_walk_monster_points.empty()) {
//					for (Monster_Point_Config_Map::const_iterator it_cfg_point_map = monster_section_cfg->point_map.begin();
//							it_cfg_point_map != monster_section_cfg->point_map.end(); ++it_cfg_point_map) {
//						int key = it_cfg_point_map->first;
//						if (key >= 100) {
//							continue;
//						}
//						base_detail_.need_walk_monster_points.insert(std::make_pair(it_cfg_point_map->first, it_cfg_point_map->second));
//					}
//				}
//
//				MSG_DEBUG(">>>> 需要清理的怪点还剩: %d个", base_detail_.need_walk_monster_points.size());
//				//std::vector<Coord> vec_monster_coord;
//				//vec_monster_coord.push_back(base_detail_.need_walk_monster_points.begin()->second.birth_coord);
//
//				if (wayfind_info_.path.empty()) {	// !vec_monster_coord.empty() &&
//					wayfind_info_.det_coord = base_detail_.need_walk_monster_points.begin()->second.birth_coord;
//					wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
//					path_find();
//					base_detail_.status = Robot_Detail::MOVE;
//				}
//
//				if (! wayfind_info_.path.empty()) {
//					base_detail_.status = Robot_Detail::MOVE;
//				} else {
//					gm_finish_task(task_info_.id);
//					exit_diff_space();
//				}
//
//
////				Coord src = wayfind_info_.cur_location.coord.grid_coord();
////				Coord det = wayfind_info_.det_coord.grid_coord();
////				//if (wayfind_info_.cur_location.coord == wayfind_info_.det_coord) {
////				if (src == det) {
////					// 采集
////					for (std::map<role_id_t, NPC_Appear_Info>::const_iterator it_npc_appear = monitor()->npc_appear_map().begin();
////							it_npc_appear != monitor()->npc_appear_map().end(); ++it_npc_appear) {
////						//int npc_type = it_npc_appear->second.npc_type;
////						//int talk = task_cfg->execute[0].talk;
////						if (4000000 < it_npc_appear->first && it_npc_appear->first < 5000000 &&
////								it_npc_appear->second.npc_type == (int)task_cfg->execute[0].talk) {
////							find_out = true;
////							MSG_10210100 msg;
////							msg.material_role_id = it_npc_appear->first;
////							Block_Buffer buf;
////							buf.make_client_message(REQ_START_GATHER, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
////							msg.serialize(buf);
////							buf.finish_message();
////							this->send_to_server(buf);
////						}
////					}
////				} else {
//////				if (! find_out) {
////					path_find();
////					base_detail_.status = Robot_Detail::MOVE;
////				}
//			}
			return 0;
		}
		case TaskExecType::TASK_EXEC_ARRIVE: {
			// 到达目的地
			task_exec_arrive(task_cfg);
			return 0;
		}
		default:
			break;
		}
	}

	// GM
	char _char[100];
	memset(_char, 0x00, sizeof(_char));
	snprintf(_char, sizeof(_char), "task 7 %d", task_info_.id);
	MSG_10299999 logic_msg;
	logic_msg.reset();
	logic_msg.command = _char;
	Block_Buffer buf;
	buf.make_client_message(REQ_LOGIC_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	logic_msg.serialize(buf);
	buf.finish_message();
	this->send_to_server(buf);

	return 0;
}

int Robot_Player::ai_task_finish(void) {
	if (task_info_.id && task_info_.status == TaskInfo::TASK_STATUS_FINISH) {
		MSG_10110004 logic_msg;
		logic_msg.task_id = task_info_.id ;
		Block_Buffer buf;
		buf.make_client_message(REQ_TASK_COMPLETE, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
		logic_msg.serialize(buf);
		buf.finish_message();
		this->send_to_server(buf);
	}
	return 0;
}

int Robot_Player::ai_task_submit(void) {
	req_fetch_task_list();
	return 0;
}

int Robot_Player::task_exec_collect(const Task_Mission_Detail_Config *task_cfg) {
	if (! task_cfg) {
		return -1;
	}

	// 采集任务
	wayfind_info_.det_scene_id = task_cfg->execute[0].sceneid;
	wayfind_info_.det_coord.set(task_cfg->execute[0].target_x, task_cfg->execute[0].target_y, Coord::PIXEL_TYPE);
	//wayfind_info_.det_coord.set(2000, 1000, Coord::PIXEL_TYPE);

	if (wayfind_info_.det_scene_id != wayfind_info_.cur_location.scene_id) {
		//wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
		return transfer_scene();
	} else {
		bool find_out = false;
		Coord src = wayfind_info_.cur_location.coord.grid_coord();
		Coord det = wayfind_info_.det_coord.grid_coord();
		//if (wayfind_info_.cur_location.coord == wayfind_info_.det_coord) {
		if (src == det) {
			// 采集
			MSG_DEBUG("  %s 采集时 npc_appear_map_.size: %d.   ", base_detail_.account.c_str(), npc_appear_map_.size());
			for (std::map<role_id_t, NPC_Appear_Info>::const_iterator it_npc_appear = npc_appear_map_.begin();
					it_npc_appear != npc_appear_map_.end(); ++it_npc_appear) {
				//int npc_type = it_npc_appear->second.npc_type;
				//int talk = task_cfg->execute[0].talk;
				if (4000000 < it_npc_appear->first && it_npc_appear->first < 5000000 &&
						it_npc_appear->second.npc_type == (int)task_cfg->execute[0].talk) {

					if (it_npc_appear->second.add_id.empty()) {
						// 不指定归属采集物
						find_out = true;
						MSG_10210100 msg;
						msg.material_role_id = it_npc_appear->first;
						Block_Buffer buf;
						buf.make_client_message(REQ_START_GATHER, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
						msg.serialize(buf);
						buf.finish_message();
						this->send_to_server(buf);
						break;

					} else {
						// 指定归属采集物
						for (size_t i_add_id = 0; i_add_id < it_npc_appear->second.add_id.size(); ++i_add_id) {
							if (base_detail_.role_id == it_npc_appear->second.add_id[i_add_id]) {
								find_out = true;
								MSG_10210100 msg;
								msg.material_role_id = it_npc_appear->first;
								Block_Buffer buf;
								buf.make_client_message(REQ_START_GATHER, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
								msg.serialize(buf);
								buf.finish_message();
								this->send_to_server(buf);
								break;
							}
						}
					}
				}
			}

			base_detail_.status = LOOP_COLLECTING;
			base_detail_.loop_collecting = 1;

		} else {
//				if (! find_out) {
			//path_find();
			//base_detail_.status = Robot_Detail::MOVE;
			path_find_move();
		}
	}

	return 0;
}

int Robot_Player::task_exec_fb(const Task_Mission_Detail_Config *task_cfg) {
	if (! task_cfg) {
		return -1;
	}

	// 完成副本任务
	wayfind_info_.det_scene_id = task_cfg->execute[0].id;
	if (wayfind_info_.det_scene_id != wayfind_info_.cur_location.scene_id) {
		//wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
		return transfer_scene();
	} else {
		if (base_detail_.walked_mosters_points) {
			if (! in_main_scene()) {
				gm_finish_task(task_info_.id);
				exit_diff_space();
			}
			return 0;
		}

		// 找怪
		const Monster_Section_Config* monster_section_cfg = CONFIG_CACHE_NPC->get_monster_section_config(wayfind_info_.cur_location.scene_id);
		if (!monster_section_cfg || monster_section_cfg->point_map.empty()) {
			return 0;
		}
		if (base_detail_.need_walk_monster_points.empty()) {
			for (Monster_Point_Config_Map::const_iterator it_cfg_point_map = monster_section_cfg->point_map.begin();
					it_cfg_point_map != monster_section_cfg->point_map.end(); ++it_cfg_point_map) {
				int key = it_cfg_point_map->first;
				if (key >= 100) {
					continue;
				}
				base_detail_.need_walk_monster_points.insert(std::make_pair(it_cfg_point_map->first, it_cfg_point_map->second));
			}
		}

		MSG_DEBUG(">>>> 需要清理的怪点还剩: %d个", base_detail_.need_walk_monster_points.size());
		//std::vector<Coord> vec_monster_coord;
		//vec_monster_coord.push_back(base_detail_.need_walk_monster_points.begin()->second.birth_coord);

		if (wayfind_info_.path.empty()) {	// !vec_monster_coord.empty() &&
			wayfind_info_.det_coord = base_detail_.need_walk_monster_points.begin()->second.birth_coord;
			wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
			///path_find();
			//base_detail_.status = Robot_Detail::MOVE;
			path_find_move();
		}

		if (! wayfind_info_.path.empty()) {
			base_detail_.status = Robot_Detail::MOVE;
		} else {
			base_detail_.status = Robot_Detail::FB_WAYFINDING;
			base_detail_.loop_fb_wayfinding = 1;
//			gm_finish_task(task_info_.id);
//			exit_diff_space();
		}
	}

	return 0;
}

int Robot_Player::task_exec_arrive(const Task_Mission_Detail_Config *task_cfg) {
	if (! task_cfg) {
		return -1;
	}

	wayfind_info_.det_scene_id = task_cfg->execute[0].sceneid;
	wayfind_info_.det_coord.set(task_cfg->execute[0].target_x, task_cfg->execute[0].target_y, Coord::PIXEL_TYPE);
	//wayfind_info_.det_coord.set(2000, 1000, Coord::PIXEL_TYPE);

	if (wayfind_info_.det_scene_id != wayfind_info_.cur_location.scene_id) {
		//wayfind_info_.det_scene_id = wayfind_info_.cur_location.scene_id;
		if (task_cfg->execute[0].id &&
				task_cfg->execute[0].id != task_cfg->execute[0].sceneid &&
				task_cfg->execute[0].id != wayfind_info_.cur_location.scene_id) {
			wayfind_info_.det_scene_id = task_cfg->execute[0].id;
			wayfind_info_.det_coord.reset();
		}
		return transfer_scene();
	} else {
		Coord src = wayfind_info_.cur_location.coord.grid_coord();
		Coord det = wayfind_info_.det_coord.grid_coord();
		if (src == det) {
			char _char[100];
			memset(_char, 0x00, sizeof(_char));
			snprintf(_char, sizeof(_char), "task 7 %d", task_info_.id);
			MSG_10299999 logic_msg;
			logic_msg.reset();
			logic_msg.command = _char;
			Block_Buffer buf;
			buf.make_client_message(REQ_LOGIC_TEST_COMMAND, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
			logic_msg.serialize(buf);
			buf.finish_message();
			this->send_to_server(buf);

			MSG_DEBUG("   %s 到达了指定NPC(scene:%d,x=%d,y=%d)，提交了任务:%d.   ", base_detail_.account.c_str(),
					wayfind_info_.cur_location.scene_id, wayfind_info_.cur_location.coord.x, wayfind_info_.cur_location.coord.y, task_info_.id);
		} else {
			//path_find();
			//base_detail_.status = Robot_Detail::MOVE;
			path_find_move();
		}
	}

	return 0;
}

bool Robot_Player::reach_dest_grid(void) {
	return wayfind_info_.cur_location.coord.grid_coord() == wayfind_info_.det_coord.grid_coord();
}

bool Robot_Player::in_main_scene(void) {
	return wayfind_info_.cur_location.scene_id == 80100001 ||
			wayfind_info_.cur_location.scene_id == 80100002 ||
			wayfind_info_.cur_location.scene_id == 80100003;
}

int Robot_Player::exit_diff_space(void) {
	//MSG_USER("%s 退出异空间 source:%d ", base_detail().account.c_str(), wayfind_info().cur_location.scene_id);

	MSG_10400005 msg;
	Block_Buffer buf;
	buf.make_client_message(10400005, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg.serialize(buf);
	buf.finish_message();
	send_to_server(buf);

	base_detail_.walked_mosters_points = false;

	MSG_10200030 msg_complete;
	buf.reset();
	buf.make_client_message(10200030, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg_complete.serialize(buf);
	buf.finish_message();
	send_to_server(buf);

	base_detail_.status = Robot_Detail::WAITING_EXIT_DIFF_SPACE;

	base_detail_.loop_exit_diff_space = 1;

	return 0;
}

void Robot_Player::handle_clear_monster_point(void) {
	MSG_USER("Handle_clear_monster_point");

	base_detail_.status = Robot_Detail::IDLE;
	base_detail_.sub_status = 0;
	if (! base_detail_.need_walk_monster_points.empty()) {
		base_detail_.need_walk_monster_points.erase(base_detail_.need_walk_monster_points.begin());
		if (base_detail_.need_walk_monster_points.empty()) {
			base_detail_.walked_mosters_points = true;
			MSG_DEBUG(">>>> 需要清理的怪点还剩: %d个", base_detail_.need_walk_monster_points.size());
		}
	}

	// send scene complete
//	MSG_USER("%s >>> 通知服务端场景加载完成 cur_scene:%d", base_detail().account.c_str(), wayfind_info().cur_location.scene_id);
//	MSG_10200030 msg;
//	Block_Buffer buf;
//	buf.make_client_message(REQ_SCENE_COMPLETE, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
//	msg.serialize(buf);
//	buf.finish_message();
//	send_to_server(buf);
}

int Robot_Player::transfer_scene(void) {
	if(! wayfind_info().det_scene_id) {
		return exit_diff_space();
	}

	const Scene_Config *des_scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(wayfind_info().det_scene_id);
	if (! des_scene_cfg) {
		return -1;
	}
	const Scene_Config *src_scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(wayfind_info().cur_location.scene_id);
	if (! src_scene_cfg) {
		return -1;
	}
	if (src_scene_cfg->type != Scene_Type::Main_Scene_Type && task_info_.id != 10100101) {
		// TODO
		if (src_scene_cfg->dungeon.relation <= 0) {
			return exit_diff_space();
		} else {
			if (src_scene_cfg->dungeon.relation != wayfind_info().det_scene_id) {
				return exit_diff_space();
			}
		}
	}

	MSG_USER("%s >< 开始转场景 source:%d =======> to:%d", base_detail().account.c_str(), wayfind_info().cur_location.scene_id, wayfind_info().det_scene_id);
	base_detail_.status = Robot_Detail::INNER_OPING;
	MSG_10400005 msg;
	msg.scene_id = wayfind_info().det_scene_id;
	Block_Buffer buf;
	buf.make_client_message(10400005, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg.serialize(buf);
	buf.finish_message();
	send_to_server(buf);
	MSG_USER("%s >< 转场景end", base_detail().account.c_str());

	MSG_10200030 msg_complete;
	buf.reset();
	buf.make_client_message(10200030, msg_detail().msg_serial++, Time_Value::gettimeofday().sec());
	msg_complete.serialize(buf);
	buf.finish_message();
	send_to_server(buf);

	//wayfind_info_.cur_location.scene_id = wayfind_info_.det_scene_id;
	//path_find();
	base_detail_.status = Robot_Detail::TURN_SCENES;
	base_detail_.loop_transfer_scene = 1;

	return 0;
}


int Robot_Player::npc_appear(const NPC_Appear_Info &appear_info) {
	if (! appear_info.role_id) {
		return -1;
	}
	npc_appear_map_.insert(std::make_pair(appear_info.role_id, appear_info));
	return 0;
}

int Robot_Player::npc_disappear(Role_Vec &role_id_vec) {
	if (role_id_vec.empty()) {
		return -1;
	}
	for (uint16_t i = 0; i < role_id_vec.size(); ++i) {
		if (npc_appear_map_.count(role_id_vec[i])) {
			npc_appear_map_.erase(role_id_vec[i]);
		}
	}
	return 0;
}
