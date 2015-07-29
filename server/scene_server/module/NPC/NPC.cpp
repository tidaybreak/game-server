/*
 * NPC.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */


#include "NPC.h"
#include "Config_Cache.h"
#include "NPC_Manager.h"
#include "ai_machine/AI_Machine.h"
#include "Msg_Active_Struct.h"
#include "Scene_Monitor.h"
#include "Move_Scene.h"

NPC::NPC(void) {
	reset();
}

NPC::~NPC(void) { }

role_id_t NPC::role_id(void)
{
	return npc_id();
}

int NPC::level(void){
	return npc_detail_.fighter_detail_.level;
}

std::string NPC::role_name(void){
	return "";
}

void NPC::set_role_name(const std::string& name){

}

int NPC::career(void)
{
	return 0;
}

int NPC::sign_in(void) {

	return this->move_appear();
}

int NPC::sign_out(void) {
	return this->move_disappear();
}

int NPC::targger_condition(int type, Fighter *fighter1) {
	return 0;
}


int NPC::start_ai(Time_Value tick) {
//	if (!npc_detail_.ai_config) return 0;
//
//	npc_detail_.watcher_tick.clear();
//	if (tick <=  Time_Value::zero) {
//		this->push_watcher_tick(ai_config().tick_time);
//		if (ai_config().tick_time <=  Time_Value::zero) {
//			MSG_USER("error tick time");
//		}
//		//tick = ai_config().tick_time;
//		Time_Value first_tick = Time_Value(0, 1000);
////		MAP_TIMER_INSTANCE->watcher().add(this, Epoll_Watcher::EVENT_TIMEOUT | Epoll_Watcher::EVENT_ONCE_TIMER, &first_tick);
//	} else {
//		this->push_watcher_tick(tick);
////		MAP_TIMER_INSTANCE->watcher().add(this, Epoll_Watcher::EVENT_TIMEOUT | Epoll_Watcher::EVENT_ONCE_TIMER, &tick);
//	}
//
//	npc_detail_.is_ai_start = true;
//	return  0;

	if (npc_record() == NULL){
		return 0;
	}
	if (tick <=  Time_Value::zero) {
		this->push_watcher_tick(0,10000);
	} else {
		this->push_watcher_tick(tick);
	}
	npc_detail_.is_ai_start = true;
	return 0;
}

int NPC::stop_ai(void) {
	npc_detail_.is_ai_start = false;
//	return MAP_TIMER_INSTANCE->watcher().remove(this);
	return 0;
}

int NPC::fini(void) {
	//this->stop_ai();
	npc_detail_.is_ai_start = false;
	fini_time_ = Time_Value::gettimeofday();
	return 0;
}

int NPC::tick(Time_Value &now) {
//	gauge_grid_coord_tick(now);
//	if(npc_detail_.check_ai_tick){
//		if(now > npc_detail_.ai_tick_time){
//			if(npc_status() != FIGHT_STATUS && npc_status() != PLOT_STATUS){
//				bev_tree_root_.update(this);
//			}
//			vision_tick();
//			this->pop_watcher_tick();
//		}
//	}else{
//		if(npc_status() != FIGHT_STATUS && npc_status() != PLOT_STATUS){
//			bev_tree_root_.update(this);
//		}
//		vision_tick();
//	}
	return 0;
}

int NPC::vision_tick(void){
	return 0;
}

int NPC::exit_move_scene(bool offline_exit){
	if(recover_state()){
		return -1;
	}
	Fighter::exit_move_scene(offline_exit);
	npc_record_ = NULL;
	npc_detail_.npc_status = RECOVER_STATUS;
	return 0;
}


int NPC::ai_config_init(void) {
	return 0;
}

void NPC::reset(void) {
	Fighter::reset();

	npc_detail_.reset();
	npc_record_ = NULL;
	//bev_tree_root_;

	fini_time_ = Time_Value::zero;
	disappear_time_ = Time_Value::zero;

	leader_id_ = 0;
	team_coord_pos = 0;
	for(int i=0; i<MAX_TEAMMATE_NUM; ++i)
	{
		teammate_id_[i] = 0;
	}
	teammate_count_ = 0;

	move_interval[0] = Time_Value::zero;
	move_interval[1] = Time_Value::zero;
	move_interval[2] = Time_Value::zero;

	hatred_map_.clear();
	most_hatred_role_.reset();
	gauge_grid_coord_.reset();
	gauge_coord_vec_.clear();
	gauge_tick_time_ = Time_Value::zero;

	cur_gt_times_ = 0;
	cfg_max_gt_times_ = 0;
	role_collected.clear();
}

