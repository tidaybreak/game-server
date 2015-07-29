/*
 * Public.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "pack/Packer.h"
#include "Public.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Public_Struct.h"
#include "Logic_Module_Head.h"
#include "Msg_Inner_Struct.h"
#include "pack/Pack_Struct.h"
#include "Msg_Task_Struct.h"
#include "Err_Code.h"
#include "Msg_Active_Struct.h"
#include "Logic_Player_Struct.h"
#include "Config_Cache_Public.h"
#include "Msg_Guide_Struct.h"
#include "Msg_Role_Scene_Struct.h"
#include "Config_Cache_Fighter.h"
#include "Msg_Other_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_Hero.h"
#include "Msg_Battle_Scene_Struct.h"
#include "Msg_Hero_Struct.h"
#include "equiper/Equiper_Func.h"
#include "equiper/Equiper_Def.h"
#include "mail/Mail_Struct.h"
#include "warer/War_Scene_Active_Manager.h"
#include "Scene_Def.h"
#include "vip/Vip_Def.h"
#include "Config_Cache_Flowers.h"
#include "secret_shop/Secret_Shop_Manager.h"
#include "gang/Gang_Manager.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Secret_Shop.h"
#include "Config_Cache_Dungeon.h"
#include "campfire/Campfire_Manager.h"
#include "Msg_World_Boss_Struct.h"
#include "world_boss/Gang_Boss_Active_Manager.h"
#include "world_boss/World_Boss_Active_Manager.h"
#include "honor_arena/Honor_Arena_Active_Manager.h"
#include "active/Active_Manager.h"
#include "helper/Helper_Def.h"
#include "DB_Def.h"
#include "Logic_Public.h"
#include "Msg_Task_Struct.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Recharge.h"
#include "task/Tasker.h"
#include "recharge/Recharge_Rebate_Manager.h"
#include "recharge/Two_Recharge_Manager.h"
#include "Config_Cache_Item.h"
#include "reward/Reward_Def.h"
#include "team/Team.h"
#include "Config_Cache.h"
#include "heroer/Logic_Heroer_Struct.h"
#include "Msg_Arena_Struct.h"
#include "ranking/Ranking_Manager.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "pack/Pack_Func.h"
#include "Config_Cache_Mail.h"
#include "recharge/First_Recharge_Manager.h"
#include "recharge/Limit_Time_Recharge_Manager.h"
#include "oper_activity/Oper_Activity_Def.h"
#include "arena/Arena_Manager.h"
#include "Config_Cache_Arena.h"

const int WAR_SCENE_ID = 80207001;
const int PIECES_ITEM_ID = 20705401;  //粉钻碎片
const int RENAME_ITEM_ID = 20601404;

const int world_boss_act_icon_id = 16;

Public::Public(void) {
	reset();
}

Public::~Public() {
	// TODO Auto-generated destructor stub
}

void Public::reset(void) {
	public_detail_ = 0;

	pwoer_switch_.clear();

	un_broadcast_ = false;
}

int Public::create_init(Public_Detail &detail, const Base_Detail &base_detail) {
	//初始化系统设置
	const Sys_Cfg &sys_cfg = CONFIG_CACHE_PUBLIC->sys_config();
	detail.is_total_open = sys_cfg.is_total_open;
	detail.is_open_music = sys_cfg.is_open_music;
	detail.music_num = sys_cfg.music_num;
	detail.is_open_voice = sys_cfg.is_open_voice;
	detail.voice_num = sys_cfg.voice_num;
	detail.is_hide_role_model = sys_cfg.is_hide_role_model;
	detail.is_hide_hero_model = sys_cfg.is_hide_hero_model;
	detail.is_hide_title = sys_cfg.is_hide_title;
	detail.player_count = sys_cfg.default_player_count;
//	detail.is_skip_plot = sys_cfg.is_skip_plot;

	init_artifacts(detail, base_detail);

	return 0;
}

void Public::load_detail(Public_Detail &detail) {
	public_detail_ = &detail;
}

void Public::save_detail(Public_Detail &detail) {
	if (public_detail().is_change) {
		detail = public_detail();
		public_detail_->is_change = false;
	}
}

void Public::module_init(void) {
	init_artifacts();
	refresh_exploit_lvl();
	reset_recharge_rebate_data();
	guide_init();
	reset_two_recharge();
	reset_limit_time_recharge();
}

int Public::client_ready_now(void) {

	{
		//通知客户端防沉迷登陆提示
		MSG_82200101 msg;
		msg.reset();
		msg.online_time = public_detail_->add_up_online_time;
		msg.fcm = public_detail_->fcm;
		THIS_SEND_TO_CLIENT(msg);
	}

	//通知不能引导的列表
	MSG_80100500 guide_msg;
	guide_msg.reset();
	const Guide_NUM_Map &guideMap = public_detail_->guide_num_map;
	for (Guide_NUM_Map::const_iterator it = guideMap.begin();
			it != guideMap.end(); ++it) {
		Guide_Config_Map::const_iterator it_cfg =
		CONFIG_CACHE_PUBLIC->guide_config().find(it->first);
		if (it_cfg == CONFIG_CACHE_PUBLIC->guide_config().end()) {
			continue;
		}

		if (it->second >= it_cfg->second.max_num) {
			guide_msg.hand_list.push_back(it->first);
		}
	}
	THIS_SEND_TO_CLIENT(guide_msg);

	{
		if(public_detail_->task_guide_id) {
			MSG_81110021 msg;
			msg.guide_id = public_detail_->task_guide_id;
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	// 战场活动图片状态
	send_war_scene_active_to_client();
	// 篝火晚会活动图片状态
	send_bonfire_active_to_client();
	// 加载信息到篝火场景
	CAMPFIRE_MANAGER->load_player_info(this->player_self());
	// 公会boss图标
	{
		int state = 0;
		int remain_time = 0;
		GANG_MANAGER->get_gang_boss_act_state(player_self()->gang_id(), state, remain_time);
		if(state == 2 || state == 3){
			send_gang_boss_active_to_client(state, remain_time);
		}
	}


	{  //收到的鲜花
		if (public_detail_->flowers_info_vec.size()) {
			MSG_82200240 act_msg;
			act_msg.reset();
			act_msg.info = public_detail_->flowers_info_vec;
			THIS_SEND_TO_CLIENT(act_msg);
		}
	}

	if (!un_broadcast_) {
		{  // 广播剧情信息
			MSG_81110009 acv_msg;
			Plot_Info info;
			for (String_Int_Map::iterator it =
					this->public_detail_->plot_info.begin();
					it != this->public_detail_->plot_info.end(); ++it) {
				info.reset();
				info.plot_id = it->first;
				info.num = it->second;
				acv_msg.info_list.push_back(info);
			}
			THIS_SEND_TO_CLIENT(acv_msg);
		}

		{
			// 剧情信息
			MSG_81000099 msg;
			msg.plot_id = public_detail().plot_id;
			msg.plot_step = public_detail().plot_step;
			THIS_SEND_TO_CLIENT(msg);
		}
	} else {
		// 广播剧情信息
		MSG_81110009 acv_msg;
		Plot_Info info;
		for (String_Int_Map::iterator it =
				this->public_detail_->plot_info.begin();
				it != this->public_detail_->plot_info.end(); ++it) {
			it->second += 1;
			info.reset();
			info.plot_id = it->first;
			info.num = it->second;
			acv_msg.info_list.push_back(info);
		}
		THIS_SEND_TO_CLIENT(acv_msg);
		un_broadcast_ = false;
		this->public_detail_->is_change = true;
	}
	send_equip_look();

	Time_Value now = Time_Value::gettimeofday();
	{//首充图标通知
		uint32_t continue_time = CONFIG_CACHE_RECHARGE->get_common_config_by_key(RCCT_FIRST_RECHARGE_TIME)*Time_Value::ONE_MINUTE_IN_SECS;
		if (public_detail_->first_recharge < First_Recharge_Receive
				&& (uint32_t)player_self()->level() >= CONFIG_CACHE_RECHARGE->get_common_config_by_key(RCCT_FIRST_RECHRAGE_START_LEVEL)
				&& now.sec() < (public_detail_->start_frist_recharge_time.sec()+ continue_time)) {
			MSG_81000408 msg;
			msg.open = 1;
			msg.active = public_detail_->first_recharge;
			msg.time = (public_detail_->start_frist_recharge_time.sec()+ continue_time) - now.sec();
			msg.icon_id = 8;
			THIS_SEND_TO_CLIENT(msg);
		}
	}
	{//限时充值活动图标通知
		if (public_detail_->limit_time_recharge < First_Recharge_Receive
				&& (uint32_t)player_self()->level() >= LIMIT_TIME_RECHARGE_MANAGER->get_icon_limit_level()
				&& now.sec() < LIMIT_TIME_RECHARGE_MANAGER->get_end_time().sec() && now.sec() >= LIMIT_TIME_RECHARGE_MANAGER->get_start_time().sec()) {
			MSG_81000408 msg;
			msg.open = 1;
			msg.active = public_detail_->limit_time_recharge;
			msg.time = 0;
			msg.icon_id = LIMIT_TIME_RECHARGE_MANAGER->get_icon_time_status().icon_id;
			THIS_SEND_TO_CLIENT(msg);
		}
	}
	player_self()->first_over_receive_to_mail();

	uint32_t level_limit = RECHARGE_REBATE_MANAGER->get_icon_limit_level();
	Icon_Time_Info &icon_time = RECHARGE_REBATE_MANAGER->get_icon_time_status();
	if((uint32_t)player_self()->level() >= level_limit &&
			now.sec() >= RECHARGE_REBATE_MANAGER->get_start_time().sec() &&
			now.sec() < RECHARGE_REBATE_MANAGER->get_end_time().sec()) {
		MSG_81000408 msg;
		msg.open = 1;
		msg.icon_id = icon_time.icon_id;
		THIS_SEND_TO_CLIENT(msg);
	}


	{
		MSG_82300036 msg_82300036;
		for (Played_Plot_Set::const_iterator it =
				public_detail_->played_plot_set.begin();
				it != public_detail_->played_plot_set.end(); ++it) {
			msg_82300036.plot_id_list.push_back(*it);
		}
		THIS_SEND_TO_CLIENT(msg_82300036);
	}

	{
		if(public_detail_->notice_id && public_detail_->is_show) {
			MSG_81110013 msg;
			msg.id = public_detail_->notice_id;
			msg.type = public_detail_->is_show ? 1 : 0;
			msg.is_play = public_detail_->is_played_effect ? 1 : 0;
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	{//调查问卷图标
		const Questionnaire_Config_Map &conf = CONFIG_CACHE_PUBLIC->get_questionnaire_config();
		if(public_detail_->ans_qid.size() < conf.size()) {
			MSG_81000408 msg;
			msg.open = 1;
			msg.active = 1;
			msg.time = 0;
			msg.icon_id = 14;
			THIS_SEND_TO_CLIENT(msg);
		}
	}
	//老玩家回归礼包发放
	if(!public_detail_->is_receive_old_player_gift && CONFIG_CACHE_PUBLIC->has_old_player(player_self()->base_detail().account_info.account_name)) {
		int res = LOGIC_PUBLIC->send_mail(OLD_PLAYER_GIFT_MAIL_ID, player_self()->role_id());
		if(res == 0) {
			public_detail_->is_receive_old_player_gift = true;
			public_detail_->detail_change();
		} else {
			MSG_DEBUG("player:%ld receive old player gift mail error , res:%d", player_self()->role_id(), res);
		}
	}

	req_receive_invite_reward_info(0);
	send_today_get_bind_gold_active();
	req_war_scene_ex_result();
	return 0;
}

void Public::sign_in(void) {
	this->create_event(Logic_Enum::EVENT_LOGIC_EQUIPER_TICK,
			&Public::equip_tick, Time_Value(EQUIPER_TICK_SEC));

	//防沉迷时间提示和超时下线
	if (public_detail_->fcm) {
		if (Time_Value::gettimeofday().sec()
				- player_self()->base_detail().last_sign_out_time.sec()
				>= FCM_RESET_TIME) {
			public_detail_->add_up_online_time = 0;
			public_detail_->is_change = true;
		}
		int this_event_h = public_detail_->add_up_online_time / FCM_STEP_TIME;
		switch (this_event_h) {
		case 0: {
			int next_event_time = FCM_STEP_TIME
					- (public_detail_->add_up_online_time % FCM_STEP_TIME);
			this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_1h,
					&Public::fcm_event_1h, Time_Value(next_event_time), 1);
			break;
		}
		case 1: {
			int next_event_time = FCM_STEP_TIME
					- (public_detail_->add_up_online_time % FCM_STEP_TIME);
			this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_2h,
					&Public::fcm_event_2h, Time_Value(next_event_time), 1);
			break;
		}
		case 2: {
			int next_event_time = FCM_STEP_TIME
					- (public_detail_->add_up_online_time % FCM_STEP_TIME);
			if (next_event_time > FCM_LAST_TIP_TIME) {
				this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_255h,
						&Public::fcm_event_255h,
						Time_Value(next_event_time - FCM_LAST_TIP_TIME), 1);
			} else {
				this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_3h,
						&Public::fcm_event_3h, Time_Value(next_event_time), 1);
			}
			break;
		}
		default:
			this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_3h,
					&Public::fcm_event_3h, Time_Value(0), 1);
		}
	}

//	stream_login登入流水表字段需要:
//
//	account_name 玩家账号
//	player_id    角色id
//	level        玩家当前等级
//	client_ip    客户端登录ip
//	login_time   登录时间戳
//	create_time  创号时间
//	last_login_time 上次登录时间戳
	MSG_STREAM("`stream_login`(`account_name`, `player_id`, `level`, `client_ip`, `login_time`, `create_time`, `last_login_time`)"" VALUES ('%s',%ld,%d,'%s',%ld,%ld,%ld)",
			player_self()->base_detail().account_info.account_name.c_str(),
			player_self()->role_id(),
			player_self()->base_detail().level,
			player_self()->base_detail().ip.c_str(),
			Time_Value::gettimeofday().sec(),
			player_self()->base_detail().create_time.sec(),
			player_self()->base_detail().last_sign_in_time.sec());

}

void Public::fcm_event_1h(void) {
	this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_2h,
			&Public::fcm_event_2h, Time_Value(FCM_STEP_TIME), 1);
	//通知客户端
	MSG_82200100 msg;
	msg.reset();
	msg.online_time = FCM_STEP_SHOW_TIME;
	THIS_SEND_TO_CLIENT(msg);
}

void Public::fcm_event_2h(void) {
	this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_255h,
			&Public::fcm_event_255h,
			Time_Value(FCM_STEP_TIME - FCM_LAST_TIP_TIME), 1);
	//通知客户端
	MSG_82200100 msg;
	msg.reset();
	msg.online_time = FCM_STEP_SHOW_TIME * 2;
	THIS_SEND_TO_CLIENT(msg);
}

void Public::fcm_event_255h(void) {
	this->create_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_3h,
			&Public::fcm_event_3h, Time_Value(FCM_LAST_TIP_TIME), 1);
	//通知客户端
	MSG_82200100 msg;
	msg.reset();
	msg.online_time = FCM_STEP_SHOW_TIME * 3 - FCM_LAST_SHOW_TIME;
	THIS_SEND_TO_CLIENT(msg);
}

void Public::fcm_event_3h(void) {
	player_self()->sign_out(ERROR_FCM_LOGOUT);
}

int Public::req_fcm_pass(int fcm) {
	public_detail_->fcm = fcm;
	DB_View::ID_Player_View_Map::iterator find_it =
	LOGIC_MONITOR->db_view()->id_player_view_map.find(this->role_id());
	if (find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		find_it->second->fcm = public_detail_->fcm;
	}

	if (fcm == 0) {
		this->remove_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_1h);
		this->remove_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_2h);
		this->remove_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_255h);
		this->remove_event(Logic_Enum::EVENT_PUBLIC_FCM_OPERATING_3h);
	}

	return 0;
}

void Public::sign_out(void) {
	if (public_detail_->fcm) {
		public_detail_->add_up_online_time += Time_Value::gettimeofday().sec()
				- player_self()->base_detail().last_sign_in_time.sec();
		public_detail_->is_change = true;
	}
	// 登出流水
//	CREATE TABLE `stream_login_logout` (
//	  `player_id` bigint(20) unsigned NOT NULL COMMENT '角色id',
//	  `level` tinyint(3) unsigned NOT NULL COMMENT '玩家当前等级',
//	  `client_ip` char(15) NOT NULL COMMENT '客户端ip',
//	  `login_time` int(10) unsigned NOT NULL COMMENT '登录时间戳',
//	  `logout_time` int(10) unsigned NOT NULL COMMENT '登出时间戳',
//	  `create_time` int(10) NOT NULL COMMENT '创号时间戳',
//	  `online_time` int(10)  unsigned NOT NULL COMMENT '在线时间戳',
//	  KEY `player_id` (`player_id`) USING BTREE,
//	  KEY `login_time` (`login_time`) USING BTREE,
//	  KEY `logout_time` (`logout_time`) USING BTREE,
//	  KEY `create_time` (`create_time`) USING BTREE
//	) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家登出流水表';
	MSG_STREAM(
			"`stream_login_logout`(`player_id`, `level`, `client_ip`, `login_time`, `logout_time`, `create_time`, `online_time`)" " VALUES (%ld,%d,'%s',%d,%d,%d,%d)",
			this->role_id(), player_self()->base_detail().level,
			player_self()->base_detail().ip.c_str(),
			player_self()->base_detail().last_sign_in_time.sec(),
			player_self()->base_detail().last_sign_out_time.sec(),
			player_self()->base_detail().create_time.sec(),
			player_self()->base_detail().last_sign_out_time.sec()
					- player_self()->base_detail().last_sign_in_time.sec());

}

void Public::trusteeship_sign_out(void) {
	if (public_detail_->fcm) {
		public_detail_->add_up_online_time += Time_Value::gettimeofday().sec()
				- player_self()->base_detail().last_sign_in_time.sec();
		public_detail_->is_change = true;
	}
}

int Public::get_drops_goods(const int scene_type, const int scene_id, role_id_t drops_role_id,
		std::vector<Drops_Item>& drops_item_vec, int item_index) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	int exp = 0;
	Money_Add_List money_list;
	Item_Vec pack_item_vec;
	int count = drops_item_vec.size();
	for (int i = 0; i < count; ++i) {
		if (drops_item_vec[i].item_id == COPPER || drops_item_vec[i].item_id == GOLD ||
				drops_item_vec[i].item_id == BIND_GOLD || drops_item_vec[i].item_id == DRAGON_SOUL ||
				drops_item_vec[i].item_id == SOULS || drops_item_vec[i].item_id == FRIENDSHIP ||
				drops_item_vec[i].item_id == REPUTATION) {
			money_list.push_back(
					Money_Add_Info(Money_Type(drops_item_vec[i].item_id),
							drops_item_vec[i].item_amount, MONEY_ADD_DROPS));
			acv_msg.property.push_back(
					Property(drops_item_vec[i].item_id,
							drops_item_vec[i].item_amount));
		} else if (drops_item_vec[i].item_id == PT_EXP_CURRENT) {
			exp = drops_item_vec[i].item_amount;
		} else {
			make_item_vec(pack_item_vec, drops_item_vec[i].item_id,
					drops_item_vec[i].item_amount, drops_item_vec[i].bind);
		}
	}

	Item_Basic_Info item_base;
	Card_Info card_info;
	for (Item_Vec::const_iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it) {
		item_base.reset();
		(*it).set_item_basic_info(item_base);
		acv_msg.item_info.push_back(item_base);
		card_info.reset();
		if ((*it).set_item_card_info(card_info)) {
			acv_msg.card_info_vec.push_back(card_info);
		}
	}

	int ret = 0;
	if (!pack_item_vec.empty()) {
		ret = this->pack_try_insert_item(PACK_T_PACKAGE_INDEX, pack_item_vec);
	}
	if (ret != 0) {
		MSG_20300000 res_msg;
		res_msg.drops_role_id = drops_role_id;
		res_msg.item_index = -2;
		THIS_SEND_TO_SCENE(res_msg);
	} else {
		if (exp > 0) {
			Exp_All exp_all = player_self()->modify_experience_inter(exp, true, true);
			if (0 == exp_all.result) {
				acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
				acv_msg.vip_exp = exp_all.vip_exp;
				acv_msg.world_level = player_self()->get_world_level_add();
			}
		}
		if (money_list.empty() == false) {
			this->pack_add_money(money_list);
			if (scene_type == FuBen_Scene_Type) {
				int count = money_list.size();
				for (int i = 0; i < count; ++i) {
					this->player_self()->listen_vip_reward(
							VIP_WORLD_TREE_SINGLE_EXPLORE, money_list[i].type,
							money_list[i].nums);
				}
			}
		}
		if (pack_item_vec.empty() == false) {
			this->pack_insert_item(PACK_T_PACKAGE_INDEX, pack_item_vec,
					Gain_Item_DM_Info(ITEM_GAIN_DROPS_GET));
			// 副本获得侦听
			std::vector<Item_Info> gain_items;
			for(Item_Vec::iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it){
				Item_Info item;
				item.id = it->id_;
				item.amount = it->amount_;
				item.bind = it->bind_;
				gain_items.push_back(item);
			}
			player_self()->hook_fb_gain_item(scene_type, scene_id, gain_items);
		}
		MSG_20300000 res_msg;
		res_msg.drops_role_id = drops_role_id;
		res_msg.item_index = item_index;
		THIS_SEND_TO_SCENE(res_msg);
		if (0 == ret
				&& (!acv_msg.item_info.empty() || !acv_msg.property.empty())) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
	}
	return ret;
}

int Public::add_drops_to_package(const int scene_type, const int scene_id, std::vector<Drops_Item>& drops_item_vec) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;

	int exp = 0;
	Money_Add_List money_list;
	Item_Vec pack_item_vec;
	int count = drops_item_vec.size();
	for (int i = 0; i < count; ++i) {
		if (drops_item_vec[i].item_id == COPPER || drops_item_vec[i].item_id == GOLD
						|| drops_item_vec[i].item_id == BIND_GOLD || drops_item_vec[i].item_id == DRAGON_SOUL
						|| drops_item_vec[i].item_id == SOULS || drops_item_vec[i].item_id == FRIENDSHIP ||
						drops_item_vec[i].item_id == REPUTATION) {
			money_list.push_back(
					Money_Add_Info(Money_Type(drops_item_vec[i].item_id), drops_item_vec[i].item_amount, MONEY_ADD_DROPS));
			acv_msg.property.push_back(Property(drops_item_vec[i].item_id, drops_item_vec[i].item_amount));

		}else if(drops_item_vec[i].item_id == PT_EXP_CURRENT){
			exp = drops_item_vec[i].item_amount;
		}else{
			make_item_vec(pack_item_vec, drops_item_vec[i].item_id,
								drops_item_vec[i].item_amount, drops_item_vec[i].bind);
		}
	}

	Item_Basic_Info item_base;
	Card_Info card_info;
	for (Item_Vec::const_iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it) {
		item_base.reset();
		(*it).set_item_basic_info(item_base);
		acv_msg.item_info.push_back(item_base);
		card_info.reset();
		if ((*it).set_item_card_info(card_info)) {
			acv_msg.card_info_vec.push_back(card_info);
		}
	}

	if(exp > 0){
		Exp_All exp_all = player_self()->modify_experience_inter(exp, true, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if (money_list.empty() == false) {
		this->pack_add_money(money_list);
		if (scene_type == FuBen_Scene_Type) {
			int count = money_list.size();
			for (int i = 0; i < count; ++i) {
				this->player_self()->listen_vip_reward(
						VIP_WORLD_TREE_SINGLE_EXPLORE, money_list[i].type,
						money_list[i].nums);
			}
		}
	}
	int ret = 0;
	if(pack_item_vec.empty() == false){
		ret = pack_insert_item(PACK_T_PACKAGE_INDEX, pack_item_vec,
					Gain_Item_DM_Info(ITEM_GAIN_DROPS_GET));
		// 副本获得侦听
		std::vector<Item_Info> gain_items;
		for(Item_Vec::iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it){
			Item_Info item;
			item.id = it->id_;
			item.amount = it->amount_;
			item.bind = it->bind_;
			gain_items.push_back(item);
		}
		player_self()->hook_fb_gain_item(scene_type, scene_id, gain_items);
	}
	if ((!acv_msg.item_info.empty() || !acv_msg.property.empty())) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return ret;
}

int Public::gather_goods(role_id_t material_role_id,
		std::vector<Drops_Item>& drops_item_vec) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	int exp = 0;
	Money_Add_List money_list;
	Item_Vec pack_item_vec;
	int count = drops_item_vec.size();
	for (int i = 0; i < count; ++i) {
		if (drops_item_vec[i].item_id == COPPER || drops_item_vec[i].item_id == GOLD
						|| drops_item_vec[i].item_id == BIND_GOLD || drops_item_vec[i].item_id == DRAGON_SOUL
						|| drops_item_vec[i].item_id == SOULS || drops_item_vec[i].item_id == FRIENDSHIP ||
						drops_item_vec[i].item_id == REPUTATION) {
			money_list.push_back(
					Money_Add_Info(Money_Type(drops_item_vec[i].item_id), drops_item_vec[i].item_amount, MONEY_ADD_DROPS));
			acv_msg.property.push_back(Property(drops_item_vec[i].item_id, drops_item_vec[i].item_amount));
		}else if(drops_item_vec[i].item_id == PT_EXP_CURRENT){
			exp = drops_item_vec[i].item_amount;
		}else{
			Item_Detail item(drops_item_vec[i].item_id, drops_item_vec[i].item_amount,
					drops_item_vec[i].bind);
			pack_item_vec.push_back(item);
			Item_Basic_Info item_base;
			item.set_item_basic_info(item_base);
			acv_msg.item_info.push_back(item_base);
		}

	}
	if(exp > 0){
		Exp_All exp_all = player_self()->modify_experience_inter(exp, true, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if (money_list.empty() == false) {
		this->pack_add_money(money_list);
	}
	int ret = 0;
	if(pack_item_vec.empty() == false){
		ret = pack_insert_item(PACK_T_PACKAGE_INDEX, pack_item_vec,
					Gain_Item_DM_Info(ITEM_GAIN_COLLECTION));
	}

	{
		MSG_20300012 msg;
		msg.role_id = material_role_id;
		msg.result = ret;
		THIS_SEND_TO_SCENE(msg);
	}

	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return ret;
}

int Public::add_goods(const int scene_type, const int scene_id, std::vector<Drops_Item>& drops_item_vec) {
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	int exp = 0;
	int exp_percent = 1;
	Money_Add_List money_list;
	Item_Vec pack_item_vec;
	int count = drops_item_vec.size();
	for (int i = 0; i < count; ++i) {
		if (drops_item_vec[i].item_id == COPPER || drops_item_vec[i].item_id == BIND_GOLD ||
				drops_item_vec[i].item_id == GOLD || drops_item_vec[i].item_id == DRAGON_SOUL ||
				drops_item_vec[i].item_id == SOULS || drops_item_vec[i].item_id == FRIENDSHIP ||
				drops_item_vec[i].item_id == REPUTATION) {
			money_list.push_back(
					Money_Add_Info((Pack::Money_Type) drops_item_vec[i].item_id,drops_item_vec[i].item_amount,
							Money_DM_Info(MONEY_ADD_DROPS)));
			acv_msg.property.push_back(
					Property(drops_item_vec[i].item_id, drops_item_vec[i].item_amount));
			if (scene_type == FuBen_Scene_Type) {
				this->player_self()->listen_vip_reward(
						VIP_WORLD_TREE_SINGLE_EXPLORE, drops_item_vec[i].item_id,
						drops_item_vec[i].item_amount);
			}
		}else if(drops_item_vec[i].item_id == PT_EXP_CURRENT){
			exp = drops_item_vec[i].item_amount;
			exp_percent = drops_item_vec[i].bind;
		}else {
			Item_Detail item(drops_item_vec[i].item_id, drops_item_vec[i].item_amount,
					drops_item_vec[i].bind);
			pack_item_vec.push_back(item);
			Item_Basic_Info item_base;
			item.set_item_basic_info(item_base);
			acv_msg.item_info.push_back(item_base);
		}
	}
	if(exp > 0){
		Exp_All exp_all = player_self()->modify_experience_inter(exp, true, true, false, true, exp_percent);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if (money_list.empty() == false) {
		this->pack_add_money(money_list);
	}
	int ret = 0;
	if(pack_item_vec.empty() == false){
		ret = pack_insert_item(PACK_T_PACKAGE_INDEX, pack_item_vec,
				Gain_Item_DM_Info(ITEM_GAIN_DROPS_GET));
		// 副本获得侦听
		std::vector<Item_Info> gain_items;
		for(Item_Vec::iterator it = pack_item_vec.begin(); it != pack_item_vec.end(); ++it){
			Item_Info item;
			item.id = it->id_;
			item.amount = it->amount_;
			item.bind = it->bind_;
			gain_items.push_back(item);
		}
		player_self()->hook_fb_gain_item(scene_type, scene_id, gain_items);
	}
	if(!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return ret;
}

int Public::task_handle_plot_info(const MSG_10110008 &msg) {
	MSG_50110008 res_msg;
	if (!msg.plot_id.empty()) {
		if (msg.action) {
			String_Int_Map::iterator it = this->public_detail_->plot_info.find(
					msg.plot_id);
			if (it != this->public_detail_->plot_info.end()) {
				if (!msg.type) {
					it->second += 1;
				}
			} else {
				this->public_detail_->plot_info.insert(
						String_Int_Map::value_type(msg.plot_id, 1));
			}
			// end plot send to scene
			MSG_20300020 scene_msg;
			scene_msg.plot_id = msg.plot_id;
			scene_msg.plot_step = 1;
			THIS_SEND_TO_SCENE(scene_msg);
		} else {
			MSG_20300020 scene_msg;
			String_Int_Map::iterator it = this->public_detail_->plot_info.find(
					msg.plot_id);
			if (it != this->public_detail_->plot_info.end()) {
				if (0 != msg.num && msg.num <= it->second) {
					res_msg.exist = 1;

					// end plot send to scene
					scene_msg.plot_step = 1;
				} else {
					it->second += 1;
				}
			} else if (msg.type) {
				this->public_detail_->plot_info.insert(
						String_Int_Map::value_type(msg.plot_id, 1));
			}
			if (public_detail_->plot_switch) {
				it = this->public_detail_->plot_info.find(msg.plot_id);
				if (it != this->public_detail_->plot_info.end()) {
					it->second += 1;
				} else {
					public_detail_->plot_info.insert(String_Int_Map::value_type(msg.plot_id, 1));
				}
				res_msg.exist = 1;
				scene_msg.plot_step = 1;
			}
			{
				// begin plot send to scene
				scene_msg.plot_id = msg.plot_id;
				THIS_SEND_TO_SCENE(scene_msg);
			}

			THIS_SEND_TO_CLIENT(res_msg);
		}
		this->public_detail_->is_change = true;
	}
	return 0;
}

int Public::task_clean_plot_info(String_Vec plot_ids) {
	for (String_Vec::const_iterator it = plot_ids.begin(); it != plot_ids.end();
			++it) {
		this->public_detail_->plot_info.erase(*it);
	}

	return 0;
}

int Public::set_power_switch(const Power_Type module,
		const int32_t switch_value) {
	Power_Map::iterator find_it = pwoer_switch_.find(module);
	if (find_it != pwoer_switch_.end()) {
		find_it->second = switch_value;
	} else {
		pwoer_switch_.insert(std::make_pair(module, switch_value));
	}
	return 0;
}

int Public::get_power_by_power_type(const Power_Type module) {
	Power_Map::iterator find_it = pwoer_switch_.find(module);
	if (find_it != pwoer_switch_.end()) {
		return (*find_it).second;
	}
	return 0;
}

int Public::set_plot_info(const std::string plot_id, int plot_step) {
	public_detail_->plot_id = plot_id;
	public_detail_->plot_step = plot_step;
	public_detail_->detail_change();
	return 0;
}

int Public::clean_plot_info(void) {
	public_detail_->plot_id.clear();
	public_detail_->plot_step = 0;
	public_detail_->detail_change();
	return 0;
}

int Public::req_plot_all_add_one(const MSG_10120110 &msg) {
	if (msg.type == 0) {
		public_detail_->plot_switch = true;
	} else {
		public_detail_->plot_switch = false;
	}
	public_detail_->detail_change();
	return 0;
}

int Public::sign_in_repeat(void) {
	pwoer_switch_.clear();
	public_plot_un_broadcast(false);
	return 0;
}

void Public::guide_init() {
	typedef boost::unordered_set<uint16_t> Temp_Set;
	Temp_Set temp_set;

	for(Guide_Step_Map::iterator it = public_detail_->guide_step_map.begin();
			it != public_detail_->guide_step_map.end();
			++it) {
		++it->second.num;
		if(it->second.num >= 2) {
			temp_set.insert(it->first);
			continue;
		}
	}

	for(Temp_Set::const_iterator it = temp_set.begin(); it != temp_set.end(); ++it) {
		public_detail_->guide_step_map.erase(*it);

		Guide_Config_Map::const_iterator it_cfg = CONFIG_CACHE_PUBLIC->guide_config().find(*it);
		if (it_cfg == CONFIG_CACHE_PUBLIC->guide_config().end()) {
			continue;
		}

		Guide_NUM_Map::iterator find_it = public_detail_->guide_num_map.find(*it);
		if (find_it == public_detail_->guide_num_map.end()) {
			find_it = public_detail_->guide_num_map.insert(
					std::make_pair(it_cfg->first, 0)).first;
		}

		if (find_it == public_detail_->guide_num_map.end()) {
			continue;
		}

		find_it->second = it_cfg->second.max_num;
	}

	temp_set.clear();
	public_detail_->detail_change();
}

int Public::req_guide(uint16_t guide_id) {
	MSG_50104000 guide_msg;
	guide_msg.id = guide_id;
	Guide_Config_Map::const_iterator it_cfg =
	CONFIG_CACHE_PUBLIC->guide_config().find(guide_id);
	if (it_cfg == CONFIG_CACHE_PUBLIC->guide_config().end()) {
		guide_msg.status = 0;
		THIS_SEND_TO_CLIENT(guide_msg);
//		return ERROR_GUIDE_NOT_EXIST;
		return 0;
	}

	//如果策划更新了配置，需要更新服务器数据
	Guide_NUM_Map::iterator it = public_detail_->guide_num_map.find(guide_id);
	if (it == public_detail_->guide_num_map.end()) {
		it = public_detail_->guide_num_map.insert(
				std::make_pair(it_cfg->first, 0)).first;
	}

	if (it == public_detail_->guide_num_map.end()) {
		guide_msg.status = 0;
		THIS_SEND_TO_CLIENT(guide_msg);
//		return ERROR_GUIDE_NOT_EXIST;
		return 0;
	}

	//已引导次数+1    注：客户端先引导，后发下一次是否引导的请求
	it->second = it->second + 1;

	//引导完成，删除引导步骤
	public_detail_->guide_step_map.erase(guide_id);

	public_detail_->detail_change();

	//引导次数超过最大，不能引导 注：客户端先引导，后发下一次是否引导的请求
	if (it->second >= it_cfg->second.max_num) {
		guide_msg.status = 0;
		THIS_SEND_TO_CLIENT(guide_msg);
		return 0;
	}

	guide_msg.status = 1;
	THIS_SEND_TO_CLIENT(guide_msg);

	return 0;
}

void Public::close_guide(uint16_t guide_id) {
	Guide_Config_Map::const_iterator it_cfg =
	CONFIG_CACHE_PUBLIC->guide_config().find(guide_id);
	if (it_cfg == CONFIG_CACHE_PUBLIC->guide_config().end()) {
		return;
	}
	Guide_NUM_Map::iterator it = public_detail_->guide_num_map.find(guide_id);
	if (it == public_detail_->guide_num_map.end()) {
		it = public_detail_->guide_num_map.insert(
				std::make_pair(it_cfg->first, it_cfg->second.max_num)).first;
	}

	if (it == public_detail_->guide_num_map.end()) {
		return;
	}

	it->second = it_cfg->second.max_num;
	//引导完成，删除引导步骤
	public_detail_->guide_step_map.erase(guide_id);
	public_detail_->detail_change();

	MSG_50104000 guide_msg;
	guide_msg.id = guide_id;
	guide_msg.status = 0;
	THIS_SEND_TO_CLIENT(guide_msg);
}

void Public::close_guide_by_type(Guide_Finish_Enum guide_finish_enum,
		uint64_t param1 /*= 0*/, uint64_t param2 /*= 0*/) {
	const Guide_Finish_Cfg_Map &guide_finish_cfg_map =
	CONFIG_CACHE_PUBLIC->get_guide_finish_cfg_map();
	Guide_Finish_Cfg_Map::const_iterator find_it = guide_finish_cfg_map.find(
			guide_finish_enum);
	for (Guide_Finish_Cfg_Map::const_iterator it = find_it;
			it != guide_finish_cfg_map.end()
					&& it != guide_finish_cfg_map.upper_bound(guide_finish_enum);
			++it) {
		switch (guide_finish_enum) {
		case GUIDE_FINISH_TYPE_DRAGON_EGG_BORN: {
			break;
		}
		case GUIDE_FINISH_TYPE_DRAGON_VALE_BUILDING_CREATE: {
			if (it->second.param1 != 0 && it->second.param1 != param1) {
				continue;
			}
			break;
		}
		case GUIDE_FINISH_TYPE_LEARN_ROLE_SKILL: {
			continue;
		}

		case GUIDE_FINISH_TYPE_LEARN_HERO_SKILL: {
			continue;
		}
		case GUIDE_FINISH_TYPE_GET_HERO: {
			if (it->second.param1 != 0 && it->second.param1 != param1) {
				continue;
			}
			break;
		}
		case GUIDE_FINISH_TYPE_HERO_TO_LEVEL: {
			if (it->second.param1 != 0 && it->second.param2 != 0
					&& (it->second.param1 != param1
							|| it->second.param2 != param2)) {
				continue;
			}
			break;
		}
		case GUIDE_FINISH_TYPE_GET_ITEM: {
			if (it->second.param1 != 0 && it->second.param1 != param1) {
				continue;
			}
			break;
		}
		case GUIDE_FINISH_TYPE_ROLE_HAS_A_EQUIP: {
			if (it->second.param1_list.find((int32_t) param1)
					== it->second.param1_list.end()) {
				continue;
			}
			break;
		}
		case GUIDE_FINISH_TYPE_HERO_HAS_A_EQUIP: {
			if (it->second.param1_list.find((int32_t) param1)
					== it->second.param1_list.end()) {
				continue;
			}
			break;
		}
		case GUIDE_FINISH_TYPE_HORSE_STAR_LEVEL: {
			if (it->second.param1 != 0 && it->second.param1 != param1) {
				continue;
			}

			if(it->second.param2 != 0 && it->second.param2 > param2) {
				continue;
			}
			break;
		}

		case GUIDE_FINISH_TYPE_WAKE_UP_HERO_TO_QUALITY_LEVEL: {
			if (it->second.param1 != 0 && it->second.param1 != param1) {
				continue;
			}

			if(it->second.param2 != 0 && it->second.param2 > param2) {
				continue;
			}
			break;
		}

		case GUIDE_FINISH_TYPE_HERO_SET_FIGHT: {
			if (it->second.param1 != 0 && it->second.param1 != param1) {
				continue;
			}

			int index = player_self()->heroer_get_hero_index(it->second.param1);
			Heroer_Detail::Hero_Map::const_iterator hero_it = player_self()->heroer_detail().hero_map.find(index);
			if(hero_it != player_self()->heroer_detail().hero_map.end()) {
				if(hero_it->second.is_task_tmp_hero) {
					continue;
				}
			}
			break;
		}

		default: {
			continue;
		}
		}  //end switch

		close_guide(it->second.guide_id);
	}  //end for
}

