/*
 * Pack_Struct.cpp
 *
 *  Created on: 2014年3月12日
 *      Author: root
 */

#include "Pack_Struct.h"
#include "DB_Operator.h"
#include "Pool_Manager.h"
#include "Base_Struct.h"
#include "Config_Cache_Item.h"
#include "Msg_Struct.h"
#include "Pack_Func.h"
#include "equiper/Equiper_Func.h"
#include "Config_Cache_Card.h"
#include "card/Card_Config.h"
#include "card/Card_Func.h"


int Money_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(gold);
	buffer.write_int32(bind_diamond);
	buffer.write_int32(diamond);
	buffer.write_int32(dragon_soul);
	buffer.write_int32(souls);
	buffer.write_int32(friendship);
	buffer.write_int32(reputation);

	buffer.write_int32(today_diamond_pay);
	buffer.write_int32(today_diamond_get);
	buffer.write_int32(today_gold_pay);
	buffer.write_int32(today_gold_get);

	return 0;
}

int Money_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(gold);
	buffer.read_int32(bind_diamond);
	buffer.read_int32(diamond);
	buffer.read_int32(dragon_soul);
	buffer.read_int32(souls);
	buffer.read_int32(friendship);
	buffer.read_int32(reputation);

	buffer.read_int32(today_diamond_pay);
	buffer.read_int32(today_diamond_get);
	buffer.read_int32(today_gold_pay);
	buffer.read_int32(today_gold_get);

	return 0;
}

void Money_Detail::reset(void) {
	gold = 0;
	bind_diamond = 0;
	diamond = 0;
	dragon_soul = 0;
	souls = 0;
	friendship = 0;
	reputation = 0;

	today_diamond_pay = 0;
	today_diamond_get = 0;
	today_gold_pay = 0;
	today_gold_get = 0;
}

Money Money_Detail::money(void) const {
	Money money;
	money.copper = gold;
	money.bind_gold = bind_diamond;
	money.gold = diamond;
	money.dragon_soul = dragon_soul;
	money.souls = souls;
	money.friendship = friendship;
	money.reputation = reputation;

	return money;
}

void Enchant_Prop_Detail::serialize(Block_Buffer & w) const {
	w.write_int32(prop_type);
	w.write_int32(prop_val);
	w.write_int8(star);
	w.write_int8(is_lock);
}

int Enchant_Prop_Detail::deserialize(Block_Buffer & r) {
	if(r.read_int32(prop_type) ||
			r.read_int32(prop_val) ||
			r.read_int8(star) ||
			r.read_int8(is_lock)) {
		return -1;
	}
	return 0;
}

void Enchant_Prop_Detail::reset(void) {
	prop_type = 0;
	prop_val = 0;
	star = 0;
	is_lock = 0;
}

void Enchant_Prop_Group_Detail::serialize(Block_Buffer & w) const {
	w.write_uint8(cnt_enchant_prop);
	for (uint16_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP && i < cnt_enchant_prop; ++i) {
		enchant_props[i].serialize(w);
	}

	w.write_int32(score);

}

int Enchant_Prop_Group_Detail::deserialize(Block_Buffer & r)  {
	r.read_uint8(cnt_enchant_prop);
	for (uint16_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP && i < cnt_enchant_prop; ++i) {
		enchant_props[i].deserialize(r);
	}

	r.read_int32(score);

	return 0;
}

void Enchant_Prop_Group_Detail::reset(void)  {
	memset(this, 0x00, sizeof(Enchant_Prop_Group_Detail));
}

void Equip_Addon_Detail::serialize(Block_Buffer &w) const {
	w.write_uint32(index);
	w.write_int32(unban_lvl);
	w.write_int32(score);
	w.write_int32(force);

	w.write_uint8(incr_level);
	w.write_int32(incr_luck_value);
	w.write_int16(incr_lvl_conti_fail_cnt);

	w.write_uint8(cnt_enchant_group);
	for (uint16_t i = 0; i < Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP && i < cnt_enchant_group; ++i) {
		enchant_groups[i].serialize(w);
	}

	for (uint16_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
		enchant_lock_prop_fail[i].serialize(w);
	}
}

