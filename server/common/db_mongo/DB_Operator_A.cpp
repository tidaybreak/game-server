/*
 * DB_Operator_B.cpp
 *
 *  Created on: 2014年9月11日
 *      Author: Linqiyou
 */

#include "DB_Operator.h"
#include "DB_Fields.h"
#include "Configurator.h"
#include "Config_Cache.h"
#include "Config_Cache_Task.h"
#include "Config_Cache_Dungeon.h"
#include "Config_Cache_Scene.h"
#include "Record_Client.h"
#include "Pool_Manager.h"
#include "Server_Struct.h"
#include "Logic_Player_Detail.h"
#include "Logic_Player_Struct.h"
#include "Config_Cache_Role.h"
#include "mongo/client/dbclient.h"
#include "gang/Chat_Gang_Struct.h"
#include "card/Card_Func.h"
#include "DB_Def.h"
#include <boost/regex.hpp>

using namespace mongo;

#include "DB_Manager.h"


int DB_Operator::update_db_cross_info(bool time_out) {
	BSONObj obj = MONGO_CONNECTION.findOne("backstage.cross_area", mongo::Query());
	if (! obj.isEmpty() && obj["cross_flag"].isNumber()) {
		int cross = obj["cross_flag"].numberInt();
		if (cross >= 0 && cross != CONFIG_CACHE->server_maintainer_cache().game_inter_flag) {
			if (time_out && cross == last_cross_inter_) return 0;

			LOG_DEBUG("inter server cross:%d", cross);

			{
				FILE *fp = fopen("../server_global_conf/server_inter.json", "w");
				if (! fp) {
					LOG_SYS("fopen");
					return 0;
				}
				char pid_str[128] = {0};
				snprintf(pid_str, sizeof(pid_str), "{\"inter_area\" : %d, \"now_area\" : %d, \"time\" : %ld}", cross,
						CONFIG_CACHE->server_maintainer_cache().game_inter_flag, Time_Value::gettimeofday().sec());
				fputs(pid_str, fp);
				fclose(fp);
			}

			// 如果还没开服就改开服方案
			if (CONFIG_CACHE->server_maintainer_cache().game_open_time > Time_Value::gettimeofday()) {
				CONFIG_CACHE->set_cross_info(cross);
				change_solution_id(CONFIG_CACHE->server_maintainer_cache().default_solution_id);
				return 1;
			}

			last_cross_inter_ = cross;
		}
	}

	return 0;
}

int DB_Operator::drop_mmo_database(void) {
	unsigned long long res_count = MONGO_CONNECTION.count(Role_Fields::COLLECTION);
	LOG_DEBUG("total:%d", res_count);
	if (res_count < 100) {
		MONGO_CONNECTION.dropDatabase("mmo");
	}
	return 0;
}

int DB_Operator::change_solution_id(int solution_id) {
	int fd = 0;
	if ((fd = open("../server_global_conf/server_maintainer.json", O_RDONLY)) < 0) {
		std::cout << "file not exist" << std::endl;
		return -1;
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		perror("fstat"), exit(1);

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		return -1;
	}

	// utf8 bom
	int begin_off = 0;
	if (statbuf.st_size >= 3) {
		if (*((char *)src) == -17 && *((char *)src + 1) == -69 && *((char *)src + 2) == -65) {
			begin_off = 3;
		}
	}

	std::string json_text;
	json_text.assign(static_cast<const char *>(src) + begin_off, static_cast<const char *>(src) + statbuf.st_size);

	if (munmap(src, statbuf.st_size) < 0) {
		return -1;
	}

	close(fd);

	std::stringstream time_s;
	time_s << "$1\"game_solution_id\": ";
	time_s << solution_id;
	time_s << ",$3";

	//问号(?)表示之前的可以选择
	std::string phone = "\\G(.*)?\"game_solution_id\"([0-9:\\s]*)?,(.*)?";
	boost::regex r(phone);
	//代替
	std::string fmt(time_s.str());
	std::string result = regex_replace(json_text, r, fmt);
	std::cout << result << std::endl;

	FILE *fp = fopen("../server_global_conf/server_maintainer.json", "w");
	if (! fp) {
		return -1;
	}
	fputs(result.c_str(), fp);
	fclose(fp);

	return 0;
}

int DB_Operator::load_operating_detail(Operating_Detail &detail) {
	//读取后台充值表,处理防止PHP通知失败没充值记录
	BSONObj pay_query_obj = BSON(Pay_Fields::ROLE_ID << (long long int)detail.role_id << Pay_Fields::IN_ACCOUNT << false);
	unsigned long long pay_res_count = MONGO_CONNECTION.count(Pay_Fields::COLLECTION, pay_query_obj);
	if(pay_res_count) {
		BSONObjBuilder pay_field_builder;
		pay_field_builder 	<< Pay_Fields::ROLE_ID << 1
							<< Pay_Fields::PAYTICKET << 1
							<< Pay_Fields::TIME << 1
							<< Pay_Fields::ORDER_ID << 1
							<< Pay_Fields::IN_ACCOUNT << 1;
		BSONObj pay_field_bson = pay_field_builder.obj();
		vector<BSONObj> pay_iter_record;
		pay_iter_record.reserve(pay_res_count);
		MONGO_CONNECTION.findN(pay_iter_record, Pay_Fields::COLLECTION, mongo::Query(pay_query_obj), pay_res_count, 0, &pay_field_bson);
		Operating_Recharge value;
		uint32_t count_gold = 0;
		for (vector<BSONObj>::iterator iter = pay_iter_record.begin(); iter != pay_iter_record.end(); ++iter) {
				BSONObj &obj = *iter;
				//判断是否到账
				if(obj[Pay_Fields::IN_ACCOUNT].booleanSafe()) continue;
				value.reset();
				value.gold = obj[Pay_Fields::PAYTICKET].numberInt();
				value.time = obj[Pay_Fields::TIME].numberLong();
				//value.order_id = obj[Pay_Fields::ORDER_ID].numberInt();
				detail.recharge_gold.push_back(value);
				count_gold += value.gold;
		}
		//更新数据库
		BSONObjBuilder builder;
		builder << Pay_Fields::IN_ACCOUNT << true;
		MONGO_CONNECTION.update(Pay_Fields::COLLECTION, mongo::Query(pay_query_obj), BSON("$set" << builder.obj() ), false, true);

		MSG_USER("info:%s, 补单数量:%d, 补单总钻石数:%d, role_id:%ld", "玩家离线充值需要补单", pay_res_count, count_gold, detail.role_id);
	}



	// 读取离线操作
	BSONObj res = MONGO_CONNECTION.findOne(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty()) {
		detail.role_id = res[Operating_Fields::ROLE_ID].numberLong();
		//读取离线好友请求
		if (res.hasField(Operating_Fields::ADD_F_INVITE_LIST.c_str())) {
			BSONObjIterator invite_iter(res.getObjectField(Operating_Fields::ADD_F_INVITE_LIST.c_str()));
			Time_Value now;
			while (invite_iter.more()) {
				BSONElement invite_obj = invite_iter.next();
				now.sec(invite_obj[Operating_Fields::Friend_List::TIME].numberLong());
				detail.invite_list.insert(std::make_pair(invite_obj[Operating_Fields::Friend_List::ROLE_ID].numberLong(),now));
			}
		}
		//读取离线好友处理
		if (res.hasField(Operating_Fields::HANDEL_F_APPLY_LIST.c_str())) {
			BSONObjIterator apply_iter(res.getObjectField(Operating_Fields::HANDEL_F_APPLY_LIST.c_str()));
			while (apply_iter.more()) {
				BSONElement apply_obj = apply_iter.next();
				detail.apply_list.insert(std::make_pair(apply_obj[Operating_Fields::Friend_List::ROLE_ID].numberLong(),apply_obj[Operating_Fields::Friend_List::HANDEL].numberInt()));
			}
		}

		//离线组队竞技奖励
    	if (res.hasField("team_award")) {
			BSONObjIterator team_iter(res.getObjectField("team_award"));
			while (team_iter.more()) {
				BSONObj team_obj = team_iter.next().embeddedObject();
				int id = team_obj["id"].numberLong();
				detail.team_award.push_back(id);
			}
    	}

		//读取离线MAIL信息
    	if (res.hasField(Operating_Fields::MAIL_LIST.c_str())) {
			BSONObjIterator mail_iter(res.getObjectField(Operating_Fields::MAIL_LIST.c_str()));
			Mail_Send_Info send_info;
			send_info.reset();
			while (mail_iter.more()) {
				//BSONElement mail_obj = mail_iter.next();
				BSONObj mail_obj = mail_iter.next().embeddedObject();
				send_info.receiver_id = mail_obj[Operating_Fields::Mail_List::RECEIVER_ID].numberLong();
				send_info.sender_id = mail_obj[Operating_Fields::Mail_List::SENDER_ID].numberLong();
				send_info.send_type = mail_obj[Operating_Fields::Mail_List::SEND_TYPE].numberInt();
				send_info.gold = mail_obj[Operating_Fields::Mail_List::GOLD].numberInt();
				send_info.sender_name = mail_obj[Operating_Fields::Mail_List::SENDER_NAME].valuestrsafe();
				send_info.title = mail_obj[Operating_Fields::Mail_List::TITLE].valuestrsafe();
				send_info.content = mail_obj[Operating_Fields::Mail_List::CONTENT].valuestrsafe();
				send_info.show_send_time.sec(mail_obj[Operating_Fields::Mail_List::SHOW_SEND_TIME].numberInt());

				send_info.item_vector.clear();
				//加载附件信息
				BSONObjIterator item_iter(mail_obj.getObjectField(Mail_Fields::Mail_List::ITEM.c_str()));
				while (item_iter.more()) {
					BSONObj item_obj = item_iter.next().embeddedObject();
					Item_Detail item;
					int result = load_item_detail(item_obj, item);
					if (result != 0)    continue;

					send_info.item_vector.push_back(item);
				}

				detail.mail_list.push_back(send_info);
			}
    	}
    	// 公会信息
    	if (res.hasField(Operating_Fields::GANGER_INFO.c_str())) {
    		BSONObjIterator ganer_iter = res.getObjectField(Operating_Fields::GANGER_INFO.c_str());
    		while ( ganer_iter.more() ) {
    			BSONElement ganger_obj = ganer_iter.next();
    			Ganger_Detail ganger_detail;
    			ganger_detail.gang_id = ganger_obj[Operating_Fields::Ganger_Info::GANG_ID].numberLong();
    			ganger_detail.last_gang_id = ganger_obj[Operating_Fields::Ganger_Info::LAST_GANG_ID].numberLong();
				ganger_detail.day_contribute = ganger_obj[Operating_Fields::Ganger_Info::DAY_CONTRIBUTE].numberInt();
				ganger_detail.contrib_of_week = ganger_obj[Operating_Fields::Ganger_Info::WEEK_CONTRIBUTE].numberInt();
				ganger_detail.day_gold_contrib_times = ganger_obj[Operating_Fields::Ganger_Info::DAY_GOLD_CONTRIB_TIMES].numberInt();
				ganger_detail.day_diam_contrib_times = ganger_obj[Operating_Fields::Ganger_Info::DAY_DIAM_CONTRIB_TIMES].numberInt();
				ganger_detail.authority = ganger_obj[Operating_Fields::Ganger_Info::AUTHORITY].numberInt();
				ganger_detail.join_time.sec( ganger_obj[Operating_Fields::Ganger_Info::JOIN_TIME].numberInt() );
				ganger_detail.leave_time.sec( ganger_obj[Operating_Fields::Ganger_Info::LEAVE_TIME].numberInt() );
				ganger_detail.headship = ganger_obj[Operating_Fields::Ganger_Info::HEADSHIP].numberInt();
	    		ganger_detail.gang_war_award_get_times_by_day = res[Ganger_Fields::GANG_WAR_AWARD_GET_TIMES_BY_DAY].numberInt();

				detail.ganger_info.push_back( ganger_detail );
    		}
		}

    	//读取离线arena_three_day_reward信息
      if (res.hasField(Operating_Fields::ARENA_THREE_DAY_REWARD.c_str())) {
    		BSONObjIterator iter(res.getObjectField(Operating_Fields::ARENA_THREE_DAY_REWARD.c_str()));
    		Arena_Reward_Info info;
    		info.reset();
    		while (iter.more()) {
    			BSONObj obj = iter.next().embeddedObject();
    			info.rank = obj[Operating_Fields::Arena_Reward::RANK].numberLong();
    			info.rank_time.sec(obj[Operating_Fields::Arena_Reward::RANK_TIME].numberInt());
    			detail.arena_three_day_reward.push_back(info);
    		}
        }
      	//读取离线arena_seven_day_reward信息
      if (res.hasField(Operating_Fields::ARENA_SEVEN_DAY_REWARD.c_str())) {
    	  BSONObjIterator iter(res.getObjectField(Operating_Fields::ARENA_SEVEN_DAY_REWARD.c_str()));
    	  Arena_Reward_Info info;
    	  info.reset();
    	  while (iter.more()) {
    		  BSONObj obj = iter.next().embeddedObject();
    		  info.rank = obj[Operating_Fields::Arena_Reward::RANK].numberLong();
    		  info.rank_time.sec(obj[Operating_Fields::Arena_Reward::RANK_TIME].numberInt());
    		  detail.arena_seven_day_reward.push_back(info);
    	  }
      	}
      	//读取离线的送花
      	detail.receive_flowers_list.clear();
        if(res.hasField(Operating_Fields::RECEIVE_FLOWERS_LIST.c_str())) {
        	//receive_flowers_list
        	 BSONObjIterator iter(res.getObjectField(Operating_Fields::RECEIVE_FLOWERS_LIST.c_str()));
        	 Flowers_Info info;
        	 while (iter.more()) {
            	 info.reset();
        		 BSONObj obj = iter.next().embeddedObject();
        		 info.role_id = obj[Operating_Fields::Receive_Flowers_List::ROLE_ID].numberLong();
        		 info.role_name = obj[Operating_Fields::Receive_Flowers_List::ROLE_NAME].valuestrsafe();
        		 info.item_id = obj[Operating_Fields::Receive_Flowers_List::ITEM_ID].numberInt();
        		 info.charm = obj[Operating_Fields::Receive_Flowers_List::CHARM].numberInt();
        		 info.friendship = obj[Operating_Fields::Receive_Flowers_List::FRIEND_NUM].numberInt();
        		 info.msg = obj[Operating_Fields::Receive_Flowers_List::MSG].valuestrsafe();
				 detail.receive_flowers_list.push_back(info);
        	 }
        }

        //读取龙谷建筑通知
        detail.building_notice_map.clear();
        if(res.hasField(Operating_Fields::DRAGON_VALE_BUILDING_NOTICE_LIST.c_str())) {
        	 BSONObjIterator iter(res.getObjectField(Operating_Fields::DRAGON_VALE_BUILDING_NOTICE_LIST.c_str()));
        	 Building_Notice notice;
        	 while (iter.more()) {
        		 notice.reset();
        		 BSONObj obj = iter.next().embeddedObject();
        		 uint8_t type = (uint8_t)obj[Operating_Fields::Dragon_Vale_Building_Notice::NOTICE_TYPE].numberInt();
        		 notice.gold_mine = (uint8_t)obj[Operating_Fields::Dragon_Vale_Building_Notice::GOLD_MINE].numberInt();
        		 notice.moon_well = (uint8_t)obj[Operating_Fields::Dragon_Vale_Building_Notice::MOON_WELL].numberInt();
        		 notice.totem = (uint8_t)obj[Operating_Fields::Dragon_Vale_Building_Notice::TOTEM].numberInt();
        		 detail.building_notice_map.insert(std::make_pair(type, notice));
        	 }
        }

        //读取龙谷掠夺战斗结果通知
        detail.rob_fight_result_map.clear();
        if(res.hasField(Operating_Fields::DRAGON_VALE_ROB_FIGHT_RESULT_LIST.c_str())) {
			 BSONObjIterator iter(res.getObjectField(Operating_Fields::DRAGON_VALE_ROB_FIGHT_RESULT_LIST.c_str()));
			 Rob_Fight_Result rob_fight_result;
			 while (iter.more()) {
				rob_fight_result.reset();
				 BSONObj obj = iter.next().embeddedObject();
				 uint8_t type = (uint8_t)obj[Operating_Fields::Dragon_Vale_Rob_Fight_Result::NOTICE_TYPE].numberInt();
				 rob_fight_result.server_name = obj[Operating_Fields::Dragon_Vale_Rob_Fight_Result::SERVER_NAME].str();
				 rob_fight_result.player_name = obj[Operating_Fields::Dragon_Vale_Rob_Fight_Result::PLAYER_NAME].str();
				 rob_fight_result.result = (uint8_t)obj[Operating_Fields::Dragon_Vale_Rob_Fight_Result::RESULT].numberInt();
				 detail.rob_fight_result_map.insert(std::make_pair(type, rob_fight_result));
			 }
        }

        //读取龙谷资源被掠夺离线通知
        detail.gain_rob_production_set.clear();
        if(res.hasField(Operating_Fields::DRAGON_VALE_GAIN_ROB_PRODUCTION_LIST.c_str())) {
			 BSONObjIterator iter(res.getObjectField(Operating_Fields::DRAGON_VALE_GAIN_ROB_PRODUCTION_LIST.c_str()));
			 Gain_Rob_Production gain_rob_production;
			 while (iter.more()) {
				 gain_rob_production.reset();
				 BSONObj obj = iter.next().embeddedObject();
				 gain_rob_production.server_name = obj[Operating_Fields::Dragon_Vale_Gain_Rob_Production::SERVER_NAME].str();
				 gain_rob_production.player_name = obj[Operating_Fields::Dragon_Vale_Gain_Rob_Production::PLAYER_NAME].str();
				 gain_rob_production.gold_mine = obj[Operating_Fields::Dragon_Vale_Gain_Rob_Production::GOLD_MINE].numberInt();
				 gain_rob_production.moon_well = obj[Operating_Fields::Dragon_Vale_Gain_Rob_Production::MOON_WELL].numberInt();
				 gain_rob_production.totem = obj[Operating_Fields::Dragon_Vale_Gain_Rob_Production::TOTEM].numberInt();
				 detail.gain_rob_production_set.insert(gain_rob_production);
			 }
        }

        //龙谷至尊宝离线通知
        detail.is_dragon_vale_special = (res[Operating_Fields::IS_DRAGON_VALE_SPECIAL].numberInt() == 1) ? true : false;
        //离线成功邀请一个玩家
        detail.inv_time = res[Operating_Fields::INV_TIME].numberLong();
        detail.like_success = res[Operating_Fields::LIKE_SUCCESS].numberInt();

        detail.wbrank = res[Operating_Fields::WBRANK].numberInt();
        detail.damage = res[Operating_Fields::DAMAGE].numberInt();
        detail.damage_rate = res[Operating_Fields::DAMAGE_RATE].numberInt();
    	// 取完清除数据
		MONGO_CONNECTION.remove(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)detail.role_id));

	} else {
		return -1;
	}

	return 0;
}

