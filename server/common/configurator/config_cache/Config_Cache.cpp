/*
 * Config_Cache.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 */

#include "Config_Cache.h"
#include "Configurator.h"
#include "Utility_Func.h"
#include "Json_Utility.h"
#include "Record_Client.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Buff.h"
#include "Config_Cache_Dungeon.h"
#include "Config_Cache_Hero.h"
#include "Config_Cache_Item.h"
#include "Config_Cache_NPC.h"
#include "Config_Cache_Package.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Task.h"
#include "Config_Cache_Team.h"
#include "Config_Cache_Gang.h"
#include "Config_Cache_Mail.h"
#include "Config_Cache_Robot.h"
#include "Config_Cache_Equip.h"
#include "Config_Cache_Rune.h"
#include "Config_Cache_Rune_Stone.h"
#include "Config_Cache_Alchemy.h"
#include "Config_Cache_Icon.h"
#include "Config_Cache_Ranking.h"
#include "Config_Cache_Arena.h"
#include "Config_Cache_Public.h"
#include "Config_Cache_Friend.h"
#include "Config_Cache_Daily.h"
#include "Config_Cache_Exchange.h"
#include "Config_Cache_Reward_Task.h"
#include "Config_Cache_Active.h"
#include "Config_Cache_Upgrade.h"
#include "Config_Cache_Dragon_Vale.h"
#include "Config_Cache_Covenant.h"
#include "Config_Cache_Achievement.h"
#include "Config_Cache_Title.h"
#include "Config_Cache_Vip.h"
#include "Config_Cache_Mount.h"
#include "Config_Cache_Valentine_Monster.h"
#include "Config_Cache_World_Boss.h"
#include "Config_Cache_Honor_Arena.h"
#include "Config_Cache_Flowers.h"
#include "Config_Cache_Secret_Shop.h"
#include "Config_Cache_Reward.h"
#include "Config_Cache_Active_Content.h"
#include "Config_Cache_Recharge.h"
#include "Config_Cache_Auction.h"
#include "Config_Cache_Fashion.h"
#include "Config_Cache_Knight_Trials.h"
#include "Config_Cache_Anci_Artifact.h"
#include "Config_Cache_Wing.h"
#include "Config_Cache_Invest_Plan.h"
#include "Config_Cache_Expedition.h"
#include "Config_Cache_ThorLottery.h"
#include "Config_Cache_Answer.h"
#include "Daemon_Server.h"
#include "Config_Cache_Card.h"
#include "Config_Cache_Chat.h"
#include "Config_Cache_Activity.h"
#include "Config_Cache_Mall.h"
#include "Config_Cache_LFSkill.h"
#include "Config_Cache_Oper_Activity.h"
#include "Config_Cache_Pet.h"
#include "Server_Struct.h"

const int PLAYER_PROPS[] = {PT_POWER, PT_AGILITY, PT_DEXTERITY, PT_ARMOR, PT_STAMINA,
			PT_PHY_ATTACK, PT_SPELL_ATTACK, PT_PHY_DEFENSE, PT_SPELL_DEFENSE, PT_HIT,
			PT_AVOID, PT_BLOOD_MAX, PT_CRIT, PT_CRIT_DEF, PT_SKILL_POINT,
			PT_BLOOD_POOL_MAX, PT_BLOOD_POOL_CURRENT
};

const int PLAYER_INIT_PROPS[] = {
		PT_MORALE_CAPS, PT_STRENGTH_MAX, PT_CRIT_HURT, PT_CURE_RATE, PT_BE_CURE_RATE, PT_STATUS_HIT_RATE, PT_STATUS_DEF_RATE, PT_SING_RATE, PT_SPEED
};

const int CONVERSION_PROPS[] = {
			PT_PHY_ATTACK, PT_SPELL_ATTACK, PT_HIT, PT_CRIT, PT_AVOID, PT_CRIT_DEF, PT_PHY_DEFENSE, PT_SPELL_DEFENSE, PT_BLOOD_MAX
};


Config_Cache::Config_Cache(void) : server_flag_(0) {
}

Config_Cache::~Config_Cache(void) {}

