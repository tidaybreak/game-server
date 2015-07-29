/*
 * Core_Define.h
 *
 *  Created on: Dec 20, 2013
 *      Author: Root
 */

#ifndef CORE_DEFINE_H_
#define CORE_DEFINE_H_

enum Link_Close_Reason {
	LINK_CLOSE_NULL 				= 1000,
	LINK_CLOSE_TIMEOUT				= 1001,		// 连接超时,可能心跳包异常
	LINK_CLOSE_BLOCK_ERROR 			= 1002,		// Svc:组收到的包时:数据块异常, 关闭该连接
	LINK_CLOSE_HEAD_ERROR 			= 1003,		// Svc:组收到的包时:包长度标识为0, 包长度标识超过max_pack_size_, 即视为无效包, 异常, 关闭该连接
	LINK_CLOSE_SEND_BLOCK_FULL		= 1004,		// Svc:发送数据时:send_block_list_ 已满 send_block_list_.size()
	LINK_CLOSE_RECV_BLOCK_FULL		= 1005,		// Svc:接收数据时:recv_block_list_ 已满 recv_block_list_.size()
	LINK_CLOSE_BY_PEER 				= 1006,		// Svc:读数据时:对方关闭
	LINK_CLOSE_RECV_ERROR			= 1007,
	LINK_CLOSE_SEND_ERROR			= 1008,
};

struct Link_Close {
	Link_Close(void): close_reason(LINK_CLOSE_NULL), close_value(0) {};
	Link_Close(Link_Close_Reason close_reason_, int64_t close_value_) : close_reason(close_reason_), close_value(close_value_) {};
	Link_Close(int close_reason_, int64_t close_value_) : close_reason(Link_Close_Reason(close_reason_)), close_value(close_value_) {};

	void reset(void) {
		close_reason = LINK_CLOSE_NULL;
		close_value = 0;
	}
	Link_Close_Reason close_reason;
	int64_t close_value;
};

struct Unit_Message {
    enum DATA_TYPE {
    	TYPE_DATA_NULL					= 0,
        TYPE_DATA_BUFFER     		= 1,
        TYPE_DATA_PLAYER   		= 2,
        TYPE_DATA_ARENA_FIGHTER  = 3,
        TYPE_DATA_DRAGON_VALE_ROB_MATCH = 4,
        TYPE_DATA_DRAGON_VALE_ROB_FIGHT = 5,
        TYPE_DATA_KNIGHT_TRIALS_MATCH_PLAYER = 6,
        TYPE_DATA_RANKING = 7,
		TYPE_DATA_KNIGHT_TRIALS_LOAD_FIGHTER_PLAYER = 8,
    };

    enum UNIT_TYPE {
    	TYPE_UNIT_NULL				= 0,
        TYPE_UNIT_CLIENT     		= 1,
        TYPE_UNIT_ACCEPT_SERVER   	= 2,
        TYPE_UNIT_CONNECT_SERVER   	= 3,
        TYPE_UNIT_LOOP   			= 4,
        TYPE_UNIT_TIMER   			= 5,
    };

    int32_t data_type;
    int32_t unit_type;
    void *ptr_data;

    int32_t pool_gid;

    Unit_Message() { reset(); };
    void reset(void) {
    	data_type = TYPE_DATA_NULL;
    	unit_type = TYPE_UNIT_NULL;
    	ptr_data = 0;
    	pool_gid = 0;
    }
};

#endif /* CORE_DEFINE_H_ */
