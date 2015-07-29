/*
 * Title.h
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#ifndef TITLE_H_
#define TITLE_H_

#include "Logic_Player_Base.h"

struct Title_Detail;

class Title : virtual public Logic_Player_Base {
public:
	Title(void);
	~Title(void);
	void reset(void);

	static int create_init(Title_Detail &detail);
	void load_detail(Title_Detail &detail);
	void save_detail(Title_Detail &detail);
	Title_Detail const &title_detail(void) const;
	void module_init(void);
	void sign_in(void);
	void sign_in_repeat(void);
	void sign_out(void);
	void client_ready_now(void);
	// 场景加载完成
	int move_scene_completed(void);
	//托管中下线调用
	void trusteeship_sign_out(void);
	void time_up(void);

	int req_fetch_title_info(void);
	int req_title_on_off(const int32_t title_id, const int8_t op);

	int add_title(int title_id, const bool is_module_init = false);
	int add_rank_title(int title_id);
//	int replace_title(int title_id, bool sync = true);	//替换称号,会增加新的称号，删除同类型的旧称号
	int erase_title(const int title_id, const bool need_erase_list = true);
	bool has_title(int title_id);

	int get_title_info(int &equip_title);
	int sync_title_info_to_map(void);

	int title_listen_task_finish(void);
	int title_property_refresh(bool sync = true);
	int get_title_type(int title_id);
	int add_rank_title(void);

	int title_listen_recharge(void);
	int title_listen_exploit(void);
	int title_listen_contribute(void);
	int title_listen_war_scene_win(void);
	int title_listen_war_scene_kill(void);
	int title_listen_arena_win(void);
	int title_listen_cumulate_equip(void);
	int title_listen_level(void);
	int title_listen_boss(void);
	int title_listen_ancient_instance(void);

	int title_auto_equip_title(void);
	bool title_check_cumulate_equip(void);
protected:
	int title_construct(void);
	int title_construct_recharge(void);
	int title_construct_exploit(void);
	int title_construct_contribute(void);
	int title_construct_war_scene_win(void);
	int title_construct_war_scene_kill(void);
	int title_construct_arena_win(void);
	int title_construct_cumulate_equip(void);
	int title_construct_level(void);
	int title_construct_boss(void);
	int title_construct_ancient_instance(void);


	void title_send_mail(const int id);

	void save_tick(void);
private:
	Title_Detail *title_detail_;
	Int_Int_Map title_map_recharge;
	Int_Int_Map title_map_exploit;
	Int_Int_Map title_map_contribute;
	Int_Int_Map title_map_war_scene_win;
	Int_Int_Map title_map_war_scene_kill;
	Int_Int_Map title_map_arena_win;
	Int_Int_Map title_map_cumulate_equip;
	Int_Int_Map title_map_level;
	Int_Int_Map title_map_boss;
	Int_Int_Map title_map_ancient_instance;
	int equip_title;
};

#endif /* TITLE_H_ */