int NPC::init_bt(void) {
	/// ai init
	AI_Data * ai_data = AI_MACHINE->find_ai_behavior(npc_detail().ai_id);
	if (ai_data != NULL) {
		bev_tree_root_.install(*ai_data->ai_behavior);
		ai_config_init();
	} else {
		bev_tree_root_.install(*AI_MACHINE->normal_ai());
	}

	return 0;
}

int NPC::active_npc(void) {
	if (move_scene() == NULL || npc_id() == 0) {
		MSG_USER("ERROR_9:npc id:%ld fini time:%ld", npc_id(), fini_time_.sec());
		return -1;
	}

	if (npc_detail_.is_ai_start) {
		MSG_USER("re active npc");
		return -1;
	}

	NPC::start_ai();

	return 0;
}

int NPC::deactive_npc(void) {
	if (move_scene() == NULL || npc_id() == 0) {
		MSG_USER("ERROR_9:npc id:%ld fini time:%ld", npc_id(), fini_time_.sec());
		return -1;
	}

	stop_ai();

	return 0;
}


int NPC::respond_error_result(int msg_id, int err, Block_Buffer *buf) {
	return 0;
}

int NPC::respond_finer_result(int msg_id, Block_Buffer *buf) {
	return 0;
}

int NPC::pop_watcher_tick(Time_Value &tv) {
	npc_detail_.check_ai_tick = false;
	return 0;
}

int NPC::pop_watcher_tick(void) {
	npc_detail_.check_ai_tick = false;
	return 0;
}

int NPC::push_watcher_tick(long sec, long usec) {
	npc_detail_.ai_tick_time = Time_Value::gettimeofday() + Time_Value(sec, usec);
	npc_detail_.check_ai_tick = true;
	return 0;
}

int NPC::push_watcher_tick(const Time_Value &tv) {
	npc_detail_.ai_tick_time = Time_Value::gettimeofday() + tv;
	npc_detail_.check_ai_tick = true;
	return 0;
}

int NPC::push_watcher_tick(Time_Value &tv) {
	npc_detail_.ai_tick_time = Time_Value::gettimeofday() + tv;
	npc_detail_.check_ai_tick = true;
	return 0;
}

int NPC::handle_timeout(const Time_Value &tv) {
	if (npc_id() == 0) {
		MSG_USER("npc timeout error");
		return 0;
	}
//	return MAP_MANAGER_INSTANCE->push_npc_ai_role_id(npc_id());
	return 0;
}

bool NPC::is_valid_coord(Coord grid_coord) {
	return true;
}

role_id_t NPC::npc_id(void) {
	return npc_detail_.npc_id;
}

int NPC::npc_type_id(void) {
	return npc_detail_.npc_type_id;
}

int NPC::npc_operation_type(void){
	return npc_detail_.operation_type;
}

void NPC::set_npc_operation_type(const int value){
	npc_detail_.operation_type = value;
}

uint8_t NPC::npc_operation_state(void){
	return npc_detail_.operation_state;
}

void NPC::set_npc_operation_state(uint8_t value){
	npc_detail_.operation_state = value;
	broad_operation_state();
}

void NPC::broad_operation_state(void){
	if(move_scene() != NULL){
		MSG_81000013 msg;
		msg.role_id = role_id();
		msg.state = npc_operation_state();
		Block_Buffer buf;
		MAKE_MSG_BUF(msg, buf);
		move_scene()->broadcast_around(0, mover_detail().block_index, buf);
	}
}

void NPC::hook_gather_act(void){
	if(npc_detail_.state_type == 1 && npc_detail_.operation_state != 10){
		set_npc_operation_state(10);
	}
}

int NPC::npc_status(void) {
	return npc_detail_.npc_status;
}