void Config_Cache::check_all_config_cache(void) {
	// 检测怪物战斗位置
	CONFIG_CACHE_NPC->check_monster_position();
	// 坐标点有效
	CONFIG_CACHE_NPC->check_monster_coord();

	check_cache_solution();

	//检查任务配置
	CONFIG_CACHE_TASK->check_task_cfg();
}

void Config_Cache::check_cache_solution(void) {
	{
		// 跨服不需要检查(调试版也不能检查了)
		const Json::Value &server_solution = CONFIG_INSTANCE->open_solution();
		int inter_solution_id = server_solution["inter_solution_id"].asInt();
		if (inter_solution_id) {
			// 场景不在开服方案
			const Server_Solution_Cache &server_solution_cache = CONFIG_CACHE->server_solution_cache();
			Monitor_Mark_Vec muid;
			Scene_Unique_ID scene_uid;
			Hash_Map<Scene_Unique_ID, Monitor_Unique_ID, NULL_MUTEX> scene_muid_map_;
			server_solution_cache.fetch_muid_by_type(muid, Server_Enum::MONITOR_SCENE);
			for (Monitor_Mark_Vec::iterator it = muid.begin(); it != muid.end(); ++it) {
				scene_uid.scene_id = (*it).id;
				scene_uid.scene_order = (*it).order;
				if (scene_muid_map_.bind(scene_uid, (*it))) {
					LOG_ABORT("scene repeat, scene_id:%d, scene_order:%d", (*it).id, (*it).order);
				}
			}

			const Json::Value json_scene = CONFIG_INSTANCE->config_json()["scene"]["scene_config"];
			for (Json::Value::const_iterator it = json_scene.begin(); it != json_scene.end(); it++) {
				int scene_id = (*it)["sceneId"].asInt();
				Monitor_Unique_ID muid;
				Scene_Unique_ID suid(scene_id, 0);
				if (scene_muid_map_.find(suid, muid)) {
					LOG_ABORT("场景:%d不在开服方案", scene_id);
				}
			}

			Hash_Map<Scene_Unique_ID, Monitor_Unique_ID, NULL_MUTEX>::iterator begin = scene_muid_map_.begin();
			Hash_Map<Scene_Unique_ID, Monitor_Unique_ID, NULL_MUTEX>::iterator end = scene_muid_map_.end();
			for (Hash_Map<Scene_Unique_ID, Monitor_Unique_ID, NULL_MUTEX>::iterator it = begin; it != end; ++it) {
				const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(it->first.scene_id);
				if (!scene_config) {
					LOG_ABORT("方案场景:%d找不到配制", it->first.scene_id);
				}

				int map_id = scene_config->map_id ? scene_config->map_id : scene_config->scene_id;
				if (!CONFIG_CACHE_SCENE->scene_map_data_cache(map_id)) {
					LOG_ABORT("场景:%d地图%d不存在", scene_config->scene_id, map_id);
				}
			}
		}
	}

}

void Config_Cache::set_cross_info(int flag) {
	server_maintainer_cache_.game_inter_flag = flag;
}

