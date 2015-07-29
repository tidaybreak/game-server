/*
 * DB_Operator_U.cpp
 *
 *  Created on: 2014年12月19日
 *      Author: root
 */

#include "heroer/Logic_Heroer_Struct.h"
#include "DB_Operator.h"
#include "mongo/client/dbclient.h"
#include "Config_Cache.h"
#include "DB_Fields.h"
#include "mail/Mail_Struct.h"
#include "mail/Mail_Def.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_Public.h"
#include "fashion/Fashion_Struct.h"
#include "Fighter_Detail.h"
#include "Config_Cache_Fighter.h"
//#include "Config_Cache_Expedition.h"
//#include "expedition/Expedition_Struct.h"
//#include "expedition_scene/Expedition_Scene_Struct.h"
//#include "gang/Gang_Struct.h"


void DB_Operator::package_add_copper(role_id_t role_id, int value) {
	BSONObjBuilder tmp_builder;
	tmp_builder << "package.money.copper" << value;

	MONGO_CONNECTION.update(Package_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id),
			BSON("$inc" << tmp_builder.obj()), true);
}

void DB_Operator::package_add_bind_gold(role_id_t role_id, int value) {
	BSONObjBuilder tmp_builder;
	tmp_builder << "package.money.bind_gold" << value;

	MONGO_CONNECTION.update(Package_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)role_id),
			BSON("$inc" << tmp_builder.obj()), true);
}

int DB_Operator::mongo_update_on_start(void) {
	const String_Set &command = CONFIG_CACHE->db_command();
	for (String_Set::const_iterator it = command.begin(); it != command.end(); ++it) {
		BSONObj res = MONGO_CONNECTION.findOne("mmo.mongo_repair", QUERY("command" << *it));
		if (! res.isEmpty()) {
			continue;
		}

		BSONObjBuilder builder;
		if (*it == "db_repair_20141219_handle_fix_wing_data") {
			db_repair_20141219_handle_fix_wing_data();
		} else if (*it == "db_repair_20141220_handle_fix_invest_data") {
			db_repair_20141220_handle_fix_invest_data();
		} else if (*it == "db_repair_20150112_handle_handle_fix_wing_state_data"){
			db_repair_20150112_handle_handle_fix_wing_state_data();
		} else if(*it == "db_repair_20150112_handle_mount_compensation"){
			//db_repair_20150112_handle_mount_compensation();
		}
		else if (*it == "xxxx") {

		}
		else if(*it == "db_repair_20150113_handle_37wan_upgrade_invest_plan_data") {
			db_repair_20150113_handle_37wan_upgrade_invest_plan_data();
		} else if(*it == "db_repair_20150114_handle_r2_auction") {
			db_repair_20150114_handle_r2_auction();
		} else if(*it == "db_repair_20150114_handle_37_helper_daily") {
			db_repair_20150114_handle_37_helper_daily();
		} else if (*it == "db_repair_20150122_hanle_repay_rune") {
			db_repair_20150122_hanle_repay_rune();
		} else if (*it == "db_repair_20150205_handle_repay_equip_card_info") {
			db_repair_20150205_handle_repay_equip_card_info();
		} else if (*it == "db_repair_20150202_handle_anci_artifact") {
			db_repair_20150202_handle_anci_artifact();
		} else if (*it == "db_repair_20150213_handle_elite_dungeon_reimburse") {
			db_repair_20150213_handle_elite_dungeon_reimburse();
//		} else if (*it == "db_repair_20150311_handle_expedition_repair") {
//			db_repair_20150311_handle_expedition_repair();
		} else if (*it == "db_repair_20150321_handle_hero_and_feats"){
			db_repair_20150321_handle_hero_and_feats();
		} else if (*it == "db_repair_20150321_handle_fashion") {
			db_repair_20150321_handle_fashion();
		} else if (*it == "db_repair_20150321_handle_skill") {
			db_repair_20150321_handle_skill();
		} else if (*it == "db_repair_20150328_handle_hero_and_feats") {
			db_repair_20150328_handle_hero_and_feats();
		}
		else {
			continue;
		}

		MONGO_CONNECTION.update("mmo.mongo_repair", QUERY("command" << *it), BSON("$set" << builder.obj()), true);
	}

	return 0;
}

void DB_Operator::db_repair_20141220_handle_fix_invest_data() {
	BSONObjBuilder saveBuilder;
	saveBuilder << "start_time" << (long long int)0;
	MONGO_CONNECTION.update("mmo.invest_plan", mongo::Query(), BSON("$set" << saveBuilder.obj()), false, true);
}

void DB_Operator::db_repair_20141219_handle_fix_wing_data() {
	int compensation_array[] = {
			0,				// 0
			100,			// 1
			500,			// 2
			1800,			// 3
			5000,			// 4
			11500,			// 5
			25000,			// 6
			68500,			// 7
			207000,			// 8
			470000,			// 9
			900000,			// 10
	};

	unsigned long long res_count = MONGO_CONNECTION.count("mmo.wing");
	BSONObjBuilder field_builder;
	field_builder 	<< "role_id" << 1
					<< "lv" << 1
					;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);

	MONGO_CONNECTION.findN(iter_record, "mmo.wing", mongo::Query(), res_count, 0, &field_bson);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {
		role_id_t role_id = (*it)["role_id"].numberLong();
		int lv = (*it)["lv"].numberInt();
		if (lv <= 0) continue;

		if (lv > 10) lv = 10;
		int money = compensation_array[lv];

		BSONObjBuilder saveBuilder;
		saveBuilder << "package.money.bind_gold" << money;
		MONGO_CONNECTION.update("mmo.package", QUERY("role_id" << (long long int)role_id),
				BSON("$inc" << saveBuilder.obj()), true);
	}
}

