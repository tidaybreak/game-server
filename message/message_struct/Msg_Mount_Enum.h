/*  
* Generate by devtool
*/

#ifndef _MSG_MOUNT_H_ENUM_
#define _MSG_MOUNT_H_ENUM_

enum Msg_Mount_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_FETCH_MOUNT_INFO = 10100600,				//请求坐骑信息         

		RES_FETCH_MOUNT_INFO = 50100600,				//请求坐骑信息返回         

		REQ_FETCH_MOUNT_STABLE_INFO = 10100601,				//请求坐骑马厩信息         

		RES_FETCH_MOUNT_STABLE_INFO = 50100601,				//请求坐骑马厩信息返回         

		REQ_MOUNT_RIDE = 10100602,				//请求坐骑骑乘         

		RES_MOUNT_RIDE = 50100602,				//请求坐骑骑乘返回         

		REQ_MOUNT_ACTIVATE = 10100603,				//请求坐骑激活         

		RES_MOUNT_ACTIVATE = 50100603,				//请求坐骑激活返回         

		REQ_MOUNT_CLICK_NEW_GROUP = 10100604,				//请求点击坐骑新组合         

		RES_MOUNT_CLICK_NEW_GROUP = 50100604,				//请求点击坐骑新组合返回         

		REQ_MOUNT_TRAIN = 10100605,				//请求坐骑培养         

		RES_MOUNT_TRAIN = 50100605,				//请求坐骑培养返回         

		REQ_MOUNT_DEPLOY = 10100606,				//请求坐骑上阵         

		RES_MOUNT_DEPLOY = 50100606,				//请求坐骑上阵返回         

		REQ_MOUNT_LEARN_SKILL = 10100608,				//请求坐骑学习技能         

		RES_MOUNT_LEARN_SKILL = 50100608,				//请求坐骑学习技能返回         

		REQ_MOUNT_FUSE_TIME = 10100609,				//请求坐骑融合生效时间         

		RES_MOUNT_FUSE_TIME = 50100609,				//请求坐骑融合生效时间返回         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_MOUNT_H_ENUM_ */
