/*  
* Generate by devtool
*/

#ifndef _MSG_ROLE_SCENE_H_ENUM_
#define _MSG_ROLE_SCENE_H_ENUM_

enum Msg_Role_Scene_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_PLAYER_LOGIN = 10100001,				//登录游戏         

		RES_PLAYER_LOGIN = 50100001,				//登录游戏(返回)         

		REQ_CREATE_ROLE = 10100005,				//创建角色         

		RES_CREATE_ROLE = 50100005,				//创建角色(返回)         

		REQ_FETCH_ROLE_LIST = 10100000,				//获取角色         

		RES_FETCH_ROLE_LIST = 50100000,				//获取角色(返回)         

		REQ_LOGIN_GATE = 10400001,				//登录网关         

		RES_LOGIN_GATE = 50400001,				//登录网关(返回)         

		RES_SCENE_TRANSFER = 50400005,				//切换场景(返回)         

		REQ_SCENE_TRANSFER = 10400005,				//切换场景(场景改变都用它)         

		REQ_OPEN_ROLE_PANEL = 10200032,				//打开人物面板         

		RES_OPEN_ROLE_PANEL = 50200032,				//打开人物面板(返回)         

		REQ_SET_PLOT_INFO = 10100010,				//设置剧情信息         

		REQ_FLY = 10200007,				//请求瞬移(坐电梯)         

		REQ_START_FLY = 10200005,				//请求飞行(坐电梯)         

		REQ_CLIENT_READY = 10100015,				//客户端初始化完成         

		REQ_BEAT_BACK = 10200008,				//移动场景击退请求         

		REQ_FCM_PASS = 10100090,				//通知防沉迷验证通过         

		REQ_ROLE_INFO = 10100006,				//从逻辑服获取角色信息         

		RES_ROLE_INFO = 50100006,				//获取角色信息(返回)         

		REQ_TAKE_LIFT_FINISH = 10200009,				//电梯到达请求         

		REQ_RANDOM_NAME = 10100007,				//获取角色随机名字         

		RES_RANDOM_NAME = 50100007,				//获取角色随机名字（返回）         

		RES_CLIENT_READY = 50100015,				//客户端初始化完成(返回)         

		REQ_ACCESS_DATE_COUNT = 10188888,				//访问数据统计         

		REQ_ROLE_INFO_SCENE = 10200006,				//从场景服获取人物信息         

		REQ_SCENE_COMPLETE = 10200030,				//场景加载完成         

		RES_SCENE_COMPLETE = 50200030,				//场景加载完成(返回)         

		REQ_RENAME = 10100008,				//请求改名         

		RES_RENAME = 50100008,				//请求改名（返回）         

		REQ_OPEN_ROLE_PANEL_LOGIC = 10100032,				//打开人物面板_逻辑服         

		RES_OPEN_ROLE_PANEL_LOGIC = 50100032,				//打开人物面板_逻辑服(返回)         

		REQ_ACT_ENTER_SCENE = 10100040,				//请求进入活动场景         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_ROLE_SCENE_H_ENUM_ */
