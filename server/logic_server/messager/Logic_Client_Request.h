/*
 * Logic_Client_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 */

#ifndef LOGIC_CLIENT_REQUEST_H_
#define LOGIC_CLIENT_REQUEST_H_

#include "Game_Typedef.h"
#include "Msg_Role_Scene_Struct.h"

class Logic_Monitor;
class Logic_Player;
struct Logic_Player_Detail;

class Logic_Client_Request {
public:
	Logic_Client_Request();
	virtual ~Logic_Client_Request();

	Logic_Monitor *monitor(void);

	// 登录, 创建角色 成功处理
	Logic_Player *process_success_login(int cid, Logic_Player_Detail &detail);

	int process_fetch_role(int cid, MSG_10100000 &msg);

	int process_random_name(int cid, MSG_10100007 &msg);

	int process_player_login(int cid, MSG_10100001 &msg);

	int process_player_create(int cid, MSG_10100005 &msg);

	/////////////////////////////////////////////////////////////////////////////
	//记录统计数据
	int record_access_count(int cid, MSG_10188888 &msg);
	//--测试命令
	int process_10199999(Logic_Player &player, Block_Buffer &buf);
	//--访问统计
	int process_10188888(Logic_Player &player, Block_Buffer &buf);

	//--人物场景
	int process_10100010(Logic_Player &player, Block_Buffer &buf);
	int process_10100015(Logic_Player &player, Block_Buffer &buf);
	int process_10100006(Logic_Player &player, Block_Buffer &buf);
	int process_10100008(Logic_Player &player, Block_Buffer &buf);
	int process_10100032(Logic_Player &player, Block_Buffer &buf);

	//public
	int process_10110017(Logic_Player &player, Block_Buffer &buf);
	int process_10120096(Logic_Player &player, Block_Buffer &buf);
	int process_10100332(Logic_Player &player, Block_Buffer &buf);


	//--背包系统
	int process_10100100(Logic_Player &player, Block_Buffer &buf);
	int process_10100101(Logic_Player &player, Block_Buffer &buf);
	int process_10100102(Logic_Player &player, Block_Buffer &buf);
	int process_10100103(Logic_Player &player, Block_Buffer &buf);
	int process_10100104(Logic_Player &player, Block_Buffer &buf);
	int process_10100105(Logic_Player &player, Block_Buffer &buf);
	int process_10100106(Logic_Player &player, Block_Buffer &buf);
	int process_10100107(Logic_Player &player, Block_Buffer &buf);
	int process_10100108(Logic_Player &player, Block_Buffer &buf);
	int process_10100109(Logic_Player &player, Block_Buffer &buf);
	int process_10100110(Logic_Player &player, Block_Buffer &buf);
	int process_10100111(Logic_Player &player, Block_Buffer &buf);
	int process_10100112(Logic_Player &player, Block_Buffer &buf);
	int process_10100113(Logic_Player &player, Block_Buffer &buf);
	//--背包系统

	//邮件系统
	int process_10150000(Logic_Player &player, Block_Buffer &buf);
	int process_10150001(Logic_Player &player, Block_Buffer &buf);
	int process_10150002(Logic_Player &player, Block_Buffer &buf);
	int process_10150003(Logic_Player &player, Block_Buffer &buf);
	int process_10150005(Logic_Player &player, Block_Buffer &buf);
	int process_10150004(Logic_Player &player, Block_Buffer &buf);
	//邮件系统
	
	//friends system
	int process_10160000(Logic_Player &player, Block_Buffer &buf);
	int process_10160001(Logic_Player &player, Block_Buffer &buf);
	int process_10160002(Logic_Player &player, Block_Buffer &buf);
	int process_10160003(Logic_Player &player, Block_Buffer &buf);
	int process_10160004(Logic_Player &player, Block_Buffer &buf);
	int process_10160013(Logic_Player &player, Block_Buffer &buf);
	int process_10160005(Logic_Player &player, Block_Buffer &buf);
	int process_10160006(Logic_Player &player, Block_Buffer &buf);
	int process_10160009(Logic_Player &player, Block_Buffer &buf);
	int process_10160010(Logic_Player &player, Block_Buffer &buf);
	int process_10160011(Logic_Player &player, Block_Buffer &buf);
	int process_10160012(Logic_Player &player, Block_Buffer &buf);
	int process_10160014(Logic_Player &player, Block_Buffer &buf);
	//friends system

