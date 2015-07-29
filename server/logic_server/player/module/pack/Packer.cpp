/*
 * Packer.cpp
 *
 *  Created on: Nov 26, 2013
 *      Author: linqiyou
 */

#include "Logic_Module_Head.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Package.h"
#include "Pack_Func.h"
#include "Pack_Struct.h"
#include "Logic_Player.h"
#include "task/Tasker.h"
#include "Packer.h"
#include "Msg_Packer_Struct.h"
#include "Msg_Active_Struct.h"
#include "mail/Mail_Struct.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "vip/Vip_Def.h"
#include "vip/Vip.h"
#include "Config_Cache_Equip.h"
#include "helper/Helper_Def.h"
#include "Logic_Public.h"
#include "rune_stone/Rune_Stone_Func.h"
#include "Config_Cache_Mail.h"
#include "card/Card_Config.h"
#include "Config_Cache_Card.h"
#include "oper_activity/Oper_Activity_Def.h"

Packer::Packer(void) : seq_generator_(0), packer_detail_(0),save_time_(0) { }

Packer::~Packer(void) { }

int Packer::create_init(Packer_Detail &detail) {
	return 0;
}

void Packer::load_detail(Packer_Detail &detail) {
	packer_detail_ = &detail;
	return ;
}

void Packer::save_detail(Packer_Detail &detail,bool logout) {
	if(packer_detail().is_change) {
		if ( logout ) {
			detail = packer_detail();
			packer_detail_->is_change = false;
		} else {
			if ( Time_Value::gettimeofday().sec() >= save_time_ ) {
				detail = packer_detail();
				packer_detail_->is_change = false;
				save_time_ = Time_Value::gettimeofday().sec() + PACK_SAVE_TIME_INTERVAL;
			}
		}
	}
	return ;
}

void Packer::module_init(void) {
	for (Goods_Map::iterator it = packer_detail_->goods_map.begin();
			it != packer_detail_->goods_map.end(); ++it) {
		it->second->seq_ = ++seq_generator_;
	}

	if (packer_detail_->capacity.pack_cap < PACK_INIT_CAPACITY) {
		packer_detail_->capacity.pack_cap = PACK_INIT_CAPACITY;
	}

	if (packer_detail_->capacity.storage_cap < STORAGE_INIT_CAPACITY) {
		packer_detail_->capacity.storage_cap = STORAGE_INIT_CAPACITY;
	}

	if (packer_detail_->capacity.spirit_equip_cap < SPIRIT_EQUIP_INIT_CAPACITY) {
		packer_detail_->capacity.spirit_equip_cap = SPIRIT_EQUIP_INIT_CAPACITY;
	}

	if (packer_detail_->capacity.spirit_package_cap < SPIRIT_PACKAGE_INIT_CAPACITY) {
		packer_detail_->capacity.spirit_package_cap = SPIRIT_PACKAGE_INIT_CAPACITY;
	}

	if (packer_detail_->capacity.rune_stone_equip_cap < RUNE_STONE_EQUIP_INIT_CAP) {
		packer_detail_->capacity.rune_stone_equip_cap = RUNE_STONE_EQUIP_INIT_CAP;
	}

	if (packer_detail_->capacity.rune_stone_pack_cap < RUNE_STONE_PACK_INIT_CAP) {
		packer_detail_->capacity.rune_stone_pack_cap = RUNE_STONE_PACK_INIT_CAP;
	}

	if (packer_detail_->capacity.thorlottery_cap < THORLOTTERY_INIT_CAPACITY) {
		packer_detail_->capacity.thorlottery_cap = THORLOTTERY_INIT_CAPACITY;
	}

	if (packer_detail_->capacity.card_cap < CARD_BOX_INIT_CAPACITY) {
		packer_detail_->capacity.card_cap = CARD_BOX_INIT_CAPACITY;
	}

	return ;
}

void Packer::sign_in(void) {

	save_time_ = Time_Value::gettimeofday().sec() + PACK_SAVE_TIME_INTERVAL;
}

void Packer::sign_out(void) {

}

int Packer::client_ready_now(void){
//	pack_clean_overdue();
	pack_active_update_money();
	return 0;
}

void Packer::reset(void) {
	seq_generator_ = 0;
	packer_detail_ = 0;
}

void Packer::daily_refresh(void) {
	//if (!this->is_inner_player()) {
		packer_detail_->money_detail.today_diamond_pay = 0;
		packer_detail_->money_detail.today_diamond_get = 0;
		packer_detail_->money_detail.today_gold_pay = 0;
		packer_detail_->money_detail.today_gold_get = 0;
	//}

	packer_detail_->is_change = true;
}

void Packer::week_refresh(void) {
	//if (this->is_inner_player()) {
		packer_detail_->money_detail.today_diamond_pay = 0;
		packer_detail_->money_detail.today_diamond_get = 0;
		packer_detail_->money_detail.today_gold_pay = 0;
		packer_detail_->money_detail.today_gold_get = 0;
	//}

	packer_detail_->is_change = true;
}

int Packer::pack_fetch_package_info(const uint32_t &pack_type_msg) {
	// 英雄装备类型做特殊处理
	Hero_Pack_Module hpm;
	pack_get_hero_pack_module(hpm, pack_type_msg);
	if (hpm != HPM_END) {
		return pack_fetch_hero_equip_info(hpm, pack_type_msg);
	}

	Pack_Type pack_type = static_cast<Pack_Type>(pack_type_msg);

	switch (pack_type) {
	case PACK_T_PACKAGE_INDEX:
	case PACK_T_EQUIP_INDEX:
	case PACK_T_STORAGE_INDEX:
	case PACK_T_ACHIEVEMENT_INDEX:
	case PACK_T_RUNE_STONE_SMELT_INDEX:
	case PACK_T_RUNE_STONE_PACK_INDEX:
	case PACK_T_RUNE_STONE_STORE_INDEX:
	case PACK_T_RUNE_STONE_EQUIP_INDEX:
	case PACK_T_THOR_LOTTERY_PACK_INDEX:
	case PACK_T_CARD_BOX_INDEX:
		break;
	default:
		return ERROR_CLIENT_PARAM;
	}

	MSG_50100100 msg_back;
	msg_back.pack_type = pack_type_msg;
	msg_back.capacity = pack_get_capacity(pack_type);
	msg_back.target_role_id = role_id();

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
		pack_insert_item_to_msg(*item_it->second, msg_back, *this);
	}

	THIS_SEND_TO_CLIENT(msg_back);

	// 请求人物装备段时，客户端要求加上部分英雄装备信息
	if (pack_type == PACK_T_EQUIP_INDEX) {
		//pack_fetch_role_panel_hero_equip(msg_back);
		pack_active_role_panel_hero_equip(player_self());
	}

	return 0;
}

int Packer::pack_fetch_hero_equip_info(const Hero_Pack_Module hero_pack_module, const uint32_t &pack_type_msg) {
	if (! pack_is_hero_type(hero_pack_module, pack_type_msg)) {
		return ERROR_CLIENT_PARAM;
	}

	MSG_50100100 msg_back;
	msg_back.pack_type = pack_type_msg;
	msg_back.capacity = pack_get_hero_capacity(pack_type_msg);
	msg_back.target_role_id = role_id();

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter_hero(pack_type_msg, it_begin, it_end);

	for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
		pack_insert_item_to_msg(*item_it->second, msg_back, *this);
	}

	THIS_SEND_TO_CLIENT(msg_back);
	return 0;
}

int Packer::pack_active_pack_rune_stone(Logic_Player* target_player) {
	if (! target_player) {
		return -1;
	}

	MSG_50100100 msg_back;

	// player
	{
		Pack_Type pack_type = PACK_T_RUNE_STONE_EQUIP_INDEX;
		msg_back.reset();
		msg_back.pack_type = pack_type;
		msg_back.capacity = target_player->pack_get_capacity(pack_type);
		msg_back.target_role_id = target_player->role_id();

		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		target_player->pack_get_iter(pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, msg_back, *this);
		}
		THIS_SEND_TO_CLIENT(msg_back);
	}

	// hero
	for (std::vector<Panal_Hero_Tag>::const_iterator it_hero_tags = target_player->heroer_detail().panal_hero_tags.begin();
			it_hero_tags != target_player->heroer_detail().panal_hero_tags.end(); ++it_hero_tags) {
		if (! it_hero_tags->hero_index) {
			continue;
		}
		int hero_index = it_hero_tags->hero_index;
		int pack_type = PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + hero_index * ONE_HERO_EQUIP_GAP;

		msg_back.reset();
		msg_back.target_role_id = target_player->role_id();
		msg_back.pack_type = pack_type;
		msg_back.capacity = target_player->pack_get_hero_capacity(pack_type);

		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		target_player->pack_get_iter_hero(pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, msg_back, *this);
		}
		THIS_SEND_TO_CLIENT(msg_back);
	}

	return 0;
}

int Packer::pack_active_role_panel_hero_equip(Logic_Player* target_player) {
	if (! target_player) {
		return -1;
	}

	MSG_50100100 msg_back;
	for (std::vector<Panal_Hero_Tag>::const_iterator it_hero_tags = target_player->heroer_detail().panal_hero_tags.begin();
			it_hero_tags != target_player->heroer_detail().panal_hero_tags.end(); ++it_hero_tags) {
		if (! it_hero_tags->hero_index) {
			continue;
		}
		int hero_index = it_hero_tags->hero_index;
		int pack_type = PACK_T_HERO_EQUIP_INDEX + hero_index * ONE_HERO_EQUIP_GAP;

		msg_back.reset();
		msg_back.target_role_id = target_player->role_id();
		msg_back.pack_type = pack_type;
		msg_back.capacity = target_player->pack_get_hero_capacity(pack_type);

		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		target_player->pack_get_iter_hero(pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, msg_back, *target_player);
		}
		THIS_SEND_TO_CLIENT(msg_back);
	}

	return 0;
}

int Packer::pack_fetch_role_panel_hero_equip(MSG_50100100 &msg_back) {
	// 战斗英雄装备
	int fight_hero_pack_type = pack_get_hero_pack_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX),
			player_self()->heroer_detail().fighter_index);
	if (fight_hero_pack_type) {
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(fight_hero_pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, msg_back, *this);
		}
	}
	// 角色面板上其它英雄装备
	for (uint16_t i = 1; i < 6; ++i) {
		int pack_type = PACK_T_HERO_EQUIP_INDEX + i * ONE_HERO_EQUIP_GAP;
		if (pack_type == fight_hero_pack_type) {
			continue;
		}
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, msg_back, *this);
		}
	}

//	MSG_50100100 res_msg_hero_rs;
//	pack_fetch_role_panel_hero_rune_stone(res_msg_hero_rs);

	return 0;
}

int Packer:: pack_fetch_role_panel_hero_equip(MSG_50100112 &res_msg) {
	// 战斗英雄装备
	int fight_hero_pack_type = pack_get_hero_pack_type(static_cast<Hero_Pack_Module>(HPM_EQUIP_INDEX),
			player_self()->heroer_detail().fighter_index);
	if (fight_hero_pack_type) {
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(fight_hero_pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, res_msg, *this);
		}
	}
	// 角色面板上其它英雄装备
	for (uint16_t i = 1; i < 6; ++i) {
		int pack_type = PACK_T_HERO_EQUIP_INDEX + i * ONE_HERO_EQUIP_GAP;
		if (pack_type == fight_hero_pack_type) {
			continue;
		}
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, res_msg, *this);
		}
	}

	//pack_fetch_role_panel_hero_rune_stone(res_msg);

	return 0;
}

int Packer::pack_fetch_role_panel_hero_rune_stone(MSG_50100100 &msg_back) {
	// 战斗英雄
	int fight_hero_pack_type = pack_get_hero_pack_type(static_cast<Hero_Pack_Module>(HPM_RUNE_STONE_EQUIP_INDEX),
			player_self()->heroer_detail().fighter_index);
	if (fight_hero_pack_type) {
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(fight_hero_pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, msg_back, *this);
		}
	}
	// 角色面板上其它英雄
	for (uint16_t i = 1; i < 6; ++i) {
		int pack_type = PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + i * ONE_HERO_EQUIP_GAP;
		if (pack_type == fight_hero_pack_type) {
			continue;
		}
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, msg_back, *this);
		}
	}

	return 0;
}

int Packer:: pack_fetch_role_panel_hero_rune_stone(MSG_50100112 &res_msg) {
	// 战斗英雄
	int fight_hero_pack_type = pack_get_hero_pack_type(static_cast<Hero_Pack_Module>(HPM_RUNE_STONE_EQUIP_INDEX),
			player_self()->heroer_detail().fighter_index);
	if (fight_hero_pack_type) {
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(fight_hero_pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, res_msg, *this);
		}
	}
	// 角色面板上其它英雄
	for (uint16_t i = 1; i < 6; ++i) {
		int pack_type = PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + i * ONE_HERO_EQUIP_GAP;
		if (pack_type == fight_hero_pack_type) {
			continue;
		}
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter_hero(pack_type, it_begin, it_end);
		for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
			pack_insert_item_to_msg(*item_it->second, res_msg, *this);
		}
	}

	return 0;
}

int Packer::pack_insert_equip(const uint32_t index, const Item_Detail &item, const Gain_Item_DM_Info &dm_info) {
	if (is_item_exist(index)) {
		return ERROR_INNER_SERVER;
	}
	uint32_t begin, end;
	pack_get_index(PACK_T_EQUIP_INDEX, begin, end);
	if (index < begin || index > end) {
		return ERROR_INNER_SERVER;
	}

	if (item.type_  != Item_Detail::EQUIP) {
		return ERROR_INNER_SERVER;
	}

	int stack_upper = Item_Detail::get_item_stack_upper(item.id_);
	int amount = item.amount_;
	Item_Detail *item_temp = POOL_MANAGER->pop_item_pool(3);
	*item_temp = item;
	item_temp->index_ = index;
	item_temp->amount_ = std::min(amount, stack_upper);
	item_temp->seq_ = ++seq_generator_;
	if(!packer_detail_->goods_map.insert(std::make_pair(index, item_temp)).second) {
		MSG_USER("insert goods map i:%d", index);
		POOL_MANAGER->push_item_pool(item_temp, 18);
	}

	Item_Vec item_vec;
	item_vec.push_back(*item_temp);
	UInt_Set changed_set;
	changed_set.insert(index);
	pack_active_add_item_detail(&changed_set, &item_vec);
	pack_active_update_item_detail(&changed_set);

	//this->push_tips(item.id_, item.amount_, GET_ITEM);
	//this->publish_tips();

	write_item_gain_stream(dm_info, item_temp->id_,
		item_temp->amount_, item_temp->bind_, PACK_T_EQUIP_INDEX);

	return 0;
}

int Packer::pack_insert_item(const Pack_Type pack_type, const Item_Detail &item, const Gain_Item_DM_Info &info,
		TIPS_TYPE tips_type, Pack_Try pack_try, const int mail_id) {

	Pack_Type type = check_and_change_pack_type(pack_type, item.id_);
	const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(item.id_);
	if (!ic) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (item.amount_/ic->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
		MSG_TRACE_ABORT("ERROR item(%d) amount(%d) limit", item.id_, item.amount_);
		return ERROR_PACK_AMOUNT_LIMIT;
	}
	int result = 0;
	Item_Detail ite = item;
	ite.init();

//	if (item.id_ < 1000000) {
//		result = this->pack_add_money(Money_Add_Info(Money_Type(item.id_), item.amount_, Money_DM_Info(MONEY_ADD_TEAM_FB)));
//		return result;
//	}

	UInt_Set changed_set;
	// 若物品可堆叠，优先叠放
	if (Item_Detail::get_item_stack_upper(ite.id_) > 1) {
		result = pack_insert_to_exist_index_first(type, ite, &changed_set, GENERATE_SEQ, pack_try);
	} else {
		result = pack_insert_to_empty_index_direct(type, ite, &changed_set, GENERATE_SEQ, pack_try);
	}

	if (result == 0) {
		//this->push_tips(item.id_, item.amount_, tips_type);
		//this->publish_tips();
		Item_Vec item_vec;
		item_vec.push_back(ite);
		pack_active_add_item_detail(&changed_set, &item_vec);
		pack_active_update_item_detail(&changed_set);
		write_item_gain_stream(info, ite.id_, ite.amount_, ite.bind_, pack_type);

	} else if (ERROR_PACK_FULL == result || ERROR_CARD_PACK_FULL == result) {
		Item_Vec item_list;
		item_list.push_back(ite);
		const Item_Gain_Source_Detail *cfg = CONFIG_CACHE_ITEM->find_item_gain_source(info.type);
		if (!cfg) {
			if (0 != LOGIC_PUBLIC->send_mail(mail_id, role_id(), item_list)) {
				LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
			}
		} else {
			const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
			Mail_Config_Map::const_iterator mailc = mail_config.find(PACK_MAIL_ID_NEW);
			if(mailc == mail_config.end()) {
				if (0 != LOGIC_PUBLIC->send_mail(mail_id, role_id(), item_list)) {
					LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
				}
				return -1;
			}
			std::string title;
			std::string contents;
			{
				const int size_buf = mailc->second.mail_title.size() + \
						sizeof(cfg->name) * 2 + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->second.mail_title.c_str(), cfg->name.c_str());
				title.assign(content_buf);
			}

			{
				const int size_buf = mailc->second.mail_content.size() + \
						sizeof(cfg->name) * 2 + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->second.mail_content.c_str(), cfg->name.c_str());
				contents.assign(content_buf);
			}
			if (0 != LOGIC_PUBLIC->send_mail(PACK_MAIL_ID_NEW, role_id(), item_list, title, contents)) {
				LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_list);
			}
		}
		if (ERROR_PACK_FULL == result) {
			player_self()->send_err_to_client(ERROR_PACK_FULL_BUT_MAIL, 0);
		} else {
			player_self()->send_err_to_client(ERROR_CARD_FULL_BUT_MAIL, 0);
		}
		result = 0;
	}

//	if (ITEM_GAIN_COLLECTION == info.type) {
//		MSG_81000103 acv_msg;
//		Chat_Content content;
//		content.type = ANN_CTTYPE_TEXT;
//		content.text = "采集获得";
//		acv_msg.content.push_back(content);
//		content.reset();
//		content.type = ANN_CTTYPE_ITEM;
//		item.set_item_basic_info(content.item);
//		acv_msg.content.push_back(content);
//		THIS_SEND_TO_CLIENT(acv_msg);
//	}

	return result;
}

int Packer::pack_insert_item(const Pack_Type pack_type, const Item_Vec &item_list, const Gain_Item_DM_Info &dm_info,
		TIPS_TYPE tips_type, Pack_Try pack_try, const int mail_id) {
	std::map<Pack_Type, Item_Vec> item_lists;
	for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
		const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(it->id_);
		if (!ic) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (it->amount_/ic->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
			MSG_TRACE_ABORT("ERROR item(%d) amount(%d) limit", it->id_, it->amount_);
			return ERROR_PACK_AMOUNT_LIMIT;
		}
		Item_Detail item = *it;
		item.init();
		int tp = item.get_item_type();
		switch(tp) {
		case Item_Detail::CARD: {
			item_lists[PACK_T_CARD_BOX_INDEX].push_back(item);
			break;
		}
		case Item_Detail::RUNE_STONE: {
			item_lists[PACK_T_RUNE_STONE_PACK_INDEX].push_back(item);
			break;
		}
		default: {
			item_lists[pack_type].push_back(item);
			break;
		}
		}
	}

	for (std::map<Pack_Type, Item_Vec>::const_iterator it = item_lists.begin(); it != item_lists.end(); ++it) {
		this->pack_insert_item_direct(it->first, it->second, dm_info,
				tips_type, pack_try, mail_id);
	}
	return 0;
}