int Equip_Addon_Detail::deserialize(Block_Buffer &r) {
	r.read_uint32(index);
	r.read_int32(unban_lvl);
	r.read_int32(score);
	r.read_int32(force);

	r.read_uint8(incr_level);
	r.read_int32(incr_luck_value);
	r.read_int16(incr_lvl_conti_fail_cnt);

	r.read_uint8(cnt_enchant_group);
	for (uint16_t i = 0; i < Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP && i <= cnt_enchant_group; ++i) {
		enchant_groups[i].deserialize(r);
	}

	for (uint8_t i = 0; i < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++i) {
		enchant_lock_prop_fail[i].deserialize(r);
	}

	return 0;
}

void Equip_Addon_Detail::reset(void) {
	memset(this, 0x00, sizeof(Equip_Addon_Detail));
}

int Equip_Addon_Detail::init_equip_addon(int item_id) {
	force = E_FUNC::calculate_equip_force(item_id);

	const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(item_id);
	if (0 == cfg) return 0;

	for (int i=0; i <cfg->equip_cfg.init_card_holes; ++i) {
		card_hole_array_[i].state = Hole_State::STATE_OPEN;
	}

//	const Int_Int* value = CONFIG_CACHE_ITEM->get_gemstone_hole_info(cfg->color);
//	if (0 == value) return 0;
	for (int i=0; i < cfg->equip_cfg.gemstone_holes.val_1; ++i) {
		gemstones[i].state = Hole_State::STATE_OPEN;
	}

	return 0;
}

void Equip_Addon_Detail::make_item_equip_info(Item_Equip_Info &info) const {
	info.index = index;
	info.unban_lvl = unban_lvl;
	info.score = force;
	info.force = force;

	info.incr_level = incr_level;
	info.incr_luck_value = incr_luck_value;
	info.incr_fail_add_rate = incr_lvl_conti_fail_cnt;

	Enchant_Prop_Group_Info enchant_prop_group_info;
	Enchant_Prop_Info enchant_prop_info;
	for (uint16_t i = 0; i < Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP && i < cnt_enchant_group; ++i) {
		enchant_prop_group_info.reset();
		for (uint16_t j = 0; j < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP && j < enchant_groups[i].cnt_enchant_prop; ++j) {
			enchant_prop_info.reset();
			enchant_prop_info.prop_type = enchant_groups[i].enchant_props[j].prop_type;
			enchant_prop_info.prop_val = enchant_groups[i].enchant_props[j].prop_val;
			enchant_prop_info.star = enchant_groups[i].enchant_props[j].star;
			enchant_prop_info.is_lock = enchant_groups[i].enchant_props[j].is_lock;
			enchant_prop_group_info.group_info.push_back(enchant_prop_info);
		}
		enchant_prop_group_info.score = enchant_groups[i].score;
		//enchant_prop_group_info.cnt_enchant_prop = enchant_groups[i].cnt_enchant_prop;
		info.enchant_prop_groups.push_back(enchant_prop_group_info);
	}

	Int_Int value;
	for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		value.reset();
		value.val_1 = gemstones[i].state;
		value.val_2 = gemstones[i].stone_id;
		info.gemstone_vec.push_back(value);
	}
}

void Equip_Addon_Detail::make_item_equip_info(Item_Equip_Info &info, Packer &packer) const {
	info.index = index;
	info.unban_lvl = unban_lvl;
	info.score = force;
	info.force = force;

	info.incr_level = incr_level;
	info.incr_luck_value = incr_luck_value;
	info.incr_fail_add_rate = incr_lvl_conti_fail_cnt;

	Enchant_Prop_Group_Info enchant_prop_group_info;
	Enchant_Prop_Info enchant_prop_info;
	for (uint16_t i = 0; i < Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP && i < cnt_enchant_group; ++i) {
		enchant_prop_group_info.reset();
		for (uint16_t j = 0; j < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP && j < enchant_groups[i].cnt_enchant_prop; ++j) {
			enchant_prop_info.reset();
			enchant_prop_info.prop_type = enchant_groups[i].enchant_props[j].prop_type;
			enchant_prop_info.prop_val = enchant_groups[i].enchant_props[j].prop_val;
			enchant_prop_info.star = enchant_groups[i].enchant_props[j].star;
			enchant_prop_info.is_lock = enchant_groups[i].enchant_props[j].is_lock;
			enchant_prop_group_info.group_info.push_back(enchant_prop_info);
		}
		enchant_prop_group_info.score = enchant_groups[i].score;
		//enchant_prop_group_info.cnt_enchant_prop = enchant_groups[i].cnt_enchant_prop;
		info.enchant_prop_groups.push_back(enchant_prop_group_info);
	}

	Card_Info card_info;
	for (int i=0; i<Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
		if (Hole_State::STATE_NOT_OPEN == card_hole_array_[i].state) continue;
		card_info.reset();
		uint32_t card_index = card_hole_array_[i].card_index;
		if (0 == card_index) {
			card_info.equip_loc  = i+1;
			info.card_info_vec.push_back(card_info);
		} else {
			const Item_Detail* card = packer.pack_get_const_item(card_index);
			if (0 == card) continue;
			card->addon.card.make_item_card_info(card->id_, card->index_, card_info);
			info.card_info_vec.push_back(card_info);
		}
	}

	Int_Int value;
	for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		value.reset();
		value.val_1 = gemstones[i].state;
		value.val_2 = gemstones[i].stone_id;
		info.gemstone_vec.push_back(value);
	}
}


