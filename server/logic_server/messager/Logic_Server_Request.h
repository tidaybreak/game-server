/*
 * Logic_Server_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef LOGIC_SERVER_REQUEST_H_
#define LOGIC_SERVER_REQUEST_H_

#include "Game_Typedef.h"

class Logic_Player;
class Logic_Monitor;
struct Logic_Player_Detail;

class Logic_Server_Request {
public:
	Logic_Server_Request();
	virtual ~Logic_Server_Request();

	Logic_Monitor *monitor(void);

	int find_player_by_buf(Block_Buffer &buf, Logic_Player *&player);

	int abnormal_logout(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int scene_login_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int gate_login_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_login_session(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int chat_login_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int player_sign_out(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int scene_logout_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int gate_logout_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int chat_logout_ready(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int gate_client_login(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int player_trusteeship(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	/////////////////////////////////////////////////////////////////////////////////////////

	int process_20200010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100012(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20010050(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200050(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200055(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200056(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200200(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200202(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200110(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200080(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200081(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20400004(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20400005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20400008(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20400009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100024(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300031(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300033(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300034(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300035(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300036(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300037(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300039(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100310(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100312(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100304(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100305(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20100307(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100030(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100500(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20100504(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20100401(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20410000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20420000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300100(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300101(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100040(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100016(int cid, role_id_t role_id, Logic_Player* player, Block_Buffer &buf);
	int process_20100017(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20100027(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20000052(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100031(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20600001(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600003(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600006(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600007(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600016(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600017(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600018(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600019(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600020(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600021(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600022(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600023(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600024(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600025(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600026(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600027(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600028(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600029(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600031(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600032(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600033(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20600034(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200021(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20170001(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20170002(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20170003(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20170005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20170007(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20170008(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
//	int process_20170009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300110(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300111(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300112(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300113(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300115(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300116(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300117(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300300(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300301(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300302(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20300303(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20300400(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20200201(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20500000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20610000(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20610010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20610011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_20610012(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20100200(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

	int process_20610020(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	////////////////php///////////////////////////

	int process_30100001(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//封号
	int process_30100010(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//封IP和解封IP
	int process_30100002(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//商品信息变动通知
	int process_30100003(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//充值通知
	int process_30100009(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//inner充值通知
	int process_30100004(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//send_mail
	int process_30100006(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//请求获取统计数据
	int process_30100007(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);//请求你提我改回复
	int process_30100008(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_30100005(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_30100011(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_30100012(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_30100013(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_30100014(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_30100015(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);
	int process_30100016(int cid, role_id_t role_id, Logic_Player *player, Block_Buffer &buf);

private:
	Logic_Monitor *monitor_;
};

typedef Singleton<Logic_Server_Request> Logic_Server_Request_Single;
#define LOGIC_SERVER_REQUEST    (Logic_Server_Request_Single::instance())

inline Logic_Monitor *Logic_Server_Request::monitor(void) {
	return monitor_;
}

#endif /* LOGIC_SERVER_REQUEST_H_ */