int Packer::pack_try_insert_item_direct(const Pack_Type pack_type, const Item_Vec &item_list) {
	// 装备不允许用insert的接口插入，必须从装备模块处操作
	if (pack_type == PACK_T_EQUIP_INDEX) return ERROR_INNER_SERVER;
	// item.json中不存在的item，id错误

	for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(it->id_);

		if (!item_config) {
			MSG_USER("item %d config not exist", it->id_);
			return ERROR_ITEM_ID;
		}
	}
	// 合并可重复item
	std::map<Item_Detail, int32_t> amount_map;
	for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
		if (it->amount_ <= 0) {
			return ERROR_CLIENT_OPERATE;
		} else {
			const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(it->id_);
			if (!ic) {
				return ERROR_CONFIG_NOT_EXIST;
			}
			if (it->amount_/ic->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
				MSG_TRACE_ABORT("ERROR item(%d) amount(%d) limit", it->id_, it->amount_);
				return ERROR_PACK_AMOUNT_LIMIT;
			}
		}
		amount_map[*it] += it->amount_;
	}

	const int remain_cap = pack_get_remain_capacity(pack_type);
	int capacity_add = 0;
	for (std::map<Item_Detail, int32_t>::iterator map_it = amount_map.begin(); map_it != amount_map.end(); ++map_it) {
		int stack_upper = Item_Detail::get_item_stack_upper(map_it->first.id_);
		int stackable = 0;
		int amount = map_it->second;
		Goods_Map::iterator it_begin;
		Goods_Map::iterator it_end;
		pack_get_iter(pack_type, it_begin, it_end);
		for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
			if (is_equal_item(map_it->first, *it->second)) {
				stackable += stack_upper - it->second->amount_;
			}
		}

		if (amount > stackable) {
			int amount_more = amount - stackable;
			capacity_add += (amount_more / stack_upper);
			if (amount_more % stack_upper != 0) {
				capacity_add++;
			}
			if (remain_cap < capacity_add) {
				return get_full_error_code(pack_type);
			}
		}
	}

	return 0;
}

int Packer::pack_insert_item_direct(const Pack_Type pack_type, const Item_Vec &item_list, const Gain_Item_DM_Info &dm_info,
		TIPS_TYPE tips_type, Pack_Try pack_try, const int mail_id) {
	int result = 0;
	Item_Vec item_li;
	for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
		const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(it->id_);
		if (!ic) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (it->amount_/ic->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
			MSG_TRACE_ABORT("ERROR item(%d) amount(%d) limit", it->id_, it->amount_);
			return ERROR_PACK_AMOUNT_LIMIT;
		}
		Item_Detail item = *it;
		item.init();
		item_li.push_back(*it);
	}
//	Item_Vec piv;
//	Money_Add_List mal;
//	for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
//		if (it->id_ < 1000000) {
//			mal.push_back(Money_Add_Info(Money_Type(it->id_), it->amount_, Money_DM_Info(MONEY_ADD_TEAM_FB)));
//		} else {
//			piv.push_back(*it);
//		}
//	}
//
//	if (!mal.empty()) {
//		result = this->pack_add_money(mal);
//		if (0 != result) {
//			return result;
//		}
//	}

	UInt_Set changed_set;
	result = this->pack_insert_to_exist_index_first(pack_type, item_li, &changed_set, GENERATE_SEQ, pack_try);

	if (result == 0) {
		for (Item_Vec::const_iterator it = item_li.begin(); it != item_li.end(); ++it) {
			//this->push_tips(it->id_, it->amount_, tips_type);
			write_item_gain_stream(dm_info, it->id_,
				it->amount_, it->bind_, pack_type);
			if (CONFIG_CACHE_EQUIP->is_suit_part(it->id_)) {
				player_self()->achievement_listen_suit_purple(it->id_);
//				player_self()->achievement_listen_suit_gold(it->id_);
			}
		}
		//this->publish_tips();
		pack_active_add_item_detail(&changed_set, &item_li);
		pack_active_update_item_detail(&changed_set);
	} else if (ERROR_PACK_FULL == result || ERROR_CARD_PACK_FULL == result) {
		const Item_Gain_Source_Detail *cfg = CONFIG_CACHE_ITEM->find_item_gain_source(dm_info.type);
		if (!cfg) {
			if (0 != LOGIC_PUBLIC->send_mail(mail_id, role_id(), item_li)) {
				LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_li);
			}
		} else {
			const Mail_Config_Map &mail_config = CONFIG_CACHE_MAIL->mail_config();
			Mail_Config_Map::const_iterator mailc = mail_config.find(PACK_MAIL_ID_NEW);
			if(mailc == mail_config.end()) {
				if (0 != LOGIC_PUBLIC->send_mail(mail_id, role_id(), item_li)) {
					LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_li);
				}
				return -1;
			}
			std::string title;
			std::string contents;
			{
				const int size_buf = mailc->second.mail_title.size() + \
						sizeof(cfg->name) * 2 + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->second.mail_title.c_str(), cfg->name.c_str());
				title.assign(content_buf);
			}

			{
				const int size_buf = mailc->second.mail_content.size() + \
						sizeof(cfg->name) * 2 + 10;
				char content_buf[size_buf];
				memset(content_buf, 0, size_buf);
				snprintf(content_buf, size_buf, mailc->second.mail_content.c_str(), cfg->name.c_str());
				contents.assign(content_buf);
			}
			if (0 != LOGIC_PUBLIC->send_mail(PACK_MAIL_ID_NEW, role_id(), item_li, title, contents)) {
				LOGIC_PUBLIC->send_mail(PACK_MAIL_ID, role_id(), item_li);
			}
		}
		if (ERROR_PACK_FULL == result) {
			player_self()->send_err_to_client(ERROR_PACK_FULL_BUT_MAIL, 0);
		} else {
			player_self()->send_err_to_client(ERROR_CARD_FULL_BUT_MAIL, 0);
		}
		result = 0;
	}

//	if (ITEM_GAIN_COLLECTION == dm_info.type && !item_list.empty()) {
//		MSG_81000103 acv_msg;
//		Chat_Content content;
//		content.type = ANN_CTTYPE_TEXT;
//		content.text = "采集获得";
//		acv_msg.content.push_back(content);
//		for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
//			content.reset();
//			content.type = ANN_CTTYPE_ITEM;
//			(*it).set_item_basic_info(content.item);
//			acv_msg.content.push_back(content);
//		}
//		THIS_SEND_TO_CLIENT(acv_msg);
//	}

	return result;
}

int Packer::pack_erase_item(const Pack_Type pack_type, const Id_Amount &id_amount,
		const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type, Pack_Try pack_try,
		const Item_Vertify_By_Condition con) {
	int result = 0;
	UInt_Set changed_set;
	result = this->pack_erase_item(pack_type, id_amount, dm_info, &changed_set, WITH_TRY, con);

	if (result == 0) {
		//this->push_tips(id_amount.id, id_amount.amount, tips_type);
		//this->publish_tips();
		result = pack_active_update_item_detail(&changed_set);
	}

	return result;
}

int Packer::pack_erase_item(const Pack_Type pack_type, const Id_Amount_Vec &id_amount_list, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type, Pack_Try pack_try) {
	int result = 0;
	UInt_Set changed_set;
	result = this->pack_erase_item(pack_type, id_amount_list, dm_info, &changed_set, WITH_TRY);

	if (result == 0) {
		for (Id_Amount_Vec::const_iterator it = id_amount_list.begin(); it != id_amount_list.end(); ++it) {
			//this->push_tips(it->id, it->amount, tips_type);
		}
		//this->publish_tips();
		result = pack_active_update_item_detail(&changed_set);
	}

	return result;
}

int Packer::pack_erase_item(const Index_Amount &index_amount, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type, Pack_Try pack_try) {
	int result = 0;
	UInt_Set changed_set;
	result = this->pack_erase_item(index_amount, dm_info, &changed_set, tips_type, WITH_TRY);

	if (result == 0) {
		result = pack_active_update_item_detail(&changed_set);
	}

	return result;
}

int Packer::pack_add_capacity(const uint8_t &pay_type, const uint32_t &pack_type_msg, const uint16_t &add_capacity, const int8_t un_free) {
	if (add_capacity <= 0) {
		return ERROR_CLIENT_PARAM;
	}
	uint32_t max_pack = this->pack_get_capacity_max();
	if (0 == max_pack) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	int add = 0;
	int price = 0;
	uint32_t caps = 0;
	int result = 0;
	Pack_Type pack_type = static_cast<Pack_Type>(pack_type_msg);

	switch (pack_type_msg) {
	case PACK_T_PACKAGE_INDEX:
		caps = packer_detail_->capacity.pack_cap;
		caps = std::min(static_cast<uint32_t>(PACK_MAX_CAPACITY), caps + add_capacity);
		if (max_pack < caps) {
			this->send_tip_to_client(ERROR_PACK_LVL_LOW, this->pack_get_next_lvl());
			return -1;
		}
		add = caps - packer_detail_->capacity.pack_cap;
		if (add > 0) {
			if (un_free) {
				int item_id = CONFIG_CACHE_PACKAGE->package_config().super_item_id;
				int item_nums = pack_calc_item(PACK_T_PACKAGE_INDEX, item_id);
				if (item_nums >= add) {
					result = pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(item_id, add),
							Lose_Item_DM_Info(ITEM_LOSE_PACKAGE_CAPACITY));
				} else {
					result = get_capacity_price(pack_type, packer_detail_->capacity.pack_cap + item_nums,
							packer_detail_->capacity.pack_cap + add, price);
					if (0 == result) {
						result = pack_try_sub_money(Money_Sub_Info(GOLD_ONLY, price, MONEY_SUB_ADD_PACKAGE_CAP));
						if (0 == result) {
							result = pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(item_id, item_nums),
									Lose_Item_DM_Info(ITEM_LOSE_PACKAGE_CAPACITY));
							result = pack_sub_money(Money_Sub_Info(GOLD_ONLY, price, MONEY_SUB_ADD_PACKAGE_CAP));
						}
					}
				}
			}
			if (result == 0) {
				packer_detail_->capacity.pack_cap = caps;
				player_self()->achievement_listen_pack_grid();
			}
		}
		break;
	case PACK_T_STORAGE_INDEX:
		caps = packer_detail_->capacity.storage_cap;
		caps = std::min(static_cast<uint32_t>(STORAGE_MAX_CAPACITY), caps + add_capacity);
		if (max_pack < caps) {
			this->send_tip_to_client(ERROR_STORAGE_LVL_LOW, this->pack_get_next_lvl());
			return -1;
		}
		add = caps - packer_detail_->capacity.storage_cap;

//		if (pay_type != 1) {
//			caps = std::min(static_cast<uint32_t>(STORAGE_MAX_CAPACITY), caps + add_capacity);
//			add = caps - packer_detail_->capacity.storage_cap;
//		} else {
//			caps = caps + add_capacity;
//			int caps_lose = caps % STORAGE_GRID_PER_LINE;
//			if (caps_lose != 0) {
//				return ERROR_CLIENT_OPERATE;
//			}
//			caps = std::min(static_cast<uint32_t>(STORAGE_MAX_CAPACITY), caps);
//			add = caps - packer_detail_->capacity.storage_cap;
//		}

		if (add > 0) {
			if (un_free) {
				int item_id = CONFIG_CACHE_PACKAGE->package_config().super_item_id;
				int item_nums = pack_calc_item(PACK_T_PACKAGE_INDEX, item_id);
				if (item_nums >= add) {
					result = pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(item_id, add),
							Lose_Item_DM_Info(ITEM_LOSE_STORAGE_CAPACITY));
				} else {
					result = get_capacity_price(pack_type, packer_detail_->capacity.storage_cap + item_nums,
							packer_detail_->capacity.storage_cap + add, price);
					if (0 == result) {
						result = pack_try_sub_money(Money_Sub_Info(GOLD_ONLY, price, MONEY_SUB_ADD_STORAGE_CAP));
						if (0 == result) {
							result = pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(item_id, item_nums),
									Lose_Item_DM_Info(ITEM_LOSE_STORAGE_CAPACITY));
							result = pack_sub_money(Money_Sub_Info(GOLD_ONLY, price, MONEY_SUB_ADD_STORAGE_CAP));
						}
					}
				}
			}
//			if (pay_type != 1) {
//				result = get_capacity_price(pack_type, packer_detail_->capacity.storage_cap, packer_detail_->capacity.storage_cap + add, price);
//			}
//			if (result == 0) {
//				switch (pay_type) {
//				case 0: {
////					int item_id = CONFIG_CACHE_PACKAGE->package_config().item_id;
////					int item_nums = 0;
////					get_capacity_super_item_nums(packer_detail_->capacity.storage_cap, caps, PACK_T_STORAGE_INDEX, item_nums);
////					result = pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(item_id, item_nums), Lose_Item_DM_Info(ITEM_LOSE_STORAGE_CAPACITY));
//					break;
//				}
//				case 1:
//					result = pack_sub_money(Money_Sub_Info(BIND_GOLD_FIRST, price, MONEY_SUB_ADD_STORAGE_CAP));
//					break;
//				default:
//					result = ERROR_CLIENT_OPERATE;
//					break;
//				}
//			}
			if (result == 0) {
				//以后如果需要获得成就再合到背包扩展一起
				packer_detail_->capacity.storage_cap = caps;

			}
		}

		break;
	case PACK_T_RUNE_STONE_EQUIP_INDEX:
//		caps = packer_detail_->capacity.rune_stone_equip_cap;
//		caps = std::min(static_cast<uint32_t>(RUNE_STONE_EQUIP_MAX_CAP), caps + add_capacity);
//		add = caps - packer_detail_->capacity.rune_stone_equip_cap;
//		if (add > 0) {
//			packer_detail_->capacity.rune_stone_equip_cap = caps;
//		}
		break;
	case PACK_T_RUNE_STONE_PACK_INDEX:
		caps = packer_detail_->capacity.rune_stone_pack_cap;
		caps = std::min(static_cast<uint32_t>(RUNE_STONE_PACK_MAX_CAP), caps + add_capacity);
		add = caps - packer_detail_->capacity.rune_stone_pack_cap;
		if (add > 0) {
			uint begin = packer_detail_->capacity.rune_stone_pack_cap + 1;
			uint end = begin + add;
			int cost_diamond = 0;
			for (uint i = begin; i < end; ++i) {
				const Rune_Stone_Pack_Grid_Cfg * rs_grid_cfg = CONFIG_CACHE_PACKAGE->find_rune_stone_pack_grid_cfg(i);
				if (! rs_grid_cfg) {
					continue;
				}
				cost_diamond += rs_grid_cfg->diamond;
			}
			if (cost_diamond > 0) {
				result = pack_sub_money(Money_Sub_Info(BIND_GOLD_FIRST, cost_diamond, MONEY_SUB_ADD_STORAGE_CAP));
			}

			if (result == 0) {
				packer_detail_->capacity.rune_stone_pack_cap = caps;
			}
		}
		break;
	default:
		result = ERROR_CLIENT_PARAM;
		break;
	}
	packer_detail_->is_change = true;

	this->pack_active_update_capacity(pack_type);

	if (0 == result) {
		result = ERROR_ADD_CAPACITY_SUCCESS;
	}
	return result;
}

int Packer::pack_discard_item(const std::vector<uint32_t> &index) {
	int result = pack_try_erase_item(index);
	if (result != 0) {
		return result;
	}

	for (std::vector<uint32_t>::const_iterator it = index.begin(); it != index.end(); ++it) {
		const Item_Detail *item = pack_get_const_item(*it);
		if (player_self()->is_equip_embed_gemstone(*item)) {
			return ERROR_EQUIP_EMBED_GEMSTONE_CANT_DISCARD;
		}
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item->id_);

		if (item_config && item_config->notAbandon == 2) {
			return ERROR_ITEM_NO_DROP;
		}
	}

	//pack_erase_item(msg.index, Lose_Item_DM_Info(ITEM_LOSE_DISCARD, this->scene_detail().scene_id), DISCARD_ITEM, WITHOUT_TRY);
	pack_erase_item(index, Lose_Item_DM_Info(ITEM_LOSE_DISCARD, 0), DISCARD_ITEM, WITHOUT_TRY);

	MSG_50100102 res_msg;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Packer::pack_move_item_msg(const uint32_t &index_from, const uint32_t &index_to) {
	int result = 0;
	switch (index_to) {
	case PACK_T_PACKAGE_INDEX:
	case PACK_T_STORAGE_INDEX:
		result = this->pack_move_item(index_from, static_cast<Pack_Type>(index_to));
		break;
	default:
		if (pack_is_pack_type(index_to, PACK_T_EQUIP_INDEX) == 0) {
			result = ERROR_CLIENT_PARAM;
		}
		if (result == 0) {
			result = this->pack_move_item(index_from, index_to);
		}
		break;
	}


	return result;
}

int Packer::pack_move_item(const uint32_t index_from, const uint32_t index_to, Pack_Try pack_try ) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_move_item(index_from, index_to);
		if (result != 0) return result;
	}

	Goods_Map::iterator it_from = packer_detail_->goods_map.find(index_from);
	Goods_Map::iterator it_to = packer_detail_->goods_map.find(index_to);

	Move_Item_DM_Info dm_info = Move_Item_DM_Info(ITEM_MOVE_TEST, index_from, index_to);

	//道具移动区别流水
	get_move_stream(index_from, index_to,dm_info);

	if (it_to == packer_detail_->goods_map.end()) {
		write_item_move_stream(dm_info,	it_from->second->id_, it_from->second->amount_, it_from->second->bind_);
		packer_detail_->goods_map.insert(std::make_pair(index_to, it_from->second));
		packer_detail_->goods_map.erase(it_from);
	} else {
		uint32_t id_to = it_to->second->id_;
		int32_t stack_upper_to = Item_Detail::get_item_stack_upper(id_to);
		int32_t remain_space = stack_upper_to - it_to->second->amount_;
		if (!is_equal_item(*it_from->second, *it_to->second) || stack_upper_to <= 1) {
			if (stack_upper_to > 1 && is_similar_item(*it_from->second, *it_to->second)) {
				if (it_to->second->bind_ != it_from->second->bind_) {
					it_to->second->bind_ = (it_to->second->bind_ > it_from->second->bind_) ? it_to->second->bind_ : it_from->second->bind_;
				}
				write_item_move_stream(dm_info, it_from->second->id_, it_from->second->amount_, it_from->second->bind_);
				pack_merge_similar_item_move(*it_to->second, *it_from->second);
				if (it_from->second->amount_ == 0) {
					packer_detail_->erase(it_from);
				}
			} else {
				write_item_move_stream(dm_info, it_from->second->id_, it_from->second->amount_, it_from->second->bind_);
				write_item_move_stream(dm_info, it_to->second->id_, it_to->second->amount_, it_to->second->bind_);
				std::swap(it_to->second, it_from->second);
			}
		} else if (remain_space == 0){
			return 0;
		} else{
			write_item_move_stream(dm_info, it_from->second->id_, it_from->second->amount_, it_from->second->bind_);
			pack_merge_equal_item(*it_to->second, *it_from->second);
			if (it_from->second->amount_ == 0) {
				packer_detail_->erase(it_from);
			}
		}
	}

	// index不交换
	it_to = packer_detail_->goods_map.find(index_to);
	if (it_to != packer_detail_->goods_map.end()) {
		it_to->second->index_ = index_to;
	}
	it_from = packer_detail_->goods_map.find(index_from);
	if (it_from != packer_detail_->goods_map.end()) {
		it_from->second->index_ = index_from;
	}

	UInt_Set changed_set;
	changed_set.insert(index_to);
	changed_set.insert(index_from);

	// 英雄背包段做特殊处理
	if (pack_is_hero_type(index_to)) {
		return pack_active_update_hero_pack_detail(&changed_set);
	}

	return this->pack_active_update_item_detail(&changed_set, FROM_MOVE);
}