void Config_Cache::refresh_all_config_cache(std::string module) {
	// 对耗时长的作特殊处理
	PERF_MON("Config_Cache");

	if (module == "") {
		refresh_server_id_ip_cache();
		refresh_server_list_cache();
	}

	if (module == "" || module == "mysql") {
		refresh_mysql_init_config();
		if (module != "")return ;
	}

	if (module == "" || module == "map_data") {
		CONFIG_CACHE_SCENE->refresh_scene_map_cache(module);
		if (module != "")return ;
	}

	if (module == "" || module == "server" || module == "scene") {
		refresh_server_config_cache();
		refresh_server_solution_cache();
		refresh_monitor_link_cache();
		CONFIG_CACHE_SCENE->refresh_scene_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "player" || module == "fighter") {
		CONFIG_CACHE_ROLE->refresh_config_cache();
		CONFIG_CACHE_BUFF->refresh_config_cache();
		CONFIG_CACHE_FIGHTER->refresh_config_cache();

		if (module == "" && DAEMON_SERVER_INSTANCE->has_in_server_list(Daemon_Server::MONITOR_LOGIC)) {
			CONFIG_CACHE_ROLE->refresh_player_name_depot();
		}
		if (module != "")return ;
	}

	if (module == "" || module == "hero") {
		CONFIG_CACHE_HERO->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "item") {
		CONFIG_CACHE_ITEM->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "npc") {
		CONFIG_CACHE_NPC->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "package") {
		CONFIG_CACHE_PACKAGE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "reward_task") {
		CONFIG_CACHE_REWARD_TASK->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "safe_guide") {
		CONFIG_CACHE_TASK->refresh_safe_guide_config();
		CONFIG_CACHE_PUBLIC->refresh_guide_finish_cfg();
		if (module != "")return ;
	}

	//task must after reward_task
	if (module == "" || module == "task") {
		CONFIG_CACHE_TASK->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "team") {
		CONFIG_CACHE_TEAM->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "guild") {
		CONFIG_CACHE_GANG->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "mail") {
		CONFIG_CACHE_MAIL->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "robot") {
		CONFIG_CACHE_ROBOT->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "equip") {
		CONFIG_CACHE_EQUIP->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "rune") {
		CONFIG_CACHE_RUNE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "rune") {
		CONFIG_CACHE_RUNE_STONE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "alchemy") {
		CONFIG_CACHE_ALCHEMY->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "answer") {
		CONFIG_CACHE_ANSWER->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "chat") {
		CONFIG_CACHE_CHAT->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "icon") {
		CONFIG_CACHE_ICON->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "rank") {
		CONFIG_CACHE_RANKING->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "arena") {
		CONFIG_CACHE_ARENA->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "guide" || module == "public") {
		CONFIG_CACHE_PUBLIC->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "questionnaire") {
		CONFIG_CACHE_PUBLIC->refresh_questionnaire_conf_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "syscfg") {
		CONFIG_CACHE_PUBLIC->refresh_sys_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "forenotice") {
		CONFIG_CACHE_PUBLIC->refresh_notice_cfg_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "challenge") {
		CONFIG_CACHE_PUBLIC->refresh_challenge_cfg_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "friend") {
		CONFIG_CACHE_FRIEND->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "dungeon") {
		CONFIG_CACHE_DUNGEON->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "helper") {
		CONFIG_CACHE_DAILY->refresh_config_cache();
		CONFIG_CACHE_ACTIVE->refresh_config_cache();
		CONFIG_CACHE_UPGRADE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "exchange") {
		CONFIG_CACHE_EXCHANGE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "activity") {
		CONFIG_CACHE_ACTIVITY->refresh_config_cache();
		if (module != "")return ;
	}

	//dragon must after icon
	if (module == "" || module == "dragon") {
		CONFIG_CACHE_DRAGON_VALE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "covenant") {
		CONFIG_CACHE_COVENANT->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "achievement") {
		CONFIG_CACHE_ACHIEVEMENT->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "title") {
		CONFIG_CACHE_TITLE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "vip") {
		CONFIG_CACHE_VIP->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "horse") {
		CONFIG_CACHE_MOUNT->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "mall") {
		CONFIG_CACHE_MALL->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "flowers") {
		CONFIG_CACHE_FLOWERS->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "secret_shop") {
		CONFIG_CACHE_SECRET_SHOP->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "secret_shop") {
		CONFIG_CACHE_REWARD->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "valentine_monster") {
		CONFIG_CACHE_VALENTINE_MONSTER->refresh_config_cache();
		if (module != "") return;
	}

	if (module == "" || module == "world_boss") {
		CONFIG_CACHE_WORLD_BOSS->refresh_config_cache();
		if (module != "") return;
	}

	if (module == "" || module == "honor_arena") {
		CONFIG_CACHE_HONOR_ARENA->refresh_config_cache();
		if (module != "") return;
	}

	if (module == "" || module == "active_content") {
		CONFIG_CACHE_ACTIVE_CONTENT->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "recharge") {
		CONFIG_CACHE_RECHARGE->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "auction") {
		CONFIG_CACHE_AUCTION->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "fashion") {
		CONFIG_CACHE_FASHION->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "knight_trials") {
		CONFIG_CACHE_KNIGHT_TRIALS->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "anci_artifact") {
		CONFIG_CACHE_ANCI_ARTIFACT->refresh_config_cache();
		if (module != "")return ;
	}

	if (module == "" || module == "devilWing") {
		CONFIG_CACHE_WING->refresh_config_cache();
		if (module != "") return;
	}

	if (module == "" || module == "pet") {
		CONFIG_CACHE_PET->refresh_config_cache();
	}

	if(module == "" || module == "invest_plan") {
		CONFIG_CACHE_INVEST_PLAN->refresh_config_cache();
		if(module != "") return;
	}

	if(module == "" || module == "expedition") {
		CONFIG_CACHE_EXPEDITION->refresh_config_cache();
		if(module != "") return;
	}
	if(module == "" || module == "thorDraw") {
		CONFIG_CACHE_THORLOTTERY->refresh_config_cache();
		if (module != "") return;
	}
	if (module == "" || module == "card") {
		CONFIG_CACHE_CARD->refresh_card_config_cache();
		if (module != "") return;
	}
	if (module == "" || module == "life_skill") {
		CONFIG_CACHE_LFSKILL->refresh_config_cache();
		if (module != "") return;
	}

	if (module == "" || module == "oper_activity") {
		CONFIG_CACHE_OPER_ACTIVITY->refresh_config_cache();
		if (module != "") return;
	}

	// 更新单人副本层次关系
	CONFIG_CACHE_SCENE->update_single_dungeon_relation();
}

