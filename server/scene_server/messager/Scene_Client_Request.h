/*
 * Scene_Client_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef SCENE_CLIENT_REQUEST_H_
#define SCENE_CLIENT_REQUEST_H_

#include "Game_Typedef.h"

class Scene_Monitor;
class Scene_Player;
struct Hero;

class Scene_Client_Request {
public:
	Scene_Client_Request();
	virtual ~Scene_Client_Request();

	Scene_Monitor *monitor(void);

	/////////////////////////////////////////////////////////////////////////////

	int process_10200030(Scene_Player &player, Block_Buffer &buf);

	int process_10299999(Scene_Player &player, Block_Buffer &buf);

	int process_10200006(Scene_Player &player, Block_Buffer &buf);

	int process_10200102(Scene_Player &player, Block_Buffer &buf);

	int process_10200103(Scene_Player &player, Block_Buffer &buf);

	int process_10200032(Scene_Player &player, Block_Buffer &buf);

	int process_10200105(Scene_Player &player, Block_Buffer &buf);

	int process_10200150(Scene_Player &player, Block_Buffer &buf);

	int process_10200156(Scene_Player &player, Block_Buffer &buf);

	int process_10200155(Scene_Player &player, Block_Buffer &buf);

	int process_10200110(Scene_Player &player, Block_Buffer &buf);

	int process_10200111(Scene_Player &player, Block_Buffer &buf);

	int process_10200113(Scene_Player &player, Block_Buffer &buf);

	int process_10200117(Scene_Player &player, Block_Buffer &buf);

	int process_10210000(Scene_Player &player, Block_Buffer &buf);

	int process_10210001(Scene_Player &player, Block_Buffer &buf);

	int process_10210100(Scene_Player &player, Block_Buffer &buf);

	int process_10210101(Scene_Player &player, Block_Buffer &buf);

	int process_10210200(Scene_Player &player, Block_Buffer &buf);

	int process_10200005(Scene_Player &player, Block_Buffer &buf);

	int process_10200007(Scene_Player &player, Block_Buffer &buf);

	int process_10200149(Scene_Player &player, Block_Buffer &buf);

	int process_10200151(Scene_Player &player, Block_Buffer &buf);

	int process_10210103(Scene_Player &player, Block_Buffer &buf);

	int process_10200008(Scene_Player &player, Block_Buffer &buf);

	int process_10210104(Scene_Player &player, Block_Buffer &buf);

	int process_10210002(Scene_Player &player, Block_Buffer &buf);

	int process_10210010(Scene_Player &player, Block_Buffer &buf);

	int process_10210011(Scene_Player &player, Block_Buffer &buf);

	int process_10200009(Scene_Player &player, Block_Buffer &buf);

	int process_10211001(Scene_Player &player, Block_Buffer &buf);

	int process_10211002(Scene_Player &player, Block_Buffer &buf);

	int process_10200200(Scene_Player &player, Block_Buffer &buf);

	int process_10200215(Scene_Player &player, Block_Buffer &buf);

	int process_10200011(Scene_Player &player, Block_Buffer &buf);

	int process_10200152(Scene_Player &player, Block_Buffer &buf);
	int process_10200153(Scene_Player &player, Block_Buffer &buf);
	int process_10200154(Scene_Player &player, Block_Buffer &buf);

	int process_10200701(Scene_Player &player, Block_Buffer &buf);
	int process_10200702(Scene_Player &player, Block_Buffer &buf);
	int process_10200703(Scene_Player &player, Block_Buffer &buf);
	int process_10200704(Scene_Player &player, Block_Buffer &buf);
	int process_10200705(Scene_Player &player, Block_Buffer &buf);

	int process_10211100(Scene_Player &player, Block_Buffer &buf);
	int process_10211101(Scene_Player &player, Block_Buffer &buf);

	int process_10211201(Scene_Player &player, Block_Buffer &buf);
	int process_10211202(Scene_Player &player, Block_Buffer &buf);
	int process_10211205(Scene_Player &player, Block_Buffer &buf);
	int process_10211207(Scene_Player &player, Block_Buffer &buf);
	int process_10211208(Scene_Player &player, Block_Buffer &buf);

	int process_10211300(Scene_Player &player, Block_Buffer &buf);
	int process_10211301(Scene_Player &player, Block_Buffer &buf);

	int process_10211310(Scene_Player &player, Block_Buffer &buf);
	int process_10211311(Scene_Player &player, Block_Buffer &buf);
	int process_10211312(Scene_Player &player, Block_Buffer &buf);

	int process_10270001(Scene_Player &player, Block_Buffer &buf);
	int process_10270002(Scene_Player &player, Block_Buffer &buf);
	int process_10270004(Scene_Player &player, Block_Buffer &buf);

	int process_10209001(Scene_Player &player, Block_Buffer &buf);
	int process_10209002(Scene_Player &player, Block_Buffer &buf);
	int process_10209003(Scene_Player &player, Block_Buffer &buf);

	// expedition start --//
	int process_10201300(Scene_Player &player, Block_Buffer &buf);
	int process_10201301(Scene_Player &player, Block_Buffer &buf);
	int process_10201302(Scene_Player &player, Block_Buffer &buf);
	int process_10201303(Scene_Player &player, Block_Buffer &buf);
	int process_10201304(Scene_Player &player, Block_Buffer &buf);
	int process_10201305(Scene_Player &player, Block_Buffer &buf);
	int process_10201306(Scene_Player &player, Block_Buffer &buf);
	int process_10201307(Scene_Player &player, Block_Buffer &buf);
	int process_10201308(Scene_Player &player, Block_Buffer &buf);
	int process_10201309(Scene_Player &player, Block_Buffer &buf);
	int process_10201310(Scene_Player &player, Block_Buffer &buf);
	int process_10201311(Scene_Player &player, Block_Buffer &buf);
	int process_10201312(Scene_Player &player, Block_Buffer &buf);
	// -- expedition end //

private:
	Scene_Monitor *monitor_;
};

typedef Singleton<Scene_Client_Request> Scene_Client_Request_Single;
#define SCENE_CLIENT_REQUEST    (Scene_Client_Request_Single::instance())

inline Scene_Monitor *Scene_Client_Request::monitor(void) {
	return monitor_;
}

#endif /* SCENE_CLIENT_REQUEST_H_ */