void Rune_Stone_Addon_Detail::serialize(Block_Buffer &w) const {
	w.write_uint32(index);
	w.write_int32(exp_lvl);
	w.write_int32(exp);
	w.write_int8(is_lock);
}

int Rune_Stone_Addon_Detail::deserialize(Block_Buffer &r) {
	r.read_uint32(index);
	r.read_int32(exp_lvl);
	r.read_int32(exp);
	r.read_int8(is_lock);

	return 0;
}

void Rune_Stone_Addon_Detail::reset(void) {
	memset(this, 0x00, sizeof(Rune_Stone_Addon_Detail));
}

int Rune_Stone_Addon_Detail::init_rune_stone_addon(int item_id) {
	exp_lvl = 1;	// 符石初始的经验等级为1级
	return 0;
}

void Rune_Stone_Addon_Detail::make_item_rune_stone_info(Item_Rune_Stone_Info &info) const {
	info.index = index;
	info.exp_lvl = exp_lvl;
	info.exp = exp;
	info.is_lock = is_lock;
}

void Card_Addon_Detail::serialize(Block_Buffer &w) const {

	w.write_int32(part_);
	w.write_int32(poten_);
	w.write_int32(star_);
	w.write_int32(lv_);
	w.write_int32(exp_);
	w.write_int32(main_attr_);
	w.write_int32(sub_attr_);
	w.write_uint32(equip_id);
	w.write_uint8(equip_loc);
}

int Card_Addon_Detail::deserialize(Block_Buffer &r) {

	r.read_int32(part_);
	r.read_int32(poten_);
	r.read_int32(star_);
	r.read_int32(lv_);
	r.read_int32(exp_);
	r.read_int32(main_attr_);
	r.read_int32(sub_attr_);
	r.read_uint32(equip_id);
	r.read_uint8(equip_loc);

	return 0;
}

void Card_Addon_Detail::reset(void) {

	part_		= 0;
	poten_		= 0;
	star_		= 0;
	lv_			= 0;
	exp_		= 0;
	main_attr_	= 0;
	sub_attr_	= 0;
	equip_id    = 0;
	equip_loc	= 0;
}

int Card_Addon_Detail::init_card_info(int item_id) {

	const Card_Config* cfg = CONFIG_CACHE_CARD->get_card_config(item_id);
	if (0 == cfg) return 0;
	if (0 != lv_ && 0 != poten_) return 0;

	star_ = 0;
	lv_ = cfg->init_lv;
	poten_ = handle_get_potential(*cfg);
	exp_ = 0;

	if (EXP_CARD_TYPE != cfg->type) {
		Int_Int part_val;
		if (0 != handle_rand_card_part(*cfg, part_val)) return ERROR_INNER_SERVER;
		part_ = part_val.val_1;
		main_attr_ = part_val.val_2;
		sub_attr_ = handle_get_sub_attr(*cfg);
	}
	return 0;
}

void Card_Addon_Detail::make_item_card_info(int id, int idx, Card_Info &info) const {
	info.reset();
	info.id = id;
	info.index = idx;
	info.lv = lv_;
	info.cur_exp = exp_;
	info.poten = poten_;
	info.main_attr.id = main_attr_;
	info.main_attr.value = handle_get_attr_value(id, main_attr_, star_, lv_, poten_);
	info.sub_attr.id = sub_attr_;
	info.sub_attr.value = handle_get_attr_value(id, sub_attr_, star_, lv_, poten_);
	info.star = star_;
	info.part = part_;
	info.item_id = equip_id;
	info.equip_loc = equip_loc;
	info.embed_state = (equip_id == 0) ? 0 : 1;
}