	//--任务系统
	int process_10110000(Logic_Player &player, Block_Buffer &buf);
//	int process_10110001(Logic_Player &player, Block_Buffer &buf);
	int process_10110002(Logic_Player &player, Block_Buffer &buf);
	int process_10110003(Logic_Player &player, Block_Buffer &buf);
	int process_10110004(Logic_Player &player, Block_Buffer &buf);
	int process_10110007(Logic_Player &player, Block_Buffer &buf);
	int process_10110008(Logic_Player &player, Block_Buffer &buf);
	int process_10110009(Logic_Player &player, Block_Buffer &buf);
	int process_10110010(Logic_Player &player, Block_Buffer &buf);
	int process_10110011(Logic_Player &player, Block_Buffer &buf);
	int process_10110012(Logic_Player &player, Block_Buffer &buf);
	int process_10110013(Logic_Player &player, Block_Buffer &buf);
	int process_10110014(Logic_Player &player, Block_Buffer &buf);
	int process_10110015(Logic_Player &player, Block_Buffer &buf);
	int process_10110016(Logic_Player &player, Block_Buffer &buf);
	int process_10110018(Logic_Player &player, Block_Buffer &buf);
	int process_10110019(Logic_Player &player, Block_Buffer &buf);

	//--任务系统

	// --single dungeon
	int process_10100200(Logic_Player &player, Block_Buffer &buf);
	int process_10100201(Logic_Player &player, Block_Buffer &buf);
	int process_10100202(Logic_Player &player, Block_Buffer &buf);
	int process_10100204(Logic_Player &player, Block_Buffer &buf);
	int process_10100205(Logic_Player &player, Block_Buffer &buf);
	int process_10100206(Logic_Player &player, Block_Buffer &buf);
	int process_10100208(Logic_Player &player, Block_Buffer &buf);
	//-- equip start
	int process_10101000(Logic_Player &player, Block_Buffer &buf);
	int process_10101001(Logic_Player &player, Block_Buffer &buf);
	int process_10101002(Logic_Player &player, Block_Buffer &buf);
	int process_10101003(Logic_Player &player, Block_Buffer &buf);
	int process_10101004(Logic_Player &player, Block_Buffer &buf);
	int process_10101005(Logic_Player &player, Block_Buffer &buf);
	int process_10101006(Logic_Player &player, Block_Buffer &buf);
	int process_10101007(Logic_Player &player, Block_Buffer &buf);
	int process_10101008(Logic_Player &player, Block_Buffer &buf);
	int process_10101009(Logic_Player &player, Block_Buffer &buf);
	int process_10101010(Logic_Player &player, Block_Buffer &buf);
	int process_10101011(Logic_Player &player, Block_Buffer &buf);
	int process_10101012(Logic_Player &player, Block_Buffer &buf);
	int process_10101013(Logic_Player &player, Block_Buffer &buf);
	int process_10101014(Logic_Player &player, Block_Buffer &buf);
	int process_10101015(Logic_Player &player, Block_Buffer &buf);
	//-- equip end

	//-- rune start
	int process_10103000(Logic_Player &player, Block_Buffer &buf);
	int process_10103001(Logic_Player &player, Block_Buffer &buf);
	int process_10103002(Logic_Player &player, Block_Buffer &buf);
	//-- rune end

	//-- rune_stone start
	int process_10103200(Logic_Player &player, Block_Buffer &buf);
	int process_10103201(Logic_Player &player, Block_Buffer &buf);
	int process_10103202(Logic_Player &player, Block_Buffer &buf);
	int process_10103203(Logic_Player &player, Block_Buffer &buf);
	int process_10103204(Logic_Player &player, Block_Buffer &buf);
	int process_10103205(Logic_Player &player, Block_Buffer &buf);
	int process_10103206(Logic_Player &player, Block_Buffer &buf);
	int process_10103207(Logic_Player &player, Block_Buffer &buf);
	int process_10103208(Logic_Player &player, Block_Buffer &buf);
	int process_10103209(Logic_Player &player, Block_Buffer &buf);
	int process_10103210(Logic_Player &player, Block_Buffer &buf);
	int process_10103211(Logic_Player &player, Block_Buffer &buf);
	//-- rune_stone end

	// learn skills
	int process_10100140(Logic_Player &player, Block_Buffer &buf);
	int process_10100141(Logic_Player &player, Block_Buffer &buf);
	int process_10100165(Logic_Player &player, Block_Buffer &buf);
	int process_10100166(Logic_Player &player, Block_Buffer &buf);
	int process_10100143(Logic_Player &player, Block_Buffer &buf);
	int process_10100144(Logic_Player &player, Block_Buffer &buf);
	// learn skills

