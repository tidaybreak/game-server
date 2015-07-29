/*
 * Move_Scene_Base.cpp
 *
 *  Created on: May 11, 2012
 *      Author: Ti
 */

#include "Move_Scene_Base.h"
#include "fighter/Fighter.h"
#include "mover/Mover_Struct.h"
#include "Config_Cache_Scene.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"
#include "Scene_Player.h"

Move_Scene_Base::Move_Scene_Base(void)
: scene_id_(0),
  scene_order_(0),
  map_id_(0),
  scene_config_id_(0),
  scene_config_(0),
  block_width_(0),
  block_height_(0),
  block_x_number_(0),
  block_y_number_(0),
  mpt_data_(0),
  log_scene_id_(0)
{
	init_toward_offset();
}

Move_Scene_Base::~Move_Scene_Base(void) { }

const std::pair<int, int> Move_Scene_Base::toward_null_offset(0, 0);
const std::pair<int, int> Move_Scene_Base::toward_up_offset(0, -1);
const std::pair<int, int> Move_Scene_Base::toward_right_up_offset(1, -1);
const std::pair<int, int> Move_Scene_Base::toward_right_offset(1, 0);
const std::pair<int, int> Move_Scene_Base::toward_right_down_offset(1, 1);
const std::pair<int, int> Move_Scene_Base::toward_down_offset(0, 1);
const std::pair<int, int> Move_Scene_Base::toward_left_down_offset(-1, 1);
const std::pair<int, int> Move_Scene_Base::toward_left_offset(-1, 0);
const std::pair<int, int> Move_Scene_Base::toward_left_up_offset(-1, -1);

void Move_Scene_Base::init_toward_offset(void) {
	toward_offset_vec_.clear();

	Toward_Offset toward_offset;

	/// toward_null
	toward_offset.reset();
	toward_offset_vec_.push_back(toward_offset);

	/// toward_up 上
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_left_down_offset);
	toward_offset.disappear_coord.push_back(toward_down_offset);
	toward_offset.disappear_coord.push_back(toward_right_down_offset);

	toward_offset.move_coord.push_back(toward_left_up_offset);
	toward_offset.move_coord.push_back(toward_up_offset);
	toward_offset.move_coord.push_back(toward_right_up_offset);
	toward_offset.move_coord.push_back(toward_left_offset);
	toward_offset.move_coord.push_back(toward_null_offset);
	toward_offset.move_coord.push_back(toward_right_offset);

	toward_offset.appear_coord.push_back(toward_left_up_offset);
	toward_offset.appear_coord.push_back(toward_up_offset);
	toward_offset.appear_coord.push_back(toward_right_up_offset);

	toward_offset_vec_.push_back(toward_offset);

	/// toward_right_up 右上
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_left_up_offset);
	toward_offset.disappear_coord.push_back(toward_left_offset);
	toward_offset.disappear_coord.push_back(toward_left_down_offset);
	toward_offset.disappear_coord.push_back(toward_down_offset);
	toward_offset.disappear_coord.push_back(toward_right_down_offset);

	toward_offset.move_coord.push_back(toward_up_offset);
	toward_offset.move_coord.push_back(toward_right_up_offset);
	toward_offset.move_coord.push_back(toward_null_offset);
	toward_offset.move_coord.push_back(toward_right_offset);

	toward_offset.appear_coord.push_back(toward_left_up_offset);
	toward_offset.appear_coord.push_back(toward_up_offset);
	toward_offset.appear_coord.push_back(toward_right_up_offset);
	toward_offset.appear_coord.push_back(toward_right_offset);
	toward_offset.appear_coord.push_back(toward_right_down_offset);

	toward_offset_vec_.push_back(toward_offset);

	/// toward_right 右
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_left_up_offset);
	toward_offset.disappear_coord.push_back(toward_left_offset);
	toward_offset.disappear_coord.push_back(toward_left_down_offset);

	toward_offset.move_coord.push_back(toward_up_offset);
	toward_offset.move_coord.push_back(toward_right_up_offset);
	toward_offset.move_coord.push_back(toward_null_offset);
	toward_offset.move_coord.push_back(toward_right_offset);
	toward_offset.move_coord.push_back(toward_down_offset);
	toward_offset.move_coord.push_back(toward_right_down_offset);

	toward_offset.appear_coord.push_back(toward_right_up_offset);
	toward_offset.appear_coord.push_back(toward_right_offset);
	toward_offset.appear_coord.push_back(toward_right_down_offset);

	toward_offset_vec_.push_back(toward_offset);

	/// toward_right_down 右下
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_left_down_offset);
	toward_offset.disappear_coord.push_back(toward_left_offset);
	toward_offset.disappear_coord.push_back(toward_left_up_offset);
	toward_offset.disappear_coord.push_back(toward_up_offset);
	toward_offset.disappear_coord.push_back(toward_right_up_offset);

	toward_offset.move_coord.push_back(toward_null_offset);
	toward_offset.move_coord.push_back(toward_right_offset);
	toward_offset.move_coord.push_back(toward_down_offset);
	toward_offset.move_coord.push_back(toward_right_down_offset);

	toward_offset.appear_coord.push_back(toward_right_up_offset);
	toward_offset.appear_coord.push_back(toward_right_offset);
	toward_offset.appear_coord.push_back(toward_right_down_offset);
	toward_offset.appear_coord.push_back(toward_down_offset);
	toward_offset.appear_coord.push_back(toward_left_down_offset);

	toward_offset_vec_.push_back(toward_offset);

	/// toward_down 下
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_left_up_offset);
	toward_offset.disappear_coord.push_back(toward_up_offset);
	toward_offset.disappear_coord.push_back(toward_right_up_offset);

	toward_offset.move_coord.push_back(toward_left_offset);
	toward_offset.move_coord.push_back(toward_null_offset);
	toward_offset.move_coord.push_back(toward_right_offset);
	toward_offset.move_coord.push_back(toward_left_down_offset);
	toward_offset.move_coord.push_back(toward_down_offset);
	toward_offset.move_coord.push_back(toward_right_down_offset);

	toward_offset.appear_coord.push_back(toward_left_down_offset);
	toward_offset.appear_coord.push_back(toward_down_offset);
	toward_offset.appear_coord.push_back(toward_right_down_offset);

	toward_offset_vec_.push_back(toward_offset);

	/// toward_left_down 左下
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_left_up_offset);
	toward_offset.disappear_coord.push_back(toward_up_offset);
	toward_offset.disappear_coord.push_back(toward_right_up_offset);
	toward_offset.disappear_coord.push_back(toward_right_offset);
	toward_offset.disappear_coord.push_back(toward_right_down_offset);

	toward_offset.move_coord.push_back(toward_left_offset);
	toward_offset.move_coord.push_back(toward_null_offset);
	toward_offset.move_coord.push_back(toward_left_down_offset);
	toward_offset.move_coord.push_back(toward_down_offset);

	toward_offset.appear_coord.push_back(toward_left_up_offset);
	toward_offset.appear_coord.push_back(toward_left_offset);
	toward_offset.appear_coord.push_back(toward_left_down_offset);
	toward_offset.appear_coord.push_back(toward_down_offset);
	toward_offset.appear_coord.push_back(toward_right_down_offset);

	toward_offset_vec_.push_back(toward_offset);

	/// toward_left 左
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_right_up_offset);
	toward_offset.disappear_coord.push_back(toward_right_offset);
	toward_offset.disappear_coord.push_back(toward_right_down_offset);

	toward_offset.move_coord.push_back(toward_left_up_offset);
	toward_offset.move_coord.push_back(toward_up_offset);
	toward_offset.move_coord.push_back(toward_left_offset);
	toward_offset.move_coord.push_back(toward_null_offset);
	toward_offset.move_coord.push_back(toward_left_down_offset);
	toward_offset.move_coord.push_back(toward_down_offset);

	toward_offset.appear_coord.push_back(toward_left_up_offset);
	toward_offset.appear_coord.push_back(toward_left_offset);
	toward_offset.appear_coord.push_back(toward_left_down_offset);

	toward_offset_vec_.push_back(toward_offset);

	/// toward_left_up 左上
	toward_offset.reset();

	toward_offset.disappear_coord.push_back(toward_right_up_offset);
	toward_offset.disappear_coord.push_back(toward_right_offset);
	toward_offset.disappear_coord.push_back(toward_right_down_offset);
	toward_offset.disappear_coord.push_back(toward_down_offset);
	toward_offset.disappear_coord.push_back(toward_left_down_offset);

	toward_offset.move_coord.push_back(toward_left_up_offset);
	toward_offset.move_coord.push_back(toward_up_offset);
	toward_offset.move_coord.push_back(toward_left_offset);
	toward_offset.move_coord.push_back(toward_null_offset);

	toward_offset.appear_coord.push_back(toward_left_down_offset);
	toward_offset.appear_coord.push_back(toward_left_offset);
	toward_offset.appear_coord.push_back(toward_left_up_offset);
	toward_offset.appear_coord.push_back(toward_up_offset);
	toward_offset.appear_coord.push_back(toward_right_up_offset);

	toward_offset_vec_.push_back(toward_offset);
}

