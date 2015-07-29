/*
 * Title_Struct.h
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#ifndef TITLE_STRUCT_H_
#define TITLE_STRUCT_H_

#include "Server_Struct.h"

struct Title_Detail_Info_Detail {
	Title_Detail_Info_Detail(void) {
		reset();
	}
	void reset(void) {
		id = 0;
		last_time = 0;
		start_time = Time_Value::zero;
		last_end_time = Time_Value::zero;
	}
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	int32_t id;
	int32_t last_time;
	Time_Value start_time;
	Time_Value last_end_time;
};
typedef boost::unordered_map<int32_t, Title_Detail_Info_Detail> Title_Detail_Info;
typedef std::list<Title_Detail_Info_Detail *> Title_Detail_Info_List;

//称号信息结构体
struct Title_Detail: public Detail {
	Title_Detail(void) {
		reset();
	}
	virtual int serialize(Block_Buffer &buffer) const;
	virtual int deserialize(Block_Buffer &buffer);
	virtual int load(void);
	virtual int save(void);
	virtual void reset(void);

	role_id_t role_id;					//角色ID
	Title_Detail_Info title_set;		//称号集合
	Title_Detail_Info_List title_time_list;// 限时处理
	int32_t equip_title;				//装备的称号
};

//称号记录
struct Title_Record_Detail: public Detail {
	Title_Record_Detail(void) {
		reset();
	}
	virtual int serialize(Block_Buffer &buffer) const;
	virtual int deserialize(Block_Buffer &buffer);
	virtual int load(void);
	virtual int save(void);
	virtual void reset(void);

	struct Charts_Info {
		role_id_t role_id;
		int title_id;
	};

	typedef std::vector<Charts_Info> Charts_Info_Vector;
	typedef boost::unordered_map<int, Charts_Info_Vector> Charts_Title_Map;
	typedef boost::unordered_map<int, Role_Id_Set> Title_Role_Map;

	Charts_Title_Map charts_title_map;			//上一次排行榜称号信息
	Title_Role_Map title_role_map;
	role_id_t gang_war_first_leader;			// 第一会长
};

#endif /* TITLE_STRUCT_H_ */
