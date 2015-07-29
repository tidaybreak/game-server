/*
 * Packer.h
 *
 *  Created on: Nov 26, 2013
 *      Author: linqiyou
 */

#ifndef PACKER_H_
#define PACKER_H_

#include "Logic_Player_Base.h"
#include "Pack_Def.h"
#include "Base_Struct.h"
#include "Msg_Struct.h"

using namespace Pack;

class Logic_Player;

struct Item_Detail;
struct Gain_Item_DM_Info;
struct Money_Add_Info;
struct Money_Sub_Info;
struct Money_DM_Info;
struct Move_Item_DM_Info;
struct Lose_Item_DM_Info;
struct Id_Amount;
struct Index_Amount;
struct Money_Detail;
struct Packer_Detail;
struct Packer_Detail;
struct MSG_50100100;
struct MSG_50100112;
struct Money_Stream_Info;
struct MSG_81000102;

class Packer : virtual public Logic_Player_Base {
public:
	static const int PACK_MAIL_ID = 1019;
	static const int PACK_MAIL_ID_NEW = 1106;
	typedef void (*Item_Handle_Func)(const Item_Detail &item, Logic_Player &player);
	typedef std::vector<Item_Detail> Item_Vec;
	typedef std::map<uint32_t, Item_Detail*> Goods_Map;
	typedef boost::unordered_map<uint32_t, Item_Detail*> Goods_Seq_Map;
	typedef std::vector<Id_Amount> Id_Amount_Vec;
	typedef std::vector<Index_Amount> Index_Amount_Vec;
	typedef std::vector<Money_Sub_Info> Money_Sub_List;
	typedef std::vector<Money_Add_Info> Money_Add_List;

	Packer();
	virtual ~Packer();

	// 创建角色时初始化
	static int create_init(Packer_Detail &detail);
	// 登录时detail赋值
	void load_detail(Packer_Detail &detail);
	// 保存时detail赋值
	void save_detail(Packer_Detail &detail,bool logout);

	inline Packer_Detail const &packer_detail(void) const;

	inline const Packer_Detail* packer_detail_pointer(void) const;

	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	int client_ready_now(void);

	void reset(void);
	void daily_refresh(void);
	void week_refresh(void);
	void tick(void);
	void recovery(void);
	void clear_item(Pack_Type pack_type);

	// msg
	int pack_fetch_package_info(const uint32_t &pack_type_msg);
	int pack_fetch_hero_equip_info(const Hero_Pack_Module hero_pack_module, const uint32_t &pack_type_msg);
	int pack_active_pack_rune_stone(Logic_Player* target_player);
	int pack_add_capacity(const uint8_t &pay_type, const uint32_t &pack_type, const uint16_t &add_capacity, const int8_t un_free = 1);
	int pack_discard_item(const std::vector<uint32_t> &index);
	int pack_move_item_msg(const uint32_t &index_from, const uint32_t &index_to);
	int pack_split_item(const uint32_t &index, const int16_t &split_num);
	int pack_sort_item(const uint32_t &pack_type);
	int pack_merge_item(const uint32_t &pack_type);
	int pack_sell_item(const Int_Int_Vec &index);
	int pack_move_tower_package_item(const uint32_t &index);
	int pack_move_item_from_package(const int8_t &type, const int32_t &index);
	//int pack_fetch_package_info_other(std::vector<Item_Basic_Info> &item_info,
	//		std::vector<Item_Equip_Info> &eq_info);
	int pack_fetch_package_info_other(MSG_50100112 &res_msg);
	int pack_fetch_role_panel_hero_equip(MSG_50100100 &msg_back);
	int pack_active_role_panel_hero_equip(Logic_Player* target_player);
	int pack_fetch_role_panel_hero_equip(MSG_50100112 &res_msg);
	int pack_fetch_role_panel_hero_rune_stone(MSG_50100100 &msg_back);
	int pack_fetch_role_panel_hero_rune_stone(MSG_50100112 &res_msg);

	// 客户端要求查看别人的装备信息用偏移量区分
	int pack_fetch_other_equip_info(Block_Buffer &basic_buf, Block_Buffer &addit_buf, int offset);

