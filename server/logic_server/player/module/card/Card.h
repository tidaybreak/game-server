/*
 * Card.h
 *
 *  Created on: 2014年11月25日
 *      Author: root
 */

#ifndef CARD_H_
#define CARD_H_

#include "pack/Packer.h"

struct Card_Info;
typedef std::vector<Card_Info> Card_Info_Vec;
struct Packer_Detail;
struct Item_Detail;

class Card : virtual public Packer {
public:
	Card(void) { reset(); }
	virtual ~Card(void);
	void reset(void);

	// 创建角色时初始化
	static int create_init(Packer_Detail &detail);

	// 登录时detail赋值
	void load_detail(Packer_Detail &detail);

	// 保存时detail赋值
	void save_detail(Packer_Detail &data);

//	Packer_Detail const &packer_detail(void) const;

	// 登录时module初始化,在这里发客户端消息无效
	void module_init(void);

	// 登录网关后后调用(只会的正常登录调用一次,顶号不会调)
	void sign_in(void);

	// 顶号时调用
	int sign_in_repeat(void);

	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	// 登出前调用
	void sign_out(void);

	// 场景加载完成
	int move_scene_completed(void);

	// 登录或转场景同步逻辑信息到场景
	int sync_scene_module_info(Block_Buffer &buf);

	// 每日0点刷新
	int trigger_daily_zero(const Time_Value &now);

	// 每周0点刷新
	int trigger_weekly_zero(const Time_Value &now);

	//托管中下线调用
	void trusteeship_sign_out(void);

public:
	int handle_embed_card(uint32_t from_index, uint32_t to_index, uint8_t loc, uint32_t card_index);

	int handle_upgrade(uint32_t card_id, std::vector<uint32_t> &card_id_vec);

	int handle_sell_card(int type, int card_index);

	int handle_open_card_hole(int equip_index, int8_t loc);

	int handle_merge_new_card(uint32_t card_index, const std::vector<uint32_t> &card_index_vec);

	int handle_get_vip_upgrade_times(void);

	int handle_vip_upgrade_card(uint32_t card_index);

	int handle_card_poten_upgrade(uint32_t card_index);

public:
	int embed_card(uint32_t from_index, uint32_t to_index, uint8_t loc, uint32_t card_index);

	int unembed_card(uint32_t from_index, uint32_t to_index, uint8_t loc, uint32_t card_index);

	void card_prop_refresh(const Item_Detail &equip, std::map<int, double> &prop_map);

	void get_all_embed_card_prop(std::map<int, double> &prop_map);

	void reset_card_location(const Item_Detail& equip);

private:
	void call_prop_refresh(uint32_t equip_index, int from);

	void check_and_refresh_prop(int pre_lv, int pos_lv, uint32_t card_index, int item_id);

	int sell_one_card(int card_index);

	int sell_cards_by_one_clicked(int card_qlt);
	int check_one_card_qlt(int qlt, Item_Detail* card, Index_Amount_Vec & index_amount_vec);
};

#endif /* CARD_H_ */