int Move_Scene_Base::init_scene_normal(int scene_id, int scene_order, int map_id) {
	scene_config_ = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (scene_config_) {
		scene_name_ = scene_config_->scene_name;
		scene_config_id_ = scene_id;
	} else {
		MSG_TRACE_ABORT("scene:%d order:%d", scene_id, scene_order);
		return -1;
	}
	map_id = scene_config_->map_id ? scene_config_->map_id : scene_id;

	scene_id_ = scene_id;

	map_id_ = map_id;
	scene_order_ = scene_order;

	mpt_data_ = CONFIG_CACHE_SCENE->scene_map_data_cache(map_id);

	if (!mpt_data_) {
		MSG_TRACE_ABORT("load mpt error:%d", map_id);
		return -1;
	}
	init_map_block();

	init_save_area(scene_id);
	init_move_value();

	return 0;
}

int Move_Scene_Base::client_scene_complete(Fighter &fighter) {
	update_move_able_info(&fighter);
	return 0;
}

int Move_Scene_Base::update_move_able_info(Mover* mover){
	mover->clear_moveable_value();
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		if((*it).move_value.empty() == false){
			MSG_81000022 msg;
			Move_Value::iterator s_it;
			for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
				if(s_it->val_2 == MOVE_ABLE_FROM){
					msg.moveable_info_vec.push_back(s_it->val_1);
					int16_t value = (*s_it).val_1;
					mover->add_moveable_value(value);
				}
			}
			OBJ_SEND_TO_CLIENT(msg, (*mover));
		}
		break;
	}
	return 0;
}

void Move_Scene_Base::init_scene_layer(size_t layer_id, Scene_Layer &scene_layer) {
	scene_layer.layer_id_ = layer_id;
	scene_layer.layer_flag_ = Scene_Layer::USED_LAYER;
	scene_layer.mover_map_.clear();
	scene_layer.grid_mover_count_.resize(mpt_data_->content.size(), 0);
	scene_layer.grid_npc_count_.resize(mpt_data_->content.size(), 0);
	scene_layer.grid_player_count_.resize(mpt_data_->content.size(), 0);
	scene_layer.scene_block_vec_.clear();
	scene_layer.next_flush_ts_ = Time_Value::zero;
	scene_layer.relive_bead_use.clear();
	scene_layer.move_value.clear();

	int block_sum = block_x_number_ * block_y_number_;
	Scene_Block scene_block;
	for (int i = 0; i < block_sum; ++i) {
		scene_block.reset();
		scene_block.block_idx_ = i;
		scene_layer.scene_block_vec_.push_back(scene_block);
	}
}

int Move_Scene_Base::init_map_block(void) {
	/// 初始化九宫大小, 数量

	block_width_ = scene_config_->scene_block_x ? scene_config_->scene_block_x : mpt_data_->head.x_len;
	block_height_ = scene_config_-> scene_block_y ? scene_config_-> scene_block_y : mpt_data_->head.y_len;

	if (mpt_data_->head.x_len < block_width_)
		block_width_ = mpt_data_->head.x_len;

	if (mpt_data_->head.y_len < block_height_)
		block_height_ = mpt_data_->head.y_len;

	block_x_number_ = (mpt_data_->head.x_len / block_width_) + (mpt_data_->head.x_len % block_width_ > 0 ? 1 : 0);
	block_y_number_ = (mpt_data_->head.y_len / block_height_) + (mpt_data_->head.y_len % block_height_ > 0 ? 1 : 0);

	/// 初始化首层(公共层)
	Scene_Layer scene_layer;
	init_scene_layer(0, scene_layer);

	scene_layer_vec_.clear();
	scene_layer_vec_.push_back(scene_layer);

	used_layer_set_.insert(scene_layer.layer_id_);

	return 0;
}

int Move_Scene_Base::init_save_area(int scene_id) {
	/*
	const Json::Value &client_scene_json = CONFIG_CACHE->client_scene_conf(scene_id);

	if (client_scene_json["peace"].asInt()) return 0;

	safe_area_.clear();
	if (client_scene_json["safe_area"] != Json::Value::null) {
		for (uint i = 0; i < client_scene_json["safe_area"].size(); ++i) {
			Safe_Data safe_data;
			safe_data.coord.x = client_scene_json["safe_area"][i]["coord"][0u].asInt();
			safe_data.coord.y = client_scene_json["safe_area"][i]["coord"][1u].asInt();
			safe_data.distance = client_scene_json["safe_area"][i]["distance"].asInt();
			safe_data.range_type = client_scene_json["safe_area"][i]["range_type"].asInt();

			if (safe_data.distance <= 0 || safe_data.coord.x <= 0 || safe_data.coord.y <= 0) continue;
			safe_area_.push_back(safe_data);
		}
	} else {

	}
	*/

	return 0;
}

int Move_Scene_Base::init_move_value(void) {
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		(*it).move_value = scene_config_->move_value;
	}

	return 0;
}

int Move_Scene_Base::change_move_value(int16_t mp_value, bool moveable){
	int val_2 = UNMOVE_ABLE_VAL;
	if(moveable){
		val_2 = MOVE_ABLE_FROM;
	}
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		bool exist = false;
		for(Move_Value::iterator s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
			if((*s_it).val_1 == mp_value){
				exist = true;
				(*s_it).val_2 = val_2;
				break;
			}
		}
		if(exist == false){
			Int_Int d_int;
			d_int.val_1 = mp_value;
			d_int.val_2 = val_2;
			(*it).move_value.push_back(d_int);
		}
		//
		for(Mover_Map::iterator player_it = (*it).mover_map_.begin();
				player_it != (*it).mover_map_.end(); ++player_it){
			if(moveable){
				player_it->second->add_moveable_value(mp_value);
			}else{
				player_it->second->del_moveable_value(mp_value);
			}
			if(player_it->second->player_self()){
				MSG_81000022 msg;
				Move_Value::iterator s_it;
				for(s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
					if(s_it->val_2 == MOVE_ABLE_FROM){
						msg.moveable_info_vec.push_back(s_it->val_1);
					}
				}
				OBJ_SEND_TO_CLIENT(msg, (*player_it->second));
			}
		}
	}

	return 0;
}

int Move_Scene_Base::tick(const Time_Value &now) {

	return 0;
}

void Move_Scene_Base::flush_tick(const Time_Value &now) {
	for (Scene_Layer_ID_Set::iterator layer_it = used_layer_set_.begin(); layer_it != used_layer_set_.end(); ++layer_it) {
		Scene_Layer &scene_layer = scene_layer_vec_[*layer_it];
		if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
			MSG_USER("FREE_LAYER");
			continue;
		}
		if (now > scene_layer.next_flush_ts_) {
			for (Scene_Block_Vec::iterator block_it = scene_layer.scene_block_vec_.begin(); block_it != scene_layer.scene_block_vec_.end(); ++block_it) {
				block_it->flush();
			}
		}
	}
}

void Move_Scene_Base::force_flush_tick(size_t layer_id, uint block_index) {
	if (this->validate_layer_id(layer_id)) {
		Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
		if (block_index < scene_layer.scene_block_vec_.size())
			scene_layer.scene_block_vec_[block_index].flush(false);
		else
			MSG_USER("error block index:%d  %d", block_index, scene_layer.scene_block_vec_.size());
	}
}