	// get item info
	int pack_get_item_id(const uint32_t index, uint32_t &id);
	int pack_calc_item(const Pack_Type pack_type);
	int pack_calc_item(const Pack_Type pack_type, const uint32_t id, Bind_Verify bind_verify = BIND_NONE, const Item_Vertify_By_Condition con = CON_START);
	int pack_calc_item_by_same_type(const Pack_Type pack_type, const uint32_t id);
	int pack_calc_unlock_item(const Pack_Type pack_type, const uint32_t id, Bind_Verify bind_verify = BIND_NONE);
	int pack_calc_item(const uint32_t index);
	bool is_item_exist(uint32_t index);
	inline bool is_item_can_sell(uint32_t index);
	void get_equip_index(const Pack_Type pack_type, Int_Vec &equip_index);
	void get_mount_equip_index(const Pack_Type pack_type, Int_Vec &equip_index);
	void get_card_indexes(const Pack_Type pack_type, Int_Vec &indexes);
	void pack_get_indexes(const Pack_Type pack_type, const uint32_t id, Int_Vec &indexes);
	uint32_t pack_get_capacity_max(void);
	uint32_t pack_get_next_lvl(void);

	// insert
	// 指定index插入装备
	int pack_insert_equip(const uint32_t index, const Item_Detail &item, const Gain_Item_DM_Info &dm_info);
	int pack_try_insert_item(const Pack_Type pack_type, const Item_Detail &item);
	int pack_insert_item(const Pack_Type pack_type, const Item_Detail &item, const Gain_Item_DM_Info &dm_info,
			TIPS_TYPE tips_type = GET_ITEM, Pack_Try pack_try = WITH_TRY, const int mail_id = PACK_MAIL_ID);
	int pack_try_insert_item(const Pack_Type pack_type, const Item_Vec &item_list);
	int pack_insert_item(const Pack_Type pack_type, const Item_Vec &item_list, const Gain_Item_DM_Info &dm_info,
			TIPS_TYPE tips_type = GET_ITEM, Pack_Try pack_try = WITH_TRY, const int mail_id = PACK_MAIL_ID);
	int pack_try_insert_item_direct(const Pack_Type pack_type, const Item_Vec &item_list);
	int pack_insert_item_direct(const Pack_Type pack_type, const Item_Vec &item_list, const Gain_Item_DM_Info &dm_info,
			TIPS_TYPE tips_type = GET_ITEM, Pack_Try pack_try = WITH_TRY, const int mail_id = PACK_MAIL_ID);
	// erase by pack_type id amount
	int pack_try_erase_item(const Pack_Type pack_type, const Id_Amount &id_amount, const Item_Vertify_By_Condition con = CON_START);
	int pack_erase_item(const Pack_Type pack_type, const Id_Amount &id_amount, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type = USE_ITEM, Pack_Try pack_try = WITH_TRY, const Item_Vertify_By_Condition con = CON_START);
	int pack_try_erase_item(const Pack_Type pack_type, const Id_Amount_Vec &id_amount_list, const bool need_tips = true);
	int pack_erase_item(const Pack_Type pack_type, const Id_Amount_Vec &id_amount_list, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type = USE_ITEM, Pack_Try pack_try = WITH_TRY);
	// erase by index amount
	int pack_try_erase_item(const Index_Amount &index_amount);
	int pack_erase_item(const Index_Amount &index_amount, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type = USE_ITEM, Pack_Try pack_try = WITH_TRY);
	int pack_try_erase_item(const Index_Amount_Vec &index_amount_list);
	int pack_erase_item(const Index_Amount_Vec &index_amount_list, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type = USE_ITEM, Pack_Try pack_try = WITH_TRY);
	// erase by index (discard)
	int pack_try_erase_item(const uint32_t index);
	int pack_erase_item(const uint32_t index, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type = DISCARD_ITEM, Pack_Try pack_try = WITH_TRY);
	int pack_try_erase_item(const UInt_Vec &index_list);
	int pack_erase_item(const UInt_Vec &index_list, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type = DISCARD_ITEM, Pack_Try pack_try = WITH_TRY);