	//-- hero start
	int process_10102000(Logic_Player &player, Block_Buffer &buf);
	int process_10102001(Logic_Player &player, Block_Buffer &buf);
	int process_10102002(Logic_Player &player, Block_Buffer &buf);
	int process_10102003(Logic_Player &player, Block_Buffer &buf);
	int process_10102004(Logic_Player &player, Block_Buffer &buf);
	int process_10102005(Logic_Player &player, Block_Buffer &buf);
	int process_10102006(Logic_Player &player, Block_Buffer &buf);
	int process_10102007(Logic_Player &player, Block_Buffer &buf);
	int process_10102020(Logic_Player &player, Block_Buffer &buf);
	int process_10102021(Logic_Player &player, Block_Buffer &buf);
	int process_10102022(Logic_Player &player, Block_Buffer &buf);
	int process_10102032(Logic_Player &player, Block_Buffer &buf);
	int process_10102033(Logic_Player &player, Block_Buffer &buf);
	int process_10102034(Logic_Player &player, Block_Buffer &buf);
	//-- hero end
	//商城 start
	int process_10151000(Logic_Player &player, Block_Buffer &buf);
	int process_10151001(Logic_Player &player, Block_Buffer &buf);
	int process_10151002(Logic_Player &player, Block_Buffer &buf);
	int process_10151003(Logic_Player &player, Block_Buffer &buf);
	//商城 end

	//-- team start
	int process_10100300(Logic_Player &player, Block_Buffer &buf);
	int process_10100301(Logic_Player &player, Block_Buffer &buf);
	int process_10100302(Logic_Player &player, Block_Buffer &buf);
	int process_10100303(Logic_Player &player, Block_Buffer &buf);
	int process_10100304(Logic_Player &player, Block_Buffer &buf);
	int process_10100305(Logic_Player &player, Block_Buffer &buf);
	int process_10100306(Logic_Player &player, Block_Buffer &buf);
//	int process_10100307(Logic_Player &player, Block_Buffer &buf);
	int process_10100308(Logic_Player &player, Block_Buffer &buf);
	int process_10100309(Logic_Player &player, Block_Buffer &buf);
	int process_10100310(Logic_Player &player, Block_Buffer &buf);
//	int process_10100311(Logic_Player &player, Block_Buffer &buf);
	int process_10100312(Logic_Player &player, Block_Buffer &buf);
	int process_10100313(Logic_Player &player, Block_Buffer &buf);
	int process_10100315(Logic_Player &player, Block_Buffer &buf);
	int process_10100316(Logic_Player &player, Block_Buffer &buf);
	int process_10100317(Logic_Player &player, Block_Buffer &buf);
	int process_10100318(Logic_Player &player, Block_Buffer &buf);
	int process_10100319(Logic_Player &player, Block_Buffer &buf);
	int process_10100320(Logic_Player &player, Block_Buffer &buf);
	int process_10100321(Logic_Player &player, Block_Buffer &buf);
	int process_10100322(Logic_Player &player, Block_Buffer &buf);
	int process_10100323(Logic_Player &player, Block_Buffer &buf);
	int process_10100324(Logic_Player &player, Block_Buffer &buf);
	int process_10100325(Logic_Player &player, Block_Buffer &buf);
	int process_10100326(Logic_Player &player, Block_Buffer &buf);
	int process_10100327(Logic_Player &player, Block_Buffer &buf);
	int process_10100328(Logic_Player &player, Block_Buffer &buf);
	int process_10100330(Logic_Player &player, Block_Buffer &buf);
	int process_10100331(Logic_Player &player, Block_Buffer &buf);
	int process_10100333(Logic_Player &player, Block_Buffer &buf);

