/*  
* Generate by devtool
*/

#ifndef _MSG_SCENE_OUTPOST_H_ENUM_
#define _MSG_SCENE_OUTPOST_H_ENUM_

enum Msg_Scene_Outpost_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_VAGRANT_ITEM_INFO = 10211300,				//请求流浪商人物品信息         

		RES_VAGRANT_ITEM_INFO = 50211300,				//请求流浪商人物品信息返回         

		REQ_VAGRANT_BUY_ITEM = 10211301,				//请求购买流浪商人物品         

		RES_VAGRANT_BUY_ITEM = 50211301,				//请求购买流浪商人物品返回         

		REQ_FETCH_TBAY_INFO = 10211310,				//获取宝藏海湾信息         

		RES_FETCH_TBAY_INFO = 50211310,				//获取宝藏海湾信息返回         

		REQ_ENTER_SCENE_OUTPOST = 10211311,				//请求进入副本关卡         

		RES_ENTER_SCENE_OUTPOST = 50211311,				//请求进入场景关卡返回         

		REQ_TBAY_BLESS = 10211312,				//请求宝藏海外神秘祝福         

		RES_TBAY_BLESS = 50211312,				//请求宝藏海湾神秘祝福返回         

		RES_FETCH_TBAY_GOLD_INFO = 50211313,				//宝藏海湾货币信息         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_SCENE_OUTPOST_H_ENUM_ */