void DB_Operator::db_repair_20150112_handle_mount_compensation(void){
	unsigned long long res_count = MONGO_CONNECTION.count("mmo.mount");
	BSONObjBuilder field_builder;
	field_builder 	<< "role_id" << 1
					<< "horse_list" << 1
					;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	int lv_prop_num[11] = {
			1,
			3,
			5,
			10,
			10,
			20,
			40,
			125,
			300,
			300,
			0,
	};
	int base_prop_num[11] ={
			0,
			10,
			50,
			130,
			330,
			730,
			1530,
			3130,
			8130,
			20130,
			20130
	};
	int total_role_num = 0;
	int total_prop_num = 0;

	MONGO_CONNECTION.findN(iter_record, "mmo.mount", mongo::Query(), res_count, 0, &field_bson);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {
		role_id_t role_id = (*it)["role_id"].numberLong();
		int prop_num = 0;
		int lv_prop_num_1 = 0;
		int lv_prop_num_2 = 0;
		if ((*it).hasField("horse_list")) {
			BSONObjIterator info_iter((*it).getObjectField("horse_list"));
			while (info_iter.more()){
				BSONElement info_obj = info_iter.next();
				int train_lv = info_obj["train_lv"].numberInt();
				int train_num = info_obj["train_num"].numberInt();
				int horse_id = info_obj["horse_id"].numberInt();
				if(train_lv < 0){
					train_lv = 0;
				}
				if(train_lv > 10){
					train_lv = 10;
				}
				if(train_num < 0){
					train_num = 0;
				}
				if(train_num > 1000){
					train_num = 1000;
				}
				prop_num += base_prop_num[train_lv] + train_num*lv_prop_num[train_lv];
				if(horse_id == 30102303){// 剑齿虎
					prop_num += 500;
					lv_prop_num_1 = 10;
				}else if(horse_id == 30301502){// 地狱犬
					lv_prop_num_2 = 5;
				}else if(horse_id == 30301101){// 恶魔熊
					prop_num += 1000;
				}else if(horse_id == 30101101){// 独角兽
					prop_num += 300;
				}
			}
	    }
		if(prop_num > 0){
			total_role_num += 1;
			total_prop_num += prop_num;
			LOG_DEBUG("20150112_handle_mount_compensation, role_id:%ld, prop_num:%d",
					role_id, total_prop_num);
			int tmp_num_1 = prop_num/5000;
			int tmp_num_2 = prop_num%5000;
			int mail_num = tmp_num_1;
			if(tmp_num_2 > 0){
				mail_num += 1;
			}
			Mail_Send_Info send_info;
			send_info.sender_id = 0;
			send_info.sender_name = "系统";
			send_info.send_type = MAIL_TYPE_SYS;
			send_info.show_send_time = Time_Value::gettimeofday();
			send_info.receiver_id = role_id;
			send_info.title = "坐骑补偿";
			send_info.content = "亲爱的玩家，鉴于游戏测试体验，我们本次更新对坐骑玩法做了更人性化的调整，并对所有拥有坐骑的玩家进行对应补偿。";

			if(lv_prop_num_1 > 0){
				send_info.item_vector.push_back(Item_Detail(20513503, lv_prop_num_1, Item_Detail::BIND));
			}
			if(lv_prop_num_2 > 0){
				send_info.item_vector.push_back(Item_Detail(20513504, lv_prop_num_2, Item_Detail::BIND));
			}
			for(int i = 1; i <= mail_num; ++i){
				int amount = 5000;
				if(i > tmp_num_1){
					amount = tmp_num_2;
				}
				send_info.item_vector.push_back(Item_Detail(20513401, amount, Item_Detail::BIND));
			}
			MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);
		}
	}
	LOG_DEBUG("db_repair_20150112_handle_mount_compensation, total_role_num:%d, total_prop_num",
			total_role_num, total_prop_num);
}

void DB_Operator::db_repair_20150112_handle_handle_fix_wing_state_data() {
	unsigned long long res_count = MONGO_CONNECTION.count("mmo.wing");
	BSONObjBuilder field_builder;
	field_builder << "role_id" << 1
			<< "lv" << 1
			;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);

	MONGO_CONNECTION.findN(iter_record, "mmo.wing", mongo::Query(), res_count, 0, &field_bson);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {
		role_id_t role_id = (*it)["role_id"].numberLong();
		int lv = (*it)["lv"].numberInt();

		int state = (lv == 0) ? 0 : 2;
		BSONObjBuilder saveBuilder;
		saveBuilder << "state" << state;
		MONGO_CONNECTION.update("mmo.wing", QUERY("role_id" << (long long int)role_id),
				BSON("$set" << saveBuilder.obj()), true);
	}
}

void DB_Operator::db_repair_20150113_handle_37wan_upgrade_invest_plan_data() {
	unsigned long long res_count = MONGO_CONNECTION.count(Invest_Plan_Fields::COLLECTION);
	BSONObjBuilder field_builder;
	field_builder 	<< Invest_Plan_Fields::ROLE_ID << 1
					<< Invest_Plan_Fields::INVESTED_MONEY << 1;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);

	MONGO_CONNECTION.findN(iter_record, Invest_Plan_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {
		role_id_t role_id = (*it)[Invest_Plan_Fields::ROLE_ID].numberLong();
		const uint32_t invested_money = (*it)[Invest_Plan_Fields::INVESTED_MONEY].numberInt();
		if (invested_money == 0) {
			continue;
		}

		const uint32_t compensate_num = invested_money * 30 / 100;
		if(compensate_num == 0) {
			continue;
		}

		Mail_Send_Info send_info;
		send_info.sender_id = 0;
		send_info.sender_name = "系统";
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.receiver_id = role_id;
		send_info.title = "等级投资（55级）补偿";
		send_info.content = "本次更新调整了等级投资奖励内容，特向开通等级投资的玩家进行相关补偿";
		send_info.show_send_time = Time_Value::gettimeofday();

		Item_Detail senior_diamond_packet;
		senior_diamond_packet.id_ = 20705403;
		senior_diamond_packet.amount_ = compensate_num / 80;
		senior_diamond_packet.bind_ = 2;

		Item_Detail diamond_packet;
		diamond_packet.id_ = 20705402;
		diamond_packet.amount_ = (compensate_num % 80) / 10;
		diamond_packet.bind_ = 2;

		if(senior_diamond_packet.amount_ > 0) {
			send_info.item_vector.push_back(senior_diamond_packet);
		}

		if(diamond_packet.amount_ > 0) {
			send_info.item_vector.push_back(diamond_packet);
		}

		if(senior_diamond_packet.amount_ > 0 || diamond_packet.amount_  > 0)  {
			MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);
		}
	}

	iter_record.clear();
}