void Packer::get_move_stream(const uint32_t index_from, const uint32_t index_to,Move_Item_DM_Info &dm_info) {

	if ( PACK_T_EQUIP_INDEX == index_to || PACK_T_PACKAGE_INDEX == index_to || PACK_T_STORAGE_INDEX == index_to
			|| PACK_T_ACHIEVEMENT_INDEX == index_to || PACK_T_RUNE_STONE_PACK_INDEX == index_to || PACK_T_RUNE_STONE_EQUIP_INDEX == index_to
			|| PACK_T_HERO_RUNE_STONE_EQUIP_INDEX == index_to || PACK_T_HERO_EQUIP_INDEX == index_to)
		get_move_stream(index_from,static_cast<Pack_Type>(index_to),dm_info);
	else {
		Pack_Type type_to = PACK_T_ALL_INDEX;
		pack_get_pack_type(index_to, type_to);
		if ( PACK_T_ALL_INDEX == type_to ) {
			if ( pack_is_hero_type(index_to) ) {
				if ( 1 == pack_get_hero_pack_type(index_to) )
					type_to = PACK_T_HERO_EQUIP_INDEX;
				else if ( 2 == pack_get_hero_pack_type(index_to) )
					type_to = PACK_T_HERO_RUNE_STONE_EQUIP_INDEX;
			}
		}
		get_move_stream(index_from,type_to,dm_info);
	}
}

void Packer::get_move_stream(const uint32_t index_from, const Pack_Type pack_type,Move_Item_DM_Info &dm_info) {

	Pack_Type type_from = PACK_T_ALL_INDEX;
	pack_get_pack_type(index_from, type_from);

	if ( type_from == pack_type ) {
		// 不考虑物品在英雄身上出现的移动
		dm_info.type = ITEM_MOVE_SAME_PACK;
		return;
	}
	switch(pack_type) {
	case PACK_T_EQUIP_INDEX :
	        // 由于前面已经判断，所以不再校验
			dm_info.type = ITEM_MOVE_PACK_EQUIP;
			break;
	case PACK_T_PACKAGE_INDEX :
		if ( type_from == PACK_T_EQUIP_INDEX )
			dm_info.type = ITEM_MOVE_EQUIP_PACK;
		else if ( type_from == PACK_T_STORAGE_INDEX )
			dm_info.type = ITEM_MOVE_STORE_PACK;
		else if ( type_from == PACK_T_ACHIEVEMENT_INDEX )
			dm_info.type = ITEM_MOVE_MEDAL_PACK;
		else if ( type_from == PACK_T_ALL_INDEX ) {
			if ( pack_is_hero_type(type_from) )
				dm_info.type = ITEM_MOVE_HERO_PACK;
		}
		break;
	case PACK_T_STORAGE_INDEX :
		dm_info.type = ITEM_MOVE_PACK_STORE;
		break;
	case PACK_T_ACHIEVEMENT_INDEX :
		dm_info.type = ITEM_MOVE_PACK_MEDAL;
		break;
	case PACK_T_RUNE_STONE_PACK_INDEX :
		if ( type_from == PACK_T_RUNE_STONE_EQUIP_INDEX)
			dm_info.type = ITEM_MOVE_RUNE_STONE_PACK;
		else {
			if ( pack_is_hero_type(type_from) ) {
				dm_info.type = ITEM_MOVE_HERO_RUNE_STONE_PACK;
			}
		}

		break;
	case PACK_T_RUNE_STONE_EQUIP_INDEX :

		dm_info.type = ITEM_MOVE_PACK_RUNE_STONE;
		break;
	case PACK_T_HERO_RUNE_STONE_EQUIP_INDEX :
		dm_info.type = ITEM_MOVE_PACK_HERO_RUNE_STONE;
		break;
	case PACK_T_HERO_EQUIP_INDEX :
		dm_info.type = ITEM_MOVE_PACK_HERO;
		break;
	default :
	    break;
	}

}

int Packer::pack_split_item(const uint32_t &index, const int16_t &split_num) {
	if (pack_is_pack_type(index, PACK_T_PACKAGE_INDEX) != 0) {
		return ERROR_CLIENT_OPERATE;
	}

	if (is_item_lock(index)) {
		return ERROR_PACK_LOCK;
	}

	Goods_Map::iterator it = packer_detail_->goods_map.find(index);
	if (it == packer_detail_->goods_map.end()) {
		return ERROR_CLIENT_OPERATE;
	} else if (Item_Detail::get_item_stack_upper(it->second->id_) <= 1
			|| split_num - it->second->amount_ >= 0) {
		return ERROR_CLIENT_OPERATE;
	}

	UInt_Set changed_set;
	Item_Detail item(*it->second);
	item.amount_ = split_num;

	int result = pack_insert_to_empty_index_direct(PACK_T_PACKAGE_INDEX, item, &changed_set, GENERATE_SEQ);
	if (result == 0) {
		it->second->amount_ -= split_num;
		changed_set.insert(index);
		pack_active_update_item_detail(&changed_set);
	}

	return result;
}

int Packer::pack_sort_item(const uint32_t &pack_type) {
	int result = this->pack_sort_item(static_cast<Pack_Type>(pack_type), MERGE_WAY_EQUAL);

	return result;
}

struct Item_Detail_PtrLess {
	bool operator() (const Item_Detail *item1, const Item_Detail *item2) const {
		return *item1 < *item2;
	}
};
int Packer::pack_sort_item(const Pack_Type pack_type, MERGE_WAY merge_way) {
	if (is_item_lock()) {
		return ERROR_PACK_LOCK;
	}

	std::vector<Item_Detail*> item_array;

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);
	item_array.reserve(std::distance(it_begin, it_end));

	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		item_array.push_back(it->second);
	}

	std::stable_sort(item_array.begin(), item_array.end(), Item_Detail_PtrLess());
	UInt_Set changed_set;
	if (merge_way == MERGE_WAY_SIMILAR) {
		pack_merge_item_array(item_array, pack_type, MERGE_WAY_SIMILAR, changed_set);
	} else {
		pack_merge_item_array(item_array, pack_type, MERGE_WAY_EQUAL, changed_set);
	}

	// 不将指针归还对象池，仅归还合并掉的
	packer_detail_->goods_map.erase(it_begin, it_end);
	const uint32_t array_size = item_array.size();
	Goods_Map::iterator it_pos = packer_detail_->goods_map.upper_bound(pack_type);
	for (uint32_t i = 0; i < array_size; ++i) {
		if (item_array[i]->amount_ > 0) {
			it_pos = packer_detail_->goods_map.insert(it_pos, std::make_pair(item_array[i]->index_, item_array[i]));
		} else {
			POOL_MANAGER->push_item_pool(item_array[i], 20);
		}
	}

	// update all changed by sort to client
	return pack_active_update_item_detail(&changed_set);
}

int Packer::pack_set_money(const Money_Detail &money_detail, const Money_DM_Info &info, Money_Opt_Type type) {
	if (money_detail.bind_diamond < 0 || money_detail.gold < 0 ||
			money_detail.diamond < 0 || money_detail.dragon_soul < 0
			|| money_detail.souls < 0 || money_detail.friendship < 0 || money_detail.reputation < 0) {
		return -1;
	}

	Money_Detail money_detail_cmp = packer_detail_->money_detail;
	packer_detail_->money_detail = money_detail;

	bool money_changed = false;
	if (money_detail_cmp.money() != packer_detail_->money_detail.money()) {
		packer_detail_->save_tick();
		money_changed = true;
	}

	if (money_changed && type == MONEY_OPT_REAL) {
		Money_Stream_Info money_stream_info;
		Time_Value now = Time_Value::gettimeofday();

		int bind_gold_chg = packer_detail_->money_detail.bind_diamond - money_detail_cmp.bind_diamond;
		int bind_gold_chg_abs = abs(bind_gold_chg);
		int gold_chg = packer_detail_->money_detail.diamond - money_detail_cmp.diamond;
		int gold_chg_abs = abs(gold_chg);
		int copper_chg = packer_detail_->money_detail.gold - money_detail_cmp.gold;
		int copper_chg_abs = abs(copper_chg);
		int dragon_soul_chg = packer_detail_->money_detail.dragon_soul - money_detail_cmp.dragon_soul;
		int dragon_soul_chg_abs = abs(dragon_soul_chg);
		int souls_chg = packer_detail_->money_detail.souls - money_detail_cmp.souls;
		int souls_chg_abs = abs(souls_chg);
		int fri_chg = packer_detail_->money_detail.friendship - money_detail_cmp.friendship;
		int fri_chg_abs = abs(fri_chg);
		int rep_chg = packer_detail_->money_detail.reputation - money_detail_cmp.reputation;
		int rep_chg_abs = abs(rep_chg);
		if (bind_gold_chg_abs > 0) {
			money_stream_info.set_basic_info(packer_detail_->role_id, now.sec(),
				BIND_GOLD, bind_gold_chg_abs, packer_detail_->money_detail.bind_diamond, info);
			this->request_money_stream(money_stream_info);
		}

		if (gold_chg_abs > 0) {
			money_stream_info.set_basic_info(packer_detail_->role_id, now.sec(),
				GOLD, gold_chg_abs, packer_detail_->money_detail.diamond, info);
			this->request_money_stream(money_stream_info);
		}

		if (copper_chg_abs > 0) {
			money_stream_info.set_basic_info(packer_detail_->role_id, now.sec(),
				COPPER, copper_chg_abs, packer_detail_->money_detail.gold, info);
			this->request_money_stream(money_stream_info);
		}

		if (dragon_soul_chg_abs > 0) {
			money_stream_info.set_basic_info(packer_detail_->role_id, now.sec(),
				DRAGON_SOUL, dragon_soul_chg_abs, packer_detail_->money_detail.dragon_soul, info);
			this->request_money_stream(money_stream_info);
		}

		if (souls_chg_abs > 0) {
			money_stream_info.set_basic_info(packer_detail_->role_id, now.sec(),
				SOULS, souls_chg_abs, packer_detail_->money_detail.souls, info);
			this->request_money_stream(money_stream_info);
		}

		if (fri_chg_abs > 0) {
			money_stream_info.set_basic_info(packer_detail_->role_id, now.sec(),
				FRIENDSHIP, fri_chg_abs, packer_detail_->money_detail.friendship, info);
			this->request_money_stream(money_stream_info);
		}

		if (rep_chg_abs > 0) {
			money_stream_info.set_basic_info(packer_detail_->role_id, now.sec(),
				REPUTATION, rep_chg_abs, packer_detail_->money_detail.reputation, info);
			this->request_money_stream(money_stream_info);
		}

	}

	return 0;
}

int Packer::request_money_stream(Money_Stream_Info &stream_info) {
	// 记录金钱获得
	if (stream_info.dm_info.type < MONEY_ADD_MAX) {
//		if (stream_info.dm_info.type == 1) {
//			player_self()->listen_vip_reward(VIP_WORLD_TREE_TEAM_EXPLORE, stream_info.money_type1, stream_info.money_chg1);
//		}
	}
	// 记录金钱损耗
//	if (stream_info.dm_info.type > MONEY_ADD_MAX) {
//		this->player_self()->money_stat_add(static_cast<Money_Type>(stream_info.money_type1), stream_info.money_chg1);
//		this->player_self()->money_stat_add(static_cast<Money_Type>(stream_info.money_type2), stream_info.money_chg2);
//	}
	MSG_STREAM("`stream_money`(`player_id`, `type`, `money_id`, `update_num`, `remain_num`, `item_id`, `item_num`, `ext_1`, `ext_2`, `time`,`level`) VALUES (%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
			stream_info.role_id,  stream_info.dm_info.type,
			stream_info.money_type1, stream_info.money_chg1, stream_info.money_cur1,
//			stream_info.money_type2, stream_info.money_chg2, stream_info.money_cur2,
			stream_info.dm_info.item_id, stream_info.dm_info.item_num,
			stream_info.dm_info.ext_1, stream_info.dm_info.ext_2, stream_info.opt_time_sec,player_self()->level());

	return 0;
}

int Packer::pack_try_erase_item(const Pack_Type pack_type, const Id_Amount_Vec &id_amount_list, const bool need_tips) {
	// 合并可重复id
	std::map<Id_Amount, int32_t> amount_map;
	for (Id_Amount_Vec::const_iterator it = id_amount_list.begin(); it != id_amount_list.end(); ++it) {
		if (it->amount > 0) {
			amount_map[*it] += it->amount;
		} else {
			return ERROR_CLIENT_OPERATE;
		}
	}

	int ret = 0;
	MSG_82100008 acv_msg;
	for (std::map<Id_Amount, int32_t>::iterator map_it = amount_map.begin(); map_it != amount_map.end(); ++map_it) {
		if (map_it->second <= 0) {
			return ERROR_CLIENT_OPERATE;
		}
		if (pack_calc_item(pack_type, map_it->first.id, map_it->first.bind_verify) < map_it->second) {
			ret = ERROR_ITEM_NOT_ENOUGH;
			acv_msg.item_ids.push_back(map_it->first.id);
		}
	}
	if (need_tips && !acv_msg.item_ids.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	if (0 != ret) {
		return ret;
	}

	if (is_item_lock()) { // 存在物品被锁定的情况下，要检测是否有物品被锁定导致不能扣除
		for (std::map<Id_Amount, int32_t>::iterator map_it = amount_map.begin(); map_it != amount_map.end(); ++map_it) {
			if (pack_calc_unlock_item(pack_type, map_it->first.id, map_it->first.bind_verify) < map_it->second) return ERROR_PACK_LOCK;
		}
	}

	return 0;
}

int Packer::pack_erase_item(const Pack_Type pack_type, const Id_Amount &id_amount, const Lose_Item_DM_Info &lose_dm_info, UInt_Set *changed_set, Pack_Try pack_try,
		const Item_Vertify_By_Condition con) {
	if (pack_try == WITH_TRY) {
		if (id_amount.amount <= 0) return ERROR_CLIENT_PARAM;
		int result = pack_try_erase_item(pack_type, id_amount, con);
		if (result != 0) return result;
	}

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);
	std::vector<uint32_t> erase_index;
	int32_t amount_temp = id_amount.amount;
	int32_t dec_per_once = 0;
	Bind_Verify bind_verify = id_amount.bind_verify;
	if (id_amount.bind_verify == BIND_NONE) {
		bind_verify = BIND_ONLY;
	}
	for (Goods_Map::iterator it = it_begin; it != it_end && amount_temp > 0; ++it) {
		if (is_item_lock(it->first)) {
			continue;
		}
		if (pack_is_req_item(*it->second, id_amount.id, bind_verify, con)) {
			dec_per_once = std::min(it->second->amount_, amount_temp);
			it->second->amount_ -= dec_per_once;

			write_item_lose_stream(lose_dm_info, it->second->id_, dec_per_once, it->second->bind_);

			amount_temp -= dec_per_once;
			if (NULL != changed_set) {
				changed_set->insert(it->first);
			}
			if (it->second->amount_ == 0) {
				erase_index.push_back(it->first);
			}
		}
	}

	if (id_amount.bind_verify == BIND_NONE) {
		for (Goods_Map::iterator it = it_begin; it != it_end && amount_temp > 0; ++it) {
			if (is_item_lock(it->first)) {
				continue;
			}
			if (pack_is_req_item(*it->second, id_amount.id, UNBIND_ONLY, con)) {
				dec_per_once = std::min(it->second->amount_, amount_temp);
				it->second->amount_ -= dec_per_once;

				write_item_lose_stream(lose_dm_info, it->second->id_, dec_per_once, it->second->bind_);

				amount_temp -= dec_per_once;
				if (NULL != changed_set) {
					changed_set->insert(it->first);
				}
				if (it->second->amount_ == 0) {
					erase_index.push_back(it->first);
				}
			}
		}
	}

	for (std::vector<uint32_t>::iterator it = erase_index.begin(); it != erase_index.end(); ++it) {
		Item_Detail *equip = pack_get_item(*it);
		if (Item_Detail::EQUIP == equip->type_) {
			player_self()->reset_card_location(*equip);
		}
		packer_detail_->erase(*it);
	}

	this->task_listen_item_modify(id_amount.id);
	return 0;
}

int Packer::pack_erase_item(const Pack_Type pack_type, const Id_Amount_Vec &id_amount_list, const Lose_Item_DM_Info &lose_dm_info,
		UInt_Set *changed_set, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_erase_item(pack_type, id_amount_list);
		if (result != 0) return result;
	}

	for (Id_Amount_Vec::const_iterator it = id_amount_list.begin(); it != id_amount_list.end(); ++it) {
		pack_erase_item(pack_type, *it, lose_dm_info, changed_set, WITHOUT_TRY);
	}

	return 0;
}

int Packer::pack_erase_item(const Index_Amount &index_amount, const Lose_Item_DM_Info &lose_dm_info, UInt_Set *changed_set, TIPS_TYPE tips_type, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_erase_item(index_amount);
		if (result != 0) return result;
	}

	Goods_Map::iterator it = packer_detail_->goods_map.find(index_amount.index);
	int item_id = it->second->id_;
	int bind = it->second->bind_;

	// 检查是否装备，重设卡片数据
	if (Item_Detail::EQUIP == it->second->type_) {
		player_self()->reset_card_location(*it->second);
	}

	if (it->second->amount_ == index_amount.amount) {
		packer_detail_->erase(it);
	} else {
		it->second->amount_ -= index_amount.amount;
	}

	//this->push_tips(item_id, index_amount.amount, tips_type);
	//this->publish_tips();

	if (NULL != changed_set) {
		changed_set->insert(index_amount.index);
	}

	write_item_lose_stream(lose_dm_info, item_id, index_amount.amount, bind);
	this->task_listen_item_modify(item_id);

	return 0;
}

int Packer::pack_try_insert_item(const Pack_Type pack_type, const Item_Vec &item_list) {
	// 装备不允许用insert的接口插入，必须从装备模块处操作
	if (pack_type == PACK_T_EQUIP_INDEX) return ERROR_INNER_SERVER;
	// item.json中不存在的item，id错误

	std::map<Pack_Type, Item_Vec> item_lists;
	for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
		const Item_Detail_Config *ic = CONFIG_CACHE_ITEM->find_item(it->id_);
		if (!ic) {
			return ERROR_CONFIG_NOT_EXIST;
		}
		if (it->amount_/ic->overlay >= PACKAGE_ITEM_MAX_AMOUNT) {
			MSG_TRACE_ABORT("ERROR item(%d) amount(%d) limit", it->id_, it->amount_);
			return ERROR_PACK_AMOUNT_LIMIT;
		}
		Item_Detail item = *it;
		item.init();
		int tp = item.get_item_type();
		switch(tp) {
		case Item_Detail::CARD: {
			item_lists[PACK_T_CARD_BOX_INDEX].push_back(item);
			break;
		}
		case Item_Detail::RUNE_STONE: {
			item_lists[PACK_T_RUNE_STONE_PACK_INDEX].push_back(item);
			break;
		}
		default: {
			item_lists[pack_type].push_back(item);
			break;
		}
		}
	}

	for (std::map<Pack_Type, Item_Vec>::const_iterator it = item_lists.begin(); it != item_lists.end(); ++it) {
		int ret = this->pack_try_insert_item_direct(it->first, it->second);
		if (0 != ret) {
			return ret;
		}
	}

	return 0;
}

