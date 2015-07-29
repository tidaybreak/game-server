/*
 * Camera.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#include "Camera.h"
//#include "Log_Connector.h"

Camera::Camera() : replace_role_id_(0) {
	// TODO Auto-generated constructor stub

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}


int Camera::init(Move_Scene *scene, Coord &grid_coord, role_id_t replace_role_id) {
//	mover_detail_.mover_id = 1;
//	mover_detail_.grid_coord = grid_coord;
//	replace_role_id_ = replace_role_id;
//	reset_scene(scene);
//
//	this->appear();

	return 0;
}

int Camera::set_buf_name(std::string &buf_name) {
	buf_stream_name_ = buf_name;

	return 0;
}

int Camera::reset(void) {
	buf_stream_name_ = "";
	buf_tick_ = Time_Value::gettimeofday();
	buf_.reset();
	replace_role_id_ = 0;

	return 0;
}

int Camera::make_appear_info(Block_Buffer &buf) {
	return 0;
}

int Camera::send_buf_to_logic(void) {
//	if (buf_.readable_bytes() == 0) {
//		MSG_USER("error buf size");
//		return 0;
//	}
//
//	uint16_t buf_read_inx = buf_.get_read_idx();
//
//	uint16_t max_package = 1024 * 40;
//	while (buf_.readable_bytes() > 0) {
//		Block_Buffer send_buf;
//		send_buf.make_message(INNER_AUTO_PLAYER_BUF);
//		send_buf.write_string(buf_stream_name_);
//		send_buf.write_int32(1);
//		uint16_t read_len = buf_.readable_bytes() > max_package ? max_package : buf_.readable_bytes();
//		send_buf.copy(buf_.get_read_ptr(), read_len);
//		send_buf.finish_message();
//		MAP_MANAGER_INSTANCE->send_to_logic_server(send_buf);
//
//		buf_.set_read_idx(buf_.get_read_idx() + read_len);
//	}
//
//	buf_.set_read_idx(buf_read_inx);

	return 0;
}

int Camera::get_buf(Block_Buffer *buf) {
	*buf = buf_;
	return 0;
}

int Camera::process_buf(Block_Buffer &old_buf, Block_Buffer &new_buf) {
//	if (!replace_role_id_) return -1;
//
//	/*
//		800000
//		800001
//		800002
//		800003
//		800501
//		800502
//		800503
//		801800
//		801801
//	*/
//
//	uint16_t len = 0;
//	uint32_t msg_id = 0;
//	int32_t status = 0;
//	for (int i = 0; i < 100; ++i) {
//		if (old_buf.readable_bytes() < 10) break;
//
//		old_buf.read_uint16(len);
//		old_buf.read_uint32(msg_id);
//		old_buf.read_int32(status);
//
//		switch (msg_id) {
//		case ACTIVE_MOVER_APPEAR: {
//			MSG_800000 msg;
//			msg.deserialize(old_buf);
//			if (msg.role_info.role_id == replace_role_id_)
//				msg.role_info.role_id = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_MOVER_DISAPPEAR: {
//			MSG_800001 msg;
//			msg.deserialize(old_buf);
//			if (msg.role_id == replace_role_id_)
//				msg.role_id = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_MOVER_MOVE: {
//			MSG_800002 msg;
//			msg.deserialize(old_buf);
//			if (msg.role_id == replace_role_id_)
//				msg.role_id = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_MONSTER_APPEAR: {
//			MSG_800003 msg;
//			msg.deserialize(old_buf);
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_PET_APPEAR: {
//			MSG_800006 msg;
//			msg.deserialize(old_buf);
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_PROPERTY_INFO: {
//			MSG_800501 msg;
//			msg.deserialize(old_buf);
//			if (msg.role_id == replace_role_id_)
//				msg.role_id = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_FIGHT_ACTION_BROADCAST: {
//			MSG_800502 msg;
//			msg.deserialize(old_buf);
//			if (msg.attack_id == replace_role_id_)
//				msg.attack_id = 1;
//
//			for (std::vector<int64_t>::iterator it = msg.target_id.begin(); it != msg.target_id.end(); ++it) {
//				if (*it == replace_role_id_)
//					*it = 1;
//			}
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_FIGHT_RESULT: {
//			MSG_800503 msg;
//			msg.deserialize(old_buf);
//			if (msg.attack_id == replace_role_id_)
//				msg.attack_id = 1;
//
//			if (msg.target_id == replace_role_id_)
//				msg.target_id = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_MOUNT_RIDE: {
//			MSG_800602 msg;
//			msg.deserialize(old_buf);
//			if (msg.role_id == replace_role_id_)
//				msg.role_id = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_FACADE_UPDATE: {
//			MSG_800015 msg;
//			msg.deserialize(old_buf);
//			if (msg.role_id == replace_role_id_)
//				msg.role_id = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_STATUS_UPDATE: {
//			MSG_801800 msg;
//			msg.deserialize(old_buf);
//
//			for (std::vector<Status_Info>::iterator it = msg.status_info.begin(); it != msg.status_info.end(); ++it) {
//				if ((*it).status_owner == replace_role_id_)
//					(*it).status_owner = 1;
//				if ((*it).status_caster == replace_role_id_)
//					(*it).status_caster = 1;
//			}
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		case ACTIVE_STATUS_ERASE_UPDATE: {
//			MSG_801801 msg;
//			msg.deserialize(old_buf);
//
//			if (msg.status_owner == replace_role_id_)
//				msg.status_owner = 1;
//
//			if (msg.status_caster == replace_role_id_)
//				msg.status_caster = 1;
//
//			new_buf.write_uint16(len);
//			new_buf.write_uint32(msg_id);
//			new_buf.write_int32(status);
//			msg.serialize(new_buf);
//			break;
//		}
//		default :{
//			MSG_USER("new msg_id:%d need add", msg_id);
//			return -1;
//			break;
//		}
//		}
//	}

	return 0;
}