size_t Move_Scene_Base::pop_layer(void) {
	size_t layer_id = 0;
	if (free_layer_set_.empty()) {
		layer_id = add_new_scene_layer();
	} else {
		Scene_Layer_ID_Set::iterator first_it = free_layer_set_.begin();
		layer_id = *first_it;
		free_layer_set_.erase(first_it);
		used_layer_set_.insert(layer_id);
		scene_layer_vec_[layer_id].layer_flag_ = Scene_Layer::USED_LAYER;
	}
	return layer_id;
}

int Move_Scene_Base::push_layer(size_t layer_id) {
	if (layer_id >= scene_layer_vec_.size()) {
		MSG_USER("layer_id = %d, scene_layer_vec_.size = %d", layer_id, scene_layer_vec_.size());
		return -1;
	}
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER");
		return -1;
	}
	scene_layer.layer_flag_ = Scene_Layer::FREE_LAYER;
	scene_layer.mover_map_.clear();
	scene_layer.next_flush_ts_ = Time_Value::zero;
	used_layer_set_.erase(layer_id);
	free_layer_set_.insert(layer_id);
	return 0;
}

size_t Move_Scene_Base::add_new_scene_layer(void) {
	size_t layer_id = used_layer_set_.size();
	Scene_Layer scene_layer;
	init_scene_layer(layer_id, scene_layer);
	used_layer_set_.insert(layer_id);
	scene_layer_vec_.push_back(scene_layer);
	return layer_id;
}

int Move_Scene_Base::enter_scene(Mover *mover) {
	mover->clear_moveable_value();
	for (Scene_Layer_Vec::iterator it = scene_layer_vec_.begin(); it != scene_layer_vec_.end(); ++it) {
		for(Move_Value::iterator s_it = (*it).move_value.begin(); s_it != (*it).move_value.end(); ++s_it){
			if((*s_it).val_2 == MOVE_ABLE_FROM){
				int16_t value = (*s_it).val_1;
				mover->add_moveable_value(value);
			}
		}
		break;
	}

	return 0;
}


int Move_Scene_Base::exit_scene(Mover *, bool offline) {

	return 0;
}

int Move_Scene_Base::appear(Mover *mover) {
	if (!mover || mover->role_id() == 0) {
		MSG_USER(" == 0, error");
		return -1;
	}

	const Mover_Detail &detail = mover->mover_detail();

	if (! validate_grid_coord(detail.grid_coord.x, detail.grid_coord.y)) {
		MSG_USER("invalid grid_x = %d, grid_y = %d", detail.grid_coord.x, detail.grid_coord.y);
		return -1;
	}

	if (! validate_layer_id(detail.layer_id)) {
		MSG_USER("invalid layer_id:%d try to layer:0", detail.layer_id);
		mover->set_layer_id(0);
	}

	// get scene layer
	Scene_Layer &scene_layer = scene_layer_vec_[detail.layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER, role:%ld", mover->role_id());
		return ERROR_LAYER_ID;
	}

	// block_index异常
	uint16_t check_block_index = 0;
	calculate_block_index_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, check_block_index);
	if (check_block_index != detail.block_index) {
		MSG_USER("coord error, role:%ld, x = %d, y = %d, block_index:%d, check_block_index:%d", mover->role_id(), detail.grid_coord.x, detail.grid_coord.y, detail.block_index, check_block_index);
		scene_layer.scene_block_vec_[detail.block_index].unbind_mover(mover);
		mover->set_block_index(check_block_index);
	}

	Block_Buffer appear_me_buf, appear_other_buf(0, 4 * 1024);

	// 通知周围区块“我”出现
	Int_Vec around;
	calculate_around_block_by_block_index_s(detail.block_index, around);
	// 周围有玩家才构造自身出现消息
	if (! no_player_around(detail.layer_id, around)) {
		mover->move_appear_info(appear_me_buf);
	}
	bool is_player = detail.mover_type == MOVER_T_PLAYER ? true : false;
	// 自己出现消息发给自己
	if (is_player) {
		mover->move_appear_info(appear_other_buf);
	}
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		if (appear_me_buf.readable_bytes() > 0)
			scene_layer.scene_block_vec_[*around_it].push_data(appear_me_buf); /// 通知其他人屏幕显示"我"
		if (is_player)
			scene_layer.scene_block_vec_[*around_it].get_block_appear_msg(appear_other_buf);
	}

	// 通知"我的屏幕"显示周围区块Mover信息
	if (is_player && appear_other_buf.readable_bytes() > 0)
		mover->send_to_client(appear_other_buf);

	scene_layer.scene_block_vec_[detail.block_index].bind_mover(mover);
	scene_layer.mover_map_[mover->role_id()] = mover;

	/// 记录格子重叠Mover
	uint32_t grid_index = 0;
	calculate_grid_index_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, grid_index);
	++scene_layer.grid_mover_count_[grid_index];
	if (is_player) {
		++scene_layer.grid_player_count_[grid_index];
	} else {
		++scene_layer.grid_npc_count_[grid_index];
	}

	return 0;
}

int Move_Scene_Base::disappear(Mover* mover) {
	if (!mover || mover->role_id() == 0) {
		MSG_USER(" == 0, error");
		return -1;
	}
	const Mover_Detail &detail = mover->mover_detail();

	if (! validate_grid_coord(detail.grid_coord.x, detail.grid_coord.y)) {
		MSG_USER("invalid grid_x = %d, grid_y = %d", detail.grid_coord.x, detail.grid_coord.y);
		return -1;
	}

	if (! validate_layer_id(detail.layer_id)) {
		MSG_USER("invalid layer_id:%d try to layer:0", detail.layer_id);
		return -1;
	}

	Scene_Layer &scene_layer = scene_layer_vec_[detail.layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER, role:%ld", mover->role_id());
		return ERROR_LAYER_ID;
	}

	// 防止客户端没发场景加载完成就发移动
	if (scene_layer.mover_map_.find(mover->role_id()) == scene_layer.mover_map_.end()) {
		MSG_USER_TRACE("not in map role:%ld", mover->role_id());
		return ERROR_MOVE_NOT_IN_SCENE;
	}

	// block_index异常
	uint16_t check_block_index = 0;
	calculate_block_index_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, check_block_index);
	if (check_block_index != detail.block_index) {
		MSG_USER("coord error, role:%ld, x = %d, y = %d, block_index:%d, check_block_index:%d", mover->role_id(), detail.grid_coord.x, detail.grid_coord.y, detail.block_index, check_block_index);
		scene_layer.scene_block_vec_[detail.block_index].unbind_mover(mover);
		mover->set_block_index(check_block_index);
		return -1;
	}

	bool is_player = detail.mover_type == MOVER_T_PLAYER ? true : false;
	/// 把所在块的数据发给该玩家
	if (is_player) {
		scene_layer.scene_block_vec_[detail.block_index].flush_to_mover(*mover);
	}

	scene_layer.scene_block_vec_[detail.block_index].unbind_mover(mover);
	scene_layer.mover_map_.erase(mover->role_id());

	Block_Buffer disappear_me_buf, disappear_other_buf;
	mover->make_disappear_info(disappear_me_buf);
	disappear_other_buf.make_message(81000010);
	int old_wri_idx = disappear_other_buf.get_write_idx();
	disappear_other_buf.write_uint16(0);
	uint16_t disappear_amount = 0;

	Int_Vec around;
	if (calculate_around_block_by_block_index(detail.block_index, around) < 0) {
		MSG_USER("block_index = %d, error", detail.block_index);
		return -1;
	}
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		scene_layer.scene_block_vec_[*around_it].push_data(disappear_me_buf);
		if (is_player)
			disappear_amount += scene_layer.scene_block_vec_[*around_it].get_block_disappear_msg(disappear_other_buf);
	}
	if (is_player && disappear_amount) {
		int new_wri_idx = disappear_other_buf.get_write_idx();
		disappear_other_buf.set_write_idx(old_wri_idx);
		disappear_other_buf.write_uint16(disappear_amount);
		disappear_other_buf.set_write_idx(new_wri_idx);
		disappear_other_buf.finish_message();
		mover->send_to_client(disappear_other_buf);
	}

	uint32_t grid_index = 0;
	calculate_grid_index_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, grid_index);
	--scene_layer.grid_mover_count_[grid_index];
	if (is_player) {
		--scene_layer.grid_player_count_[grid_index];
	} else {
		--scene_layer.grid_npc_count_[grid_index];
	}

	return 0;
}