void Config_Cache::refresh_server_config_cache(void) {
	const Json::Value &server_conf = CONFIG_INSTANCE->config_json()["server"]["server_conf"];
	for (Json::Value::iterator it = server_conf["command"].begin(); it != server_conf["command"].end(); ++it) {
		server_maintainer_cache_.command.insert((*it).asString());
	}

	server_maintainer_cache_.default_solution_id = server_conf["default_solution_id"].asInt();
	server_maintainer_cache_.center_url = server_conf["center_url"].asString();

	const Json::Value &mongo_operator = CONFIG_INSTANCE->config_json()["server"]["mongo_repair"];
	for (Json::Value::iterator it = mongo_operator["command"].begin(); it != mongo_operator["command"].end(); ++it) {
		db_command_.insert((*it).asString());
	}

}

void Config_Cache::refresh_maintainer_cache(void) {
	const Json::Value &server_identity = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_identity"];
	server_maintainer_cache_.server_identity = server_identity["server_identity"].asInt();
	ABORT_INT_ZERO(server_maintainer_cache_.server_identity);

	const Json::Value &server_maintainer = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_maintainer"];
	if (server_maintainer.isMember("record_log_level")) {
		std::string skill_extra_bit = server_maintainer["record_log_level"].asCString();
		Json_Utility::assign_int_by_string(skill_extra_bit, server_maintainer_cache_.record_log_level);
	}

	if (server_maintainer.isMember("package_frequency")) {
		server_maintainer_cache_.package_frequency = server_maintainer["package_frequency"].asUInt();
	}

	if (server_maintainer.isMember("local_log_level")) {
		std::string skill_extra_bit = server_maintainer["local_log_level"].asCString();
		Json_Utility::assign_int_by_string(skill_extra_bit, server_maintainer_cache_.local_log_level);
	}

	if (server_maintainer.isMember("game_ver_type")) {
		server_maintainer_cache_.game_ver_type = server_maintainer["game_ver_type"].asInt();
	}

	if (server_maintainer_cache_.game_open_time == Time_Value::zero) {
		server_maintainer_cache_.game_open_time.sec(server_maintainer["game_open_time"].asInt());
	}

	if (server_maintainer_cache_.game_merge_time == Time_Value::zero) {
		server_maintainer_cache_.game_merge_time.sec(server_maintainer["game_merge_time"].asInt());
	}

	server_maintainer_cache_.game_server_flag = server_maintainer["game_server_flag"].asString();
	server_maintainer_cache_.platform_des = server_maintainer["platform_des"].asString();

	server_maintainer_cache_.game_port_prefix = server_maintainer["game_port_prefix"].asInt();
	server_maintainer_cache_.game_solution_id = server_maintainer["game_solution_id"].asInt();
	server_maintainer_cache_.server_close_delay = server_maintainer["server_close_delay"].asInt();

	server_maintainer_cache_.game_inter_flag = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_inter"]["inter_area"].asInt();

	server_maintainer_cache_.db_work_num = server_maintainer["db_work_num"].asInt();
	server_maintainer_cache_.record_output_type = server_maintainer["record_output_type"].asInt();
	server_maintainer_cache_.record_id = server_maintainer["record_id"].asInt();
	server_maintainer_cache_.md5_key = server_maintainer["md5_key"].asString();
	server_maintainer_cache_.verify_msg_time = server_maintainer["verify_msg_time"].asBool();
	server_maintainer_cache_.config_not_update = server_maintainer["config_not_update"].asBool();
	server_maintainer_cache_.valgrind_allow = server_maintainer["valgrind_allow"].asBool();

	if (server_maintainer.isMember("inner_tick_time"))
		Json_Utility::assign_time_value(server_maintainer["inner_tick_time"], server_maintainer_cache_.inner_tick_time_);
	if (server_maintainer.isMember("inner_time_out"))
		Json_Utility::assign_time_value(server_maintainer["inner_time_out"], server_maintainer_cache_.inner_time_out_);
	if (server_maintainer_cache_.inner_tick_time_ == Time_Value::zero)
		LOG_ABORT("error inner_tick_time_");
	if (server_maintainer_cache_.inner_time_out_ == Time_Value::zero)
		LOG_ABORT("error inner_time_out");

	const Json::Value &server_miscellaneous = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_miscellaneous"];
	if (server_miscellaneous.isMember("command")) {
		server_maintainer_cache_.command_mode = server_miscellaneous["command"]["mode"].asInt();
	}

	server_maintainer_cache_.chat_monitor_url = server_miscellaneous["chat_monitor_url"].asString();
	server_maintainer_cache_.chat_monitor_key = server_miscellaneous["chat_monitor_key"].asString();
	server_maintainer_cache_.pt_server_id = server_miscellaneous["pt_server_id"].asString();

	server_maintainer_cache_.mongo_ip = server_maintainer["mongodb_server"]["ip"].asString();
	server_maintainer_cache_.mongo_port = server_maintainer["mongodb_server"]["port"].asInt();

	server_maintainer_cache_.mysql_ip = server_maintainer["mysql_server"]["ip"].asString();
	server_maintainer_cache_.mysql_port = server_maintainer["mysql_server"]["port"].asInt();
	server_maintainer_cache_.mysql_user = server_maintainer["mysql_server"]["user"].asString();
	server_maintainer_cache_.mysql_pw = server_maintainer["mysql_server"]["passwd"].asString();
	server_maintainer_cache_.mysql_dbname = server_maintainer["mysql_server"]["dbname"].asString();

	ABORT_STRING_NULL(server_maintainer_cache_.game_server_flag);
	ABORT_INT_ZERO(server_maintainer_cache_.game_port_prefix);
	ABORT_INT_ZERO(server_maintainer_cache_.game_solution_id);

}