bool NPC::ai_status(void) {
	return npc_detail_.is_ai_start;
}


bool NPC::is_npc(void) {
	return true;
}

bool NPC::is_monster(void) {
	return false;
}

bool NPC::is_material(void) {
	return false;
}

bool NPC::is_machine(void) {
	return false;
}

bool NPC::is_special_npc(void) {
	return false;
}

bool NPC::is_drops(void) {
	return false;
}

bool NPC::is_hero(void) {
	return false;
}

bool NPC::is_player_monster(void){
	return false;
}

Time_Value &NPC::birth_time(void) {
	return npc_detail_.birth_time;
}

Coord &NPC::birth_coord(void) {
	return npc_detail_.birth_coord;
}

//const AI_Info &NPC::ai_config(void) {
//	return *npc_detail_.ai_config;
//}

AI_Running_Info &NPC::ai_running_info(void) {
	return npc_detail_.ai_running_info;
}

NPC_Record *NPC::npc_record(void) {
	return npc_record_;
}

void NPC::set_record(NPC_Record *record) {
	npc_record_ = record;
}
//
//int NPC::die_process(const Attack_Info &info, Fighter &killer) {
//	npc_detail_.npc_status = DEAD_STATUS;
//
//	// 通知逻辑服怪物死亡，不限值是否被玩家杀死
//	Block_Buffer buf;
//	buf.make_message(INNER_NPC_DIE_NOFITY);
//	buf.write_int64(role_id());
//	buf.write_int64(master_id());
//	buf.write_int64(info.attacker);
//	buf.write_int32(npc_type_id());
//	buf.write_int32(scene_id());
//	buf.finish_message();
//	MAP_MANAGER_INSTANCE->send_to_logic_server(buf);
//
//	/// erase alive map
//	npc_record()->remove_npc_from_alive(npc_type_id(), npc_id());
//
//	if (!ai_status()) {
//		start_ai();
//	}
//
//	pop_watcher_tick();
//	push_watcher_tick(0, 1000);
//
//	return 0;
//}

int NPC::normal_die_self(void) {
	//if (!npc_record_) {
	//	MSG_USER("ERROR_9 npc has recover");
	//	return -1;
	//}

	if (npc_status() == RECOVER_STATUS || npc_status() == DEAD_STATUS) return -1;

//	Attack_Info info;
//	die_process(info, *this);

	return 0;
}

int NPC::recover_self(void) {
	if (npc_detail_.npc_status == RECOVER_STATUS) {
		MSG_DEBUG("npc is recover status");
		return 0;
	}
	if (!ai_status()) {
		start_ai();
	}

	disappear_time_ = Time_Value::gettimeofday();

	if (npc_record())
	{
		npc_record()->remove_npc_from_alive(npc_type_id(), npc_id());
	}
	exit_move_scene();
	return 0;
}

void NPC::set_npc_status(int npc_status) {
	if (npc_detail_.npc_status == RECOVER_STATUS) {
		MSG_USER_TRACE("error npc set_status in recover_status");
		return ;
	}
	npc_detail_.npc_status = npc_status;
}

bool NPC::recover_state(void) {
	if (npc_detail_.npc_status == RECOVER_STATUS){
		return true;
	}
	return false;
}

bool NPC::npc_dead_state(void) {
	if (npc_detail_.npc_status == DEAD_STATUS){
		return true;
	}
	return false;
}

bool NPC::dormancy_state(void) {
	if (npc_detail_.npc_status == DORMANCY_STATUS){
		return true;
	}
	return false;
}

role_id_t NPC::master_id(void)  {
	return npc_detail_.direct_master;
}

int NPC::refresh_cache_config(void) {
//	npc_detail_.ai_config = CONFIG_CACHE->find_ai_config_cache(npc_detail_.ai_id);
//
//	if (npc_detail_.ai_config == NULL)
//		return -1;

	return 0;
}