int Move_Scene_Base::move(Mover *mover, uint16_t x, uint16_t y, int coord_flag) {
	if (!mover ) {
		MSG_USER(" == 0, error");
		return -1;
	}

	const Mover_Detail &detail = mover->mover_detail();

	// get scene_layer
	Scene_Layer &scene_layer = scene_layer_vec_[detail.layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER, role:%ld", mover->role_id());
		return ERROR_LAYER_ID;
	}

	// 防止客户端没发场景加载完成就发移动
	if (scene_layer.mover_map_.find(mover->role_id()) == scene_layer.mover_map_.end()) {
		MSG_USER_TRACE("not in map role:%ld", mover->role_id());
		return ERROR_MOVE_NOT_IN_SCENE;
	}

	// block_index异常
	uint16_t new_block_index = 0;
	if (! validate_block_index(detail.block_index)) {
		calculate_block_index_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, new_block_index);
		MSG_USER("invalidate block_index:%d, new_block_index:%d", detail.block_index, new_block_index);
		mover->set_block_index(new_block_index);
	}

	// 如果是像素坐标，将像素(pix)坐标转换为格子(grid)坐标
	uint16_t new_grid_x = x, new_grid_y = y;
	if (coord_flag == pix_coord_flag) {
		calculate_grid_coord_by_pix_coord_s(x, y, new_grid_x, new_grid_y);
	}

	// 计算格子下标
	uint32_t old_grid_index = 0, new_grid_index = 0;
	calculate_grid_index_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, old_grid_index);
	calculate_grid_index_by_grid_coord_s(new_grid_x, new_grid_y, new_grid_index);

	// 计算朝向
	int toward = detail.toward;
	if (detail.grid_coord.x == new_grid_x && detail.grid_coord.y == new_grid_y) {
		//MSG_USER_TRACE("same coord x = %d, y = %d", new_grid_x, new_grid_y);
		//return -1;
	} else  {
		toward = calculate_toward_by_grid_coord(detail.grid_coord.x, detail.grid_coord.y, new_grid_x, new_grid_y);
	}

	// 计算区块坐标
	uint16_t old_block_x = 0, old_block_y = 0, new_block_x = 0, new_block_y = 0;
	calculate_block_coord_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, old_block_x, old_block_y);
	calculate_block_coord_by_grid_coord_s(new_grid_x, new_grid_y, new_block_x, new_block_y);

	// 是否玩家移动
	bool is_player = detail.mover_type == MOVER_T_PLAYER ? true : false;

	if (old_block_x == new_block_x && old_block_y == new_block_y) { // 未跨区快
		// 通知周围玩家“我”移动
		Block_Buffer msg_buf;
		mover->set_grid_coord(new_grid_x, new_grid_y);
		mover->set_pixel_coord(x, y);
		mover->make_move_info(msg_buf, toward, x, y);

		std::vector<int> around;
		calculate_around_block_by_block_coord_s(old_block_x, old_block_y, around);

		for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
			scene_layer.scene_block_vec_[*around_it].push_data(msg_buf);
		}

		--scene_layer.grid_mover_count_[old_grid_index];
		++scene_layer.grid_mover_count_[new_grid_index];
		if (is_player) {
			--scene_layer.grid_player_count_[old_grid_index];
			++scene_layer.grid_player_count_[new_grid_index];
		} else {
			--scene_layer.grid_npc_count_[old_grid_index];
			++scene_layer.grid_npc_count_[new_grid_index];
		}

	} else { /// 跨区快
		Coord_Set disappear_set, move_set, appear_set;

		calculate_diff_block_broadcast_by_block_coord(old_block_x, old_block_y, new_block_x, new_block_y,
				disappear_set, move_set, appear_set);

		{ /// 发送消失消息的区块
			Block_Buffer disappear_me_buf, disappear_other_buf;
			mover->make_disappear_info(disappear_me_buf);
			disappear_other_buf.make_message(81000010);
			int old_wri_idx = disappear_other_buf.get_write_idx();
			disappear_other_buf.write_uint16(0);
			uint16_t disappear_amount = 0;
			for (Coord_Set::iterator around_it = disappear_set.begin(); around_it != disappear_set.end(); ++around_it) {
				scene_layer.scene_block_vec_[*around_it].push_data(disappear_me_buf);
				if (is_player)
					disappear_amount += scene_layer.scene_block_vec_[*around_it].get_block_disappear_msg(disappear_other_buf);
			}
			if (is_player && disappear_amount) {
				int new_wri_idx = disappear_other_buf.get_write_idx();
				disappear_other_buf.set_write_idx(old_wri_idx);
				disappear_other_buf.write_uint16(disappear_amount);
				disappear_other_buf.set_write_idx(new_wri_idx);
				disappear_other_buf.finish_message();
				mover->send_to_client(disappear_other_buf); /// tell me screen disappear other
			}
		}

		{ /// 发送移动消息的区块
			Block_Buffer move_buf;
			mover->set_grid_coord(new_grid_x, new_grid_y);
			mover->set_pixel_coord(x, y);
			mover->make_move_info(move_buf, toward, x, y);

			for (Coord_Set::iterator it = move_set.begin(); it != move_set.end(); ++it) {
				scene_layer.scene_block_vec_[*it].push_data(move_buf);
			}
		}

		{ /// 发送出现消息的区块
			Block_Buffer self_appear_buf, other_appear_buf(0, 8 * 1024);
			mover->move_appear_info(self_appear_buf);
			for (Coord_Set::iterator around_it = appear_set.begin(); around_it != appear_set.end(); ++around_it) {
				scene_layer.scene_block_vec_[*around_it].push_data(self_appear_buf);
				if (is_player)
					scene_layer.scene_block_vec_[*around_it].get_block_appear_msg(other_appear_buf);
			}
			if (is_player && other_appear_buf.readable_bytes() > 0)
				mover->send_to_client(other_appear_buf);
		}

		{ /// 把原来块的数据发给该玩家
			if (is_player) {
				scene_layer.scene_block_vec_[detail.block_index].flush_to_mover(*mover);
			}
		}

		{ /// 改变所在区块
			--scene_layer.grid_mover_count_[old_grid_index];
			++scene_layer.grid_mover_count_[new_grid_index];
			if (is_player) {
				--scene_layer.grid_player_count_[old_grid_index];
				++scene_layer.grid_player_count_[new_grid_index];
			} else {
				--scene_layer.grid_npc_count_[old_grid_index];
				++scene_layer.grid_npc_count_[new_grid_index];
			}

			uint16_t new_block_index;
			calculate_block_index_by_block_coord_s(new_block_x, new_block_y, new_block_index);

			scene_layer.scene_block_vec_[detail.block_index].unbind_mover(mover);
			mover->set_block_index(new_block_index);
			scene_layer.scene_block_vec_[detail.block_index].bind_mover(mover);
		}
	}

	return 0;
}

bool Move_Scene_Base::validate_move_coord(int coord_flag, int x, int y) {
	if (coord_flag == grid_coord_flag) {
		if (! validate_grid_coord(x, y)) {
			MSG_USER("grid_coord error, grid_x = %d, grid_y = %d.", x, y);
			return false;
		}
	} else if (coord_flag ==  pix_coord_flag) {
		if (! validate_pix_coord(x, y)) {
			MSG_USER("pix_coord error, pix_x = %d, pix_y = %d.", x, y);
			return false;
		}
	} else {
		MSG_USER("unknow error, coord_flag = %d, x = %d, y = %d.", coord_flag, x, y);
		return false;
	}

	return true;
}

bool Move_Scene_Base::get_rand_coord(Coord &coord) {
	int rand_x = random()%mpt_data_->head.x_len;
	int rand_y = random()%mpt_data_->head.y_len;

	uint32_t grid_index = 0;
	calculate_grid_index_by_grid_coord_s(rand_x, rand_y, grid_index);
	if (this->mpt_data_->content[grid_index] >= MOVE_ABLE_FROM) {
		coord.x = rand_x * mpt_data_->head.cell_width;
		coord.y = rand_y * mpt_data_->head.cell_height;
		return true;
	}

	return false;
}


static int horizontal_weight = 20;		/// 水平方向权值
static int vertical_weight = 10;		/// 垂直方向权值
static int diagonal_weight = 22;		/// 对角线方向权值

int calculate_h_value(const Coord &src_coord, const Coord &dst_coord) {
	return 10 * (std::abs(src_coord.x - dst_coord.x) * horizontal_weight
			+ std::abs(src_coord.y - dst_coord.y) * vertical_weight);
}