void DB_Operator::db_repair_20150114_handle_r2_auction(void) {
	BSONObjBuilder saveBuilder;
	saveBuilder << "over_time" << (long long int)Time_Value::gettimeofday().sec();
	MONGO_CONNECTION.update("mmo.auction_goods", mongo::Query(), BSON("$set" << saveBuilder.obj()), false, true);
}

void DB_Operator::db_repair_20150114_handle_37_helper_daily(void) {
	BSONObjBuilder saveBuilder;
	UInt_Vec all_award_receive; all_award_receive.clear();
	saveBuilder << "all_receive" << all_award_receive;
	MONGO_CONNECTION.update("mmo.daily", mongo::Query(), BSON("$set" << saveBuilder.obj()), false, true);
}

const static int RUNE_REPAY_DATA[20] = {1, 1, 2, 3, 4, 4, 4, 5, 5, 5, 6, 7, 7, 8, 8, 9, 9, 9, 10, 10};

int get_repay_item_id_by_part_and_career(int part, int career, int lv, int &amount) {
	int item_id = 0;
	switch(part) {
	case 1:				// weapon
		if (3==career) {
			item_id = 21502010;
		} else {
			item_id = 21501010;
		}
		break;
	case 2:				//hat
		item_id = 21503010;
		amount = 2;
		break;
	case 3:				// cloth
		item_id = 21505010;
		break;
	case 4:				// shoe
		item_id = 21504010;
		amount = 2;
		break;
	case 5:case 6:	// necklace
		item_id = 21505010;
		break;
	case 7:case 8:	//ring
		if (3 == career) {
			item_id = 21502010;
		} else {
			item_id = 21501010;
		}
		break;
	default:
		break;
	}

	if (item_id == 0) return 0;
	return item_id + (lv -1) * 10;
}

void DB_Operator::db_repair_20150122_hanle_repay_rune(void) {

	Int_Int_Map repay_item_map;
	unsigned long long res_count = MONGO_CONNECTION.count("mmo.rune");
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.rune", mongo::Query(), res_count);

	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {
		repay_item_map.clear();
		role_id_t role_id = (*it)["role_id"].numberLong();

		BSONObj get_career_obj = BSON("career" << 1);
		BSONObj career_obj = MONGO_CONNECTION.findOne("mmo.role",
				QUERY("role_id" << (long long int)role_id), &get_career_obj);
		int career = career_obj["career"].numberInt();

		BSONObjIterator iter_rune = it->getObjectField("parts");
		while(iter_rune.more()) {
			BSONObj obj_rune = iter_rune.next().embeddedObject();
			int parts = obj_rune["rune_part"].numberInt();
			parts %= 100;
			BSONObjIterator iter_hole = obj_rune.getObjectField("holes");
			while(iter_hole.more()) {
				BSONObj obj_hole = iter_hole.next().embeddedObject();
				int level = obj_hole["level"].numberInt();
				if (level <= 0 || level >= 20) continue;
				int repay_level = RUNE_REPAY_DATA[level - 1];
				int amount = 1;
				int repay_item_id = get_repay_item_id_by_part_and_career(parts, career, repay_level, amount);
				if (0 == repay_item_id) continue;
				repay_item_map[repay_item_id] += amount;
			}
		}

		if (repay_item_map.empty()) continue;
		// send emails
		Mail_Send_Info send_info;
		send_info.sender_id = 0;
		send_info.sender_name = "系统";
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.receiver_id = role_id;
		send_info.title = "宝石补偿";
		send_info.content = "本次更新调整了宝石内容，特向开通宝石的玩家进行相关补偿";
		send_info.show_send_time = Time_Value::gettimeofday();
		for (Int_Int_Map::iterator item_it = repay_item_map.begin();
				item_it != repay_item_map.end(); ++item_it) {
			Item_Detail item(item_it->first, item_it->second, 2);
			send_info.item_vector.push_back(item);
		}

		if (!send_info.item_vector.empty()) {
			MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);
		}
	}
}

void DB_Operator::db_repair_20150205_handle_repay_equip_card_info(void) {

	unsigned long long role_count = MONGO_CONNECTION.count("mmo.role");
	BSONObjBuilder field_builder;
	field_builder << "role_id" << 1;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(role_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.role", mongo::Query(), role_count, 0, &field_bson);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {
		Packer_Detail detail;
		detail.role_id = (*it)["role_id"].numberLong();

		// read data
		load_packer_detail(detail);

		// fixed data
		for (Goods_Map::iterator item_it = detail.goods_map.begin();
				item_it != detail.goods_map.end(); ++item_it) {
			if (0 == item_it->second) continue;
			if (Item_Detail::CARD == item_it->second->type_) {
				item_it->second->addon.card.equip_id = 0;
				item_it->second->addon.card.equip_loc = 0;
			} else if (Item_Detail::EQUIP == item_it->second->type_) {
				const Item_Detail_Config *cfg_item = CONFIG_CACHE_ITEM->find_item(item_it->second->id_);
				if (0 == cfg_item) continue;
				for (int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
					item_it->second->addon.equip.card_hole_array_[i].card_index = 0;
					if (i >= cfg_item->equip_cfg.card_holes) continue;
					item_it->second->addon.equip.card_hole_array_[i].state = Hole_State::STATE_OPEN;
				}
			}
		}

		// save data
		save_packer_detail(detail);
		detail.reset();
	}
}