int NPC::move_inner(int x, int y)
{
	Coord pre_grid_coord = this->grid_coord();
	Coord coord(grid_x_to_pixel(x), grid_y_to_pixel(y), Coord::PIXEL_TYPE);
	Coord_Vec coord_vec;
	coord_vec.push_back(coord);
	int result = Mover::move(coord_vec);
	if(result == 0){
		Coord pre_coord;
		int count = gauge_coord_vec_.size();
		if(count <= 0){
			gauge_grid_coord_ = pre_grid_coord;
			Time_Value now = Time_Value::gettimeofday();
			//gauge_tick_time_ = now + move_interval[0];
			gauge_tick_time_ = now;
			pre_coord = pre_grid_coord;
		}else{
			pre_coord = gauge_coord_vec_[count-1].coord;
		}
		Gauge_Coord gauge_coord;
		gauge_coord.coord = coord.grid_coord();
		int toward = move_scene()->calculate_toward_by_grid_coord(pre_coord.x, pre_coord.y, gauge_coord.coord.x, gauge_coord.coord.y);
		if (toward == Move_Scene::toward_left || toward == Move_Scene::toward_right) {
			gauge_coord.time = move_interval[0];
		} else if (toward == Move_Scene::toward_up || toward == Move_Scene::toward_down) {
			gauge_coord.time = move_interval[1];
		} else {
			gauge_coord.time = move_interval[2];
		}
		this->gauge_coord_vec_.push_back(gauge_coord);
	}
	return result;
}

int NPC::move_inner_vec(Coord_Vec coord_vec){
	Coord pre_grid_coord = this->grid_coord();
	int result = Mover::move(coord_vec);
	if(result == 0){
		Coord pre_coord;
		int count = gauge_coord_vec_.size();
		if(count <= 0){
			gauge_grid_coord_ = pre_grid_coord;
			Time_Value now = Time_Value::gettimeofday();
			//gauge_tick_time_ = now + move_interval[0];
			gauge_tick_time_ = now;
			pre_coord = pre_grid_coord;
		}else{
			pre_coord = gauge_coord_vec_[count-1].coord;
		}
		Coord_Vec::iterator it;
		for(it = coord_vec.begin(); it != coord_vec.end(); ++it){
			Gauge_Coord gauge_coord;
			gauge_coord.coord = it->grid_coord();
			int toward = move_scene()->calculate_toward_by_grid_coord(pre_coord.x, pre_coord.y, gauge_coord.coord.x, gauge_coord.coord.y);
			if (toward == Move_Scene::toward_left || toward == Move_Scene::toward_right) {
				gauge_coord.time = move_interval[0];
			} else if (toward == Move_Scene::toward_up || toward == Move_Scene::toward_down) {
				gauge_coord.time = move_interval[1];
			} else {
				gauge_coord.time = move_interval[2];
			}
			pre_coord = gauge_coord.coord;
			this->gauge_coord_vec_.push_back(gauge_coord);
		}
	}
	return result;
}

//int NPC::path_find(Coord &src_coord, Coord &dst_coord, Coord_Vec &path, bool mover, bool obstacle) {
//	if (! validate_mover_detail())
//		return -1;
//	if (! validate_movable_by_coord(dst_coord.x, dst_coord.y))
//		return -2;
//	return scene_->path_find(mover_detail_.layer_id, src_coord, dst_coord, path, mover_detail_.moveable_value, mover, obstacle);
//	return 0;
//}
int NPC::path_find(const Coord &src_coord, Coord &dst_coord, Coord_Vec &path, bool mover, bool obstacle)
{
	int ret = -1;
	if(this->validate_mover_scene())
	{
		int move_able_id = move_scene()->mpt_value_by_coord(dst_coord.x, dst_coord.y);
		if(move_able_id < Move_Scene::MOVE_ABLE_FROM || move_able_id > Move_Scene::MOVE_ABLE_END){
			return -1;
		}
		Coord src_coord1 = src_coord;
		Int_Int moveable;
		ret = move_scene()->path_find(mover_detail().layer_id, src_coord1, dst_coord, path, moveable, mover, obstacle);
	}
	return ret;
}

void NPC::set_leader_id(role_id_t id)
{
	leader_id_ = id;
}

role_id_t NPC::get_leader_id(void)
{
	return leader_id_;
}

