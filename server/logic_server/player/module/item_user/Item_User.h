/*
 * Item_User.h
 *
 *  Created on: Jan 10, 2013
 *      Author: "linqiyou"
 */

#ifndef ITEM_USER_H_
#define ITEM_USER_H_

#include "Logic_Player_Base.h"
#include "Item_User_Def.h"

namespace Json {
class Value;
}

class Packer;
struct Item_User_Detail;
struct Item_Prop_Effect_Config;

class Item_User: virtual public Packer {
public:
	enum Resource_Type {
		RESOURCE_TYPE_START = 1,
		RESOURCE_COPPER = 1,
		RESOURCE_EXP = 2,
		RESOURCE_SOULS = 3,
		RESOURCE_FRIENDSHIP = 4,
		RESOURCE_TYPE_END
	};
	Item_User(void);
	virtual ~Item_User(void);

	// 创建角色时初始化
	static int create_init(Item_User_Detail &detail);
	// 登录时detail赋值
	void load_detail(Item_User_Detail &detail);
	// 保存时detail赋值
	void save_detail(Item_User_Detail &detail);

	inline Item_User_Detail const &item_user_detail(void) const;

	// 登录时module初始化
	void module_init(void);
	// 登录后调用
	void sign_in(void);
	// 登出前调用
	void sign_out(void);

	void reset(void);

	int item_user_use_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value);
	int use_special_gift(const int32_t item_index, const Int_Vec &select_index_vector);
	int item_res_client_use_item(int result, uint32_t item_id);
	int req_buy_and_use(const int32_t item_id, const int32_t amount);

//	virtual void pack_clean_overdue(void);
	int use_drug_inner(const uint32_t index, const double val, const int amount, Block_Buffer &buf);

private:
	int use_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, uint32_t &id);
//	Item_Effect_Type get_item_type(uint32_t id);

	// use
	int check_item_param(const int amount, const Item_Detail *item, const Time_Value &now, const Time_Value &cd,
			const CD_Type &type);
	int use_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value,
			const Item_Detail *item, Item_Target target);
	int use_normal_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value,
			const Item_Detail *item, const Item_Prop_Effect_Config &effect, Item_Target target);
	int use_lottery_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_drug_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_material_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_card_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_resource_item(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_consume_tool(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_mount(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_gem_compose(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	int use_little_fun(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item, Item_Vec &item_info);
	// buy and use
	int use_normal_item(const Money_Sub_Info &money_sub_info, const Item_Detail &item, const Json::Value &effect,
			const Item_Target &target);
	int use_status_item(const Money_Sub_Info &money_sub_info, const Item_Detail &item, int status_id, int target);
	int use_lottery_item(const Money_Sub_Info &money_sub_info, const Item_Detail &item);

	static Item_Target get_item_target(const uint32_t item_id);
	static bool is_item_batch_using(const uint32_t item_id);
	static int get_item_cooldown_info(const uint32_t item_id, CD_Type &type, Time_Value &cd);
	int use_item_tran_sence(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item);
	int use_item_modify_gender(const uint32_t &index, const int32_t &amount, const int32_t &e_value, const Item_Detail *item);

private:
	void open_lottery_broadcast(const uint32_t item_id,const Item_Vec &item_list);
	Item_User_Detail *item_user_detail_;
};

inline Item_User_Detail const &Item_User::item_user_detail(void) const{
	return *item_user_detail_;
}

struct Effect
{
    static const std::string BLOOD;
    static const std::string MAGIC;
    static const std::string COMPREHENSION;
    static const std::string ANGER;
    static const std::string LEVEL_UP;
    static const std::string CHANGE_GENDER;

    static const std::string ADD_EXP;
	static const std::string ADD_COUPON;
	static const std::string ADD_COPPER;
	static const std::string ADD_MOUNT_FACADE;
	static const std::string ADD_FACTION_MEDAL;
	static const std::string ADD_FACTION_CONTRIBUTION;
	static const std::string ADD_FACTION_LEVEL;
	static const std::string ADD_FACTION_GOLD;
	static const std::string ADD_PET_FAME;
	static const std::string ADD_PET_LING;
	static const std::string ADD_CARD_SOUL;
	static const std::string ADD_SUOYAOTA_TIMES;
	static const std::string ADD_GOLD;
	static const std::string ADD_TIME_TIANDI_CIFU;
	static const std::string ADD_TIME_ZHANSHEN_CIFU;
	static const std::string ADD_TIME_YUZHOU_CIFU;
	static const std::string ADD_TIME_TIANGUAN_CIFU;
	static const std::string ADD_FRAGMENT_LEFT;
	static const std::string ADD_BLUE_SOUL;
	static const std::string ADD_PURPLE_SOUL;
	static const std::string ADD_MULTI_ARENA_HONOR;
};

#endif /* ITEM_USER_H_ */