void DB_Operator::db_repair_20150202_handle_anci_artifact(void) {
	unsigned long long anci_artifact_count = MONGO_CONNECTION.count("mmo.anci_artifact");
	std::vector<BSONObj> iter_record;
	iter_record.reserve(anci_artifact_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.anci_artifact", mongo::Query(), anci_artifact_count);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it){
		BSONObj &res = *it;
		role_id_t role_id = res["role_id"].numberLong();

		bool complete = false;
		vector<BSONObj> complete_ids;
		BSONObj complete_list = res.getObjectField("complete_ids");
		BSONObjIterator iter_complete(complete_list);
		while (iter_complete.more()) {
			BSONObj obj = iter_complete.next().embeddedObject();
			int id = (uint32_t)obj[Anci_Artifact_Fields::AA_Info::ID].numberInt();
			if (id == 1032) {
				complete = true;
			}
			complete_ids.push_back(obj);
		}
		if (complete) continue;
		BSONObjBuilder building_builder;
		building_builder << Anci_Artifact_Fields::AA_Info::ID << 1032;
		complete_ids.push_back(building_builder.obj());


		BSONObj list = res.getObjectField("aa_info");
		BSONObjIterator iter_building(list);
		vector<BSONObj> aa_info;
		while (iter_building.more()) {
			BSONObj obj = iter_building.next().embeddedObject();
			int id = (uint32_t)obj[Anci_Artifact_Fields::AA_Info::ID].numberInt();
			if (id != 1032) {
				aa_info.push_back(obj);
			}
		}

		vector<BSONObj> aa_unban_info;
		BSONObj aa_unban_list = res.getObjectField("aa_unban_info");
		BSONObjIterator iter_aa_unban(aa_unban_list);
		while (iter_aa_unban.more()) {
				BSONObj obj = iter_aa_unban.next().embeddedObject();
				aa_unban_info.push_back(obj);
		}

		BSONObjBuilder builder;
		builder << Anci_Artifact_Fields::ROLE_ID << (long long int)role_id
	//			<< Anci_Artifact_Fields::PERFECTPOINT << (long long int)detail.perfectPoint
		        << Anci_Artifact_Fields::AA_INFO_MAP << aa_info
		        << Anci_Artifact_Fields::AA_COMPLETE_IDS << complete_ids
		        << Anci_Artifact_Fields::AA_UNBAN_INFO << aa_unban_info
		        ;

		MONGO_CONNECTION.update(Anci_Artifact_Fields::COLLECTION, QUERY(Anci_Artifact_Fields::ROLE_ID << (long long int)role_id),
				BSON("$set" << builder.obj()), true);
	}

}

const static int L1_Array[] = {21709101, 21709201, 21709301, 21709401, 21709501, 21709601, 21709701};// 45~49
const static int L2_Array[] = {21709101, 21709201, 21709301, 21709401, 21709501, 21709601, 21709701,
		21709102, 21709202, 21709302, 21709402, 21709502, 21709602, 21709702}; // 50~59
const static int L3_Array[] = {21709101, 21709201, 21709301, 21709401, 21709501, 21709601, 21709701,
		21709102, 21709202, 21709302, 21709402, 21709502, 21709602, 21709702, 21709103, 21709203,
		21709303, 21709403, 21709503, 21709603, 21709703}; // 60~69

void DB_Operator::db_repair_20150213_handle_elite_dungeon_reimburse(void) {

	unsigned long long role_count = MONGO_CONNECTION.count("mmo.role");
	BSONObjBuilder field_builder;
	field_builder << "role_id" << 1 << "level" << 1;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(role_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.role", mongo::Query(), role_count, 0, &field_bson);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {

		role_id_t role_id = (*it)["role_id"].numberLong();
		int lv = (*it)["level"].numberInt();
		if (lv < 45) continue;

		Mail_Send_Info send_info;
		send_info.sender_id = 0;
		send_info.sender_name = "系统";
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.receiver_id = role_id;
		send_info.title = "英雄之门奖励补偿";
		send_info.content = "本次更新调整了英雄之门任务奖励，特向各位英雄补偿任务奖励-宝石图纸";
		send_info.show_send_time = Time_Value::gettimeofday();

		Item_Detail item;
		item.amount_ = 1;
		item.bind_ = 2;
		if (lv < 50) {
			uint size = sizeof(L1_Array) / sizeof(L1_Array[0]);
			for (uint i = 0; i < size; ++i) {
				item.id_ = L1_Array[i];
				send_info.item_vector.push_back(item);
			}
		}else if (lv < 60) {
			uint size = sizeof(L2_Array) / sizeof(L2_Array[0]);
			for (uint i = 0; i < size; ++i) {
				item.id_ = L2_Array[i];
				send_info.item_vector.push_back(item);
			}
		} else {
			uint size = sizeof(L3_Array) / sizeof(L3_Array[0]);
			for (uint i = 0; i < size; ++i) {
				item.id_ = L3_Array[i];
				send_info.item_vector.push_back(item);
			}
		}

		if (send_info.item_vector.empty()) continue;
		MONGO_OPERATOR->save_operating_offline_mail(role_id, send_info);
	}
}