void NPC::add_teammate(role_id_t id)
{
	for(int i=0; i<MAX_TEAMMATE_NUM; ++i)
	{
		if( teammate_id_[i] == 0)
		{
			teammate_id_[i] = id;
			++teammate_count_;
			break;
		}
	}
}

void NPC::del_all_teammate(void)
{
	for(int i=0; i<MAX_TEAMMATE_NUM; ++i)
	{
		teammate_id_[i] = 0;
	}
	teammate_count_ = 0;
}

int NPC::get_move_inner_coord(Coord& src_coord, Coord& available_coord)
{
	if(this->validate_mover_scene())
	{
		int layer_id = this->mover_detail().layer_id;
		return move_scene()->collect_available_coord(layer_id, src_coord, available_coord, 1, 2);
	}
	return -1;
}

void NPC::set_move_speed(int value)
{
	npc_detail_.move_speed = value;
	refresh_move_interval();
}

int NPC::get_move_speed(void)
{
	return npc_detail_.move_speed;
}

void NPC::refresh_move_interval(void)
{
	if(get_move_speed() >= 25)
	{
		double x_interval, y_interval, s_interval;

		double time_speed = get_move_speed(); 					// 每毫秒移动多少像素
		double frames_speed = time_speed/interval; 			// 每帧移动像素

//		x_interval = (1000.0 / interval) * ceil(grid_width / round(frames_speed)); // x向移动一格所需毫秒
//		y_interval = (1000.0 / interval) * ceil(grid_height / round(frames_speed)); // y向移动一格所需毫秒
//		s_interval = (1000.0 / interval) * ceil(grid_s / round(frames_speed)); // 斜向移动一格所需毫秒

		x_interval = (1000.0 / interval) * (10 / frames_speed); // x向移动一格所需毫秒
		y_interval = (1000.0 / interval) * (grid_height / frames_speed); // y向移动一格所需毫秒
		s_interval = (1000.0 / interval) * (grid_s / frames_speed); // 斜向移动一格所需毫秒
		if (x_interval/1000 < 0 || x_interval * 1000 <= 0) {
			MSG_USER_TRACE("ERROR_9 error speed:%d x_interval:%ld", get_move_speed(), x_interval);
			return ;
		}

		if (y_interval/1000 < 0 || y_interval * 1000 <= 0) {
			MSG_USER_TRACE("ERROR_9 error speed:%d y_interval:%ld", get_move_speed(), y_interval);
			return ;
		}

		if (s_interval/1000 < 0 || s_interval * 1000 <= 0) {
			MSG_USER_TRACE("ERROR_9 error speed:%d s_interval:%ld", get_move_speed(), s_interval);
			return ;
		}

		move_interval[0].set(x_interval/1000, x_interval * 1000);
		move_interval[1].set(y_interval/1000, y_interval * 1000);
		move_interval[2].set(s_interval/1000, s_interval * 1000);
	}
	else
	{
		move_interval[0] = Time_Value::zero;
		move_interval[1] = Time_Value::zero;
		move_interval[2] = Time_Value::zero;
	}
}


Hatred_Map &NPC::hatred_map(void) {
	return hatred_map_;
}

Hatred_Info &NPC::most_hatred_role(void) {
	return most_hatred_role_;
}

int NPC::find_hatred_map(const role_id_t &role_id, Hatred_Info &hatred_info) {
	Hatred_Map::iterator iter = hatred_map_.find(role_id);
	if (iter != hatred_map_.end()) {
		hatred_info = iter->second;
		return 0;
	}

	return -1;
}

void NPC::del_all_hatred(bool self, bool other) {
	if (other) {
//		Fighter *fighter = NULL;
//		for (Hatred_Map::iterator it = hatred_map_.begin(); it != hatred_map_.end(); ++it) {
//			fighter = get_fighter_pointer(it->second.role_id);
//			if (fighter && fighter != this) {
//				fighter->not_be_hatred(this->role_id());
//			}
//		}
	}
	if (self) {
		hatred_map_.clear();
		most_hatred_role_.reset();
		//enter_fight_timestamp_ = Time_Value::zero;
		//hatred_lock_timestamp_ = Time_Value::zero;
	}
}

void NPC::set_talk_vec(std::vector<Int_Int>& talk_vec){
	this->npc_detail_.talk_vec = talk_vec;
}

