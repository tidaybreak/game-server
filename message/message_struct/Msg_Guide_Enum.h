/*  
* Generate by devtool
*/

#ifndef _MSG_GUIDE_H_ENUM_
#define _MSG_GUIDE_H_ENUM_

enum Msg_Guide_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_NEW_HAND_GUIDE = 10104000,				//请求新手引导         

		RES_NEW_HAND_GUIDE = 50104000,				//请求新手引导(返回)         

		REQ_NEW_HAND_GUIDE_SET = 10104001,				//请求新手引导设置         

		REQ_SAVE_PLAYED_PLOT = 10104002,				//请求保存已播放的剧情         

		RES_SAVE_PLAYED_PLOT = 50104002,				//请求保存已播放的剧情(返回)         

		REQ_TASK_GUIDE = 10104003,				//请求设置任务引导进度         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_GUIDE_H_ENUM_ */