	//-- team end
	//-- gang start
	int process_10170000(Logic_Player &player, Block_Buffer &buf);
	int process_10170001(Logic_Player &player, Block_Buffer &buf);
	int process_10170002(Logic_Player &player, Block_Buffer &buf);
	int process_10170003(Logic_Player &player, Block_Buffer &buf);
	int process_10170004(Logic_Player &player, Block_Buffer &buf);
	int process_10170005(Logic_Player &player, Block_Buffer &buf);
	int process_10170006(Logic_Player &player, Block_Buffer &buf);
	int process_10170007(Logic_Player &player, Block_Buffer &buf);
	int process_10170008(Logic_Player &player, Block_Buffer &buf);
	int process_10170009(Logic_Player &player, Block_Buffer &buf);
	int process_10170010(Logic_Player &player, Block_Buffer &buf);
	int process_10170011(Logic_Player &player, Block_Buffer &buf);
	int process_10170012(Logic_Player &player, Block_Buffer &buf);
	int process_10170013(Logic_Player &player, Block_Buffer &buf);
	int process_10170014(Logic_Player &player, Block_Buffer &buf);
	int process_10170015(Logic_Player &player, Block_Buffer &buf);
	int process_10170016(Logic_Player &player, Block_Buffer &buf);
	int process_10170017(Logic_Player &player, Block_Buffer &buf);
	int process_10170018(Logic_Player &player, Block_Buffer &buf);
	int process_10170019(Logic_Player &player, Block_Buffer &buf);
	int process_10170020(Logic_Player &player, Block_Buffer &buf);
	int process_10170021(Logic_Player &player, Block_Buffer &buf);
	int process_10170022(Logic_Player &player, Block_Buffer &buf);
	int process_10170023(Logic_Player &player, Block_Buffer &buf);
	int process_10170024(Logic_Player &player, Block_Buffer &buf);
	int process_10170025(Logic_Player &player, Block_Buffer &buf);
	int process_10170026(Logic_Player &player, Block_Buffer &buf);
	int process_10170027(Logic_Player &player, Block_Buffer &buf);
	int process_10170028(Logic_Player &player, Block_Buffer &buf);
	int process_10170029(Logic_Player &player, Block_Buffer &buf);
	int process_10170030(Logic_Player &player, Block_Buffer &buf);
	int process_10170031(Logic_Player &player, Block_Buffer &buf);
	int process_10170032(Logic_Player &player, Block_Buffer &buf);
	int process_10170033(Logic_Player &player, Block_Buffer &buf);
	int process_10170034(Logic_Player &player, Block_Buffer &buf);
	int process_10170035(Logic_Player &player, Block_Buffer &buf);
	int process_10170036(Logic_Player &player, Block_Buffer &buf);
	int process_10170037(Logic_Player &player, Block_Buffer &buf);
	int process_10170038(Logic_Player &player, Block_Buffer &buf);
	int process_10170039(Logic_Player &player, Block_Buffer &buf);
	int process_10170041(Logic_Player &player, Block_Buffer &buf);
	int process_10170042(Logic_Player &player, Block_Buffer &buf);
	int process_10170043(Logic_Player &player, Block_Buffer &buf);
	int process_10170044(Logic_Player &player, Block_Buffer &buf);
	int process_10170045(Logic_Player &player, Block_Buffer &buf);
	int process_10170047(Logic_Player &player, Block_Buffer &buf);
	int process_10170048(Logic_Player &player, Block_Buffer &buf);
	int process_10170049(Logic_Player &player, Block_Buffer &buf);
	//-- gang end
	//fcm
	int process_10100090(Logic_Player &player, Block_Buffer &buf);
	//fcm end
	//alchemy start
	int process_10152000(Logic_Player &player, Block_Buffer &buf);
	int process_10152001(Logic_Player &player, Block_Buffer &buf);
	//alchemy end

	//arena start
	int process_10100400(Logic_Player &player, Block_Buffer &buf);
	int process_10100405(Logic_Player &player, Block_Buffer &buf);
	int process_10100406(Logic_Player &player, Block_Buffer &buf);
	int process_10100407(Logic_Player &player, Block_Buffer &buf);
	int process_10100408(Logic_Player &player, Block_Buffer &buf);
	int process_10100410(Logic_Player &player, Block_Buffer &buf);

	int process_10100412(Logic_Player &player, Block_Buffer &buf);
	int process_10100413(Logic_Player &player, Block_Buffer &buf);
	int process_10100414(Logic_Player &player, Block_Buffer &buf);
	int process_10100415(Logic_Player &player, Block_Buffer &buf);

	int process_10100420(Logic_Player &player, Block_Buffer &buf);
	int process_10100401(Logic_Player &player, Block_Buffer &buf);
	int process_10100402(Logic_Player &player, Block_Buffer &buf);
	int process_10100416(Logic_Player &player, Block_Buffer &buf);
	int process_10100417(Logic_Player &player, Block_Buffer &buf);
	//arena end

	//guide start
	int process_10104000(Logic_Player &player, Block_Buffer &buf);
	int process_10104001(Logic_Player &player, Block_Buffer &buf);
	int process_10104002(Logic_Player &player, Block_Buffer &buf);
	int process_10120095(Logic_Player &player, Block_Buffer &buf);
	int process_10104003(Logic_Player &player, Block_Buffer &buf);
	//guide end

	//ranking
	int process_10171000(Logic_Player &player, Block_Buffer &buf);
	int process_10171001(Logic_Player &player, Block_Buffer &buf);
	int process_10171002(Logic_Player &player, Block_Buffer &buf);
	int process_10171003(Logic_Player &player, Block_Buffer &buf);