//void DB_Operator::db_repair_20150311_handle_expedition_repair(void) {
//	Role_Hash_Set gang_ids;
//	Expedition_Manager_Detail::Int64_Gang_Data_Info_Map gang_data_info_map;
//	Inter_ExpeditionManagerData_Map map;
//
//	{
//		unsigned long long row_count = MONGO_CONNECTION.count( Gang_Fields::COLLECTION );
//		BSONObjBuilder field_builder;
//		field_builder 	<< Gang_Fields::GANG_ID << 1
//						<< Gang_Fields::STATUS << 1
//						;
//		BSONObj field_bson = field_builder.obj();
//		vector<BSONObj> iter_record;
//		iter_record.reserve( row_count );
//
//		MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, QUERY(Gang_Fields::STATUS << Gang_DB_View::GANG_STAT_NORMAL), row_count, 0, &field_bson);
//		//MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, mongo::Query(), row_count, 0, &field_bson);
//	//	MONGO_CONNECTION.findN(iter_record, Gang_Fields::COLLECTION, mongo::Query(), row_count);
//		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
//			BSONObj& obj = *iter;
//			int64_t gang_id = obj[Gang_Fields::GANG_ID].numberLong();
//			int status	= obj[Gang_Fields::STATUS].numberInt();
//			if (gang_id == 0 || status != Gang_DB_View::GANG_STAT_NORMAL) {
//				continue;
//			}
//			gang_ids.insert(gang_id);
//		}
//		if (gang_ids.empty()) {
//			return;
//		}
//	}
//
//	{
//		BSONObj res = MONGO_CONNECTION.findOne(Expedition_Manager_Detail_Fields::COLLECTION,
//						QUERY(Role_Fields::ROLE_ID << (long long int)0));
//		if (!res.isEmpty()) {
//			Gang_Data_Info gdi;
//			BSONObjIterator iter_set(res.getObjectField(Expedition_Manager_Detail_Fields::GANG_DATA_INFO_MAP.c_str()));
//			while (iter_set.more()) {
//				gdi.reset();
//				BSONObj obj = iter_set.next().embeddedObject();
//				int64_t gang_id = obj[Gang_Fields::GANG_ID].numberLong();
//				gdi.line = obj[Expedition_Manager_Detail_Fields::Gang_Data_Info_Map::LINE].numberInt();
//				gdi.coord.type = obj[Role_Fields::Location::COORD_TYPE].numberInt();
//				gdi.coord.x = obj[Role_Fields::Location::COORD_X].numberInt();
//				gdi.coord.y = obj[Role_Fields::Location::COORD_Y].numberInt();
//				gang_data_info_map.insert(std::make_pair(gang_id, gdi));
//			}
//		}
//	}
//
//	{
//		unsigned long long row_count = MONGO_CONNECTION.count( Expedition_Manager_Data_Fields::COLLECTION );
//
//		vector<BSONObj> iter_record;
//		iter_record.reserve( row_count );
//
//		Expedition_Manager_Data emd;
//		MONGO_CONNECTION.findN(iter_record, Expedition_Manager_Data_Fields::COLLECTION, mongo::Query(), row_count);
//		for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
//			BSONObj& obj = *iter;
//			emd.reset();
//			emd.inter_ = obj[Expedition_Manager_Data_Fields::INTER].numberInt();
//			{
//				BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_POINT_ID.c_str()));
//				while (iter_set.more()) {
//					BSONObj obj = iter_set.next().embeddedObject();
//					int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
//					int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
//					emd.gang_id_point_id_.insert(std::make_pair(gang_id, point_id));
//				}
//			}
//			{
//				BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_CUR_LINE.c_str()));
//				while (iter_set.more()) {
//					BSONObj obj = iter_set.next().embeddedObject();
//					int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
//					int cur_line = obj[Expedition_Manager_Data_Fields::CUR_LINE].numberInt();
//					emd.gang_id_cur_line_.insert(std::make_pair(gang_id, cur_line));
//				}
//			}
////			{
////				BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::UUID_GANG_ID_POINT_ID.c_str()));
////				while (iter_set.more()) {
////					BSONObj obj = iter_set.next().embeddedObject();
////					int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
////					BSONObjIterator iter_set_2(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_POINT_ID.c_str()));
////					while (iter_set_2.more()) {
////						BSONObj obj = iter_set_2.next().embeddedObject();
////						int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
////						int point_id = obj[Expedition_Manager_Data_Fields::POINT_ID].numberInt();
////						emd.uuid_gang_id_point_id_[uuid].insert(std::make_pair(gang_id, point_id));
////					}
////				}
////			}
////			{
////				BSONObjIterator iter_set(obj.getObjectField(Expedition_Manager_Data_Fields::UUID_GANG_ID_CUR_LINE.c_str()));
////				while (iter_set.more()) {
////					BSONObj obj = iter_set.next().embeddedObject();
////					int64_t uuid = obj[Expedition_Global_Manager_Data_Fields::UUID].numberLong();
////					BSONObjIterator iter_set_2(obj.getObjectField(Expedition_Manager_Data_Fields::GANG_ID_CUR_LINE.c_str()));
////					while (iter_set_2.more()) {
////						BSONObj obj = iter_set_2.next().embeddedObject();
////						int64_t gang_id = obj[Role_Fields::GANG_ID].numberLong();
////						int cur_line = obj[Expedition_Manager_Data_Fields::CUR_LINE].numberInt();
////						emd.uuid_gang_id_cur_line_[uuid].insert(std::make_pair(gang_id, cur_line));
////					}
////				}
////			}
//			map.insert(std::make_pair(emd.inter_, emd));
//		}
//		if (map.empty()) {
//			return ;
//		}
//	}
//
//	Inter_ExpeditionManagerData_Map::iterator it = map.find(CONFIG_CACHE->server_maintainer_cache().game_inter_flag);
//	if (it == map.end()) {
//		it = map.find(0);
//	}
//	if (it != map.end()) {
//		Gang_Data_Info gdi;
//		for (Role_Hash_Set::const_iterator it_gang = gang_ids.begin();
//				it_gang != gang_ids.end(); ++it_gang) {
//			Expedition_Manager_Detail::Int64_Gang_Data_Info_Map::const_iterator it_gdim = gang_data_info_map.find(*it_gang);
//			if (it_gdim != gang_data_info_map.end()) {
//				continue;
//			}
//			gdi.reset();
//			Role_Int_Map::iterator it_gicl = it->second.gang_id_cur_line_.find(*it_gang);
//			if (it_gicl != it->second.gang_id_cur_line_.end()) {
//				gdi.line = it_gicl->second;
//			}
//			Role_UInt_Map::iterator it_gipi = it->second.gang_id_point_id_.find(*it_gang);
//			if (it_gipi != it->second.gang_id_point_id_.end()) {
//				const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(it_gipi->second);
//				if (empc) {
//					gdi.coord.type = Coord::PIXEL_TYPE;
//					gdi.coord.x = empc->x;
//					gdi.coord.y = empc->y;
//				}
//			}
//			gang_data_info_map.insert(std::make_pair(*it_gang, gdi));
//		}
//	}
//	{
//		std::vector<BSONObj> gdim;
//		for (Expedition_Manager_Detail::Int64_Gang_Data_Info_Map::const_iterator it = gang_data_info_map.begin();
//				it != gang_data_info_map.end(); ++it) {
//			gdim.push_back(BSON(Gang_Fields::GANG_ID << (long long int)it->first
//					<< Expedition_Manager_Detail_Fields::Gang_Data_Info_Map::LINE << it->second.line
//					<< Role_Fields::Location::COORD_TYPE << it->second.coord.type
//					<< Role_Fields::Location::COORD_X << it->second.coord.x
//					<< Role_Fields::Location::COORD_Y << it->second.coord.y));
//		}
//		BSONObjBuilder builder;
//		builder << Expedition_Manager_Detail_Fields::GANG_DATA_INFO_MAP << gdim;
//		MONGO_CONNECTION.update(Expedition_Manager_Detail_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)0),
//					BSON("$set" << builder.obj()), true);
//	}
//}

const static int hero_fix_data[][4] = {
//		{60210021, 1, 1},// 琳娜
//		{60230004, 1, 1},// 凯莱布丽安
		{60210002, 2, 1, 20},// 玛拉凯斯
		{60230003, 3, 1, 20},// 莫拉
		{60220002, 4, 2, 30},// 海尔辛
		{60230010, 5, 2, 30},// 波耶西亚
		{60210003, 6, 2, 30},// 巴尔
		{60230001, 7, 3, 40},// 谢尔格拉
		{60220003, 8, 3, 40},// 大衮
		{60230002, 9, 3, 40},// 瓦尔迷娜
		{60210004, 10, 4, 50},//血腥
		{60210001, 10, 2, 30},//美瑞蒂亚
		{60220001, 10, 2, 30},// 维尔
		{60230006, 11, 4, 50},// 魄伊特
		{60220006, 12, 4, 50},// 梅法拉
		{60220004, 5, 2, 30} //娜米拉
		};

