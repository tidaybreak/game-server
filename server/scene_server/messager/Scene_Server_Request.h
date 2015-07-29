/*
 * Scene_Server_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef SCENE_SERVER_REQUEST_H_
#define SCENE_SERVER_REQUEST_H_

#include "Game_Typedef.h"

class Scene_Monitor;
class Scene_Player;

class Scene_Server_Request {
public:
	Scene_Server_Request();
	virtual ~Scene_Server_Request();

	Scene_Monitor *monitor(void);

	int find_player_by_buf(Block_Buffer &buf, Scene_Player *&player);

	int logic_login_ready(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int logic_logout_ready(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	/////////////////////////////////////////////////////////////////////////////////////////

	int process_20110001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20200050(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20200055(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20200056(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20200100(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100018(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100019(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100022(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100023(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100025(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300030(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300032(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300033(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300034(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300035(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300038(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300040(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20400000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20400001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20400002(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20400003(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20400007(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20410001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100200(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100028(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100029(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100031(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100032(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100030(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100050(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100051(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100303(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100304(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100305(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100306(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100400(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100401(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100500(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100501(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100502(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100503(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300012(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20420000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20170000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20170001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20170002(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20170004(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20170006(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100309(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100310(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300100(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300101(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100403(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100026(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20100040(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100600(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20600000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600002(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600004(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600007(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600008(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600009(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600010(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600012(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600014(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600015(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20600031(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300110(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300111(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300112(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300114(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300116(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300300(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300301(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300302(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20300303(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300400(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20200020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20300200(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20200022(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20200023(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20010061(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20100060(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20610000(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20610001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20610010(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);
	int process_20610012(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20610020(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);


	int process_20020001(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

	int process_20999999(int cid, role_id_t role_id, Scene_Player *player, Block_Buffer &buf);

private:
	int process_20400000_1(Block_Buffer &buf, Scene_Player *player);
	int process_20400000_2(Block_Buffer &buf, Scene_Player *player);
	int process_20400000_3(Block_Buffer &buf, Scene_Player *player);

	int process_20400001_1(Block_Buffer &buf);
	int process_20400001_2(Block_Buffer &buf);
	int process_20400001_3(Block_Buffer &buf);

private:
	Scene_Monitor *monitor_;
};

typedef Singleton<Scene_Server_Request> Scene_Server_Request_Single;
#define SCENE_SERVER_REQUEST    (Scene_Server_Request_Single::instance())

inline Scene_Monitor *Scene_Server_Request::monitor(void) {
	return monitor_;
}

#endif /* SCENE_SERVER_REQUEST_H_ */
