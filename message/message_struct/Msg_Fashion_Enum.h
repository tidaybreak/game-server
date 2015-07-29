/*  
* Generate by devtool
*/

#ifndef _MSG_FASHION_H_ENUM_
#define _MSG_FASHION_H_ENUM_

enum Msg_Fashion_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FASHION_INFO = 10105001,				//获取已拥有的时装         

		RESP_FASHION_INFO = 50105001,				//获取已拥有的时装(返回)         

		REQ_ALTER_FASHION_STATE = 10105002,				//改变时装穿戴状态         

		RESP_ALTER_FASHION_STATE = 50105002,				//改变时装穿戴状态(返回）         

		REQ_MERGE_FASHION = 10105003,				//合成时装         

		REQ_UPGRADE_FASHION = 10105004,				//升级时装         

		REQ_FASHION_INHERIT = 10105005,				//时装传承         

		RES_MERGE_FASHION = 50105003,				//合成时装(返回)         

		RES_UPGRADE_FASHION = 50105004,				//升级时装(返回)         

		RES_FASHION_INHERIT = 50105005,				//时装传承(返回)         

		REQ_PICK_UP_CARD = 10105006,				//时装抽卡牌         

		RESP_PICK_UP_CARD = 50105006,				//时装抽卡牌(返回)         

		REQ_GET_WING_ITEM = 10105011,				//获取翅膀道具         

		RESP_GET_WING_ITEM = 50105011,				//获取翅膀道具(返回)         

		REQ_ACTIVE_WING = 10105012,				//激活翅膀道具         

		RESP_ACTIVE_WING = 50105012,				//激活翅膀道具(返回)         

		RESP_GET_WING_INFO = 50105013,				//获取翅膀信息(返回)         

		REQ_GET_WING_INFO = 10105013,				//获取翅膀信息         

		REQ_UPGRADE_WING = 10105014,				//翅膀进阶         

		RESP_UPGRADE_WING = 50105014,				//翅膀进阶(返回)         

		REQ_AUTO_UPGRADE_WING = 10105015,				//自动翅膀进阶         

		RESP_AUTO_UPGRADE_WING = 50105015,				//自动翅膀进阶(返回)         

		REQ_CHANG_WING_SHOWED = 10105016,				//显示/隐藏翅膀avatar         

		RESP_CHANG_WING_SHOWED = 50105016,				//显示/隐藏翅膀avatar(返回)         

		REQ_CHANG_FASHION_SHOW_STATE = 10105007,				//改变时装显示状态         

		RESP_CHANG_FASHION_SHOW_STATE = 50105007,				//改变时装显示状态(返回)         

		REQ_GET_RET_RWARD = 10105017,				//领取返利         

		RESP_GET_RET_RWARD = 50105017,				//领取返利(返回）         

		REQ_ACTIVE_WING_EXPERIENCE = 10105018,				//激活翅膀体验         

		RESP_GET_FASHION_INFO = 50105020,				//新版获取时装信息(返回）         

		REQ_GET_FASHION_INFO = 10105020,				//新版获取时装信息         

		REQ_USE_FASHION_ITEM = 10105021,				//使用时装道具         

		RESP_USE_FASHION_ITEM = 50105021,				//使用时装道具(返回）         

		REQ_FASHION_PART_UPGARDE = 10105022,				//升级时装部位         

		RESP_FASHION_PART_UPGARDE = 50105022,				//升级时装部位(返回)         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_FASHION_H_ENUM_ */