int DB_Operator::save_operating_offline_friend_add(role_id_t role_id, role_id_t other_role_id, int64_t addtime) {
	BSONObjBuilder builder;
	BSONObj invite_obj = BSON( Operating_Fields::Friend_List::ROLE_ID << (long long int)role_id
								<< Operating_Fields::Friend_List::TIME << (long long int)addtime);

	builder.append(Operating_Fields::ADD_F_INVITE_LIST, invite_obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)other_role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_apply_handel(role_id_t role_id, role_id_t other_role_id, int8_t handel) {
	BSONObjBuilder builder;
	BSONObj apply_obj = BSON( Operating_Fields::Friend_List::ROLE_ID << (long long int)role_id
								<< Operating_Fields::Friend_List::HANDEL << (int8_t)handel);

	builder.append(Operating_Fields::HANDEL_F_APPLY_LIST, apply_obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)other_role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_ganger_info(role_id_t role_id, Ganger_Detail& detail) {
	BSONObjBuilder builder;
	BSONObj ganger_obj = BSON( Operating_Fields::Ganger_Info::GANG_ID << (long long int)detail.gang_id
								<< Operating_Fields::Ganger_Info::LAST_GANG_ID << (long long int)detail.last_gang_id
								<< Operating_Fields::Ganger_Info::DAY_CONTRIBUTE << detail.day_contribute
								<< Operating_Fields::Ganger_Info::WEEK_CONTRIBUTE << detail.contrib_of_week
								<< Operating_Fields::Ganger_Info::DAY_GOLD_CONTRIB_TIMES << detail.day_gold_contrib_times
								<< Operating_Fields::Ganger_Info::DAY_DIAM_CONTRIB_TIMES << detail.day_diam_contrib_times
								<< Operating_Fields::Ganger_Info::AUTHORITY << detail.authority
								<< Operating_Fields::Ganger_Info::JOIN_TIME << (unsigned int)detail.join_time.sec()
								<< Operating_Fields::Ganger_Info::LEAVE_TIME << (unsigned int)detail.leave_time.sec()
								<< Operating_Fields::Ganger_Info::HEADSHIP << detail.headship
								<< Ganger_Fields::GANG_WAR_AWARD_GET_TIMES_BY_DAY << detail.gang_war_award_get_times_by_day);

	builder.append(Operating_Fields::GANGER_INFO, ganger_obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_arena_three_day_reward(role_id_t role_id, Arena_Reward_Info &info){
	BSONObjBuilder builder;
	BSONObj obj = BSON( Operating_Fields::Arena_Reward::RANK << (int)info.rank
								<< Operating_Fields::Arena_Reward::RANK_TIME << (long long int)info.rank_time.sec());

	builder.append(Operating_Fields::ARENA_THREE_DAY_REWARD, obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_arena_seven_day_reward(role_id_t role_id, Arena_Reward_Info &info){
	BSONObjBuilder builder;
	BSONObj obj = BSON( Operating_Fields::Arena_Reward::RANK << (int)info.rank
								<< Operating_Fields::Arena_Reward::RANK_TIME << (long long int)info.rank_time.sec());

	builder.append(Operating_Fields::ARENA_SEVEN_DAY_REWARD, obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_receive_flowers(role_id_t role_id, std::string &role_name, uint32_t item_id,  uint32_t charm, uint32_t friend_num, std::string &msg,  role_id_t receive_role_id) {

	BSONObjBuilder builder;
	BSONObj apply_obj = BSON( Operating_Fields::Receive_Flowers_List::ROLE_ID << (long long int)role_id
								<< Operating_Fields::Receive_Flowers_List::ROLE_NAME << role_name
								<< Operating_Fields::Receive_Flowers_List::ITEM_ID << item_id
								<< Operating_Fields::Receive_Flowers_List::CHARM << charm
								<< Operating_Fields::Receive_Flowers_List::FRIEND_NUM << friend_num
								<< Operating_Fields::Receive_Flowers_List::MSG << msg);

	builder.append(Operating_Fields::RECEIVE_FLOWERS_LIST, apply_obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)receive_role_id),
			BSON("$push" << builder.obj() ), true);

	return 0;
}

int DB_Operator::save_operating_offline_dragon_vale_building(role_id_t role_id, uint8_t notice_type, uint8_t gold_mine,
										uint8_t moon_well, uint8_t totem) {
	BSONObjBuilder builder;
	BSONObj notice_obj = BSON(Operating_Fields::Dragon_Vale_Building_Notice::NOTICE_TYPE << notice_type
								<< Operating_Fields::Dragon_Vale_Building_Notice::GOLD_MINE << gold_mine
								<< Operating_Fields::Dragon_Vale_Building_Notice::MOON_WELL << moon_well
								<< Operating_Fields::Dragon_Vale_Building_Notice::TOTEM << totem);

	builder.append(Operating_Fields::DRAGON_VALE_BUILDING_NOTICE_LIST, notice_obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_dragon_vale_rob_fight_result(role_id_t role_id, uint8_t notice_type, std::string server_name,
										std::string player_name, uint8_t result) {
	BSONObjBuilder builder;
	BSONObj notice_obj = BSON(Operating_Fields::Dragon_Vale_Rob_Fight_Result::NOTICE_TYPE << notice_type
								<< Operating_Fields::Dragon_Vale_Rob_Fight_Result::SERVER_NAME << server_name
								<< Operating_Fields::Dragon_Vale_Rob_Fight_Result::PLAYER_NAME << player_name
								<< Operating_Fields::Dragon_Vale_Rob_Fight_Result::RESULT << result);

	builder.append(Operating_Fields::DRAGON_VALE_ROB_FIGHT_RESULT_LIST, notice_obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_dragon_vale_gain_rob_production(role_id_t role_id, std::string server_name,
										std::string player_name, uint32_t gold_mine, uint32_t moon_well,
										uint32_t totem) {
	BSONObjBuilder builder;
	BSONObj notice_obj = BSON(Operating_Fields::Dragon_Vale_Gain_Rob_Production::SERVER_NAME << server_name
								<< Operating_Fields::Dragon_Vale_Gain_Rob_Production::PLAYER_NAME << player_name
								<< Operating_Fields::Dragon_Vale_Gain_Rob_Production::GOLD_MINE << gold_mine
								<< Operating_Fields::Dragon_Vale_Gain_Rob_Production::MOON_WELL << moon_well
								<< Operating_Fields::Dragon_Vale_Gain_Rob_Production::TOTEM << totem);

	builder.append(Operating_Fields::DRAGON_VALE_GAIN_ROB_PRODUCTION_LIST, notice_obj);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_dragon_vale_special(role_id_t role_id, bool is_specail) {
	BSONObjBuilder builder;
	builder << Operating_Fields::IS_DRAGON_VALE_SPECIAL << (is_specail ? 1 : 0);

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_invite_player(role_id_t role_id, int64_t time) {
	BSONObjBuilder builder;
	builder << Operating_Fields::INV_TIME << (long long int)time;

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_like_success(role_id_t role_id) {
	BSONObjBuilder builder;
	builder << Operating_Fields::LIKE_SUCCESS << 1;

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_active_content_listen_world_boss_end_info(role_id_t role_id , uint32_t damage, uint32_t rank, uint32_t damage_rate) {
	BSONObjBuilder builder;
	builder << Operating_Fields::DAMAGE << damage << Operating_Fields::DAMAGE_RATE << damage_rate << Operating_Fields::WBRANK << rank;

	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$set" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_chat_record(role_id_t role_id, int64_t target, uint8_t chat_type, std::vector<Chat_Content>& contents) {
	Time_Value now = Time_Value::gettimeofday();
	BSONObjBuilder builder;
	std::vector<BSONObj> record_vector;
	for(std::vector<Chat_Content>::iterator it = contents.begin(); it != contents.end(); it++) {
		record_vector.push_back( BSON( Chat_Operating_Fields::Record::CHAT_TYPE << chat_type
								<< Chat_Operating_Fields::Record::CONTENT << it->text
								<< Chat_Operating_Fields::Record::TYPE << it->type

								<< Chat_Operating_Fields::Record::ROLE_ROLE_ID << (long long int)it->role_Info.role_id
								<< Chat_Operating_Fields::Record::ROLE_ROLE_NAME << it->role_Info.role_name
								<< Chat_Operating_Fields::Record::ROLE_GENDER << it->role_Info.gender
								<< Chat_Operating_Fields::Record::ROLE_GUILD_NAME << it->role_Info.guild_name
								<< Chat_Operating_Fields::Record::ROLE_CAREER << it->role_Info.career
								<< Chat_Operating_Fields::Record::ROLE_LEVEL << it->role_Info.level
								<< Chat_Operating_Fields::Record::ROLE_VIP_LEVEL << it->role_Info.vip_level
								<< Chat_Operating_Fields::Record::ROLE_COMBAT << it->role_Info.combat

								<< Chat_Operating_Fields::Record::ITEM_ID << it->item.item_base_info.id
								<< Chat_Operating_Fields::Record::ITEM_AMOUNT << it->item.item_base_info.amount
								<< Chat_Operating_Fields::Record::ITEM_BIND << it->item.item_base_info.bind) );

	}
	builder << Chat_Operating_Fields::ROLE_ID << (long long int)role_id
								  <<Chat_Operating_Fields::CHAT_TYPE << chat_type
							      <<Chat_Operating_Fields::TIME << (long long int)now.sec()
								  <<Chat_Operating_Fields::RECORD << record_vector
								  <<Chat_Operating_Fields::TARGET << (long long int)target;

	MONGO_CONNECTION.insert(Chat_Operating_Fields::PRIVATE_CHAT_COLLECTION, builder.obj());
	return 0;
}

int DB_Operator::load_offline_private_chat_record(role_id_t role_id, Time_Value& time, std::vector<Chat_Record>& chat_record) {
	BSONObj query_obj = BSON( Chat_Operating_Fields::TARGET << (long long int)role_id
							<< Chat_Operating_Fields::TIME << BSON("$gte" << (long long int)time.sec() ));
	unsigned long long res_count = MONGO_CONNECTION.count(Chat_Operating_Fields::PRIVATE_CHAT_COLLECTION, query_obj);
	BSONObjBuilder field_builder;
	field_builder 	<< Chat_Operating_Fields::ROLE_ID << 1
					<< Chat_Operating_Fields::CHAT_TYPE << 1
					<< Chat_Operating_Fields::TIME << 1
					<< Chat_Operating_Fields::RECORD << 1
					<< Chat_Operating_Fields::TARGET << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Chat_Operating_Fields::PRIVATE_CHAT_COLLECTION, mongo::Query(query_obj).sort( BSON( Chat_Operating_Fields::TIME << -1 ) ), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		Chat_Record cr;
		cr.chat_type = obj[Chat_Operating_Fields::CHAT_TYPE].numberInt();
		cr.send_time = obj[Chat_Operating_Fields::TIME].numberInt();
		BSONObjIterator it(iter->getObjectField(Chat_Operating_Fields::RECORD.c_str()));
		BSONObj ccobj;
		Chat_Content cc;
		while (it.more()){
			cc.reset();
			ccobj = it.next().embeddedObject();
			cc.type = ccobj[Chat_Operating_Fields::Record::TYPE].numberInt();
			cc.text = ccobj[Chat_Operating_Fields::Record::CONTENT].valuestrsafe();

			cc.item.item_base_info.id = ccobj[Chat_Operating_Fields::Record::ITEM_ID].numberInt();
			cc.item.item_base_info.amount = ccobj[Chat_Operating_Fields::Record::ITEM_AMOUNT].numberInt();
			cc.item.item_base_info.bind = ccobj[Chat_Operating_Fields::Record::ITEM_BIND].numberInt();

			cc.role_Info.role_id = ccobj[Chat_Operating_Fields::Record::ROLE_ROLE_ID].numberLong();
			cc.role_Info.role_name = ccobj[Chat_Operating_Fields::Record::ROLE_ROLE_NAME].valuestrsafe();
			cc.role_Info.gender = ccobj[Chat_Operating_Fields::Record::ROLE_GENDER].numberInt();
			cc.role_Info.guild_name = ccobj[Chat_Operating_Fields::Record::ROLE_GUILD_NAME].valuestrsafe();
			cc.role_Info.career = ccobj[Chat_Operating_Fields::Record::ROLE_CAREER].numberInt();
			cc.role_Info.level = ccobj[Chat_Operating_Fields::Record::ROLE_LEVEL].numberInt();
			cc.role_Info.vip_level = ccobj[Chat_Operating_Fields::Record::ROLE_VIP_LEVEL].numberInt();
			cc.role_Info.combat = ccobj[Chat_Operating_Fields::Record::ROLE_COMBAT].numberInt();


			cr.content.push_back(cc);
		}
		chat_record.push_back(cr);
	}
	return 0;
}

int DB_Operator::load_private_chat_record(role_id_t role_id, role_id_t target, std::vector<Chat_Record>& chat_record) {
	BSONObj query_obj = BSON( "$or"	<< BSON_ARRAY( BSON( Chat_Operating_Fields::ROLE_ID << (long long int)role_id << Chat_Operating_Fields::TARGET << (long long int)target)
			<< BSON( Chat_Operating_Fields::ROLE_ID << (long long int)target << Chat_Operating_Fields::TARGET << (long long int)role_id)));
//	unsigned long long res_count = MONGO_CONNECTION.count(Chat_Operating_Fields::PRIVATE_CHAT_COLLECTION, query_obj);
	unsigned long long res_count = 100;
	BSONObjBuilder field_builder;
	field_builder 	<< Chat_Operating_Fields::ROLE_ID << 1
					<< Chat_Operating_Fields::CHAT_TYPE << 1
					<< Chat_Operating_Fields::TIME << 1
					<< Chat_Operating_Fields::RECORD << 1
					<< Chat_Operating_Fields::TARGET << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Chat_Operating_Fields::PRIVATE_CHAT_COLLECTION, mongo::Query(query_obj).sort( BSON( Chat_Operating_Fields::TIME << -1 ) ), res_count, 0, &field_bson);
	uint32_t last_time = 0;
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		Chat_Record cr;
		cr.chat_type = obj[Chat_Operating_Fields::CHAT_TYPE].numberInt();
		cr.send_time = obj[Chat_Operating_Fields::TIME].numberInt();
		last_time = cr.send_time;
		BSONObjIterator it(iter->getObjectField(Chat_Operating_Fields::RECORD.c_str()));
		BSONObj ccobj;
		Chat_Content cc;
		while (it.more()){
			cc.reset();
			ccobj = it.next().embeddedObject();
			cc.type = ccobj[Chat_Operating_Fields::Record::TYPE].numberInt();
			cc.text = ccobj[Chat_Operating_Fields::Record::CONTENT].valuestrsafe();

			cc.item.item_base_info.id = ccobj[Chat_Operating_Fields::Record::ITEM_ID].numberInt();
			cc.item.item_base_info.amount = ccobj[Chat_Operating_Fields::Record::ITEM_AMOUNT].numberInt();
			cc.item.item_base_info.bind = ccobj[Chat_Operating_Fields::Record::ITEM_BIND].numberInt();

			cc.role_Info.role_id = ccobj[Chat_Operating_Fields::Record::ROLE_ROLE_ID].numberLong();
			cc.role_Info.role_name = ccobj[Chat_Operating_Fields::Record::ROLE_ROLE_NAME].valuestrsafe();
			cc.role_Info.gender = ccobj[Chat_Operating_Fields::Record::ROLE_GENDER].numberInt();
			cc.role_Info.guild_name = ccobj[Chat_Operating_Fields::Record::ROLE_GUILD_NAME].valuestrsafe();
			cc.role_Info.career = ccobj[Chat_Operating_Fields::Record::ROLE_CAREER].numberInt();
			cc.role_Info.level = ccobj[Chat_Operating_Fields::Record::ROLE_LEVEL].numberInt();
			cc.role_Info.vip_level = ccobj[Chat_Operating_Fields::Record::ROLE_VIP_LEVEL].numberInt();
			cc.role_Info.combat = ccobj[Chat_Operating_Fields::Record::ROLE_COMBAT].numberInt();


			cr.content.push_back(cc);
		}
		chat_record.push_back(cr);
	}
	//TODO DELETE
	return 0;
}

int DB_Operator::save_gang_chat_record(Gang_ID_Chat_Record_Map& gang_chat_record_map) {
	for(Gang_ID_Chat_Record_Map::iterator iter = gang_chat_record_map.begin(); iter != gang_chat_record_map.end(); iter++) {
		if(iter->second.is_change) {
			iter->second.is_change = false;
			BSONObjBuilder builder;
			std::vector<BSONObj> cr_vector;
			for(std::vector<Chat_Record>::iterator cr_iter = iter->second.chat_record_vec.begin(); cr_iter != iter->second.chat_record_vec.end(); cr_iter++){
				std::vector<BSONObj> record_vector;
				for(std::vector<Chat_Content>::iterator it = cr_iter->content.begin(); it != cr_iter->content.end(); it++) {
					record_vector.push_back( BSON( Chat_Operating_Fields::Record::CHAT_TYPE << it->type
											<< Chat_Operating_Fields::Record::CONTENT << it->text
											<< Chat_Operating_Fields::Record::TYPE << it->type
											<< Chat_Operating_Fields::Record::ROLE_ROLE_ID << (long long int)it->role_Info.role_id
											<< Chat_Operating_Fields::Record::ROLE_ROLE_NAME << it->role_Info.role_name
											<< Chat_Operating_Fields::Record::ROLE_GENDER << it->role_Info.gender
											<< Chat_Operating_Fields::Record::ROLE_GUILD_NAME << it->role_Info.guild_name
											<< Chat_Operating_Fields::Record::ROLE_CAREER << it->role_Info.career
											<< Chat_Operating_Fields::Record::ROLE_LEVEL << it->role_Info.level
											<< Chat_Operating_Fields::Record::ROLE_VIP_LEVEL << it->role_Info.vip_level
											<< Chat_Operating_Fields::Record::ROLE_COMBAT << it->role_Info.combat
											<< Chat_Operating_Fields::Record::ITEM_ID << it->item.item_base_info.id
											<< Chat_Operating_Fields::Record::ITEM_AMOUNT << it->item.item_base_info.amount
											<< Chat_Operating_Fields::Record::ITEM_BIND << it->item.item_base_info.bind) );
				}
				cr_vector.push_back( BSON( Chat_Operating_Fields::TIME << (long long int)cr_iter->send_time
										<< Chat_Operating_Fields::CHAT_TYPE << cr_iter->chat_type
										<< Chat_Operating_Fields::RECORD << record_vector) );
			}
			builder << Chat_Operating_Fields::RECORDS << cr_vector;
			MONGO_CONNECTION.update(Chat_Operating_Fields::GANG_CHAT_RECORD_COLLECTION, QUERY(Chat_Operating_Fields::TARGET << (long long int)iter->first),
					BSON("$set" << builder.obj() ), true);
		}
	}
	return 0;
}

int DB_Operator::load_gang_chat_record(Gang_ID_Chat_Record_Map& gang_chat_record_map) {
	gang_chat_record_map.clear();
	unsigned long long res_count = MONGO_CONNECTION.count(Chat_Operating_Fields::GANG_CHAT_RECORD_COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< Chat_Operating_Fields::RECORDS << 1
					<< Chat_Operating_Fields::TARGET << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Chat_Operating_Fields::GANG_CHAT_RECORD_COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		gang_id_t gang_id = obj[Chat_Operating_Fields::TARGET].numberLong();
		Gang_Chat_Record gcr;
		gcr.reset();

		BSONObjIterator crit(iter->getObjectField(Chat_Operating_Fields::RECORDS.c_str()));
		BSONObj crobj;
		Chat_Record cr;
		while (crit.more()){
			cr.reset();
			crobj = crit.next().embeddedObject();
			cr.chat_type = crobj[Chat_Operating_Fields::CHAT_TYPE].numberInt();
			cr.send_time =  crobj[Chat_Operating_Fields::TIME].numberInt();

				BSONObjIterator ccit(crobj.getObjectField(Chat_Operating_Fields::RECORD.c_str()));
				BSONObj ccobj;
				Chat_Content cc;
				while (ccit.more()) {
					cc.reset();
					ccobj = ccit.next().embeddedObject();

					cc.type = ccobj[Chat_Operating_Fields::Record::TYPE].numberInt();
					cc.text = ccobj[Chat_Operating_Fields::Record::CONTENT].valuestrsafe();

					cc.item.item_base_info.id = ccobj[Chat_Operating_Fields::Record::ITEM_ID].numberInt();
					cc.item.item_base_info.amount = ccobj[Chat_Operating_Fields::Record::ITEM_AMOUNT].numberInt();
					cc.item.item_base_info.bind = ccobj[Chat_Operating_Fields::Record::ITEM_BIND].numberInt();

					cc.role_Info.role_id = ccobj[Chat_Operating_Fields::Record::ROLE_ROLE_ID].numberLong();
					cc.role_Info.role_name = ccobj[Chat_Operating_Fields::Record::ROLE_ROLE_NAME].valuestrsafe();
					cc.role_Info.gender = ccobj[Chat_Operating_Fields::Record::ROLE_GENDER].numberInt();
					cc.role_Info.guild_name = ccobj[Chat_Operating_Fields::Record::ROLE_GUILD_NAME].valuestrsafe();
					cc.role_Info.career = ccobj[Chat_Operating_Fields::Record::ROLE_CAREER].numberInt();
					cc.role_Info.level = ccobj[Chat_Operating_Fields::Record::ROLE_LEVEL].numberInt();
					cc.role_Info.vip_level = ccobj[Chat_Operating_Fields::Record::ROLE_VIP_LEVEL].numberInt();
					cc.role_Info.combat = ccobj[Chat_Operating_Fields::Record::ROLE_COMBAT].numberInt();

					cr.content.push_back(cc);
				}

			gcr.chat_record_vec.push_back(cr);
		}

		gang_chat_record_map.insert(std::make_pair(gang_id, gcr));

	}
	return 0;
}

int DB_Operator::save_operating_team_award(role_id_t role_id, int level_id) {
	BSONObjBuilder builder;
	BSONObj obj = BSON("id" << level_id);
	builder.append("team_award", obj);
	MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	return 0;
}

int DB_Operator::save_operating_offline_mail(role_id_t role_id, Mail_Send_Info &mail, DBClientConnection *dest_conn) {
	BSONObjBuilder builder;
	std::vector<BSONObj> item_vector;
	//BSONObj obj;
	for(Item_Vec::const_iterator item_iter = mail.item_vector.begin(); item_iter != mail.item_vector.end(); item_iter++ ) {
		item_vector.push_back( BSON(Package_Fields::Item::INDEX << item_iter->index_ << Package_Fields::Item::ID
				<< item_iter->id_ << Package_Fields::Item::AMOUNT << item_iter->amount_
				<< Package_Fields::Item::BIND << item_iter->bind_
				<< Package_Fields::Item::OVERDUE_TIME << (long long int)item_iter->overdue_time.sec()));
	}
	BSONObj mail_obj = BSON( Operating_Fields::Mail_List::RECEIVER_ID << (long long int)mail.receiver_id
								<< Operating_Fields::Mail_List::SENDER_ID << (long long int)mail.sender_id
								<< Operating_Fields::Mail_List::SEND_TYPE << mail.send_type
								<< Operating_Fields::Mail_List::SENDER_NAME << mail.sender_name
								<< Operating_Fields::Mail_List::GOLD << mail.gold
								<< Operating_Fields::Mail_List::TITLE << mail.title
								<< Operating_Fields::Mail_List::CONTENT << mail.content
								<< Operating_Fields::Mail_List::ITEM << item_vector
								<< Operating_Fields::Mail_List::SHOW_SEND_TIME << (long long int)mail.show_send_time.sec());

	builder.append(Operating_Fields::MAIL_LIST, mail_obj);

	if (dest_conn) {
		dest_conn->update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
				BSON("$push" << builder.obj() ), true);
	} else {
		MONGO_CONNECTION.update(Operating_Fields::COLLECTION, QUERY(Operating_Fields::ROLE_ID << (long long int)role_id),
			BSON("$push" << builder.obj() ), true);
	}
	return 0;
}

int DB_Operator::load_packer_detail(Packer_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Package_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
	if (! res.isEmpty() && res.hasField(Package_Fields::PACKAGE.c_str())) {
		detail.role_id = res[Role_Fields::ROLE_ID].numberLong();

		BSONObj package = res.getObjectField(Package_Fields::PACKAGE.c_str());
		detail.capacity.pack_cap = package[Package_Fields::PACK_CAPACITY].numberInt();
		detail.capacity.storage_cap = package[Package_Fields::STORAGE_CAPACITY].numberInt();
		detail.capacity.spirit_equip_cap = package[Package_Fields::SPIRIT_EQUIP_CAPACITY].numberInt();
		detail.capacity.spirit_package_cap = package[Package_Fields::SPIRIT_PACKAGE_CAPACITY].numberInt();
		detail.capacity.rune_stone_equip_cap = package[Package_Fields::RUNE_STONE_EQUIP_CAP].numberInt();
		detail.capacity.rune_stone_pack_cap = package[Package_Fields::RUNE_STONE_PACK_CAP].numberInt();
		detail.capacity.thorlottery_cap = package[Package_Fields::THORLOTTERY_PACK_CAP].numberInt();

		BSONObj money = package.getObjectField(Package_Fields::MONEY.c_str());
		detail.money_detail.gold = money[Package_Fields::Money::COPPER].numberInt();
		detail.money_detail.bind_diamond = money[Package_Fields::Money::BIND_GOLD].numberInt();
		detail.money_detail.diamond = money[Package_Fields::Money::GOLD].numberInt();
		detail.money_detail.dragon_soul = money[Package_Fields::Money::DRAGON_SOUL].numberInt();
		detail.money_detail.souls = money[Package_Fields::Money::SOULS].numberInt();
		detail.money_detail.friendship = money[Package_Fields::Money::FRIENDSHIP].numberInt();
		detail.money_detail.reputation = money[Package_Fields::Money::REPUTATION].numberInt();
		detail.money_detail.today_diamond_pay = money[Package_Fields::Money::TODAY_GOLD_PAY].numberInt();
		detail.money_detail.today_diamond_get = money[Package_Fields::Money::TODAY_GOLD_GET].numberInt();
		detail.money_detail.today_gold_pay = money[Package_Fields::Money::TODAY_COPPER_PAY].numberInt();
		detail.money_detail.today_gold_get = money[Package_Fields::Money::TODAY_COPPER_GET].numberInt();

		BSONObjIterator iter(package.getObjectField(Package_Fields::ITEM.c_str()));
		BSONObj obj;
		while (iter.more()){
			obj = iter.next().embeddedObject();
			Item_Detail *item = POOL_MANAGER->pop_item_pool(1);
			if (load_item_detail(obj, *item) == 0) {
				if(!detail.goods_map.insert(std::make_pair(item->index_, item)).second) {
					MSG_USER("insert goods map i:%d", item->index_);
					POOL_MANAGER->push_item_pool(item, 11);
				}
			} else {
				MSG_USER("load_item_detail error i:%d", item->index_);
				POOL_MANAGER->push_item_pool(item, 12);
			}
		}

		long sec = package[Package_Fields::LAST_WIPE_TIME].numberLong();
		detail.last_wipe_time.set(sec, 0);
	}

	return 0;
}

int DB_Operator::save_packer_detail(Packer_Detail &detail) {
	std::vector<BSONObj> vc_item;
	BSONObj obj;
	for (Goods_Map::const_iterator iter = detail.goods_map.begin(); iter != detail.goods_map.end(); ++iter) {
		save_item_detail(*iter->second, obj, detail);
		vc_item.push_back(obj);
	}

	BSONObjBuilder tmp_builder;
	tmp_builder << Package_Fields::PACKAGE << BSON(Package_Fields::PACK_CAPACITY << detail.capacity.pack_cap
		<< Package_Fields::STORAGE_CAPACITY << detail.capacity.storage_cap
		<< Package_Fields::SPIRIT_EQUIP_CAPACITY << detail.capacity.spirit_equip_cap
		<< Package_Fields::SPIRIT_PACKAGE_CAPACITY << detail.capacity.spirit_package_cap
		<< Package_Fields::RUNE_STONE_EQUIP_CAP << detail.capacity.rune_stone_equip_cap
		<< Package_Fields::RUNE_STONE_PACK_CAP << detail.capacity.rune_stone_pack_cap
		<< Package_Fields::THORLOTTERY_PACK_CAP << detail.capacity.thorlottery_cap
		<< Package_Fields::LAST_WIPE_TIME << (long long int)detail.last_wipe_time.sec()
		<< Package_Fields::MONEY << BSON(Package_Fields::Money::COPPER << detail.money_detail.gold
				<< Package_Fields::Money::BIND_GOLD << detail.money_detail.bind_diamond
				<< Package_Fields::Money::GOLD << detail.money_detail.diamond
				<< Package_Fields::Money::DRAGON_SOUL << detail.money_detail.dragon_soul
				<< Package_Fields::Money::SOULS << detail.money_detail.souls
				<< Package_Fields::Money::FRIENDSHIP << detail.money_detail.friendship
				<< Package_Fields::Money::REPUTATION << detail.money_detail.reputation
				<< Package_Fields::Money::TODAY_GOLD_PAY << detail.money_detail.today_diamond_pay
				<< Package_Fields::Money::TODAY_GOLD_GET << detail.money_detail.today_diamond_get
				<< Package_Fields::Money::TODAY_COPPER_PAY << detail.money_detail.today_gold_pay
				<< Package_Fields::Money::TODAY_COPPER_GET << detail.money_detail.today_gold_get)
		<< Package_Fields::ITEM << vc_item);

	MONGO_CONNECTION.update(Package_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << tmp_builder.obj()), true);

	return 0;
}

int DB_Operator::load_item_user_detail(Item_User_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Item_User_Fields::COLLECTION,
				QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));

	if (res.isEmpty()) {
		return 0;
	}
	detail.role_id = res[Role_Fields::ROLE_ID].numberLong();
	BSONObjIterator iter(res.getObjectField(Item_User_Fields::USE_TIME.c_str()));

	BSONObj obj;
	int i = 0;
	while (iter.more()) {
		obj = iter.next().embeddedObject();
		long int sec = obj[Item_User_Fields::ITEM_USE_SEC].numberLong();
		if (i < CD_T_VEC_SIZE) {
			detail.can_use_time_[i].set(sec, 0);
			++i;
		}
	}

	BSONObjIterator iter_prop(res.getObjectField(Item_User_Fields::ITEM_PROP.c_str()));
	while (iter_prop.more()) {
		obj = iter_prop.next().embeddedObject();
		int prop_type = obj[Item_User_Fields::ITEM_PROP_TYPE].numberInt();
		double value = obj[Item_User_Fields::ITEM_PROP_VALUE].numberDouble();
		detail.extra_prop_.insert(std::make_pair(prop_type, value));
	}

	BSONObjIterator item_had_used_times(res.getObjectField(Item_User_Fields::ITEM_HAD_USED_TIMES.c_str()));
	while (item_had_used_times.more()) {
		obj = item_had_used_times.next().embeddedObject();
		role_id_t prop_type = obj[Item_User_Fields::ITEM_HAD_USED_TIMES_TYPE].numberLong();
		int value = obj[Item_User_Fields::ITEM_HAD_USED_TIMES_VALUE].numberInt();
		detail.item_had_used_times_.insert(std::make_pair(prop_type, value));
	}

	return 0;
}

int DB_Operator::save_item_user_detail(Item_User_Detail &detail) {
	std::vector<BSONObj> vc_use_item;
	for (Item_User_Detail::Item_Use_Time::const_iterator iter = detail.can_use_time_.begin(); iter != detail.can_use_time_.end(); ++iter) {
		BSONObjBuilder builder;
		builder << Item_User_Fields::ITEM_USE_SEC << (long long int)(iter->sec());
		vc_use_item.push_back(builder.obj());
	}

	std::vector<BSONObj> vc_item_prop;
	for (Int_Double_Map::const_iterator it = detail.extra_prop_.begin(); it != detail.extra_prop_.end(); ++it) {
		BSONObjBuilder builder;
		builder << Item_User_Fields::ITEM_PROP_TYPE << it->first << Item_User_Fields::ITEM_PROP_VALUE << it->second;
		vc_item_prop.push_back(builder.obj());
	}

	std::vector<BSONObj> vc_item_had_used_times;
	for (std::map<role_id_t, int>::const_iterator it = detail.item_had_used_times_.begin(); it != detail.item_had_used_times_.end(); ++it) {
		BSONObjBuilder builder;
		builder << Item_User_Fields::ITEM_HAD_USED_TIMES_TYPE << (long long int)(it->first) << Item_User_Fields::ITEM_HAD_USED_TIMES_VALUE << it->second;
		vc_item_had_used_times.push_back(builder.obj());
	}

	BSONObjBuilder tmp_builder;
	tmp_builder << Item_User_Fields::USE_TIME << vc_use_item << Item_User_Fields::ITEM_PROP << vc_item_prop << Item_User_Fields::ITEM_HAD_USED_TIMES << vc_item_had_used_times;

	MONGO_CONNECTION.update(Item_User_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
		BSON("$set" << tmp_builder.obj()), true);

	return 0;
}

int DB_Operator::load_tasker_detail(Tasker_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Task_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));

	if (res.isEmpty())
		return 0;

	detail.is_ever_open_reward_task_list = (res[Task_Fields::IS_EVER_OPEN_REWARD_TASK_LIST].numberInt() == 1) ? true :false;
	detail.is_close_auto_refresh = (res[Task_Fields::IS_CLOSE_AUTO_REFRESH].numberInt() == 1) ? true :false;
	detail.refresh_reward_task_num = (uint8_t)res[Task_Fields::REFRESH_REWARD_TASK_NUM].numberInt();
	detail.is_open_countdown = (res[Task_Fields::IS_OPEN_COUNTDOWN].numberInt() == 1) ? true : false;
	detail.begin_countdown_time.sec(res[Task_Fields::BEGIN_COUNTDOWN_TIME_SEC].numberLong());
	detail.begin_countdown_time.usec(res[Task_Fields::BEGIN_COUNTDOWN_TIME_USEC].numberLong());
	detail.used_free_refresh_count = (uint32_t)res[Task_Fields::USED_FREE_REFRESH_COUNT].numberInt();
	detail.used_accept_count = (uint32_t)res[Task_Fields::USED_ACCEPT_COUNT].numberInt();
	detail.give_up_num = (uint32_t)res[Task_Fields::GIVE_UP_NUM].numberInt();
	detail.target_player_id =  (role_id_t)res[Task_Fields::TARGET_PLAYER_ID].numberLong();
	detail.task_id = (uint32_t)res[Task_Fields::TASK_ID].numberInt();
	detail.ring_num = (uint32_t)res[Task_Fields::RING_NUM].numberInt();
	detail.is_diamond_recover = (res[Task_Fields::IS_DIAMOND_RECOVER].numberInt() == 1) ? true : false;
	detail.did_ring_num = (uint32_t)res[Task_Fields::DID_RING_NUM].numberInt();
	detail.did_reward_task_num = (uint32_t)res[Task_Fields::DID_REWARD_TASK_NUM].numberInt();
	detail.ring_task_id = (uint32_t)res[Task_Fields::RING_TASK_ID].numberInt();
	detail.is_exist_cross_day = (res[Task_Fields::IS_EXIST_CROSS_DAY].numberInt() == 1) ? true : false;
	detail.did_reward_task_num_today = (uint32_t)res[Task_Fields::DID_REWARD_TASK_NUM_TODAY].numberInt();
	detail.is_submit_close_ring_task = (res[Task_Fields::IS_SUBMIT_CLOSE_RING_TASK].numberInt() == 1) ? true : false;

	{
		int task_id = 0;
		BSONObjIterator iter(res.getObjectField(Task_Fields::SUBMITED_TASK.c_str()));
		while (iter.more())
		{
			task_id = iter.next().numberInt();
			detail.task_submited_once_.insert(task_id);
		}
	}
	{
		BSONObjIterator iter(res.getObjectField(Task_Fields::TASK.c_str()));
		TaskInfo task_info;
		while (iter.more())
		{
			BSONObj task_obj = iter.next().embeddedObject();
			uint32_t task_id = task_obj[Task_Fields::Task::TASK_ID].numberInt();
			if (!CONFIG_CACHE_TASK->find_task(task_id)) {
				MSG_USER("task_id error %d", task_id);
				continue;
			}

			int cond_size = 0;
			task_info.reset();
			task_info.__condition_list.clear();

			task_info.__task_id = task_id;

			task_info.__game_type = task_obj[Task_Fields::Task::GAME_TYPE].numberInt();
			task_info.__accept_tick.set(
					task_obj[Task_Fields::Task::ACCEPT_TICK_SEC].numberLong(),
					task_obj[Task_Fields::Task::ACCEPT_TICK_USEC].numberLong());
			task_info.__refresh_tick.set(
					task_obj[Task_Fields::Task::REFRESH_TICK_SEC].numberLong(),
					task_obj[Task_Fields::Task::REFRESH_TICK_USEC].numberLong());
			task_info.__prev_task = task_obj[Task_Fields::Task::PREV_TASK].numberInt();
			task_info.__post_task = task_obj[Task_Fields::Task::POST_TASK].numberInt();
			task_info.__post_task2 = task_obj[Task_Fields::Task::POST_TASK2].numberInt();
			task_info.__post_task3 = task_obj[Task_Fields::Task::POST_TASK3].numberInt();
			task_info.__item_money_flag = task_obj[Task_Fields::Task::ITEM_MONEY_FLAG].numberInt();
			task_info.__recruid_hero = task_obj[Task_Fields::Task::RECRUID_HERO].numberInt() ? true : false;
			task_info.__is_req_fast_finish = task_obj[Task_Fields::Task::IS_REQ_FAST_FINISH].numberInt() ? true : false;
			task_info.__hero_id = task_obj[Task_Fields::Task::HERO_ID].numberInt();
			task_info.__is_pack_accept = (task_obj[Task_Fields::Task::IS_PACK_ACCEPT].numberInt() == 1) ? true : false;


			BSONObjIterator status_iter(task_obj.getObjectField(Task_Fields::Task::STATUS.c_str()));
			while (status_iter.more())
			{
				task_info.set_task_status(status_iter.next().numberInt());
			}

			BSONObjIterator logic_type_iter(task_obj.getObjectField(Task_Fields::Task::LOGIC_TYPE.c_str()));
			while (logic_type_iter.more())
			{
				task_info.set_logic_type(logic_type_iter.next().numberInt());
			}

			BSONObjIterator cond_iter(task_obj.getObjectField(Task_Fields::Task::COND_LIST.c_str()));
			TaskCondition task_cond;
			while (cond_iter.more())
			{
				if(cond_size > 10) {
					break;
				}

				BSONObj cond_obj = cond_iter.next().embeddedObject();
				task_cond.reset();
				task_cond.__type = cond_obj[Task_Fields::Task::CondList::TYPE].numberInt();
				task_cond.__current_value = cond_obj[Task_Fields::Task::CondList::CURRENT_VALUE].numberInt();

				BSONObjIterator ext_iter(task_obj.getObjectField(Task_Fields::Task::CondList::EXT_LIST.c_str()));
				while(ext_iter.more()) {
					BSONObj ext_obj = ext_iter.next().embeddedObject();
					role_id_t ext = (role_id_t)ext_obj[Task_Fields::Task::CondList::ExtList::EXT].numberLong();
					task_cond.ext_set.insert(ext);
				}

				task_info.__condition_list.push_back(task_cond);

				++cond_size;
			}

			if (detail.bind_task(task_info.__task_id, task_info) != 0)
			{
				MSG_USER("bind_task error: %d", task_id);
				continue;
			}
		}
	}

	{
		BSONObjIterator iter(res.getObjectField(Task_Fields::NPC_TYPE_MAP.c_str()));
		Task_Npc_Type task_npc_type;
		while (iter.more())
		{
			BSONObj npc_obj = iter.next().embeddedObject();
			task_npc_type.reset();
			task_npc_type.npc_type = npc_obj[Task_Fields::Npc_Type_Map::NPC_TYPE].numberInt();
			if (task_npc_type.npc_type < 0) {
				continue;
			}
			task_npc_type.npc_id = npc_obj[Task_Fields::Npc_Type_Map::NPC_ID].numberLong();
			task_npc_type.funcName = npc_obj[Task_Fields::Npc_Type_Map::FUNCNAME].String();
			task_npc_type.direction = npc_obj[Task_Fields::Npc_Type_Map::DIRECTION].numberInt();
			if (npc_obj.hasField(Task_Fields::Npc_Type_Map::SCENEID.c_str())) {
				task_npc_type.sceneId = npc_obj[Task_Fields::Npc_Type_Map::SCENEID].String();
			}
			if (npc_obj.hasField(Task_Fields::Npc_Type_Map::OPEN.c_str())) {
				task_npc_type.open = npc_obj[Task_Fields::Npc_Type_Map::OPEN].String();
			}
			task_npc_type.x = npc_obj[Task_Fields::Npc_Type_Map::X].numberInt();
			task_npc_type.y = npc_obj[Task_Fields::Npc_Type_Map::Y].numberInt();
			if (npc_obj.hasField(Task_Fields::Npc_Type_Map::NPC_NAME.c_str())) {
				task_npc_type.npc_name = npc_obj[Task_Fields::Npc_Type_Map::NPC_NAME].String();
			}

			detail.npc_type_map_.insert(Task_Npc_Type_Map::value_type(task_npc_type.npc_type, task_npc_type));
		}
	}

	{
		role_id_t npc_id = 0;
		BSONObjIterator iter(res.getObjectField(Task_Fields::NPC_ID.c_str()));
		while (iter.more())
		{
			npc_id = iter.next().numberLong();
			detail.npc_id_.insert(Role_Id_Set::value_type(npc_id));
		}
	}

	{
		uint32_t task_id = 0;
		BSONObjIterator iter(res.getObjectField(Task_Fields::TASK_GIFT_LEVEL.c_str()));
		while (iter.more())
		{
			task_id = (uint32_t)iter.next().numberInt();
			detail.task_gift_level_set.insert(task_id);
		}
	}

	{
		BSONObjIterator iter(res.getObjectField(Task_Fields::TASK_ICON_HERO_LIST.c_str()));
		while (iter.more())
		{
			BSONObj task_icon_hero = iter.next().embeddedObject();
			uint32_t key = (uint32_t)task_icon_hero[Task_Fields::Task_Icon_Hero::TASK_ID].numberInt();
			uint32_t value = (uint32_t)task_icon_hero[Task_Fields::Task_Icon_Hero::HERO_ID].numberInt();
			detail.task_icon_hero_id_Map.insert(std::make_pair(key, value));
		}
	}

	{
		BSONObjIterator iter(res.getObjectField(Task_Fields::SCENE_ID_LIST.c_str()));
		while(iter.more()) {
			BSONObj scene_id_json = iter.next().embeddedObject();
			uint32_t scene_id = (uint32_t)scene_id_json[Task_Fields::Scene_Id::SCENE_ID].numberInt();
			detail.scene_id_set.insert(scene_id);
		}
	}

	{
		BSONObjIterator iter(res.getObjectField(Task_Fields::SCENE_NPC_LIST.c_str()));
		while(iter.more()) {
			BSONObj scene_npc_json = iter.next().embeddedObject();
			uint32_t scene_id = (uint32_t)scene_npc_json[Task_Fields::Scene_Npc::SCENE_ID].numberInt();
			uint32_t npc_id = (uint32_t)scene_npc_json[Task_Fields::Scene_Npc::NPC_ID].numberInt();
			Svr_Scene_Npc svr_scene_npc;
			svr_scene_npc.scene_id = scene_id;
			svr_scene_npc.npc_id = npc_id;
			detail.svr_scene_npc_set.insert(svr_scene_npc);
		}
	}

	return 0;
}

void DB_Operator::save_one_task_of_tasker_detail(std::vector<int> &status_vc,
		std::vector<int> &logic_type_vc,
		std::vector<BSONObj> &task_vc, std::vector<BSONObj> &cond_vc,
		const TaskInfo &task_info) {

    cond_vc.clear();
    status_vc.clear();
    logic_type_vc.clear();

	std::vector<long long int> ext_vec;

    for (TaskInfo::TaskConditionList::const_iterator cond_iter = task_info.__condition_list.begin();
            cond_iter != task_info.__condition_list.end(); ++cond_iter)
    {
    	ext_vec.clear();
    	for(EXT_SET::const_iterator ext_it = cond_iter->ext_set.begin();
    			ext_it != cond_iter->ext_set.end(); ++ext_it) {
    		ext_vec.push_back((long long int)(*ext_it));
    	}

    	cond_vc.push_back(BSON(Task_Fields::Task::CondList::TYPE << (cond_iter->__type)
    	      << Task_Fields::Task::CondList::CURRENT_VALUE << (cond_iter->__current_value)
    	      << Task_Fields::Task::CondList::EXT_LIST << (ext_vec)));
    }

    for (int i = TaskInfo::TASK_STATUS_NONE; i < TaskInfo::TASK_STATUS_END; ++i)
    {
        if (task_info.__status.test(i))
        	status_vc.push_back(i);
    }

    for (int i = TaskCondition::TASK_CT_NONE; i < TaskCondition::TASK_CT_END; ++i)
    {
        if (task_info.__logic_type.test(i))
            logic_type_vc.push_back(i);
    }

    task_vc.push_back(BSON(Task_Fields::Task::TASK_ID << task_info.__task_id
                << Task_Fields::Task::GAME_TYPE << task_info.__game_type
                << Task_Fields::Task::ACCEPT_TICK_SEC << (long long int)(task_info.__accept_tick.sec())
                << Task_Fields::Task::ACCEPT_TICK_USEC << (long long int)(task_info.__accept_tick.usec())
                << Task_Fields::Task::REFRESH_TICK_SEC << (long long int)(task_info.__refresh_tick.sec())
                << Task_Fields::Task::REFRESH_TICK_USEC << (long long int)(task_info.__refresh_tick.usec())
                << Task_Fields::Task::PREV_TASK << task_info.__prev_task
                << Task_Fields::Task::POST_TASK << task_info.__post_task
                << Task_Fields::Task::POST_TASK2 << task_info.__post_task2
                << Task_Fields::Task::POST_TASK3 << task_info.__post_task3
                << Task_Fields::Task::ITEM_MONEY_FLAG << task_info.__item_money_flag
                << Task_Fields::Task::IS_REQ_FAST_FINISH << (task_info.__is_req_fast_finish ? 1 : 0)
                << Task_Fields::Task::RECRUID_HERO << (task_info.__recruid_hero ? 1 : 0)
                << Task_Fields::Task::HERO_ID << task_info.__hero_id
                << Task_Fields::Task::IS_PACK_ACCEPT << (task_info.__is_pack_accept ? 1: 0)
                << Task_Fields::Task::STATUS << status_vc
                << Task_Fields::Task::LOGIC_TYPE << logic_type_vc
                << Task_Fields::Task::COND_LIST << cond_vc));
}

int DB_Operator::save_tasker_detail(Tasker_Detail &detail) {
    std::vector<BSONObj> task_vc, cond_vc;
    std::vector<int> status_vc, logic_type_vc, submited_vc;
    std::vector<BSONObj> npc_type_vc;
    std::vector<long long int> npc_id_vc;
    std::vector<int> task_gift_level_vc;
    std::vector<BSONObj> task_icon_hero_vc;
    std::vector<BSONObj> scene_id_vc;
    std::vector<BSONObj> scene_npc_vc;

    task_vc.clear();
    submited_vc.clear();
    npc_type_vc.clear();
    npc_id_vc.clear();
    task_gift_level_vc.clear();
    task_icon_hero_vc.clear();
    scene_id_vc.clear();
    scene_npc_vc.clear();

    for (TaskMap::const_iterator iter = detail.task_map_.begin(); iter != detail.task_map_.end(); ++iter)
    {
        cond_vc.clear();
        status_vc.clear();
        logic_type_vc.clear();
        save_one_task_of_tasker_detail(status_vc, logic_type_vc, task_vc, cond_vc, iter->second);
    }

    for(RewardTaskMap::const_iterator it = detail.reward_task_map.begin();
    		it != detail.reward_task_map.end(); ++it) {
        cond_vc.clear();
        status_vc.clear();
        logic_type_vc.clear();
    	save_one_task_of_tasker_detail(status_vc, logic_type_vc, task_vc, cond_vc, it->second);
    }

	for (TaskIdSet::iterator iter = detail.task_submited_once_.begin();
			iter != detail.task_submited_once_.end(); ++iter)
	{
		submited_vc.push_back(*iter);
	}

	for (Task_Npc_Type_Map::iterator iter = detail.npc_type_map_.begin(); iter != detail.npc_type_map_.end(); ++iter) {
		Task_Npc_Type task_npc_type = iter->second;
		npc_type_vc.push_back(BSON(Task_Fields::Npc_Type_Map::NPC_TYPE << task_npc_type.npc_type
					<< Task_Fields::Npc_Type_Map::NPC_ID << (long long int)task_npc_type.npc_id
					<< Task_Fields::Npc_Type_Map::FUNCNAME << task_npc_type.funcName
					<< Task_Fields::Npc_Type_Map::DIRECTION << task_npc_type.direction
					<< Task_Fields::Npc_Type_Map::SCENEID << task_npc_type.sceneId
					<< Task_Fields::Npc_Type_Map::X << task_npc_type.x
					<< Task_Fields::Npc_Type_Map::Y << task_npc_type.y
					<< Task_Fields::Npc_Type_Map::NPC_NAME << task_npc_type.npc_name));
	}

	for (Role_Id_Set::iterator iter = detail.npc_id_.begin(); iter != detail.npc_id_.end(); ++iter) {
		npc_id_vc.push_back((long long int)(*iter));
	}

	for (Task_Gift_Level_Set::iterator it = detail.task_gift_level_set.begin();
			it != detail.task_gift_level_set.end(); ++it)
	{
		task_gift_level_vc.push_back(*it);
	}

	for(Task_Icon_Hero_Id_Map::iterator it = detail.task_icon_hero_id_Map.begin();
			it != detail.task_icon_hero_id_Map.end();
			++it) {
		BSONObjBuilder builder;
		builder << Task_Fields::Task_Icon_Hero::TASK_ID << it->first
				<< Task_Fields::Task_Icon_Hero::HERO_ID << it->second;
		task_icon_hero_vc.push_back(builder.obj());
	}

	for(Scene_Id_Set::const_iterator it = detail.scene_id_set.begin(); it != detail.scene_id_set.end(); ++it) {
		BSONObjBuilder builder;
		builder << Task_Fields::Scene_Id::SCENE_ID << *it;
		scene_id_vc.push_back(builder.obj());
	}

	for(Svr_Scene_Npc_Set::const_iterator it = detail.svr_scene_npc_set.begin(); it != detail.svr_scene_npc_set.end(); ++it) {
		BSONObjBuilder builder;
		builder << Task_Fields::Scene_Npc::SCENE_ID << it->scene_id
				<< Task_Fields::Scene_Npc::NPC_ID << it->npc_id;
		scene_npc_vc.push_back(builder.obj());
	}

    BSONObjBuilder builder;
    builder << Task_Fields::IS_SUBMIT_CLOSE_RING_TASK << (detail.is_submit_close_ring_task ? 1 : 0)
    	<< Task_Fields::DID_REWARD_TASK_NUM_TODAY << detail.did_reward_task_num_today
    	<< Task_Fields::IS_EXIST_CROSS_DAY << (detail.is_exist_cross_day ? 1 : 0)
    	<< Task_Fields::RING_TASK_ID << detail.ring_task_id
    	<< Task_Fields::DID_REWARD_TASK_NUM << detail.did_reward_task_num
    	<< Task_Fields::DID_RING_NUM << detail.did_ring_num
    	<< Task_Fields::IS_DIAMOND_RECOVER << (detail.is_diamond_recover ? 1 : 0)
    	<< Task_Fields::RING_NUM << detail.ring_num
    	<< Task_Fields::IS_EVER_OPEN_REWARD_TASK_LIST <<(detail.is_ever_open_reward_task_list ? 1 : 0)
    	<< Task_Fields::IS_CLOSE_AUTO_REFRESH << (detail.is_close_auto_refresh ? 1 : 0)
        << Task_Fields::REFRESH_REWARD_TASK_NUM << (detail.refresh_reward_task_num)
    	<< Task_Fields::IS_OPEN_COUNTDOWN << (detail.is_open_countdown ? 1 : 0)
    	<< Task_Fields::BEGIN_COUNTDOWN_TIME_SEC << (long long int)detail.begin_countdown_time.sec()
    	<< Task_Fields::BEGIN_COUNTDOWN_TIME_USEC << (long long int)detail.begin_countdown_time.usec()
    	<< Task_Fields::USED_FREE_REFRESH_COUNT << detail.used_free_refresh_count
    	<< Task_Fields::USED_ACCEPT_COUNT << detail.used_accept_count
    	<< Task_Fields::GIVE_UP_NUM << detail.give_up_num
    	<< Task_Fields::TARGET_PLAYER_ID << (long long int)detail.target_player_id
    	<< Task_Fields::TASK_ID << detail.task_id
        << Task_Fields::SUBMITED_TASK << submited_vc
        << Task_Fields::TASK << task_vc
        << Task_Fields::NPC_ID << npc_id_vc
        << Task_Fields::NPC_TYPE_MAP << npc_type_vc
        << Task_Fields::TASK_GIFT_LEVEL << task_gift_level_vc
        << Task_Fields::TASK_ICON_HERO_LIST << task_icon_hero_vc
        << Task_Fields::SCENE_ID_LIST << scene_id_vc
        << Task_Fields::SCENE_NPC_LIST << scene_npc_vc;

	MONGO_CONNECTION.update(Task_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_item_detail(const BSONObj &obj, Item_Detail &item) {
	int result = -1;
	if (obj.hasField(Package_Fields::Item::INDEX.c_str()) && obj.hasField(Package_Fields::Item::ID.c_str())
			&& obj.hasField(Package_Fields::Item::AMOUNT.c_str()) && obj.hasField(Package_Fields::Item::BIND.c_str())) {

		result = 0;
		item.reset();
		// set_basic为指针以及固定的值如hole分配了空间
		item.set_basic(obj[Package_Fields::Item::INDEX].numberInt(), obj[Package_Fields::Item::ID].numberInt(),
				obj[Package_Fields::Item::AMOUNT].numberInt(), obj[Package_Fields::Item::BIND].numberInt());
		item.overdue_time.sec(obj[Package_Fields::Item::OVERDUE_TIME].numberInt());

		if (item.type_ == Item_Detail::EQUIP ) {
			result = load_item_equip_detail(obj, item.addon.equip);
		} else if (item.type_ == Item_Detail::RUNE_STONE) {
			result = load_item_rune_stone_detail(obj, item.addon.rune_stone);
//		} else if (item.type == Item_Detail::CARD) {
//			result = load_card_detail(obj, item.addon.card);
//		} else if (item.type == Item_Detail::SKILL_STONE) {
//			result = load_skill_stone_detail(obj, item.addon.skill_stone);
//		} else if (item.type == Item_Detail::PET_CARD) {
//			result = load_pet_card_detail(obj, item.addon.pet_card);
//		} else if (item.type == Item_Detail::RUNE) {
//			result = load_rune_detail(obj, item.addon.rune);
//		} else if (item.type == Item_Detail::TASK_ITEM) {
//			result = load_task_item_detail(obj, item.addon.task_item);
//		}  else if (item.type == Item_Detail::MOUNT_EQUIP) {
//			result = load_mount_equip_detail(obj, item.addon.mount_equip);
//		}  else if (item.type == Item_Detail::GEM) {
//			result = load_gem_detail(obj, item.addon.gem);
//		}  else if (item.type == Item_Detail::SPIRIT) {
//			result = load_spirit_detail(obj, item.addon.spirit);
//		}  else if (item.type == Item_Detail::BAOPAN) {
//			result = load_baopan_detail(obj, item.addon.baopan);
//		}  else if (item.type == Item_Detail::FABAO) {
//			result = load_fabao_detail(obj, item.addon.fabao);
//		}  else if (item.type == Item_Detail::EXP_BALL) {
//			result = load_exp_ball_detail(obj, item.addon.exp_ball);
		} else if (item.type_ == Item_Detail::CARD) {
			result = load_item_card_detail(obj, item.addon.card);
		}
	}

	return result;
}

int DB_Operator::save_item_detail(const Item_Detail &item, mongo::BSONObj &obj, const Packer_Detail& packer_detail) {
	BSONObjBuilder item_builder;
	item_builder << Package_Fields::Item::INDEX << item.index_ << Package_Fields::Item::ID
			<< item.id_ << Package_Fields::Item::AMOUNT << item.amount_
			<< Package_Fields::Item::BIND << item.bind_
			<< Package_Fields::Item::OVERDUE_TIME << (long long int)item.overdue_time.sec();

	BSONObj addon_obj;
	if (item.type_ == Item_Detail::EQUIP) {
		save_item_equip_detail(item.addon.equip, addon_obj, packer_detail);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type_ == Item_Detail::RUNE_STONE) {
		save_item_rune_stone_detail(item.addon.rune_stone, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	/*
	} else if (item.type == Item_Detail::CARD) {
		save_card_detail(item.addon.card, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::SKILL_STONE) {
		save_skill_stone_detail(item.addon.skill_stone, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::PET_CARD) {
		save_pet_card_detail(item.addon.pet_card, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::RUNE) {
		save_rune_detail(item.addon.rune, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::TASK_ITEM) {
		save_task_item_detail(item.addon.task_item, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::MOUNT_EQUIP) {
		save_mount_equip_detail(item.addon.mount_equip, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::GEM) {
		save_gem_detail(item.addon.gem, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::SPIRIT) {
		save_spirit_detail(item.addon.spirit, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::BAOPAN) {
		save_baopan_detail(item.addon.baopan, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::FABAO) {
		save_fabao_detail(item.addon.fabao, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	} else if (item.type == Item_Detail::EXP_BALL) {
		save_exp_ball_detail(item.addon.exp_ball, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	*/
	} else if (item.type_ == Item_Detail::CARD) {
		save_item_card_detail(item.addon.card, addon_obj);
		item_builder.appendElementsUnique(addon_obj);
	}

	obj = item_builder.obj();
	return 0;
}

int DB_Operator::load_item_equip_detail(const mongo::BSONObj &obj, Equip_Addon_Detail &equip) {
	equip.reset();
	equip.unban_lvl = obj[Package_Fields::Item::UNBAN_LVL].numberInt();
	equip.score = obj[Package_Fields::Item::SCORE].numberInt();
	equip.force = obj[Package_Fields::Item::FORCE].numberInt();

	equip.incr_level = obj[Package_Fields::Item::INCR_LEVEL].numberInt();
	equip.incr_luck_value = obj[Package_Fields::Item::INCR_LUCK_VALUE].numberInt();
	equip.incr_lvl_conti_fail_cnt = obj[Package_Fields::Item::INCR_LVL_CONTI_FAIL_CNT].numberInt();

	equip.enchant_times = obj[Package_Fields::Item::ENCHANT_TIMES].numberInt();
	equip.cnt_enchant_group = obj[Package_Fields::Item::CNT_ENCHANT_GROUP].numberInt();
	uint8_t idx_enchant_groups = 0;
	BSONObjIterator it_enchant_groups(obj.getObjectField(Package_Fields::Item::ENCHANT_GROUPS.c_str()));
	while (it_enchant_groups.more()) {
		if (idx_enchant_groups >= equip.cnt_enchant_group || idx_enchant_groups >= Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP) {
			break;
		}
		BSONObj obj_group = it_enchant_groups.next().embeddedObject();
		equip.enchant_groups[idx_enchant_groups].score = obj_group[Package_Fields::Item::Enchant_Groups::SCORE].numberInt();
		equip.enchant_groups[idx_enchant_groups].cnt_enchant_prop = obj_group[Package_Fields::Item::Enchant_Groups::CNT_ENCHANT_PROP].numberInt();

		uint8_t idx_enchant_props = 0;
		BSONObjIterator it_enchant_props(obj_group.getObjectField(Package_Fields::Item::Enchant_Groups::ENCHANT_PROPS.c_str()));
		while (it_enchant_props.more()) {
			if (idx_enchant_props >= equip.enchant_groups[idx_enchant_groups].cnt_enchant_prop ||
					idx_enchant_props >= Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP) {
				break;
			}
			BSONObj obj_prop = it_enchant_props.next().embeddedObject();
			equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_type =
					obj_prop[Package_Fields::Item::Enchant_Groups::Enchant_Props::PROP_TYPE].numberInt();
			equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_val =
					obj_prop[Package_Fields::Item::Enchant_Groups::Enchant_Props::PROP_VAL].numberInt();
			equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].star =
					obj_prop[Package_Fields::Item::Enchant_Groups::Enchant_Props::STAR].numberInt();
			equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].is_lock =
					obj_prop[Package_Fields::Item::Enchant_Groups::Enchant_Props::IS_LOCK].numberInt();

			++idx_enchant_props;
		}
		++idx_enchant_groups;
	}

	uint8_t idx_prop_fail = 0;
	BSONObjIterator it_prop_fail(obj.getObjectField(Package_Fields::Item::ENCHANT_LOCK_PROP_FAIL.c_str()));
	while (it_prop_fail.more()) {
		if (idx_prop_fail >=  Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP) {
			break;
		}
		BSONObj obj_fail = it_prop_fail.next().embeddedObject();
		equip.enchant_lock_prop_fail[idx_prop_fail].prop = obj_fail[Package_Fields::Item::Enchant_Lock_Prop_Fail::PROP].numberInt();
		equip.enchant_lock_prop_fail[idx_prop_fail].fail_cnt = obj_fail[Package_Fields::Item::Enchant_Lock_Prop_Fail::FAIL_CNT].numberInt();

		++idx_prop_fail;
	}

	BSONObjIterator it_card_hole(obj.getObjectField(Package_Fields::Item::CARD_HOLE.c_str()));
	while(it_card_hole.more()) {
		BSONObj obj_card_hole = it_card_hole.next().embeddedObject();
		int loc = obj_card_hole[Package_Fields::Item::Card_Hole::LOC].numberInt();
		int card_index = obj_card_hole[Package_Fields::Item::Card_Hole::CARD_INDEX].numberInt();
		int8_t state = obj_card_hole[Package_Fields::Item::Card_Hole::STATE].numberInt();
		if (loc < Equip_Addon_Detail::MAX_CARD_HOLE) {
			equip.card_hole_array_[loc].state = state;//Hole_State::STATE_OPEN;
			equip.card_hole_array_[loc].card_index = card_index;
		}
	}

	BSONObjIterator it_stone_hole(obj.getObjectField(Package_Fields::Item::STONE.c_str()));
	while(it_stone_hole.more()) {
		BSONObj obj_stone_hole = it_stone_hole.next().embeddedObject();
		int idx = obj_stone_hole[Package_Fields::Item::Stone_Hole::STONE_HOLE_INDEX].numberInt();
		int8_t state = obj_stone_hole[Package_Fields::Item::Stone_Hole::STONE_HOLE_STATE].numberInt();
		int stone_id = obj_stone_hole[Package_Fields::Item::Stone_Hole::STONE_ID].numberInt();
		if (idx < Equip_Addon_Detail::MAX_GEMSTONE_HOLE) {
			equip.gemstones[idx].state = state;
			equip.gemstones[idx].stone_id = stone_id;
		}
	}

	return 0;
}

int DB_Operator::load_item_card_detail(const mongo::BSONObj &obj, Card_Addon_Detail &card) {
	card.reset();

	card.part_ = obj[Package_Fields::Item::CARD_PART].numberInt();
	card.poten_ = obj[Package_Fields::Item::CARD_POTEN].numberInt();
	card.star_ = obj[Package_Fields::Item::CARD_STAR].numberInt();
	card.lv_ = obj[Package_Fields::Item::CARD_LV].numberInt();
	card.exp_ = obj[Package_Fields::Item::CARD_EXP].numberInt();
	card.equip_id = obj[Package_Fields::Item::CARD_EQUIP_INDEX].numberInt();
	card.equip_loc = obj[Package_Fields::Item::CARD_EQUIP_LOC].numberInt();
	card.main_attr_ = obj[Package_Fields::Item::CARD_MAIN_ATTR].numberInt();
	card.sub_attr_ = obj[Package_Fields::Item::CARD_SUB_ATTR].numberInt();
	return 0;
}

int DB_Operator::save_item_equip_detail(const Equip_Addon_Detail &equip, mongo::BSONObj &obj, const Packer_Detail& packer_detail) {
	// 附魔
	std::vector<BSONObj> vec_enchant_group;
	std::vector<BSONObj> vec_ecchant_prop;
	for (uint8_t idx_enchant_groups = 0;
			idx_enchant_groups < equip.cnt_enchant_group &&
			idx_enchant_groups < Equip_Addon_Detail::MAX_ENCHANT_PROP_GROUP;
			++idx_enchant_groups) {

		vec_ecchant_prop.clear();
		for (uint8_t idx_enchant_props = 0;
				idx_enchant_props < equip.enchant_groups[idx_enchant_groups].cnt_enchant_prop &&
				idx_enchant_props < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP;
				++idx_enchant_props) {
			vec_ecchant_prop.push_back(BSON(Package_Fields::Item::Enchant_Groups::Enchant_Props::PROP_TYPE
					<< equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_type
					<< Package_Fields::Item::Enchant_Groups::Enchant_Props::PROP_VAL
					<< equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].prop_val
					<< Package_Fields::Item::Enchant_Groups::Enchant_Props::STAR
					<< equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].star
					<< Package_Fields::Item::Enchant_Groups::Enchant_Props::IS_LOCK
					<< equip.enchant_groups[idx_enchant_groups].enchant_props[idx_enchant_props].is_lock
					));
		}
		vec_enchant_group.push_back(BSON(Package_Fields::Item::Enchant_Groups::SCORE
				<< equip.enchant_groups[idx_enchant_groups].score
				<< Package_Fields::Item::Enchant_Groups::CNT_ENCHANT_PROP
				<< equip.enchant_groups[idx_enchant_groups].cnt_enchant_prop
				<< Package_Fields::Item::Enchant_Groups::ENCHANT_PROPS << vec_ecchant_prop
				));
	}

	std::vector<BSONObj> vec_enchant_fail;
	for (uint8_t idx_prop_fail = 0;	idx_prop_fail < Enchant_Prop_Group_Detail::MAX_ENCHANT_PROP; ++idx_prop_fail) {
		vec_enchant_fail.push_back(BSON(Package_Fields::Item::Enchant_Lock_Prop_Fail::PROP
				<< equip.enchant_lock_prop_fail[idx_prop_fail].prop
				<< Package_Fields::Item::Enchant_Lock_Prop_Fail::FAIL_CNT
				<< equip.enchant_lock_prop_fail[idx_prop_fail].fail_cnt
				));
	}

	std::vector<BSONObj> vec_card_hole;
	for (int i = 0; i < Equip_Addon_Detail::MAX_CARD_HOLE; ++i) {
		vec_card_hole.push_back(BSON(Package_Fields::Item::Card_Hole::LOC << i
				<< Package_Fields::Item::Card_Hole::STATE << equip.card_hole_array_[i].state
				<< Package_Fields::Item::Card_Hole::CARD_INDEX << equip.card_hole_array_[i].card_index
				));
	}

	std::vector<BSONObj> vec_stone_hole;
	for (int i = 0; i < Equip_Addon_Detail::MAX_GEMSTONE_HOLE; ++i) {
		vec_stone_hole.push_back(BSON(Package_Fields::Item::Stone_Hole::STONE_HOLE_INDEX << i
				<< Package_Fields::Item::Stone_Hole::STONE_HOLE_STATE << equip.gemstones[i].state
				<< Package_Fields::Item::Stone_Hole::STONE_ID <<  equip.gemstones[i].stone_id
				));
	}

	BSONObjBuilder equip_builder;
	equip_builder << Package_Fields::Item::UNBAN_LVL << equip.unban_lvl
			<< Package_Fields::Item::SCORE << equip.score
			<< Package_Fields::Item::FORCE << equip.force
			<< Package_Fields::Item::INCR_LEVEL << equip.incr_level
			<< Package_Fields::Item::INCR_LUCK_VALUE << equip.incr_luck_value
			<< Package_Fields::Item::INCR_LVL_CONTI_FAIL_CNT << equip.incr_lvl_conti_fail_cnt
			<< Package_Fields::Item::CNT_ENCHANT_GROUP << equip.cnt_enchant_group
			<< Package_Fields::Item::ENCHANT_TIMES <<  equip.enchant_times
			<< Package_Fields::Item::ENCHANT_GROUPS << vec_enchant_group
			<< Package_Fields::Item::ENCHANT_LOCK_PROP_FAIL << vec_enchant_fail
			<< Package_Fields::Item::CARD_HOLE << vec_card_hole
			<< Package_Fields::Item::STONE <<  vec_stone_hole
			;

	std::vector<BSONObj> card_info_vec;
	card_info_vec.clear();
	int size = sizeof(equip.card_hole_array_) / sizeof(equip.card_hole_array_[0]);
	for(int i=0; i<size; ++i) {
		int card_idx = equip.card_hole_array_[i].card_index;
		if (0 == card_idx) continue;
		Goods_Map::const_iterator find_it = packer_detail.goods_map.find(card_idx);
		if (packer_detail.goods_map.end() == find_it) continue;
		if (Item_Detail::CARD != find_it->second->type_) continue;

		int id = find_it->second->id_;
		int star = find_it->second->addon.card.star_;
		int lv = find_it->second->addon.card.lv_;
		int poten = find_it->second->addon.card.poten_;
		int main_id = find_it->second->addon.card.main_attr_;
		int sub_id = find_it->second->addon.card.sub_attr_;
		double main_value = handle_get_attr_value(id, main_id, star, lv, poten);
		double sub_value = handle_get_attr_value(id, sub_id, star, lv, poten);
		card_info_vec.push_back( BSON(
				Ranking_Fields::Item_Equip_Info::Card_Info::INDEX << find_it->second->index_
				<< Ranking_Fields::Item_Equip_Info::Card_Info::LV << lv
				<< Ranking_Fields::Item_Equip_Info::Card_Info::CUR_EXP << find_it->second->addon.card.exp_
				<< Ranking_Fields::Item_Equip_Info::Card_Info::ID << id
				<< Ranking_Fields::Item_Equip_Info::Card_Info::POTEN << poten
				<< Ranking_Fields::Item_Equip_Info::Card_Info::MAIN_ATTR << BSON(Ranking_Fields::KEY << main_id << Ranking_Fields::VALUE << main_value)
				<< Ranking_Fields::Item_Equip_Info::Card_Info::SUB_ATTR << BSON(Ranking_Fields::KEY << sub_id << Ranking_Fields::VALUE << sub_value)
				<< Ranking_Fields::Item_Equip_Info::Card_Info::STAR << star
				<< Ranking_Fields::Item_Equip_Info::Card_Info::PART << find_it->second->addon.card.part_
				<< Ranking_Fields::Item_Equip_Info::Card_Info::ITEM_ID << find_it->second->addon.card.equip_id
				<< Ranking_Fields::Item_Equip_Info::Card_Info::EQUIP_LOC << find_it->second->addon.card.equip_loc
		 ));
	}
	if (!card_info_vec.empty()) {
		equip_builder << Ranking_Fields::Item_Equip_Info::CARD_INFO << card_info_vec;
	}

	obj = equip_builder.obj();
	return 0;
}

int DB_Operator::load_item_rune_stone_detail(const mongo::BSONObj &obj, Rune_Stone_Addon_Detail &rune_stone) {
	rune_stone.reset();
	rune_stone.exp_lvl = obj[Package_Fields::Item::EXP_LVL].numberInt();
	rune_stone.exp = obj[Package_Fields::Item::EXP].numberInt();
	rune_stone.is_lock = obj[Package_Fields::Item::IS_LOCK].numberInt();
	return 0;
}

int DB_Operator::save_item_rune_stone_detail(const Rune_Stone_Addon_Detail &rune_stone, mongo::BSONObj &obj) {
	BSONObjBuilder rune_stone_builder;
	rune_stone_builder << Package_Fields::Item::EXP_LVL << rune_stone.exp_lvl
			<< Package_Fields::Item::EXP << rune_stone.exp
			<< Package_Fields::Item::IS_LOCK << rune_stone.is_lock
			;
	obj = rune_stone_builder.obj();
	return 0;
}

int DB_Operator::save_item_card_detail(const Card_Addon_Detail &card, mongo::BSONObj &obj) {

	BSONObjBuilder builder;
	builder << Package_Fields::Item::CARD_PART << card.part_
			<< Package_Fields::Item::CARD_POTEN << card.poten_
			<< Package_Fields::Item::CARD_STAR << card.star_
			<< Package_Fields::Item::CARD_LV << card.lv_
			<< Package_Fields::Item::CARD_EXP << card.exp_
			<< Package_Fields::Item::CARD_EQUIP_INDEX << card.equip_id
			<< Package_Fields::Item::CARD_EQUIP_LOC << card.equip_loc
			<< Package_Fields::Item::CARD_MAIN_ATTR << card.main_attr_
			<< Package_Fields::Item::CARD_SUB_ATTR << card.sub_attr_;

	obj = builder.obj();

	return 0;
}

//int DB_Operator::update_activation_code_info(const std::string &code) {
//	BSONObjBuilder tmp_builder;
//	tmp_builder << Activation_Code_Fields::STATUS << 1;
//
//	MONGO_CONNECTION.update(Activation_Code_Fields::COLLECTION, QUERY(Activation_Code_Fields::CODE << code),
//		BSON("$set" << tmp_builder.obj()));
//	return 0;
//}

int DB_Operator::load_all_activatiion_code_key(String_Set &keys) {
	keys.clear();
	unsigned long long res_count = MONGO_CONNECTION.count(Activation_Code_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< Activation_Code_Fields::KEY << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Activation_Code_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		std::string key = obj[Activation_Code_Fields::KEY].valuestrsafe();
		keys.insert(key);
	}
	return 0;
}

int DB_Operator::load_all_activation_receive_id(Long_Id_Set &ids) {
	ids.clear();
	unsigned long long res_count = MONGO_CONNECTION.count(Activation_Receive_Id_Fields::COLLECTION);
	vector<BSONObj> res;
	BSONObjBuilder field_builder;
	field_builder 	<< Activation_Receive_Id_Fields::ID << 1;
	BSONObj field_bson = field_builder.obj();
	vector<BSONObj> iter_record;
	iter_record.reserve(res_count);
	MONGO_CONNECTION.findN(iter_record, Activation_Receive_Id_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);
	for (vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj &obj = *iter;
		uint32_t id = obj[Activation_Receive_Id_Fields::ID].numberLong();
		ids.insert(id);
	}
	return 0;
}

int DB_Operator::save_activation_code_receive_id(long_id_t id) {
	BSONObjBuilder builder;
	builder << Activation_Receive_Id_Fields::ID << (long long int)id;
	MONGO_CONNECTION.insert(Activation_Receive_Id_Fields::COLLECTION, builder.obj());
	return 0;
}

//int DB_Operator::query_activation_code_info(Activation_Code_Info &code_info, const std::string &code) {
//	BSONObj res = MONGO_CONNECTION.findOne(Activation_Code_Fields::COLLECTION, QUERY(Activation_Code_Fields::CODE << code));
//	code_info.reset();
//	if (res.isEmpty())
//		return 0;
//
//	code_info.type = res[Activation_Code_Fields::TYPE].numberInt();
//	code_info.status = res[Activation_Code_Fields::STATUS].numberInt();
//	code_info.code = res[Activation_Code_Fields::CODE].valuestrsafe();
//	return 0;
//}

int DB_Operator::save_sgl_dungeon_detail(Sgl_Dungeon_Detail &detail) {
	BSONObjBuilder builder;
	vector<BSONObj> vc_dungeon;
	vector<BSONObj> vc_nor_elite;
	vector<BSONObj> vc_hor_elite;
	vector<BSONObj> vc_award;
	vector<BSONObj> vc_wipe_scenes;
	vector<BSONObj> vc_wipe_infos;

	for (Sgl_Dungeon_Map::iterator it = detail.sgl_dungeon_map_.begin();
			it != detail.sgl_dungeon_map_.end(); ++it){
		vc_dungeon.push_back(BSON(Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DUNGEON_ID << (long long int)it->first
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TYPE << it->second.type_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::PROGRESS << it->second.progress_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TTL_COUNTER << it->second.ttl_counter_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DAILY_COUNTER << it->second.daily_counter_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_GOLD_NUM << it->second.add_gold_num
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_PROP_NUM << it->second.add_prop_num
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::CLEAR_MONSTER << it->second.clear_monster_count_));
	}

	for (Sgl_Dungeon_Map::iterator it = detail.nor_elite_map_.begin();
			it != detail.nor_elite_map_.end(); ++it){
		vc_nor_elite.push_back(BSON(Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DUNGEON_ID << (long long int)it->first
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TYPE << it->second.type_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::PROGRESS << it->second.progress_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TTL_COUNTER << it->second.ttl_counter_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DAILY_COUNTER << it->second.daily_counter_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_GOLD_NUM << it->second.add_gold_num
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_PROP_NUM << it->second.add_prop_num
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::CLEAR_MONSTER << it->second.clear_monster_count_));
	}

	for (Sgl_Dungeon_Map::iterator it = detail.hor_elite_map_.begin();
			it != detail.hor_elite_map_.end(); ++it){
		vc_hor_elite.push_back(BSON(Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DUNGEON_ID << (long long int)it->first
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TYPE << it->second.type_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::PROGRESS << it->second.progress_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TTL_COUNTER << it->second.ttl_counter_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DAILY_COUNTER << it->second.daily_counter_
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_GOLD_NUM << it->second.add_gold_num
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_PROP_NUM << it->second.add_prop_num
				<< Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::CLEAR_MONSTER << it->second.clear_monster_count_));
	}

	//chapter award
	for (std::vector<Int_Int>::iterator it = detail.chapter_award_vec.begin();
			it != detail.chapter_award_vec.end(); ++it){
		vc_award.push_back(BSON(Sgl_Dungeon_Fields::Chapter_Award::CHAPTER_ID << (int)it->val_1
				<< Sgl_Dungeon_Fields::Chapter_Award::AWARD_STATUS << (int)it->val_2));
	}

	//lord_trail_info
	vector<BSONObj> elite_items;
	for(Item_Info_Map::iterator it = detail.elite_trail_info.elite_items.begin();
			it != detail.elite_trail_info.elite_items.end(); ++it){
		elite_items.push_back(BSON(Sgl_Dungeon_Fields::Item_Info::ID << (int)it->second.id
				<< Sgl_Dungeon_Fields::Item_Info::AMOUNT << (int)it->second.amount
				<< Sgl_Dungeon_Fields::Item_Info::BIND << (int)it->second.bind
						));
	}
	BSONObj elite_trail_info_obj = BSON(Sgl_Dungeon_Fields::Trail_Info::ELITE_ITEMS << elite_items);

	//
	vector<BSONObj> lord_items;
	for(Item_Info_Map::iterator it = detail.lord_trail_info.elite_items.begin();
			it != detail.lord_trail_info.elite_items.end(); ++it){
		lord_items.push_back(BSON(Sgl_Dungeon_Fields::Item_Info::ID << (int)it->second.id
				<< Sgl_Dungeon_Fields::Item_Info::AMOUNT << (int)it->second.amount
				<< Sgl_Dungeon_Fields::Item_Info::BIND << (int)it->second.bind
						));
	}
	BSONObj lord_trail_info_obj = BSON(Sgl_Dungeon_Fields::Trail_Info::ELITE_ITEMS << lord_items);


	// wipe scene
	for(std::vector<int>::iterator it = detail.wipe_scenes.begin();
			it != detail.wipe_scenes.end(); ++it){
		vc_wipe_scenes.push_back(
				BSON(
						Sgl_Dungeon_Fields::Wipe_Scene::SCENE_ID << (int)(*it)
				)
			);
	}
	// wipe info
	for(std::vector<Wipe_Info>::iterator it = detail.wipe_infos.begin();
			it != detail.wipe_infos.end(); ++it){
		vector<BSONObj> vc_items;
		for(std::vector<Item_Info>::iterator s_it = it->items.begin();
				s_it != it->items.end(); ++s_it){
			vc_items.push_back(
					BSON(Sgl_Dungeon_Fields::Item_Info::ID << (int)s_it->id
					<< Sgl_Dungeon_Fields::Item_Info::AMOUNT << (int)s_it->amount
					<< Sgl_Dungeon_Fields::Item_Info::BIND << (int)s_it->bind
					)
				);
		}

		vc_wipe_infos.push_back(
				BSON(Sgl_Dungeon_Fields::Wipe_Info::INDEX << (int)it->index
				<< Sgl_Dungeon_Fields::Wipe_Info::SCENE_ID << (int)it->scene_id
				<< Sgl_Dungeon_Fields::Wipe_Info::EXP << (int)it->exp
				<< Sgl_Dungeon_Fields::Wipe_Info::COPPER << (int)it->copper
				<< Sgl_Dungeon_Fields::Wipe_Info::ITEMS << vc_items
				)
			);
	}

	builder << Sgl_Dungeon_Fields::ROLE_ID << (long long int)detail.role_id
			<< Sgl_Dungeon_Fields::SGLDUNGEONITEM << vc_dungeon
			<< Sgl_Dungeon_Fields::NOR_ELITE_ITEM << vc_nor_elite
			<< Sgl_Dungeon_Fields::HOR_ELITE_ITEM << vc_hor_elite
			<< Sgl_Dungeon_Fields::CHAPTER_AWARD_LIST << vc_award
			<< Sgl_Dungeon_Fields::TRAIL_INFO << elite_trail_info_obj
			<< Sgl_Dungeon_Fields::LORD_TRAIL_INFO << lord_trail_info_obj
			<< Sgl_Dungeon_Fields::SINGLE_VER << detail.single_ver
			<< Sgl_Dungeon_Fields::ELITE_VER << detail.elite_ver
			<< Sgl_Dungeon_Fields::LORD_VER << detail.lord_ver
			<< Sgl_Dungeon_Fields::SINGLE_COMPLETE_NUM << detail.single_complete_num
			<< Sgl_Dungeon_Fields::NOR_ELITE_COMPLETE_NUM << detail.nor_elite_complete_num
			<< Sgl_Dungeon_Fields::HOR_ELITE_COMPLETE_NUM << detail.hor_elite_complete_num
			<< Sgl_Dungeon_Fields::ELITE_RESET_TIMES << detail.elite_reset_times
			<< Sgl_Dungeon_Fields::ELITE_CH_NUM << detail.elite_ch_num
			<< Sgl_Dungeon_Fields::ELITE_ADD_NUM << detail.elite_add_num
			<< Sgl_Dungeon_Fields::ELITE_ADD_BUY_NUM << detail.elite_add_buy_num
			<< Sgl_Dungeon_Fields::ELITE_TASK_ADD_NUM << detail.elite_task_add_num
			<< Sgl_Dungeon_Fields::WIPE_TYPE << detail.wipe_type
			<< Sgl_Dungeon_Fields::WIPE_NUM << detail.wipe_num
			<< Sgl_Dungeon_Fields::WIPE_ELITE_DOUBLE << (int)detail.wipe_elite_double
			<< Sgl_Dungeon_Fields::WIPE_ELITE_DOUBLE_NOT_SELECTED << (int)detail.wipe_elite_double_not_selected
			<< Sgl_Dungeon_Fields::WIPE_BEGIN_TIME << (long long int)detail.wipe_begin_time.sec()
			<< Sgl_Dungeon_Fields::WIPE_SCENES << vc_wipe_scenes
			<< Sgl_Dungeon_Fields::WIPE_INFOS << vc_wipe_infos
			;

	MONGO_CONNECTION.update(Sgl_Dungeon_Fields::COLLECTION, QUERY(Sgl_Dungeon_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);
	return 0;
}

int DB_Operator::load_sgl_dungeon_detail(Sgl_Dungeon_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Sgl_Dungeon_Fields::COLLECTION,
			QUERY(Sgl_Dungeon_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty())
		return 0;
	// single
	{
		BSONObj objItems = res.getObjectField(Sgl_Dungeon_Fields::SGLDUNGEONITEM.c_str());
		BSONObjIterator items_it(objItems);
		while (items_it.more()) {
			BSONObj item_obj = items_it.next().embeddedObject();
			Sgl_Dungeon_Data data;
			data.dungeon_id_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DUNGEON_ID].numberLong();
			data.type_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TYPE].numberInt();
			data.progress_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::PROGRESS].numberInt();

			data.ttl_counter_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TTL_COUNTER].numberInt();
			data.daily_counter_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DAILY_COUNTER].numberInt();
			data.add_gold_num = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_GOLD_NUM].numberInt();
			data.add_prop_num = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_PROP_NUM].numberInt();
			data.clear_monster_count_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::CLEAR_MONSTER].numberInt();
			detail.sgl_dungeon_map_.insert(Sgl_Dungeon_Map::value_type(data.dungeon_id_, data));
		}
	}

	// nor elite
	{
		BSONObj objItems = res.getObjectField(Sgl_Dungeon_Fields::NOR_ELITE_ITEM.c_str());
		BSONObjIterator items_it(objItems);
		while (items_it.more()) {
			BSONObj item_obj = items_it.next().embeddedObject();
			Sgl_Dungeon_Data data;
			data.dungeon_id_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DUNGEON_ID].numberLong();
			data.type_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TYPE].numberInt();
			data.progress_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::PROGRESS].numberInt();

			data.ttl_counter_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TTL_COUNTER].numberInt();
			data.daily_counter_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DAILY_COUNTER].numberInt();
			data.add_gold_num = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_GOLD_NUM].numberInt();
			data.add_prop_num = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_PROP_NUM].numberInt();
			data.clear_monster_count_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::CLEAR_MONSTER].numberInt();
			detail.nor_elite_map_.insert(Sgl_Dungeon_Map::value_type(data.dungeon_id_, data));
		}
	}
	// hor elite
	{
		BSONObj objItems = res.getObjectField(Sgl_Dungeon_Fields::HOR_ELITE_ITEM.c_str());
		BSONObjIterator items_it(objItems);
		while (items_it.more()) {
			BSONObj item_obj = items_it.next().embeddedObject();
			Sgl_Dungeon_Data data;
			data.dungeon_id_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DUNGEON_ID].numberLong();
			data.type_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TYPE].numberInt();
			data.progress_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::PROGRESS].numberInt();

			data.ttl_counter_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::TTL_COUNTER].numberInt();
			data.daily_counter_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::DAILY_COUNTER].numberInt();
			data.add_gold_num = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_GOLD_NUM].numberInt();
			data.add_prop_num = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::ADD_PROP_NUM].numberInt();
			data.clear_monster_count_ = item_obj[Sgl_Dungeon_Fields::Sgl_Dungeon_Fields_Item::CLEAR_MONSTER].numberInt();
			detail.hor_elite_map_.insert(Sgl_Dungeon_Map::value_type(data.dungeon_id_, data));
		}
	}
	//chapter award

	BSONObj obj_awards = res.getObjectField(Sgl_Dungeon_Fields::CHAPTER_AWARD_LIST.c_str());
	BSONObjIterator awards_it(obj_awards);
	while (awards_it.more()) {
		BSONObj item_obj = awards_it.next().embeddedObject();
		Int_Int data;
		data.val_1 = item_obj[Sgl_Dungeon_Fields::Chapter_Award::CHAPTER_ID].numberInt();
		data.val_2 = item_obj[Sgl_Dungeon_Fields::Chapter_Award::AWARD_STATUS].numberInt();
		detail.chapter_award_vec.push_back(data);
	}
	// elite_trail_info
	BSONObj elite_trail_info_obj = res.getObjectField(Sgl_Dungeon_Fields::TRAIL_INFO.c_str());
	if(elite_trail_info_obj.isEmpty() == false){
		// items
		BSONObj obj_items = elite_trail_info_obj.getObjectField(Sgl_Dungeon_Fields::Trail_Info::ELITE_ITEMS.c_str());
		BSONObjIterator obj_items_it(obj_items);
		while (obj_items_it.more()) {
			BSONObj w_item_obj = obj_items_it.next().embeddedObject();
			Item_Info i_info;
			i_info.id = w_item_obj[Sgl_Dungeon_Fields::Item_Info::ID].numberInt();
			i_info.amount = w_item_obj[Sgl_Dungeon_Fields::Item_Info::AMOUNT].numberInt();
			i_info.bind = w_item_obj[Sgl_Dungeon_Fields::Item_Info::BIND].numberInt();
			detail.elite_trail_info.elite_items.insert(std::make_pair(i_info.id, i_info));
		}
	}

	// lord_trail_info
	BSONObj lord_trail_info_obj = res.getObjectField(Sgl_Dungeon_Fields::LORD_TRAIL_INFO.c_str());
	if(lord_trail_info_obj.isEmpty() == false){
		// items
		BSONObj obj_items = lord_trail_info_obj.getObjectField(Sgl_Dungeon_Fields::Trail_Info::ELITE_ITEMS.c_str());
		BSONObjIterator obj_items_it(obj_items);
		while (obj_items_it.more()) {
			BSONObj w_item_obj = obj_items_it.next().embeddedObject();
			Item_Info i_info;
			i_info.id = w_item_obj[Sgl_Dungeon_Fields::Item_Info::ID].numberInt();
			i_info.amount = w_item_obj[Sgl_Dungeon_Fields::Item_Info::AMOUNT].numberInt();
			i_info.bind = w_item_obj[Sgl_Dungeon_Fields::Item_Info::BIND].numberInt();
			detail.lord_trail_info.elite_items.insert(std::make_pair(i_info.id, i_info));
		}
	}

	detail.single_ver= res[Sgl_Dungeon_Fields::SINGLE_VER].numberInt();
	detail.elite_ver= res[Sgl_Dungeon_Fields::ELITE_VER].numberInt();
	detail.lord_ver= res[Sgl_Dungeon_Fields::LORD_VER].numberInt();
	detail.single_complete_num = res[Sgl_Dungeon_Fields::SINGLE_COMPLETE_NUM].numberInt();
	detail.nor_elite_complete_num = res[Sgl_Dungeon_Fields::NOR_ELITE_COMPLETE_NUM].numberInt();
	detail.hor_elite_complete_num = res[Sgl_Dungeon_Fields::HOR_ELITE_COMPLETE_NUM].numberInt();
	detail.elite_reset_times = res[Sgl_Dungeon_Fields::ELITE_RESET_TIMES].numberInt();
	detail.elite_ch_num = res[Sgl_Dungeon_Fields::ELITE_CH_NUM].numberInt();
	detail.elite_add_num = res[Sgl_Dungeon_Fields::ELITE_ADD_NUM].numberInt();
	detail.elite_add_buy_num = res[Sgl_Dungeon_Fields::ELITE_ADD_BUY_NUM].numberInt();
	detail.elite_task_add_num = res[Sgl_Dungeon_Fields::ELITE_TASK_ADD_NUM].numberInt();

	// wipe type
	detail.wipe_type = res[Sgl_Dungeon_Fields::WIPE_TYPE].numberInt();
	// wipe num
	detail.wipe_num = res[Sgl_Dungeon_Fields::WIPE_NUM].numberInt();
	// wipe elite double
	detail.wipe_elite_double = res[Sgl_Dungeon_Fields::WIPE_ELITE_DOUBLE].numberInt();
	detail.wipe_elite_double_not_selected = res[Sgl_Dungeon_Fields::WIPE_ELITE_DOUBLE_NOT_SELECTED].numberInt();
	// wipe begin tims
	detail.wipe_begin_time.sec(res[Sgl_Dungeon_Fields::WIPE_BEGIN_TIME].numberInt());
	// wipe scenes
	BSONObj obj_wipe_scenes = res.getObjectField(Sgl_Dungeon_Fields::WIPE_SCENES.c_str());
	BSONObjIterator obj_wipe_scenes_it(obj_wipe_scenes);
	while (obj_wipe_scenes_it.more()) {
		BSONObj wipe_scene_obj = obj_wipe_scenes_it.next().embeddedObject();
		int scene_id = wipe_scene_obj[Sgl_Dungeon_Fields::Wipe_Scene::SCENE_ID].numberInt();
		detail.wipe_scenes.push_back(scene_id);
	}

	// wipe infos
	BSONObj obj_wipe_infos = res.getObjectField(Sgl_Dungeon_Fields::WIPE_INFOS.c_str());
	BSONObjIterator obj_wipe_infos_it(obj_wipe_infos);
	while (obj_wipe_infos_it.more()) {
		BSONObj wipe_info_obj = obj_wipe_infos_it.next().embeddedObject();
		Wipe_Info w_info;
		w_info.scene_id = wipe_info_obj[Sgl_Dungeon_Fields::Wipe_Info::SCENE_ID].numberInt();
		w_info.index = wipe_info_obj[Sgl_Dungeon_Fields::Wipe_Info::INDEX].numberInt();
		w_info.exp = wipe_info_obj[Sgl_Dungeon_Fields::Wipe_Info::EXP].numberInt();
		w_info.copper = wipe_info_obj[Sgl_Dungeon_Fields::Wipe_Info::COPPER].numberInt();
		// items
		BSONObj obj_w_items = wipe_info_obj.getObjectField(Sgl_Dungeon_Fields::Wipe_Info::ITEMS.c_str());
		BSONObjIterator obj_w_items_it(obj_w_items);
		while (obj_w_items_it.more()) {
			BSONObj w_item_obj = obj_w_items_it.next().embeddedObject();
			Item_Info i_info;
			i_info.id = w_item_obj[Sgl_Dungeon_Fields::Item_Info::ID].numberInt();
			i_info.amount = w_item_obj[Sgl_Dungeon_Fields::Item_Info::AMOUNT].numberInt();
			i_info.bind = w_item_obj[Sgl_Dungeon_Fields::Item_Info::BIND].numberInt();
			w_info.items.push_back(i_info);
		}

		detail.wipe_infos.push_back(w_info);
	}

	// 单人副本中途插副本容错处理
	bool single_ver_change = false;
	int single_ver = CONFIG_CACHE_DUNGEON->get_single_ver();
	if(detail.single_ver != single_ver){
		single_ver_change = true;
		detail.single_ver = single_ver;
		int max_dungeon_index = 0;
		for(Sgl_Dungeon_Map::iterator it = detail.sgl_dungeon_map_.begin();
				it != detail.sgl_dungeon_map_.end(); ){
			int dungeon_index = CONFIG_CACHE_SCENE->get_single_dungeon_relation_index(it->second.dungeon_id_);
			if(dungeon_index <= 0){// 废除的副本
				it = detail.sgl_dungeon_map_.erase(it);
			}else{//
				if(dungeon_index > max_dungeon_index){
					max_dungeon_index = dungeon_index;
				}
				++it;
			}
		}
		int complete_num = 0;
		int id_index = 0;
		Int_Vec dungeon_ids = CONFIG_CACHE_SCENE->get_all_single_dungeon_id();
		for(Int_Vec::iterator it = dungeon_ids.begin(); it != dungeon_ids.end(); ++it){
			id_index += 1;
			if(id_index > max_dungeon_index){
				break;
			}
			int scene_id = *it;
			Sgl_Dungeon_Map::iterator find_it = detail.sgl_dungeon_map_.find(scene_id);
			if(find_it == detail.sgl_dungeon_map_.end()){// 新增副本 直接让通关
				Sgl_Dungeon_Data data;
				data.dungeon_id_ = scene_id;
				data.clear_monster_count_ = 1;
				data.progress_ = Sgl_Dungeon_Data::COMPLETED;
				detail.sgl_dungeon_map_[data.dungeon_id_] = data;
			}else{//
				if(id_index < max_dungeon_index){//
					find_it->second.progress_ = Sgl_Dungeon_Data::COMPLETED;
				}else if(id_index == max_dungeon_index){
					if(find_it->second.progress_ == Sgl_Dungeon_Data::COMPLETED){// 最后一关打通的，开启下一关
						complete_num = max_dungeon_index;
						const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(find_it->second.dungeon_id_);
						if (cfg && cfg->dungeon.next_id != 0) {
							Sgl_Dungeon_Data data;
							data.dungeon_id_ = cfg->dungeon.next_id;
							data.progress_ = Sgl_Dungeon_Data::NOT_COMPLETED;
							detail.sgl_dungeon_map_[data.dungeon_id_] = data;
						}
					}else{
						complete_num = max_dungeon_index - 1;
					}
				}
			}
		}
		//
		detail.single_complete_num = complete_num;
		if(detail.single_complete_num < 0){
			detail.single_complete_num = 0;
		}
		detail.detail_change();
	}
	//
	// 上古副本中途插副本容错处理
	bool elite_change = false;
	int elite_ver = CONFIG_CACHE_DUNGEON->get_elite_ver();
	if(single_ver_change || detail.elite_ver != elite_ver){
		elite_change = true;
		detail.elite_ver = elite_ver;
		int max_dungeon_index = 0;
		for(Sgl_Dungeon_Map::iterator it = detail.nor_elite_map_.begin();
				it != detail.nor_elite_map_.end(); ){
			int dungeon_index = CONFIG_CACHE_SCENE->get_elite_dungeon_relation_index(it->second.dungeon_id_, 1);
			if(dungeon_index <= 0){// 废除的副本
				it = detail.nor_elite_map_.erase(it);
			}else{//
				if(dungeon_index > max_dungeon_index){
					max_dungeon_index = dungeon_index;
				}
				++it;
			}
		}
		int complete_num = 0;
		int id_index = 0;
		Int_Vec dungeon_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(1);
		for(Int_Vec::iterator it = dungeon_ids.begin(); it != dungeon_ids.end(); ++it){
			id_index += 1;
			if(id_index > max_dungeon_index){
				break;
			}
			int scene_id = *it;
			Sgl_Dungeon_Map::iterator find_it = detail.nor_elite_map_.find(scene_id);
			if(find_it == detail.nor_elite_map_.end()){// 新增副本 直接让通关
				Sgl_Dungeon_Data data;
				data.dungeon_id_ = scene_id;
				data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
				detail.nor_elite_map_[data.dungeon_id_] = data;
			}else{//
				if(id_index < max_dungeon_index){//
					find_it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
				}else if(id_index == max_dungeon_index){
					if(find_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP ||
							find_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){// 最后一关打通的，开启下一关
						complete_num = max_dungeon_index;
						const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(find_it->second.dungeon_id_);
						if (cfg && cfg->dungeon.next_id != 0) {
							Sgl_Dungeon_Data data;
							data.dungeon_id_ = cfg->dungeon.next_id;
							bool b_sgl_complete = false;
							int next_single_id = 0;
							const Scene_Config* cfg_next = CONFIG_CACHE_SCENE->scene_config_cache(
									data.dungeon_id_);
							if (cfg_next) {
								next_single_id = cfg_next->dungeon.elite;
							}
							Sgl_Dungeon_Map::iterator next_single_it = detail.sgl_dungeon_map_.find(next_single_id);
							if (next_single_it != detail.sgl_dungeon_map_.end()) {
								if (next_single_it->second.progress_ == Sgl_Dungeon_Data::COMPLETED) {
									b_sgl_complete = true;
								}
							}
							if(b_sgl_complete){//
								data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
							}else{//
								data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK;
							}
							detail.nor_elite_map_[data.dungeon_id_] = data;
						}
					}else{
						complete_num = max_dungeon_index - 1;
						bool b_sgl_complete = false;
						int next_single_id = 0;
						const Scene_Config* cfg_next = CONFIG_CACHE_SCENE->scene_config_cache(
								find_it->second.dungeon_id_);
						if (cfg_next) {
							next_single_id = cfg_next->dungeon.elite;
						}
						Sgl_Dungeon_Map::iterator next_single_it = detail.sgl_dungeon_map_.find(next_single_id);
						if (next_single_it != detail.sgl_dungeon_map_.end()) {
							if (next_single_it->second.progress_ == Sgl_Dungeon_Data::COMPLETED) {
								b_sgl_complete = true;
							}
						}
						if(b_sgl_complete){//
							find_it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
						}else{//
							find_it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK;
						}
					}
				}
			}
		}
		//
		detail.nor_elite_complete_num = complete_num;
		if(detail.nor_elite_complete_num < 0){
			detail.nor_elite_complete_num = 0;
		}
		detail.detail_change();
	}

	//
	// 魔神殿中途插副本容错处理
	int lord_ver = CONFIG_CACHE_DUNGEON->get_lord_temple_ver();
	if(elite_change || detail.lord_ver != lord_ver){
		detail.lord_ver = lord_ver;
		int max_dungeon_index = 0;
		for(Sgl_Dungeon_Map::iterator it = detail.hor_elite_map_.begin();
				it != detail.hor_elite_map_.end(); ){
			int dungeon_index = CONFIG_CACHE_SCENE->get_elite_dungeon_relation_index(it->second.dungeon_id_, 2);
			if(dungeon_index <= 0){// 废除的副本
				it = detail.hor_elite_map_.erase(it);
			}else{//
				if(dungeon_index > max_dungeon_index){
					max_dungeon_index = dungeon_index;
				}
				++it;
			}
		}
		int complete_num = 0;
		int id_index = 0;
		Int_Vec dungeon_ids = CONFIG_CACHE_SCENE->get_all_elite_dungeon_id(2);
		for(Int_Vec::iterator it = dungeon_ids.begin(); it != dungeon_ids.end(); ++it){
			id_index += 1;
			if(id_index > max_dungeon_index){
				break;
			}
			int scene_id = *it;
			Sgl_Dungeon_Map::iterator find_it = detail.hor_elite_map_.find(scene_id);
			if(find_it == detail.hor_elite_map_.end()){// 新增副本 直接让通关
				Sgl_Dungeon_Data data;
				data.dungeon_id_ = scene_id;
				data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
				detail.hor_elite_map_[data.dungeon_id_] = data;
			}else{//
				if(id_index < max_dungeon_index){//
					find_it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_REAP;
				}else if(id_index == max_dungeon_index){
					if(find_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP ||
							find_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP){// 最后一关打通的，开启下一关
						complete_num = max_dungeon_index;
						const Scene_Config* cfg = CONFIG_CACHE_SCENE->scene_config_cache(find_it->second.dungeon_id_);
						if (cfg && cfg->dungeon.next_id != 0) {
							Sgl_Dungeon_Data data;
							data.dungeon_id_ = cfg->dungeon.next_id;
							data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
							detail.hor_elite_map_[data.dungeon_id_] = data;
						}
					}else{
						complete_num = max_dungeon_index - 1;
						bool b_sgl_complete = false;
						if(find_it->second.dungeon_id_ == CONFIG_CACHE_ROLE->get_elite_first_id(2)){
							int open_id = CONFIG_CACHE_DUNGEON->get_lord_open_id();
							Sgl_Dungeon_Map::iterator open_it = detail.nor_elite_map_.find(open_id);
							if (open_it != detail.nor_elite_map_.end()) {
								if(open_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP ||
										open_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP ){
									b_sgl_complete = true;
								}
							}
						}else{
							b_sgl_complete = true;
						}
						if(b_sgl_complete){//
							find_it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
						}else{//
							find_it->second.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK;
						}
					}
				}
			}
		}
		//
		detail.hor_elite_complete_num = complete_num;
		if(detail.hor_elite_complete_num < 0){
			detail.hor_elite_complete_num = 0;
		}
		//
		if(detail.hor_elite_map_.size() == 0){//没有激活
			bool b_sgl_complete = false;
			int open_id = CONFIG_CACHE_DUNGEON->get_lord_open_id();
			Sgl_Dungeon_Map::iterator open_it = detail.nor_elite_map_.find(open_id);
			if (open_it != detail.nor_elite_map_.end()) {
				if(open_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_REAP ||
						open_it->second.progress_ == Sgl_Dungeon_Data::ELITE_DUNGEON_UNREAP ){
					b_sgl_complete = true;
				}
			}
			Sgl_Dungeon_Data data;
			data.dungeon_id_ = CONFIG_CACHE_ROLE->get_elite_first_id(2);
			if(b_sgl_complete){//
				data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_ACTIVE;
			}else{//
				data.progress_ = Sgl_Dungeon_Data::ELITE_DUNGEON_LOCK;
			}
			detail.hor_elite_map_[data.dungeon_id_] = data;
		}
		detail.detail_change();
	}
	return 0;
}