int Packer::pack_insert_to_exist_index_first(const Pack_Type pack_type, const Item_Vec &item_list, UInt_Set *changed_set,
		Seq_Type seq_type, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_insert_item(pack_type, item_list);
		if (result != 0) return result;
	}

	for (Item_Vec::const_iterator it = item_list.begin(); it != item_list.end(); ++it) {
		if (pack_insert_to_exist_index_first(pack_type, *it, changed_set, seq_type, WITHOUT_TRY) != 0) {
			MSG_USER("pack insert error.");
		}
	}

	return 0;
}

int Packer::pack_insert_to_exist_index_first(const Pack_Type pack_type, const Item_Detail &item,
		UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_insert_item(pack_type, item);
		if (result != 0) return result;
	}

	int amount = item.amount_;
	int stack_upper = Item_Detail::get_item_stack_upper(item.id_);
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (amount == 0) {
			break;
		}
		if (is_equal_item(*it->second, item) && stack_upper - it->second->amount_ > 0) {
			int remain_space = stack_upper - it->second->amount_;
			if (amount - (stack_upper - it->second->amount_) > 0) {
				amount -= remain_space;
				it->second->amount_ += remain_space;
			} else {
				it->second->amount_ += amount;
				amount = 0;
			}
			if (NULL != changed_set) {
				changed_set->insert(it->first);
			}
		}
	}

	if (amount != 0) {
		Item_Detail item_temp(item);
		item_temp.amount_ = amount;
		return pack_insert_to_empty_index_direct(pack_type, item_temp, changed_set, seq_type, WITHOUT_TRY);
	}

	return 0;
}

int Packer::pack_try_insert_to_empty_index_direct(const Pack_Type pack_type, const Item_Detail &item) {
	if (item.amount_ < 0) {
		return ERROR_CLIENT_PARAM;
	}

	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item.id_);

	if (!item_config) {
		MSG_USER("item %d config not exist", item.id_);
		return ERROR_ITEM_ID;
	}

	int amount = item.amount_;
	int stack_upper = Item_Detail::get_item_stack_upper(item.id_);
	int capacity_add = (amount / stack_upper);
	if (amount % stack_upper != 0) {
		capacity_add++;
	}

	if (pack_get_remain_capacity(pack_type) < capacity_add) {
		return get_full_error_code(pack_type);
	}

	return 0;
}

void Packer::get_equip_index(const Pack_Type pack_type, Int_Vec &equip_index) {
	equip_index.clear();

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (it->second->type_ == Item_Detail::EQUIP) {
			equip_index.push_back(it->first);
		}
	}
}

void Packer::pack_get_indexes(const Pack_Type pack_type, const uint32_t id, Int_Vec &indexes) {
	indexes.clear();
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (it->second->id_ == id) {
			indexes.push_back(it->first);
		}
	}
}

uint32_t Packer::pack_get_capacity_max(void) {
	int level = this->level();
	const Package_Config package_config = CONFIG_CACHE_PACKAGE->package_config();
	UInt_Int_Map::const_iterator it = package_config.pack_grid_lvl_conf_.find(level);
	if (it != package_config.pack_grid_lvl_conf_.end()) {
		return it->second + 1 + PACK_INIT_CAPACITY;
	}
	return 0;
}

uint32_t Packer::pack_get_next_lvl(void) {
	uint32_t level = (uint32_t)this->level();
	const Package_Config package_config = CONFIG_CACHE_PACKAGE->package_config();
	int grids = 0;
	UInt_Int_Map::const_iterator it = package_config.pack_grid_lvl_conf_.find(level);
	if (it != package_config.pack_grid_lvl_conf_.end()) {
		grids = it->second;
	}
	for (UInt_Int_Map::const_iterator it = package_config.pack_grid_lvl_conf_.begin();
			it != package_config.pack_grid_lvl_conf_.end(); ++it) {
		if (level < it->first && grids < it->second) {
			return it->first;
		}
	}
	return level;
}

int Packer::pack_calc_item(const uint32_t index) {
	int amount = 0;
	Goods_Map::iterator it = packer_detail_->goods_map.find(index);
	if (it != packer_detail_->goods_map.end()) {
		amount = it->second->amount_;
	}

	if (amount < 0) MSG_USER("item amount < 0 in index %d.\n", index);
	return amount;
}

inline bool Packer::is_item_can_sell(uint32_t index) {
	const Item_Detail *item = pack_get_const_item(index);
	if (NULL == item) return false;
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item->id_);
	if (!item_config) return false;
	if (item_config->sellPrice) {
		return true;
	}
	return false;
}

Item_Detail *Packer::pack_get_item(const uint32_t index) {
	return pack_get_item_pointer(index);
}

Item_Detail *Packer::pack_get_item(const Pack_Type pack_type, const int32_t id, Bind_Verify bind_verify) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (pack_is_req_item(*it->second, id, bind_verify)) {
			return it->second;
		}
	}

	return NULL;
}

Item_Detail *Packer::pack_get_item_hero(const int pack_type, const int32_t id, Bind_Verify bind_verify) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter_hero(pack_type, it_begin, it_end);

	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (pack_is_req_item(*it->second, id, bind_verify)) {
			return it->second;
		}
	}

	return NULL;
}

const Item_Detail *Packer::pack_get_const_item(const uint32_t index) {
	return pack_get_item_pointer(index);
}

int Packer::pack_hook_sell_item(const Id_Amount_Vec &id_amount_vec) {
	uint32_t num = 0;
	for(uint32_t i = 0; i < id_amount_vec.size(); ++i) {
		num += id_amount_vec[i].amount;
	}

	player_self()->task_listen_sell_item(num);
	return 0;
}

int Packer::pack_get_capacity_public(const Pack_Type pack_type) {
	return pack_get_capacity(pack_type);
}

int Packer::pack_get_hero_capacity_public(const int pack_type) {
	return pack_get_hero_capacity(pack_type);
}

int Packer::client_get_item_effect(const Item_Vec &item_list,const Money_Add_List &money_add_list,MSG_81000102 &res_msg) {

	if (!item_list.empty()) {
		Item_Basic_Info item_base;
		Card_Info card_info;
		for (Item_Vec::const_iterator item_it = item_list.begin(); item_it != item_list.end(); ++item_it) {
			if (item_it->type_ == Item_Detail::CARD && item_it->amount_ > 1) {
				for (int i = 0; i < item_it->amount_; ++i) {
					item_base.reset();
					(*item_it).set_item_basic_info(item_base);
					item_base.amount = 1;
					res_msg.item_info.push_back(item_base);
					card_info.reset();
					if ((*item_it).set_item_card_info(card_info)) {
						res_msg.card_info_vec.push_back(card_info);
					}
				}
			} else {

				item_base.reset();
				(*item_it).set_item_basic_info(item_base);
				res_msg.item_info.push_back(item_base);
				card_info.reset();
				if ((*item_it).set_item_card_info(card_info)) {
					res_msg.card_info_vec.push_back(card_info);
				}
			}
		}
	}
	if (!money_add_list.empty()) {
//		pack_add_money(money_add_list);
		for (Money_Add_List::const_iterator money_it = money_add_list.begin(); money_it != money_add_list.end();
				++money_it) {
			res_msg.property.push_back(Property((*money_it).type, (*money_it).nums));
		}
	}
	if (!res_msg.item_info.empty() || !res_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(res_msg);
	}
	return 0;
}

int Packer::pack_discard_all(const Pack_Type pack_type) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	UInt_Vec erase_index;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (it->second->id_ > 0) {
			erase_index.push_back(it->first);
		}
	}
	pack_erase_item(erase_index, Lose_Item_DM_Info(ITEM_LOSE_TEST));
	packer_detail_->save_tick();
	return 0;
}

//void Packer::pack_clean_overdue(void) {//packer_detail_->goods_map.insert(std::make_pair(index, item_temp));
//	UInt_Vec index;
//	Time_Value now;
//	now = Time_Value::gettimeofday();
//	for (Goods_Map::iterator it = this->packer_detail_->goods_map.begin();
//			it != this->packer_detail_->goods_map.end(); ++it) {
//		if (it->second->overdue_time > Time_Value::zero && it->second->overdue_time <= now) {
//			index.push_back(it->second->index_);
//		}
//	}
//
//	pack_erase_item(index, Lose_Item_DM_Info(ITEM_LOSE_OVERDUE_TIME, 0), DISCARD_ITEM, WITHOUT_TRY);
//}

int Packer::pack_is_pack_type(const int32_t index, Pack_Type pack_type) {
	bool is_infer_type = false;
	if (PACK_T_EQUIP_INDEX < index && index <= PACK_T_EQUIP_INDEX + packer_detail_->capacity.equip_cap) {
		is_infer_type = (pack_type == PACK_T_EQUIP_INDEX);
	} else if (PACK_T_PACKAGE_INDEX < index && index <= PACK_T_PACKAGE_INDEX + packer_detail_->capacity.pack_cap) {
		is_infer_type = (pack_type == PACK_T_PACKAGE_INDEX);
	} else if (PACK_T_STORAGE_INDEX < index && index <= PACK_T_STORAGE_INDEX + packer_detail_->capacity.storage_cap) {
		is_infer_type = (pack_type == PACK_T_STORAGE_INDEX);
	} else if (PACK_T_ACHIEVEMENT_INDEX < index && index <= PACK_T_ACHIEVEMENT_INDEX + packer_detail_->capacity.achievement_cap) {
		is_infer_type = (pack_type == PACK_T_ACHIEVEMENT_INDEX);
	} else if (PACK_T_RUNE_STONE_SMELT_INDEX < index && index <= PACK_T_RUNE_STONE_SMELT_INDEX + packer_detail_->capacity.rune_stone_smelt_cap) {
		is_infer_type = (pack_type == PACK_T_RUNE_STONE_SMELT_INDEX);
	} else if (PACK_T_RUNE_STONE_PACK_INDEX < index && index <= PACK_T_RUNE_STONE_PACK_INDEX + packer_detail_->capacity.rune_stone_pack_cap) {
		is_infer_type = (pack_type == PACK_T_RUNE_STONE_PACK_INDEX);
	} else if (PACK_T_RUNE_STONE_EQUIP_INDEX < index && index <= PACK_T_RUNE_STONE_EQUIP_INDEX + packer_detail_->capacity.rune_stone_equip_cap) {
		is_infer_type = (pack_type == PACK_T_RUNE_STONE_EQUIP_INDEX);
	} else if (PACK_T_THOR_LOTTERY_PACK_INDEX < index && index <= PACK_T_THOR_LOTTERY_PACK_INDEX + packer_detail_->capacity.thorlottery_cap) {
		is_infer_type = (pack_type == PACK_T_THOR_LOTTERY_PACK_INDEX);
	} else if (PACK_T_CARD_BOX_INDEX < index && index <= PACK_T_CARD_BOX_INDEX + packer_detail_->capacity.card_cap) {
		is_infer_type = (pack_type == PACK_T_CARD_BOX_INDEX);
	}


	return is_infer_type? 0 : -1;
}

int Packer::pack_is_hero_type(const Hero_Pack_Module hero_pack_module, const int32_t type_or_index) {
	switch (hero_pack_module) {
	case HPM_EQUIP_INDEX: {
		int gap = type_or_index / ONE_HERO_EQUIP_GAP * ONE_HERO_EQUIP_GAP;
		return (PACK_T_HERO_EQUIP_INDEX + ONE_HERO_EQUIP_GAP <= gap) && (gap < PACK_T_HERO_EQUIP_INDEX + ALL_HERO_EQUIP_GAP);
	}
	case HPM_RUNE_STONE_EQUIP_INDEX: {
		int gap = type_or_index / ONE_HERO_EQUIP_GAP * ONE_HERO_EQUIP_GAP;
		return (PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + ONE_HERO_EQUIP_GAP <= gap) && (gap < PACK_T_HERO_RUNE_STONE_EQUIP_INDEX + ALL_HERO_EQUIP_GAP);
	}
	default:
		return false;
	}
}

bool Packer::pack_is_hero_type(const int32_t type_or_index) {
	return pack_get_hpm(type_or_index) != HPM_END;
}

int Packer::pack_get_pack_type(const int32_t index, Pack_Type &pack_type) {
	if (PACK_T_EQUIP_INDEX < index && index <= PACK_T_EQUIP_INDEX + packer_detail_->capacity.equip_cap) {
		pack_type = PACK_T_EQUIP_INDEX;
	} else if (PACK_T_PACKAGE_INDEX < index && index <= PACK_T_PACKAGE_INDEX + packer_detail_->capacity.pack_cap) {
		pack_type = PACK_T_PACKAGE_INDEX;
	} else if (PACK_T_STORAGE_INDEX < index && index <= PACK_T_STORAGE_INDEX + packer_detail_->capacity.storage_cap) {
		pack_type = PACK_T_STORAGE_INDEX;
	} else if (PACK_T_ACHIEVEMENT_INDEX < index && index <= PACK_T_ACHIEVEMENT_INDEX + packer_detail_->capacity.achievement_cap) {
		pack_type = PACK_T_ACHIEVEMENT_INDEX;
	} else if (PACK_T_RUNE_STONE_SMELT_INDEX < index && index <= PACK_T_RUNE_STONE_SMELT_INDEX + packer_detail_->capacity.rune_stone_smelt_cap) {
		pack_type = PACK_T_RUNE_STONE_SMELT_INDEX;
	} else if (PACK_T_RUNE_STONE_PACK_INDEX < index && index <= PACK_T_RUNE_STONE_PACK_INDEX + packer_detail_->capacity.rune_stone_pack_cap) {
		pack_type = PACK_T_RUNE_STONE_PACK_INDEX;
	} else if (PACK_T_RUNE_STONE_EQUIP_INDEX < index && index <= PACK_T_RUNE_STONE_EQUIP_INDEX + packer_detail_->capacity.rune_stone_equip_cap) {
		pack_type = PACK_T_RUNE_STONE_EQUIP_INDEX;
	} else if (PACK_T_THOR_LOTTERY_PACK_INDEX < index && index <= PACK_T_THOR_LOTTERY_PACK_INDEX + packer_detail_->capacity.thorlottery_cap) {
		pack_type = PACK_T_THOR_LOTTERY_PACK_INDEX;
	} else if (PACK_T_CARD_BOX_INDEX < index && index <= PACK_T_CARD_BOX_INDEX + packer_detail_->capacity.card_cap) {
		pack_type = PACK_T_CARD_BOX_INDEX;
	} else {
		return -1;
	}

	return 0;
}

int Packer::get_pack_storage_move_err_code(const int32_t index,int &err_code) {

	bool full = true;
	if(PACK_T_STORAGE_INDEX < index && index <= (PACK_T_STORAGE_INDEX + STORAGE_MAX_CAPACITY) ) {
		if ( index > PACK_T_STORAGE_INDEX + packer_detail_->capacity.storage_cap ) {
			int begin = PACK_T_STORAGE_INDEX + 1;
			int end = PACK_T_STORAGE_INDEX + packer_detail_->capacity.storage_cap;
			for ( int i = begin;i< end ;++i ) {
				if ( !packer_detail_->goods_map.count(i) ) {
					full = false;
					break;
				}
			}
			if ( full )
				err_code = ERROR_STORAGE_FULL;
			else
				err_code = ERROR_STORAGE_INDEX_NOT_EXIST;
		}
	}

	if(PACK_T_PACKAGE_INDEX < index && index <= (PACK_T_PACKAGE_INDEX + PACK_MAX_CAPACITY) ) {
		if ( index > PACK_T_PACKAGE_INDEX + packer_detail_->capacity.pack_cap ) {
			int begin = PACK_T_PACKAGE_INDEX + 1;
			int end = PACK_T_PACKAGE_INDEX + packer_detail_->capacity.pack_cap;
			for ( int i = begin;i< end ;++i ) {
				if ( !packer_detail_->goods_map.count(i) ) {
					full = false;
					break;
				}
			}
			if ( full )
				err_code = ERROR_PACK_FULL;
			else
				err_code = ERROR_PACK_INDEX_NOT_EXIST;
		}
	}

	return 0;
}

int Packer::pack_get_hero_pack_type(const int32_t pack_index) {
	/*
	 * 通过pack_index计算pack_type
	 */
	if (PACK_T_HERO_EQUIP_INDEX < pack_index && pack_index < PACK_T_HERO_EQUIP_INDEX_END) {
		return pack_index / ONE_HERO_EQUIP_GAP * ONE_HERO_EQUIP_GAP;
	} else if (PACK_T_HERO_RUNE_STONE_EQUIP_INDEX < pack_index && pack_index < PACK_T_HERO_RUNE_STONE_EQUIP_INDEX_END) {
		return pack_index / ONE_HERO_EQUIP_GAP * ONE_HERO_EQUIP_GAP;
	}
	return 0;
}

int Packer::pack_get_hero_pack_type(const Hero_Pack_Module hpm, const int32_t hero_index) {
	/*
	 * 通过hero_index即英雄index计算pack_type
	 */
	if (hero_index <= 0 || hero_index > 1000) {
		return -1;
	}
	switch (hpm) {
	case HPM_EQUIP_INDEX:
		return HPM_EQUIP_INDEX + hero_index * ONE_HERO_EQUIP_GAP;
	case HPM_RUNE_STONE_EQUIP_INDEX:
		return HPM_RUNE_STONE_EQUIP_INDEX + hero_index * ONE_HERO_EQUIP_GAP;
	default:
		return 0;
	}

	return 0;
}

int Packer::pack_get_hero_pack_module(Hero_Pack_Module &hpm, const int32_t type_or_index) {
	if (PACK_T_HERO_EQUIP_INDEX < type_or_index && type_or_index < PACK_T_HERO_EQUIP_INDEX_END) {
		hpm = HPM_EQUIP_INDEX;
	} else if (PACK_T_HERO_RUNE_STONE_EQUIP_INDEX < type_or_index && type_or_index < PACK_T_HERO_RUNE_STONE_EQUIP_INDEX_END) {
		hpm = HPM_RUNE_STONE_EQUIP_INDEX;
	} else {
		hpm = HPM_END;
	}
	return 0;
}

int Packer::pack_get_hpm(const int32_t type_or_index) {
	if (PACK_T_HERO_EQUIP_INDEX < type_or_index && type_or_index < PACK_T_HERO_EQUIP_INDEX_END) {
		return HPM_EQUIP_INDEX;
	} else if (PACK_T_HERO_RUNE_STONE_EQUIP_INDEX < type_or_index && type_or_index < PACK_T_HERO_RUNE_STONE_EQUIP_INDEX_END) {
		return HPM_RUNE_STONE_EQUIP_INDEX;
	} else {
		return HPM_END;
	}

	return 0;
}

