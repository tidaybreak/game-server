/*  
* Generate by devtool
*/

#ifndef _MSG_ANCI_ARTIFACT_H_ENUM_
#define _MSG_ANCI_ARTIFACT_H_ENUM_

enum Msg_Anci_Artifact_Enum {	
	/* CLIENT MSG_ */
/**/

/* SERVER MSG */
		REQ_ANCI_ARTIFACT_LIST = 10101100,				//获取神器信息         

		RES_ANCI_ARTIFACT_LIST = 50101100,				//获取神器信息（返回）         

		REQ_ANCI_ARTIFACT_UNBAN = 10101101,				//上古神器解封         

		RES_ANCI_ARTIFACT_UNBAN = 50101101,				//上古神器解封(返回)         

		REQ_ANCI_ARTIFACT_ACTIVE = 10101102,				//上古神器条件激活         

		RES_ANCI_ARTIFACT_ACTIVE = 50101102,				//上古神器条件激活（返回）         

		REQ_ANCI_ARTIFACT_BOX_GET = 10101103,				//上古神器完美宝箱领取         

		RES_ANCI_ARTIFACT_BOX_GET = 50101103,				//上古神器完美宝箱领取（返回）         


	
/* SERVER INITIATIVE MSG */
/**/
};

#endif /* _MSG_ANCI_ARTIFACT_H_ENUM_ */