int calculate_relative_g_value(const Coord &src_coord, const Coord &dst_coord) {
	int toward_index = 1;
	for (; toward_index < 9; ++toward_index) {
		if (src_coord.x + toward_direction[toward_index][0] == dst_coord.x
				&& src_coord.y + toward_direction[toward_index][1] == dst_coord.y)
			break;
	}
	if (toward_index == 9) {
		MSG_USER("not border coord, src_coord.grid_x = %d, src_coord.grid_y = %d, "
				"dst_coord.grid_x = %d, dst_coord.grid_y = %d.", src_coord.x, src_coord.x,
				dst_coord.y, dst_coord.y);
		return -1;
	}
	switch(toward_index) {
	case Move_Scene_Base::toward_up:
		return vertical_weight;
	case Move_Scene_Base::toward_right_up:
		return diagonal_weight;
	case Move_Scene_Base::toward_right:
		return horizontal_weight;
	case Move_Scene_Base::toward_right_down:
		return diagonal_weight;
	case Move_Scene_Base::toward_down:
		return vertical_weight;
	case Move_Scene_Base::toward_left_down:
		return diagonal_weight;
	case Move_Scene_Base::toward_left:
		return horizontal_weight;
	case Move_Scene_Base::toward_left_up:
		return diagonal_weight;
	default: {
		MSG_USER("toward_index = %d, error", toward_index);
		break;
	}
	}
	return 0;
}


int Move_Scene_Base::path_find_sight_line(size_t layer_id, Coord &src_coord,
		Coord &dst_coord,
		Coord_Vec &path,
		Int_Int &moveable_value,
		bool mover,
		bool obstacle) {
	int dx = dst_coord.x - src_coord.x;
	int dy = dst_coord.y - src_coord.y;
	int max_step = std::max(std::abs(dx), std::abs(dy));
	double x_inc = static_cast<double>(dx) / static_cast<double>(max_step);
	double y_inc = static_cast<double>(dy) / static_cast<double>(max_step);

	Coord_Vec path_res;
	Coord node = src_coord;

	double px = static_cast<double>(src_coord.x);
	double py = static_cast<double>(src_coord.y);

	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER");
		return -1;
	}
	for (int i = 0; i < max_step; ++i) {
		px += x_inc;
		py += y_inc;

		node.x = ((px > 0) ? static_cast<int>(px + 0.5) : static_cast<int>(px - 0.5));
		node.y = ((py > 0) ? static_cast<int>(py + 0.5) : static_cast<int>(py - 0.5));

		if (obstacle) {
			int8_t mpt_value = this->mpt_data_->content[node.y * mpt_data_->head.x_len + node.x];
			if (mpt_value < Move_Scene_Base::MOVE_ABLE_FROM || (mpt_value >= Move_Scene_Base::MOVE_ABLE_END))
				return -1;
		}

		if (mover && node != dst_coord) {
			uint32_t grid_index = 0;
			calculate_grid_index_by_grid_coord(node.x, node.y, grid_index);
			if (grid_index >=0 && scene_layer.grid_mover_count_[grid_index] > 0)
				return -1;
		}

		path_res.push_back(node);
	}
	for (Coord_Vec::reverse_iterator it = path_res.rbegin(); it != path_res.rend(); ++it) {
		path.push_back(*it);
	}
	return 0;
}

int Move_Scene_Base::path_find_a_star(size_t layer_id, Coord &src_coord,
		Coord &dst_coord,
		Coord_Vec &path,
		Int_Int &moveable_value,
		bool mover,
		bool obstacle,
		int max_find) {
	Path_Node_Set open_list;
	Path_Node_Map closed_list;

	Path_Node begin_node(0, calculate_h_value(src_coord, dst_coord), src_coord, src_coord);
	open_list.insert(begin_node);
	closed_list[src_coord] = begin_node;

	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER");
		return -3;
	}

	int open_list_find = 0;
	uint32_t grid_index = 0;
	while (! open_list.empty()) {
		if (open_list_find > max_find) {
			return -1;
		}

		Path_Node node = *open_list.begin();
		open_list.erase(node);

		closed_list[node.cur_coord] = node;

		if (node.cur_coord == dst_coord) {
			Path_Node back_node = node;
			while (back_node.cur_coord != src_coord) {
				path.push_back(back_node.cur_coord);
				back_node = closed_list[back_node.prev_coord];
			}
			return 0;
		}

		/// 找出node周围的块
		for (int i = 1; i < 9; ++i) {
			Coord coord(node.cur_coord.x + toward_direction[i][0], node.cur_coord.y + toward_direction[i][1]);

			if (closed_list.count(coord)) /// already in closed list
				continue;
			if (coord.x < 0 || coord.x >= mpt_data_->head.x_len || coord.y < 0 || coord.y >= mpt_data_->head.y_len)
				continue;
			if (obstacle) {
				int8_t mpt_value = this->mpt_data_->content[coord.y * mpt_data_->head.x_len + coord.x];
				if (mpt_value < Move_Scene_Base::MOVE_ABLE_FROM || (mpt_value >= Move_Scene_Base::MOVE_ABLE_END))
					continue;
			}
			if (mover && coord != dst_coord) {
				calculate_grid_index_by_grid_coord(coord.x, coord.y, grid_index);
				if (grid_index >=0 && scene_layer.grid_mover_count_[grid_index] > 0) continue;
			}
			bool exist_open_node = false;
			for (Path_Node_Set::iterator it = open_list.begin(); it != open_list.end(); ++it) {
				if (it->cur_coord == coord) { /// 相邻格子已在open_list中
					Path_Node open_node = *it;
					int g_val = node.g_val + calculate_relative_g_value(node.cur_coord, open_node.cur_coord);
					if (g_val < open_node.g_val) {
						open_node.g_val = g_val;
						open_node.prev_coord = node.cur_coord;
						open_list.erase(it);
						open_list.insert(open_node);
					}
					exist_open_node = true;
					break;;
				}
			}
			if (exist_open_node) continue;

			/// 相邻格子未在open_list中
			Path_Node new_node(node.g_val + calculate_relative_g_value(node.cur_coord, coord),
					calculate_h_value(coord, dst_coord),
					coord, node.cur_coord);
			open_list.insert(new_node);
		}

		open_list_find++;
	}

	return 0;
}

bool Move_Scene_Base::path_arrive_a_star(size_t layer_id, Coord &src_coord,
		Coord &dst_coord,
		Int_Int &moveable_value,
		bool mover,
		bool obstacle) {
	Path_Node_Set open_list;
	Path_Node_Map closed_list;

	Path_Node begin_node(0, calculate_h_value(src_coord, dst_coord), src_coord, src_coord);
	open_list.insert(begin_node);
	closed_list[src_coord] = begin_node;

	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER");
		return -1;
	}

	uint32_t grid_index = 0;
	while (! open_list.empty()) {
		Path_Node node = *open_list.begin();
		open_list.erase(node);

		closed_list[node.cur_coord] = node;

		if (node.cur_coord == dst_coord) {
			return true;
		}

		/// 找出node周围的块
		for (int i = 1; i < 9; ++i) {
			Coord coord(node.cur_coord.x + toward_direction[i][0], node.cur_coord.y + toward_direction[i][1]);

			if (closed_list.count(coord)) /// already in closed list
				continue;
			if (coord.x < 0 || coord.x >= mpt_data_->head.x_len || coord.y < 0 || coord.y >= mpt_data_->head.y_len)
				continue;
			if (obstacle) {
				int8_t mpt_value = this->mpt_data_->content[coord.y * mpt_data_->head.x_len + coord.x];
				if (mpt_value < Move_Scene_Base::MOVE_ABLE_FROM || (mpt_value >= Move_Scene_Base::MOVE_ABLE_END))
					continue;
			}
			if (mover && coord != dst_coord) {
				calculate_grid_index_by_grid_coord(coord.x, coord.y, grid_index);
				if (grid_index >=0 && scene_layer.grid_mover_count_[grid_index] > 0) continue;
			}
			bool exist_open_node = false;
			for (Path_Node_Set::iterator it = open_list.begin(); it != open_list.end(); ++it) {
				if (it->cur_coord == coord) { /// 相邻格子已在open_list中
					Path_Node open_node = *it;
					int g_val = node.g_val + calculate_relative_g_value(node.cur_coord, open_node.cur_coord);
					if (g_val < open_node.g_val) {
						open_node.g_val = g_val;
						open_node.prev_coord = node.cur_coord;
						open_list.erase(it);
						open_list.insert(open_node);
					}
					exist_open_node = true;
					break;;
				}
			}
			if (exist_open_node) continue;

			/// 相邻格子未在open_list中
			Path_Node new_node(node.g_val + calculate_relative_g_value(node.cur_coord, coord),
					calculate_h_value(coord, dst_coord),
					coord, node.cur_coord);
			open_list.insert(new_node);
		}
	}

	return false;
}