	//体力
	int process_10120000(Logic_Player &player, Block_Buffer &buf);
	int process_10120001(Logic_Player &player, Block_Buffer &buf);

	//Chat
	int process_10120002(Logic_Player &player, Block_Buffer &buf);

	//exchange start
	int process_10100500(Logic_Player &player, Block_Buffer &buf);
	int process_10100501(Logic_Player &player, Block_Buffer &buf);
	int process_10100502(Logic_Player &player, Block_Buffer &buf);
	//exchange edn

	//daily helper start
	int process_10106000(Logic_Player &player, Block_Buffer &buf);
	int process_10106001(Logic_Player &player, Block_Buffer &buf);
	int process_10106002(Logic_Player &player, Block_Buffer &buf);
	int process_10106003(Logic_Player &player, Block_Buffer &buf);
	int process_10106004(Logic_Player &player, Block_Buffer &buf);
	int process_10106005(Logic_Player &player, Block_Buffer &buf);
	int process_10106006(Logic_Player &player, Block_Buffer &buf);
	//daily helper end

	// world boss start
	int process_10111102(Logic_Player &player, Block_Buffer &buf);
	int process_10111103(Logic_Player &player, Block_Buffer &buf);
	// world boss end

	// war scene start
	int process_10111000(Logic_Player &player, Block_Buffer &buf);
	int process_10111001(Logic_Player &player, Block_Buffer &buf);
	int process_10111003(Logic_Player &player, Block_Buffer &buf);
	// war scene end

	//sys cfg start
	int process_10120003(Logic_Player &player, Block_Buffer &buf);
	int process_10120004(Logic_Player &player, Block_Buffer &buf);
	//sys cfg end

	// player awake start
	int process_10120020(Logic_Player &player, Block_Buffer &buf);
	int process_10120021(Logic_Player &player, Block_Buffer &buf);
	// player awake end

	// player artifact start
	int process_10120030(Logic_Player &player, Block_Buffer &buf);
	// player artifact end

	// forenotice start
	int process_10120040(Logic_Player &player, Block_Buffer &buf);
	int process_10120041(Logic_Player &player, Block_Buffer &buf);
	// forenotice end

	// exploit begin
	int process_10120080(Logic_Player &player, Block_Buffer &buf);
	// exploit end

	//covenant start
	int process_10106100(Logic_Player &player, Block_Buffer &buf);
	int process_10106101(Logic_Player &player, Block_Buffer &buf);
	int process_10106102(Logic_Player &player, Block_Buffer &buf);
	//covenant end

	//dragon vale begin
	int process_10180000(Logic_Player &player, Block_Buffer &buf);
	int process_10180001(Logic_Player &player, Block_Buffer &buf);
	int process_10180002(Logic_Player &player, Block_Buffer &buf);
	int process_10180003(Logic_Player &player, Block_Buffer &buf);
	int process_10180004(Logic_Player &player, Block_Buffer &buf);
	int process_10180005(Logic_Player &player, Block_Buffer &buf);
	int process_10180006(Logic_Player &player, Block_Buffer &buf);
	int process_10180007(Logic_Player &player, Block_Buffer &buf);
	int process_10180008(Logic_Player &player, Block_Buffer &buf);
	int process_10180009(Logic_Player &player, Block_Buffer &buf);
	int process_10180010(Logic_Player &player, Block_Buffer &buf);
	int process_10180011(Logic_Player &player, Block_Buffer &buf);
	int process_10180012(Logic_Player &player, Block_Buffer &buf);
	int process_10180013(Logic_Player &player, Block_Buffer &buf);
	int process_10180014(Logic_Player &player, Block_Buffer &buf);
	int process_10180015(Logic_Player &player, Block_Buffer &buf);
	int process_10180016(Logic_Player &player, Block_Buffer &buf);
	int process_10180017(Logic_Player &player, Block_Buffer &buf);
	int process_10180018(Logic_Player &player, Block_Buffer &buf);
	int process_10180019(Logic_Player &player, Block_Buffer &buf);
	int process_10180020(Logic_Player &player, Block_Buffer &buf);
	int process_10180021(Logic_Player &player, Block_Buffer &buf);
	int process_10180022(Logic_Player &player, Block_Buffer &buf);
	int process_10180023(Logic_Player &player, Block_Buffer &buf);
	int process_10180024(Logic_Player &player, Block_Buffer &buf);

	//dragon vale end

	//achievement begin
	int process_10100800(Logic_Player &player, Block_Buffer &buf);
	int process_10100801(Logic_Player &player, Block_Buffer &buf);
	int process_10100802(Logic_Player &player, Block_Buffer &buf);
	//achievement end