	// move
	int pack_try_move_item(const uint32_t index_from, const uint32_t index_to);
	int pack_move_item(const uint32_t index_from, const uint32_t index_to, Pack_Try pack_try = WITH_TRY);
	int pack_try_move_item(const uint32_t index_from, const Pack_Type pack_type_to);
	int pack_move_item(const uint32_t index_from, const Pack_Type pack_type_to, Item_Handle_Func item_func = NULL, Pack_Try pack_try = WITH_TRY);
	int pack_move_item(const uint32_t index_from, const Pack_Type pack_type_to, UInt_Set &changed, Item_Handle_Func item_func = NULL, Pack_Try pack_try = WITH_TRY);
	int pack_try_move_item(const Pack_Type pack_type_from, const Pack_Type pack_type_to);
	int pack_move_item(const Pack_Type pack_type_from, const Pack_Type pack_type_to, Item_Handle_Func item_func = NULL, Pack_Try pack_try = WITH_TRY);
	int pack_try_move_to_hero_pack(const uint32_t index_from, const uint32_t index_to);
	// 尽力移动每个物品格 如果没有一个物品被移动则认为是失败

	int pack_attempt_move_everyitem(const Pack_Type pack_type_from, const Pack_Type pack_type_to, int &move_item_nums, Item_Handle_Func item_func = NULL);

	// 此接口还是保留
	const Money_Detail &pack_get_money(void);

	int pack_try_add_money(const Money_Add_Info &info);
	int pack_add_money(const Money_Add_Info &info, Money_Opt_Type type = MONEY_OPT_REAL, Money_Tips tips = MONEY_TIPS_YES);
	int pack_try_add_money(const Money_Add_List &money_add_list);
	int pack_add_money(const Money_Add_List &money_add_list, Money_Opt_Type type = MONEY_OPT_REAL, Money_Tips tips = MONEY_TIPS_YES);

	// 返回能购买的数量
	int pack_get_buy_power(const Money_Sub_Type sub_type, int unit_price);
	int get_item_price(const std::vector<Id_Amount> &item_info, Money_Add_List &add_info, const std::vector<const Item_Detail*> &item_vec);//获得所传物品列表价格

	int pack_try_sub_money(const Money_Sub_Info &info);
	int pack_sub_money(const Money_Sub_Info &info, Money_Opt_Type type = MONEY_OPT_REAL, Money_Tips tips = MONEY_TIPS_YES);
	int pack_try_sub_money(const Money_Sub_List &money_sub_list);
	int pack_sub_money(const Money_Sub_List &money_sub_list, Money_Opt_Type type = MONEY_OPT_REAL, Money_Tips tips = MONEY_TIPS_YES);

	int pack_is_pack_type(const int32_t index, Pack_Type pack_type);
	int pack_is_hero_type(const Hero_Pack_Module hero_pack_module, const int32_t type_or_index);
	bool pack_is_hero_type(const int32_t type_or_index);
	int pack_get_pack_type(const int32_t index, Pack_Type &pack_type);
	int pack_get_hero_pack_type(const int32_t pack_index);
	int pack_get_hero_pack_type(const Hero_Pack_Module hpm, const int32_t hero_index);
	int pack_get_hero_pack_module(Hero_Pack_Module &hpm, const int32_t type_or_index);
	int pack_get_hpm(const int32_t type_or_index);

	int get_pack_storage_move_err_code(const int32_t index,int &err_code);
	// 按金钱扣除类型加解锁
	inline void lock_money_by_sub_type(Money_Sub_Type type);
	inline void unlock_money_by_sub_type(Money_Sub_Type type);
	// lock
	void lock_money(Money_Type type);
	void lock_item(uint32_t index);

	void unlock_money(Money_Type type);
	void unlock_item(uint32_t index);

	void set_change(void);

	// 剩余格数
	inline int pack_get_remain_capacity(const Pack_Type pack_type) {
		return pack_get_capacity(pack_type) - pack_get_size(pack_type);
	}
	int pack_get_size(const Pack_Type pack_type);

	inline void set_last_wipe_time(void);
	inline const Time_Value &get_last_wipe_time(void);

	const Item_Detail *pack_get_const_item(const uint32_t index);

	// hook
	int pack_hook_sell_item(const Id_Amount_Vec &id_amount_vec);
	// hook

	int pack_discard_all(const Pack_Type pack_type);

	//任务调用
	int pack_get_capacity_public(const Pack_Type pack_type);
	int pack_get_hero_capacity_public(const int pack_type);

	int client_get_item_effect(const Item_Vec &item_list,const Money_Add_List &money_add_list,MSG_81000102 &res_msg);

protected:
	Item_Detail *pack_get_item(const uint32_t index);
	Item_Detail *pack_get_item(const Pack_Type pack_type, const int32_t item_id, Bind_Verify bind_verify = BIND_NONE);
	Item_Detail *pack_get_item_hero(const int pack_type, const int32_t item_id, Bind_Verify bind_verify = BIND_NONE);