void NPC::set_chat_vec(std::vector<Int_Int>& chat_vec){
	this->npc_detail_.chat_vec = chat_vec;
}

int NPC::get_talk_id(void){
	if(this->npc_detail_.talk_vec.empty()){
		return 0;
	}
	int count = this->npc_detail_.talk_vec.size();
	for(int i=0; i<count; ++i){
		if(random()%1000 < this->npc_detail_.talk_vec[i].val_2){
			return this->npc_detail_.talk_vec[i].val_1;
		}
	}
	int index = random()%count;
	return this->npc_detail_.talk_vec[index].val_1;
}

int NPC::get_chat_id(void){
	if(this->npc_detail_.chat_vec.empty()){
		return 0;
	}
	int rad_value = random()%1000;
	int total_value = 0;
	int count = this->npc_detail_.chat_vec.size();
	for(int i=0; i<count; ++i){
		total_value += npc_detail_.chat_vec[i].val_2;
		if(total_value > rad_value){
			return this->npc_detail_.chat_vec[i].val_1;
		}
	}
	return 0;
}

void NPC::get_team_coord(const Coord &target, Coord &team_coord){
	int count = this->npc_detail_.team_count;
	int pos = this->npc_detail_.team_pos;
	if(count > MAX_TEAMMATE_NUM || pos > MAX_TEAMMATE_NUM){
		team_coord = target;
	}
	team_coord.x = target.x + TEAM_POS_X[count][pos];
	team_coord.y = target.y + TEAM_POS_Y[count][pos];

	uint16_t pixel_x = grid_x_to_pixel(team_coord.x);
	uint16_t pixel_y = grid_y_to_pixel(team_coord.y);
	if(validate_move_value(pixel_x, pixel_y) == false)
	{
		team_coord = target;
	}
}
void NPC::set_team_info(int count, int pos){
	this->npc_detail_.team_pos = pos;
	this->npc_detail_.team_count = count;
}


void NPC::retify_gauge_grid_coord(void){
	this->gauge_grid_coord_ = this->grid_coord();
	gauge_coord_vec_.clear();
	gauge_tick_time_ = Time_Value::zero;
}

Coord& NPC::gauge_grid_coord(void){
	return this->gauge_grid_coord_;
}

int NPC::get_gauge_coord_num(void){
	int num = gauge_coord_vec_.size();
	return num;
}

// gt times beg -----------
int NPC::get_cur_gt_times(void) {// 当前采集次数
	return cur_gt_times_;
}

int NPC::add_cur_gt_times(const int times) {
	if (times > 0) {
		cur_gt_times_ += times;
	}
	return cur_gt_times_;
}

int NPC::get_cfg_max_gt_times(void) {// 配置最大采集次数
	return cfg_max_gt_times_;
}

int NPC::set_cfg_max_gt_times(const int times) {
	if (times > 0) {
		cfg_max_gt_times_ += times;
	} else {
		cfg_max_gt_times_ = 1;
	}
	return cfg_max_gt_times_;
}
// gt times end ----------------

bool NPC::had_collect(const role_id_t role_id) {
	return 0 != role_collected.count(role_id);
}

void NPC::add_role_collect(const Role_Hash_Set role_set) {
	role_collected.insert(role_set.begin(), role_set.end());
	this->cur_gt_times_ = role_collected.size();
}

void NPC::clear_role_collect(void) {
	role_collected.clear();
	this->cur_gt_times_ = 0;
}

void NPC::gauge_grid_coord_tick(const Time_Value& now){
	if(this->gauge_coord_vec_.empty()){
		return;
	}
	if(now > gauge_tick_time_){
		Gauge_Coord_Vec::iterator it = gauge_coord_vec_.begin();
		gauge_grid_coord_ = it->coord.grid_coord();
		Gauge_Coord_Vec::iterator it_next = gauge_coord_vec_.erase(it);
		if(it_next == gauge_coord_vec_.end()){//移动完成，重新纠正位置
			this->retify_gauge_grid_coord();
		}else{
			gauge_tick_time_ = gauge_tick_time_ + it_next->time;
		}
	}
}