	//title begin
	int process_10100900(Logic_Player &player, Block_Buffer &buf);
	int process_10100901(Logic_Player &player, Block_Buffer &buf);
	//title end

	//vip start
	int process_10106200(Logic_Player &player, Block_Buffer &buf);
	int process_10106201(Logic_Player &player, Block_Buffer &buf);
	int process_10106202(Logic_Player &player, Block_Buffer &buf);
	int process_10106203(Logic_Player &player, Block_Buffer &buf);
	int process_10106204(Logic_Player &player, Block_Buffer &buf);
	int process_10106205(Logic_Player &player, Block_Buffer &buf);
	int process_10106206(Logic_Player &player, Block_Buffer &buf);
	int process_10106207(Logic_Player &player, Block_Buffer &buf);
	//vip end

	// mount start
	int process_10100600(Logic_Player &player, Block_Buffer &buf);
	int process_10100601(Logic_Player &player, Block_Buffer &buf);
	int process_10100602(Logic_Player &player, Block_Buffer &buf);
	int process_10100603(Logic_Player &player, Block_Buffer &buf);
	int process_10100604(Logic_Player &player, Block_Buffer &buf);

	int process_10100605(Logic_Player &player, Block_Buffer &buf);
	int process_10100606(Logic_Player &player, Block_Buffer &buf);
	int process_10100608(Logic_Player &player, Block_Buffer &buf);
	int process_10100609(Logic_Player &player, Block_Buffer &buf);
	// mount end

	//flowers start
	int process_10120050(Logic_Player &player, Block_Buffer &buf);
	int process_10120051(Logic_Player &player, Block_Buffer &buf);
	//flowers end

	// secret_shop start
	int process_10120010(Logic_Player &player, Block_Buffer &buf);
	int process_10120011(Logic_Player &player, Block_Buffer &buf);
	//secret_shop end

	//reward start
	int process_10106309(Logic_Player &player, Block_Buffer &buf);

	int process_10106300(Logic_Player &player, Block_Buffer &buf);
	int process_10106301(Logic_Player &player, Block_Buffer &buf);
	int process_10106302(Logic_Player &player, Block_Buffer &buf);
	int process_10106303(Logic_Player &player, Block_Buffer &buf);
	int process_10106304(Logic_Player &player, Block_Buffer &buf);
	int process_10106305(Logic_Player &player, Block_Buffer &buf);

	int process_10106306(Logic_Player &player, Block_Buffer &buf);
	int process_10106307(Logic_Player &player, Block_Buffer &buf);
	int process_10106308(Logic_Player &player, Block_Buffer &buf);
	//reward end

	// wipe out single dungeon start
	int process_10100210(Logic_Player &player, Block_Buffer &buf);
	int process_10100211(Logic_Player &player, Block_Buffer &buf);
	int process_10100212(Logic_Player &player, Block_Buffer &buf);
	int process_10100213(Logic_Player &player, Block_Buffer &buf);
	int process_10100214(Logic_Player &player, Block_Buffer &buf);
	// wipe out single dungeon end

	// campfire start
	int process_10100700(Logic_Player &player, Block_Buffer &buf);
	// campfire end

	//
	int process_10120070(Logic_Player &player, Block_Buffer &buf);
	//

	int process_10100040(Logic_Player &player, Block_Buffer &buf);