void Config_Cache::refresh_server_solution_cache(void) {
	if (!server_maintainer_cache_.game_port_prefix) {
		LOG_ABORT("error port prefix");
	}
	server_solution_cache_.reset();

	const Json::Value &server_solution = CONFIG_INSTANCE->open_solution();
	server_solution_cache_.solution_id = server_solution["solution_id"].asInt();

	int port_prefix = server_maintainer_cache_.game_port_prefix * 1000;

	int index = 0;
	Hash_Map<Monitor_Unique_ID, int, NULL_MUTEX> repeat_muid_check;
	for (Json::Value::const_iterator monitor_it = server_solution["monitor_list"].begin(); monitor_it != server_solution["monitor_list"].end(); ++monitor_it) {
		Monitor_Info server_monitor;

		server_monitor.name = (*monitor_it)["name"].asString();
		server_monitor.command = (*monitor_it)["command"].asString();
		server_monitor.run_on = (*monitor_it)["run_on"].asInt();
		server_monitor.index = index++;
		server_monitor.check = (*monitor_it)["check"].asString();

		Int_Set m_set;
		for (Json::Value::iterator acceptor_it = (*monitor_it)["acceptor"].begin(); acceptor_it != (*monitor_it)["acceptor"].end(); ++acceptor_it) {
			Monitor_Acceptor acceptor;

			acceptor.monitor_type = (Server_Enum::MONITOR_TYPE)(*acceptor_it)["monitor_type"].asInt();
			if (m_set.count(acceptor.monitor_type)) {
				LOG_ABORT("error repeat config monitor type:%d", acceptor.monitor_type);
			}
			m_set.insert(acceptor.monitor_type);

			acceptor.client_port_suffix = (*acceptor_it)["client_port_suffix"].asInt();
			acceptor.client_port = (*acceptor_it)["ingore_prefix"].asBool() ? acceptor.client_port_suffix : port_prefix + acceptor.client_port_suffix;

			acceptor.inner_port_suffix = (*acceptor_it)["inner_port_suffix"].asInt();
			acceptor.inner_port = (*acceptor_it)["ingore_prefix"].asBool() ? acceptor.inner_port_suffix : port_prefix + acceptor.inner_port_suffix;

			if ((*acceptor_it)["client_recv_timeout"] != Json::Value::null) {
				// 内网客户端要调试，特殊处理
				if (CONFIG_CACHE->server_maintainer_cache().md5_key.empty())
					acceptor.client_recv_timeout = Time_Value(900);
				else
					Json_Utility::assign_time_value((*acceptor_it)["client_recv_timeout"], acceptor.client_recv_timeout);
			}

			if ((*acceptor_it)["inner_recv_timeout"] != Json::Value::null)
				Json_Utility::assign_time_value((*acceptor_it)["inner_recv_timeout"], acceptor.inner_recv_timeout);

			Monitor_Unique_ID monitor_index;
			monitor_index.type = acceptor.monitor_type;
			monitor_index.uuid = CONFIG_CACHE->server_flag();
			monitor_index.port = acceptor.inner_port;
			Server_Id_IP_Map::iterator ip_find_it = server_ip_ip_cache_.find(server_monitor.run_on);
			if (ip_find_it != server_ip_ip_cache_.end())
				monitor_index.ip = ip_find_it->second.telecom_ip;
			else
				LOG_ABORT("开服run_on不存在:%d", server_monitor.run_on);
			if ((*acceptor_it)["list"] != Json::Value::null) {
				for (Json::Value::iterator list_it = (*acceptor_it)["list"].begin(); list_it != (*acceptor_it)["list"].end(); ++list_it) {
					if ((*list_it).isArray()) {
						monitor_index.id = (*list_it)[0u].asInt();
						if ((*list_it).size() == 2)
							monitor_index.order = (*list_it)[1u].asInt();
						if ((*list_it).size() > 2)
							LOG_ABORT("open solution error, id:%d", monitor_index.id);
					} else {
						monitor_index.id = (*list_it)["id"].asInt();
						monitor_index.order = (*list_it)["order"].asInt();
					}
					if (repeat_muid_check.exist(monitor_index))
						LOG_ABORT("repeat monitor, id:%d", monitor_index.id);
					repeat_muid_check.bind(monitor_index, 1);
					acceptor.monitor_id_order_vec_.push_back(monitor_index);
					if (server_monitor.run_on > 8000) {
						server_solution_cache_.inter_server.insert(monitor_index.id);
					}
				}
			} else {
				acceptor.monitor_id_order_vec_.push_back(monitor_index);
			}

			server_monitor.acceptor_list.push_back(acceptor);
		}
		server_solution_cache_.monitor_list.push_back(server_monitor);
	}
}