Item_Detail::Item_Detail(void) {
	reset();
}

Item_Detail::Item_Detail(uint32_t id, int32_t amount, uint8_t bind) {
	reset();
	id_ = id;
	amount_ = amount;
	bind_ = bind;
	init();
}

Item_Detail::Item_Detail(const Item_Basic_Info &item) {
	reset();
//	item_basic = item;
	index_ = item.index;
	seq_ = item.seq;
	id_ = item.id;
	amount_ = item.amount;
	bind_ = item.bind;
	init();
}

Item_Detail::~Item_Detail() {}

int Item_Detail::init() {
	switch (bind_) {
	case UNDEFINED: {
		const Item_Detail_Config *cfg = CONFIG_CACHE_ITEM->find_item(id_);
		if (cfg) {
			bind_ = cfg->bind;
		} else {
			bind_ = BIND;
		}
		break;
	}
	case UNBIND:
		bind_ = UNBIND;
		break;
	// 若策划配置错了，物品为绑定
	default:
		bind_ = BIND;
		break;
	}

	get_item_type(id_, type_);
	if (type_ == EQUIP) {
		addon.equip.reset();
		addon.equip.init_equip_addon(id_);
	} else if(type_ == RUNE_STONE) {
		addon.rune_stone.reset();
		addon.rune_stone.init_rune_stone_addon(id_);
//	} else if (type_ == CARD) {
//		addon.card.reset();
//		Int_Vec pro_type_vector;
//		addon.card.init_card(item_basic.id, pro_type_vector);
//	} else if (type == SKILL_STONE) {
//		addon.skill_stone.reset();
//	} else if (type == PET_CARD) {
//		addon.pet_card.reset();
//	} else if (type == RUNE) {
//		addon.rune.reset();
//	} else if(type == TASK_ITEM) {
//		addon.task_item.reset();
//	} else if(type == MOUNT_EQUIP) {
//		addon.mount_equip.reset();
//	} else if(type == GEM) {
//		addon.gem.reset();
//		addon.gem.gem_id = item_basic.id;
//	} else if(type == SPIRIT) {
//		addon.spirit.reset();
//	} else if(type == BAOPAN) {
//		addon.baopan.reset();
//		addon.baopan.init_baopan(item_basic.id);
//	} else if(type == FABAO) {
//		addon.fabao.reset();
//		addon.fabao.init_fabao(item_basic.id);
//	} else if(type == EXP_BALL) {
//		addon.exp_ball.reset();
	} else if (type_ == CARD){
//		addon.card.reset();
		addon.card.init_card_info(id_);
	} else {
		memset(&addon, 0x00, sizeof (addon));
	}
	return 0;
}

int Item_Detail::serialize(Block_Buffer &buffer) const {
//	item_basic.serialize(buffer);
	buffer.write_uint32(index_);
	buffer.write_uint32(seq_);
	buffer.write_uint32(id_);
	buffer.write_int32(amount_);
	buffer.write_uint8(bind_);
	buffer.write_int64(overdue_time.sec());

	buffer.write_int32(type_);
	/*switch (type) {
	case EQUIP:
		addon.equip.serialize(buffer);
		break;
	case CARD:
		addon.card.serialize(buffer);
		break;
	case SKILL_STONE:
		addon.skill_stone.serialize(buffer);
		break;
	case PET_CARD:
		addon.pet_card.serialize(buffer);
		break;
	case RUNE:
		addon.rune.serialize(buffer);
		break;
	case TASK_ITEM:
		addon.task_item.serialize(buffer);
		break;
	case MOUNT_EQUIP:
		addon.mount_equip.serialize(buffer);
		break;
	case GEM:
		addon.gem.serialize(buffer);
		break;
	case SPIRIT:
		addon.spirit.serialize(buffer);
		break;
	case BAOPAN:
		addon.baopan.serialize(buffer);
		break;
	case FABAO:
		addon.fabao.serialize(buffer);
		break;
	case EXP_BALL:
		addon.exp_ball.serialize(buffer);
		break;
	default:
		break;
	}*/

	return 0;
}