	//active content start
	int process_10106400(Logic_Player &player, Block_Buffer &buf);
	int process_10106401(Logic_Player &player, Block_Buffer &buf);
	int process_10106402(Logic_Player &player, Block_Buffer &buf);
	int process_10106403(Logic_Player &player, Block_Buffer &buf);
	int process_10106404(Logic_Player &player, Block_Buffer &buf);
	int process_10106405(Logic_Player &player, Block_Buffer &buf);
	int process_10106406(Logic_Player &player, Block_Buffer &buf);
	int process_10106407(Logic_Player &player, Block_Buffer &buf);
	int process_10106408(Logic_Player &player, Block_Buffer &buf);
	int process_10106409(Logic_Player &player, Block_Buffer &buf);
	int process_10106410(Logic_Player &player, Block_Buffer &buf);
	int process_10106411(Logic_Player &player, Block_Buffer &buf);
	int process_10106412(Logic_Player &player, Block_Buffer &buf);
	int process_10106413(Logic_Player &player, Block_Buffer &buf);
	int process_10106420(Logic_Player &player, Block_Buffer &buf);
	int process_10106421(Logic_Player &player, Block_Buffer &buf);
	int process_10106422(Logic_Player &player, Block_Buffer &buf);
	int process_10106423(Logic_Player &player, Block_Buffer &buf);
	int process_10106424(Logic_Player &player, Block_Buffer &buf);
	int process_10106425(Logic_Player &player, Block_Buffer &buf);
	int process_10106426(Logic_Player &player, Block_Buffer &buf);
	int process_10106427(Logic_Player &player, Block_Buffer &buf);
	int process_10106431(Logic_Player &player, Block_Buffer &buf);
	int process_10106448(Logic_Player &player, Block_Buffer &buf);
	int process_10106449(Logic_Player &player, Block_Buffer &buf);
	int process_10106428(Logic_Player &player, Block_Buffer &buf);
	int process_10106429(Logic_Player &player, Block_Buffer &buf);
	int process_10106430(Logic_Player &player, Block_Buffer &buf);
	int process_10106432(Logic_Player &player, Block_Buffer &buf);
	int process_10106433(Logic_Player &player, Block_Buffer &buf);
	int process_10106434(Logic_Player &player, Block_Buffer &buf);
	int process_10106435(Logic_Player &player, Block_Buffer &buf);
	int process_10106436(Logic_Player &player, Block_Buffer &buf);
	int process_10106437(Logic_Player &player, Block_Buffer &buf);
	int process_10106438(Logic_Player &player, Block_Buffer &buf);
	int process_10106439(Logic_Player &player, Block_Buffer &buf);
	int process_10106440(Logic_Player &player, Block_Buffer &buf);
	int process_10106441(Logic_Player &player, Block_Buffer &buf);
	int process_10106442(Logic_Player &player, Block_Buffer &buf);
	int process_10106443(Logic_Player &player, Block_Buffer &buf);
	int process_10106444(Logic_Player &player, Block_Buffer &buf);
	int process_10106445(Logic_Player &player, Block_Buffer &buf);
	int process_10106446(Logic_Player &player, Block_Buffer &buf);
	int process_10106447(Logic_Player &player, Block_Buffer &buf);
	//active content end

	int process_10120052(Logic_Player &player, Block_Buffer &buf);

	int process_10120090(Logic_Player &player, Block_Buffer &buf);
	int process_10120091(Logic_Player &player, Block_Buffer &buf);
	int process_10120092(Logic_Player &player, Block_Buffer &buf);
	int process_10120093(Logic_Player &player, Block_Buffer &buf);
	int process_10120094(Logic_Player &player, Block_Buffer &buf);

	// fashion start
//	int process_10105001(Logic_Player &player, Block_Buffer &buf);
	int process_10105002(Logic_Player &player, Block_Buffer &buf);
//	int process_10105003(Logic_Player &player, Block_Buffer &buf);
//	int process_10105004(Logic_Player &player, Block_Buffer &buf);
//	int process_10105005(Logic_Player &player, Block_Buffer &buf);
//	int process_10105006(Logic_Player &player, Block_Buffer &buf);
	int process_10105007(Logic_Player &player, Block_Buffer &buf);

	int process_10105020(Logic_Player &player, Block_Buffer &buf);
	int process_10105021(Logic_Player &player, Block_Buffer &buf);
	int process_10105022(Logic_Player &player, Block_Buffer &buf);
	// wing's handler
	int process_10105011(Logic_Player &player, Block_Buffer &buf);
	int process_10105012(Logic_Player &player, Block_Buffer &buf);
	int process_10105013(Logic_Player &player, Block_Buffer &buf);
	int process_10105014(Logic_Player &player, Block_Buffer &buf);
	int process_10105015(Logic_Player &player, Block_Buffer &buf);
	int process_10105016(Logic_Player &player, Block_Buffer &buf);
	int process_10105017(Logic_Player &player, Block_Buffer &buf);
	int process_10105018(Logic_Player &player, Block_Buffer &buf);
	// fashion end

	//auction
	int process_10107000(Logic_Player &player, Block_Buffer &buf);
	int process_10107001(Logic_Player &player, Block_Buffer &buf);
	int process_10107002(Logic_Player &player, Block_Buffer &buf);
	int process_10107003(Logic_Player &player, Block_Buffer &buf);
	int process_10107004(Logic_Player &player, Block_Buffer &buf);

	//knight
	int process_10107100(Logic_Player &player, Block_Buffer &buf);
	int process_10107101(Logic_Player &player, Block_Buffer &buf);
	int process_10107102(Logic_Player &player, Block_Buffer &buf);
	int process_10107103(Logic_Player &player, Block_Buffer &buf);
	int process_10107104(Logic_Player &player, Block_Buffer &buf);

