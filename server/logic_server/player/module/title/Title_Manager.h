/*
 * Title_Manager.h
 *
 *  Created on: 2014年7月25日
 *      Author: Linqiyou
 */

#ifndef TITLE_MANAGER_H_
#define TITLE_MANAGER_H_

#include "Title_Struct.h"
#include "ranking/Ranking_Def.h"

class Title_Manager {
public:
	const static int TITLE_AMOUNT_ALL = 30;
	Title_Manager(void);
	virtual ~Title_Manager(void);
	void start(void);
	int load_detail(void);
	int save_detail(void);
	void reset(void);

	//更新排行榜称号
	int update_charts_title(Title_Record_Detail::Charts_Info_Vector &charts_info_vector, RankingDef::Ranking_Type charts_type);

	//排行榜称号处理   type  1:增加称号  2:移除称号
	int charts_title_process(Title_Record_Detail::Charts_Info_Vector &charts_info_vector, int type);

	//获得排行榜称号id
	int get_charts_title_id(int charts_type, int rank, int career, int &title_id);

	//转职称号
	int add_change_career_title(role_id_t role_id, int career);

	//添加好友称号
	int add_friend_title(role_id_t role_id, int friend_amount);

	int add_gang_war_title(const role_id_t leader_id);

	//称号处理, type(1：增加称号，2：删除称号)
	int title_deal(role_id_t role_id, int title_id, int type, int equip);
	int add_equip_title(int title_id, Int_Vec &equip_title);
	int erase_equip_title(int title_id, Int_Vec &equip_title);
	int get_title_type(int title_id);
	bool handle_title(role_id_t role_id, int title_id);

	Title_Record_Detail &title_record_detail(void);
private:
	Title_Record_Detail title_record_detail_;
};

typedef Singleton<Title_Manager> Title_Manager_Single;

#define TITLE_MANAGER Title_Manager_Single::instance()

#endif /* TITLE_MANAGER_H_ */