int Item_Detail::deserialize(Block_Buffer &buffer){
	reset();
//	item_basic.deserialize(buffer);
	buffer.read_uint32(index_);
	buffer.read_uint32(seq_);
	buffer.read_uint32(id_);
	buffer.read_int32(amount_);
	buffer.read_uint8(bind_);

	int64_t sec = 0;
	buffer.read_int64(sec);
	overdue_time.set(sec, 0);

	int type_value = 0;
	buffer.read_int32(type_value);
	type_ = static_cast<Item_Type>(type_value);
	/*switch (type) {
	case EQUIP:
		addon.equip.deserialize(buffer);
		break;
	case CARD:
		addon.card.deserialize(buffer);
		break;
	case SKILL_STONE:
		addon.skill_stone.deserialize(buffer);
		break;
	case PET_CARD:
		addon.pet_card.deserialize(buffer);
		break;
	case RUNE:
		addon.rune.deserialize(buffer);
		break;
	case TASK_ITEM:
		addon.task_item.deserialize(buffer);
		break;
	case MOUNT_EQUIP:
		addon.mount_equip.deserialize(buffer);
		break;
	case GEM:
		addon.gem.deserialize(buffer);
		break;
	case SPIRIT:
		addon.spirit.deserialize(buffer);
		break;
	case BAOPAN:
		addon.baopan.deserialize(buffer);
		break;
	case FABAO:
		addon.fabao.deserialize(buffer);
		break;
	case EXP_BALL:
		addon.exp_ball.deserialize(buffer);
		break;
	default:
		break;
	}*/

	return 0;
}

void Item_Detail::reset(void){
//	item_basic.reset();
	index_ = 0;
	seq_ = 0;
	id_ = 0;
	amount_ = 0;
	bind_ = 0;
	type_ = NORMAL;
	overdue_time = Time_Value::zero;
	memset(&addon, 0x00, sizeof(addon));
}

int Item_Detail::is_item_type(const uint32_t item_id, Item_Type item_type) {
	bool is_infer_type = false;
	if (EQUIP_START <= item_id && item_id < EQUIP_END) {
		is_infer_type = (item_type == EQUIP);
	}
	if (RUNE_STONE_START <= item_id && item_id < RUNE_STONE_END) {
		is_infer_type = (item_type == RUNE_STONE);
	}

	return is_infer_type? 0 : -1;
}

int Item_Detail::get_item_type(const uint32_t item_id, Item_Type &item_type) {
	int first_type = item_id / 10000000;
	switch(first_type) {
	case EQUIP:
		item_type = EQUIP;
		break;
	case TOOL:
		item_type = TOOL;
		break;
	case DRUG:
		item_type = DRUG;
		break;
	case RUNE_STONE:
		item_type = RUNE_STONE;
		break;
	case CARD:
		item_type = CARD;
		break;
	default:
		item_type = NORMAL;
	}
	return item_type;
}

int Item_Detail::get_item_type(void) const {
	int first_type = this->id_ / 10000000;
	switch(first_type) {
	case EQUIP:
		return EQUIP;
	case TOOL:
		return TOOL;
	case DRUG:
		return DRUG;
	case RUNE_STONE:
		return RUNE_STONE;
	case CARD:
		return CARD;
	default:
		break;
	}
	return NORMAL;
}

int Item_Detail::get_item_type_second(void) const {
	int first_type = this->id_ / 10000000;
	int second_type = this->id_ / 100000;
	switch(first_type) {
	case EQUIP:
		return EQUIP;
	case TOOL:
	{
		switch(second_type) {
		case TOOL_TASK:
			return TOOL_TASK;
		case TOOL_LOTTERY:
			return TOOL_LOTTERY;
		case TOOL_FLOWER:
			return TOOL_FLOWER;
		case TOOL_DRUG:
			return TOOL_DRUG;
		case TOOL_MATERIAL:
			return TOOL_MATERIAL;
		case TOOL_CARD:
			return TOOL_CARD;
		case TOOL_RESOURCE:
			return TOOL_RESOURCE;
		case TOOL_CONSUME_TOOL:
			return TOOL_CONSUME_TOOL;
		case TOOL_ROLL:
			return TOOL_ROLL;
		case TOOL_GEMSTONE:
			return TOOL_GEMSTONE;
		default:
			break;
		}
		break;
	}
	case DRUG:
		return DRUG;
	case RUNE_STONE:
		return RUNE_STONE;
	case CARD:
		return CARD;
	default:
		break;
	}
	return NORMAL;
}

int Item_Detail::get_item_color(void) const {
	int color = (this->id_ / 100) % 10;
	if (color < COLOR_BEGIN || color > COLOR_END) {
		return 0;
	}
	return color;
}