	// anci_artifact
	int process_10101100(Logic_Player &player, Block_Buffer &buf);
	int process_10101101(Logic_Player &player, Block_Buffer &buf);
	int process_10101102(Logic_Player &player, Block_Buffer &buf);
	int process_10101103(Logic_Player &player, Block_Buffer &buf);

	//invest plan
	int process_10101200(Logic_Player &player, Block_Buffer &buf);
	int process_10101201(Logic_Player &player, Block_Buffer &buf);
	int process_10101202(Logic_Player &player, Block_Buffer &buf);
	int process_10101203(Logic_Player &player, Block_Buffer &buf);
	int process_10101204(Logic_Player &player, Block_Buffer &buf);
	int process_10101205(Logic_Player &player, Block_Buffer &buf);
	int process_10101206(Logic_Player &player, Block_Buffer &buf);
	int process_10101207(Logic_Player &player, Block_Buffer &buf);

	// expedition
	int process_10101300(Logic_Player &player, Block_Buffer &buf);
	int process_10101301(Logic_Player &player, Block_Buffer &buf);

	//thorlottery
	int process_10108001(Logic_Player &player, Block_Buffer &buf);
	int process_10108002(Logic_Player &player, Block_Buffer &buf);
	int process_10108003(Logic_Player &player, Block_Buffer &buf);

	//card
	int process_10109002(Logic_Player &player, Block_Buffer &buf);
	int process_10109003(Logic_Player &player, Block_Buffer &buf);
	int process_10109005(Logic_Player &player, Block_Buffer &buf);
	int process_10109006(Logic_Player &player, Block_Buffer &buf);
	int process_10109007(Logic_Player &player, Block_Buffer &buf);
	int process_10109008(Logic_Player &player, Block_Buffer &buf);
	int process_10109009(Logic_Player &player, Block_Buffer &buf);
	int process_10109010(Logic_Player &player, Block_Buffer &buf);

	int process_10120097(Logic_Player &player, Block_Buffer &buf);
	int process_10120098(Logic_Player &player, Block_Buffer &buf);
	int process_10120099(Logic_Player &player, Block_Buffer &buf);
	int process_10120100(Logic_Player &player, Block_Buffer &buf);
	int process_10120101(Logic_Player &player, Block_Buffer &buf);
	int process_10120102(Logic_Player &player, Block_Buffer &buf);

	int process_10120110(Logic_Player &player, Block_Buffer &buf);

	int process_10106900(Logic_Player &player, Block_Buffer &buf);
	int process_10106901(Logic_Player &player, Block_Buffer &buf);

	int process_10120060(Logic_Player &player, Block_Buffer &buf);
	int process_10120061(Logic_Player &player, Block_Buffer &buf);

	int process_10112000(Logic_Player &player, Block_Buffer &buf);
	int process_10112001(Logic_Player &player, Block_Buffer &buf);

	//life_skill
	int process_10116000(Logic_Player &player, Block_Buffer &buf);
	int process_10116001(Logic_Player &player, Block_Buffer &buf);
	int process_10116002(Logic_Player &player, Block_Buffer &buf);
	int process_10116003(Logic_Player &player, Block_Buffer &buf);
	int process_10116004(Logic_Player &player, Block_Buffer &buf);
	int process_10116005(Logic_Player &player, Block_Buffer &buf);

	//oper activity
	int process_10117000(Logic_Player &player, Block_Buffer &buf);
	int process_10117001(Logic_Player &player, Block_Buffer &buf);
	int process_10117002(Logic_Player &player, Block_Buffer &buf);
	int process_10117003(Logic_Player &player, Block_Buffer &buf);

	// pet process
	int process_10113001(Logic_Player &player, Block_Buffer &buf);
	int process_10113002(Logic_Player &player, Block_Buffer &buf);
	int process_10113003(Logic_Player &player, Block_Buffer &buf);
	int process_10113004(Logic_Player &player, Block_Buffer &buf);
	int process_10113005(Logic_Player &player, Block_Buffer &buf);
	int process_10113006(Logic_Player &player, Block_Buffer &buf);
	int process_10113007(Logic_Player &player, Block_Buffer &buf);
	int process_10113008(Logic_Player &player, Block_Buffer &buf);
	int process_10113009(Logic_Player &player, Block_Buffer &buf);
	int process_10113010(Logic_Player &player, Block_Buffer &buf);

private:
	Logic_Monitor *monitor_;
};

typedef Singleton<Logic_Client_Request> Logic_Client_Request_Single;
#define LOGIC_CLIENT_REQUEST    (Logic_Client_Request_Single::instance())

inline Logic_Monitor *Logic_Client_Request::monitor(void) {
	return monitor_;
}

#endif /* LOGIC_CLIENT_REQUEST_H_ */