int Move_Scene_Base::path_find(size_t layer_id, Coord &src_coord,
		Coord &dst_coord,
		Coord_Vec &path,
		Int_Int &moveable_value,
		bool mover,
		bool obstacle) {
	int ret = path_find_sight_line(layer_id, src_coord, dst_coord, path, moveable_value, mover, obstacle);
	if (ret < 0) {
		ret = path_find_a_star(layer_id, src_coord, dst_coord, path, moveable_value, mover, obstacle);
	}
	return ret;
}

int Move_Scene_Base::mover_change_coord(Mover *mover, Coord &new_coord, bool cal_toward/* = true*/) {
	if (!mover ) {
		MSG_USER(" == 0, error.");
		return -1;
	}
	if (! validate_grid_coord(new_coord.x, new_coord.y)) {
		MSG_USER("error coord, grid_x = %d, grid_y = %d.", new_coord.x, new_coord.y);
		return -1;
	}

	const Mover_Detail &detail = mover->mover_detail();

	Scene_Layer &scene_layer = scene_layer_vec_[detail.layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER, role:%ld", mover->role_id());
		return ERROR_LAYER_ID;
	}

	// 防止客户端没发场景加载完成就发移动
	if (scene_layer.mover_map_.find(mover->role_id()) == scene_layer.mover_map_.end()) {
		MSG_USER_TRACE("not in map role:%ld", mover->role_id());
		return ERROR_MOVE_NOT_IN_SCENE;
	}

	// 防止重复坐标
	if (detail.grid_coord.x == new_coord.x && detail.grid_coord.y == new_coord.y) {
		//MSG_USER_TRACE("same coord x = %d, y = %d", new_grid_x, new_grid_y);
		return -1;
	}

	// 把现在包刷出去
	this->force_flush_tick(detail.layer_id, detail.block_index);

	// block_index异常
	if (! validate_block_index(detail.block_index)) {
		MSG_USER("invalidate block_index = %d", detail.block_index);
		uint16_t new_block_index = 0;
		calculate_block_index_by_grid_coord(detail.grid_coord.x, detail.grid_coord.y, new_block_index);
		mover->set_block_index(new_block_index);
	}

	uint16_t old_block_index = detail.block_index;
	uint16_t new_block_index = 0;
	if (calculate_block_index_by_grid_coord(new_coord.x, new_coord.y, new_block_index) != 0) {
		MSG_USER("error coord, grid_x = %d, grid_y = %d.", new_coord.x, new_coord.y);
		return -1;
	}

	uint32_t old_grid_index = 0;
	uint32_t new_grid_index = 0;
	calculate_grid_index_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, old_grid_index);
	calculate_grid_index_by_grid_coord_s(new_coord.x, new_coord.y, new_grid_index);

	if(cal_toward){
		int toward = 0;
		if ((toward = calculate_toward_by_grid_coord(detail.grid_coord.x, detail.grid_coord.y,
				new_coord.x, new_coord.y)) == 0) {
			MSG_USER("error coord, old_x = %d, old_y = %d, new_x = %d, new_y = %d.",
					detail.grid_coord.x,
					detail.grid_coord.y,
					new_coord.x, new_coord.y);
		}
		if (toward != 0)
			mover->set_toward(toward);
	}

	mover->set_block_index(new_block_index);
	mover->set_grid_coord(new_coord.x, new_coord.y);
	mover->set_pixel_coord(grid_x_to_pixel(new_coord.x), grid_y_to_pixel(new_coord.y));
	if (old_block_index != new_block_index) {
		scene_layer.scene_block_vec_[old_block_index].unbind_mover(mover);
		scene_layer.scene_block_vec_[new_block_index].bind_mover(mover);
	}
	bool is_player = detail.mover_type == MOVER_T_PLAYER ? true : false;
	if (old_grid_index != new_grid_index) {
		--scene_layer.grid_mover_count_[old_grid_index];
		++scene_layer.grid_mover_count_[new_grid_index];
		if (is_player) {
			--scene_layer.grid_player_count_[old_grid_index];
			++scene_layer.grid_player_count_[new_grid_index];
		} else {
			--scene_layer.grid_npc_count_[old_grid_index];
			++scene_layer.grid_npc_count_[new_grid_index];
		}
	}
	return 0;
}

int Move_Scene_Base::broadcast_mover_change_coord(Mover *mover,
		const Coord &new_coord,
		Block_Buffer *disappear_me_buf,
		Block_Buffer *move_me_buf,
		Block_Buffer *appear_me_buf) {
	if (!mover ) {
		MSG_USER("arg  == 0, error");
		return -1;
	}

	const Mover_Detail &detail = mover->mover_detail();
	Scene_Layer &scene_layer = scene_layer_vec_[detail.layer_id];
	if (scene_layer.layer_flag_ != Scene_Layer::USED_LAYER) {
		MSG_USER("FREE_LAYER");
		return -1;
	}
	if (! validate_block_index(detail.block_index)) {
		MSG_USER("invalidate block_index = %d", detail.block_index);
		uint16_t new_block_index = 0;
		calculate_block_index_by_grid_coord(detail.grid_coord.x, detail.grid_coord.y, new_block_index);
		mover->set_block_index(new_block_index);
	}

	/// 计算区块坐标
	uint16_t old_block_x = 0, old_block_y = 0, new_block_x = 0, new_block_y = 0;
	calculate_block_coord_by_grid_coord_s(detail.grid_coord.x, detail.grid_coord.y, old_block_x, old_block_y);
	calculate_block_coord_by_grid_coord_s(new_coord.x, new_coord.y, new_block_x, new_block_y);

	uint16_t old_block_index = detail.block_index;
	uint16_t new_block_index = 0;
	calculate_block_index_by_block_coord_s(new_block_x, new_block_y, new_block_index);

	bool is_player = detail.mover_type == MOVER_T_PLAYER ? true : false;
	if (old_block_index == new_block_index) { /// 未跨区快
		/// 通知周围玩家“我”移动
		if (move_me_buf && move_me_buf->readable_bytes() > 0) {
			this->broadcast_around(detail.layer_id, new_block_index, *move_me_buf);
		}
	} else { /// 跨区快
		/// 计算广播区域
		Coord_Set disappear_set, move_set, appear_set;

		calculate_diff_block_broadcast_by_block_coord(old_block_x, old_block_y, new_block_x, new_block_y,
				disappear_set, move_set, appear_set);

		{ /// 发送消失消息的区块
			Block_Buffer disappear_other_buf;
			disappear_other_buf.make_message(81000010);
			int old_wri_idx = disappear_other_buf.get_write_idx();
			disappear_other_buf.write_uint16(0);
			uint16_t disappear_amount = 0;
			for (Coord_Set::iterator disappear_it = disappear_set.begin(); disappear_it != disappear_set.end(); ++disappear_it) {
				if (disappear_me_buf && disappear_me_buf->readable_bytes() > 0)
					scene_layer.scene_block_vec_[*disappear_it].push_data(*disappear_me_buf);
				if (is_player)
					disappear_amount += scene_layer.scene_block_vec_[*disappear_it].get_block_disappear_msg(disappear_other_buf);
			}
			if (is_player && disappear_amount) {
				int new_wri_idx = disappear_other_buf.get_write_idx();
				disappear_other_buf.set_write_idx(old_wri_idx);
				disappear_other_buf.write_uint16(disappear_amount);
				disappear_other_buf.set_write_idx(new_wri_idx);
				disappear_other_buf.finish_message();
				mover->send_to_client(disappear_other_buf); /// tell me screen disappear other
			}
		}

		{ /// 发送移动消息的区块
			if (move_me_buf && move_me_buf->readable_bytes() > 0) {
				for (Coord_Set::iterator move_it = move_set.begin(); move_it != move_set.end(); ++move_it) {
					scene_layer.scene_block_vec_[*move_it].push_data(*move_me_buf);
				}
			}
		}


		{ /// 发送出现消息的区块
			Block_Buffer appear_other_buf;
			for (Coord_Set::iterator appear_it = appear_set.begin(); appear_it != appear_set.end(); ++appear_it) {
				if (appear_me_buf && appear_me_buf->readable_bytes() > 0)
					scene_layer.scene_block_vec_[*appear_it].push_data(*appear_me_buf);
				if (is_player)
					scene_layer.scene_block_vec_[*appear_it].get_block_appear_msg(appear_other_buf);
			}
			if (is_player && appear_other_buf.readable_bytes() > 0)
				mover->send_to_client(appear_other_buf);
		}


		{ /// 把原来块的数据发给该玩家
			if (is_player) {
				scene_layer.scene_block_vec_[old_block_index].flush_to_mover(*mover);
			}
		}
	}

	return 0;
}