int Public::req_guide_add_item_in_plot(uint32_t item_id, uint32_t item_num) {
	if (!item_id || !item_num) {
		return 0;
	}

	const Item_Detail_Config *item_cfg = CONFIG_CACHE_ITEM->find_item(item_id);
	if (!item_cfg) {
		return ERROR_ITEM_NOT_EXIST;
	}

	int pack_item_amount = player_self()->pack_calc_item(
			Pack::PACK_T_PACKAGE_INDEX, item_id);
	if (pack_item_amount >= (int) item_num) {
		return 0;
	}

	Item_Detail item(item_id, item_num,
			static_cast<Item_Detail::Bind_Type>(Item_Detail::BIND));
	player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,
			Gain_Item_DM_Info(Pack::ITEM_GAIN_GUIDE_ADD_IN_PLOT));

	MSG_81000102 acv_msg;
	Item_Basic_Info item_basic;
	item.set_item_basic_info(item_basic);
	acv_msg.item_info.push_back(item_basic);
	if (!acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	return 0;
}

int Public::req_guide_set(uint16_t guide_id, uint8_t step) {
	Guide_Config_Map::const_iterator it_cfg =
	CONFIG_CACHE_PUBLIC->guide_config().find(guide_id);
	if (it_cfg == CONFIG_CACHE_PUBLIC->guide_config().end()) {
//		return ERROR_GUIDE_NOT_EXIST;
		return 0;
	}

	//如果策划更新了配置，需要更新服务器数据
	Guide_NUM_Map::iterator it = public_detail_->guide_num_map.find(guide_id);
	if (it == public_detail_->guide_num_map.end()) {
		it = public_detail_->guide_num_map.insert(
				std::make_pair(it_cfg->first, 0)).first;
	}

	if (it == public_detail_->guide_num_map.end()) {
		MSG_TRACE_ABORT("insert fail");
//		return ERROR_GUIDE_NOT_EXIST;
		return 0;
	}

//	public_detail_->guide_step_map.erase(guide_id);

	Guide_Step_Map::iterator guide_it = public_detail_->guide_step_map.find(guide_id);
	if(guide_it != public_detail_->guide_step_map.end()) {
		guide_it->second.step = step;
	}
	else {
		Guide_Step_Info guide_step_info;
		guide_step_info.step = step;
		guide_step_info.num = 0;
		public_detail_->guide_step_map.insert(std::make_pair(guide_id, guide_step_info));
	}

	public_detail_->detail_change();

	return 0;
}