int Packer::pack_get_item_id(const uint32_t index, uint32_t &id) {
	Goods_Map::iterator it = packer_detail_->goods_map.find(index);
	if (it != packer_detail_->goods_map.end()) {
		id = it->second->id_;
		return 0;
	}
	return ERROR_ITEM_NOT_EXIST;
}

int Packer::pack_move_item(const uint32_t index_from, const Pack_Type pack_type_to, UInt_Set &changed, Item_Handle_Func item_func, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_move_item(index_from, pack_type_to);
		if (result != 0) return result;
	}

	Item_Detail item = *(pack_get_const_item(index_from));

	if (item_func != NULL) {
		item_func(item, *this->player_self());
	}

	Move_Item_DM_Info dm_info(ITEM_MOVE_TEST, index_from, pack_type_to);
	get_move_stream(index_from,pack_type_to,dm_info);
	write_item_move_stream(dm_info,item.id_, item.amount_, item.bind_);

	changed.insert(index_from);
	packer_detail_->erase(index_from);
	pack_insert_to_exist_index_first(pack_type_to, item, &changed, DONT_GENERATE_SEQ, WITHOUT_TRY);

	return 0;
}

int Packer::pack_move_item(const uint32_t index_from, const Pack_Type pack_type_to, Item_Handle_Func item_func, Pack_Try pack_try ) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_move_item(index_from, pack_type_to);
		if (result != 0) return result;
	}

	Pack_Type type_from = PACK_T_ALL_INDEX;
	pack_get_pack_type(index_from, type_from);

	Item_Detail item = *(pack_get_const_item(index_from));

	if (item_func != NULL) {
		item_func(item, *this->player_self());
	}

	Move_Item_DM_Info dm_info(ITEM_MOVE_TEST, index_from, pack_type_to);
	get_move_stream(index_from,pack_type_to,dm_info);

	write_item_move_stream(dm_info,item.id_, item.amount_, item.bind_);

	UInt_Set changed_set;
	changed_set.insert(index_from);
	packer_detail_->erase(index_from);
	pack_insert_to_exist_index_first(pack_type_to, item, &changed_set, DONT_GENERATE_SEQ, WITHOUT_TRY);

	pack_active_update_item_detail(&changed_set,FROM_MOVE);
	{
		// 当物品成仓库往背包插入时
		if ( changed_set.size() >= 2 && pack_type_to == PACK_T_PACKAGE_INDEX && type_from == PACK_T_STORAGE_INDEX ) {
			MSG_50100103 msg;
			msg.reset();
			UInt_Set::iterator iter = changed_set.begin();
			if ( iter != changed_set.end() ) {
				msg.index = *iter;
				THIS_SEND_TO_CLIENT(msg);
			}
		}
	}
	return 0;
}

int Packer::pack_move_item(const Pack_Type pack_type_from, const Pack_Type pack_type_to, Item_Handle_Func item_func, Pack_Try pack_try ) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_move_item(pack_type_from, pack_type_to);
		if (result != 0) return result;
	}

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type_from, it_begin, it_end);
	Move_Item_DM_Info dm_info = Move_Item_DM_Info(ITEM_MOVE_TEST, pack_type_from, pack_type_to);

	UInt_Set changed_set;
	Item_Vec item_list;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		item_list.push_back(*it->second);
		if (item_func != NULL) {
			item_func(*it->second, *this->player_self());
		}
		changed_set.insert(it->first);
		write_item_move_stream(dm_info,	it->second->id_, it->second->amount_, it->second->bind_);
	}

	packer_detail_->erase(it_begin, it_end);
	pack_insert_to_exist_index_first(pack_type_to, item_list, &changed_set, DONT_GENERATE_SEQ, WITHOUT_TRY);

	return pack_active_update_item_detail(&changed_set);
}

int Packer::pack_attempt_move_everyitem(const Pack_Type pack_type_from, const Pack_Type pack_type_to,
		int &move_item_nums, Item_Handle_Func item_func) {
	move_item_nums = 0;
	if (pack_type_from == pack_type_to) {
		return ERROR_CLIENT_OPERATE;
	}

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type_from, it_begin, it_end);
	if (it_begin == it_end) {
		return ERROR_NO_ITEM_GET;
	}

	std::vector<Index_Amount> item_vec;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		item_vec.push_back(Index_Amount(it->first, it->second->amount_));
	}

	// 尝试移动每个道具
	UInt_Set changed;
	for (std::vector<Index_Amount>::iterator it = item_vec.begin(); it != item_vec.end(); ++it) {
		int item_nums = it->amount;
		if (0 == pack_move_item(it->index, pack_type_to, changed, item_func)) {
			move_item_nums += item_nums;
		}
	}

	if (move_item_nums == 0) {
		return get_full_error_code(pack_type_to);
	} else {
		pack_active_update_item_detail(&changed);
		return 0;
	}
}

const Money_Detail &Packer::pack_get_money(void) {
	return packer_detail_->money_detail;
}

int Packer::pack_insert_to_empty_index_direct(const Pack_Type pack_type, const Item_Detail &item,
		UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_insert_to_empty_index_direct(pack_type, item);
		if (result != 0) {
			return result;
		}
	}

	int amount = item.amount_;
	int stack_upper = Item_Detail::get_item_stack_upper(item.id_);
	// 由于空的格子需要多个值来记录多个背包的空格，目前的实现较难加入此值，如有性能问题，再考虑加入空格子的标记
	uint32_t index_begin = 0;
	uint32_t index_end = 0;
	pack_get_index(pack_type, index_begin, index_end);
	for (uint32_t i = index_begin; amount > 0 && i < index_end; ++i) {
		if (!is_item_exist(i)) {
			Item_Detail *item_temp = POOL_MANAGER->pop_item_pool(4);
			*item_temp = item;
			item_temp->index_ = i;
			item_temp->amount_ = std::min(amount, stack_upper);
			amount -= item_temp->amount_;
			if (seq_type == GENERATE_SEQ) {
				item_temp->seq_ = ++seq_generator_;
			}
			pack_modify_item_before_insert(*item_temp);
			if(!packer_detail_->goods_map.insert(std::make_pair(i, item_temp)).second) {
				MSG_USER("insert goods map i:%d", i);
				POOL_MANAGER->push_item_pool(item_temp, 19);
			}
			if (NULL != changed_set) {
				changed_set->insert(i);
			}
		}
	}

	return 0;
}

int Packer::pack_calc_item(const Pack_Type pack_type) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	uint32_t item_amount = 0;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		item_amount += it->second->amount_;
	}

	return item_amount;
}

int Packer::pack_calc_item(const Pack_Type pack_type, const uint32_t id, Bind_Verify bind_verify, const Item_Vertify_By_Condition con) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	Time_Value now = Time_Value::gettimeofday();
	uint32_t item_amount = 0;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (pack_is_req_item(*it->second, id, bind_verify, con)) {
			item_amount += it->second->amount_;
		}
	}

	return item_amount;
}

int Packer::pack_calc_item_by_same_type(const Pack_Type pack_type, const uint32_t id) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	uint32_t item_amount = 0;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if ((it->second->id_) / 1000 == id / 1000) {
			item_amount += it->second->amount_;
		}
	}

	return item_amount;
}

int Packer::pack_calc_unlock_item(const Pack_Type pack_type, const uint32_t id, Bind_Verify bind_verify) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	uint32_t item_amount = 0;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		if (pack_is_req_item(*it->second, id, bind_verify)) {
			if (!is_item_lock(it->first)) {
				item_amount += it->second->amount_;
			}
		}
	}

	return item_amount;
}

int Packer::pack_active_update_money(void) {
	MSG_81000101 res_msg;
	res_msg.type = 1;
	Property prop;
	prop.type = COPPER;
	prop.value = packer_detail().money_detail.gold;
	res_msg.property.push_back(prop);

	prop.type = GOLD;
	prop.value = packer_detail().money_detail.diamond;
	res_msg.property.push_back(prop);

	prop.type = BIND_GOLD;
	prop.value = packer_detail().money_detail.bind_diamond;
	res_msg.property.push_back(prop);

	prop.type = DRAGON_SOUL;
	prop.value = packer_detail().money_detail.dragon_soul;
	res_msg.property.push_back(prop);

	prop.type = SOULS;
	prop.value = packer_detail().money_detail.souls;
	res_msg.property.push_back(prop);

	prop.type = FRIENDSHIP;
	prop.value = packer_detail().money_detail.friendship;
	res_msg.property.push_back(prop);

	prop.type = REPUTATION;
	prop.value = packer_detail().money_detail.reputation;
	res_msg.property.push_back(prop);

	THIS_SEND_TO_CLIENT(res_msg);
//	Block_Buffer buf;
//	MSG_82100002 msg;
//	msg.money = packer_detail_->money_detail.money();
//
//	THIS_SEND_TO_CLIENT(msg);
//
//	packer_detail_->save_tick();

	return 0;
}

int Packer::pack_active_update_capacity(const Pack_Type pack_type) {
	Block_Buffer buf;

	MSG_82100001 msg;
	msg.type = pack_type;
	msg.capacity = pack_get_capacity(pack_type);

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Packer::pack_active_update_hero_capacity(const int pack_type) {
	Block_Buffer buf;
	MSG_82100001 msg;
	msg.type = pack_type;
	msg.capacity = pack_get_hero_capacity(pack_type);
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Packer::pack_active_update_item_detail(const UInt_Set *changed_index, const Update_From_Type from_type) {
	if (NULL == changed_index) return -1;

	// 不同背包要分别发送
	std::map<Pack_Type, MSG_82100000> update_item_msg_map;
	std::map<Pack_Type, MSG_82100003> erase_item_msg_map;
	std::map<Pack_Type, MSG_82100004> update_addit_msg_map;

	for (UInt_Set::const_iterator index_it = changed_index->begin(); index_it != changed_index->end(); ++index_it) {
		Pack_Type type = PACK_T_ALL_INDEX;
		pack_get_pack_type(*index_it, type);
		Goods_Map::iterator item_it = packer_detail_->goods_map.find(*index_it);
		if (item_it == packer_detail_->goods_map.end()) {
			erase_item_msg_map[type].item_erased.push_back(*index_it);
		} else {
			pack_insert_item_to_msg(*item_it->second, PACK_TYPE, update_item_msg_map[type], update_addit_msg_map[type], *this);
			if (from_type == FROM_MOVE) {
				for (UInt_Set::const_iterator index_it2 = changed_index->begin(); index_it2 != changed_index->end(); ++index_it2) {
					if (*index_it2 != *index_it) {
						update_item_msg_map[type].index_list.push_back(*index_it2);
						break;
					}
				}
			}
		}
	}

	Block_Buffer buf;
	for (std::map<Pack_Type, MSG_82100003>::iterator it = erase_item_msg_map.begin(); it != erase_item_msg_map.end(); ++it) {
		buf.reset();
		if (it->second.item_erased.size() > 0) {
			THIS_SEND_TO_CLIENT(it->second);
		}
	}

	for (std::map<Pack_Type, MSG_82100000>::iterator it = update_item_msg_map.begin(); it != update_item_msg_map.end(); ++it) {
		buf.reset();
		if (it->second.item_info.size() > 0) {
			THIS_SEND_TO_CLIENT(it->second);
		}
	}
	// 客户端要求附加信息移动到基础信息之前
	for (std::map<Pack_Type, MSG_82100004>::iterator it = update_addit_msg_map.begin(); it != update_addit_msg_map.end(); ++it) {
		buf.reset();
		if (it->second.equip_addit.size() > 0 || it->second.rune_stone_addit.size() > 0 //||
				//it->second.card_addit.size() > 0 || it->second.rune_addit.size() > 0 ||
				//it->second.gem_addit.size() > 0 || it->second.spirit_addit.size() > 0 ||
				//it->second.baopan_addit.size() > 0 || it->second.fabao_addit.size() > 0 || it->second.exp_ball_addit.size() > 0
				) {
			//Utility_Func::get_block_buff_by_msg(ACTIVE_ITEM_ADDIT_INFO_UPDATE, it->second, buf);

			THIS_SEND_TO_CLIENT(it->second);
		}
	}

	packer_detail_->save_tick();

	return 0;
}

int Packer::pack_active_add_item_detail(const UInt_Set *changed_index, const Item_Vec *item_vec) {
	if (NULL == changed_index || NULL == item_vec) return -1;

	MSG_82100007 effect_msg;

	boost::unordered_map<uint32_t, int> already_item;
	for (Item_Vec::const_iterator it_msg = item_vec->begin(); it_msg != item_vec->end(); ++it_msg) {
		boost::unordered_map<uint32_t, int>::iterator it = already_item.find(it_msg->id_);
		if (it != already_item.end()) {
			it->second += it_msg->amount_;
		} else {
			already_item.insert(std::make_pair(it_msg->id_, it_msg->amount_));
		}
	}
	for (UInt_Set::const_iterator index_it = changed_index->begin(); index_it != changed_index->end(); ++index_it) {
		Pack_Type type = PACK_T_ALL_INDEX;
		pack_get_pack_type(*index_it, type);
		Goods_Map::iterator item_it = packer_detail_->goods_map.find(*index_it);
		if (item_it != packer_detail_->goods_map.end() && PACK_T_PACKAGE_INDEX == type) {
			const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_it->second->id_);
			if (!item_config) {
				return ERROR_CONFIG_NOT_EXIST;
			}
			if (item_config->time != Time_Value::zero) {
				item_it->second->overdue_time = Time_Value::gettimeofday() + item_config->time;
			}

			boost::unordered_map<uint32_t, int>::iterator it = already_item.find(item_it->second->id_);
			if (it != already_item.end()) {
				Item_Basic_Info item_base;
				item_it->second->set_item_basic_info(item_base);
				if (item_base.amount < it->second) {
					it->second -= item_base.amount;
				} else {
					item_base.amount = it->second;
					already_item.erase(it);
				}
				effect_msg.item_info.push_back(item_base);
			}
		}
	}

//	for (Item_Vec::const_iterator it = item_vec->begin(); it != item_vec->end(); ++it) {
//		int amount = it->amount_;
//		int stack_upper = Item_Detail::get_item_stack_upper(it->id_);
//		if (amount > stack_upper && stack_upper > 0) {
//			for (; amount > 0; ) {
//				Item_Basic_Info item_base;
//				it->set_item_basic_info(item_base);
//				item_base.amount = std::min(amount, stack_upper);
//				amount -= item_base.amount;
//				effect_msg.item_info.push_back(item_base);
//			}
//		} else {
//			Item_Basic_Info item_base;
//			it->set_item_basic_info(item_base);
//			effect_msg.item_info.push_back(item_base);
//		}
//	}

	if (!effect_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(effect_msg);
	}

	return 0;
}

int Packer::pack_active_update_hero_pack_detail(const UInt_Set *changed_index) {
	if (NULL == changed_index) return -1;

	// 不同背包要分别发送
	std::map<int, MSG_82100000> update_item_msg_map;
	std::map<int, MSG_82100003> erase_item_msg_map;
	std::map<int, MSG_82100004> update_addit_msg_map;

	for (UInt_Set::const_iterator index_it = changed_index->begin(); index_it != changed_index->end(); ++index_it) {
		int type = 0;
		if (pack_is_hero_type(*index_it)) {
			type = pack_get_hero_pack_type(*index_it);
		} else {
			Pack_Type pt = PACK_T_ALL_INDEX;
			pack_get_pack_type(*index_it, pt);
			type = pt;
		}
		if (! type) {
			continue;
		}
		Goods_Map::iterator item_it = packer_detail_->goods_map.find(*index_it);
		if (item_it == packer_detail_->goods_map.end()) {
			erase_item_msg_map[type].item_erased.push_back(*index_it);
		} else {
			pack_insert_item_to_msg(*item_it->second, PACK_TYPE, update_item_msg_map[type], update_addit_msg_map[type], *this);
			for (UInt_Set::const_iterator index_it2 = changed_index->begin(); index_it2 != changed_index->end(); ++index_it2) {
				if (*index_it2 != *index_it) {
					update_item_msg_map[type].index_list.push_back(*index_it2);
					break;
				}
			}
		}
	}

	Block_Buffer buf;
	for (std::map<int, MSG_82100003>::iterator it = erase_item_msg_map.begin(); it != erase_item_msg_map.end(); ++it) {
		buf.reset();
		if (it->second.item_erased.size() > 0) {
			THIS_SEND_TO_CLIENT(it->second);
		}
	}

	for (std::map<int, MSG_82100000>::iterator it = update_item_msg_map.begin(); it != update_item_msg_map.end(); ++it) {
		buf.reset();
		if (it->second.item_info.size() > 0) {
			THIS_SEND_TO_CLIENT(it->second);
		}
	}

	// 客户端要求附加信息移动到基础信息之前
	for (std::map<int, MSG_82100004>::iterator it = update_addit_msg_map.begin(); it != update_addit_msg_map.end(); ++it) {
		buf.reset();
		if (it->second.equip_addit.size() > 0 || it->second.rune_stone_addit.size() > 0 //||
				//it->second.card_addit.size() > 0 || it->second.rune_addit.size() > 0 ||
				//it->second.gem_addit.size() > 0 || it->second.spirit_addit.size() > 0 ||
				//it->second.baopan_addit.size() > 0 || it->second.fabao_addit.size() > 0 || it->second.exp_ball_addit.size() > 0
				) {
			//Utility_Func::get_block_buff_by_msg(ACTIVE_ITEM_ADDIT_INFO_UPDATE, it->second, buf);
			THIS_SEND_TO_CLIENT(it->second);
		}
	}

	packer_detail_->save_tick();

	return 0;
}

Pack_Type Packer::check_and_change_pack_type(const Pack_Type pack_type, int id) {
	if (pack_type != Pack_Type::PACK_T_PACKAGE_INDEX)
		return pack_type;
	if (0 == is_item_type(id, Item_Detail::CARD))
		return Pack_Type::PACK_T_CARD_BOX_INDEX;
	return pack_type;
}

int Packer::pack_add_money(const Money_Add_Info &info, Money_Opt_Type type, Money_Tips tips) {
	if (info.nums <= 0) {
		MSG_USER_TRACE("add nums : %d", info.nums);
		return ERROR_INNER_SERVER;
	}
	// 金钱警报
//	if (info.nums > 10000000) {
//		if (info.type == GOLD || info.type == COPPER) {
//			Block_Buffer buf;
//			Warning_Struct warning;
//			if (info.type == GOLD) {
//				warning.warning_type = GOLD_ADD_OVER_LIMIT;
//			} else {
//				warning.warning_type = COPPER_ADD_OVER_LIMIT;
//			}
//			warning.role_id = packer_detail_->role_id;
//			warning.sec = LOGIC_MANAGER_INSTANCE->tick_time().sec();
//			warning.str_int_map.insert(std::make_pair("add nums", info.nums));
//			warning.str_int_map.insert(std::make_pair("gold_remain", packer_detail_->money_detail.gold));
//			warning.str_int_map.insert(std::make_pair("copper_remain", packer_detail_->money_detail.copper));
//			std::string backstrace_str;
//			backstrace_string(backstrace_str);
//			warning.str_str_map.insert(std::make_pair("call_stack", backstrace_str));
//
//			buf.make_message(INNER_WARNING);
//			warning.serialize(buf);
//			buf.finish_message();
//			DB_MANAGER_INSTANCE->push_data_block(buf);
//			return ERROR_ADD_MONEY_TO_LARGE;
//		}
//	}

	if (info.dm_info.type >= MONEY_ADD_MAX) {
		MSG_USER_TRACE("add dm_info type: %d wrong", info.type);
		return -1;
	}

	Money_Detail new_money_detail = packer_detail_->money_detail;
	Money_Detail old_money_detail = packer_detail_->money_detail;
	bool need_check_get_limit = is_need_trade_get_limit(info);
	switch (info.type) {
	case COPPER:
		new_money_detail.gold += info.nums;
		break;
	case BIND_GOLD:
		new_money_detail.bind_diamond += info.nums;
		break;
	case GOLD:
		new_money_detail.diamond += info.nums;
		break;
	case DRAGON_SOUL:
		new_money_detail.dragon_soul += info.nums;
		break;
	case SOULS:
		new_money_detail.souls += info.nums;
		break;
	case FRIENDSHIP:
		new_money_detail.friendship += info.nums;
		break;
	case REPUTATION:
		new_money_detail.reputation += info.nums;
		break;
	default:
		MSG_USER("add_type param error.");
		return -1;
	}

	if (need_check_get_limit) {
		int result = check_trade_get_limit(info, type);
		if (result != 0) {
			return result;
		}
	}

	if (0 != this->pack_set_money(new_money_detail, info.dm_info, type)) {
		switch (info.type){
		case COPPER:
			return ERROR_COPPER_OVER_LIMIT;
		case BIND_GOLD:
			return ERROR_GOLD_OVER_LIMIT;
		case GOLD:
			return ERROR_GOLD_OVER_LIMIT;
		case SOULS:
			return ERROR_SOULS_OVER_LIMIT;
		case FRIENDSHIP:
			return ERROR_FRIENDSHIP_OVER_LIMIT;
		default:
			return ERROR_INNER_SERVER;
		}
	}

	if (need_check_get_limit) {
		modify_trade_get_limit(info, type);
	}

	if (tips == MONEY_TIPS_YES) {
		pack_send_money_tips(old_money_detail, new_money_detail);
	}
	switch (info.type){
	case COPPER: {
		player_self()->achievement_listen_gold();
		if (info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_TEAM_FB_AWARD)) {
			player_self()->listen_vip_reward(VIP_WORLD_TREE_TEAM_EXPLORE, COPPER, info.nums);
		}
		break;
	}
	case BIND_GOLD:
		if (
				info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_USE_ITEM) ||//所有礼包
				info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_WAR_AWARD) || //战场
				info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_LEVEL_ACE) ||//升级
				info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_ONLINE_REWARD) ||//在线奖励,在线礼包
				info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_MONTH_CARD_INVEST_PROFIT) ||//投资计划
				info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_UPGRADE_INVEST_LEVEL_PROFIT) ||//投资计划
				false
		) {
			player_self()->push_add_today_get_bind_gold(info.nums);
		}
		break;
	case GOLD:
		break;
	case DRAGON_SOUL:
		if (info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_TEAM_FB_AWARD)) {
			player_self()->listen_vip_reward(VIP_WORLD_TREE_TEAM_EXPLORE, DRAGON_SOUL, info.nums);
		}
		break;
	case SOULS:
		if (info.dm_info.type == static_cast<uint16_t>(MONEY_ADD_TEAM_FB_AWARD)) {
			player_self()->listen_vip_reward(VIP_WORLD_TREE_TEAM_EXPLORE, SOULS, info.nums);
		}
		break;
	case FRIENDSHIP:{
		player_self()->achievement_listen_friendship();
		if (MONEY_OPT_REAL == type) {
			player_self()->add_friendship_accumulate(info.nums);
		}
		break;
	}
	default:
		break;
	}

	return 0;
}
int Packer::pack_sub_money(const Money_Sub_Info &info, Money_Opt_Type type, Money_Tips tips) {
	if (info.nums <= 0) {
		MSG_USER_TRACE("sub nums : %d", info.nums);
		return ERROR_INNER_SERVER;
	}
	if (info.dm_info.type <= MONEY_ADD_MAX) {
		MSG_USER_TRACE("sub dm_info type: %d wrong", info.type);
		return -1;
	}

//	if (consume_check(info) != 0){
//		return -2;
//	}

	int32_t gold_sub = 0;
	Money_Detail new_money_detail = packer_detail_->money_detail;
	Money_Detail old_money_detail = packer_detail_->money_detail;
	bool need_check_pay_limit = is_need_trade_pay_limit(info);
	switch (info.type) {
	case COPPER_ONLY:
		if (is_money_lock(COPPER)) {
			return ERROR_PACK_LOCK;
		}
		new_money_detail.gold -= info.nums;
		break;
	case BIND_GOLD_FIRST:
		if (is_money_lock(BIND_GOLD)) {
			return ERROR_PACK_LOCK;
		}
		if ((int64_t)new_money_detail.bind_diamond + (int64_t)new_money_detail.diamond < info.nums) {
			return ERROR_GOLD_NOT_ENOUGH;
		} else if (new_money_detail.bind_diamond < info.nums) {
			if (is_money_lock(GOLD)) {
				return ERROR_PACK_LOCK;
			}
			new_money_detail.diamond -= info.nums - new_money_detail.bind_diamond;
			gold_sub += info.nums - new_money_detail.bind_diamond;
			new_money_detail.bind_diamond = 0;
		} else {
			new_money_detail.bind_diamond -= info.nums;
		}
		break;
	case BIND_GOLD_ONLY: {
		if (is_money_lock(BIND_GOLD)) {
			return ERROR_PACK_LOCK;
		}
		new_money_detail.bind_diamond -= info.nums;
		break;
	}
	case GOLD_ONLY:
		if (is_money_lock(GOLD)) {
			return ERROR_PACK_LOCK;
		}
		new_money_detail.diamond -= info.nums;
		gold_sub += info.nums;
		break;
	case DRAGON_SOUL_ONLY:
		if (is_money_lock(DRAGON_SOUL)) {
			return ERROR_PACK_LOCK;
		}
		new_money_detail.dragon_soul -= info.nums;
		break;
	case SOULS_ONLY:
		if (is_money_lock(SOULS)) {
			return ERROR_PACK_LOCK;
		}
		new_money_detail.souls -= info.nums;
		break;
	case FRIENDSHIP_ONLY:
		if (is_money_lock(FRIENDSHIP)) {
			return ERROR_PACK_LOCK;
		}
		new_money_detail.friendship -= info.nums;
		break;
	case REPUTATION_ONLY:
		if (is_money_lock(REPUTATION)) {
			return ERROR_PACK_LOCK;
		}
		new_money_detail.reputation -= info.nums;
		break;
	default:
		MSG_USER("sub_type param error.");
		return -1;
	}

	if (need_check_pay_limit) {
		int result = check_trade_pay_limit(info, type);
		if (result != 0) {
			return result;
		}
	}

	if (0 != this->pack_set_money(new_money_detail, info.dm_info, type)) {
		switch (info.type) {
		case COPPER_ONLY:
			return ERROR_COPPER_NOT_ENOUGH;
		case BIND_GOLD_FIRST:
			return ERROR_GOLD_NOT_ENOUGH;
		case BIND_GOLD_ONLY:
			return ERROR_BIND_GOLD_NOT_ENOUGH;
		case GOLD_ONLY:
			return ERROR_GOLD_NOT_ENOUGH;
		case DRAGON_SOUL_ONLY:
			return ERROR_DRAGON_SOUL_NOT_ENOUGH;
		case SOULS_ONLY:
			return ERROR_SOULS_NOT_ENOUGH;
		case FRIENDSHIP_ONLY:
			return ERROR_FRIENDSHIP_NOT_ENOUGH;
		case REPUTATION_ONLY:
			return ERROR_REPUTATION_NOT_ENOUGH;
		default:
			return -1;
		}
	}

	if (need_check_pay_limit) {
		modify_trade_pay_limit(info, type);
	}

	if (tips == MONEY_TIPS_YES) {
		pack_send_money_tips(old_money_detail, new_money_detail);
	}

	if (type == MONEY_OPT_REAL
			&& info.type == GOLD_ONLY) {
		trigger_diamond_sub_info(info.nums, Pack::Money_Lose_Type(info.dm_info.type));
	}
	if (type == MONEY_OPT_REAL
			&& (info.type == BIND_GOLD_FIRST || info.type == BIND_GOLD_ONLY || info.type == GOLD_ONLY)) {
		MSG_81000102 acv_msg;
		acv_msg.type = 1;
		switch (info.type) {
		case BIND_GOLD_FIRST: {
			if (gold_sub == info.nums) {
				acv_msg.property.push_back(Property(GOLD, info.nums));
			} else {
				acv_msg.property.push_back(Property(BIND_GOLD, info.nums - gold_sub));
				if (0 != gold_sub) {
					acv_msg.property.push_back(Property(GOLD, gold_sub));
				}
			}
			break;
		}
		case BIND_GOLD_ONLY: {
			acv_msg.property.push_back(Property(BIND_GOLD, info.nums));
			break;
		}
		case GOLD_ONLY: {
			acv_msg.property.push_back(Property(GOLD, info.nums));
			break;
		}
//		case COPPER_ONLY: {
//			acv_msg.property.push_back(Property(COPPER, info.nums));
//			break;
//		}
		default:
			break;
		}
		THIS_SEND_TO_CLIENT(acv_msg);
	}
//	// 添加消费统计
//	if (info.type == GOLD_ONLY && type != MONEY_OPT_TRY
//			 && info.dm_info.type != MONEY_SUB_MAIL_MONEY && info.dm_info.type != MONEY_SUB_MAIL_POSTAGE
//			 && info.dm_info.type != MONEY_SUB_TRADE_USE && info.dm_info.type != MONEY_SUB_MARKET_CHARGE
//			 && info.dm_info.type != MONEY_SUB_MARKET_PLEDGE && info.dm_info.type != MONEY_SUB_MARKET_STORAGE
//			 && info.dm_info.type != MONEY_SUB_MARKET_BUY && info.dm_info.type != MONEY_SUB_FACTION_DONATE
//			 && info.dm_info.type != MONEY_SUB_GM && info.dm_info.type != MONEY_SUB_MAX) {
////		add_consume_total(LOGIC_MANAGER_INSTANCE->tick_time(), info.nums, info.dm_info);
////		active_common_hd_board(CHT_CONSUME);
//	}

	return 0;
}