int Move_Scene_Base::collect_oval_mover(size_t layer_id, const Coord &coord, Mover_Set &mover, int range, int focus) {
	if (range <= 0) {
		MSG_USER("range <= 0");
		return -1;
	}
	uint16_t block_index;
	if (calculate_block_index_by_grid_coord(coord.x, coord.y, block_index) < 0) {
		MSG_USER("coord error.");
		return -1;
	}

	std::vector<int> around;
	if (calculate_around_block_by_block_index(block_index, around) < 0) {
		MSG_USER("block_index error !\n");
		return -1;
	}
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		Mover_Map &_map = scene_layer.scene_block_vec_[*around_it].block_mover_map_;
		for (Mover_Map::iterator it = _map.begin(); it != _map.end(); ++it) {
			if (is_in_oval(coord, it->second->mover_detail().grid_coord, focus, range)) {
				mover.insert(it->second);
			}
		}
	}
	return 0;
}

int Move_Scene_Base::collect_sector_mover(size_t layer_id, const Coord &orgin, const Coord &arc_mid, int degree, Mover_Set &mover, bool check_around/* = true*/) {
	uint16_t block_index;
	if (calculate_block_index_by_grid_coord(arc_mid.x, arc_mid.y, block_index) < 0) {
		MSG_USER("coord error.");
		return -1;
	}

	std::vector<int> around;
	if (calculate_around_block_by_block_index(block_index, around) < 0) {
		MSG_USER("block_index error !");
		return -1;
	}

	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		Mover_Map &_map = scene_layer.scene_block_vec_[*around_it].block_mover_map_;
		for (Mover_Map::iterator it = _map.begin(); it != _map.end(); ++it) {
			if (is_in_sector(orgin, arc_mid, it->second->mover_detail().grid_coord, degree, check_around)) {
				mover.insert(it->second);
			}
		}
	}

	return 0;
}

int Move_Scene_Base::collect_sector_fight_player(size_t layer_id, const Coord &orgin, const Coord &arc_mid, int degree, Scene_Player_Set &players, bool check_around/* = true*/){
	uint16_t block_index;
	if (calculate_block_index_by_grid_coord(arc_mid.x, arc_mid.y, block_index) < 0) {
		MSG_USER("coord error.");
		return -1;
	}

	std::vector<int> around;
	if (calculate_around_block_by_block_index(block_index, around) < 0) {
		MSG_USER("block_index error !");
		return -1;
	}

	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		Mover_Map &_map = scene_layer.scene_block_vec_[*around_it].block_mover_map_;
		for (Mover_Map::iterator it = _map.begin(); it != _map.end(); ++it) {
			if(it->second->player_self() && it->second->player_self()->can_fight()){
				if (is_in_sector(orgin, arc_mid, it->second->mover_detail().grid_coord, degree, check_around)) {
					players.insert(it->second->player_self());
				}
			}
		}
	}

	return 0;
}

int Move_Scene_Base::calc_point_in_line_by_b(const Mover &_detail, const Coord &pointA, const Coord &pointB, int distance, Coord &pointC, int toward, bool check_move_able, bool from_end_point) {
	Coord pointA_fix = pointA;
	double angle;
	calc_direction_by_a_b(pointB, pointA_fix, angle, toward);
	double dx_cb = distance * cos(angle);
	double dy_cb = distance * sin(angle);
	int real_dx = pixel_x_to_grid(double_to_int(dx_cb));
	int real_dy = pixel_y_to_grid(double_to_int(dy_cb));
	if (pointA_fix.x > pointB.x) {
		real_dx = -real_dx;
	}
	if (pointA_fix.y > pointB.y) {
		real_dy = -real_dy;
	}

	int result = 0;
	if (from_end_point)
		result = calc_point_in_line_by_direction(_detail, pointB, real_dx, real_dy, pointC, check_move_able);
	else
		result = calc_point_in_line_by_direction(_detail, pointA, real_dx, real_dy, pointC, check_move_able);

	return result;

}

// 参考了DDA画线算法
int Move_Scene_Base::calc_point_in_line_by_direction(const Mover &_detail, const Coord &pointStart, int real_dx, int real_dy, Coord &pointC, bool check_move_able) {
	pointC = pointStart;
	Coord temp;
	// 第3点C的坐标，下面要逐步检测是否可走
	temp.x = get_positive_or_zero(pointStart.x + real_dx);
	temp.y = get_positive_or_zero(pointStart.y + real_dy);

	if (!check_move_able) {	// 若无需检测是否可走
		pointC = temp;
		return 0;
	}

	int result = 0;
	/* dx,dy为起点与终点横坐标，纵坐标差量 */
	int dx = 0;
	int dy = 0;
	/* 坐标方向上的增量 */
	double x_inc = 0;
	double y_inc = 0;
	double x, y;
	int steps = 0;

	x = pointStart.x;
	y = pointStart.y;
	dx = temp.x - pointStart.x;
	dy = temp.y - pointStart.y;

	if (abs(dx) > abs(dy)) /* 判断步长1的方向 */
	{
		steps = abs(dx);
	} else {
		steps = abs(dy);
	}
	if (steps == 0) {
		return 0;
	}

	x_inc = (double) dx / (double) steps;
	y_inc = (double) dy / (double) steps;
	for (int i = 0; i < steps; ++i) {
		int mpt = mpt_value_by_coord(double_to_int(x + x_inc), double_to_int(y + y_inc));
		if (mpt < Move_Scene_Base::MOVE_ABLE_FROM || (mpt >= Move_Scene_Base::MOVE_ABLE_END)) {
			result = -1;
			break;
		}

		x += x_inc;
		y += y_inc;
	}

	pointC.x = double_to_int(x);
	pointC.y = double_to_int(y);

	return result;
}

int Move_Scene_Base::calc_point_in_line_by_a(const Mover &_detail, const Coord &pointA, const Coord &pointB, int distance, Coord &pointC, int toward, bool check_move_able) {
	Coord pointA_fix = pointA;
	double angle;
	calc_direction_by_a_b(pointB, pointA_fix, angle, toward);
	double dx_cb = distance * cos(angle);
	double dy_cb = distance * sin(angle);
	int real_dx = pixel_x_to_grid(double_to_int(dx_cb));
	int real_dy = pixel_y_to_grid(double_to_int(dy_cb));
	if (pointA_fix.x > pointB.x) {
		real_dx = -real_dx;
	}
	if (pointA_fix.y > pointB.y) {
		real_dy = -real_dy;
	}

	return calc_point_in_line_by_direction(_detail, pointA, real_dx, real_dy, pointC, check_move_able);
}

int Move_Scene_Base::calc_point_in_line_near_b(const Mover &_detail, const Coord &pointA, const Coord &pointB, Coord &pointC, bool check_move_able) {
	pointC = pointA;
	int length_AB = hypot(get_pixel_width(pointA.x - pointB.x), get_pixel_height(pointA.y - pointB.y));
	if (length_AB < 80) {
		return 0;
	}

	return calc_point_in_line_by_a(_detail, pointA, pointB, length_AB - 50, pointC, check_move_able);
}


void Move_Scene_Base::Scene_Block::reset(void) {
	block_idx_ = 0;
	block_player_map_.clear();
	block_mover_map_.clear();
	block_buf_.reset();
	offset_map_.clear();
}

void Move_Scene_Base::Scene_Block::flush_reset(void) {
	block_buf_.reset();
	offset_map_.clear();
}