int DB_Operator::load_heroer_detail(Heroer_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Heroer_Fields::COLLECTION, QUERY(Heroer_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Heroer_Fields::ROLE_ID].numberLong();
	detail.outer_index = res[Heroer_Fields::OUTER_INDEX].numberInt();
	detail.fighter_index = res[Heroer_Fields::FIGHTER_INDEX].numberInt();
	detail.last_fighter_idx = res[Heroer_Fields::LAST_FIGHTER_IDX].numberInt();

	BSONObjIterator it_clicked_heros(res.getObjectField(Heroer_Fields::CLICKED_NEW_HEROS.c_str()));
	while (it_clicked_heros.more()) {
		int clicked_new_hero_id = it_clicked_heros.next().numberInt();
		if (clicked_new_hero_id) {
			detail.clicked_new_heros.insert(clicked_new_hero_id);
		}
	}

	BSONObjIterator it_assist_heros(res.getObjectField(Heroer_Fields::ASSIST_HEROS.c_str()));
	while(it_assist_heros.more()) {
		BSONObj obj_assist_info = it_assist_heros.next().embeddedObject();
		int loc = obj_assist_info[Heroer_Fields::Assist_Hero::LOC].numberInt();
		int hero_idx = obj_assist_info[Heroer_Fields::Assist_Hero::HERO_IDX].numberInt();
		detail.hero_assist_map[loc] = hero_idx;
	}

	Hero_Info_Detail hero_info_detail;
	BSONObjIterator it_hero_info(res.getObjectField(Heroer_Fields::HERO_INFOS.c_str()));
	while (it_hero_info.more()) {
		BSONObj obj_hero_info = it_hero_info.next().embeddedObject();
		hero_info_detail.reset();
		hero_info_detail.hero_id = obj_hero_info[Heroer_Fields::Hero_Info::HERO_ID].numberInt();
		hero_info_detail.fight_date = obj_hero_info[Heroer_Fields::Hero_Info::FIGHT_DATE].numberInt();
		hero_info_detail.hero_index = obj_hero_info[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt();
		hero_info_detail.hero_facade_id = obj_hero_info[Heroer_Fields::Hero_Info::HERO_FACADE_ID].numberInt();
		hero_info_detail.hero_name = obj_hero_info[Heroer_Fields::Hero_Info::HERO_NAME].valuestrsafe();
		hero_info_detail.is_outer = obj_hero_info[Heroer_Fields::Hero_Info::IS_OUTER].numberInt();
		hero_info_detail.is_fighter = obj_hero_info[Heroer_Fields::Hero_Info::IS_FIGHTER].numberInt();
		hero_info_detail.level = obj_hero_info[Heroer_Fields::Hero_Info::LEVEL].numberInt();
		hero_info_detail.exp = obj_hero_info[Heroer_Fields::Hero_Info::EXP].numberInt();
		hero_info_detail.formation = obj_hero_info[Heroer_Fields::Hero_Info::FORMATION].numberInt();
		hero_info_detail.awake_lvl = obj_hero_info[Heroer_Fields::Hero_Info::AWAKE_LVL].numberInt();
		hero_info_detail.is_task_tmp_hero = obj_hero_info[Heroer_Fields::Hero_Info::IS_TMP_HERO].numberInt();
		hero_info_detail.intimacy_lv = obj_hero_info[Heroer_Fields::Hero_Info::INTIMACY_LV].numberInt();
		hero_info_detail.intimacy_lv = hero_info_detail.intimacy_lv == 0 ? 1 : hero_info_detail.intimacy_lv;
		hero_info_detail.intimacy_exp = obj_hero_info[Heroer_Fields::Hero_Info::INTIMACY_EXP].numberInt();
		hero_info_detail.community_times = obj_hero_info[Heroer_Fields::Hero_Info::COMMUNITY_TIMES].numberInt();
		hero_info_detail.gift_times = obj_hero_info[Heroer_Fields::Hero_Info::GIFT_TIMES].numberInt();

		Artifact_Detail artifact_detail;
		BSONObjIterator it_artifact(obj_hero_info.getObjectField(Heroer_Fields::Hero_Info::ARTIFACTS.c_str()));
		while (it_artifact.more()) {
			BSONObj obj_artifact = it_artifact.next().embeddedObject();
			artifact_detail.reset();
			artifact_detail.artifact_id = obj_artifact[Heroer_Fields::Hero_Info::Artifact::ARTIFACT_ID].numberInt();
			artifact_detail.artifact_lvl = obj_artifact[Heroer_Fields::Hero_Info::Artifact::ARTIFACT_LVL].numberInt();
			hero_info_detail.artifacts.insert(std::make_pair(artifact_detail.artifact_id, artifact_detail));
		}

		detail.hero_map.insert(std::make_pair(hero_info_detail.hero_index,hero_info_detail));
	}

	load_heroer_status_detail(detail);
	return 0;
}

int DB_Operator::save_heroer_detail(Heroer_Detail &detail, bool save_arena) {
	Int_Vec clicked_new_heros(detail.clicked_new_heros.begin(), detail.clicked_new_heros.end());

	std::vector<BSONObj> hero_infos;
	for (Heroer_Detail::Hero_Map::iterator it = detail.hero_map.begin();
			it != detail.hero_map.end(); ++it) {

		std::vector<BSONObj> artifact_bson_vec;
		for (Artifact_Map::const_iterator it_artifact = it->second.artifacts.begin();
				it_artifact != it->second.artifacts.end(); ++it_artifact) {
			artifact_bson_vec.push_back(BSON(
					Heroer_Fields::Hero_Info::Artifact::ARTIFACT_ID << it_artifact->second.artifact_id
					<< Heroer_Fields::Hero_Info::Artifact::ARTIFACT_LVL << it_artifact->second.artifact_lvl
					));
		}

		BSONObjBuilder hero_info_builder;
		hero_info_builder << Heroer_Fields::Hero_Info::HERO_ID << it->second.hero_id
				<< Heroer_Fields::Hero_Info::FIGHT_DATE << it->second.fight_date
				<< Heroer_Fields::Hero_Info::HERO_INDEX << it->second.hero_index
				<< Heroer_Fields::Hero_Info::HERO_FACADE_ID << it->second.hero_facade_id
				<< Heroer_Fields::Hero_Info::HERO_NAME << it->second.hero_name
				<< Heroer_Fields::Hero_Info::IS_OUTER << it->second.is_outer
				<< Heroer_Fields::Hero_Info::IS_FIGHTER << it->second.is_fighter
				<< Heroer_Fields::Hero_Info::LEVEL << it->second.level
				<< Heroer_Fields::Hero_Info::EXP << it->second.exp
				<< Heroer_Fields::Hero_Info::FORMATION << it->second.formation
				<< Heroer_Fields::Hero_Info::AWAKE_LVL << it->second.awake_lvl
				<< Heroer_Fields::Hero_Info::IS_TMP_HERO << it->second.is_task_tmp_hero
				<< Heroer_Fields::Hero_Info::COMBAT_POWER << it->second.hero_fighter_detail.fetch_fight_property(PT_FORCE)
				<< Heroer_Fields::Hero_Info::INTIMACY_LV << it->second.intimacy_lv
				<< Heroer_Fields::Hero_Info::INTIMACY_EXP << it->second.intimacy_exp
				<< Heroer_Fields::Hero_Info::COMMUNITY_TIMES <<  it->second.community_times
				<< Heroer_Fields::Hero_Info::GIFT_TIMES << it->second.gift_times
				<< Heroer_Fields::Hero_Info::ARTIFACTS << artifact_bson_vec
				;

		if (save_arena) {
			BSONObj arena_fight_obj;
			if (it->second.hero_index == detail.fighter_index) {
				handle_save_fighter_detail(it->second.hero_fighter_detail, arena_fight_obj);
				hero_info_builder.append(Fighter_Fields::ARENA_FIGHT, arena_fight_obj);
			}
		}

		hero_infos.push_back(hero_info_builder.obj());
	}


	std::vector<BSONObj> assist_hero_vec;
	for (Int_Int_Map::iterator it = detail.hero_assist_map.begin(); it != detail.hero_assist_map.end(); ++it) {
		assist_hero_vec.push_back(BSON(
				Heroer_Fields::Assist_Hero::LOC << it->first <<
				Heroer_Fields::Assist_Hero::HERO_IDX << it->second));
	}

	BSONObjBuilder builder;
	builder << Heroer_Fields::ROLE_ID << (long long int)detail.role_id
			<< Heroer_Fields::OUTER_INDEX << detail.outer_index
			<< Heroer_Fields::FIGHTER_INDEX << detail.fighter_index
			<< Heroer_Fields::LAST_FIGHTER_IDX << detail.last_fighter_idx
			<< Heroer_Fields::CLICKED_NEW_HEROS << clicked_new_heros
			<< Heroer_Fields::ASSIST_HEROS << assist_hero_vec
			<< Heroer_Fields::HERO_INFOS << hero_infos;

	MONGO_CONNECTION.update(Heroer_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	save_heroer_status_detail(detail);
	return 0;
}

int DB_Operator::load_fight_hero_detail(Heroer_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Heroer_Fields::COLLECTION, QUERY(Heroer_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Heroer_Fields::ROLE_ID].numberLong();
	detail.outer_index = res[Heroer_Fields::OUTER_INDEX].numberInt();
	detail.fighter_index = res[Heroer_Fields::FIGHTER_INDEX].numberInt();
	detail.last_fighter_idx = res[Heroer_Fields::LAST_FIGHTER_IDX].numberInt();

	Hero_Info_Detail hero_info_detail;
	BSONObjIterator iter(res.getObjectField(Heroer_Fields::HERO_INFOS.c_str()));
	while (iter.more()) {
		BSONObj obj = iter.next().embeddedObject();
		//if (detail.fighter_index == obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt()) {
		//if (detail.outer_index == obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt()) {
		if (detail.fighter_index == obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt()) {
			hero_info_detail.reset();
			hero_info_detail.hero_id = obj[Heroer_Fields::Hero_Info::HERO_ID].numberInt();
			hero_info_detail.hero_index = obj[Heroer_Fields::Hero_Info::HERO_INDEX].numberInt();
			hero_info_detail.hero_facade_id = obj[Heroer_Fields::Hero_Info::HERO_FACADE_ID].numberInt();
			hero_info_detail.hero_name = obj[Heroer_Fields::Hero_Info::HERO_NAME].valuestrsafe();
			hero_info_detail.is_outer = obj[Heroer_Fields::Hero_Info::IS_OUTER].numberInt();
			hero_info_detail.is_fighter = obj[Heroer_Fields::Hero_Info::IS_FIGHTER].numberInt(); //
			hero_info_detail.level = obj[Heroer_Fields::Hero_Info::LEVEL].numberInt();
			hero_info_detail.exp = obj[Heroer_Fields::Hero_Info::EXP].numberInt();
			hero_info_detail.formation = obj[Heroer_Fields::Hero_Info::FORMATION].numberInt();
			hero_info_detail.awake_lvl = obj[Heroer_Fields::Hero_Info::AWAKE_LVL].numberInt();
			hero_info_detail.is_task_tmp_hero = obj[Heroer_Fields::Hero_Info::IS_TMP_HERO].numberInt();

			Artifact_Detail artifact_detail;
			BSONObjIterator it_artifact(obj.getObjectField(Heroer_Fields::Hero_Info::ARTIFACTS.c_str()));
			while (it_artifact.more()) {
				BSONObj obj_artifact = it_artifact.next().embeddedObject();
				artifact_detail.reset();
				artifact_detail.artifact_id = obj_artifact[Heroer_Fields::Hero_Info::Artifact::ARTIFACT_ID].numberInt();
				artifact_detail.artifact_lvl = obj_artifact[Heroer_Fields::Hero_Info::Artifact::ARTIFACT_LVL].numberInt();
				hero_info_detail.artifacts.insert(std::make_pair(artifact_detail.artifact_id, artifact_detail));
			}

			//arena
			if (obj.hasField(Fighter_Fields::ARENA_FIGHT.c_str())) {
				hero_info_detail.hero_fighter_detail.force = 0;
				if (hero_info_detail.hero_index == detail.fighter_index)
					handle_load_fighter_detail(hero_info_detail.hero_fighter_detail, obj[Fighter_Fields::ARENA_FIGHT].embeddedObject());
			}

			detail.hero_map.insert(std::make_pair(hero_info_detail.hero_index, hero_info_detail));
			Panal_Hero_Tag p_h_tag;
			p_h_tag.hero_id = hero_info_detail.hero_id;
			p_h_tag.hero_index = hero_info_detail.hero_index;
			p_h_tag.hero_level = hero_info_detail.hero_fighter_detail.level;
			p_h_tag.is_fight = 1;
			p_h_tag.is_last_fight = 0;
			detail.panal_hero_tags.push_back(p_h_tag);
		}
	}

	return 0;
}

int DB_Operator::load_rune_detail(Rune_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Rune_Fields::COLLECTION, QUERY(Rune_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Rune_Fields::ROLE_ID].numberLong();

	Rune_Part_Detail part_detail;
	Rune_Hole_Detail hole_detail;
	BSONObjIterator it_part(res.getObjectField(Rune_Fields::PARTS.c_str()));
	while (it_part.more()) {
		BSONObj obj_part = it_part.next().embeddedObject();
		part_detail.reset();
		part_detail.part = obj_part[Rune_Fields::Part::PART].numberInt();

		BSONObjIterator it_hole(obj_part.getObjectField(Rune_Fields::Part::HOLES.c_str()));
		while (it_hole.more()) {
			BSONObj obj_hole = it_hole.next().embeddedObject();
			hole_detail.reset();
			hole_detail.index = obj_hole[Rune_Fields::Part::Hole::INDEX].numberInt();
			hole_detail.level = obj_hole[Rune_Fields::Part::Hole::LEVEL].numberInt();
			hole_detail.energy = obj_hole[Rune_Fields::Part::Hole::ENERGY].numberInt();

			part_detail.hole_detail_map.insert(std::make_pair(hole_detail.index, hole_detail));
		}

		detail.part_detail_map.insert(std::make_pair(part_detail.part, part_detail));
	}
	return 0;
}

int DB_Operator::save_rune_detail(const Rune_Detail &detail) {
	std::vector<BSONObj> vec_part;
	std::vector<BSONObj> vec_hole;
	for (Rune_Part_Detail_Map::const_iterator it_part = detail.part_detail_map.begin();
			it_part != detail.part_detail_map.end(); ++it_part) {

		vec_hole.clear();
		for (Rune_Hole_Detail_Map::const_iterator it_hole = it_part->second.hole_detail_map.begin();
				it_hole != it_part->second.hole_detail_map.end(); ++it_hole) {
			//int index = it_hole->second.index;
			vec_hole.push_back(BSON(Rune_Fields::Part::Hole::INDEX << it_hole->second.index
					<< Rune_Fields::Part::Hole::LEVEL << it_hole->second.level
					<< Rune_Fields::Part::Hole::ENERGY << it_hole->second.energy));
		}

		vec_part.push_back(BSON(Rune_Fields::Part::PART << it_part->second.part
				<< Rune_Fields::Part::HOLES << vec_hole));
	}

	BSONObjBuilder builder;
	builder << Rune_Fields::ROLE_ID << (long long int)detail.role_id
			<< Rune_Fields::PARTS << vec_part;

	MONGO_CONNECTION.update(Rune_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_rune_stone_detail(Rune_Stone_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Rune_Stone_Fields::COLLECTION, QUERY(Rune_Stone_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Rune_Stone_Fields::ROLE_ID].numberLong();
	detail.points = res[Rune_Stone_Fields::POINTS].numberInt();
	detail.remain_double_times = 3;
	{
		// 不跨天读DB里的remain_double_times
		BSONObj res_role = MONGO_CONNECTION.findOne(Role_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id));
		if (! res_role.isEmpty()) {
			Time_Value last_sign_out_time(res_role[Role_Fields::LAST_SIGN_OUT_TIME].numberInt());
			if (is_same_day(last_sign_out_time, Time_Value::gettimeofday())) {
				detail.remain_double_times = res[Rune_Stone_Fields::REMAIN_DOUBLE_TIMES].numberInt();
			}
		}
	}

	Smelter_Detail smelter_detail;
	Rune_Hole_Detail hole_detail;
	BSONObjIterator it_smelter(res.getObjectField(Rune_Stone_Fields::SMELTER_DETAILS.c_str()));
	while (it_smelter.more()) {
		BSONObj obj_smelter = it_smelter.next().embeddedObject();
		smelter_detail.reset();
		smelter_detail.smelter_idx = obj_smelter[Rune_Stone_Fields::Smelter_Detail::SMELTER_IDX].numberInt();
		smelter_detail.is_open = obj_smelter[Rune_Stone_Fields::Smelter_Detail::IS_OPEN].numberInt();

		detail.smelter_detail_map.insert(std::make_pair(smelter_detail.smelter_idx, smelter_detail));
	}

	return 0;
}

int DB_Operator::save_rune_stone_detail(const Rune_Stone_Detail &detail) {
	std::vector<BSONObj> vec_smelter;
	for (Smelter_Detail_Map::const_iterator it_smelter = detail.smelter_detail_map.begin();
			it_smelter != detail.smelter_detail_map.end(); ++it_smelter) {
		vec_smelter.push_back(BSON(
				Rune_Stone_Fields::Smelter_Detail::SMELTER_IDX << it_smelter->second.smelter_idx
				<< Rune_Stone_Fields::Smelter_Detail::IS_OPEN << it_smelter->second.is_open
				));
	}

	BSONObjBuilder builder;
	builder << Rune_Stone_Fields::ROLE_ID << (long long int)detail.role_id
			<< Rune_Stone_Fields::POINTS << detail.points
			<< Rune_Stone_Fields::REMAIN_DOUBLE_TIMES << detail.remain_double_times
			<< Rune_Stone_Fields::SMELTER_DETAILS << vec_smelter;

	MONGO_CONNECTION.update(Rune_Stone_Fields::COLLECTION, QUERY(Role_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_status_detail(Logic_Status_Detail &detail) {
	BSONObj res = MONGO_CONNECTION.findOne(Status_Fields::COLLECTION, QUERY(Status_Fields::ROLE_ID << (long long int)detail.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	detail.role_id = res[Status_Fields::ROLE_ID].numberLong();
//	detail.status_detail.role_id = detail.role_id;

	BSONObjIterator it_part(res.getObjectField(Status_Fields::STATUS_MAP.c_str()));
	Status_Info status_info;
	while (it_part.more()) {
		BSONObj obj_st = it_part.next().embeddedObject();
		status_info.reset();
		status_info.status_id_ = obj_st[Status_Fields::Status_Map::STATUS_ID].numberInt();
		status_info.overlay_ = obj_st[Status_Fields::Status_Map::OVERLAY].numberInt();
		status_info.action_times = obj_st[Status_Fields::Status_Map::ACTION_TIMES].numberInt();
		status_info.trigger_times = obj_st[Status_Fields::Status_Map::TRIGGER_TIMES].numberInt();
		status_info.cfg_action_times = obj_st[Status_Fields::Status_Map::CFG_ACTION_TIMES].numberInt();
		status_info.cfg_trigger_times = obj_st[Status_Fields::Status_Map::CFG_TRIGGER_TIMES].numberInt();
		status_info.last_status_from_ = obj_st[Status_Fields::Status_Map::LAST_STATUS_FROM].numberInt();
		status_info.last_caster_role_id_ = obj_st[Status_Fields::Status_Map::LAST_CASTER_ROLE_ID].numberLong();
		status_info.last_caster_master_id_ = obj_st[Status_Fields::Status_Map::LAST_CASTER_MASTER_ID].numberLong();
//		if (obj_st.hasField(Status_Fields::Status_Map::IS_FIGHT_BUFF.c_str())) {
			status_info.is_fight_buff = obj_st[Status_Fields::Status_Map::IS_FIGHT_BUFF].booleanSafe();
//		}
		status_info.last_time_ms = obj_st[Status_Fields::Status_Map::LAST_TIME_MS].numberLong();

		BSONObjIterator it_bs(obj_st.getObjectField(Status_Fields::Status_Map::BUFF_SCENE.c_str()));
		int scene_id = 0;
		while (it_bs.more()) {
			scene_id = it_bs.next().numberInt();
			status_info.buff_scene.push_back(scene_id);
		}

		BSONObjIterator it_sv(obj_st.getObjectField(Status_Fields::Status_Map::SCENE_VISIBLE.c_str()));
		while (it_sv.more()) {
			scene_id = it_sv.next().numberInt();
			status_info.scene_visible.push_back(scene_id);
		}

		if (obj_st.hasField(Status_Fields::Status_Map::LAST_START_TIME.c_str())) {
			BSONObj obj = obj_st[Status_Fields::Status_Map::LAST_START_TIME].embeddedObject();
			status_info.last_start_time.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
		}
		if (obj_st.hasField(Status_Fields::Status_Map::LAST_END_TIME.c_str())) {
			BSONObj obj = obj_st[Status_Fields::Status_Map::LAST_END_TIME].embeddedObject();
			status_info.last_end_time.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
		}
		BSONObjIterator it_fifo(obj_st.getObjectField(Status_Fields::Status_Map::STATUS_FIFO.c_str()));
		role_id_t role_id = 0;
		while (it_fifo.more()) {
			role_id = it_fifo.next().numberLong();
			status_info.status_fifo.push_back(role_id);
		}
		BSONObjIterator it_info(obj_st.getObjectField(Status_Fields::Status_Map::STATUS_INFO_MAP.c_str()));
		Status_Info_Detail status_info_detail;
		while (it_info.more()) {
			BSONObj obj_info = it_info.next().embeddedObject();
			role_id = obj_info[Status_Fields::ROLE_ID].numberLong();
			status_info_detail.reset();
			status_info_detail.status_id = obj_info[Status_Fields::Status_Map::Status_Info_Map::STATUS_ID].numberInt();
			status_info_detail.status_from = obj_info[Status_Fields::Status_Map::Status_Info_Map::STATUS_FROM].numberInt();
			status_info_detail.caster_role_id = obj_info[Status_Fields::Status_Map::Status_Info_Map::CASTER_ROLE_ID].numberLong();
			status_info_detail.caster_master_id = obj_info[Status_Fields::Status_Map::Status_Info_Map::CASTER_MASTER_ID].numberLong();
			status_info_detail.overlay = obj_info[Status_Fields::Status_Map::Status_Info_Map::OVERLAY].numberInt();

			BSONObjIterator it_em(obj_info.getObjectField(Status_Fields::Status_Map::Status_Info_Map::EFFECT_MAP.c_str()));
			Status_Effect_Detail status_effect_detail;
			int ht = 0;
			while (it_em.more()) {
				BSONObj obj_em = it_em.next().embeddedObject();
				status_effect_detail.reset();
				BSONObjIterator it_ht(obj_em.getObjectField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::HEAT_TYPE.c_str()));
				while (it_ht.more()) {
					ht = it_ht.next().numberInt();
					status_effect_detail.heat_type.insert(ht);
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_MS.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_MS].embeddedObject();
					status_effect_detail.delay_time_ms.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::NEXT_OPT.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::NEXT_OPT].embeddedObject();
					status_effect_detail.next_opt.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::INTERVAL.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::INTERVAL].embeddedObject();
					status_effect_detail.interval.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}
				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_TICK.c_str())) {
					BSONObj obj = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_TICK].embeddedObject();
					status_effect_detail.delay_time_tick.set(obj[Time_Value_Fields::SEC].numberInt(), obj[Time_Value_Fields::USEC].numberInt());
				}

				if (obj_em.hasField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::TRIGGER_BUFF.c_str())) {
					BSONObj obj_tb = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::TRIGGER_BUFF].embeddedObject();
					BSONObjIterator it_dt(obj_tb.getObjectField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DATA.c_str()));
					Effect_Argv_Prop prop;
					while (it_dt.more()) {
						BSONObj obj_dt = it_dt.next().embeddedObject();
						prop.reset();
						prop.effect_value = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::EFFECT_VALUE].numberDouble();
						prop.i_effect_val1 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL1].numberInt();
						prop.i_effect_val2 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL2].numberInt();
						prop.i_effect_val3 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL3].numberInt();
						prop.d_effect_val1 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL1].numberDouble();
						prop.d_effect_val2 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL2].numberDouble();
						prop.d_effect_val3 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL3].numberDouble();
						prop.d_effect_val4 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL4].numberDouble();
						prop.d_effect_val5 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL5].numberDouble();
						prop.d_effect_val6 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL6].numberDouble();
						prop.d_effect_val7 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL7].numberDouble();
						prop.d_effect_val8 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL8].numberDouble();
						prop.d_effect_val9 = obj_dt[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL9].numberDouble();
						status_effect_detail.trigger_buff.data.push_back(prop);
					}
					BSONObjIterator it_dg(obj_tb.getObjectField(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DISAPPEAR_GAP.c_str()));
					while (it_dg.more()) {
						ht = it_dg.next().numberInt();
						status_effect_detail.trigger_buff.disappear_gap.push_back(ht);
					}
					status_effect_detail.trigger_buff.status_id = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_ID].numberInt();
					status_effect_detail.trigger_buff.last_time_ms = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::LAST_TIME_MS].numberLong();
					status_effect_detail.trigger_buff.cfg_action_gap = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_ACTION_GAP].numberInt();
					status_effect_detail.trigger_buff.cfg_trigger_gap = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_TRIGGER_GAP].numberInt();
					status_effect_detail.trigger_buff.status_from = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_FROM].numberInt();
					status_effect_detail.trigger_buff.caster_id = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CASTER_ID].numberLong();
					status_effect_detail.trigger_buff.overlay = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::OVERLAY].numberInt();
					status_effect_detail.trigger_buff.is_fight_buff = obj_tb[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::IS_FIGHT_BUFF].boolean();
				}
				status_effect_detail.effect_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_ID].numberInt();
				status_effect_detail.effect_value = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_VALUE].numberInt();
				status_effect_detail.i_effect_value1 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE1].numberInt();
				status_effect_detail.i_effect_value2 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE2].numberInt();
				status_effect_detail.i_effect_value3 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE3].numberInt();
				status_effect_detail.d_effect_value1 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE1].numberDouble();
				status_effect_detail.d_effect_value2 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE2].numberDouble();
				status_effect_detail.d_effect_value3 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE3].numberDouble();
				status_effect_detail.d_effect_value4 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE4].numberDouble();
				status_effect_detail.d_effect_value5 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE5].numberDouble();
				status_effect_detail.d_effect_value6 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE6].numberDouble();
				status_effect_detail.d_effect_value7 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE7].numberDouble();
				status_effect_detail.d_effect_value8 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE8].numberDouble();
				status_effect_detail.d_effect_value9 = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE9].numberDouble();
				status_effect_detail.effect_type = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TYPE].numberInt();
				status_effect_detail.status_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::STATUS_ID].numberInt();
				status_effect_detail.effect_times = 0;//obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TIMES].numberInt();
				status_effect_detail.overlay = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::OVERLAY].numberInt();
				status_effect_detail.is_overlay = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_OVERLAY].Bool();
				status_effect_detail.is_hit = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_HIT].Bool();
				status_effect_detail.skill_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::SKILL_ID].numberInt();
				status_effect_detail.buff_chance = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::BUFF_CHANCE].numberInt();
				status_effect_detail.caster_id = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::CASTER_ID].numberLong();
				status_effect_detail.action_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TIMES].numberInt();
				status_effect_detail.action_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TICK].numberInt();
				status_effect_detail.passive_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TIMES].numberInt();
				status_effect_detail.passive_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TICK].numberInt();
				status_effect_detail.is_delay = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_DELAY].Bool();
				status_effect_detail.delay_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIMES].numberInt();
				status_effect_detail.delay_action_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TIMES].numberInt();
				status_effect_detail.delay_passive_times = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TIMES].numberInt();
				status_effect_detail.delay_action_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TICK].numberInt();
				status_effect_detail.delay_passive_tick = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TICK].numberInt();
				status_effect_detail.last_cure_value = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_VALUE].numberDouble();
				status_effect_detail.last_cure_rate = obj_em[Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_RATE].numberDouble();

				status_effect_detail.buff_scene = status_info.buff_scene;
				status_info_detail.effect_map.push_back(status_effect_detail);
			}
			status_info.status_info_map.insert(std::make_pair(role_id, status_info_detail));
		}
		detail.status_map.insert(std::make_pair(status_info.status_id_, status_info));
	}
	return 0;
}