int Packer::trigger_diamond_sub_info(const int32_t sub_num, const Pack::Money_Lose_Type type) {
	//VIP系统监听消费累加接口
	player_self()->listen_vip_cost_money(sub_num, type);


	{//日常活跃度监听
		if(type != MONEY_SUB_MAIL_MONEY
					&& type != MONEY_SUB_TRADE_USE
					&& type != MONEY_SUB_MARKET_BUY
					&& type != MONEY_SUB_GM) {
			player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_COST_GOLD);
			player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_GOLD_USE, sub_num);
		}
	}
	return 0;
}

inline void Packer::pack_get_iter(const Pack_Type pack_type, Goods_Map::iterator &begin,
		Goods_Map::iterator &end) {
	switch (pack_type) {
	case PACK_T_ALL_INDEX:
		begin = packer_detail_->goods_map.begin();
		end = packer_detail_->goods_map.end();
		break;
	case PACK_T_EQUIP_INDEX:
	case PACK_T_PACKAGE_INDEX:
	case PACK_T_STORAGE_INDEX:
	case PACK_T_ACHIEVEMENT_INDEX:
	case PACK_T_RUNE_STONE_SMELT_INDEX:
	case PACK_T_RUNE_STONE_PACK_INDEX:
	case PACK_T_RUNE_STONE_EQUIP_INDEX:
	case PACK_T_THOR_LOTTERY_PACK_INDEX:
	case PACK_T_CARD_BOX_INDEX:
		begin = packer_detail_->goods_map.upper_bound(pack_type);
		end = packer_detail_->goods_map.upper_bound(pack_type + pack_get_capacity(pack_type));
		break;
	default:
		begin = end;
		MSG_USER("pack_type param error.");
		break;
	}
}

inline void Packer::pack_get_iter_hero(const int pack_type, Goods_Map::iterator &begin,
		Goods_Map::iterator &end) {
	if (pack_is_hero_type(pack_type)) {	//PACK_T_HERO_EQUIP_INDEX < pack_type && pack_type < PACK_T_HERO_EQUIP_INDEX + ALL_HERO_EQUIP_GAP
		begin = packer_detail_->goods_map.upper_bound(pack_type);
		end = packer_detail_->goods_map.upper_bound(pack_type + pack_get_hero_capacity(pack_type));
	} else {
		begin = end;
		//MSG_USER("int pack_type param error.");
	}
}

int Packer::pack_get_capacity(const Pack_Type pack_type) {
	uint32_t capacity = 0;
	switch (pack_type) {
	case PACK_T_EQUIP_INDEX:
		capacity = packer_detail_->capacity.equip_cap;
		break;
	case PACK_T_PACKAGE_INDEX:
		capacity = packer_detail_->capacity.pack_cap;
		break;
	case PACK_T_STORAGE_INDEX:
		capacity = packer_detail_->capacity.storage_cap;
		break;
	case PACK_T_ACHIEVEMENT_INDEX:
		capacity = packer_detail_->capacity.achievement_cap;
		break;
	case PACK_T_RUNE_STONE_SMELT_INDEX:
		capacity = packer_detail_->capacity.rune_stone_smelt_cap;
		break;
	case PACK_T_RUNE_STONE_PACK_INDEX:
		capacity = packer_detail_->capacity.rune_stone_pack_cap;
		break;
	case PACK_T_RUNE_STONE_EQUIP_INDEX:
		capacity = packer_detail_->capacity.rune_stone_equip_cap;
		break;
	case PACK_T_THOR_LOTTERY_PACK_INDEX:
		capacity = packer_detail_->capacity.thorlottery_cap;
		break;
	case PACK_T_CARD_BOX_INDEX:
		capacity = packer_detail_->capacity.card_cap;
		break;
	default:
		break;
	}
	return capacity;
}

int Packer::pack_get_hero_capacity(const int pack_type) {
	if (player_self()->heroer_detail().hero_map.empty()) {
		return 0;
	}
	uint32_t capacity = 0;
	if (PACK_T_HERO_EQUIP_INDEX < pack_type && pack_type < PACK_T_HERO_EQUIP_INDEX_END) {
		capacity = packer_detail_->capacity.hero_equip_cap;
	} else if (PACK_T_HERO_RUNE_STONE_EQUIP_INDEX < pack_type && pack_type < PACK_T_HERO_RUNE_STONE_EQUIP_INDEX_END) {
		int hero_index = player_self()->get_hero_index_by_pack_index(pack_type);
		Heroer_Detail::Hero_Map::const_iterator it_hero = player_self()->heroer_detail().hero_map.find(hero_index);
		if (it_hero != player_self()->heroer_detail().hero_map.end()) {
			capacity = std_equip_cap(it_hero->second.level);
		}
	}
	return capacity;
}

int Packer::pack_inner_set_cap(const Pack_Type pack_type, const uint32_t set_cap) {
	if (set_cap <= 0) {
		return -1;
	}
	int result = 0;

	switch (pack_type) {
	case PACK_T_RUNE_STONE_EQUIP_INDEX:
		packer_detail_->capacity.rune_stone_equip_cap = std::min(static_cast<uint32_t>(RUNE_STONE_EQUIP_MAX_CAP), set_cap);
		break;
	case PACK_T_RUNE_STONE_PACK_INDEX:
		packer_detail_->capacity.rune_stone_pack_cap = std::min(static_cast<uint32_t>(RUNE_STONE_PACK_MAX_CAP), set_cap);
		break;
	default:
		result = ERROR_INNER_SERVER;
		break;
	}

	packer_detail_->is_change = true;

	this->pack_active_update_capacity(pack_type);

	if (0 == result) {
		result = ERROR_ADD_CAPACITY_SUCCESS;
	}

	return result;
}

int Packer::pack_get_size(const Pack_Type pack_type) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);
	return std::distance(it_begin, it_end);
}

inline void Packer::set_last_wipe_time(void) {
	packer_detail_->last_wipe_time = Time_Value::gettimeofday();
}

inline const Time_Value &Packer::get_last_wipe_time(void) {
	return packer_detail_->last_wipe_time;
}

void Packer::clear_item(Pack_Type pack_type) {
	switch (pack_type) {
	case PACK_T_PACKAGE_INDEX:
	case PACK_T_EQUIP_INDEX:
	case PACK_T_STORAGE_INDEX:
		break;
	default:
		return;
	}

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	packer_detail_->erase(it_begin, it_end);
}

int Packer::pack_fetch_other_equip_info(Block_Buffer &basic_buf, Block_Buffer &addit_buf, int offset) {
	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(PACK_T_EQUIP_INDEX, it_begin, it_end);

	MSG_82100000 basic_msg;
	MSG_82100004 addit_msg;

	for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
		pack_insert_item_to_msg(*item_it->second, PACK_TYPE, basic_msg, addit_msg, *this, offset);
	}

	if (!basic_msg.item_info.empty()) {
		basic_msg.serialize(basic_buf);
	}

	if (!addit_msg.equip_addit.empty()) {
		addit_msg.serialize(addit_buf);
	}

	return 0;
}