	int pack_get_capacity(const Pack_Type pack_type);
	int pack_get_hero_capacity(const int pack_type);

	int pack_inner_set_cap(const Pack_Type pack_type, const uint32_t set_cap);

	int pack_sort_item(const Pack_Type pack_type, MERGE_WAY merge_way);

	// active informing
	// money
	int pack_active_update_money(void);
	// capacity
	int pack_active_update_capacity(const Pack_Type pack_type);
	int pack_active_update_hero_capacity(const int pack_type);
	// item
	int pack_active_update_item_detail(const UInt_Set *changed_index, const Update_From_Type from_type = FROM_NULL);
	int pack_active_add_item_detail(const UInt_Set *changed_index, const Item_Vec *item_vec); // 背包模块（获得物品客户端飘特效\时效处理）
	int pack_active_update_hero_pack_detail(const UInt_Set *changed_index);

	Pack_Type check_and_change_pack_type(const Pack_Type pack_type, int id);

	void write_item_gain_stream(const Gain_Item_DM_Info &gain_dm_info, int item_id, int amount, int bind, const Pack_Type pack_type = PACK_T_PACKAGE_INDEX);
	void write_item_lose_stream(const Lose_Item_DM_Info &lose_dm_info, int item_id, int amount, int bind, const Pack_Type pack_type = PACK_T_PACKAGE_INDEX);
private:
	// 确定背包的容积是否足够(不叠放情况下)
	int pack_try_insert_to_empty_index_direct(const Pack_Type pack_type, const Item_Detail &item);
	// 向背包中放入批量物品，先尝试叠放，再尝试放入新的格子中
	int pack_insert_to_exist_index_first(const Pack_Type pack_type, const Item_Vec &item_list,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try = WITH_TRY);
	// 向背包中放入物品，先尝试叠放，再尝试放入新的格子中
	int pack_insert_to_exist_index_first(const Pack_Type pack_type, const Item_Detail &item,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try = WITH_TRY);
	// 向背包中放入物品，只放入新的格子中
	int pack_insert_to_empty_index_direct(const Pack_Type pack_type, const Item_Detail &item,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try = WITH_TRY);

