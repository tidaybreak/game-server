/*
 * Knight_Trials.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "Knight_Trials.h"
#include "Knight_Trials_Struct.h"
#include "Logic_Module_Head.h"
#include "Knight_Trials_Def.h"
#include "Msg_Inner_Enum.h"
#include "Logic_Player.h"
#include "Knight_Trials_Manager.h"
#include "Config_Cache.h"
#include "Config_Cache_Knight_Trials.h"
#include "Msg_Knight_Trials_Struct.h"
#include "Err_Code.h"
#include "Knight_Trials_Config.h"
#include "Msg_Active_Struct.h"
#include "helper/Helper_Def.h"
#include "vip/Vip_Def.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "DB_Def.h"

Knight_Trials::Knight_Trials(void) {
	reset();
}

Knight_Trials::~Knight_Trials() {
	// TODO Auto-generated destructor stub
}

void Knight_Trials::reset(void) {
	knight_trials_detail_ = 0;
	knight_trials_player_detail_ = 0;
	sign_force = 0;
}

int Knight_Trials::create_init(Knight_Trials_Detail &detail) {

	return 0;
}

void Knight_Trials::load_detail(Knight_Trials_Detail &detail) {
	knight_trials_detail_ = &detail;
}

void Knight_Trials::save_detail(Knight_Trials_Detail &detail) {
	if (knight_trials_detail().is_change) {
		detail = knight_trials_detail();
		knight_trials_detail_->is_change = false;
	}
}

void Knight_Trials::load_player_detail(Knight_Trials_Player_Detail &detail) {
	knight_trials_player_detail_ = &detail;
}

void Knight_Trials::save_player_detail(Knight_Trials_Player_Detail &detail) {
	if (knight_trials_player_detail().is_change) {
		detail = knight_trials_player_detail();
		knight_trials_player_detail_->is_change = false;
	}
}

int Knight_Trials::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Knight_Trials::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Knight_Trials::trigger_daily_six(const Time_Value &now) {
	//if(player_self()->level() < 40) { return 0; }
	if(!player_self()->is_icon_open(KNIGHT_TRIALS_ICON_ID)) { return 0; }
	++knight_trials_detail_->match_day;//记录当前是第几天耍人,因为第一天的话; 异步跨服或者异步数据库或者本服在线匹配返回的玩家要往当前关卡数组放人; 否则往隐藏的关卡放人
	knight_trials_detail_->reset_status = false;
	knight_trials_detail_->detail_change();
	refres_knight_trials_on_local();//先在本服刷人

	//fixme 有可能fixme, 现在是本地和跨服同时去取数据;本地要保证必须10个,本地的返回通过插入排序;跨服回来是匹配战力最近的替换;
	//如果这种异步问题很大的话;需要把这以下代码移动到本地匹配完成后再发跨服消息,到时候要注意一个蛋疼的问题是要注意六点这个时间点;
	//原因在于本地取的玩家不在线;也会出现异步的情况;跨服本身是异步;看数据包返回的顺序啊;要严格区分
//	Block_Buffer buf;
//	buf.make_message(INNER_KNIGHT_TRIALS_MATCH_PLAYER);
//	buf.write_uint8(KNIGHT_TRIALS_SIX_MATCH);
//	buf.write_int64(player_self()->role_id());
//	buf.write_int64(CONFIG_CACHE->server_flag());
//	buf.write_int32(player_self()->fighter_detail().highest_force);
//	buf.finish_message();
//	return LOGIC_MONITOR->send_to_scene(KNIGHT_TRIALS_INNER_SCENE_ID, 0, buf);
	//2014年10月31日 17:28:23 先屏蔽跨服匹配
	return 0;
}

int Knight_Trials::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Knight_Trials::move_scene_completed(void) {

	return 0;
}

int Knight_Trials::client_ready_now(void) {
	MSG_82200250 msg;
	uint32_t all_num = CONFIG_CACHE_KNIGHT_TRIALS->get_points_conf().size();
	msg.num = all_num - knight_trials_detail_->cur_points;
	msg.rnum = all_num - knight_trials_detail_->cur_receive;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Knight_Trials::module_init(void) {
	if(knight_trials_detail_->role_id_set.size()) {
		refres_knight_trials_player_on_local();
	}
}

void Knight_Trials::sign_in(void) {
	sign_force = player_self()->fighter_detail().ttl_force;
}

void Knight_Trials::sign_out(void) {
	int new_force = player_self()->fighter_detail().force + player_self()->get_fight_hero_force();
	KNIGHT_TRIALS_MANAGER->login_out_update_map(sign_force, new_force, player_self()->role_id());
}

void Knight_Trials::trusteeship_sign_out(void) {

}

int Knight_Trials::sign_in_repeat(void) {

	return 0;
}

void Knight_Trials::update_add_up_reward(Uint_Uint &uu) {
	for(std::vector<Uint_Uint>::iterator it = knight_trials_detail_->reward.begin();
			it != knight_trials_detail_->reward.end(); ++it) {
		if(it->val_1 == uu.val_1) {
			it->val_2 += uu.val_2;
			return;
		}
	}
	knight_trials_detail_->reward.push_back(uu);
}

void Knight_Trials::refres_box_reward_on_rand(uint8_t points) {
	const Knight_Trials_Points_Conf *pconf = CONFIG_CACHE_KNIGHT_TRIALS->get_points_conf_by_points(points);
	if(!pconf) { return; }
	const Knight_Trials_Box_Reward_Conf_List *conf = CONFIG_CACHE_KNIGHT_TRIALS->get_box_reward_list_by_type(pconf->boxType);
	if(conf) {
		Knight_Trials_Box_Reward_Conf_Vec box_reward_vec;
		box_reward_vec.clear();
		Int_Vec weight;
		weight.clear();

		for(Knight_Trials_Box_Reward_Conf_List::const_iterator it = conf->begin(); it != conf->end(); ++it) {
			if(it->slevel <= (uint32_t)player_self()->level() && it->elevel >= (uint32_t)player_self()->level()) {
				box_reward_vec.push_back(*it);
				weight.push_back(it->rate);
			}
		}

		if(weight.size() < pconf->num) {
			return;
		}
		//得到已经记录的
		Record_Box_Reward_Conf_Map::iterator rit = knight_trials_detail_->recode_box_reward.find(points);
		if(rit != knight_trials_detail_->recode_box_reward.end()) {
			rit->second.clear();
		} else {
			Record_Box_Reward_List list;
			list.clear();
			std::pair<Record_Box_Reward_Conf_Map::iterator, bool> re = knight_trials_detail_->recode_box_reward.insert(std::make_pair(points, list));
			if(re.second) {
				rit = re.first;
			} else {
				return;
			}
		}

		//求出第一个物品
		for(uint j = 0; j < pconf->num; ++j) {
			uint32_t index = Utility_Func::lottery_one(weight);
			{
				if(box_reward_vec.size() <= index) {return;}
				if(weight.size() <= index) {return;}
				Record_Box_Reward fvalue;
				fvalue.reset();
				fvalue.id = box_reward_vec[index].reward_id;
				fvalue.type = box_reward_vec[index].type;
				srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
				uint32_t num = random() % (box_reward_vec[index].end - box_reward_vec[index].start + 1);
				fvalue.num = box_reward_vec[index].start + num;
				rit->second.push_back(fvalue);
			}

			{//删除已经随机出来的奖励;
				Knight_Trials_Box_Reward_Conf_Vec::iterator del_it = box_reward_vec.begin();
				del_it += index;
				box_reward_vec.erase(del_it);
			}
			{//删除已经随机出来的权重种子
				Int_Vec::iterator del_it = weight.begin();
				del_it += index;
				weight.erase(del_it);
			}
		}
		//求出第2个物品
//		index = Utility_Func::lottery_one(weight);
//		{
//			if(box_reward_vec.size() <= index) {return;}
//			if(weight.size() <= index) {return;}
//			Record_Box_Reward fvalue;
//			fvalue.reset();
//			fvalue.id = box_reward_vec[index].reward_id;
//			fvalue.type = box_reward_vec[index].type;
//			srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
//			uint32_t num = random() % (box_reward_vec[index].end - box_reward_vec[index].start + 1);
//			fvalue.num = box_reward_vec[index].start + num;
//			rit->second.push_back(fvalue);
//		}

		knight_trials_detail_->detail_change();
	}
}

void Knight_Trials::refres_knight_trials_on_local(void) {
	knight_trials_detail_->role_id_set.clear();
	knight_trials_player_detail_->hide_player_fighter_vec.clear();
	UInt8_Vec rank;
	rank.clear();
	for(uint32_t i = 1; i<= CONFIG_CACHE_KNIGHT_TRIALS->get_points_conf().size(); ++i) {
		rank.push_back(i);
	}

	int hforce = player_self()->fighter_detail().force + player_self()->get_fight_hero_force();
	if(hforce < player_self()->fighter_detail().highest_force) {
		hforce = player_self()->fighter_detail().highest_force;
	}
	MSG_DEBUG("start knight match highest role_id:%ld force:%d", player_self()->role_id(), hforce);
	KNIGHT_TRIALS_MANAGER->match_player_role_id_by_rankvec(rank, knight_trials_detail_->role_id_set,
			hforce, player_self()->role_id());
	knight_trials_detail_->detail_change();
	refres_knight_trials_player_on_local(true);
}

void Knight_Trials::refres_knight_trials_player_on_local(bool clear) {
	Knight_Trials_Player kplayer;
	Role_Id_Set tmp_rs = knight_trials_detail_->role_id_set;
//	for(Role_Id_Set::iterator it = tmp_rs.begin(); it != tmp_rs.end(); ++it) {
//		if(KNIGHT_TRIALS_MANAGER->match_player_local(*it, kplayer, player_self()->role_id(), knight_trials_detail_->match_day, clear) == 0) {
//			insert_knight_trials_player_fighter_info_vec(kplayer);
//		}
//		clear = false;//这一句非常非常重要
//	}
	Block_Buffer buf;
	buf.make_message(MONGO_KNIGHT_TRIALS_MATCH_PLAYER);
	buf.write_int64(0);//本地
	buf.write_int64(player_self()->role_id());
	buf.write_int64(knight_trials_detail_->match_day);
	buf.write_bool(clear);

	buf.write_uint16(tmp_rs.size());
	for(Role_Id_Set::iterator it = tmp_rs.begin(); it != tmp_rs.end(); ++it) {
		buf.write_int64(*it);
	}

	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	knight_trials_detail_->detail_change();
}

void Knight_Trials::insert_knight_trials_player_fighter_info_vec_callback(Knight_Trials_Player &player) {
	Knight_Trials_Role_Info value;
	knight_trials_player_to_role_info(player, value);
	//跨服替换的是战斗玩家数据里面的数据;所以不需要对基础detail里面的set进行删除; 切记,不能乱啊
	if(knight_trials_detail_->match_day > 1) {//不是第一次刷新
		//以后永远不能是跨服了
//		if(knight_trials_player_detail_->hide_player_fighter_vec.size() + knight_trials_detail_->role_id_set.size()
//				< 10) { //尽最大可能匹配10个人
			bool not_find = true;
			for(Knight_Trials_Role_Info_Vec::iterator it = knight_trials_player_detail_->hide_player_fighter_vec.begin();
					it != knight_trials_player_detail_->hide_player_fighter_vec.end(); ++it) {
				if((it->force+it->hero_force) > (value.force+value.hero_force)) {
					knight_trials_player_detail_->hide_player_fighter_vec.insert(it, value);
					not_find = false;
					break;
				}
			}
			if(knight_trials_player_detail_->hide_player_fighter_vec.empty() || not_find) {
				knight_trials_player_detail_->hide_player_fighter_vec.push_back(value);
			}
//		} else {
//			//找出替换的索引
//			int index = -1; //fixme 靠;写到这里发现,如果本服匹配的玩家都还没异步完成,这里跨服返回了;真有可能找不到替换对象;
//			//或者替换是不准的;不过先不考虑这里了;有问题就改上一个fixme
//			int splice_force = INT_MAX;
//			for(uint32_t i = 0; i < knight_trials_player_detail_->hide_player_fighter_vec.size(); ++i) {
//				if(abs(knight_trials_player_detail_->hide_player_fighter_vec[i].fighter.ttl_force - player.fighter.ttl_force)
//					< splice_force) {
//					index = i;
//				}
//			}
//			if(index >= 0) {
//				knight_trials_player_detail_->hide_player_fighter_vec[index] = player;
//			}
//		}
	} else { //首次刷新
//		if(knight_trials_player_detail_->cur_player_fighter_vec.size() + knight_trials_detail_->role_id_set.size()
//				< 10) { //尽最大可能匹配10个人
			bool not_find = true;
			for(Knight_Trials_Role_Info_Vec::iterator it = knight_trials_player_detail_->cur_player_fighter_vec.begin();
					it != knight_trials_player_detail_->cur_player_fighter_vec.end(); ++it) {
				if((it->force+it->hero_force) > (value.force+value.hero_force)) {
					knight_trials_player_detail_->cur_player_fighter_vec.insert(it, value);
					not_find = false;
					break;
				}
			}
			if(knight_trials_player_detail_->cur_player_fighter_vec.size() == 0 || not_find) {
				knight_trials_player_detail_->cur_player_fighter_vec.push_back(value);
			}
//		} else {
//			//找出替换的索引
//			int index = -1; //fixme 靠;写到这里发现,如果本服匹配的玩家都还没异步完成,这里跨服返回了;真有可能找不到替换对象;
//			//或者替换是不准的;不过先不考虑这里了;有问题就改上一个fixme
//			int splice_force = INT_MAX;
//			for(uint32_t i = 0; i < knight_trials_player_detail_->cur_player_fighter_vec.size(); ++i) {
//				if(abs(knight_trials_player_detail_->cur_player_fighter_vec[i].fighter.ttl_force - player.fighter.ttl_force)
//					< splice_force) {
//					index = i;
//				}
//			}
//			if(index >= 0) {
//				knight_trials_player_detail_->cur_player_fighter_vec[index] = player;
//			}
//		}
	}
//	knight_trials_player_detail_->detail_change();
}

void Knight_Trials::insert_knight_trials_player_fighter_info_vec(Knight_Trials_Player &player) {
	Knight_Trials_Role_Info value;
	knight_trials_player_to_role_info(player, value);
	Role_Id_Set::iterator it = knight_trials_detail_->role_id_set.find(player.base.role_id);
	if(it == knight_trials_detail_->role_id_set.end()){
		return;
	}
	knight_trials_detail_->role_id_set.erase(it);
	if(knight_trials_detail_->match_day > 1) {//不是第一次刷新
		bool not_find = true;
		for(Knight_Trials_Role_Info_Vec::iterator it = knight_trials_player_detail_->hide_player_fighter_vec.begin();
				it != knight_trials_player_detail_->hide_player_fighter_vec.end(); ++it) {
			if((it->force+it->hero_force) > (value.force+value.hero_force)) {
				knight_trials_player_detail_->hide_player_fighter_vec.insert(it, value);
				not_find = false;
				break;
			}
		}
		if(knight_trials_player_detail_->hide_player_fighter_vec.size() == 0 || not_find) {
			knight_trials_player_detail_->hide_player_fighter_vec.push_back(value);
		}
	} else { //首次刷新
		bool not_find = true;
		for(Knight_Trials_Role_Info_Vec::iterator it = knight_trials_player_detail_->cur_player_fighter_vec.begin();
				it != knight_trials_player_detail_->cur_player_fighter_vec.end(); ++it) {
			if((it->force+it->hero_force) > (value.force+value.hero_force)) {
				not_find = false;
				knight_trials_player_detail_->cur_player_fighter_vec.insert(it, value);
				break;
			}
		}
		if(knight_trials_player_detail_->cur_player_fighter_vec.size() == 0 || not_find) {
			knight_trials_player_detail_->cur_player_fighter_vec.push_back(value);
		}
	}
//	knight_trials_player_detail_->detail_change();
	knight_trials_detail_->detail_change();
	//今天匹配完成;判断是否昨天的骑士试练是否已经全部通关;如果全部通关自动重置
	if(knight_trials_detail_->role_id_set.size() == 0 && knight_trials_detail_->cur_receive >= CONFIG_CACHE_KNIGHT_TRIALS->get_points_conf().size()) {
		req_up_hide_knight_trials_palyer();
	}
}

int Knight_Trials::req_open_knight_trials_plan(void) {
	MSG_50107100 msg;
	msg.reset();
	msg.points = knight_trials_detail_->cur_receive;  //客户端的通关数就是可领取奖励的数量
	msg.receive = knight_trials_detail_->cur_points; //客户端是要显示可以领取的奖励;而我这里不是;我这里是记录了领取过的
	msg.reward = knight_trials_detail_->reward;

	msg.reset_status = 0;
	if(!knight_trials_detail_->reset_status) {
		if(knight_trials_detail_->match_day > 1) {
			if(knight_trials_player_detail_->hide_player_fighter_vec.size()) {
				msg.reset_status = 1;
			}
		}
	}
	int points = 1;
	for(Knight_Trials_Role_Info_Vec::iterator it = knight_trials_player_detail_->cur_player_fighter_vec.begin();
			it != knight_trials_player_detail_->cur_player_fighter_vec.end(); ++it) {
//		value.reset();
//		value.points = points;
//		value.role_id = it->base.role_id;
//		value.role_name = it->base.role_name;
//		value.head_id = it->base.head_id;
//		value.gender = it->base.gender;
//		value.career = it->base.career;
//		value.level = it->base.level;
//		{//vip
//			Time_Value now = Time_Value::gettimeofday();
//			if(now.sec() > it->base.vip_over_time.sec()) {
//				value.vip = it->base.vip;
//			} else {
//				value.vip = 0;
//			}
//		}
//		value.server_name = it->server_name;
//		value.force = it->fighter.force;
//
//		{//hero
//			if(it->heroer.fighter_index) {
//				Heroer_Detail::Hero_Map::iterator hero_it = it->heroer.hero_map.find(it->heroer.fighter_index);
//				if(hero_it != it->heroer.hero_map.end()) {
//					value.hero_id = hero_it->second.hero_id;
//					value.hero_force = hero_it->second.hero_fighter_detail.force;
//					value.hero_level = hero_it->second.level;
//					value.hero_name = hero_it->second.hero_name;
//				}
//			}
//		}
//		value.mount_id = it->base.mount_model_id;
//		value.fashion = it->base.avatar_vec;
//		value.gang_name = it->base.gang_name;
		it->points = points;
		msg.info.push_back(*it);
		++points;
	}


	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Knight_Trials::knight_trials_listen_icon_open(const uint16_t icon_id) {
	if(icon_id == KNIGHT_TRIALS_ICON_ID) {
		trigger_daily_six(Time_Value::gettimeofday());
	}
}

void Knight_Trials::listen_knight_trials_win_or_lost(uint8_t battle_result) {
	if(battle_result) { //胜利
		if(knight_trials_detail_->cur_points < knight_trials_player_detail_->cur_player_fighter_vec.size()) {
			uint32_t exp = CONFIG_CACHE_KNIGHT_TRIALS->get_exp_buy_level_points(player_self()->level(), knight_trials_detail_->cur_points);
			if(exp) {
				Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, true);
				if (0 == exp_all.result) {
					//背包特效
					MSG_81000102 acv_msg;
					acv_msg.type = 0;
					acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
					acv_msg.vip_exp = exp_all.vip_exp;
					acv_msg.world_level = player_self()->get_world_level_add();
					THIS_SEND_TO_CLIENT(acv_msg);
				}
			}
			++knight_trials_detail_->cur_points;
			//刷新奖励
			refres_box_reward_on_rand(knight_trials_detail_->cur_points);
			knight_trials_detail_->detail_change();
		}

		if(knight_trials_detail_->cur_points >= knight_trials_player_detail_->cur_player_fighter_vec.size()){
			player_self()->task_listen_pass_kngiht_trials();
		}

		//task listen
		player_self()->task_listen_knight_trial(knight_trials_detail_->cur_points);
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_KNIGHT_TRIALS);
		player_self()->task_listen_knight_trial_any_pass(); //通关任意一关骑士试炼调用这个接口
	}
	MSG_50107101 msg;
	msg.reset();
	msg.battle_result = battle_result;
	THIS_SEND_TO_CLIENT(msg);
}

int Knight_Trials::req_knight_trials_enter_fighter(void) {
	if(knight_trials_detail_->cur_points != knight_trials_detail_->cur_receive) {
		return ERROR_CLIENT_PARAM; //还有奖励没领取完
	}
	if(knight_trials_detail_->cur_points >= knight_trials_player_detail_->cur_player_fighter_vec.size()) {
		return ERROR_CLIENT_PARAM; //已经通关完
	}

	Knight_Trials_Role_Info &KRoleInfo = knight_trials_player_detail_->cur_player_fighter_vec[knight_trials_detail_->cur_points];
	Block_Buffer buf;
	buf.make_message(MONGO_KNIGHT_TRIALS_LOAD_FIGHTER_PLAYER);
	buf.write_int64(player_self()->role_id());
	buf.write_int64(KRoleInfo.role_id);
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);

	return 0;
}

int Knight_Trials::local_callback_knight_trals_enter_fighter(Knight_Trials_Player &kplayer) {
	Block_Buffer buf;
	buf.make_message(INNER_KNIGHT_TRIALS_ENTER_FIGHTER);

	buf.write_int8(kplayer.base.career);
	buf.write_int8(kplayer.base.gender);
	buf.write_string(kplayer.base.role_name);
	int size = kplayer.base.avatar_vec.size();
	buf.write_int8(size);
	for (Int_Vec::iterator it = kplayer.base.avatar_vec.begin();
			it != kplayer.base.avatar_vec.end(); ++it) {
		buf.write_int32(*it);
	}

	kplayer.fighter.serialize(buf);

	{
		Logic_Player *logic_player = POOL_MANAGER->pop_logic_player();
		if (!logic_player) {
			MSG_DEBUG("logic_player_pool_.pop() return 0.");
			return -1;
		}
		logic_player->load_base_detail(kplayer.base);
		logic_player->Logic_Fighter::load_detail(kplayer.fighter);
		logic_player->Logic_Heroer::load_detail(kplayer.heroer);
		// 战斗英雄信息
		bool has_fight_hero = false;
		int fight_hero_index = logic_player->heroer_detail().fighter_index;
		Heroer_Detail::Hero_Map::const_iterator it_hero = logic_player->heroer_detail().hero_map.find(fight_hero_index);
		if (it_hero != logic_player->heroer_detail().hero_map.end()) {
			has_fight_hero = true;
			buf.write_int8(1);
			logic_player->refresh_heroer_base_detail();
			logic_player->heroer_detail().hero_base_detail_.serialize(buf);

			it_hero->second.hero_fighter_detail.serialize(buf);
		}
		if (has_fight_hero == false) {
			buf.write_int8(0);
		}
		POOL_MANAGER->push_logic_player(logic_player);
		POOL_MANAGER->push_knight_trials_player_data_pool(&kplayer);
	}

//	kplayer.heroer.serialize_fighter(buf);

	buf.finish_message();

	player_self()->send_to_scene(buf);
	return 0;
}

int Knight_Trials::req_up_hide_knight_trials_palyer(void) {
	MSG_DEBUG("req_up_hide_knight_trials_palyer");
	//判断是否能重置
	if(knight_trials_detail_->reset_status || knight_trials_detail_->match_day <= 1 ||
			!knight_trials_player_detail_->hide_player_fighter_vec.size()) {
		return ERROR_CLIENT_PARAM;
	}
	//重置数据
	knight_trials_detail_->cur_points = 0;
	knight_trials_detail_->cur_receive = 0;
	knight_trials_detail_->reset_status = true;
	knight_trials_detail_->reward.clear();
	knight_trials_detail_->detail_change();
	//重置挑战玩家
	knight_trials_player_detail_->cur_player_fighter_vec = knight_trials_player_detail_->hide_player_fighter_vec;
	knight_trials_player_detail_->hide_player_fighter_vec.clear();
	knight_trials_player_detail_->detail_change();
	//置换数据库
	Block_Buffer buf;
	buf.make_message(MONGO_KNIGHT_TRIALS_RESERT_PLAYER);
	buf.write_int64(player_self()->role_id());
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	//回应客户端
	MSG_50107104 msg;
	THIS_SEND_TO_CLIENT(msg);
	//发送最新数据到客户端
	req_open_knight_trials_plan();
	return 0;
}

int Knight_Trials::req_receive_knight_trials_reward_box(void) {
	if(knight_trials_detail_->cur_points <= knight_trials_detail_->cur_receive) {
		return ERROR_IS_RECEIVE_ED;//已经领取过奖励
	}
	//如果还没生成奖励;就生成奖励
	Record_Box_Reward_Conf_Map::iterator rit = knight_trials_detail_->recode_box_reward.find(knight_trials_detail_->cur_points);
	if(rit == knight_trials_detail_->recode_box_reward.end()) {
		refres_box_reward_on_rand(knight_trials_detail_->cur_points);
		rit = knight_trials_detail_->recode_box_reward.find(knight_trials_detail_->cur_points);
	}
	//如果还是没有; 那就肯定有很严重的问题
	if(rit == knight_trials_detail_->recode_box_reward.end()) {
		return ERROR_INNER_SERVER;
	}
	//获取奖励
	Money_Add_List money_list;
	money_list.clear();
	Item_Vec item_list;
	item_list.clear();
	MSG_81000102 acv_msg;
	acv_msg.type = 0; //背包特效
	for(Record_Box_Reward_List::iterator it = rit->second.begin(); it != rit->second.end(); ++it) {
		switch(it->type) {
			case KNIGHT_TRIALS_BOX_REWARD_CONF_TYPE_MONEY: {
				if(it->num) {
					money_list.push_back(Money_Add_Info(Money_Type(it->id), it->num, Money_DM_Info(MONEY_ADD_KNIGHT_TRIALS)));
					acv_msg.property.push_back(Property(it->id, it->num));
				}
				break;
			}
			case KNIGHT_TRIALS_BOX_REWARD_CONF_TYPE_ITEM: {
				if(it->num) {
					Item_Detail item(it->id, it->num, Item_Detail::BIND);
					item_list.push_back(item);
					Item_Basic_Info ibi;
					item.set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
				}
				break;
			}
		}
	}
	if(!item_list.empty()) {
		int res = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_KNIGHT_TRIALS));
		if(res && res != ERROR_PACK_FULL_BUT_MAIL) {
			return res;
		}
	}
	if(!money_list.empty()) {
		int res = player_self()->pack_add_money(money_list);
		if(res) {
			return res;
		}
	}
	//VIP的监听(世界之树)
	for(Money_Add_List::iterator it = money_list.begin(); it != money_list.end(); ++it) {
		player_self()->listen_vip_reward(VIP_WORLD_TREE_KNIGHT_TRIALS, it->type, it->nums);
	}
	//记录累计获取奖励
	++knight_trials_detail_->cur_receive;
	MSG_50107103 msg;
	msg.reset();
	Uint_Uint uu;
	for(Record_Box_Reward_List::iterator it = rit->second.begin(); it != rit->second.end(); ++it) {
		if(it->num) {
			uu.reset();
			uu.val_1 = it->id;
			uu.val_2 = it->num;
			update_add_up_reward(uu);//记录领取的奖励到累计领取奖励中
			msg.receive_reward.push_back(uu);
		}
	}
	//回应客户端
	msg.total_reward = knight_trials_detail_->reward;
	THIS_SEND_TO_CLIENT(msg);
	if(acv_msg.property.size() || acv_msg.item_info.size()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	knight_trials_detail_->detail_change();

	MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`) VALUES (%d,%ld,%ld,%d)",  8,  player_self()->role_id(), Time_Value::gettimeofday().sec(),
			player_self()->level());
	return 0;
}

int Knight_Trials::req_open_knight_trials_reward_box(void) {
	if(knight_trials_detail_->cur_points <= knight_trials_detail_->cur_receive) {
		return ERROR_IS_RECEIVE_ED;//已经领取过奖励
	}
	//如果还没生成奖励;就生成奖励
	Record_Box_Reward_Conf_Map::iterator rit = knight_trials_detail_->recode_box_reward.find(knight_trials_detail_->cur_points);
	if(rit == knight_trials_detail_->recode_box_reward.end() || !rit->second.size()) {
		refres_box_reward_on_rand(knight_trials_detail_->cur_points);
		rit = knight_trials_detail_->recode_box_reward.find(knight_trials_detail_->cur_points);
	}
	//如果还是没有; 那就肯定有很严重的问题
	if(rit == knight_trials_detail_->recode_box_reward.end()) {
		return ERROR_INNER_SERVER;
	}
	//回应客户端
	MSG_50107102 msg;
	msg.reset();
	Uint_Uint uu;
	for(Record_Box_Reward_List::iterator it = rit->second.begin(); it != rit->second.end(); ++it) {
		uu.reset();
		uu.val_1 = it->id;
		uu.val_2 = it->num;
		msg.reward.push_back(uu);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

//void Knight_Trials::set_to_uuid(int64_t uuid) {
//	knight_trials_detail_->to_uuid = uuid;
//}
//
//void Knight_Trials::set_to_role_id(role_id_t role_id) {
//	knight_trials_detail_->to_role_id = role_id;
//}

void Knight_Trials::knight_trials_player_to_role_info(Knight_Trials_Player &player, Knight_Trials_Role_Info &value) {
	value.role_id = player.base.role_id;
	value.role_name = player.base.role_name;
	value.head_id = player.base.head_id;
	value.gender = player.base.gender;
	value.career = player.base.career;
	value.level = player.base.level;
	{//vip
		Time_Value now = Time_Value::gettimeofday();
		if(now.sec() > player.base.vip_over_time.sec()) {
			value.vip = player.base.vip;
		} else {
			value.vip = 0;
		}
	}
	value.server_name = player.server_name;
	value.force = player.fighter.force;
	{//hero
		if(player.heroer.fighter_index) {
			Heroer_Detail::Hero_Map::iterator hero_it = player.heroer.hero_map.find(player.heroer.fighter_index);
			if(hero_it != player.heroer.hero_map.end()) {
				value.hero_id = hero_it->second.hero_id;
				value.hero_force = hero_it->second.hero_fighter_detail.force;
				value.hero_level = hero_it->second.level;
				value.hero_name = hero_it->second.hero_name;
			}
		}
	}
	value.mount_id = player.base.mount_model_id;
	value.fashion = player.base.avatar_vec;
	value.gang_name =player.base.gang_name;
}