int Packer::pack_sell_item(const Int_Int_Vec &index) {
	// 若能删除，则物品存在，下面无需判断
	Index_Amount_Vec index_amount_list;
	for (Int_Int_Vec::const_iterator it = index.begin(); it != index.end(); ++it) {
		index_amount_list.push_back(Index_Amount(it->val_1, it->val_2));
	}
	int result = pack_try_erase_item(index_amount_list);
	if (result != 0) {
		return result;
	}

	Id_Amount_Vec id_amount_list;
	std::vector<const Item_Detail*> item_vec;
	int item_nums = index.size();
	id_amount_list.resize(item_nums);
	item_vec.resize(item_nums);
	for (int i = 0; i < item_nums; ++i) {
		item_vec[i] = pack_get_const_item(index[i].val_1);
		if (player_self()->is_equip_embed_gemstone(*item_vec[i])) {
			return ERROR_EQUIP_EMBED_GEMSTONE;
		}
		id_amount_list[i].id = item_vec[i]->id_;
		if (index[i].val_1 > 0 && item_vec[i]->amount_ < index[i].val_2) {
			return ERROR_CLIENT_OPERATE;
		}
		id_amount_list[i].amount = index[i].val_2;
	}

	Money_Add_List add_info;
	result = get_item_price(id_amount_list, add_info, item_vec);
	if (result != 0) {
		return result;
	}
	result = pack_try_add_money(add_info);
	if (result != 0) {
		return result;
	}

	// 删除指定位置物品
	pack_erase_item(index_amount_list, Lose_Item_DM_Info(ITEM_LOSE_SELL), SELL_ITEM, WITHOUT_TRY);
	pack_add_money(add_info);

	MSG_50100109 res_msg;
	THIS_SEND_TO_CLIENT(res_msg);

	this->pack_hook_sell_item(id_amount_list);
	MSG_81000102 acv_msg;
	for (Money_Add_List::const_iterator it = add_info.begin(); it != add_info.end(); ++it) {
		acv_msg.property.push_back(Property(it->type, it->nums));
	}
	if (!acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return ERROR_PACK_SELL_SUCCESS;
}

void Packer::lock_money(Money_Type type) {
	packer_detail_->money_lock_map[type] = Time_Value::gettimeofday().sec();
}

void Packer::lock_item(uint32_t index) {
	packer_detail_->item_lock_map[index] = Time_Value::gettimeofday().sec();
}

void Packer::unlock_money(Money_Type type) {
	packer_detail_->money_lock_map.erase(type);
}

void Packer::unlock_item(uint32_t index) {
	packer_detail_->item_lock_map.erase(index);
}

// 同一时间被锁定的格子，金钱不会很多，而且正常状态下，都会在很短情况下就解锁，不会超时再解锁，所以不用太关注这里的效率
void Packer::tick(void) {
//	Time_Value now;
//	now = Time_Value::gettimeofday();
//	while (!packer_detail_->timeptr_list.empty()) {
//		Timeptr_Detail timeptr_detail = packer_detail_->timeptr_list.front();
//		if (timeptr_detail.overdue_time > now) break;
//		packer_detail_->timeptr_list.pop_front();
//
//		if (0 == this->pack_try_erase_item(UInt_Vec(timeptr_detail.item.index_)) ) {
//			this->pack_erase_item(UInt_Vec(timeptr_detail.item.index_), Lose_Item_DM_Info(ITEM_LOSE_OVERDUE_TIME, 0), DISCARD_ITEM, WITHOUT_TRY);
//		}
//		break;
//	}

//	if (!is_money_lock() && !is_item_lock()) {
//		return 0;
//	}
//
//	for (Int_Int_Map::iterator it = packer_detail_->money_lock_map.begin();
//			it != packer_detail_->money_lock_map.end();) {
//		if (it->second + MAX_LOCK_SEC < now.sec()) {
//			it = packer_detail_->money_lock_map.erase(it);
//		} else {
//			++it;
//		}
//	}
//
//	for (Int_Int_Map::iterator it = packer_detail_->item_lock_map.begin();
//			it != packer_detail_->item_lock_map.end();) {
//		if (it->second + MAX_LOCK_SEC < now.sec()) {
//			it = packer_detail_->item_lock_map.erase(it);
//		} else {
//			++it;
//		}
//	}

}

inline void Packer::pack_get_index(const Pack_Type pack_type, uint32_t &begin, uint32_t &end) {
	switch (pack_type) {
	case PACK_T_EQUIP_INDEX:
		begin = PACK_T_EQUIP_INDEX + 1;
		end = begin + packer_detail_->capacity.equip_cap;
		break;
	case PACK_T_PACKAGE_INDEX:
		begin = PACK_T_PACKAGE_INDEX + 1;
		end = begin + packer_detail_->capacity.pack_cap;
		break;
	case PACK_T_STORAGE_INDEX:
		begin = PACK_T_STORAGE_INDEX + 1;
		end = begin + packer_detail_->capacity.storage_cap;
		break;
	case PACK_T_RUNE_STONE_SMELT_INDEX:
		begin = PACK_T_RUNE_STONE_SMELT_INDEX + 1;
		end = begin + packer_detail_->capacity.rune_stone_smelt_cap;
		break;
	case PACK_T_RUNE_STONE_PACK_INDEX:
		begin = PACK_T_RUNE_STONE_PACK_INDEX + 1;
		end = begin + packer_detail_->capacity.rune_stone_pack_cap;
		break;
	case PACK_T_RUNE_STONE_EQUIP_INDEX:
		begin = PACK_T_RUNE_STONE_EQUIP_INDEX + 1;
		end = begin + packer_detail_->capacity.rune_stone_equip_cap;
		break;
	case PACK_T_THOR_LOTTERY_PACK_INDEX:
		begin = PACK_T_THOR_LOTTERY_PACK_INDEX + 1;
		end = begin + packer_detail_->capacity.thorlottery_cap;
		break;
	case PACK_T_CARD_BOX_INDEX:
		begin = PACK_T_CARD_BOX_INDEX + 1;
		end = begin + packer_detail_->capacity.card_cap;
		break;
	default:
		begin = 0;
		end = 0;
		break;
	}
}

inline void Packer::pack_get_index(const int pack_type, uint32_t &begin, uint32_t &end) {
	if (PACK_T_HERO_EQUIP_INDEX < pack_type && pack_type < PACK_T_HERO_EQUIP_INDEX + ALL_HERO_EQUIP_GAP) {
		begin = PACK_T_STORAGE_INDEX + 1;
		end = begin + packer_detail_->capacity.hero_equip_cap;
	} else {
		begin = 0;
		end = 0;
	}
}

inline bool Packer::is_money_lock(Money_Type type) {
	return packer_detail_->money_lock_map.count(type);
}

inline bool Packer::is_item_lock(uint32_t index) {
	return packer_detail_->item_lock_map.count(index);
}

inline bool Packer::is_money_lock(void) {
	return !packer_detail_->money_lock_map.empty();
}

inline bool Packer::is_item_lock(void) {
	return !packer_detail_->item_lock_map.empty();
}

bool Packer::is_item_exist(uint32_t index) {
	return packer_detail_->goods_map.count(index);
}

void Packer::set_change(void) {
	packer_detail_->is_change = true;
}

inline Item_Detail *Packer::pack_get_item_pointer(const uint32_t index) {
	Goods_Map::iterator it = packer_detail_->goods_map.find(index);
	if (it != packer_detail_->goods_map.end()) {
		return it->second;
	}
	return NULL;
}

int Packer::pack_try_insert_item(const Pack_Type pack_type, const Item_Detail &item) {
	Pack_Type type = check_and_change_pack_type(pack_type, item.id_);
	return pack_try_insert_item(type, Item_Vec(1, item));
}

int Packer::pack_get_buy_power(const Money_Sub_Type sub_type, int unit_price) {
	if (unit_price <= 0) return 0;
	int64_t total_money;
	switch (sub_type) {
	case COPPER_ONLY:
		total_money = packer_detail_->money_detail.gold;
		break;
	case BIND_GOLD_FIRST:
		total_money = (int64_t)packer_detail_->money_detail.bind_diamond + (int64_t)packer_detail_->money_detail.diamond;
		break;
	case GOLD_ONLY:
		total_money = packer_detail_->money_detail.diamond;
		break;
	case DRAGON_SOUL_ONLY:
		total_money = packer_detail_->money_detail.dragon_soul;
		break;
	case SOULS_ONLY:
		total_money = packer_detail_->money_detail.souls;
		break;
	case FRIENDSHIP_ONLY:
		total_money = packer_detail_->money_detail.friendship;
		break;
	case REPUTATION_ONLY:
		total_money = packer_detail_->money_detail.reputation;
		break;
	default:
		total_money = 0;
		break;
	}

	return total_money / unit_price;
}

int Packer::pack_try_sub_money(const Money_Sub_Info &info) {
	Money_Detail money_detail = packer_detail_->money_detail;
	int result = pack_sub_money(info, MONEY_OPT_TRY, MONEY_TIPS_NO);
	if (result != 0) return result;
	packer_detail_->money_detail = money_detail;
	return 0;
}

int Packer::pack_try_erase_item(const Pack_Type pack_type, const Id_Amount &id_amount, const Item_Vertify_By_Condition con) {
	if (id_amount.amount <= 0) return ERROR_CLIENT_OPERATE;
	MSG_82100008 acv_msg;
	int ret = 0;
	int amount_without_con = pack_calc_item(pack_type, id_amount.id, id_amount.bind_verify);
	int amount_with_con = pack_calc_item(pack_type, id_amount.id, id_amount.bind_verify, con);

	if (amount_without_con < id_amount.amount) {
		ret = ERROR_ITEM_NOT_ENOUGH;
	}

	if (0 == ret && amount_with_con < id_amount.amount) {
		ret = ERROR_PACK_ITEM_OVERDUE;
	}

	if (ret != 0) {
		if (ERROR_ITEM_NOT_ENOUGH == ret) {
			acv_msg.item_ids.push_back(id_amount.id);
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		return ret;
	}
	if (is_item_lock()) { // 存在物品被锁定的情况下，要检测是否有物品被锁定导致不能扣除
		if (pack_calc_unlock_item(pack_type, id_amount.id, id_amount.bind_verify) < id_amount.amount) {
			return ERROR_PACK_LOCK;
		}
	}
	return 0;
}

int Packer::pack_try_add_money(const Money_Add_Info &info) {
	Money_Detail money_detail = packer_detail_->money_detail;
	int result = pack_add_money(info, MONEY_OPT_TRY, MONEY_TIPS_NO);
	if (result != 0) return result;
	packer_detail_->money_detail = money_detail;
	return 0;
}

int Packer::pack_erase_item(const uint32_t index, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type, Pack_Try pack_try) {
	int result = 0;
	UInt_Set changed_set;
	result = this->pack_erase_item(index, dm_info, &changed_set, tips_type, pack_try);

	if (result == 0) {
		result = pack_active_update_item_detail(&changed_set);
	}

	return result;
}

int Packer::pack_erase_item(const uint32_t index, const Lose_Item_DM_Info &lose_dm_info, UInt_Set* changed_set, TIPS_TYPE tips_type, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_erase_item(index);
		if (result != 0) return result;
	}

	Goods_Map::iterator it = packer_detail_->goods_map.find(index);

	// 检查是否装备，重设卡片数据
	if (Item_Detail::EQUIP == it->second->type_) {
		player_self()->reset_card_location(*it->second);
	}

	//this->push_tips(it->second->id_, it->second->amount_, tips_type);
	//this->publish_tips();

	int item_id = it->second->id_;
	write_item_lose_stream(lose_dm_info,
		it->second->id_, it->second->amount_, it->second->bind_);

	packer_detail_->erase(it);

	this->task_listen_item_modify(item_id);

	if (NULL != changed_set) {
		changed_set->insert(index);
	}

	return 0;
}

int Packer::pack_try_erase_item(const Index_Amount &index_amount) {
	if (is_item_lock(index_amount.index)) {
		return ERROR_PACK_LOCK;
	}

	if (index_amount.amount <= 0) {
		return ERROR_CLIENT_OPERATE;
	}

	const Item_Detail *item = pack_get_const_item(index_amount.index);
	// 检测存在物品
	if (NULL == item) return ERROR_ITEM_NOT_EXIST;
	int ret = 0;
	MSG_82100008 acv_msg;
	// 检测物品数量
	if (pack_is_req_item(*item, item->id_, index_amount.bind_verify)) {
		if (index_amount.amount > item->amount_) {
			ret = ERROR_ITEM_NOT_ENOUGH;
			acv_msg.item_ids.push_back(item->id_);
		}
	} else {
		if (index_amount.bind_verify == UNBIND_ONLY) {
			ret = ERROR_UNBIND_ITEM_NOT_ENOUGH;
			acv_msg.item_ids.push_back(item->id_);
		}
		if (index_amount.bind_verify == BIND_ONLY) {
			ret = ERROR_BIND_ITEM_NOT_ENOUGH;
			acv_msg.item_ids.push_back(item->id_);
		}
	}
	if (!acv_msg.item_ids.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	return ret;
}

int Packer::pack_try_erase_item(const Index_Amount_Vec &index_amount_list) {
	// 合并重复index
	std::map<Index_Amount, int32_t> amount_map;
	for (Index_Amount_Vec::const_iterator it = index_amount_list.begin(); it != index_amount_list.end(); ++it) {
		if (it->amount > 0) {
			amount_map[*it] += it->amount;
		} else {
			return ERROR_CLIENT_OPERATE;
		}
	}

	for (std::map<Index_Amount, int32_t>::iterator map_it = amount_map.begin(); map_it != amount_map.end(); ++map_it) {
		int result = pack_try_erase_item(Index_Amount(map_it->first.index, map_it->second, map_it->first.bind_verify));
		if (result != 0) return result;
	}

	return 0;
}

int Packer::pack_erase_item(const Index_Amount_Vec &index_amount_list, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type, Pack_Try pack_try) {
	int result = 0;
	UInt_Set changed_set;
	result = this->pack_erase_item(index_amount_list, dm_info, &changed_set, tips_type, pack_try);

	if (result == 0) {
		result = pack_active_update_item_detail(&changed_set);
	}

	return result;
}

int Packer::pack_try_erase_item(const uint32_t index) {
	if (is_item_lock(index)) {
		return ERROR_PACK_LOCK;
	}
	if (!is_item_exist(index)) {
		return ERROR_ITEM_NOT_EXIST;
	}

	return 0;
}

int Packer::pack_try_erase_item(const UInt_Vec &index_list) {
	// 此处不用合并index，若有多次删除一个格子的情况，与删除一次的效果是一样的
	for (UInt_Vec::const_iterator it = index_list.begin(); it != index_list.end(); ++it) {
		int result = pack_try_erase_item(*it);
		if (result != 0) return result;
	}

	return 0;
}

int Packer::pack_erase_item(const UInt_Vec &index_list, const Lose_Item_DM_Info &dm_info, TIPS_TYPE tips_type, Pack_Try pack_try) {
	int result = 0;
	UInt_Set changed_set;
	result = this->pack_erase_item(index_list, dm_info, &changed_set, tips_type, pack_try);

	if (result == 0) {
		result = pack_active_update_item_detail(&changed_set);
	}

	return result;
}

int Packer::pack_erase_item(const Index_Amount_Vec &index_amount_list, const Lose_Item_DM_Info &lose_dm_info,
		UInt_Set* changed_set, TIPS_TYPE tips_type, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_erase_item(index_amount_list);
		if (result != 0) return result;
	}

	for (Index_Amount_Vec::const_iterator it = index_amount_list.begin(); it != index_amount_list.end(); ++it) {
		pack_erase_item(*it, lose_dm_info, changed_set, tips_type, WITHOUT_TRY);
	}

	return 0;
}

int Packer::pack_erase_item(const UInt_Vec &index_list, const Lose_Item_DM_Info &lose_dm_info, UInt_Set* changed_set, TIPS_TYPE tips_type, Pack_Try pack_try) {
	if (pack_try == WITH_TRY) {
		int result = pack_try_erase_item(index_list);
		if (result != 0) return result;
	}

	for (UInt_Vec::const_iterator it = index_list.begin(); it != index_list.end(); ++it) {
		pack_erase_item(*it, lose_dm_info, changed_set, tips_type, WITH_TRY);
	}

	return 0;
}

int Packer::pack_try_move_item(const uint32_t index_from, const uint32_t index_to) {
	// 英雄背包段做特殊处理
	if (pack_is_hero_type(index_to)) {
		return pack_try_move_to_hero_pack(index_from, index_to);
	}

	Pack_Type type;
	if (0 != pack_get_pack_type(index_to, type)) {
		int err_code = ERROR_PACK_INDEX_NOT_EXIST;
		{
			// 主要是为了处理仓库和背包之间的移动错误提示
			if ( (PACK_T_STORAGE_INDEX < index_to && index_to <= PACK_T_STORAGE_INDEX + STORAGE_MAX_CAPACITY) ||
					(PACK_T_PACKAGE_INDEX < index_to && index_to <= PACK_T_PACKAGE_INDEX + PACK_MAX_CAPACITY) ) {
				get_pack_storage_move_err_code(index_to,err_code);
			}
		}
		return err_code;
	}

	if (!is_item_exist(index_from)) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (is_item_lock(index_from) || is_item_lock(index_to)) {
		return ERROR_PACK_LOCK;
	}

	return 0;
}

int Packer::pack_try_move_to_hero_pack(const uint32_t index_from, const uint32_t index_to) {
	if (! pack_is_hero_type(index_to)) {
		return ERROR_PACK_INDEX_NOT_EXIST;
	}
	if (!is_item_exist(index_from)) {
		return ERROR_ITEM_NOT_EXIST;
	}
	if (is_item_lock(index_from) || is_item_lock(index_to)) {
		return ERROR_PACK_LOCK;
	}
	return 0;
}


int Packer::pack_try_move_item(const uint32_t index_from, const Pack_Type pack_type_to) {
	// 不允许移动至当前包裹
	if (pack_is_pack_type(index_from, pack_type_to) == 0) return ERROR_CLIENT_PARAM;
	if (is_item_lock(index_from)) {
		return ERROR_PACK_LOCK;
	}
	const Item_Detail* item = pack_get_const_item(index_from);
	if (NULL == item) return ERROR_ITEM_NOT_EXIST;

	return pack_try_insert_item(pack_type_to, *item);
}

int Packer::pack_try_move_item(const Pack_Type pack_type_from, const Pack_Type pack_type_to) {
	if (pack_type_to == PACK_T_EQUIP_INDEX) return ERROR_CLIENT_PARAM;

	if (is_item_lock()) {
		return ERROR_PACK_LOCK;
	}

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type_from, it_begin, it_end);

	UInt_Set changed_set;
	Item_Vec item_list;
	for (Goods_Map::iterator it = it_begin; it != it_end; ++it) {
		item_list.push_back(*it->second);
	}

	return pack_try_insert_item(pack_type_to, item_list);
}

int Packer::pack_try_add_money(const Money_Add_List &money_add_list) {
	Money_Detail money_detail = packer_detail_->money_detail;
	int result = pack_add_money(money_add_list, MONEY_OPT_TRY, MONEY_TIPS_NO);
	if (result != 0) return result;
	packer_detail_->money_detail = money_detail;
	return 0;
}

int Packer::pack_add_money(const Money_Add_List &money_add_list, Money_Opt_Type type, Money_Tips tips) {
	Money_Detail old_money_detail = packer_detail_->money_detail;
	int result = 0;
	Money_Add_List temp_add_list;
	for (Money_Add_List::const_iterator it = money_add_list.begin(); it != money_add_list.end(); ++it) {
		if (it->nums > 0) {
			temp_add_list.push_back(*it);
		}
	}
	if (temp_add_list.empty()) {
		MSG_USER_TRACE("add_list empty");
		return ERROR_INNER_SERVER;
	}
	for (Money_Add_List::const_iterator it = temp_add_list.begin(); it != temp_add_list.end(); ++it) {
		result = pack_add_money(*it, MONEY_OPT_TRY, MONEY_TIPS_NO);
		if (result != 0) break;
	}

	// 回滚
	packer_detail_->money_detail = old_money_detail;
	if (result != 0) {
		return result;
	}

	for (Money_Add_List::const_iterator it = temp_add_list.begin(); it != temp_add_list.end(); ++it) {
		pack_add_money(*it, type, MONEY_TIPS_NO);
	}

	if (tips == MONEY_TIPS_YES) {
		pack_send_money_tips(old_money_detail, packer_detail_->money_detail);
	}
	for (Money_Add_List::const_iterator it = temp_add_list.begin(); it != temp_add_list.end(); ++it) {
		switch ((*it).type){
		case COPPER: {
			player_self()->achievement_listen_gold();
			break;
		}
		case BIND_GOLD:
			break;
		case GOLD:
			break;
		case SOULS:
			break;
		case FRIENDSHIP:{
			player_self()->achievement_listen_friendship();
			break;
		}
		default:
			break;
		}
	}

	return 0;
}

int Packer::pack_try_sub_money(const Money_Sub_List &money_sub_list) {
	Money_Detail money_detail = packer_detail_->money_detail;
	int result = pack_sub_money(money_sub_list, MONEY_OPT_TRY, MONEY_TIPS_NO);
	if (result != 0) return result;
	packer_detail_->money_detail = money_detail;
	return 0;
}

