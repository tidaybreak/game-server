/*
 * Camera.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef CAMERA_H_
#define CAMERA_H_


#include "NPC.h"

class Camera: public NPC {
public:
	Camera();
	virtual ~Camera();

	int init(Move_Scene *scene, Coord &grid_coord, role_id_t replace_role_id);
	int set_buf_name(std::string &buf_name);

	int reset(void);

	virtual bool is_player(void) { return true; }

	virtual int make_appear_info(Block_Buffer &buf);

	int send_buf_to_logic(void);

	int process_buf(Block_Buffer &old_buf, Block_Buffer &new_buf);

	int get_buf(Block_Buffer *buf);

private:
	// auto player watch
	std::string buf_stream_name_;
	Time_Value buf_tick_;
	Block_Buffer buf_;
	role_id_t replace_role_id_;
};
#endif /* CAMERA_H_ */