void Config_Cache::refresh_merge_cache(void) {

	{
		const Json::Value& server_list = CONFIG_INSTANCE->config_json()["server_list"];
		if (server_list == Json::Value::null || server_list["list"] == Json::Value::null || server_list["list"].size() < 2) {
			std::cout << "config/server_list.json 不存在或格式错误，至少2个服! exit " << std::endl;
			exit(0);
		}

		for (uint i = 0; i < server_list["list"].size(); ++i) {
			const Json::Value& dest_json = server_list["list"][i];
			if(!dest_json["ip"] || !dest_json["port"]) {
				std::cout << "config/server_list.json error, ip or port error, exit " << std::endl;
				exit(0);
			}
			Int_String ip_port;
			ip_port.str_value = dest_json["ip"].asString();
			ip_port.int_value = dest_json["port"].asUInt();
			merge_server_list_.push_back(ip_port);
		}

		for(uint i = 0; i< server_list["list"].size(); ++i) {
			const Json::Value& dest_json = server_list["list"][i];
			if(!dest_json["mysql_port"]
				  || !dest_json["mysql_user"]
				  || !dest_json["mysql_pw"]
			      || !dest_json["mysql_dbname"]) {
				std::cout << "config/server_list.json error, mysql port or username or password or database name error, exit " << std::endl;
				exit(0);
			}
			Mysql_Conf mysql_conf;
			mysql_conf.mysql_ip = dest_json["ip"].asString();
			mysql_conf.mysql_port = dest_json["mysql_port"].asInt();
			mysql_conf.mysql_user = dest_json["mysql_user"].asString();
			mysql_conf.mysql_pw = dest_json["mysql_pw"].asString();
			mysql_conf.mysql_dbname = dest_json["mysql_dbname"].asString();
			merge_mysql_list_.push_back(mysql_conf);
		}
	}

	const Json::Value& merge_config = CONFIG_INSTANCE->config_json()["merge_config"];
	if (merge_config == Json::Value::null || merge_config["filter"] == Json::Value::null || merge_config["filter"]["level_lt"] == Json::Value::null) {
		std::cout << "config/merge_config.json 不存在或格式错误! exit " << std::endl;
		exit(0);
	}

	if (merge_config == Json::Value::null || merge_config["filter"] == Json::Value::null || merge_config["filter"]["last_login"] == Json::Value::null) {
		std::cout << "config/merge_config.json 不存在或格式错误! exit " << std::endl;
		exit(0);
	}

	server_maintainer_cache_.merge_login_day_ = merge_config["filter"]["last_login"].asInt();
	server_maintainer_cache_.merge_level_lt_ = merge_config["filter"]["level_lt"].asInt();

	CONFIG_CACHE_MAIL->refresh_merge_config_cache();

}