int Packer::pack_sub_money(const Money_Sub_List &money_sub_list, Money_Opt_Type type, Money_Tips tips) {
	Money_Detail old_money_detail = packer_detail_->money_detail;
	int result = 0;
	Money_Sub_List temp_sub_list;
	for (Money_Sub_List::const_iterator it = money_sub_list.begin(); it != money_sub_list.end(); ++it) {
		if (it->nums > 0) {
			temp_sub_list.push_back(*it);
		}
	}
	if (temp_sub_list.empty()) {
		MSG_USER_TRACE("sub_list empty");
		return ERROR_INNER_SERVER;
	}
	for (Money_Sub_List::const_iterator it = temp_sub_list.begin(); it != temp_sub_list.end(); ++it) {
		result = pack_sub_money(*it, MONEY_OPT_TRY, MONEY_TIPS_NO);
		if (result != 0) break;
	}

	// 回滚
	packer_detail_->money_detail = old_money_detail;
	if (result != 0) {
		return result;
	}

	for (Money_Sub_List::const_iterator it = temp_sub_list.begin(); it != temp_sub_list.end(); ++it) {
		pack_sub_money(*it, type, MONEY_TIPS_NO);
	}

	if (tips == MONEY_TIPS_YES) {
		pack_send_money_tips(old_money_detail, packer_detail_->money_detail);
	}

	return 0;
}

void Packer::recovery() {
	role_id_t role_id = packer_detail_->role_id;
	packer_detail_->reset();
	packer_detail_->role_id = role_id;
}

int Packer::pack_merge_item(const uint32_t &pack_type) {
	int result = 0;
	if (pack_type != PACK_T_PACKAGE_INDEX && pack_type != PACK_T_STORAGE_INDEX) {
		result = ERROR_CLIENT_PARAM;
	} else {
		result = this->pack_sort_item(static_cast<Pack_Type>(pack_type), MERGE_WAY_SIMILAR);
	}
	return result;
}

void Packer::pack_send_money_tips(const Money_Detail &old_money_detail, const Money_Detail &new_money_detail) {
	MSG_81000101 res_msg;
	res_msg.type = 0;
	Property prop;

	int copper_chg = new_money_detail.gold - old_money_detail.gold;
	int copper_chg_abs = abs(copper_chg);
	if (copper_chg_abs > 0) {
		prop.type = COPPER;
		prop.value = packer_detail().money_detail.gold;
		res_msg.property.push_back(prop);
	}

	int bind_gold_chg = new_money_detail.bind_diamond - old_money_detail.bind_diamond;
	int bind_gold_chg_abs = abs(bind_gold_chg);
	if (bind_gold_chg_abs > 0) {
		prop.type = BIND_GOLD;
		prop.value = packer_detail().money_detail.bind_diamond;
		res_msg.property.push_back(prop);
	}

	int gold_chg = new_money_detail.diamond - old_money_detail.diamond;
	int gold_chg_abs = abs(gold_chg);
	if (gold_chg_abs > 0) {
		prop.type = GOLD;
		prop.value = packer_detail().money_detail.diamond;
		res_msg.property.push_back(prop);
	}

	int dragon_soul_chg = new_money_detail.dragon_soul - old_money_detail.dragon_soul;
	int dragon_soul_chg_abs = abs(dragon_soul_chg);
	if (dragon_soul_chg_abs > 0) {
		prop.type = DRAGON_SOUL;
		prop.value = packer_detail().money_detail.dragon_soul;
		res_msg.property.push_back(prop);
	}

	int souls_chg = new_money_detail.souls - old_money_detail.souls;
	int souls_chg_abs = abs(souls_chg);
	if (souls_chg_abs > 0) {
		prop.type = SOULS;
		prop.value = packer_detail().money_detail.souls;
		res_msg.property.push_back(prop);
	}

	int fri_chg = new_money_detail.friendship - old_money_detail.friendship;
	int fri_chg_abs = abs(fri_chg);
	if (fri_chg_abs > 0) {
		prop.type = FRIENDSHIP;
		prop.value = packer_detail().money_detail.friendship;
		res_msg.property.push_back(prop);
	}


	int rep_chg = new_money_detail.reputation - old_money_detail.reputation;
	int rep_chg_abs = abs(rep_chg);
	if (rep_chg_abs > 0) {
		prop.type = REPUTATION;
		prop.value = packer_detail().money_detail.reputation;
		res_msg.property.push_back(prop);
	}

	if (!res_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(res_msg);
	}
}

void Packer::write_item_gain_stream(const Gain_Item_DM_Info &gain_dm_info, int item_id, int amount, int bind, const Pack_Type pack_type) {
	int pack_item_amount = pack_calc_item(pack_type, item_id);
	Time_Value now = Time_Value::gettimeofday();
	MSG_STREAM("`item_stream`(`is_role_id`, `is_type`, `is_op`, `remain_num`, `is_item_id`, `is_num`, `is_op_time`, `is_bind`) VALUES (%ld,%d,%d,%d,%d,%d,%d,%d)", this->role_id(),  gain_dm_info.type, 1, pack_item_amount, item_id, amount, now.sec(), bind);
//	LOGIC_BACKSTAGE_INSTANCE->request_item_stream(packer_detail_->role_id, gain_dm_info.type,
//		item_id, amount, bind, gain_dm_info.ext_1, gain_dm_info.ext_2, now.sec());

	player_self()->close_guide_by_type(GUIDE_FINISH_TYPE_GET_ITEM, item_id);

	this->task_listen_item_modify(item_id, amount, bind);

	player_self()->active_content_listen_suit_collect(item_id);
	player_self()->achievement_listen_complete_suit(item_id);
//	player_self()->task_listen_package_item(item_id, amount, bind);
//	player_self()->task_listen_collect_item(item_id, amount, bind);

//	const Int_Hash_Set &equip_set = this->equip_set();
//	if (equip_set.count(item_id)) {
//		this->write_wander_equip_info(item_id);
//	}
	if (CONFIG_CACHE_EQUIP->is_suit_part(item_id)) {
		player_self()->achievement_listen_suit_purple(item_id);
//			player_self()->achievement_listen_suit_gold(item.id_);
	}
}

int Packer::pack_move_item_from_package(const int8_t &type, const int32_t &index) {
	int result = 0;
	if (type == 0) {
		result = this->pack_move_item(index, PACK_T_PACKAGE_INDEX);
	} else {
		int item_nums = 0;
		result = this->pack_attempt_move_everyitem(static_cast<Pack_Type>(index), PACK_T_PACKAGE_INDEX, item_nums);
	}

	if (0 == result) {
		MSG_50100111 res_msg;
		THIS_SEND_TO_CLIENT(res_msg);
	}

	return result;
}

int Packer::pack_fetch_package_info_other(MSG_50100112 &res_msg) {
//	// 英雄装备类型做特殊处理
//	if (pack_is_hero_equip_type(pack_type_msg)) {
//		return pack_fetch_hero_equip_info(pack_type_msg);
//	}

	Pack_Type pack_type = PACK_T_EQUIP_INDEX;

	switch (pack_type) {
	case PACK_T_PACKAGE_INDEX:
	case PACK_T_EQUIP_INDEX:
	case PACK_T_STORAGE_INDEX:
		break;
	default:
		return ERROR_CLIENT_PARAM;
	}

	Goods_Map::iterator it_begin;
	Goods_Map::iterator it_end;
	pack_get_iter(pack_type, it_begin, it_end);

	for (Goods_Map::iterator item_it = it_begin; item_it != it_end; ++item_it) {
		pack_insert_item_to_msg(*item_it->second, res_msg, *this);
	}

	// 请求人物装备段时，客户端要求加上部分英雄装备信息
	if (pack_type == PACK_T_EQUIP_INDEX) {
		//pack_fetch_role_panel_hero_equip(res_msg);
		pack_active_role_panel_hero_equip(player_self());
	}

	return 0;
}

int Packer::get_item_price(const std::vector<Id_Amount> &item_info, Money_Add_List &add_info, const std::vector<const Item_Detail*> &item_vec) {
	int money_temp = 0;
	add_info.clear();
	int i = 0;
	for (std::vector<Id_Amount>::const_iterator it = item_info.begin(); it != item_info.end(); ++it) {
		const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(it->id);
		if (!item_config) return ERROR_CONFIG_NOT_EXIST;
		int add_sell_price = 0;
		const Equip_Unban_Lvl_Cfg *unban_lvl_cfg = CONFIG_CACHE_EQUIP->find_unban_lvl_cfg(item_vec[i]->addon.equip.unban_lvl);
		if (unban_lvl_cfg) {
			add_sell_price = unban_lvl_cfg->add_sell_price;
		}
		money_temp = (item_config->sellPrice + add_sell_price) * it->amount;
		if (money_temp <= 0) {
			return ERROR_ITEM_UNSELLABLE;
		} else {
			add_info.push_back(Money_Add_Info(COPPER, money_temp, Money_DM_Info(MONEY_ADD_SELL_GOODS, it->id, it->amount)));
		}
		++i;
	}

	return 0;
}

int Packer::pack_move_tower_package_item(const uint32_t &index) {
	int result = 0;
	if (index != 0) {
		result = this->pack_move_item(index, PACK_T_PACKAGE_INDEX);
	} else {
//		int item_nums;
		result = -1;//pack_attempt_move_everyitem(PACK_T_TOWER_WIPE_INDEX, PACK_T_PACKAGE_INDEX, item_nums);
	}


	return result;
}

void Packer::write_item_lose_stream(const Lose_Item_DM_Info &lose_dm_info, int item_id, int amount, int bind, const Pack_Type pack_type) {
//	LOGIC_BACKSTAGE_INSTANCE->request_item_stream(packer_detail_->role_id, lose_dm_info.type,
//		item_id, amount, bind, lose_dm_info.ext_1, lose_dm_info.ext_2, LOGIC_MANAGER_INSTANCE->tick_time().sec());
	int pack_item_amount = pack_calc_item(pack_type, item_id);
	Time_Value now = Time_Value::gettimeofday();
	MSG_STREAM("`item_stream`(`is_role_id`, `is_type`, `is_op`, `remain_num`, `is_item_id`, `is_num`, `is_op_time`, `is_bind`) VALUES (%ld,%d,%d,%d,%d,%d,%d,%d)", this->role_id(),  lose_dm_info.type, 2, pack_item_amount, item_id, amount, now.sec(), bind);
//	player_self()->task_listen_package_item(item_id, amount, bind);
//	player_self()->task_listen_collect_item(item_id, amount, bind);
}

void Packer::write_item_move_stream(const Move_Item_DM_Info &move_dm_info, int item_id, int amount, int bind) {
	int pack_item_amount = 0;
	if (pack_is_pack_type(move_dm_info.ext_1, Pack::PACK_T_STORAGE_INDEX) == 0 ||
			pack_is_pack_type(move_dm_info.ext_2, Pack::PACK_T_STORAGE_INDEX) == 0 ||
			   move_dm_info.ext_2 == PACK_T_STORAGE_INDEX ) {
		//仓库进出流水,这个时候记录人物背包的数量
//		int stream_type = (pack_is_pack_type(move_dm_info.ext_1, Pack::PACK_T_STORAGE_INDEX) == 0) ?
//				ITEM_MOVE_STORE_PACK : ITEM_MOVE_PACK_STORE;
		pack_item_amount = pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, item_id);
	}
	Time_Value now = Time_Value::gettimeofday();
	MSG_USER("`item_stream`(`is_role_id`, `is_type`, `is_op`, `remain_num`, `is_item_id`, `is_num`, `is_op_time`, `is_bind`) VALUES (%ld,%d,%d,%d,%d,%d,%d,%d)", this->role_id(),  move_dm_info.type, 3, pack_item_amount, item_id, amount, now.sec(), bind);
	MSG_STREAM("`item_stream`(`is_role_id`, `is_type`, `is_op`, `remain_num`, `is_item_id`, `is_num`, `is_op_time`, `is_bind`) VALUES (%ld,%d,%d,%d,%d,%d,%d,%d)", this->role_id(),  move_dm_info.type, 3, pack_item_amount, item_id, amount, now.sec(), bind);
//		LOGIC_BACKSTAGE_INSTANCE->request_item_stream(packer_detail_->role_id, stream_type, item_id,
//				amount, bind, move_dm_info.ext_1, move_dm_info.ext_2, LOGIC_MANAGER_INSTANCE->tick_time().sec());

	if (!(pack_is_pack_type(move_dm_info.ext_2, Pack::PACK_T_PACKAGE_INDEX) == 0
			&& pack_is_pack_type(move_dm_info.ext_1, Pack::PACK_T_PACKAGE_INDEX) == 0)) {
		if (pack_is_pack_type(move_dm_info.ext_1, Pack::PACK_T_PACKAGE_INDEX) == 0){
			this->task_listen_item_modify(item_id);
		}

		if (pack_is_pack_type(move_dm_info.ext_2, Pack::PACK_T_PACKAGE_INDEX) == 0){
			this->task_listen_item_modify(item_id);
		}
	}
}

void Packer::task_listen_item_modify(int item_id, int amount, int bind) {
	int pack_item_amount = pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, item_id);
	player_self()->task_listen_item_update(item_id, pack_item_amount);
	player_self()->task_listen_have_equip_but_no_erase();
}

int Packer::modify_trade_pay_limit(const Money_Sub_Info &info, Money_Opt_Type type) {
	bool is_inner_player = false;//this->is_inner_player();
	const Trade_Limit_Config &trade_limit_config = CONFIG_CACHE_PACKAGE->trade_limit_config(is_inner_player);
	if (info.type == GOLD_ONLY) {
		if (trade_limit_config.gold_pay_limit > 0) {
			packer_detail_->money_detail.today_diamond_pay += info.nums;
			if (type == MONEY_OPT_REAL) {
				int limit_left = trade_limit_config.gold_pay_limit - packer_detail_->money_detail.today_diamond_pay;
				active_client_trade_limit_info(GOLD, 0, limit_left, trade_limit_config.gold_pay_limit);
			}
		}
	} else if (info.type == COPPER_ONLY) {
		if (trade_limit_config.copper_pay_limit > 0) {
			packer_detail_->money_detail.today_gold_pay += info.nums;
			if (type == MONEY_OPT_REAL) {
				int limit_left = trade_limit_config.copper_pay_limit - packer_detail_->money_detail.today_gold_pay;
				active_client_trade_limit_info(COPPER, 0, limit_left, trade_limit_config.copper_pay_limit);
			}
		}
	}

	return 0;
}

int Packer::check_trade_pay_limit(const Money_Sub_Info &info, Money_Opt_Type type) {
	bool is_inner_player = false;//this->is_inner_player();
	const Trade_Limit_Config &trade_limit_config = CONFIG_CACHE_PACKAGE->trade_limit_config(is_inner_player);
	if (info.type == GOLD_ONLY) {
		if (trade_limit_config.gold_pay_limit > 0) {
			int limit_left = trade_limit_config.gold_pay_limit - packer_detail_->money_detail.today_diamond_pay;
			if (info.nums > limit_left) {
				active_client_trade_limit_info(GOLD, 0, limit_left, trade_limit_config.gold_pay_limit);
				if (is_inner_player) {
					return ERROR_PERM_LIMIT;
				} else {
					return ERROR_GOLD_OVER_TRADE_LIMIT;
				}
			}
		}
	} else if (info.type == COPPER_ONLY) {
		if (trade_limit_config.copper_pay_limit > 0) {
			int limit_left = trade_limit_config.copper_pay_limit - packer_detail_->money_detail.today_gold_pay;
			if (info.nums > limit_left) {
				active_client_trade_limit_info(COPPER, 0, limit_left, trade_limit_config.copper_pay_limit);
				if (is_inner_player) {
					return ERROR_PERM_LIMIT;
				} else {
					return ERROR_COPPER_OVER_TRADE_LIMIT;
				}
			}
		}
	}

	return 0;
}

int Packer::modify_trade_get_limit(const Money_Add_Info &info, Money_Opt_Type type) {
	bool is_inner_player = false;//this->is_inner_player();
	const Trade_Limit_Config &trade_limit_config = CONFIG_CACHE_PACKAGE->trade_limit_config(is_inner_player);
	if (info.type == GOLD) {
		if (trade_limit_config.gold_get_limit > 0) {
			packer_detail_->money_detail.today_diamond_get += info.nums;
			if (type == MONEY_OPT_REAL) {
				int limit_left = trade_limit_config.gold_get_limit - packer_detail_->money_detail.today_diamond_get;
				active_client_trade_limit_info(GOLD, 1, limit_left, trade_limit_config.gold_get_limit);
			}
		}
	} else if (info.type == COPPER) {
		if (trade_limit_config.copper_get_limit > 0) {
			packer_detail_->money_detail.today_gold_get += info.nums;
			if (type == MONEY_OPT_REAL) {
				int limit_left = trade_limit_config.copper_get_limit - packer_detail_->money_detail.today_gold_get;
				active_client_trade_limit_info(COPPER, 1, limit_left, trade_limit_config.copper_get_limit);
			}
		}
	}

	return 0;
}

int Packer::check_trade_get_limit(const Money_Add_Info &info, Money_Opt_Type type) {
	bool is_inner_player = false;//this->is_inner_player();
	const Trade_Limit_Config &trade_limit_config = CONFIG_CACHE_PACKAGE->trade_limit_config(is_inner_player);
	if (info.type == GOLD) {
		if (trade_limit_config.gold_get_limit > 0) {
			int limit_left = trade_limit_config.gold_get_limit - packer_detail_->money_detail.today_diamond_get;
			if (info.nums > limit_left) {
				active_client_trade_limit_info(GOLD, 1, limit_left, trade_limit_config.gold_get_limit);
				if (is_inner_player) {
					return ERROR_PERM_LIMIT;
				} else {
					return ERROR_GOLD_OVER_TRADE_LIMIT;
				}
			}
		}
	} else if (info.type == COPPER) {
		if (trade_limit_config.copper_get_limit > 0) {
			int limit_left = trade_limit_config.copper_get_limit - packer_detail_->money_detail.today_gold_get;
			if (info.nums > limit_left) {
				active_client_trade_limit_info(COPPER, 1, limit_left, trade_limit_config.copper_get_limit);
				if (is_inner_player) {
					return ERROR_PERM_LIMIT;
				} else {
					return ERROR_COPPER_OVER_TRADE_LIMIT;
				}
			}
		}
	}

	return 0;
}

void Packer::active_client_trade_limit_info(int money_type, int trade_type, int limit_left, int limit_total) {
//	if (this->is_inner_player()) {	// 内部玩家不通知交易额度信息
//		return;
//	}

	MSG_82100005 msg;
	msg.limit_type = money_type;
	msg.trade_type = trade_type;
	msg.limit_left = limit_left;
	msg.limit_total = limit_total;
	THIS_SEND_TO_CLIENT(msg);
}

void Packer::pack_modify_item_before_insert(Item_Detail &item) {
	Item_Detail::Item_Type item_type = Item_Detail::NORMAL;
	item.get_item_type(item.id_, item_type);
	switch (item_type) {
	case Item_Detail::EQUIP:
		break;
	default:
		break;
	}
}

//int Packer::pack_insert_timeptr(Item_Detail &item) {
//	if (item.overdue_time > Time_Value::zero) {
//		Time_Value overdue_time = item.overdue_time - Time_Value::gettimeofday();
//		if (overdue_time < Time_Value::zero) {
//			pack_clean_overdue();
//		} else {
//			this->create_event(Logic_Enum::EVENT_LOGIC_PACK_TIME, &Item_User::pack_clean_overdue, overdue_time, 1);
//		}
//	}
//	return 0;
//}
//
//int Packer::pack_insert_timeptr(Item_Vec &item_list) {
//	for (Item_Vec::iterator it = item_list.begin(); it != item_list.end(); ++it) {
//		pack_insert_timeptr(*it);
//	}
//	return 0;
//}