void Item_Detail::set_basic(const uint32_t index, const uint32_t id, const int32_t amount, const uint8_t bind) {
	index_ = index;
	id_ = id;
	amount_ = (amount > 0)? amount : 0;
	bind_ = bind;
	// get item type
	init();
}

int32_t Item_Detail::get_item_stack_upper(const uint32_t item_id) {
	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if (!item_config) {
		MSG_USER("error item %d", item_id);
		return 1;
	}

	return item_config->overlay;
}

//是否不可交易
bool Item_Detail::is_item_nontradable(void) const{
	return BIND == bind_;//return CONFIG_INSTANCE->item(item_id)["forbid_trade"].asBool();
}

int Item_Detail::get_item_remain_amount(void) {
	return get_item_stack_upper(id_) - amount_;
}

bool Item_Detail::operator == (const Item_Detail &cmp) const {
	if (is_equal_item(*this, cmp)) {
		return true;
	} else {
		return false;
	}
}

bool is_similar_item(const Item_Detail &item1, const Item_Detail &item2) {
	return item1.id_ == item2.id_;
}

bool is_equal_item(const Item_Detail &item1, const Item_Detail &item2) {
	if (item1.id_ == item2.id_ && item1.bind_ == item2.bind_) {
		if (0 == memcmp(&item1.addon, &item2.addon, sizeof(Item_Detail::Addon))) {
			return true;
		}
	}
	return false;
}

void Item_Detail::set_item_basic_info(Item_Basic_Info &item_base) const {
	item_base.seq = seq_;
	item_base.index = index_;
	item_base.id = id_;
	item_base.bind = bind_;
	item_base.amount = amount_;
	item_base.overdue_time = overdue_time.sec();
}

void Item_Detail::get_item_basic_info(const Item_Basic_Info &item_base) {
	seq_ = item_base.seq;
	index_ = item_base.index;
	id_ = item_base.id;
	bind_ = item_base.bind;
	amount_ = item_base.amount;
}

bool Item_Detail::set_item_card_info(Card_Info &card_info) const {
	if (this->type_ != Item_Detail::CARD) return false;
	addon.card.make_item_card_info(id_, index_, card_info);
	return true;
}

bool operator<(const Item_Detail &item1, const Item_Detail &item2) {
	int type1 = item1.get_item_type();
	int type2 = item2.get_item_type();
	if (type1 == type2) {
		return less_detail(item1, item2);
	} else if (type1 == Item_Detail::EQUIP){
		return true;
	} else if (type2 == Item_Detail::EQUIP) {
		return false;
	} else if (type1 == Item_Detail::RUNE_STONE) {
		return true;
	} else if (type2 == Item_Detail::RUNE_STONE) {
		return false;
	}else if (type1 == Item_Detail::CARD) {
		return true;
	}else if (type2 == Item_Detail::CARD) {
		return false;
	} else {
		return less_detail(item1, item2);
	}
	return false;
}

bool less_detail(const Item_Detail &item1, const Item_Detail &item2) {
	int color1 = item1.get_item_color();
	int color2 = item2.get_item_color();
	if (color1 > color2) {
		return true;
	} else if (color1 < color2) {
		return false;
	}
	const Item_Detail_Config *item1_con = CONFIG_CACHE_ITEM->find_item(item1.id_);
	const Item_Detail_Config *item2_con = CONFIG_CACHE_ITEM->find_item(item2.id_);
	if (!item1_con || !item2_con) {
		if (!item1_con && !item2_con) {
			MSG_USER("item_id error %d, %d", item1.id_, item2.id_);
			return item1.id_ < item2.id_;
		} else if (!item1_con) {
			MSG_USER("item_id error %d", item1.id_);
			return false;
		} else if (!item2_con) {
			MSG_USER("item_id error %d", item2.id_);
			return true;
		}
		return true;
	}

	if (item1_con->needLevel < item2_con->needLevel) {
		return true;
	} else if (item1_con->needLevel == item2_con->needLevel
			&& item1.id_ < item2.id_) {
		return true;
	} else if (item1_con->needLevel == item2_con->needLevel
			&& item1.id_ == item2.id_ && item1_con->bind > item2_con->bind) {
		return true;
	} else if (item1_con->needLevel == item2_con->needLevel
			&& item1.id_ == item2.id_ && item1_con->bind == item2_con->bind
			&& item1.amount_ < item2.amount_) {
		return true;
	}
	return false;
}