void Config_Cache::refresh_monitor_link_cache(void) {
	const Json::Value &server_solution = CONFIG_INSTANCE->open_solution();

	int port_prefix = server_maintainer_cache_.game_port_prefix * 1000;

	for (Json::Value::const_iterator monitor_it = server_solution["monitor_list"].begin(); monitor_it != server_solution["monitor_list"].end(); ++monitor_it) {
		for (Json::Value::iterator acceptor_it = (*monitor_it)["acceptor"].begin(); acceptor_it != (*monitor_it)["acceptor"].end(); ++acceptor_it) {
			Monitor_Link_Info monitor_link;

			int run_on = (*monitor_it)["run_on"].asInt();
			Server_Id_IP_Map::iterator ip_find_it = server_ip_ip_cache_.find(run_on);
			if (ip_find_it != server_ip_ip_cache_.end()) {
				monitor_link.telecom_ip = ip_find_it->second.telecom_ip;
				monitor_link.unicom_ip = ip_find_it->second.unicom_ip;
			} else {
				LOG_ABORT("开服run_on:%d不存在", run_on);
			}

			Server_Enum::MONITOR_TYPE monitor_type = (Server_Enum::MONITOR_TYPE)(*acceptor_it)["monitor_type"].asInt();
			int client_port_suffix = (*acceptor_it)["client_port_suffix"].asInt();
			monitor_link.client_port = (*acceptor_it)["ingore_prefix"].asBool() ? client_port_suffix : port_prefix + client_port_suffix;

			int inner_port_suffix = (*acceptor_it)["inner_port_suffix"].asInt();
			monitor_link.inner_port = (*acceptor_it)["ingore_prefix"].asBool() ? inner_port_suffix : port_prefix + inner_port_suffix;

			if ((*acceptor_it)["list"] != Json::Value::null) {
				for (Json::Value::iterator list_it = (*acceptor_it)["list"].begin(); list_it != (*acceptor_it)["list"].end(); ++list_it) {
					Monitor_Unique_ID monitor_index;
					monitor_index.uuid = CONFIG_CACHE->server_flag();
					monitor_index.type = monitor_type;

					if ((*list_it).isArray()) {
						monitor_index.id = (*list_it)[0u].asInt();
						if ((*list_it).size() == 2)
							monitor_index.order = (*list_it)[1u].asInt();
						if ((*list_it).size() > 2)
							LOG_ABORT("open solution error, id:%d", monitor_index.id);
					} else {
						monitor_index.id = (*list_it)["id"].asInt();
						monitor_index.order = (*list_it)["order"].asInt();
					}

					set_map_second_value_by_key(monitor_index, monitor_link_cache_, monitor_link);
				}
			} else {
				Monitor_Unique_ID monitor_index(CONFIG_CACHE->server_flag(), monitor_type, 0, 0);
				set_map_second_value_by_key(monitor_index, monitor_link_cache_, monitor_link);
			}
		}
	}
}