	// 移除物品不指定位置（若移除0个或负数个物品，返回成功，但不做任何操作）
	int pack_erase_item(const Pack_Type pack_type, const Id_Amount &id_amount, const Lose_Item_DM_Info &lose_dm_info, UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY, const Item_Vertify_By_Condition con = CON_START);
	// 移除物品不指定位置（批量）
	int pack_erase_item(const Pack_Type pack_type, const Id_Amount_Vec &id_amount_list, const Lose_Item_DM_Info &lose_dm_info,
			UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置指定数量物品
	int pack_erase_item(const Index_Amount &index_amount, const Lose_Item_DM_Info &lose_dm_info, UInt_Set *changed_set, TIPS_TYPE tips_type = TIPS_NONE, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置指定数量物品（批量）
	int pack_erase_item(const Index_Amount_Vec &index_amount_list, const Lose_Item_DM_Info &lose_dm_info, UInt_Set *changed_set, TIPS_TYPE tips_type = TIPS_NONE, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置所有物品
	int pack_erase_item(const uint32_t index, const Lose_Item_DM_Info &lose_dm_info, UInt_Set *changed_set, TIPS_TYPE tips_type = TIPS_NONE, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置所有物品（批量）
	int pack_erase_item(const UInt_Vec &index_list, const Lose_Item_DM_Info &lose_dm_info, UInt_Set *changed_set, TIPS_TYPE tips_type = TIPS_NONE, Pack_Try pack_try = WITH_TRY);

	int pack_set_money(const Money_Detail &money_detail, const Money_DM_Info &info, Money_Opt_Type type);
	void pack_send_money_tips(const Money_Detail &old_money_detail, const Money_Detail &new_money_detail);
	int request_money_stream(Money_Stream_Info &money_stream_info);
	int trigger_diamond_sub_info(const int32_t sub_num, const Pack::Money_Lose_Type type);

	inline void pack_get_iter(const Pack_Type pack_type, Goods_Map::iterator &begin, Goods_Map::iterator &end);
	inline void pack_get_iter_hero(const int pack_type, Goods_Map::iterator &begin, Goods_Map::iterator &end);
	inline void pack_get_index(const Pack_Type pack_type, uint32_t &begin, uint32_t &end);
	inline void pack_get_index(const int pack_type, uint32_t &begin, uint32_t &end);

	inline bool is_money_lock(Money_Type type);
	inline bool is_item_lock(uint32_t index);

	inline bool is_money_lock(void);
	inline bool is_item_lock(void);

	inline Item_Detail *pack_get_item_pointer(const uint32_t index);

	void write_item_move_stream(const Move_Item_DM_Info &move_dm_info, int item_id, int amount, int bind);
	void task_listen_item_modify(int item_id, int amount = 0, int bind = 0);

	int check_trade_pay_limit(const Money_Sub_Info &info, Money_Opt_Type type);
	int modify_trade_pay_limit(const Money_Sub_Info &info, Money_Opt_Type type);
	int check_trade_get_limit(const Money_Add_Info &info, Money_Opt_Type type);
	int modify_trade_get_limit(const Money_Add_Info &info, Money_Opt_Type type);
	void active_client_trade_limit_info(int money_type, int trade_type, int limit_left, int limit_total);
	void pack_modify_item_before_insert(Item_Detail &item);
	void get_move_stream(const uint32_t index_from, const uint32_t index_to,Move_Item_DM_Info &dm_info);
	void get_move_stream(const uint32_t index_from, const Pack_Type pack_type,Move_Item_DM_Info &dm_info);

//	// 时效处理
//	int pack_insert_timeptr(Item_Detail &item);
//	int pack_insert_timeptr(Item_Vec &item_list);
private:
	uint32_t seq_generator_;
	Packer_Detail *packer_detail_;
	uint32_t save_time_;                        // 背包保存时间特殊处理
};

inline Packer_Detail const &Packer::packer_detail(void) const{
	return *packer_detail_;
}

inline const Packer_Detail* Packer::packer_detail_pointer(void) const{
	return packer_detail_;
}

/*
 * BIND_COPPER_FIRST = 1,
	COPPER_ONLY = 2,
	BIND_COPPER_ONLY = 3,
	COUPON_ONLY = 4,
	GOLD_ONLY = 5,
	GOLD_BOND_ONLY = 6,
	MALL_SCORE_ONLY = 7,
	MILITARY_EXPLOIT_ONLY = 8*/

inline void Packer::lock_money_by_sub_type(Money_Sub_Type type) {
	switch (type) {
	case COPPER_ONLY:
		this->lock_money(COPPER);
		break;
	case BIND_GOLD_FIRST:
		this->lock_money(BIND_GOLD);
		this->lock_money(GOLD);
		break;
	case GOLD_ONLY:
		this->lock_money(GOLD);
		break;
	case BIND_GOLD_ONLY:
		this->lock_money(BIND_GOLD);
		break;
	case DRAGON_SOUL_ONLY:
		this->lock_money(DRAGON_SOUL);
		break;
	case SOULS_ONLY:
		this->lock_money(SOULS);
		break;
	case FRIENDSHIP_ONLY:
		this->lock_money(FRIENDSHIP);
		break;
	case REPUTATION_ONLY:
		this->lock_money(REPUTATION);
		break;
	default:
		MSG_USER_TRACE("wrong Money_Sub_Type : %d", type);
		break;
	}
}

inline void Packer::unlock_money_by_sub_type(Money_Sub_Type type) {
	switch (type) {
	case COPPER_ONLY:
		this->unlock_money(COPPER);
		break;
	case BIND_GOLD_FIRST:
		this->unlock_money(BIND_GOLD);
		this->unlock_money(GOLD);
		break;
	case GOLD_ONLY:
		this->unlock_money(GOLD);
		break;
	case BIND_GOLD_ONLY:
		this->unlock_money(BIND_GOLD);
		break;
	case DRAGON_SOUL_ONLY:
		this->unlock_money(DRAGON_SOUL);
		break;
	case SOULS_ONLY:
		this->unlock_money(SOULS);
		break;
	case FRIENDSHIP_ONLY:
		this->unlock_money(FRIENDSHIP);
		break;
	case REPUTATION_ONLY:
		this->unlock_money(REPUTATION);
		break;
	default:
		MSG_USER_TRACE("wrong Money_Sub_Type : %d", type);
		break;
	}
}

#endif /* PACKER_H_ */