bool operator<(const Id_Amount &id_amount1, const Id_Amount &id_amount2) {
	if (id_amount1.id < id_amount2.id) return true;
	if (id_amount1.id == id_amount2.id && id_amount1.bind_verify > id_amount2.bind_verify) return true;
	return false;
}

Id_Amount& Id_Amount::operator= (const Item_Id_Amount& id_amount) {
	this->id = id_amount.id;
	this->amount = id_amount.amount;
	return *this;
}

const uint16_t Capacity::hero_equip_cap = 20;
const uint16_t Capacity::equip_cap = 19;
const uint16_t Capacity::achievement_cap = 14;
const uint16_t Capacity::rune_stone_smelt_cap = 24;

Packer_Detail::Packer_Detail(void) {
	reset();
}

Packer_Detail::~Packer_Detail(void) {

}

int Packer_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Packer_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

void Packer_Detail::reset(void) {
	role_id = 0;
	capacity.pack_cap = PACK_INIT_CAPACITY;
	capacity.storage_cap = STORAGE_INIT_CAPACITY;
	capacity.spirit_equip_cap = SPIRIT_EQUIP_INIT_CAPACITY;
	capacity.spirit_package_cap = SPIRIT_PACKAGE_INIT_CAPACITY;
	capacity.rune_stone_equip_cap = RUNE_STONE_EQUIP_INIT_CAP;
	capacity.rune_stone_pack_cap = RUNE_STONE_PACK_INIT_CAP;
	capacity.thorlottery_cap = THORLOTTERY_INIT_CAPACITY;
	capacity.card_cap = CARD_BOX_INIT_CAPACITY;
	money_detail.reset();

	for (Goods_Map::const_iterator it = goods_map.begin(); it != goods_map.end(); ++it) {
		POOL_MANAGER->push_item_pool(it->second, 17);
	}
	goods_map.clear();
	money_lock_map.clear();
	item_lock_map.clear();
	last_wipe_time = Time_Value::zero;
	is_change = false;
}

int Packer_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_packer_detail(*this);
	return 0;
}

int Packer_Detail::load(void) {
	if (role_id)
		MONGO_OPERATOR->load_packer_detail(*this);
	return 0;
}

Packer_Detail &Packer_Detail::operator=(const Packer_Detail &detail) {
	role_id = detail.role_id;
	capacity = detail.capacity;
	money_detail = detail.money_detail;

	Item_Detail *item = 0;
	for (Goods_Map::const_iterator it = detail.goods_map.begin(); it != detail.goods_map.end(); ++it) {
		item = POOL_MANAGER->pop_item_pool(2);
		if (! item) {
			MSG_USER("item_pool().pop() return 0.");
			continue;
		}
		(*item) = *(it->second);
		if(!goods_map.insert(std::make_pair(item->index_, item)).second) {
			MSG_USER("insert goods map i:%d", item->index_);
			POOL_MANAGER->push_item_pool(item, 16);
		}
	}

	last_wipe_time = detail.last_wipe_time;
	is_change = detail.is_change;

	return *this;
}

void Packer_Detail::erase(uint32_t index) {
	Goods_Map::iterator it = goods_map.find(index);
	if (it != goods_map.end()) {
		//LOGIC_MANAGER_INSTANCE->item_pool().push(it->second);
//		LOGIC_MANAGER_INSTANCE->pool_repo().push_item_detail(it->second);
		POOL_MANAGER->push_item_pool(it->second, 15);
		goods_map.erase(it);
	}
}

void Packer_Detail::erase(Goods_Map::iterator iter) {
	//LOGIC_MANAGER_INSTANCE->item_pool().push(iter->second);
//	LOGIC_MANAGER_INSTANCE->pool_repo().push_item_detail(iter->second);
	POOL_MANAGER->push_item_pool(iter->second,14);
	goods_map.erase(iter);
}

void Packer_Detail::erase(Goods_Map::iterator begin, Goods_Map::iterator end) {
	for (Goods_Map::iterator it = begin; it != end; ) {
		//LOGIC_MANAGER_INSTANCE->item_pool().push(it->second);
//		LOGIC_MANAGER_INSTANCE->pool_repo().push_item_detail(it->second);
		POOL_MANAGER->push_item_pool(it->second, 13);
		goods_map.erase(it++);
	}
}