void Config_Cache::refresh_server_list_cache(void) {
	server_list_map_.clear();
	const Json::Value &servers_config = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_list"];
	if (servers_config == Json::Value::null || servers_config["server_list"].size() == 0) {
		LOG_ABORT("");
	}

	int agent = 0;
	int server = 0;
	for (uint i = 0; i < servers_config["server_list"].size(); ++i) {
		agent = servers_config["server_list"][i]["agent_num"].asInt();
		server = servers_config["server_list"][i]["server_num"].asInt();
		if (agent < 100 || agent > 999 || server < 1000 || server > 9999) {
			LOG_ABORT("");
		}

		set_map_second_value_by_key(Server_Key(agent, server), server_list_map_, i);
	}

	if (server_list_map_.empty()) LOG_ABORT("");

	server_flag_ = (*server_list_map_.begin()).first.agent_num * 10000;
	server_flag_ += (*server_list_map_.begin()).first.server_num;
}

void Config_Cache::refresh_mysql_init_config(void) {
	mysql_init_sql_map_.clear();
	const Json::Value &mysql_init_config = CONFIG_INSTANCE->config_json()["mysql"]["init"];
	if (mysql_init_config == Json::Value::null || mysql_init_config.size() == 0) {
		LOG_ABORT("");
	}

	std::string table_name = "";
	std::string create_sql = "";
	for (uint i = 0; i < mysql_init_config.size(); ++i) {
		table_name = mysql_init_config[i]["table_name"].asString();
		create_sql = mysql_init_config[i]["create_sql"].asString();
		String_String_Map::iterator it = mysql_init_sql_map_.find("table_name");
		if(table_name != "" && create_sql != "" && it == mysql_init_sql_map_.end()){
			mysql_init_sql_map_.insert(std::make_pair(table_name, create_sql));
		}
	}
}

std::vector<Int_String> &Config_Cache::merge_server_list(void) {
	return merge_server_list_;
}

std::vector<Mysql_Conf> &Config_Cache::merge_mysql_list(void) {
	return merge_mysql_list_;
}

void Config_Cache::refresh_server_id_ip_cache(void) {
	const Json::Value &game_server_list = CONFIG_INSTANCE->config_json()["server_global_conf"]["server_maintainer"]["game_server_list"];
	int server_id = 0;
	IP_Info ip_info;
	for (Json::Value::const_iterator it = game_server_list.begin(); it != game_server_list.end(); ++it) {
		server_id = atoi(it.key().asCString());
		ip_info.telecom_ip = (*it)["telecom_ip"].asString();
		ip_info.unicom_ip = (*it)["unicom_ip"].asString();
		set_map_second_value_by_key(server_id, server_ip_ip_cache_, ip_info);
	}
}


int64_t Config_Cache::server_flag(void) {
	return server_flag_;
}

const Server_Maintainer_Cache &Config_Cache::server_maintainer_cache(void) const {
	return server_maintainer_cache_;
}

const Server_Solution_Cache &Config_Cache::server_solution_cache(void) const {
	return server_solution_cache_;
}

const Server_Id_IP_Map &Config_Cache::server_ip_cache() const {
	return server_ip_ip_cache_;
}

const String_String_Map &Config_Cache::mysql_init_config() const {
	return mysql_init_sql_map_;
}

const Monitor_Link_Map &Config_Cache::monitor_link_cache() const {
	return monitor_link_cache_;
}

const String_Set &Config_Cache::db_command() const {
	return db_command_;
}

const Server_List_Map &Config_Cache::server_list_map() const {
	return server_list_map_;
}