int Public::req_save_played_plot(uint32_t plot_id) {
	public_detail_->played_plot_set.insert(plot_id);
	public_detail_->detail_change();
	MSG_50104002 msg;
	msg.plot_id = plot_id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Public::notify_guide_setp_list() {
	if(public_detail_->guide_step_map.empty()) {
		return;
	}

	MSG_80100501 msg;
	for(Guide_Step_Map::const_iterator it = public_detail_->guide_step_map.begin();
			it != public_detail_->guide_step_map.end();
			++it) {
		Guide_Info guide_info;
		guide_info.guide_id = it->first;
		guide_info.step = it->second.step;
		msg.guide_info_list.push_back(guide_info);
	}
	THIS_SEND_TO_CLIENT(msg);
}

int Public::req_task_guide(const uint32_t guide_id) {
	if(guide_id != public_detail_->task_guide_id) {
		public_detail_->task_guide_id = guide_id;
		public_detail_->detail_change();
	}

	return 0;
}

int Public::req_role_info(role_id_t role_id) {
	if (role_id) {
		DB_View::ID_Player_View_Map::iterator iter =
		LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			MSG_USER("player(role_id=%ld) is not in db_view", role_id);
			return ERROR_FRIEND_ROLE_NOT_EXIST;
		}
		MSG_50100006 msg;
		msg.role_info.role_id = role_id;/*人物id*/
		msg.role_info.role_name = iter->second->role_name;/*人物姓名*/
		msg.role_info.gender = iter->second->gender;/*性别（1男，2女）*/
		msg.role_info.guild_name = iter->second->gang_name;/*公会名称*/
		msg.role_info.career = iter->second->career;/*职业*/
		msg.role_info.level = iter->second->level;/*人物等级*/
		msg.role_info.vip_level = iter->second->vip();/*vip等级*/
		msg.role_info.combat = iter->second->force;/*战斗力*/
		msg.role_info.title_id = iter->second->title_id; /*称号*/
		msg.role_info.head_photo_id = iter->second->friend_.head_photo_id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Public::req_open_role_panel_logic(role_id_t role_id, int8_t open_type) {
	Logic_Player *target_player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (! target_player) {
		return ERROR_PLAYER_NOT_ONLINE;
	}

	{
		Block_Buffer buf;
		buf.make_message(INNER_REQ_OPEN_PANEL);
		buf.write_int64(player_self()->role_id());
		buf.write_int8(open_type);
		buf.finish_message();
		target_player->send_to_scene(buf);
	}

//	MSG_50100032 res_msg;
//	res_msg.role_id = role_id;
//	res_msg.open_type = open_type;
//	target_player->fighter_detail().property_list(res_msg.prop_value);
//	// fight hero
//	Heroer_Detail::Hero_Map::const_iterator it_hero = target_player->heroer_detail().hero_map.find(target_player->heroer_detail().fighter_index);
//	if (it_hero != target_player->heroer_detail().hero_map.end()) {
//		int hero_init_morale_type = it_hero->second.hero_fighter_detail.init_morale.type;
//		double hero_init_morale_val = it_hero->second.hero_fighter_detail.init_morale.total();
//		for (uint16_t i = 0; i < res_msg.prop_value.size(); ++i) {
//			if (res_msg.prop_value[i].type == hero_init_morale_type) {
//				res_msg.prop_value[i].value += hero_init_morale_val;
//			}
//		}
//	}
//	res_msg.prop_value.push_back(Property(PT_CAREER, target_player->career()));
//	res_msg.prop_value.push_back(Property(PT_CHARM, target_player->base_detail().charm)); /*魅力值*/
//	res_msg.role_name = target_player->role_name();/*人物姓名*/
//	res_msg.gender = target_player->base_detail().gender;/*性别（1男，2女）*/
//	res_msg.gang_id = target_player->base_detail().gang_id;/*职业*/
//	res_msg.gang_name = target_player->base_detail().gang_name;/*人物id*/
//	res_msg.headship = target_player->ganger_detail().headship;
//	res_msg.avatar_vec = target_player->base_detail().avatar_vec;
//	THIS_SEND_TO_CLIENT(res_msg);

	/*
	 * 各模块主动消息
	 */
//	player_self()->active_panal_hero_tags(target_player);
//	player_self()->active_panal_hero_info(target_player);
//	player_self()->active_rune_info(target_player);
//	player_self()->active_equip_master_info(target_player);
//	player_self()->pack_active_pack_rune_stone();

	return 0;
}

int Public::req_rename(const std::string& msg_new_name) {
	std::string new_name = msg_new_name;
	new_name.erase(0, new_name.find_first_not_of(" \t"));
	new_name.erase(new_name.find_last_not_of(" \t") + 1);
	if (new_name.empty() || new_name.size() > 100) {
		return ERROR_CLIENT_PARAM;
	}
	// 角色名是否存在
	DB_View::Name_Player_View_Map::iterator n_find_it =
	LOGIC_MONITOR->db_view()->name_player_view_map.find(new_name);
	if (n_find_it != LOGIC_MONITOR->db_view()->name_player_view_map.end()) {
		return ERROR_EXIST_ROLE_NAME;
	}
	// check cost item
	Id_Amount cost_item(RENAME_ITEM_ID, 1);
	if (cost_item.id && cost_item.amount) {
		int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,
				cost_item);
		if (try_res) {
			return try_res;
		}
	}

	//-------------------------- all check passed line ------------------------------------------------
	if (cost_item.id && cost_item.amount) {
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_item,
				Lose_Item_DM_Info(ITEM_LOSE_RENAME));
	}

	// base_detail and player view at logic server
	player_self()->set_role_name(new_name);

	// sync new name to other server
	Block_Buffer inner_buf;
	inner_buf.make_message(INNER_SYNC_PLAYER_NAME);
	inner_buf.write_string(new_name);
	inner_buf.finish_message();
	this->send_to_scene(inner_buf);
	this->send_to_local_chat(inner_buf);

	// notify to scene to broad around
	String_Info_Change string_info_change;
	string_info_change.type = SICT_ROLE_NAME;
	string_info_change.value = new_name;
	std::vector<String_Info_Change> change_info;
	change_info.push_back(string_info_change);
	player_self()->active_string_info_change(change_info);

	// refresh role UI

	// refresh gang UI
	if (player_self()->ganger_detail().gang_id) {
		GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, role_id(),
				player_self()->ganger_detail().gang_id);
	}

	// refresh friend UI

	GangWarGangRankInfo_Map::iterator it = GANG_MANAGER->gang_war_info().gang_rank_map.find(player_self()->ganger_detail().gang_id);
	if (it != GANG_MANAGER->gang_war_info().gang_rank_map.end()) {
		for (Ganer_Rank_Info::iterator it_gri = it->second.ganger_rank_info.begin(); it_gri != it->second.ganger_rank_info.end(); ++it_gri) {
			if(role_id() == it_gri->second.role_id) {
				it_gri->second.role_name = new_name;
			}
		}
	}



	MSG_50100008 res_msg;
	res_msg.result = 1;
	res_msg.new_name = new_name;
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

int Public::req_buy_phy_power(int bind_gold_first) {
	Logic_Player* player = this->player_self();
	if (!player) {
		return -1;
	}
	int vip_lv = player->vip();
	int max_buy_times = CONFIG_CACHE_FIGHTER->get_phy_power_max_buy_times(
			vip_lv);
	int cur_buy_times = player->fighter_detail().phy_power_buy_times;
	int phy_power_cur = player->fighter_detail().current_strength();
	int phy_power_recover_time = player->fighter_detail().phy_power_recover_time;
	if (cur_buy_times >= max_buy_times) {
		return ERROR_PHY_POWER_BUY_TIMES_FULL;
	}
	if (phy_power_cur >= player->fighter_detail().max_strength()) {
		return ERROR_PHY_POWER_BUY_VALUE_FULL;
	}
	int cost_value = CONFIG_CACHE_FIGHTER->get_phy_power_buy_cost(
			cur_buy_times + 1);
	//money
	int result;
	result = player->pack_sub_money(
			Money_Sub_Info(
					Money_Sub_Type(
							bind_gold_first == 1 ? BIND_GOLD_FIRST : GOLD_ONLY),
					cost_value, MONEY_SUB_FB_BUY_PHY));
	if (result != 0) {
		MSG_50120000 msg;
		msg.result = 0;
		OBJ_SEND_TO_CLIENT(msg, (*player));
		return result;
	}
	int recover_value = 20;
	cur_buy_times += 1;
	phy_power_cur += recover_value;

	if (phy_power_cur >= player->fighter_detail().max_strength()) {
		phy_power_cur = player->fighter_detail().max_strength();
		phy_power_recover_time = 0;
	}
	player->sync_phy_power_info(phy_power_cur, phy_power_recover_time,
			cur_buy_times);
	//sync scene
	{
		MSG_20100030 msg;
		msg.phy_power_buy_times = player->fighter_detail().phy_power_buy_times;
		msg.phy_power_cur = player->fighter_detail().current_strength();
		msg.phy_power_recover_time =
				player->fighter_detail().phy_power_recover_time;
		THIS_SEND_TO_SCENE(msg);
	}
	// to client
	{
		MSG_50120000 msg;
		msg.result = 1;
		OBJ_SEND_TO_CLIENT(msg, (*player));
	}
	return 0;
}

int Public::fetch_phy_power_info(void) {
	Logic_Player* player = this->player_self();
	if (!player) {
		return -1;
	}
	MSG_50120001 msg;
	msg.phy_power_buy_times = player->fighter_detail().phy_power_buy_times;
	msg.phy_power_cur = player->fighter_detail().current_strength();
	msg.phy_power_max = player->fighter_detail().max_strength();
	msg.phy_power_recover_time =
			player->fighter_detail().phy_power_recover_time;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::restor_phy_power(const int value){
	Logic_Player* player = this->player_self();
	if (!player) {
		return -1;
	}
	int phy_power_cur = player->fighter_detail().current_strength();
	int cur_buy_times = player->fighter_detail().phy_power_buy_times;
	int phy_power_recover_time = player->fighter_detail().phy_power_recover_time;
	if (phy_power_cur >= player->fighter_detail().max_strength()) {
		return ERROR_PHY_POWER_BUY_VALUE_FULL;
	}
	phy_power_cur += value;

	if (phy_power_cur >= player->fighter_detail().max_strength()) {
		phy_power_cur = player->fighter_detail().max_strength();
		phy_power_recover_time = 0;
	}
	player->sync_phy_power_info(phy_power_cur, phy_power_recover_time,
			cur_buy_times);
	//sync scene
	{
		MSG_20100030 msg;
		msg.phy_power_buy_times = player->fighter_detail().phy_power_buy_times;
		msg.phy_power_cur = player->fighter_detail().current_strength();
		msg.phy_power_recover_time =
				player->fighter_detail().phy_power_recover_time;
		THIS_SEND_TO_SCENE(msg);
	}
	return 0;
}

int Public::req_chat_bugle(const std::vector<Chat_Content> &content) {
	Logic_Player* player = this->player_self();
	if (!player) {
		return -1;
	}
	int res = player->pack_erase_item(Pack_Type(PACK_T_PACKAGE_INDEX),
			Id_Amount(CHAT_BUGLE, 1, BIND_NONE),
			Lose_Item_DM_Info(ITEM_LOST_CHAT));
	if (res)
		return res;
	MSG_20100099 msg;
	msg.content = content;
	THIS_SEND_TO_LOCAL_CHAT(msg);
	return 0;
}

int Public::req_war_scene_enter(bool check_open/* = true*/) {
	// 还没有登录到跨服战场,同步信息过去,请求进入
	if (!public_detail_) {
		return -1;
	}
	if (player_self() == NULL) {
		return -1;
	}
	const Scene_Config *scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
			player_self()->location().scene_id);
	if (scene_cfg && scene_cfg->type != Main_Scene_Type &&
			scene_cfg->type != Arena_Panel_Scene_Type) {
		return ERROR_COPY_IS_GOING;
	}
	Time_Value now = Time_Value::gettimeofday();
	if (now < public_detail_->war_forbid_time) {
		return ERROR_WAR_SCENE_FORBID_TIME;
	}
	int level_limit = CONFIG_CACHE_DUNGEON->get_war_level_limit();
	if (level() < level_limit) {
		return ERROR_WAR_SCENE_LEVEL_LIMIT;
	}

	int act_time = 0;
	int remain_time = 0;
	int act_status = 0;
	WAR_SCENE_ACTIVE_MANAGER->get_active_info(act_status, act_time, remain_time);
	if (check_open) {
		if (act_status != 3) {
			return ERROR_WAR_ACTIVITY_NOT_OPEN;
		}
	}

	int max_income_num = 0;
	int remain_income_num = 0;
	int income_rate = 0;
	this->get_war_income_info(max_income_num, remain_income_num, income_rate);
	std::string server_name = CONFIG_CACHE->server_maintainer_cache().game_server_flag;
	int level = this->level();
	int force = this->player_self()->fighter_detail().force;
	role_id_t gang_id = this->player_self()->gang_id();
	Block_Buffer buf;
	buf.make_message(INNER_WAR_SCENE_ENTER);
	this->player_self()->monitor_link().logic_muid.serialize(buf);
	buf.write_int64(player_self()->role_id());
	buf.write_int32(level);
	buf.write_int32(force);
	buf.write_int64(gang_id);
	buf.write_int8(remain_income_num);
	buf.write_int16(income_rate);
	buf.write_int32(public_detail_->war_merit);
	buf.write_int32(remain_time);
	buf.write_int32(act_time);
	buf.write_string(server_name);
	buf.finish_message();

	LOGIC_MONITOR->send_to_scene(WAR_SCENE_ID, 0, buf);

	return 0;
}

int Public::req_war_scene_reset_enter_num(void) {
	if (public_detail_) {
		public_detail_->war_enter_num = 0;
		public_detail_->war_forbid_time = Time_Value::zero;
		public_detail_->detail_change();
		send_war_scene_active_to_client();
	}
	return 0;
}

