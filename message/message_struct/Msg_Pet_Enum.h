/*  
* Generate by devtool
*/

#ifndef _MSG_PET_H_ENUM_
#define _MSG_PET_H_ENUM_

enum Msg_Pet_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_GET_PET_INFO = 10113001,				//获取宠物信息         

		RESP_GET_PET_INFO = 50113001,				//获取宠物信息(返回）         

		REQ_PET_UPGRADE = 10113002,				//宠物升级, 进阶         

		RESP_PET_UPGRADE = 50113002,				//宠物升级, 进阶(返回）         

		REQ_PET_AUTO_UPGRADE = 10113003,				//宠物自动升级，进阶         

		RESP_PET_AUTO_UPGRADE = 50113003,				//宠物自动升级，进阶(返回)         

		REQ_ACTIVE_PET = 10113004,				// 宠物激活         

		RESP_ACTIVE_PET = 50113004,				// 宠物激活(返回）         

		REQ_PET_ILLUSION = 10113005,				//宠物幻化         

		RESP_PET_ILLUSION = 50113005,				//宠物幻化(返回)         

		REQ_ACTIVE_PET_ILLUSION = 10113006,				//物品激活宠物幻化         

		RESP_ACTIVE_PET_ILLUSION = 50113006,				//物品激活宠物幻化(返回)         

		REQ_GET_PET_ACTIVE_ITEM = 10113007,				//获取激活宠物物品         

		RESP_GET_PET_ACTIVE_ITEM = 50113007,				//获取激活宠物物品(返回)         

		REQ_PET_LEARN_SKILL_NORMAL = 10113008,				//宠物低级学习         

		RESP_PET_LEARN_SKILL_NORMAL = 50113008,				//宠物低级学习(返回)         

		REQ_PET_LEARN_SKILL_ADVANCE = 10113009,				//宠物高级学习         

		RESP_PET_LEARN_SKILL_ADVANCE = 50113009,				//宠物高级学习(返回)         

		REQ_CHANGE_PET_STATE = 10113010,				//改变宠物跟随状态         

		RESP_CHANGE_PET_STATE = 50113010,				//改变宠物跟随状态(返回）         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_PET_H_ENUM_ */