struct Hero_Repair_Data {
	void reset(void) {
		id = 0;
		exploit_lvl = 0;
		quality = 0;
		lv = 0;
	}
	int id;
	int exploit_lvl;
	int quality;
	int lv;
};
typedef boost::unordered_map<int, Hero_Repair_Data> Hero_Repair_Map;
void DB_Operator::db_repair_20150321_handle_hero_and_feats(void) {

	Hero_Repair_Map repair_map;
	int size_1 = sizeof(hero_fix_data) / sizeof(hero_fix_data[0]);
	Hero_Repair_Data data;
	for (int i=0; i < size_1; ++i) {
		data.reset();
		for (int j=0; j < 4; ++j) {
			if (j == 0) data.id = hero_fix_data[i][j];
			else if(j == 1) data.exploit_lvl = hero_fix_data[i][j];
			else if(j == 2) data.quality = hero_fix_data[i][j];
			else if(j == 3) data.lv = hero_fix_data[i][j];
		}
		repair_map[data.id] = data;
	}

	unsigned long long data_count = MONGO_CONNECTION.count("mmo.public");
	BSONObjBuilder field_builder;
	field_builder << "role_id" << 1 << "exploit_lvl" << 1;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(data_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.public", mongo::Query(), data_count, 0, &field_bson);
	Heroer_Detail detail;
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {

		role_id_t role_id = (*it)["role_id"].numberLong();
		int exploit_lvl = (*it)["exploit_lvl"].numberInt();
		int max_lv = exploit_lvl;
		detail.role_id = role_id;
		load_heroer_detail(detail);

		for (std::map<int, Hero_Info_Detail>::iterator hero_it = detail.hero_map.begin();
				hero_it != detail.hero_map.end(); ++hero_it) {

			Hero_Repair_Map::const_iterator find_it = repair_map.find(hero_it->second.hero_id);
			if(repair_map.end() == find_it) continue;

			if (find_it->second.exploit_lvl > max_lv)
				max_lv = find_it->second.exploit_lvl;
			if (find_it->second.lv > hero_it->second.level) {
				hero_it->second.level = find_it->second.lv;
				detail.detail_change();
			}
		}

		if (detail.is_change) {
			save_heroer_detail(detail, false);
		}
		if (max_lv > exploit_lvl) {
			BSONObjBuilder saveBuilder;
			saveBuilder << "exploit_lvl" << max_lv << "exploit_val" << 0;
			MONGO_CONNECTION.update("mmo.public", QUERY("role_id" << (long long int)role_id),
					BSON("$set" << saveBuilder.obj()), true);
		}
		detail.reset();
	}
}


void DB_Operator::db_repair_20150328_handle_hero_and_feats(void) {

	Hero_Repair_Map repair_map;
	int size_1 = sizeof(hero_fix_data) / sizeof(hero_fix_data[0]);
	Hero_Repair_Data data;
	for (int i=0; i < size_1; ++i) {
		data.reset();
		for (int j=0; j < 4; ++j) {
			if (j == 0) data.id = hero_fix_data[i][j];
			else if(j == 1) data.exploit_lvl = hero_fix_data[i][j];
			else if(j == 2) data.quality = hero_fix_data[i][j];
			else if(j == 3) data.lv = hero_fix_data[i][j];
		}
		repair_map[data.id] = data;
	}

	const std::map<int, int> exploit_map = CONFIG_CACHE_PUBLIC->cfg_exploit_lvl_map();

	unsigned long long data_count = MONGO_CONNECTION.count("mmo.public");
	BSONObjBuilder field_builder;
	field_builder << "role_id" << 1 << "exploit_lvl" << 1 << "exploit_val" << 1;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(data_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.public", mongo::Query(), data_count, 0, &field_bson);
	Heroer_Detail detail;
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {

		role_id_t role_id = (*it)["role_id"].numberLong();
		int exploit_lvl = (*it)["exploit_lvl"].numberInt();
		int exploit_val = (*it)["exploit_val"].numberInt();
//		int max_lv = exploit_lvl;
		detail.role_id = role_id;
		load_heroer_detail(detail);

		for (std::map<int, Hero_Info_Detail>::iterator hero_it = detail.hero_map.begin();
				hero_it != detail.hero_map.end(); ++hero_it) {

			Hero_Repair_Map::const_iterator find_it = repair_map.find(hero_it->second.hero_id);
			if(repair_map.end() == find_it) continue;

//			if (find_it->second.exploit_lvl > max_lv)
//				max_lv = find_it->second.exploit_lvl;
//			if (find_it->second.lv > hero_it->second.level) {
//				hero_it->second.level = find_it->second.lv;
//				detail.detail_change();
//			}
			if (find_it->second.quality > hero_it->second.awake_lvl) {
				hero_it->second.awake_lvl = find_it->second.quality;
				detail.detail_change();
			}
		}

		if (detail.is_change) {
			save_heroer_detail(detail, false);
		}
		detail.reset();

		std::map<int, int>::const_iterator find_it = exploit_map.find(exploit_lvl);
		if (find_it == exploit_map.end()) continue;
		exploit_val += find_it->second;
		BSONObjBuilder saveBuilder;
		saveBuilder << "exploit_val" << exploit_val;
		MONGO_CONNECTION.update("mmo.public", QUERY("role_id" << (long long int)role_id),
				BSON("$set" << saveBuilder.obj()), true);
	}
}

void DB_Operator::db_repair_20150321_handle_fashion(void) {

	unsigned long long res_count = MONGO_CONNECTION.count("mmo.fashion");
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.fashion", mongo::Query(), res_count);
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {
		int ttl = 0;
		role_id_t role_id = (*it)["role_id"].numberLong();
		int count = ((*it)["fashion_item"]).Array().size();
		ttl = 499 * count;
		if (ttl <= 0) continue;

		Mail_Send_Info send_info;

		const BSONObj& fashions_obj = (*it).getObjectField("fashion_item");
		BSONObjIterator iter(fashions_obj);
		while (iter.more()) {
			const BSONObj &obj = iter.next().embeddedObject();
			int id = obj["id"].numberInt();
			send_info.item_vector.push_back(Item_Detail(id, 1, Item_Detail::BIND));
		}

		if (send_info.item_vector.empty()) continue;
		send_info.sender_id = 0;
		send_info.sender_name = "系统";
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.receiver_id = role_id;
		send_info.title = "时装补偿";
		send_info.content = "尊敬的玩家,由于时装系统的修改,会清空过去已有的所有时装, 作为补偿,每件时装将返还新的时装道具至背包, 敬请留意";
		send_info.show_send_time = Time_Value::gettimeofday();
		MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);
	}
}

const static int fix_skill_data[][7][68] = {
		{
				{15,20,22411012,22412012,22411022,22412022,22411062,22411082,22411013,22412032,22412082,22412013,22412092},
				{21,25,22411012,22412012,22411022,22412022,22411062,22411082,22411013,22412032,22412082,22412013,22412092,22411032,22412052,22411092,22411014,22412014,22411023,22412023},
				{26,30,22411012,22412012,22411022,22412022,22411062,22411082,22411013,22412032,22412082,22412013,22412092,22411032,22412052,22411092,22411014,22412014,22411023,22412023,22412093,22411102,22411202,22411302,22411063,22411015,22412102,22412202,22412302,22412033,22412015},
				{31,35,22411012,22412012,22411022,22412022,22411062,22411082,22411013,22412032,22412082,22412013,22412092,22411032,22412052,22411092,22411014,22412014,22411023,22412023,22412093,22411102,22411202,22411302,22411063,22411015,22412102,22412202,22412302,22412033,22412015,22411024,22412024,22411033,22412053},
				{36,40,22411012,22412012,22411022,22412022,22411062,22411082,22411013,22412032,22412082,22412013,22412092,22411032,22412052,22411092,22411014,22412014,22411023,22412023,22412093,22411102,22411202,22411302,22411063,22411015,22412102,22412202,22412302,22412033,22412015,22411024,22412024,22411033,22412053,22411093,22411064,22412034,22411103,22411203,22411303,22411034,22411025,22412103,22412203,22412303,22412054,22412025},
				{41,45,22411012,22412012,22411022,22412022,22411062,22411082,22411013,22412032,22412082,22412013,22412092,22411032,22412052,22411092,22411014,22412014,22411023,22412023,22412093,22411102,22411202,22411302,22411063,22411015,22412102,22412202,22412302,22412033,22412015,22411024,22412024,22411033,22412053,22411093,22411064,22412034,22411103,22411203,22411303,22411034,22411025,22412103,22412203,22412303,22412054,22412025,22411065,22412035},
				{46,99,22411012,22412012,22411022,22412022,22411062,22411082,22411013,22412032,22412082,22412013,22412092,22411032,22412052,22411092,22411014,22412014,22411023,22412023,22412093,22411102,22411202,22411302,22411063,22411015,22412102,22412202,22412302,22412033,22412015,22411024,22412024,22411033,22412053,22411093,22411064,22412034,22411103,22411203,22411303,22411034,22411025,22412103,22412203,22412303,22412054,22412025,22411065,22412035,22411035,22412055}
		},
		{
				{15,20,22421012,22422012,22421022,22422022,22421052,22421082,22421013,22422052,22422082,22422013,22422092},
				{21,25,22421012,22422012,22421022,22422022,22421052,22421082,22421013,22422052,22422082,22422013,22422092,22421032,22422032,22421092,22421014,22422014,22421023,22422023},
				{26,30,22421012,22422012,22421022,22422022,22421052,22421082,22421013,22422052,22422082,22422013,22422092,22421032,22422032,22421092,22421014,22422014,22421023,22422023,22422093,22421102,22421202,22421302,22421053,22421015,22422102,22422202,22422302,22422053,22422015},
				{31,35,22421012,22422012,22421022,22422022,22421052,22421082,22421013,22422052,22422082,22422013,22422092,22421032,22422032,22421092,22421014,22422014,22421023,22422023,22422093,22421102,22421202,22421302,22421053,22421015,22422102,22422202,22422302,22422053,22422015,22421024,22422024,22421033,22422033},
				{36,40,22421012,22422012,22421022,22422022,22421052,22421082,22421013,22422052,22422082,22422013,22422092,22421032,22422032,22421092,22421014,22422014,22421023,22422023,22422093,22421102,22421202,22421302,22421053,22421015,22422102,22422202,22422302,22422053,22422015,22421024,22422024,22421033,22422033,22421093,22421054,22422054,22421103,22421203,22421303,22421034,22421025,22422103,22422203,22422303,22422034,22422025},
				{41,45,22421012,22422012,22421022,22422022,22421052,22421082,22421013,22422052,22422082,22422013,22422092,22421032,22422032,22421092,22421014,22422014,22421023,22422023,22422093,22421102,22421202,22421302,22421053,22421015,22422102,22422202,22422302,22422053,22422015,22421024,22422024,22421033,22422033,22421093,22421054,22422054,22421103,22421203,22421303,22421034,22421025,22422103,22422203,22422303,22422034,22422025,22421055,22422055},
				{46,99,22421012,22422012,22421022,22422022,22421052,22421082,22421013,22422052,22422082,22422013,22422092,22421032,22422032,22421092,22421014,22422014,22421023,22422023,22422093,22421102,22421202,22421302,22421053,22421015,22422102,22422202,22422302,22422053,22422015,22421024,22422024,22421033,22422033,22421093,22421054,22422054,22421103,22421203,22421303,22421034,22421025,22422103,22422203,22422303,22422034,22422025,22421055,22422055,22421035,22422035}
		},
		{
				{15,20,22431012,22432012,22431022,22432022,22431052,22431082,22431013,22432082,22432013,22432042,22432092},
				{21,25,22431012,22432012,22431022,22432022,22431052,22431082,22431013,22432082,22432013,22432042,22432092,22431042,22432051,22431092,22431014,22432014,22431023,22432023},
				{26,30,22431012,22432012,22431022,22432022,22431052,22431082,22431013,22432082,22432013,22432042,22432092,22431042,22432051,22431092,22431014,22432014,22431023,22432023,22432093,22431102,22431202,22431302,22431053,22431015,22432055,22432202,22432302,22432015,22432043},
				{31,35,22431012,22432012,22431022,22432022,22431052,22431082,22431013,22432082,22432013,22432042,22432092,22431042,22432051,22431092,22431014,22432014,22431023,22432023,22432093,22431102,22431202,22431302,22431053,22431015,22432055,22432202,22432302,22432015,22432043,22431024,22432024,22431043,22432052},
				{36,40,22431012,22432012,22431022,22432022,22431052,22431082,22431013,22432082,22432013,22432042,22432092,22431042,22432051,22431092,22431014,22432014,22431023,22432023,22432093,22431102,22431202,22431302,22431053,22431015,22432055,22432202,22432302,22432015,22432043,22431024,22432024,22431043,22432052,22431093,22431054,22432044,22431103,22431203,22431303,22431044,22431025,22432103,22432203,22432303,22432053,22432025},
				{41,45,22431012,22432012,22431022,22432022,22431052,22431082,22431013,22432082,22432013,22432042,22432092,22431042,22432051,22431092,22431014,22432014,22431023,22432023,22432093,22431102,22431202,22431302,22431053,22431015,22432055,22432202,22432302,22432015,22432043,22431024,22432024,22431043,22432052,22431093,22431054,22432044,22431103,22431203,22431303,22431044,22431025,22432103,22432203,22432303,22432053,22432025,22431055,22432045},
				{46,99,22431012,22432012,22431022,22432022,22431052,22431082,22431013,22432082,22432013,22432042,22432092,22431042,22432051,22431092,22431014,22432014,22431023,22432023,22432093,22431102,22431202,22431302,22431053,22431015,22432055,22432202,22432302,22432015,22432043,22431024,22432024,22431043,22432052,22431093,22431054,22432044,22431103,22431203,22431303,22431044,22431025,22432103,22432203,22432303,22432053,22432025,22431055,22432045,22432054,22431045}
		}
};

struct Skill_Repair_Data {
	void reset(void) {
		min_lv = 0;
		max_lv = 0;
		item_id_vec.clear();
	}
	int min_lv;
	int max_lv;
	Int_Vec item_id_vec;
};
typedef boost::unordered_map<int, std::vector<Skill_Repair_Data> > Skill_Repair_Map;
void DB_Operator::db_repair_20150321_handle_skill(void) {

	Skill_Repair_Map data_map;
	Skill_Repair_Data data;
	for (int i=0; i<3; ++i) {
		data_map[i+1] = std::vector<Skill_Repair_Data>();
		for (int j=0; j<7; ++j) {
			data.reset();
			for (int k=0; k<68; ++k) {
				int val = fix_skill_data[i][j][k];
				if (val == 0) continue;
				if (k == 0) {
					data.min_lv = val;
				} else if (k == 1) {
					data.max_lv = val;
				} else {
					data.item_id_vec.push_back(val);
				}
			}
			data_map[i+1].push_back(data);
		}
	}

	unsigned long long res_count = MONGO_CONNECTION.count("mmo.role");
	BSONObjBuilder field_builder;
	field_builder << "role_id" << 1 << "career" << 1 << "level" << 1;
	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, "mmo.role", mongo::Query(), res_count, 0, &field_bson);
	Fighter_Detail detail;
	Skill_DB_Info skill;
	const Skill_Cache_Map& skill_map = CONFIG_CACHE_FIGHTER->skill_cache_map();
	for (std::vector<BSONObj>::iterator it = iter_record.begin(); it != iter_record.end(); ++it) {

		role_id_t role_id = (*it)["role_id"].numberLong();
		int career = (*it)["career"].numberInt();
		int lv = (*it)["level"].numberInt();

		detail.role_id = role_id;
		load_fighter_detail(detail);

		for (Skill_Cache_Map::const_iterator skill_it = skill_map.begin();
				skill_it != skill_map.end(); ++skill_it) {

			if (skill_it->second.skill_career != career) continue;
			Skill_Detail_Map::const_iterator find_cfg_it = skill_it->second.skill_level_map.find(1);
			if (find_cfg_it == skill_it->second.skill_level_map.end()) continue;
			if (find_cfg_it->second.lv_lim > lv) continue;

			Talent_Map::iterator find_it = detail.talent_map.find(skill_it->second.skill_series);
			if (find_it == detail.talent_map.end()) continue;

			Skill_DB_Info_Map::iterator find_skill_it = find_it->second.skill_db_info_map.find(
					skill_it->second.skill_id);
			if (find_skill_it != find_it->second.skill_db_info_map.end()) {
				find_skill_it->second.skill_level = 1;
				find_skill_it->second.skill_loc = 0;
			} else {
				skill.reset();
				skill.skill_id =  skill_it->second.skill_id;
				skill.skill_level = 1;
				skill.skill_loc = 0;
				find_it->second.skill_db_info_map[skill.skill_id] = skill;
			}
			detail.detail_change();
		}

		if (detail.is_change) {
			save_fighter_detail(detail, false);
		}
		detail.reset();

		// send mail
		Skill_Repair_Map::const_iterator find_it = data_map.find(career);
		if (find_it == data_map.end()) continue;

		for (std::vector<Skill_Repair_Data>::const_iterator repair_it = find_it->second.begin();
				repair_it != find_it->second.end(); ++repair_it) {
			if (repair_it->min_lv > lv || repair_it->max_lv < lv) continue;

			Mail_Send_Info send_info;
			send_info.sender_id = 0;
			send_info.sender_name = "系统";
			send_info.send_type = MAIL_TYPE_SYS;
			send_info.show_send_time = Time_Value::gettimeofday();
			send_info.receiver_id = role_id;
			send_info.title = "技能补偿";
			send_info.content = "亲爱的玩家,本版本技能系统进行一定优化.作为补偿,将根据等级段补偿相应的技能书和资源, 敬请留意";
			for (Int_Vec::const_iterator id_it = repair_it->item_id_vec.begin();
					id_it != repair_it->item_id_vec.end(); ++id_it) {
				send_info.item_vector.push_back(Item_Detail(*id_it, 1, Item_Detail::BIND));
			}
			MONGO_OPERATOR->save_operating_offline_mail(send_info.receiver_id, send_info);
		}
	}
}