void Move_Scene_Base::Scene_Block::flush(bool scene_limit) {
	if (block_buf_.is_write_begin()) return;

	size_t rd_orig = 0;
	Mover_Offset_Map::iterator offset_it;
	for (Mover_Map::iterator player_it = block_player_map_.begin(); player_it != block_player_map_.end(); ++player_it) {
		/// 设置偏移
		offset_it = offset_map_.find(player_it->second->role_id());
		if (offset_it != offset_map_.end()) {
			rd_orig = block_buf_.get_read_idx();
			block_buf_.set_read_idx(offset_it->second);
		}

		// 推数据
		if (block_buf_.readable_bytes() > 0 && player_it->second->move_scene()) {
			if (scene_limit) {
				if (! player_it->second->is_in_battle()) {
					player_it->second->send_to_client(block_buf_);
				}
			} else {
				player_it->second->send_to_client(block_buf_);
			}
		}

		/// 重置偏移
		if (offset_it != offset_map_.end()) {
			block_buf_.set_read_idx(rd_orig);
		}
	}
	flush_reset();
}

void Move_Scene_Base::Scene_Block::flush_to_mover(Mover &mover) {
	if (block_buf_.readable_bytes() <= 0)
		return;

	Mover_Offset_Map::iterator offset_it = offset_map_.find(mover.role_id());
	if (offset_it == offset_map_.end()) {
		mover.send_to_client(block_buf_);
	} else {
		/// 设置偏移
		size_t rd_orig = block_buf_.get_read_idx();
		block_buf_.set_read_idx(offset_it->second);

		/// 推数据
		if (block_buf_.readable_bytes() > 0)
			mover.send_to_client(block_buf_);

		/// 重置偏移
		block_buf_.set_read_idx(rd_orig);
	}
}

bool Move_Scene_Base::Scene_Block::bind_mover(Mover *mover) {
	if (!mover ) {
		MSG_USER(" == 0");
		return false;
	}
	if (! block_mover_map_.insert(std::make_pair(mover->role_id(), mover)).second) {
		MSG_TRACE_ABORT("insert failure role_id = %ld", mover->role_id());
		return false;
	}
	if (mover->mover_type() == MOVER_T_PLAYER) {
		if (! block_player_map_.insert(std::make_pair(mover->role_id(), mover)).second) {
			MSG_TRACE_ABORT("insert failure role_id = %ld", mover->role_id());
			return false;
		}
		offset_map_[mover->role_id()] = block_buf_.get_write_idx();
	}
	return true;
}

bool Move_Scene_Base::Scene_Block::unbind_mover(Mover *mover) {
	if (!mover ) {
		MSG_USER(" == 0");
		return false;
	}
	Mover_Map::iterator find_it = block_mover_map_.find(mover->role_id());
	if (find_it == block_mover_map_.end()) {
		MSG_TRACE_ABORT("erase failure role_id = %ld", mover->role_id());
		return false;
	} else {
		block_mover_map_.erase(mover->role_id());
	}

	if (mover->mover_type() == MOVER_T_PLAYER) {
		Mover_Map::iterator find_it = block_player_map_.find(mover->role_id());
		if (find_it == block_player_map_.end()) {
			MSG_TRACE_ABORT("erase failure role_id = %ld", mover->role_id());
			return false;
		} else {
			block_player_map_.erase(mover->role_id());
		}
	}

	return true;
}

void Move_Scene_Base::Scene_Block::push_data(Block_Buffer &buf) {
	if (! this->block_player_map_.empty()) {
		block_buf_.copy(&buf);
	}
}

void Move_Scene_Base::Scene_Block::get_block_appear_msg(Block_Buffer &buf) {
	for (Mover_Map::iterator it = block_mover_map_.begin(); it != block_mover_map_.end(); ++it) {
		it->second->move_appear_info(buf);
	}
}

int Move_Scene_Base::Scene_Block::get_block_disappear_msg(Block_Buffer &buf) {
	int disappear_amount = 0;
	for (Mover_Map::iterator it = block_mover_map_.begin(); it != block_mover_map_.end(); ++it) {
		buf.write_int64(it->second->role_id());
		disappear_amount++;
	}
	return disappear_amount;
}

int Move_Scene_Base::collect_round_mover(size_t layer_id, const Coord &coord, Mover_Set &mover, int range) {
	if (range <= 0) {
		//MSG_USER("distance <= 0");
		return -1;
	}
	uint16_t block_index;
	if (calculate_block_index_by_grid_coord(coord.x, coord.y, block_index) < 0) {
		MSG_USER("coord error.");
		return -1;
	}

	std::vector<int> around;
	if (calculate_around_block_by_block_index(block_index, around) < 0) {
		MSG_USER("block_index error !");
		return -1;
	}
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		Mover_Map &mover_map = scene_layer.scene_block_vec_[*around_it].block_mover_map_;
		for (Mover_Map::iterator it = mover_map.begin(); it != mover_map.end(); ++it) {
			if (calculate_pixels_distance(coord, it->second->mover_detail().grid_coord) <= range) {
				mover.insert(it->second);
			}
		}
	}
	return 0;
}

int Move_Scene_Base::collect_round_fight_player(size_t layer_id, const Coord &coord, Scene_Player_Set &players, int range){
	if (range <= 0) {
		//MSG_USER("distance <= 0");
		return -1;
	}
	uint16_t block_index;
	if (calculate_block_index_by_grid_coord(coord.x, coord.y, block_index) < 0) {
		MSG_USER("coord error.");
		return -1;
	}

	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];

	std::vector<int> around;
	if (scene_layer.scene_block_vec_.size() == 1) {
		around.push_back(0);
	} else {
		if (calculate_around_block_by_block_index(block_index, around) < 0) {
			MSG_USER("block_index error !");
			return -1;
		}
	}

	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		Mover_Map &mover_map = scene_layer.scene_block_vec_[*around_it].block_mover_map_;
		for (Mover_Map::iterator it = mover_map.begin(); it != mover_map.end(); ++it) {
			if(it->second->player_self() && it->second->can_fight()){
				if (calculate_pixels_distance(coord, it->second->grid_coord()) <= range) {
					players.insert(it->second->player_self());
				}
			}
		}
	}
	return 0;
}


int Move_Scene_Base::collect_round_player(size_t layer_id, const Coord &coord, Scene_Player_Set &players, int range){
	if (range <= 0) {
		//MSG_USER("distance <= 0");
		return -1;
	}
	uint16_t block_index;
	if (calculate_block_index_by_grid_coord(coord.x, coord.y, block_index) < 0) {
		MSG_USER("coord error.");
		return -1;
	}

	std::vector<int> around;
	if (calculate_around_block_by_block_index(block_index, around) < 0) {
		MSG_USER("block_index error !");
		return -1;
	}
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		Mover_Map &mover_map = scene_layer.scene_block_vec_[*around_it].block_mover_map_;
		for (Mover_Map::iterator it = mover_map.begin(); it != mover_map.end(); ++it) {
			if(it->second->player_self() == NULL){
				continue;
			}
			if (calculate_pixels_distance(coord, it->second->grid_coord()) <= range) {
				players.insert(it->second->player_self());
			}
		}
	}
	return 0;
}

int Move_Scene_Base::collect_grid_mover(size_t layer_id, const Coord &coord, Mover_Set &mover, int range) {
	if (range <= 0) {
		MSG_USER("distance <= 0");
		return -1;
	}
	uint16_t block_index;
	if (calculate_block_index_by_grid_coord(coord.x, coord.y, block_index) < 0) {
		MSG_USER("coord error.");
		return -1;
	}

	std::vector<int> around;
	if (calculate_around_block_by_block_index(block_index, around) < 0) {
		MSG_USER("block_index error !\n");
		return -1;
	}
	Scene_Layer &scene_layer = scene_layer_vec_[layer_id];
	for (std::vector<int>::iterator around_it = around.begin(); around_it != around.end(); ++around_it) {
		Mover_Map &mover_map = scene_layer.scene_block_vec_[*around_it].block_mover_map_;
		for (Mover_Map::iterator it = mover_map.begin(); it != mover_map.end(); ++it) {
			if (calculate_grid_distance(coord, it->second->mover_detail().grid_coord) <= range) {
				mover.insert(it->second);
			}
		}
	}

	return 0;
}