int Public::war_scene_enter(role_id_t war_scene_id, int side, Coord& coord) {
	int scene_id = WAR_SCENE_ID;
	std::string str_side = "light";
	if (side == 1) {
		str_side = "dark";
	}
	this->player_self()->inner_transmit(scene_id, 0, coord, war_scene_id,
			str_side);
	if (public_detail_) {
		public_detail_->war_enter_num += 1;
		public_detail_->detail_change();
		send_war_scene_active_to_client();
	}
	// TOD助手日常和活跃度侦听
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_WARER);
	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_WAR_NUM);
	// 任务侦听战场进入
	player_self()->task_listen_war_all();

	MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`) VALUES (%d,%ld,%ld,%d)",  1,  player_self()->role_id(), Time_Value::gettimeofday().sec(),
				player_self()->level());
	return 0;
}

int Public::req_war_scene_enter_num(void) {
	if (player_self() == NULL) {
		return -1;
	}
	int max_income_num = 0;
	int remain_income_num = 0;
	int income_rate = 0;
	this->get_war_income_info(max_income_num, remain_income_num, income_rate);

	int merit = public_detail().war_merit;
	int max_merit = 0;

	MSG_50111001 msg;
	msg.total_merit = merit;
	msg.max_merit = max_merit;
	msg.max_num = max_income_num;
	msg.remain_num = remain_income_num;
	msg.income_rate = income_rate;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::req_war_scene_ex_result(void){
	if(!public_detail_){
		return 0;
	}
	if(public_detail_->war_result != 0 && public_detail_->war_coe > 0){
		MSG_50111004 msg;
		msg.result = public_detail_->war_result;
		msg.rank = public_detail_->war_rank;
		msg.kill_num = public_detail_->war_kill_num;
		msg.coe = public_detail_->war_coe;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Public::req_war_scene_receive(void){
	if(this->public_detail_->war_result == 0){
		MSG_50111004 msg;
		msg.result = 0;
		THIS_SEND_TO_CLIENT(msg);
		return ERROR_VIP_TREE_NOT_RECEIVE;
	}
	Item_Vec item_vec;

	CONFIG_CACHE_DUNGEON->get_war_result_ex_reward(public_detail_->war_result,
			public_detail_->war_kill_num, public_detail_->war_coe, item_vec);
	if(item_vec.empty() == false && public_detail_->war_coe > 0){
		int pack_result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,
									item_vec, Gain_Item_DM_Info(Pack::ITEM_GAIN_WAR_SCENE));
		if(pack_result == 0){
			MSG_81000102 res_msg;
			res_msg.type = 0;
			Item_Basic_Info item_base;
			Card_Info card_info;
			for (Item_Vec::iterator item_it = item_vec.begin(); item_it != item_vec.end(); ++item_it) {
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
			if(!res_msg.card_info_vec.empty() || !res_msg.item_info.empty() ||
					!res_msg.property.empty()){
				THIS_SEND_TO_CLIENT(res_msg);
			}
		}
	}
	MSG_50111003 msg;
	msg.result = 1;
	THIS_SEND_TO_CLIENT(msg);
	public_detail_->detail_change();

	public_detail_->war_result = 0;
	return 0;
}

void Public::inner_war_scene_result(const int result, const int rank, const int kill_num, const int coe){
	if(!player_self() || !public_detail_){
		return;
	}
	if(public_detail_->war_result != 0 && public_detail_->war_coe > 0){// send email
		std::string mail_sender = "system";
		std::string mail_title = "battle scene reward";
		std::string mail_content = "battle scene reward";
		const Mail_Base_Config *cfg_mail = CONFIG_CACHE_MAIL->find_config(1052);
		if(cfg_mail){
			mail_sender = cfg_mail->sender_name;
			mail_title = cfg_mail->mail_title;
			mail_content = cfg_mail->mail_content;
		}
		Mail_Send_Info send_info;
		send_info.sender_id = 0;
		send_info.sender_name = mail_sender;
		send_info.send_type = MAIL_TYPE_SYS;
		send_info.show_send_time = Time_Value::gettimeofday();
		send_info.receiver_id = role_id();
		send_info.title = mail_title;
		send_info.content = mail_content;
		CONFIG_CACHE_DUNGEON->get_war_result_ex_reward(public_detail_->war_result,
				public_detail_->war_kill_num, public_detail_->war_coe, send_info.item_vector);
		player_self()->send_mail_to_self(send_info);
	}
	public_detail_->war_result = result;
	public_detail_->war_rank = rank;
	public_detail_->war_kill_num = kill_num;
	public_detail_->war_coe = coe;
	public_detail_->detail_change();
	if(public_detail_->war_result != 0 && public_detail_->war_coe > 0){
		MSG_50111004 msg;
		msg.result = result;
		msg.rank = rank;
		msg.kill_num = kill_num;
		msg.coe = coe;
		THIS_SEND_TO_CLIENT(msg);
	}
}

int Public::req_fetch_month_hero_info(void){
	int cfg_id = 0;
	int remain_day = 0;
	SECRET_SHOP_MANAGER->get_month_hero_info(cfg_id, remain_day);
	MSG_50120070 msg;
	msg.cfg_index = cfg_id;
	msg.remain_day = remain_day;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Public::add_gang_merit_contribute(int add_merit, int add_contribute) {
	if (player_self() == NULL) {
		return;
	}
	if (public_detail_ == NULL) {
		return;
	}
	if (add_merit > 0) {
		int src_value = public_detail_->war_merit;
		public_detail_->war_merit += add_merit;
		if (public_detail_->war_merit > 50000) {
			public_detail_->war_merit = 50000;
			int add_value = public_detail_->war_merit - src_value;
			change_exploit_val(add_value);
		} else {
			change_exploit_val(add_merit);
		}
	}
	if (add_contribute > 0) {
		player_self()->add_contribute(add_contribute);
	}
	return;
}

void Public::add_war_merit_piece(int add_merit, int add_piece, int arena_coin) {
	if (player_self() == NULL) {
		return;
	}
	if (public_detail_ == NULL) {
		return;
	}
	if (add_merit > 0) {
		int src_value = public_detail_->war_merit;
		public_detail_->war_merit += add_merit;
		if (public_detail_->war_merit > 50000) {
			public_detail_->war_merit = 50000;
			int add_value = public_detail_->war_merit - src_value;
			change_exploit_val(add_value);
		} else {
			change_exploit_val(add_merit);
		}
	}
	if (add_piece > 0 || arena_coin > 0 ) {
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		Item_Vec items;
		if(add_piece > 0){
			Item_Detail item_detail(PIECES_ITEM_ID, add_piece, Item_Detail::BIND);
			items.push_back(item_detail);
			Item_Basic_Info ibi;
			item_detail.set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
		}
		if(arena_coin > 0){
			Item_Detail item_detail(21002401, arena_coin, Item_Detail::BIND);
			items.push_back(item_detail);
			Item_Basic_Info ibi;
			item_detail.set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
		}

		int p_result = player_self()->pack_insert_item(
				Pack::PACK_T_PACKAGE_INDEX, items,
				Gain_Item_DM_Info(Pack::ITEM_GAIN_WAR_SCENE));
		if (p_result != 0 && p_result != ERROR_PACK_FULL_BUT_MAIL) {
			Mail_Send_Info send_info;
			send_info.sender_id = 0;
			send_info.receiver_id = this->role_id();
			send_info.send_type = MAIL_TYPE_SYS;
			send_info.show_send_time = Time_Value::gettimeofday();
			if(add_piece > 0){
				Item_Detail item_detail(PIECES_ITEM_ID, add_piece, Item_Detail::BIND);
				send_info.item_vector.push_back(item_detail);
			}
			if(arena_coin > 0){
				Item_Detail item_detail(21002401, arena_coin, Item_Detail::BIND);
				send_info.item_vector.push_back(item_detail);
			}
			send_info.sender_name = "war_scene";
			send_info.title = "war_scene gain";
			send_info.content = "";
			this->player_self()->send_mail_to_self(send_info);
		} else {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
	}
	public_detail_->detail_change();
	return;
}

void Public::sync_war_forbid_time(const Time_Value time) {
	if (public_detail_ == NULL) {
		return;
	}
	public_detail_->war_forbid_time = time;
	public_detail_->detail_change();

}

void Public::send_war_scene_active_to_client(void) {
	if (player_self() == NULL) {
		return;
	}
	int lv_limit = WAR_SCENE_ACTIVE_MANAGER->get_enter_level_limit();
	if (player_self()->level() < lv_limit) {
		return;
	}
	int act_status = 0;
	int remain_time = 0;
	int act_time = 0;
	WAR_SCENE_ACTIVE_MANAGER->get_active_info(act_status, act_time, remain_time);
	int max_income_num = 0;
	int remain_income_num = 0;
	int income_rate = 0;
	this->get_war_income_info(max_income_num, remain_income_num, income_rate);
	MSG_81000408 msg;
	msg.icon_id = 18;
	msg.ex_data = remain_income_num;
	if (act_status == 1) {
		msg.open = 0;
		msg.active = 0;
		msg.time = 0;
	} else if (act_status == 2) {
		msg.open = 1;
		msg.active = 0;
		msg.time = remain_time;
	} else {
		msg.open = 1;
		msg.active = 1;
		msg.time = remain_time;
	}
	THIS_SEND_TO_CLIENT(msg);
}

void Public::send_bonfire_active_to_client(void) {
	if (player_self() == NULL) {
		return;
	}
	int lv_limit = CAMPFIRE_MANAGER->get_enter_level_limit();
	if (player_self()->level() < lv_limit) {
		return;
	}
	int status = 0;
	int remain_time = 0;
	int act_id = 0;
	CAMPFIRE_MANAGER->get_active_info(status, remain_time, act_id);
	MSG_81000408 msg;
	msg.icon_id = 3;
	if (status == 1) {
		msg.open = 0;
		msg.active = 0;
		msg.time = 0;
	} else if (status == 2) {
		msg.open = 1;
		msg.active = 0;
		msg.time = remain_time;
	} else {
		msg.open = 1;
		msg.active = 1;
		msg.time = remain_time;
	}
	THIS_SEND_TO_CLIENT(msg);
}

void Public::send_act_scene_to_client(const int act_icon_id){
	return;
	if (player_self() == NULL) {
		return;
	}
	int status = 0;
	int remain_time = 0;
	int act_id = 0;
	int lv_limit = 0;
	switch(act_icon_id){
		case 16:{
			lv_limit = WORLD_BOSS_ACTIVE_MANAGER->get_enter_level_limit();
			WORLD_BOSS_ACTIVE_MANAGER->get_active_info(status, remain_time, act_id);
			break;
		}
		default:{
			return;
			break;
		}
	}
	if (player_self()->level() < lv_limit) {
		return;
	}
	MSG_81000408 msg;
	msg.icon_id = act_icon_id;
	if (status == 1) {
		msg.open = 0;
		msg.active = 0;
		msg.time = 0;
		THIS_SEND_TO_CLIENT(msg);
	} else if (status == 2) {
		msg.open = 1;
		msg.active = 0;
		msg.time = remain_time;
		THIS_SEND_TO_CLIENT(msg);
	} else if(status == 3){
		msg.open = 1;
		msg.active = 1;
		msg.time = remain_time;
		THIS_SEND_TO_CLIENT(msg);
	}
}

void Public::send_gang_boss_active_to_client(const int state, const int remain_time){
	MSG_81000408 msg;
	msg.icon_id = 19;
	if (state == 1) {
		msg.open = 0;
		msg.active = 0;
		msg.time = 0;
		THIS_SEND_TO_CLIENT(msg);
	} else if (state == 2) {
		msg.open = 1;
		msg.active = 0;
		msg.time = remain_time;
		THIS_SEND_TO_CLIENT(msg);
	} else if(state == 3){
		msg.open = 1;
		msg.active = 1;
		msg.time = remain_time;
		THIS_SEND_TO_CLIENT(msg);
	}
}

void Public::listen_fighter_level(const int new_level, const int src_level) {
	// 篝火晚会活动图标刷新
	int bonfire_lv_limit = CAMPFIRE_MANAGER->get_enter_level_limit();
	if (src_level < bonfire_lv_limit && new_level >= bonfire_lv_limit) {
		this->send_bonfire_active_to_client();
	}
	int world_boss_lv_limit = WORLD_BOSS_ACTIVE_MANAGER->get_enter_level_limit();
	if(src_level < world_boss_lv_limit && new_level >= world_boss_lv_limit){
		send_act_scene_to_client(world_boss_act_icon_id);
	}
	// 战场活动图标刷新
	int war_lv_limit = WAR_SCENE_ACTIVE_MANAGER->get_enter_level_limit();
	if (src_level < war_lv_limit && new_level >= war_lv_limit) {
		this->send_war_scene_active_to_client();
	}
	hook_equip_look_level_up(new_level);


	bool is_need_save = false;
	const Notice_Cfg_Map &notice_cfg_map =  CONFIG_CACHE_PUBLIC->get_notice_cfg_map();
	for(Notice_Cfg_Map::const_iterator it = notice_cfg_map.begin(); it != notice_cfg_map.end(); ++it) {
		if(it->second.type == NOTICE_TYPE_LEVEL
				&& it->second.id > public_detail_->notice_id
				&& new_level >= (int)it->second.role_level) {
			is_need_save = true;
			public_detail_->notice_id = it->second.id;
			public_detail_->is_played_effect = false;
			public_detail_->is_show = true;
			MSG_81110013 msg;
			msg.id = public_detail_->notice_id;
			msg.type = 1;
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	if(is_need_save) {
		public_detail_->detail_change();
	}
}

int Public::req_arena_panel_info(void){
	// 单人竞技场挑战次数、赛季剩余天数
	int single_arena_total_num = 0;
	int single_arena_enter_num = 0;
	this->player_self()->get_arena_enter_num_info(single_arena_total_num, single_arena_enter_num);
	int single_arena_num = single_arena_total_num - single_arena_enter_num;
	if(single_arena_num < 0){
		single_arena_num = 0;
	}
	int single_remain_day = this->player_self()->get_arena_season_remain_day();
	// 战场收益次数
	int war_max_income_num = 0;
	int war_remain_income_num = 0;
	int war_income_rate = 0;
	this->get_war_income_info(war_max_income_num, war_remain_income_num, war_income_rate);

	MSG_50100408 msg;
	msg.single_remain_num = single_arena_num;
	msg.single_remain_day = single_remain_day;
	msg.war_remain_num = war_remain_income_num;
	msg.team_remain_num = player_self()->profile_num_sports_all();
	const Arena_Adore_Public &arena_data = CONFIG_CACHE_PUBLIC->get_arena_adore_data();

	if ( public_detail_->arena_worship_times <= arena_data.adore_times ) {
		msg.arena_worship = arena_data.adore_times - public_detail_->arena_worship_times;
	}
	msg.be_worship = player_self()->be_worship();
	msg.rank = player_self()->local_rank();
	Arena_Rank_Player* arena_rank_player = ARENA_MANAGER->get_local_first_player();
	if ( arena_rank_player ) {
		msg.first_player.fight = arena_rank_player->fight();
		msg.first_player.group = arena_rank_player->group();
		msg.first_player.level = arena_rank_player->level();
		msg.first_player.name = arena_rank_player->role_name();
		msg.first_player.rank = arena_rank_player->local_rank();
		msg.first_player.role_id = arena_rank_player->role_id();
		msg.first_player.score = arena_rank_player->score();
		msg.first_player.seg = arena_rank_player->seg();
		msg.first_be_worship = arena_rank_player->be_worship();
		Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(arena_rank_player->role_id());
		if (player ) {
			msg.first_avatar = player->base_detail().avatar_vec;
			msg.first_career = player->base_detail().career;
			msg.first_gender = player->base_detail().gender;
		}else {
			Player_DB_View *db_view = LOGIC_MONITOR->find_player_db_view_by_role_id(arena_rank_player->role_id());
			if ( db_view ) {
				msg.first_avatar = db_view->avatar_vec;
				msg.first_career = db_view->career;
				msg.first_gender = db_view->gender;
			}
		}
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::req_get_sys_cfg() {
	MSG_50120003 msg;
	msg.is_total_open = public_detail_->is_total_open;
	msg.is_open_music = public_detail_->is_open_music;
	msg.music_num = public_detail_->music_num;
	msg.is_open_voice = public_detail_->is_open_voice;
	msg.voice_num = public_detail_->voice_num;
	msg.is_hide_role_model = public_detail_->is_hide_role_model;
	msg.is_hide_hero_model = public_detail_->is_hide_hero_model;
	msg.is_hide_title = public_detail_->is_hide_title;
	msg.player_count = public_detail_->player_count;
	msg.is_skip_plot = public_detail_->is_skip_plot;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::req_set_sys_cfg(const MSG_10120004 &msg) {
	const Sys_Cfg &cfg = CONFIG_CACHE_PUBLIC->sys_config();
	if (msg.player_count > cfg.max_player_count
			|| msg.player_count < cfg.min_player_count) {
		return ERROR_SYS_CFG_INVALID;
	}

	public_detail_->is_total_open = msg.is_total_open;
	public_detail_->is_open_music = msg.is_open_music;
	public_detail_->music_num = msg.music_num;
	public_detail_->is_open_voice = msg.is_open_voice;
	public_detail_->voice_num = msg.voice_num;
	public_detail_->is_hide_role_model = msg.is_hide_role_model;
	public_detail_->is_hide_hero_model = msg.is_hide_hero_model;
	public_detail_->is_hide_title = msg.is_hide_title;
	public_detail_->player_count = msg.player_count;
	public_detail_->is_skip_plot = msg.is_skip_plot;

	public_detail_->detail_change();
	return 0;
}

int Public::trigger_daily_zero(const Time_Value &now,
		bool logic_check/* = false*/) {
	if (public_detail_->war_enter_num != 0) {
		public_detail_->war_enter_num = 0;
		public_detail_->detail_change();
		send_war_scene_active_to_client();
	}
	if (public_detail_->war_merit != 0 || public_detail_->add_campfire != 0) {
		public_detail_->war_merit = 0;
		public_detail_->add_campfire = 0;
		public_detail_->detail_change();
		send_war_scene_active_to_client();
	}
	{//今日累计获得粉钻数量
		uint32_t old = public_detail_->today_rec_bind_gold;
		public_detail_->today_rec_bind_gold = 0;
		if(old != public_detail_->today_rec_bind_gold) {
			public_detail_->detail_change();
		}
	}
	return 0;
}

int Public::trigger_daily_six(const Time_Value& now,
		bool logic_check/* = false*/) {

	set_vip_upgrade_card(0);
	player_self()->handle_get_vip_upgrade_times();
	set_arena_worship(0);

	return 0;
}

int Public::trigger_weekly_zero(const Time_Value &now,
		bool logic_check/* = false*/) {
	return 0;
}

void Public::equip_tick(void) {
	player_self()->equiper_tick();
}

int Public::active_string_info_change(
		std::vector<String_Info_Change> &change_info) {
	Block_Buffer buf;
	buf.make_message(INNER_STRING_INFO_CHANGE);
	uint16_t change_info_size = change_info.size();
	buf.write_uint16(change_info_size);
	for (uint16_t i = 0; i < change_info_size; ++i) {
		change_info[i].serialize(buf);
	}
	buf.write_int64(role_id());
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

int Public::active_int_info_change(const MSG_81000067& active_msg) {
	Block_Buffer w;
	w.make_message(INNER_INT_INFO_CHANGE);
	w.write_int64(active_msg.role_id);
	w.write_int8(active_msg.int_type);
	uint16_t __vec_32_change_info_vec_size =
			active_msg.vec_32_change_info.size();
	w.write_uint16(__vec_32_change_info_vec_size);
	for (uint16_t i = 0; i < __vec_32_change_info_vec_size; ++i) {
		active_msg.vec_32_change_info[i].serialize(w);
	}
	w.finish_message();
	this->send_to_scene(w);
	return 0;
}

int Public::req_fetch_awake_lvl(void) {
	MSG_50120020 res_msg;
	res_msg.awake_lvl = player_self()->awake_lvl();
	//res_msg.awake_lvl = public_detail_->awake_lvl;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Public::req_awake_lvl_upgrade(bool from_hero_msg) {
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(
			player_hero_id(player_self()->career()));
	if (!hero_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	int awake_lvl = player_self()->awake_lvl();
	//if (public_detail_->awake_lvl >= hero_config->max_awake_lvl) {
	if (awake_lvl >= hero_config->max_awake_lvl) {
		//return ERROR_HERO_AWAKE_LVL_MAX;	// 客户端不要求提示
		return -1;
	}

//	// condition: task
//	Int_IntVec_Map::const_iterator it_awake_tasks = hero_config->awake_tasks_map.find(public_detail_->awake_lvl + 1);
//	if (! player_self()->is_fini_tasks(player_hero_id(player_self()->career()), it_awake_tasks->second)) {
//		return ERROR_HERO_AWAKE_NOT_FINI_TASK;
//	}

	// check material
	size_t size_cfg_cost_items = hero_config->awake_lvl_cost_items.size();
	Id_Amount_Vec awake_cost_items;
	//if ((uint)public_detail_->awake_lvl < size_cfg_cost_items) {
	if ((uint) awake_lvl < size_cfg_cost_items) {
		awake_cost_items = hero_config->awake_lvl_cost_items[awake_lvl];
		if (!awake_cost_items.empty()) {
			int try_res = player_self()->pack_try_erase_item(
					PACK_T_PACKAGE_INDEX, awake_cost_items);
			if (try_res) {
				return try_res;
			}
		}
	}

	//-------------------------------- all check passed ---------------------------------------------

	// Sub material
	if (!awake_cost_items.empty()) {
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, awake_cost_items,
				Lose_Item_DM_Info(ITEM_LOSE_AWAKE));
	}

	//++public_detail_->awake_lvl;
	player_self()->add_awake_lvl();

	// 加技能点
	player_self()->add_skill_pointer(1);

	// 觉醒技能
	if (!hero_config->awake_skill_id_vec.empty()) {
//		Skill_Info_Vec skill_info_vec;
//		Skill_Info skill_info;
//		for (uint8_t i = 0; i < hero_config->awake_skill_id_vec.size(); ++i) {
//			skill_info.skill_id = hero_config->awake_skill_id_vec[i];
//			skill_info.skill_lv = 1;
//			skill_info_vec.push_back(skill_info);
//		}
//		player_self()->add_be_skills(skill_info_vec);

		for (uint8_t i = 0; i < hero_config->awake_skill_id_vec.size(); ++i) {
			int skill_id = hero_config->awake_skill_id_vec[i];
			const Skill_Config *skill_config =
			CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
			if (!skill_config) {
				continue;
			}
			int talent_id = skill_config->skill_series;
			int skill_lv = 1;

			player_self()->manual_learn_skill(talent_id, skill_id, skill_lv);
		}
	}

	//public_detail_->detail_change();

	if (from_hero_msg) {
		MSG_50102007 res_msg;
		res_msg.result = 1;
		res_msg.is_hero = 0;
		res_msg.hero_index = 0;
		THIS_SEND_TO_CLIENT(res_msg);
	} else {
		MSG_50120021 res_msg;
		res_msg.result = 1;
		THIS_SEND_TO_CLIENT(res_msg);
	}

	return 0;
}
//
//int Public::awake_lvl(void) const {
//	return public_detail_->awake_lvl;
//}

int Public::player_hero_id(const int career) {
	switch (career) {
	case CAREER_SOLDIER:
		return PHI_SOLDIER;
	case CAREER_ASSASSIN:
		return PHI_ASSASSIN;
	case CAREER_MAGE:
		return PHI_MAGE;
	default:
		return 0;
	}
}

int Public::req_fetch_player_artifact_info(void) {
	MSG_50120030 res_msg;
	Artifact_Info artifact_info;
	for (Artifact_Map::const_iterator it_artifact =
			public_detail_->artifacts.begin();
			it_artifact != public_detail_->artifacts.end(); ++it_artifact) {
		artifact_info.reset();
		artifact_info.artifact_id = it_artifact->second.artifact_id;
		artifact_info.artifact_lvl = it_artifact->second.artifact_lvl;
		res_msg.artifact_vec.push_back(artifact_info);
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Public::req_player_artifact_upgrade(const int artifact_id,
		bool from_hero_msg) {
	const Hero_Config *hero_config = CONFIG_CACHE_HERO->find_hero(
			player_hero_id(player_self()->career()));
	if (!hero_config) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (!hero_config->artifact_ids.count(artifact_id)) {
		return ERROR_CLIENT_OPERATE;
	}

	const Artifact_Cfg_Id *artifact_cfg_id = CONFIG_CACHE_HERO->find_artifact(
			artifact_id);
	if (!artifact_cfg_id) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	Artifact_Map::iterator it_artifact = public_detail_->artifacts.find(
			artifact_id);
	if (it_artifact == public_detail_->artifacts.end()) {
		return ERROR_INNER_SERVER;
	}
	if (it_artifact->second.artifact_lvl >= artifact_cfg_id->max_lvl) {
		return ERROR_HERO_ARTIFACT_LVL_MAX;
	}

	const Artifact_Cfg_Lvl *artifact_cfg_lvl_next =
	CONFIG_CACHE_HERO->find_artifact_lvl(it_artifact->second.artifact_lvl + 1);
	if (!artifact_cfg_lvl_next) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	// check soul
	int cost_soul = ceil(
			artifact_cfg_id->cost_soul_param
					* artifact_cfg_lvl_next->cost_soul);
	Money_Sub_Info money_info(
			Money_Sub_Info(Money_Sub_Type(SOULS_ONLY), cost_soul,
					MONEY_SUB_ARTIFACT_UPGRADE));
	if (money_info.nums > 0) {
		int try_res = player_self()->pack_try_sub_money(money_info);
		if (try_res) {
			return try_res;
		}
	}

	// check material
	Id_Amount_Vec cost_items = artifact_cfg_lvl_next->cost_items;
	for (uint8_t i = 0; i < cost_items.size(); ++i) {
		cost_items[i].amount = ceil(
				cost_items[i].amount * artifact_cfg_id->cost_item_param);
	}
	if (!artifact_cfg_lvl_next->cost_items.empty()) {
		int try_res = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,
				cost_items);
		if (try_res) {
			return try_res;
		}
	}

	//---------------- all check passed ------------------
	// Sub money
	if (money_info.nums > 0) {
		player_self()->pack_sub_money(money_info);
	}
	// Sub material
	if (!artifact_cfg_lvl_next->cost_items.empty()) {
		player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, cost_items,
				Lose_Item_DM_Info(ITEM_LOSE_ARTIFACT_UPGRADE));
	}

	int orig_artifact_lvl = it_artifact->second.artifact_lvl;

	++it_artifact->second.artifact_lvl;

	player_self()->refresh_artifacts_pro_val();

	player_self()->heroer_prop_refresh();

	public_detail_->detail_change();

	int orig_artifact_step =
			orig_artifact_lvl ? (orig_artifact_lvl - 1) / 10 + 1 : 0;
	int artifact_step = (it_artifact->second.artifact_lvl - 1) / 10 + 1;
	bool is_step_up = artifact_step > orig_artifact_step ? 1 : 0;

	// announce
	if (is_step_up) {
		int show_step = 0, show_level = 0;
		if (it_artifact->second.artifact_lvl % 10 == 0) {
			show_step = it_artifact->second.artifact_lvl / 10;
			show_level = 10;
		} else {
			show_step = it_artifact->second.artifact_lvl / 10 + 1;
			show_level = it_artifact->second.artifact_lvl % 10;
		}
		player_self()->create_ann_artifact_upgrade(hero_config->hero_name,
				artifact_cfg_id->artifact_name, artifact_step, show_level);
	}

	if (from_hero_msg) {
		MSG_50102020 res_msg;
		res_msg.result = 1;
		res_msg.is_hero = 0;
		res_msg.artifact_id = artifact_id;
		res_msg.hero_index = 0;
		res_msg.is_step_up = is_step_up;
		res_msg.artifact_lvl = it_artifact->second.artifact_lvl;
		THIS_SEND_TO_CLIENT(res_msg);
	}

	return 0;
}

int Public::get_player_artifacts_pro_val(
		std::map<int, double>& artifacts_pro_val) {
	artifacts_pro_val.clear();
	for (Artifact_Map::iterator it_artifact = public_detail_->artifacts.begin();
			it_artifact != public_detail_->artifacts.end(); ++it_artifact) {
		if (it_artifact->second.artifact_lvl) {
			const Artifact_Cfg_Lvl* artifact_cfg_lvl =
			CONFIG_CACHE_HERO->find_artifact_lvl(
					it_artifact->second.artifact_lvl);
			if (!artifact_cfg_lvl->pro_val_map.empty()) {
				// 资质
				const Artifact_Cfg_Id* artifact_cfg_id =
				CONFIG_CACHE_HERO->find_artifact(it_artifact->first);
				if (!artifact_cfg_id || artifact_cfg_id->aptitude_map.empty()) {
					continue;
				}
				for (std::map<int, double>::const_iterator it_aptitude_map =
						artifact_cfg_id->aptitude_map.begin();
						it_aptitude_map != artifact_cfg_id->aptitude_map.end();
						++it_aptitude_map) {
					std::map<int, double>::const_iterator it_pro_val_map =
							artifact_cfg_lvl->pro_val_map.find(
									it_aptitude_map->first);
					if (it_pro_val_map != artifact_cfg_lvl->pro_val_map.end()) {
						int pro = it_pro_val_map->first;
						double val = it_pro_val_map->second;
						double aptitude = it_aptitude_map->second;
						artifacts_pro_val[pro] += val * aptitude;
					}
				}
			}
		}
	}
	return 0;
}

void Public::init_artifacts(void) {
	const Hero_Config* hero_config = CONFIG_CACHE_HERO->find_hero(
			player_hero_id(player_self()->career()));
	if (!hero_config) {
		return;
	}
	//if (public_detail_->artifacts.size() < hero_config->artifact_ids.size()) {
	Artifact_Detail artifact_detail;
	for (Int_Set::const_iterator it_artifact_ids =
			hero_config->artifact_ids.begin();
			it_artifact_ids != hero_config->artifact_ids.end();
			++it_artifact_ids) {
		if ((*it_artifact_ids) > PHI_END) {
			break;
		}
		if (!public_detail_->artifacts.count(*it_artifact_ids)) {
			artifact_detail.reset();
			artifact_detail.artifact_id = *it_artifact_ids;
			public_detail_->artifacts.insert(
					std::make_pair(artifact_detail.artifact_id,
							artifact_detail));
		}
	}
	//}
}

void Public::init_artifacts(Public_Detail &detail,
		const Base_Detail &base_detail) {
	const Hero_Config* hero_config = CONFIG_CACHE_HERO->find_hero(
			player_hero_id(base_detail.career));
	if (!hero_config) {
		return;
	}
	Artifact_Detail artifact_detail;
	for (Int_Set::const_iterator it_artifact_ids =
			hero_config->artifact_ids.begin();
			it_artifact_ids != hero_config->artifact_ids.end();
			++it_artifact_ids) {
		if ((*it_artifact_ids) > PHI_END) {
			break;
		}
		if (!detail.artifacts.count(*it_artifact_ids)) {
			artifact_detail.reset();
			artifact_detail.artifact_id = *it_artifact_ids;
			detail.artifacts.insert(
					std::make_pair(artifact_detail.artifact_id,
							artifact_detail));
		}
	}
}

void Public::player_artifacts_prop_refresh(void) {
	// 人物属性加成
	player_self()->addition_prop_refresh(AT_ARTIFACTS, PST_FIXED,
			player_self()->artifacts_pro_val(), 263);
}

int Public::req_forenotice_receive_gift(const int32_t gift_lvl) {
	const Forenotice_Cfg_Lvl* forenotice_cfg_lvl =
	CONFIG_CACHE_PUBLIC->find_forenotice_lvl(gift_lvl);
	if (!forenotice_cfg_lvl) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	if (player_self()->level() < gift_lvl) {
		return ERROR_ROLE_LV_LIM;
	}
	if (public_detail_->fornotice_gifts_status.count(
			forenotice_cfg_lvl->level)) {
		return ERROR_HAS_RECEIVED;
	}

	MSG_81000102 acv_msg;
	Item_Vec item_vec;
	for (uint16_t i = 0; i < forenotice_cfg_lvl->gifts.size(); ++i) {
		if (!forenotice_cfg_lvl->gifts[i].id
				|| forenotice_cfg_lvl->gifts[i].amount <= 0) {
			continue;
		}
		Item_Detail item(forenotice_cfg_lvl->gifts[i].id,
				forenotice_cfg_lvl->gifts[i].amount,
				forenotice_cfg_lvl->gifts[i].bind);
		item_vec.push_back(item);
		Item_Basic_Info ibi;
		item.set_item_basic_info(ibi);
		acv_msg.item_info.push_back(ibi);
	}

	if (player_self()->pack_get_remain_capacity(PACK_T_PACKAGE_INDEX)
			< (int) item_vec.size()) {
		return ERROR_PACK_NOT_ENOUGH;
		//send_mail;
	}

	// insert in package
	if (!item_vec.empty()) {
		int ret_insert = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX,
				item_vec, Gain_Item_DM_Info(ITEM_GAIN_FORENOTICE_GIFT));
		if (ret_insert) {
			return ret_insert;
		}
	}

	// money gifts
	if (forenotice_cfg_lvl->copper > 0) {
		pack_add_money(
				Money_Add_Info(Pack::COPPER, forenotice_cfg_lvl->copper,
						MONEY_ADD_FORENOTICE_GIFTS));
		acv_msg.property.push_back(Property(Pack::COPPER, forenotice_cfg_lvl->copper));
	}
	if (forenotice_cfg_lvl->soul > 0) {
		pack_add_money(
				Money_Add_Info(Pack::SOULS, forenotice_cfg_lvl->soul,
						MONEY_ADD_FORENOTICE_GIFTS));
		acv_msg.property.push_back(Property(Pack::SOULS, forenotice_cfg_lvl->soul));
	}
	if (forenotice_cfg_lvl->bind_diamond > 0) {
		pack_add_money(
				Money_Add_Info(Pack::BIND_GOLD,
						forenotice_cfg_lvl->bind_diamond,
						MONEY_ADD_FORENOTICE_GIFTS));
		acv_msg.property.push_back(Property(Pack::BIND_GOLD, forenotice_cfg_lvl->bind_diamond));
	}
	if (forenotice_cfg_lvl->dragon_soul > 0) {
		pack_add_money(
				Money_Add_Info(Pack::DRAGON_SOUL,
						forenotice_cfg_lvl->dragon_soul,
						MONEY_ADD_FORENOTICE_GIFTS));
		acv_msg.property.push_back(Property(Pack::DRAGON_SOUL, forenotice_cfg_lvl->dragon_soul));
	}

	public_detail_->fornotice_gifts_status.insert(std::make_pair(gift_lvl, 1));

	public_detail_->detail_change();

	if (!acv_msg.item_info.empty() || !acv_msg.property.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	MSG_50120040 res_msg;
	res_msg.gift_lvl = gift_lvl;
	res_msg.result = 1;
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Public::req_forenotice_received_gifts(void) {
	MSG_50120041 res_msg;
	for (Fornotice_Gifts_Status_Map::const_iterator it_gifts =
			public_detail_->fornotice_gifts_status.begin();
			it_gifts != public_detail_->fornotice_gifts_status.end();
			++it_gifts) {
		res_msg.gift_lvls.push_back(it_gifts->first);
	}
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Public::req_exploit_info(void) {
	MSG_50120080 res_msg;
	res_msg.exploit_val = public_detail_->exploit_val;
	res_msg.exploit_lvl = public_detail_->exploit_lvl;
	res_msg.target_role_id = role_id();
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Public::active_exploit_info(Logic_Player* target_player) {
	if (! target_player) {
		return -1;
	}
	MSG_50120080 res_msg;
	res_msg.exploit_val = target_player->public_detail().exploit_val;
	res_msg.exploit_lvl = target_player->public_detail().exploit_lvl;
	res_msg.target_role_id = target_player->role_id();
	THIS_SEND_TO_CLIENT(res_msg);
	return 0;
}

int Public::title_nobility_announce(const int exploit_lvl) {
	const Nobility_Config *cfg = CONFIG_CACHE_PUBLIC->find_title_nobility_cfg(exploit_lvl);
	if (!cfg || 0 == cfg->broadcast_id) {
		return -1;
	}
	player_self()->create_ann_title_nobility(player_self(), cfg->name);
	return 0;
}

int Public::refresh_exploit_lvl(void) {
	const std::map<int, int>& cfg_exploit_lvl_map =
	CONFIG_CACHE_PUBLIC->cfg_exploit_lvl_map();
	if (cfg_exploit_lvl_map.empty()) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	int exploit_lvl = 0;
	for (std::map<int, int>::const_iterator it_cfg_lvl_map =
			cfg_exploit_lvl_map.begin();
			it_cfg_lvl_map != cfg_exploit_lvl_map.end(); ++it_cfg_lvl_map) {
		if (public_detail_->exploit_val >= it_cfg_lvl_map->second) {
			++exploit_lvl;
		} else {
			break;
		}
	}

	if (exploit_lvl > public_detail_->exploit_lvl) {
		public_detail_->exploit_lvl = exploit_lvl;
		public_detail_->detail_change();
		player_self()->achievement_listen_exploit_lvl();
	}

	return 0;
}

int Public::change_exploit_val(int change_val) {
	if (change_val) {
		public_detail_->exploit_val += change_val;
		public_detail_->exploit_val =
				public_detail_->exploit_val < 0 ?
						0 : public_detail_->exploit_val;
		refresh_exploit_lvl();
		req_exploit_info();
		player_self()->title_listen_exploit();
	}
	return 0;
}

int Public::req_handsel_flowers(uint32_t item_id, std::string &role_name,
		std::string &msg) {
	const Flowers_Config *conf =
	CONFIG_CACHE_FLOWERS->get_flowers_conf_by_item_id(item_id);
	if (!conf) {
		return ERROR_CLIENT_PARAM;
	}
	//判断是否为好友
	role_id_t role_id = LOGIC_MONITOR->get_role_id_by_name(role_name);
	if (!role_id) {
		return ERROR_FRIEND_ROLE_NOT_EXIST;
	}
	if (player_self()->is_friend(role_id)) {
		return ERROR_FRIEND_NOTEXIST;
	}
	//判断物品是否足够
	int return_status = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
			Id_Amount(conf->item_id, 1),
			Lose_Item_DM_Info(ITEM_LOSE_HANDSEL_FLOWERS));
	if (return_status != 0) {
		return return_status;
	}
	//判断是否有寄语
	if (!conf->have_msg) {
		msg.clear();
	}
	//获取对方玩家对象
	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(
			role_id);
	//处理飘花效果
	MSG_82200241 effec_msg;
	switch (conf->effect) {
	case 1: {	//单人
		THIS_SEND_TO_CLIENT(effec_msg);
		if (receiver) {
			OBJ_SEND_TO_CLIENT(effec_msg, (*receiver));
		}
		break;
	}
	case 2: {	//全服
		//效果buf
		Block_Buffer effec_buf;
		effec_buf.make_message(effec_msg.msg_id);
		effec_msg.serialize(effec_buf);
		effec_buf.finish_message();
		//全服飘花效果,由聊天服转发
		Block_Buffer buf;
		buf.make_message(LOGIC_CHAT_BROADCAST_BUFF);
		buf.write_int32(0);
		buf.copy(&effec_buf);
		buf.finish_message();
		this->send_to_local_chat(buf);
		break;
	}
	}
	//系统广播
	if (conf->announ) {
		//发送广播
		Chat_Item_Info item;
		item.reset();
		item.item_base_info.id = conf->item_id;
		item.item_base_info.amount = 1;
		player_self()->create_handsel_flowers_announce(conf->announ, role_id,
				item);
	}
	//魅力值增加 和 友好度增加
	{			//自己
		player_self()->add_charm(conf->charm);
		player_self()->add_friendship_degree(role_id, conf->friend_num);
		//聊天框提示
		MSG_81000102 mod_msg;
		mod_msg.reset();
		mod_msg.type = 0;			//0获得1消耗
		Property p; //金币501502、钻石501602、粉钻501702、英灵502903、exp502503, 魅力503503
		p.reset();
		p.type = 503503;	// 魅力503503
		p.value = conf->charm;
		mod_msg.property.push_back(p);
		THIS_SEND_TO_CLIENT(mod_msg);
	}
	{	//对方
		std::string my_role_name = player_self()->role_name();
		if (receiver) {	//对方在线情况
			receiver->self_receive_flowers(player_self()->role_id(),
					my_role_name, *conf, msg);
		} else {	//对方不在线情况
			Logic_Player::offline_receive_flowers(player_self()->role_id(),
					my_role_name, item_id, conf->charm, conf->friend_num, msg,
					role_id);
		}
	}
	{	//请求送花(返回)
		MSG_50120050 msg;
		msg.reset();
		msg.item_id = item_id;
		msg.charm = conf->charm;
		msg.friendship = conf->friend_num;
		THIS_SEND_TO_CLIENT(msg);
	}
	player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_HANDSEL_FLOWER);
	return 0;
}

int Public::req_fetch_secret_shop_info(void) {
	if (player_self() == NULL) {
		return -1;
	}
	return SECRET_SHOP_MANAGER->req_fetch_shop_info(player_self());
}

int Public::req_secret_shop_buy_item(int index) {
	if (player_self() == NULL) {
		return -1;
	}
	return SECRET_SHOP_MANAGER->req_buy_shop_item(player_self(), index);
}

void Public::offline_add_flowers(std::vector<Flowers_Info> &flowers_info_vec) {
	for (std::vector<Flowers_Info>::iterator it = flowers_info_vec.begin();
			it != flowers_info_vec.end(); ++it) {
		player_self()->add_charm(it->charm);
		player_self()->add_friendship_degree(it->role_id, it->friendship);
		public_detail_->flowers_info_vec.push_back(*it);
	}
	public_detail_->detail_change();
}

void Public::self_receive_flowers(role_id_t role_id, std::string &role_name,
		const Flowers_Config &conf, std::string &msg) {
	player_self()->add_charm(conf.charm);
	player_self()->add_friendship_degree(role_id, conf.friend_num);
	//通知收到鲜花
	MSG_82200240 act_msg;
	act_msg.reset();
	Flowers_Info fi;
	fi.reset();
	fi.role_id = role_id;
	fi.role_name = role_name;
	fi.item_id = conf.item_id;
	fi.charm = conf.charm;
	fi.friendship = conf.friend_num;
	fi.msg = msg;
	public_detail_->flowers_info_vec.push_back(fi);
	act_msg.info = public_detail_->flowers_info_vec;
	public_detail_->detail_change();
	THIS_SEND_TO_CLIENT(act_msg);
}

int Public::req_look_flowers(void) {
	{
		MSG_81000102 mod_msg;
		for (std::vector<Flowers_Info>::iterator it =
				public_detail_->flowers_info_vec.begin();
				it != public_detail_->flowers_info_vec.end(); ++it) {
			//聊天框提示
			mod_msg.reset();
			mod_msg.type = 0;	//0获得1消耗
			Property p; //金币501502、钻石501602、粉钻501702、英灵502903、exp502503, 魅力503503
			p.reset();
			p.type = 503503;	// 魅力503503
			p.value = it->charm;
			mod_msg.property.push_back(p);
			THIS_SEND_TO_CLIENT(mod_msg);
		}
	}
	public_detail_->flowers_info_vec.clear();
	public_detail_->detail_change();
	return 0;
}

void Public::get_war_income_info(int& max_income_num, int& remain_income_num,
		int& income_rate) {
	//
	//初始：第一场100%，第二场100%
	//装备初级奖章：第一场20%
	//装备中级奖章：第一场40%
	//装备高级奖章：第一场80%
	//装备顶级奖章：第一场120%
	//1 2 3 4
	int rate = 100;
	int remain_num = 0;
	int max_num = 2;		// 最大收益次数
	int ac_num = player_self()->achievement_war_scene_times();
	if (ac_num == 0) {
		max_num = 2;
		if (max_num > public_detail_->war_enter_num) {
			remain_num = max_num - public_detail_->war_enter_num;
		}
		if (remain_num == 2) {
			rate = 100;
		} else if (remain_num == 1) {
			rate = 100;
		} else {
			rate = 0;
		}
	} else if (ac_num == 1) {
		max_num = 2;
		if (max_num > public_detail_->war_enter_num) {
			remain_num = max_num - public_detail_->war_enter_num;
		}
		if (remain_num == 2) {
			rate = 120;
		} else if (remain_num == 1) {
			rate = 100;
		} else {
			rate = 0;
		}
	} else if (ac_num == 2) {
		max_num = 2;
		if (max_num > public_detail_->war_enter_num) {
			remain_num = max_num - public_detail_->war_enter_num;
		}
		if (remain_num == 2) {
			rate = 140;
		} else if (remain_num == 1) {
			rate = 100;
		} else {
			rate = 0;
		}
	} else if (ac_num == 3) {
		max_num = 2;
		if (max_num > public_detail_->war_enter_num) {
			remain_num = max_num - public_detail_->war_enter_num;
		}
		if (remain_num == 2) {
			rate = 180;
		} else if (remain_num == 1) {
			rate = 100;
		}else {
			rate = 0;
		}
	} else if (ac_num >= 4) {
		max_num = 2;
		if (max_num > public_detail_->war_enter_num) {
			remain_num = max_num - public_detail_->war_enter_num;
		}
		if (remain_num == 2) {
			rate = 220;
		} else if (remain_num == 1) {
			rate = 100;
		} else {
			rate = 0;
		}
	}
	if(remain_num < 0){
		remain_num = 0;
	}
	max_income_num = max_num;
	remain_income_num = remain_num;
	income_rate = rate;
}

int Public::req_campfire_enter(void) {
	if (!public_detail_ || player_self() == NULL) {
		return -1;
	}
	const Scene_Config *scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
			player_self()->location().scene_id);
	if (scene_cfg && scene_cfg->type != Main_Scene_Type) {
		return ERROR_COPY_IS_GOING;
	}
	int level_limit = CAMPFIRE_MANAGER->get_enter_level_limit();
	if (level() < level_limit) {
		return ERROR_BONFIRE_ENTER_LV_LIMIT;
	}
	int act_id = 0;
	int act_status = 0;
	int remain_time = 0;
	CAMPFIRE_MANAGER->get_active_info(act_status, remain_time, act_id);
	if (act_status != 3) {
		return ERROR_BONFIRE_NO_START;
	}
	int dest_scene = 80214001;
	int fire_point_id = 0;
	gang_id_t gang_id = player_self()->ganger_detail().gang_id;
	std::string gang_name = GANG_MANAGER->gang_name(gang_id);
	if(gang_id != 0){
		fire_point_id = CAMPFIRE_MANAGER->get_campfire_point_id(gang_id);
	}
	Coord coord(4433, 1388, 1);
	coord = CONFIG_CACHE_SECRET_SHOP->get_campfire_birth_coord(fire_point_id);
	std::string arg3 = "";
	int64_t arg1 = 0;

	// sync to scene
	Block_Buffer buf;
	buf.make_message(INNER_CAMPFIRE_SCENE_ENTER);
	buf.write_int64(role_id());
	buf.write_int8(public_detail_->add_campfire);
	buf.write_int64(gang_id);
	buf.write_string(gang_name);
	buf.write_int8(player_self()->ganger_detail().headship);
	buf.finish_message();
	player_self()->monitor()->send_to_scene(dest_scene, 0, buf);

	// enter scene
	int transmit_result = player_self()->inner_transmit(dest_scene, 0, coord, arg1, arg3);
	if(transmit_result == 0){
		// TOD助手日常和活跃度侦听
		player_self()->daily_helper_listen(DAILY_FULFILL_LISTEN_CAMPFIRE);
		// 奖励找回监听当次活动首次参与
		if (act_id != this->public_detail_->campfire_id) {
			this->public_detail_->campfire_id = act_id;
			this->public_detail_->detail_change();
			player_self()->reward_back_listen_fulfill(REWARD_BACK_PRO_PARTY);
		}
		// 进入篝火精力恢复定时处理
		player_self()->handle_energy_fire();
	}
	return transmit_result;
}

int Public::req_act_enter_scene(const int active_id){
	if (!public_detail_ || player_self() == NULL) {
		return -1;
	}
	const Scene_Config *scene_cfg = CONFIG_CACHE_SCENE->scene_config_cache(
			player_self()->location().scene_id);
	if (scene_cfg && scene_cfg->type != Main_Scene_Type) {
		return ERROR_COPY_IS_GOING;
	}
	switch(active_id){
		case 16:{
			return WORLD_BOSS_ACTIVE_MANAGER->req_enter_scene(player_self());
			break;
		}
		case 80210001:
		case 19:{
			return GANG_BOSS_ACTIVE_MANAGER->req_enter_scene(player_self());
			break;
		}
		case 22:{
			return HONOR_ARENA_ACTIVE_MANAGER->req_enter_scene(player_self());
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

int Public::req_world_boss_kill_info(const int8_t icon_id){
	if(!player_self()){
		return 0;
	}
	role_id_t gang_id = player_self()->gang_id();
	int kill_num = 0;
	int hour = 0;
	int minute = 0;
	int boss_lv = 0;
	GANG_MANAGER->get_boss_kill_info(gang_id, kill_num, boss_lv, hour, minute);
	MSG_50111102 msg;
	msg.icon_id = icon_id;
	msg.kill_num = kill_num;
	msg.boss_lv = boss_lv;
	msg.value1 = hour;
	msg.value2 = minute;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::req_world_boss_set_time(const int8_t icon_id, const int8_t hour, const int8_t minute){
	if(!player_self()){
		return 0;
	}
	// check valid
	int8_t r_hour;
	int8_t r_minute;
	role_id_t gang_id = player_self()->gang_id();
	int result = GANG_MANAGER->set_boss_open_time(gang_id, hour, minute, r_hour, r_minute);
	if(result == 0){
		char str_out[64];
		memset(str_out, 0, sizeof(str_out));
		snprintf(str_out, sizeof(str_out), "%02d:%02d", r_hour, r_minute);
		player_self()->create_gang_boss_announce(gang_id, 7113, role_id(), str_out);
	}
	MSG_50111103 msg;
	msg.icon_id = icon_id;
	msg.hour = r_hour;
	msg.minute = r_minute;
	if(result == 0){
		msg.result = 1;
	}else{
		msg.result = 0;
	}
	THIS_SEND_TO_CLIENT(msg);
	return result;
}

int Public::cmd_add_firewood(int add_campfire) {
	if (!public_detail_ || player_self() == NULL) {
		return -1;
	}
	public_detail_->add_campfire = add_campfire;
	public_detail_->detail_change();
	return 0;
}

int Public::save_campfire_player_info(const MSG_20300116& msg) {
	public_detail_->campfire_finish_time = msg.finish_time;
	public_detail_->campfire_rank = msg.rank;
	std::vector<int32_t>::const_iterator iter;
	for (iter = msg.killed_match_man.begin(); iter != msg.killed_match_man.end(); ++iter) {
		public_detail_->killed_match_man.insert(*iter);
	}
	public_detail_->campfire_lighter = msg.is_lighter;
	public_detail_->campfire_recieve_stat = msg.recieve_stat;
	public_detail_->campfire_rank_rev_stat = msg.rank_rev_stat;
	public_detail_->add_campfire = msg.add_firewood;

	public_detail_->detail_change();

	return 0;
}

int Public::activity_result(int act_id, int type, int result, int value1, int value2, int value3) {
	switch (act_id) {
	case FuBen_Scene_Type: {
		if (type == 1) {
			player_self()->daily_helper_listen(
					DAILY_FULFILL_LISTEN_SINGLE_EXPLORE);
		}
		break;
	}
	case Elite_Scene_Type: {
		if (type == 1) {
			player_self()->daily_helper_listen(
					DAILY_FULFILL_LISTEN_ELITE_EXPLORE);
		}
		break;
	}
	case Team_Scene_Type: {
//		Team *team = player_self()->get_team();
//		if (team && team->type() == Team::TEAM_TYPE_SPORTS) {
//			player_self()->team_sub_profile_num();
//		}
		break;
	}
	case Team_Fb_Scene_Type: {
		if (type == 1) {
			player_self()->daily_helper_listen(
					DAILY_FULFILL_LISTEN_TEAM_EXPLORE);
			player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_TEAM_FB_NUM);
			if (0 == player_self()->team_sub_profile_num()) {
				player_self()->reward_back_listen_fulfill(
						REWARD_BACK_PRO_TEAM_EXPLORE);
			}
		}
		break;
	}
	case TEAM_DEFEND_Scene_Type: {
		if (type == 1) {
			player_self()->daily_helper_listen(
					DAILY_FULFILL_LISTEN_Team_Defend);
			if (0 == player_self()->team_sub_profile_num()) {
				player_self()->reward_back_listen_fulfill(
						REWARD_BACK_PRO_TEAM_DEFEND);
			}
		}
		break;
	}
	case HERO_DREAM_Scene_Type: {
		if (type == 1) {
			if (0 == player_self()->team_sub_profile_num()) {
				player_self()->reward_back_listen_fulfill(
						REWARD_BACK_PRO_HERO_DREAMS);
			}
		}
		break;
	}
	case War_Scene_Type: {
		if (type == 0) {
			if (result == 1) {
				this->player_self()->task_listen_war();
				this->player_self()->active_content_listen_warer_win();
				if (value2 == 1) {
					player_self()->achievement_listen_war_light_win_nums();
				} else {
					player_self()->achievement_listen_war_dark_win_nums();
				}
			}
			// 额外奖励记录
			inner_war_scene_result(result, 0, value1, value3);

		}else if(type == 1){
			uint32_t kill_num = value1;
			this->player_self()->task_listen_kill_enemy_in_war(kill_num);
		}
		break;
	}
	default: {
		break;
	}
	}
	return 0;
}

void Public::public_plot_un_broadcast(const bool ub) {
	un_broadcast_ = ub;
}

void Public::send_equip_look(void) {
	MSG_81000408 acv_msg;
	acv_msg.icon_id = 6;
	if (level() > 35) {
		acv_msg.open = 0;
		acv_msg.active = 0;
		acv_msg.time = 0;
	} else if (level() >= 18 && level() <= 35) {
		acv_msg.open = 1;
		acv_msg.active = 0;
		THIS_SEND_TO_CLIENT(acv_msg);
	}
}
void Public::hook_equip_look_level_up(const int new_level) {
	MSG_81000408 acv_msg;
	acv_msg.icon_id = 6;
	if (level() > 35) {
		acv_msg.open = 0;
		acv_msg.active = 0;
		acv_msg.time = 0;
		THIS_SEND_TO_CLIENT(acv_msg);
	} else if (level() >= 18 && level() <= 35) {
		acv_msg.open = 1;
		acv_msg.active = 0;
		THIS_SEND_TO_CLIENT(acv_msg);
	}

	Time_Value now = Time_Value::gettimeofday();
	{//首充图标通知
		uint32_t continue_time = CONFIG_CACHE_RECHARGE->get_common_config_by_key(RCCT_FIRST_RECHARGE_TIME)*Time_Value::ONE_MINUTE_IN_SECS;
		if (public_detail_->first_recharge < First_Recharge_Receive
				&& (uint32_t)player_self()->level() == CONFIG_CACHE_RECHARGE->get_common_config_by_key(RCCT_FIRST_RECHRAGE_START_LEVEL)) {
			public_detail_->start_frist_recharge_time = Time_Value::gettimeofday();
			public_detail_->detail_change();
			if(now.sec() < (public_detail_->start_frist_recharge_time.sec()+ continue_time)) {
				MSG_81000408 msg;
				msg.open = 1;
				msg.active = public_detail_->first_recharge;
				msg.time = (public_detail_->start_frist_recharge_time.sec()+ continue_time) - now.sec();
				msg.icon_id = 8;
				THIS_SEND_TO_CLIENT(msg);
			}
		}
	}
	{//限时充值活动图标通知
		if (public_detail_->limit_time_recharge < First_Recharge_Receive
				&& (uint32_t)player_self()->level() >= LIMIT_TIME_RECHARGE_MANAGER->get_icon_limit_level()
				&& now.sec() < LIMIT_TIME_RECHARGE_MANAGER->get_end_time().sec() && now.sec() >= LIMIT_TIME_RECHARGE_MANAGER->get_start_time().sec()) {
			MSG_81000408 msg;
			msg.open = 1;
			msg.active = public_detail_->limit_time_recharge;
			msg.time = 0;
			msg.icon_id = LIMIT_TIME_RECHARGE_MANAGER->get_icon_time_status().icon_id;
			THIS_SEND_TO_CLIENT(msg);
		}
	}
}

int Public::send_proposal(uint8_t type, std::string &contents) {
	if (type >= 4 || type <= 0) {
		return ERROR_CLIENT_PARAM;
	}
	Block_Buffer buf;
	buf.make_message(MONGO_DATA_SAVE_PROPOSAL);
	buf.write_int64(player_self()->role_id());
	buf.write_uint8(type);
	buf.write_string(contents);
	buf.write_int64(Time_Value::gettimeofday().sec());
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);

	LOGIC_PUBLIC->send_mail(1032, player_self()->role_id());

	MSG_50120052 msg;
	msg.reset();
	msg.type = type;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Public::recharge_listen(uint32_t num) {
	Time_Value now = Time_Value::gettimeofday();
	//send email
	/***********************************************/
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(1043);
	if(mailc) {
		std::string contents;
		const int size_buf = 1024;
		char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mailc->mail_content.c_str(), num);
		contents.assign(content_buf);
		LOGIC_PUBLIC->send_mail(1043, player_self()->role_id(),  "", contents);
	}
	/************************************************/
	//end email end

		if (public_detail_->first_recharge == First_Recharge_No) {
			public_detail_->first_recharge = First_Recharge_Has;
		}

	if (public_detail_->two_recharge == First_Recharge_No) {
		public_detail_->two_recharge = First_Recharge_Has;
		req_open_two_recharge();
	}

	if(LIMIT_TIME_RECHARGE_MANAGER->get_start_time().sec() <= now.sec() && now.sec() < LIMIT_TIME_RECHARGE_MANAGER->get_end_time().sec()) {
		if (public_detail_->limit_time_recharge == First_Recharge_No) {
			public_detail_->limit_time_recharge = First_Recharge_Has;
			req_open_limit_time_recharge();
		}
	}

	{//首充图标通知
		uint32_t continue_time = CONFIG_CACHE_RECHARGE->get_common_config_by_key(RCCT_FIRST_RECHARGE_TIME)*Time_Value::ONE_MINUTE_IN_SECS;
		if (public_detail_->first_recharge < First_Recharge_Receive
				&& (uint32_t)player_self()->level() >= CONFIG_CACHE_RECHARGE->get_common_config_by_key(RCCT_FIRST_RECHRAGE_START_LEVEL)
				&& now.sec() < (public_detail_->start_frist_recharge_time.sec()+ continue_time)) {
			MSG_81000408 msg;
			msg.open = 1;
			msg.active = public_detail_->first_recharge;
			msg.time = (public_detail_->start_frist_recharge_time.sec()+ continue_time) - now.sec();
			msg.icon_id = 8;
			THIS_SEND_TO_CLIENT(msg);
		}
	}

	public_detail_->recharge_money += num;
	public_detail_->recharge_money_his += num;
	public_detail_->recharge_activities_money += num;
	public_detail_->detail_change();
	player_self()->achievement_listen_recharge_diamond();
	player_self()->title_listen_recharge();
	active_rechare_rebate_status();
	req_open_recharge_rebate();
	req_open_first_recharge();
	player_self()->active_listen_recharge();
	{
		Date_Time oper_tmp_date = Date_Time(now);
		oper_tmp_date.hour(0);
		oper_tmp_date.second(0);
		oper_tmp_date.minute(0);
		player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_RECHARGE_ON_DAY, num, oper_tmp_date.time_sec());
	}
	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_RECHARGE_NUM, num);
	player_self()->oper_activity_listen_type_common(OPER_ACTIVITY_ITEM_RECHARGE_NUM_AND_LOGIN_DAILY_NUM, num);
}

int Public::req_open_two_recharge(void) {
	reset_two_recharge();
	MSG_50120099 msg;
	msg.status = public_detail_->two_recharge;
	msg.rec_num = TWO_RECHARGE_MANAGER->get_rec_num();
	msg.over_time = TWO_RECHARGE_MANAGER->get_end_time().sec();
//	Time_Value open_time_zero = Time_Value::zero;
//	get_zero_time(CONFIG_CACHE->server_maintainer_cache().game_open_time, open_time_zero);
//	msg.open_time = open_time_zero.sec();
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::req_open_limit_time_recharge(void) {
	reset_limit_time_recharge();
	MSG_50120101 msg;
	msg.status = public_detail_->limit_time_recharge;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::req_receive_two_recharge(void) {
	reset_two_recharge();
	if (public_detail_->two_recharge != First_Recharge_Has) {
		return ERROR_CLIENT_PARAM;
	}
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	//领取奖励
	const First_Recharge_Reward_Config &conf =
	CONFIG_CACHE_RECHARGE->get_two_recharge_item_reward();
	if (conf.reward_item.size()) {
		Item_Vec item;
		for (First_Recharge_Item_Config_Vec::const_iterator item_it = conf.reward_item.begin();
				item_it != conf.reward_item.end(); ++item_it) {
			if(item_it->time == 0 || item_it->time > (uint32_t)get_time_subdays(CONFIG_CACHE->server_maintainer_cache().game_open_time, Time_Value::gettimeofday())) {
				Item_Detail it_em(item_it->item_id, item_it->num,
						Item_Detail::UNDEFINED);
				item.push_back(it_em);
				Item_Basic_Info ibi;
				it_em.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
			}
		}
		if (item.size()) {
			int result = player_self()->pack_insert_item(
					Pack::PACK_T_PACKAGE_INDEX, item,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_FIRST_RECHARGE),
					TIPS_TYPE(GET_ITEM), Pack_Try(WITH_TRY),
					FIRST_RECHARGE_MAIL_ID);
			if (result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if (conf.reward_money.size()) {
		Money_Add_List money;
		money.clear();
		for (First_Recharge_Item_Config_Vec::const_iterator money_it = conf.reward_money.begin();
				money_it != conf.reward_money.end(); ++money_it) {
			if(money_it->time == 0 || money_it->time > (uint32_t)get_time_subdays(CONFIG_CACHE->server_maintainer_cache().game_open_time, Time_Value::gettimeofday())) {
				money.push_back(
						Money_Add_Info(Money_Type(money_it->item_id),
								money_it->num,
								Money_DM_Info(MONEY_ADD_FIRST_RECHARGE)));
				acv_msg.property.push_back(
						Property(money_it->item_id, money_it->num));
			}
		}
		if (money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if (conf.exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(conf.exp, false, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	public_detail_->two_recharge = First_Recharge_Receive;
	public_detail_->detail_change();

	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	TWO_RECHARGE_MANAGER->up_rec_num();
	{		//发送回复消息
		MSG_50120100 msg;
		msg.rec_num = TWO_RECHARGE_MANAGER->get_rec_num();
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Public::req_receive_limit_time_recharge(void) {
	reset_limit_time_recharge();
	if (public_detail_->limit_time_recharge != First_Recharge_Has) {
		return ERROR_CLIENT_PARAM;
	}
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	//领取奖励
	const Limit_Time_Recharge_Reward_Config &conf =
	CONFIG_CACHE_RECHARGE->get_limit_time_recharge_reward();
	if (conf.reward_item.size()) {
		Item_Vec item;
		for (First_Recharge_Item_Config_Vec::const_iterator item_it = conf.reward_item.begin();
				item_it != conf.reward_item.end(); ++item_it) {
			if(item_it->time == 0 || item_it->time > (uint32_t)get_time_subdays(CONFIG_CACHE->server_maintainer_cache().game_open_time, Time_Value::gettimeofday())) {
				Item_Detail it_em(item_it->item_id, item_it->num,
						Item_Detail::UNDEFINED);
				item.push_back(it_em);
				Item_Basic_Info ibi;
				it_em.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
			}
		}
		if (item.size()) {
			int result = player_self()->pack_insert_item(
					Pack::PACK_T_PACKAGE_INDEX, item,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_LIMIT_TIME_RECHARGE),
					TIPS_TYPE(GET_ITEM), Pack_Try(WITH_TRY),
					FIRST_RECHARGE_MAIL_ID);
			if (result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if (conf.reward_money.size()) {
		Money_Add_List money;
		money.clear();
		for (First_Recharge_Item_Config_Vec::const_iterator money_it = conf.reward_money.begin();
				money_it != conf.reward_money.end(); ++money_it) {
			if(money_it->time == 0 || money_it->time > (uint32_t)get_time_subdays(CONFIG_CACHE->server_maintainer_cache().game_open_time, Time_Value::gettimeofday())) {
				money.push_back(
						Money_Add_Info(Money_Type(money_it->item_id),
								money_it->num,
								Money_DM_Info(MONEY_ADD_LIMIT_TIME_RECHARGE)));
				acv_msg.property.push_back(
						Property(money_it->item_id, money_it->num));
			}
		}
		if (money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if (conf.exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(conf.exp, false, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	public_detail_->limit_time_recharge = First_Recharge_Receive;
	public_detail_->detail_change();

	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	{		//发送回复消息
		MSG_50120102 msg;
		THIS_SEND_TO_CLIENT(msg);
	}
	{		//发送关闭图标
		MSG_81000408 msg;
		msg.open = 0;
		msg.active = 1;
		msg.time = 0;
		msg.icon_id = LIMIT_TIME_RECHARGE_MANAGER->get_icon_time_status().icon_id;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

void Public::reset_two_recharge(void) {
	if(public_detail_->last_two_recharge_stime.sec() != TWO_RECHARGE_MANAGER->get_start_time().sec()) {
		public_detail_->last_two_recharge_stime = TWO_RECHARGE_MANAGER->get_start_time();
		public_detail_->two_recharge = First_Recharge_No;
		public_detail_->detail_change();
	}
}

void Public::reset_limit_time_recharge(void) {
	if(public_detail_->last_limit_time_recharge_stime.sec() != LIMIT_TIME_RECHARGE_MANAGER->get_start_time().sec()) {
		public_detail_->last_limit_time_recharge_stime = LIMIT_TIME_RECHARGE_MANAGER->get_start_time();
		public_detail_->limit_time_recharge = First_Recharge_No;
		public_detail_->detail_change();
	}
}

int Public::req_open_first_recharge(void) {
	MSG_50120090 msg;
	msg.status = public_detail_->first_recharge;
	Time_Value open_time_zero = Time_Value::zero;
	get_zero_time(CONFIG_CACHE->server_maintainer_cache().game_open_time, open_time_zero);
	msg.open_time = open_time_zero.sec();
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Public::first_over_receive_to_mail(void) {
	if (public_detail_->first_recharge != First_Recharge_Has) {
		return;
	}
	Time_Value now = Time_Value::gettimeofday();
	if (now.sec() > FIRST_RECHARGE_MANAGER->get_end_time().sec()) {
		//领取奖励
		const First_Recharge_Reward_Config &conf =
		CONFIG_CACHE_RECHARGE->get_first_recharge_item_reward();
		if (conf.reward_item.size()) {
			Item_Vec item;
			for (First_Recharge_Item_Config_Vec::const_iterator item_it = conf.reward_item.begin();
					item_it != conf.reward_item.end(); ++item_it) {
				if(item_it->time == 0 || item_it->time > (uint32_t)get_time_subdays(CONFIG_CACHE->server_maintainer_cache().game_open_time, Time_Value::gettimeofday())) {
					Item_Detail it_em(item_it->item_id, item_it->num,
							Item_Detail::UNDEFINED);
					item.push_back(it_em);
				}
			}
			if (item.size()) {
				LOGIC_PUBLIC->send_mail(1046, player_self()->role_id(), item);
			}
		}
		public_detail_->first_recharge = First_Recharge_Receive;
		public_detail_->detail_change();
	}
}

int Public::req_receive_first_recharge(void) {
	if (public_detail_->first_recharge != First_Recharge_Has) {
		return ERROR_CLIENT_PARAM;
	}
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	//领取奖励
	const First_Recharge_Reward_Config &conf =
	CONFIG_CACHE_RECHARGE->get_first_recharge_item_reward();
	if (conf.reward_item.size()) {
		Item_Vec item;
		for (First_Recharge_Item_Config_Vec::const_iterator item_it = conf.reward_item.begin();
				item_it != conf.reward_item.end(); ++item_it) {
			if(item_it->time == 0 || item_it->time > (uint32_t)get_time_subdays(CONFIG_CACHE->server_maintainer_cache().game_open_time, Time_Value::gettimeofday())) {
				Item_Detail it_em(item_it->item_id, item_it->num,
						Item_Detail::UNDEFINED);
				item.push_back(it_em);
				Item_Basic_Info ibi;
				it_em.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
			}
		}
		if (item.size()) {
			int result = player_self()->pack_insert_item(
					Pack::PACK_T_PACKAGE_INDEX, item,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_FIRST_RECHARGE),
					TIPS_TYPE(GET_ITEM), Pack_Try(WITH_TRY),
					FIRST_RECHARGE_MAIL_ID);
			if (result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if (conf.reward_money.size()) {
		Money_Add_List money;
		money.clear();
		for (First_Recharge_Item_Config_Vec::const_iterator money_it = conf.reward_money.begin();
				money_it != conf.reward_money.end(); ++money_it) {
			if(money_it->time == 0 || money_it->time > (uint32_t)get_time_subdays(CONFIG_CACHE->server_maintainer_cache().game_open_time, Time_Value::gettimeofday())) {
				money.push_back(
						Money_Add_Info(Money_Type(money_it->item_id),
								money_it->num,
								Money_DM_Info(MONEY_ADD_FIRST_RECHARGE)));
				acv_msg.property.push_back(
						Property(money_it->item_id, money_it->num));
			}
		}
		if (money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if (conf.exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(conf.exp, false, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	public_detail_->first_recharge = First_Recharge_Receive;
	public_detail_->detail_change();

	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	{		//发送回复消息
		MSG_50120091 msg;
		THIS_SEND_TO_CLIENT(msg);
	}
	{		//发送关闭图标
		MSG_81000408 msg;
		msg.open = 0;
		msg.active = 1;
		msg.time = 0;
		msg.icon_id = 8;
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

void Public::reset_recharge_rebate_data(void) {
	const Time_Value &start_time = RECHARGE_REBATE_MANAGER->get_start_time();
	if (start_time.sec() != public_detail_->last_recharge_rebate_stime.sec()) {
		public_detail_->recharge_money = 0;
		public_detail_->recharge_feedback_receive_list.clear();
		public_detail_->last_recharge_rebate_stime = start_time;
		public_detail_->detail_change();
	}
}

int Public::req_recharge_rebate_status(void) {
	active_rechare_rebate_status();
	return 0;
}
double Public::get_world_level_add_percent(void) {
	if (0 == get_world_level_add()) {
		return 1.0;
	}
	return  (get_world_level_add() + 100)/ 100.0;
}
int Public::get_world_level_add(void) {
	return CONFIG_CACHE_PUBLIC->get_world_level_add_exp(level(), RANKING_MANAGER->world_level());
}
void Public::active_rechare_rebate_status(void) {
	reset_recharge_rebate_data();		//有可能需要刷新充值返利
	Time_Value now = Time_Value::gettimeofday();
	const Time_Value &end_time = RECHARGE_REBATE_MANAGER->get_end_time();
	if (now.sec() < end_time.sec()) {
		MSG_82200203 msg;
		msg.reset();
		const Recharge_Rebate_Config_Map &conf =
		CONFIG_CACHE_RECHARGE->get_recharge_rebate_reward();
		for (Recharge_Rebate_Config_Map::const_iterator it = conf.begin();
				it != conf.end(); ++it) {
			if(it->second.act_id && it->second.act_id != RECHARGE_REBATE_MANAGER->get_act_id()) continue;
			if (it->second.money <= public_detail_->recharge_money) {
				if (public_detail_->recharge_feedback_receive_list.find(
						it->first)
						== public_detail_->recharge_feedback_receive_list.end()) {
					//++
					++msg.num;
				}
			}
		}
		if (msg.num) {
			THIS_SEND_TO_CLIENT(msg);
		}
	}
}

int Public::req_open_recharge_rebate(void) {
	reset_recharge_rebate_data();		//有可能需要刷新充值返利
	Time_Value now = Time_Value::gettimeofday();
	const Time_Value &end_time = RECHARGE_REBATE_MANAGER->get_end_time();
	if (now.sec() < end_time.sec()) {
		MSG_50120092 msg;
		msg.reset();
		msg.addup_money = public_detail_->recharge_money;
		msg.end_time = end_time.sec();
		msg.act_id = RECHARGE_REBATE_MANAGER->get_act_id();
		const Recharge_Rebate_Config_Map &conf =
		CONFIG_CACHE_RECHARGE->get_recharge_rebate_reward();
		Uint_Uint uu;
		for (Recharge_Rebate_Config_Map::const_iterator it = conf.begin();
				it != conf.end(); ++it) {
			if(it->second.act_id && it->second.act_id != RECHARGE_REBATE_MANAGER->get_act_id()) continue;
			uu.reset();
			uu.val_1 = it->first;
			if (it->second.money <= public_detail_->recharge_money) {
				if (public_detail_->recharge_feedback_receive_list.find(
						it->first)
						== public_detail_->recharge_feedback_receive_list.end()) {
					uu.val_2 = Recharge_Rebate_Can;
				} else {
					uu.val_2 = Recharge_Rebate_Receive;
				}
			} else {
				uu.val_2 = Recharge_Rebate_No;
			}
			msg.status.push_back(uu);
		}
		THIS_SEND_TO_CLIENT(msg);
	}
	return 0;
}

int Public::req_receive_recharge_rebate(uint32_t id) {
	//判断活动时间
	Time_Value now = Time_Value::gettimeofday();
	const Time_Value &end_time = RECHARGE_REBATE_MANAGER->get_end_time();
	if (now.sec() > end_time.sec()) {
		return ERROR_CLIENT_PARAM;
	}
	//判断配置是否有客户端传上来的ID
	const Recharge_Rebate_Config_Map &conf =
	CONFIG_CACHE_RECHARGE->get_recharge_rebate_reward();
	Recharge_Rebate_Config_Map::const_iterator conf_val = conf.find(id);
	if (conf_val == conf.end()) {
		return ERROR_CLIENT_PARAM;
	}
	if(conf_val->second.act_id && conf_val->second.act_id != RECHARGE_REBATE_MANAGER->get_act_id()) {
		return ERROR_CLIENT_PARAM;
	}
	//判断是否能领取
	if (conf_val->second.money > public_detail_->recharge_money) {
		return ERROR_CLIENT_PARAM;
	}
	//判断客户端传上来的ID是否已经领取过
	if (public_detail_->recharge_feedback_receive_list.find(conf_val->first)
			!= public_detail_->recharge_feedback_receive_list.end()) {
		return ERROR_CLIENT_PARAM;
	}
	//发放奖励
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if (conf_val->second.reward.reward_item.size()) {
		Item_Vec item;
		for (UInt_UInt_Map::const_iterator item_it =
				conf_val->second.reward.reward_item.begin();
				item_it != conf_val->second.reward.reward_item.end();
				++item_it) {
			make_item_vec(item, item_it->first, item_it->second, Item_Detail::UNDEFINED);
//			Item_Detail it_em(item_it->first, item_it->second,
//					Item_Detail::UNDEFINED);
//			item.push_back(it_em);
//			Item_Basic_Info ibi;
//			it_em.set_item_basic_info(ibi);
//			acv_msg.item_info.push_back(ibi);
		}
		Item_Basic_Info ibi;
		Card_Info card_info;
		for (Item_Vec::iterator it = item.begin(); it != item.end(); ++it) {
			ibi.reset();
			card_info.reset();
			(*it).set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
			if ((*it).set_item_card_info(card_info)) {
				acv_msg.card_info_vec.push_back(card_info);
			}
		}
		if (item.size()) {
			int result = player_self()->pack_insert_item(
					Pack::PACK_T_PACKAGE_INDEX, item,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_FIRST_RECHARGE),
					TIPS_TYPE(GET_ITEM), Pack_Try(WITH_TRY),
					FIRST_RECHARGE_MAIL_ID);
			if (result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if (conf_val->second.reward.reward_money.size()) {
		Money_Add_List money;
		money.clear();
		for (UInt_UInt_Map::const_iterator money_it =
				conf_val->second.reward.reward_money.begin();
				money_it != conf_val->second.reward.reward_money.end();
				++money_it) {
			money.push_back(
					Money_Add_Info(Money_Type(money_it->first),
							money_it->second,
							Money_DM_Info(MONEY_ADD_FIRST_RECHARGE)));
			acv_msg.property.push_back(
					Property(money_it->first, money_it->second));
		}
		if (money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if (conf_val->second.reward.exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(conf_val->second.reward.exp, false, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	//记录领取ID
	public_detail_->recharge_feedback_receive_list.insert(id);
	public_detail_->detail_change();
	//发送公告
	if (conf_val->second.ann_id) {
		player_self()->create_ann_recharge_rebate(conf_val->second.ann_id);
	}
	//回应客户端
	MSG_50120093 msg;
	msg.reset();
	msg.id = id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Public::listen_task_status_change(uint32_t task_id, uint8_t task_status) {
	bool is_need_save = false;
	const Notice_Cfg_Map &notice_cfg_map =  CONFIG_CACHE_PUBLIC->get_notice_cfg_map();
	for(Notice_Cfg_Map::const_iterator it = notice_cfg_map.begin(); it != notice_cfg_map.end(); ++it) {
		if(it->second.type == NOTICE_TYPE_TASK_DELETE_NOTICE
				&& it->second.id > public_detail_->notice_id
				&& it->second.task_id == task_id
				&& it->second.task_status == task_status) {
			MSG_81110013 msg;
			msg.id = it->second.id;
			msg.type = 0;
			THIS_SEND_TO_CLIENT(msg);

			public_detail_->notice_id = it->second.id;
			public_detail_->is_show = false;

			is_need_save = true;
		}

		if(it->second.type == NOTICE_TYPE_TASK
				&& it->second.id > public_detail_->notice_id
				&& it->second.task_id == task_id
				&& it->second.task_status == task_status) {
			public_detail_->notice_id = it->second.id;
			public_detail_->is_played_effect = false;
			public_detail_->is_show = true;
			MSG_81110013 msg;
			msg.id = public_detail_->notice_id;
			msg.type = 1;
			THIS_SEND_TO_CLIENT(msg);

			is_need_save = true;
		}
	}

	if(is_need_save) {
		public_detail_->detail_change();
	}
}

int Public::req_set_forenotice_played_effect(uint32_t id) {
	if(public_detail_->notice_id != id) {
//		return ERROR_NOTICE_ID_NOT_EXIST;
		MSG_USER("notice_id=%d, id=%d", public_detail_->notice_id, id);
		return 0;
	}

	public_detail_->is_played_effect = true;
	public_detail_->detail_change();

	return 0;
}

int Public::req_send_challenge_to_gang_chat(int64_t role_id, std::string role_name) {
	if(!player_self()->gang_id()) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	Gang_DB_View* gang_view = GANG_MANAGER->gang_view_by_id(player_self()->gang_id());
	if (! gang_view) {
		return ERROR_GANG_NOT_IN_GANG;
	}

	const Challenge_Cfg &challenge_cfg = CONFIG_CACHE_PUBLIC->get_challenge_cfg();
	if(player_self()->ganger_detail().cur_contribute < (int)challenge_cfg.ganger_devote) {
		return ERROR_GANGER_CONTRIBUTE_LACK;
	}

	player_self()->ganger_detail_no_const().cur_contribute -= challenge_cfg.ganger_devote;
	player_self()->ganger_detail_no_const().detail_change();
	player_self()->active_ganger_info_update();

	player_self()->create_ann_challenge(role_id, role_name);
	return 0;
}

int Public::req_questionnaire_info(void) {
	const Questionnaire_Config_Map &conf = CONFIG_CACHE_PUBLIC->get_questionnaire_config();
//	if(public_detail_->ans_qid.size() >= conf.size()) {
//		return 0;
//	}
	MSG_50120097 msg;
	for(Questionnaire_Config_Map::const_iterator it = conf.begin(); it != conf.end(); ++it) {
		if(public_detail_->ans_qid.find(it->first) == public_detail_->ans_qid.end()) {
			msg.qid = it->first;
			break;
		}
	}
	//if(msg.qid) {
		THIS_SEND_TO_CLIENT(msg);
	//}
	return 0;
}

int Public::req_submit_questionnaire(uint8_t qid, std::vector<Questionnaire_Answer_Info> &ans) {
	const Questionnaire_Reward_Config *conf = CONFIG_CACHE_PUBLIC->get_questionnaire_reward_config_byqid(qid);
	if(!conf) { return 0;}
	if(public_detail_->ans_qid.find(qid) != public_detail_->ans_qid.end()) {
		return ERROR_QUESTIONNAIRE_RE_SUBMIT;
	}

	//发放奖励
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if (conf->reward_item.size()) {
		Item_Vec item;
		for (UInt_UInt_Map::const_iterator item_it = conf->reward_item.begin();
				item_it != conf->reward_item.end(); ++item_it) {
			Item_Detail it_em(item_it->first, item_it->second,
					Item_Detail::UNDEFINED);
			item.push_back(it_em);
			Item_Basic_Info ibi;
			it_em.set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
		}
		if (item.size()) {
			int result = player_self()->pack_insert_item(
					Pack::PACK_T_PACKAGE_INDEX, item,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_QUESTIONNAIRE),
					TIPS_TYPE(GET_ITEM), Pack_Try(WITH_TRY),
					FIRST_RECHARGE_MAIL_ID);
			if (result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if (conf->reward_money.size()) {
		Money_Add_List money;
		money.clear();
		for (UInt_UInt_Map::const_iterator money_it = conf->reward_money.begin();
				money_it != conf->reward_money.end(); ++money_it) {
			money.push_back(
					Money_Add_Info(Money_Type(money_it->first),
							money_it->second,
							Money_DM_Info(MONEY_ADD_QUESTIONNAIRE)));
			acv_msg.property.push_back(
					Property(money_it->first, money_it->second));
		}
		if (money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if (conf->exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(conf->exp, false, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	public_detail_->ans_qid.insert(qid);
	public_detail_->detail_change();
	//异步存库
	Block_Buffer buf;
	buf.make_message(MONGO_SAVE_QUESTIONNAIRE_INFO);
	buf.write_int64(player_self()->role_id());
	buf.write_uint16(ans.size());
	for(std::vector<Questionnaire_Answer_Info>::iterator it = ans.begin(); it != ans.end(); ++it) {
		it->serialize(buf);
	}
	buf.finish_message();
	DB_MANAGER->push_data_block(buf, 0);
	req_questionnaire_info();
	return 0;
}


int Public::req_open_invite_reward_info(void) {
	MSG_50120060 msg;
	if(public_detail_->inv_time != Time_Value::zero) {
		if(!public_detail_->is_receive_inv_ones) {
			msg.ones = INVITE_REWARD_TYPE_CAN_RECEIVE;
		} else {
			msg.ones = INVITE_REWARD_TYPE_HAS_RECEIVE;
		}
	} else {
		msg.ones = INVITE_REWARD_TYPE_CAN_NOT_RECEIVE;
	}
	if(get_time_subdays(public_detail_->rec_inv_time, Time_Value::gettimeofday()) != 0) {
		if(public_detail_->rec_inv_time.sec() < public_detail_->inv_time.sec()) {
			msg.daily = INVITE_REWARD_TYPE_CAN_RECEIVE;
		} else {
			msg.daily = INVITE_REWARD_TYPE_CAN_NOT_RECEIVE;
		}
	} else {
		msg.daily = INVITE_REWARD_TYPE_HAS_RECEIVE;
	}
	msg.like = public_detail_->is_receive_like_reward?0:1;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Public::req_receive_invite_reward_info(uint8_t type) {
//	if(type) {//daily
		if(get_time_subdays(public_detail_->rec_inv_time, Time_Value::gettimeofday()) != 0) {
			if(public_detail_->rec_inv_time.sec() < public_detail_->inv_time.sec()) {
//				const Invite_Reward_Config &conf = CONFIG_CACHE_PUBLIC->get_invite_daily_reward_config();
//				int res = receive_invite_reward(conf);
//				if(res) {
//					return res;
//				}
				LOGIC_PUBLIC->send_mail(2004, player_self()->role_id());//每日奖励
				public_detail_->rec_inv_time = Time_Value::gettimeofday();
				public_detail_->detail_change();
				MSG_STREAM("`stream_fb_click`(`player_id`, `level`, `ip`, `type`, `time`)"" VALUES (%ld,%d,'%s',%d, %ld)",
						player_self()->role_id(),
						player_self()->base_detail().level,
						player_self()->base_detail().ip.c_str(),
						7,//7是成功领取daily friends的invite的奖励
						Time_Value::gettimeofday().sec());
			} //else {
//				return ERROR_REWARD_CAN_NOT_REWARD;
//			}
		}// else {
//			return ERROR_REWARD_ALREADY_RECEIVE;
//		}
//	} else {//ones
		if(public_detail_->inv_time != Time_Value::zero) {
			if(!public_detail_->is_receive_inv_ones) {
//				const Invite_Reward_Config &conf = CONFIG_CACHE_PUBLIC->get_invite_one_reward_config();
//				int res = receive_invite_reward(conf);
//				if(res) {
//					return res;
//				}
				LOGIC_PUBLIC->send_mail(2003, player_self()->role_id());//一次性奖励
				public_detail_->is_receive_inv_ones = true;
				public_detail_->detail_change();
				MSG_STREAM("`stream_fb_click`(`player_id`, `level`, `ip`, `type`, `time`)"" VALUES (%ld,%d,'%s',%d, %ld)",
						player_self()->role_id(),
						player_self()->base_detail().level,
						player_self()->base_detail().ip.c_str(),
						6,//6是成功领取first friend的invite的奖励
						Time_Value::gettimeofday().sec());
			}// else {
//				return ERROR_REWARD_ALREADY_RECEIVE;
//			}
		} //else {
//			return ERROR_REWARD_CAN_NOT_REWARD;
//		}
// }
//	MSG_50120061 msg;
//	msg.type = type;
//	THIS_SEND_TO_CLIENT(msg);
	req_open_invite_reward_info();
	return 0;
}

int Public::receive_invite_reward(const Invite_Reward_Config &conf) {
	//发放奖励
	MSG_81000102 acv_msg;
	acv_msg.type = 0;
	if (conf.reward_item.size()) {
		Item_Vec item;
		for (Invite_Item_Config_Vec::const_iterator item_it = conf.reward_item.begin();
				item_it != conf.reward_item.end(); ++item_it) {
			Item_Detail it_em(item_it->item_id, item_it->num,item_it->bind);
			item.push_back(it_em);
			Item_Basic_Info ibi;
			it_em.set_item_basic_info(ibi);
			acv_msg.item_info.push_back(ibi);
		}
		if (item.size()) {
			int result = player_self()->pack_insert_item(
					Pack::PACK_T_PACKAGE_INDEX, item,
					Gain_Item_DM_Info(Pack::ITEM_GAIN_INVITE_REWARD),
					TIPS_TYPE(GET_ITEM), Pack_Try(WITH_TRY),
					FIRST_RECHARGE_MAIL_ID);
			if (result && ERROR_PACK_FULL_BUT_MAIL != result) {
				return result;
			}
		}
	}
	if (conf.reward_money.size()) {
		Money_Add_List money;
		money.clear();
		for (Invite_Item_Config_Vec::const_iterator money_it = conf.reward_money.begin();
				money_it != conf.reward_money.end(); ++money_it) {
			money.push_back(
					Money_Add_Info(Money_Type(money_it->item_id),
							money_it->num,
							Money_DM_Info(MONEY_ADD_INVITE_REWARD)));
			acv_msg.property.push_back(
					Property(money_it->item_id, money_it->num));
		}
		if (money.size()) {
			player_self()->pack_add_money(money);
		}
	}
	if (conf.exp) {
		Exp_All exp_all = player_self()->modify_experience_inter(conf.exp, false, false, false, true);
		if (0 == exp_all.result) {
			acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
			acv_msg.vip_exp = exp_all.vip_exp;
			acv_msg.world_level = player_self()->get_world_level_add();
		}
	}
	if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}

void Public::set_inv_time(const Time_Value &time) {
	public_detail_->inv_time = time;
	public_detail_->detail_change();
	req_receive_invite_reward_info(0);
}

void Public::get_like_reward(void) {
	if(!public_detail_->is_receive_like_reward) {
		public_detail_->is_receive_like_reward = true;
		LOGIC_PUBLIC->send_mail(2001, player_self()->role_id());
		public_detail_->detail_change();
		req_open_invite_reward_info();
		MSG_STREAM("`stream_fb_click`(`player_id`, `level`, `ip`, `type`, `time`)"" VALUES (%ld,%d,'%s',%d, %ld)",
				player_self()->role_id(),
				player_self()->base_detail().level,
				player_self()->base_detail().ip.c_str(),
				5,//5是成功领取like的奖励
				Time_Value::gettimeofday().sec());
	}
}

void Public::public_listen_level_up(int32_t level) {
		//充值返利图标
		//if(CONFIG_CACHE->server_maintainer_cache().game_ver_type == 0) {////判断国内(0)还是国外
			uint32_t level_limit = RECHARGE_REBATE_MANAGER->get_icon_limit_level();
			Icon_Time_Info &icon_time = RECHARGE_REBATE_MANAGER->get_icon_time_status();
			Time_Value now = Time_Value::gettimeofday();
			if((uint32_t)player_self()->level() >= level_limit &&
					now.sec() >= RECHARGE_REBATE_MANAGER->get_start_time().sec() &&
					now.sec() < RECHARGE_REBATE_MANAGER->get_end_time().sec()) {
				MSG_81000408 msg;
				msg.open = 1;
				msg.icon_id = icon_time.icon_id;
				THIS_SEND_TO_CLIENT(msg);
			}
		//}
		if(level == 10){//创号发送欢迎邮件
			LOGIC_PUBLIC->send_mail(2005, player_self()->role_id());
		}
}

void Public::player_try_time_out(uint32_t type, const Time_Value &over_time /*= Time_Value::zero*/) {
	switch(type) {
	case TRY_VIP_TIMEOUT:
		if(public_detail_->vip_over_time != over_time) {
			public_detail_->vip_over_time.set(over_time);
			public_detail_->is_vip_readed = false;

			MSG_81110019 msg;
			msg.type = 1;
			THIS_SEND_TO_CLIENT(msg);
		}
		break;

	case TRY_MAGIC_WING_TIMEOUT:
		if(!public_detail_->is_magic_wing_over_time && !public_detail_->is_magic_wing_readed) {
			public_detail_->is_magic_wing_over_time = true;
			public_detail_->is_magic_wing_readed = false;

			MSG_81110019 msg;
			msg.type = 2;
			THIS_SEND_TO_CLIENT(msg);
		}
		break;

	case TRY_GENIUS_TIMEOUT:
		if(public_detail_->genius_over_time != over_time) {
			public_detail_->genius_over_time.set(over_time);
			public_detail_->is_genius_readed = false;

			MSG_81110019 msg;
			msg.type = 3;
			THIS_SEND_TO_CLIENT(msg);
		}
		break;
	}

	public_detail_->detail_change();
}

void Public::notify_try_time_out_list() {
	MSG_81110020 msg;
	if(public_detail_->vip_over_time != Time_Value::zero && !public_detail_->is_vip_readed) {
		msg.type_list.push_back((uint8_t)1);
	}

	if(public_detail_->is_magic_wing_over_time && !public_detail_->is_magic_wing_readed) {
		msg.type_list.push_back((uint8_t)2);
	}

	if(public_detail_->genius_over_time != Time_Value::zero && !public_detail_->is_genius_readed) {
		msg.type_list.push_back((uint32_t)3);
	}

	if(!msg.type_list.empty()) {
		THIS_SEND_TO_CLIENT(msg);
	}
}

int Public::req_player_readed_timeout_try_message(const uint8_t type) {
	if(type != TRY_VIP_TIMEOUT &&  type != TRY_MAGIC_WING_TIMEOUT && type != TRY_GENIUS_TIMEOUT) {
		return ERROR_CLIENT_PARAM;
	}

	switch(type) {
		case TRY_VIP_TIMEOUT:
			if(public_detail_->vip_over_time != Time_Value::zero) {
				public_detail_->is_vip_readed = true;
			}
			break;

		case TRY_MAGIC_WING_TIMEOUT:
			if(public_detail_->is_magic_wing_over_time) {
				public_detail_->is_magic_wing_readed = true;
			}
			break;

		case TRY_GENIUS_TIMEOUT:
			if(public_detail_->genius_over_time != Time_Value::zero) {
				public_detail_->is_genius_readed = true;
			}
			break;
	}

	public_detail_->detail_change();

	return 0;
}

void Public::push_add_today_get_bind_gold(uint32_t num) {
	public_detail_->today_rec_bind_gold += num;
	public_detail_->detail_change();
	send_today_get_bind_gold_active();
}
void Public::send_today_get_bind_gold_active(void) {
	MSG_82200204 msg;
	msg.num = public_detail_->today_rec_bind_gold;
	THIS_SEND_TO_CLIENT(msg);
}
int Public::friendship_accumulate(void) {
	return public_detail_->friendship_accumulate;
}
int Public::add_friendship_accumulate(const int friendship) {
	if (friendship > 0) {
		public_detail_->friendship_accumulate += friendship;
		public_detail_->detail_change();
	}
	return public_detail_->friendship_accumulate;
}

int Public::get_vip_upgrade_card(void) const {

	return public_detail_->vip_upgrade_cards_;
}

void Public::set_vip_upgrade_card(int times) {

	public_detail_->vip_upgrade_cards_ = times;
}

int Public::get_arena_worship(void) {

	return public_detail_->arena_worship_times;
}

void Public::set_arena_worship(const int value) {
	public_detail_->arena_worship_times = value;
	public_detail_->detail_change();
}