int DB_Operator::save_status_detail(const Logic_Status_Detail &detail) {
	std::vector<BSONObj> status_map;
	std::vector<BSONObj> status_fifo;
	std::vector<BSONObj> status_info_map;
	std::vector<BSONObj> effect_map;
	std::vector<BSONObj> trigger_buff;
	std::vector<BSONObj> data;
	std::vector<int> heat_type;
	for (Logic_Status_Detail::Status_Map::const_iterator it = detail.status_map.begin();
			it != detail.status_map.end(); ++it) {
		status_fifo.clear();
		status_info_map.clear();
		Status_Info si = it->second;
		for (std::deque<role_id_t>::const_iterator it_sim = it->second.status_fifo.begin();
				it_sim != it->second.status_fifo.end(); ++it_sim) {
			status_fifo.push_back(BSON(Status_Fields::ROLE_ID << (long long int)*it_sim));
		}
		for (Status_Info::Status_Info_Map::const_iterator it_sim = it->second.status_info_map.begin();
				it_sim != it->second.status_info_map.end(); ++it_sim) {
			effect_map.clear();
			for (Status_Info_Detail::Effect_Vec::const_iterator it_em = it_sim->second.effect_map.begin();
					it_em != it_sim->second.effect_map.end(); ++it_em) {
				heat_type.clear();
				trigger_buff.clear();
				data.clear();
				heat_type.assign((*it_em).heat_type.begin(), (*it_em).heat_type.end());
				for (Effect_Argv_Prop_Vec::const_iterator it_data = (*it_em).trigger_buff.data.begin(); it_data != (*it_em).trigger_buff.data.end(); ++it_data) {
					data.push_back(BSON(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::EFFECT_VALUE << it_data->effect_value
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL1 << it_data->i_effect_val1
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL2 << it_data->i_effect_val2
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::I_EFFECT_VAL3 << it_data->i_effect_val3
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL1 << it_data->d_effect_val1
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL2 << it_data->d_effect_val2
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL3 << it_data->d_effect_val3
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL4 << it_data->d_effect_val4
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL5 << it_data->d_effect_val5
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL6 << it_data->d_effect_val6
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL7 << it_data->d_effect_val7
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL8 << it_data->d_effect_val8
							<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::Data::D_EFFECT_VAL9 << it_data->d_effect_val9));
				}
				trigger_buff.push_back(BSON(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_ID << (*it_em).trigger_buff.status_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::LAST_TIME_MS << (long long int)(*it_em).trigger_buff.last_time_ms
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_ACTION_GAP << (*it_em).trigger_buff.cfg_action_gap
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CFG_TRIGGER_GAP << (*it_em).trigger_buff.cfg_trigger_gap
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::STATUS_FROM << (*it_em).trigger_buff.status_from
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::CASTER_ID << (long long int)(*it_em).trigger_buff.caster_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::OVERLAY << (*it_em).trigger_buff.overlay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DISAPPEAR_GAP << (*it_em).trigger_buff.disappear_gap
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::DATA << data
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::Trigger_Buff::IS_FIGHT_BUFF << (*it_em).trigger_buff.is_fight_buff));
				effect_map.push_back(BSON(Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_ID << (*it_em).effect_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_VALUE << (*it_em).effect_value
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE1 << (*it_em).i_effect_value1
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE2 << (*it_em).i_effect_value2
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::I_EFFECT_VALUE3 << (*it_em).i_effect_value3
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE1 << (*it_em).d_effect_value1
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE2 << (*it_em).d_effect_value2
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE3 << (*it_em).d_effect_value3
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE4 << (*it_em).d_effect_value4
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE5 << (*it_em).d_effect_value5
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE6 << (*it_em).d_effect_value6
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE7 << (*it_em).d_effect_value7
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE8 << (*it_em).d_effect_value8
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::D_EFFECT_VALUE9 << (*it_em).d_effect_value9
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TYPE << (*it_em).effect_type
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::STATUS_ID << (*it_em).status_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::HEAT_TYPE << heat_type
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::EFFECT_TIMES << (*it_em).effect_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::OVERLAY << (*it_em).overlay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_OVERLAY << (*it_em).is_overlay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_HIT << (*it_em).is_hit
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::SKILL_ID << (*it_em).skill_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::TRIGGER_BUFF << trigger_buff
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::BUFF_CHANCE << (*it_em).buff_chance
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::CASTER_ID << (long long int)(*it_em).caster_id
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TIMES << (*it_em).action_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::ACTION_TICK << (*it_em).action_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TIMES << (*it_em).passive_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::PASSIVE_TICK << (*it_em).passive_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::NEXT_OPT << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).next_opt.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).next_opt.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::INTERVAL << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).interval.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).interval.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::IS_DELAY << (*it_em).is_delay
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIMES << (*it_em).delay_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_MS << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).delay_time_ms.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).delay_time_ms.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TIMES << (*it_em).delay_action_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TIMES << (*it_em).delay_passive_times
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_TIME_TICK << BSON(Time_Value_Fields::SEC << (long long int)(*it_em).delay_time_tick.sec()
								<< Time_Value_Fields::USEC << (long long int)(*it_em).delay_time_tick.usec())
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_ACTION_TICK << (*it_em).delay_action_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::DELAY_PASSIVE_TICK << (*it_em).delay_passive_tick
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_VALUE << (*it_em).last_cure_value
						<< Status_Fields::Status_Map::Status_Info_Map::Effect_Map::LAST_CURE_RATE << (*it_em).last_cure_rate));
			}
			status_info_map.push_back(BSON(Status_Fields::ROLE_ID << (long long int)it_sim->first
					<< Status_Fields::Status_Map::Status_Info_Map::STATUS_ID << it_sim->second.status_id
					<< Status_Fields::Status_Map::Status_Info_Map::STATUS_FROM << it_sim->second.status_from
					<< Status_Fields::Status_Map::Status_Info_Map::CASTER_ROLE_ID << (long long int)it_sim->second.caster_role_id
					<< Status_Fields::Status_Map::Status_Info_Map::CASTER_MASTER_ID << (long long int)it_sim->second.caster_master_id
					<< Status_Fields::Status_Map::Status_Info_Map::OVERLAY << it_sim->second.overlay
					<< Status_Fields::Status_Map::Status_Info_Map::EFFECT_MAP << effect_map));
		}

		status_map.push_back(BSON(Status_Fields::Status_Map::STATUS_ID << it->first
				<< Status_Fields::Status_Map::OVERLAY << it->second.overlay_
				<< Status_Fields::Status_Map::ACTION_TIMES << it->second.action_times
				<< Status_Fields::Status_Map::TRIGGER_TIMES << it->second.trigger_times
				<< Status_Fields::Status_Map::CFG_ACTION_TIMES << it->second.cfg_action_times
				<< Status_Fields::Status_Map::CFG_TRIGGER_TIMES << it->second.trigger_times
				<< Status_Fields::Status_Map::LAST_STATUS_FROM << it->second.last_status_from_
				<< Status_Fields::Status_Map::LAST_CASTER_ROLE_ID << (long long int)it->second.last_caster_role_id_
				<< Status_Fields::Status_Map::LAST_CASTER_MASTER_ID << (long long int)it->second.last_caster_master_id_
				<< Status_Fields::Status_Map::IS_FIGHT_BUFF << it->second.is_fight_buff
				<< Status_Fields::Status_Map::LAST_TIME_MS << (long long int)it->second.last_time_ms
				<< Status_Fields::Status_Map::LAST_START_TIME << BSON(Time_Value_Fields::SEC << (long long int)it->second.last_start_time.sec()
						<< Time_Value_Fields::USEC << (long long int)it->second.last_start_time.usec())
				<< Status_Fields::Status_Map::LAST_END_TIME << BSON(Time_Value_Fields::SEC << (long long int)it->second.last_end_time.sec()
						<< Time_Value_Fields::USEC << (long long int)it->second.last_end_time.usec())
				<< Status_Fields::Status_Map::STATUS_FIFO << status_fifo
				<< Status_Fields::Status_Map::STATUS_INFO_MAP << status_info_map
				<< Status_Fields::Status_Map::BUFF_SCENE << it->second.buff_scene
				<< Status_Fields::Status_Map::SCENE_VISIBLE << it->second.scene_visible
				));
	}

	BSONObjBuilder builder;
	builder << Status_Fields::ROLE_ID << (long long int)detail.role_id
			<< Status_Fields::STATUS_MAP << status_map;

	MONGO_CONNECTION.update(Status_Fields::COLLECTION, QUERY(Status_Fields::ROLE_ID << (long long int)detail.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}
