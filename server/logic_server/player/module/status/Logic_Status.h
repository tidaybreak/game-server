/*
 * Logic_Status.h
 *
 *  Created on: 2014年6月12日
 *      Author: Linqiyou
 */

#ifndef LOGIC_STATUS_H_
#define LOGIC_STATUS_H_
#include "Logic_Player_Base.h"

struct Logic_Status_Detail;
struct Status_Effect_Argv;

class Logic_Status :virtual public Logic_Player_Base {
public:
	Logic_Status();
	virtual ~Logic_Status();

	// 创建角色时初始化
	static int create_init(Logic_Status_Detail &detail);
	// 登录时detail赋值
	void load_detail(Logic_Status_Detail &detail);
	// 保存时detail赋值
	void save_detail(Logic_Status_Detail &detail);

	Logic_Status_Detail const &logic_status_detail(void) const;

	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);
	int client_ready_now(void);	// 场景加载完成
	int move_scene_completed(void);

    void reset(void);

    void save_tick(void);

    int sync_status_module_info(Block_Buffer &buf);

	int load_status_info(Block_Buffer &buf);
	int sync_status_info(Block_Buffer &buf);

	double exp_sum(const int scene_id = 0);	// buff经验加成

	// msg
	int logic_insert_status(const Status_Effect_Argv &argv);
	// msg

	//inter
	bool has_status_id(const int status_id); // buff是否存在
	double find_rune_stone_smelt_status_rate(const int status_id); // 查找符石熔炼buff概率
	void logic_del_status(const int status_id);
	int special_status_handle(void);

private:
	Logic_Status_Detail *logic_status_detail_;
	Int_Hash_Set pre_send_to_client_;
};


#endif /* LOGIC_STATUS_H_ */
