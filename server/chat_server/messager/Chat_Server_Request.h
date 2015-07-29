/*
 * Chat_Server_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef CHAT_SERVER_REQUEST_H_
#define CHAT_SERVER_REQUEST_H_

#include "Game_Typedef.h"

class Chat_Player;
class Chat_Monitor;

class Chat_Server_Request {
public:
	Chat_Server_Request();
	virtual ~Chat_Server_Request();

	Chat_Monitor *monitor(void);

	int logic_login_ready(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);

	int logic_logout_ready(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);

	int process_20100015(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);

	int process_20100028(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);

	int process_20100029(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	// gang start
	int process_20170000(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	// gang end
	int process_20500000(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	//通知聊天服客户端初始化完成
	int process_20010085(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	//friend_start
	  //black_list
	int process_20160000(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	//friend_end

	//组队信息
	int process_20100301(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	int process_20100302(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	//chat
	int process_20100099(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	int process_20100098(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);

	//vip
	int process_20200024(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	//level
	int process_20200025(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);

	int process_30100008(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);
	int process_30100005(int cid, role_id_t role_id, Chat_Player *player, Block_Buffer &buf);

private:
	Chat_Monitor *monitor_;
};

typedef Singleton<Chat_Server_Request> Chat_Server_Request_Single;
#define CHAT_SERVER_REQUEST    (Chat_Server_Request_Single::instance())

inline Chat_Monitor *Chat_Server_Request::monitor(void) {
	return monitor_;
}

#endif /* CHAT_SERVER_REQUEST_H_ */
