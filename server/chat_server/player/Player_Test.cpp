
#include "Chat_Client_Request.h"
#include "Chat_Player.h"
#include "Chat_Player_Base.h"
#include "Msg_Active_Struct.h"
#include "Msg_Active_Enum.h"
#include "Msg_Struct.h"
#include "Chat_Player.h"
#include "Chat_Monitor.h"
#include "Config_Cache.h"
#include "Msg_Chat_Struct.h"
#include "chater/Chater_Manager.h"
#include "Daemon_Server.h"
#include "Game_Version.h"

int Chat_Player::echo_test_result(std::string result, role_id_t value) {
	MSG_83000000 msg;
	msg.chat_type = 100;
	std::stringstream stream;
	stream <<  result;
	if (value != -1)
		stream <<  value;

	Chat_Content content;
	content.text = stream.str();
	msg.content.push_back(content);

	Block_Buffer buf;
	msg.serialize(buf);
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

int Chat_Player::test_command(std::string cmd_string) {
	std::vector<std::string> cmd_tok;

	std::istringstream cmd_stream(cmd_string);
	std::copy(std::istream_iterator<std::string>(cmd_stream),
			std::istream_iterator<std::string>(), std::back_inserter(cmd_tok));

	std::cout << "cmd_string = [" << (cmd_string) << "]" << std::endl;
	if (cmd_tok.empty())
		return 0;

	// 0:关闭 1:全开 2:开放部分命令
	int command_mode = CONFIG_CACHE->server_maintainer_cache().command_mode;
	if (command_mode == 1) {
	} else if (command_mode == 2) {
		if (!CONFIG_CACHE->server_maintainer_cache().command.count(cmd_tok[0]))
			return 0;
	} else {
		return 0;
	}


	if (cmd_tok[0] == "v" || cmd_tok[0] == "version") {
		{
			std::string s;
			get_pack_md5(s);
			echo_test_result("服务端包MD5:" + s + "\n");
		}
		{
			std::string s;
			get_version_svn_ver(s);
			echo_test_result(s);
		}
		{
			std::string s;
			get_version_svn_url(s);
			echo_test_result(s);
		}
		{
			std::string s;
			get_version_time(s);
			echo_test_result(s);
		}
		{
			std::string s;
			get_version_build(s);
			echo_test_result(s);
		}
		{
			std::string s;
			get_pack_md5(s);
			if (DAEMON_SERVER_INSTANCE->server_md5_ == "") {
				DAEMON_SERVER_INSTANCE->get_server_md5();
			}
			echo_test_result(DAEMON_SERVER_INSTANCE->server_md5_+ " "+ " ");
		}
		{
			std::string s;
			get_path_md5(s);
			echo_test_result(s+"//server");
		}
	}
	/*if (cmd_tok[0] == "chat") {
		if(cmd_tok.size() == 2)  {
			std::string content = cmd_tok[1];
			world_chat(contents);
		}

		if(cmd_tok.size() == 3)  {
			role_id_t role_id = atoi(cmd_tok[1].c_str());
			std::string content = cmd_tok[2];
			chat_to_player(role_id, content);
		}
	}*/
	if(cmd_tok[0] == "getprivatecatrecord") {
		player_self()->req_history_private_chat_record(1001000000000088);
	}
	if(cmd_tok[0] == "switch_verification_chat") {
		if(cmd_tok.size() > 1) {
			CHATER_MANAGER->not_verification_chat();
		} else {
			CHATER_MANAGER->switch_verification_chat();
		}
	}
	if (cmd_tok[0] == "syschat") {
		if(cmd_tok.size() == 2)  {
			int type = atoi(cmd_tok[1].c_str());
			MSG_83000000 csMsg;

			if (type == 1) {
				Chat_Content con1;
				con1.type = 2;
				con1.role_Info.role_id = 1;
				con1.role_Info.role_name = "zhangsan";
				con1.role_Info.gender = 0;
				con1.role_Info.guild_name = "china";
				con1.role_Info.career = 1;
				con1.role_Info.level = 10;
				con1.role_Info.vip_level = 2;
				csMsg.content.push_back(con1);

				std::vector<Chat_Content *> contents;
				for (std::vector<Chat_Content>::iterator it = csMsg.content.begin(); it != csMsg.content.end(); ++it) {
					contents.push_back(&*it);
				}
				system_chat(contents);

			//	std::vector<Chat_Content *> contents;
			//	contents.push_back(&con1);

			//	system_chat(contents);
			} else if (type == 2) {
				Chat_Content con2;
				con2.type = 1;
				con2.item.item_base_info.index = 65000;
				con2.item.item_base_info.seq = 1000;
				con2.item.item_base_info.id = 20400001;
				con2.item.item_base_info.amount = 20;
				con2.item.item_base_info.bind = 0;
				csMsg.content.push_back(con2);

				std::vector<Chat_Content *> contents;
				for (std::vector<Chat_Content>::iterator it = csMsg.content.begin(); it != csMsg.content.end(); ++it) {
					contents.push_back(&*it);
				}
				system_chat(contents);
			}else if (type == 3) {
				Chat_Content con31;
				con31.type = 2;
				con31.role_Info.role_id = 1;
				con31.role_Info.role_name = "zhangsan";
				con31.role_Info.gender = 0;
				con31.role_Info.guild_name = "china";
				con31.role_Info.career = 1;
				con31.role_Info.level = 10;
				con31.role_Info.vip_level = 2;
				csMsg.content.push_back(con31);

				Chat_Content con32;
				con32.type = 1;
				con32.item.item_base_info.index = 65005;
				con32.item.item_base_info.seq = 10002;
				con32.item.item_base_info.id = 20400001;
				con32.item.item_base_info.amount = 20;
				con32.item.item_base_info.bind = 0;
				csMsg.content.push_back(con32);

				Chat_Content con33;
				con33.type = 1;
				con33.item.item_base_info.index = 650068;
				con33.item.item_base_info.seq = 10008;
				con33.item.item_base_info.id = 20400002;
				con33.item.item_base_info.amount = 20;
				con33.item.item_base_info.bind = 0;
				csMsg.content.push_back(con33);

				std::vector<Chat_Content *> contents;
				for (std::vector<Chat_Content>::iterator it = csMsg.content.begin(); it != csMsg.content.end(); ++it) {
					contents.push_back(&*it);
				}
				system_chat(contents);
			}else if (type == 4) {
				Chat_Content con41;
				con41.type = 2;
				con41.role_Info.role_id = 1;
				con41.role_Info.role_name = "zhangsan";
				con41.role_Info.gender = 0;
				con41.role_Info.guild_name = "china";
				con41.role_Info.career = 1;
				con41.role_Info.level = 10;
				con41.role_Info.vip_level = 2;
				csMsg.content.push_back(con41);

				Chat_Content con42;
				con42.type = 1;
				con42.item.item_base_info.index = 65000048;
				con42.item.item_base_info.seq = 100004;
				con42.item.item_base_info.id = 20400001;
				con42.item.item_base_info.amount = 20;
				con42.item.item_base_info.bind = 0;
				csMsg.content.push_back(con42);

				Chat_Content con43;
				con43.type = 2;
				con43.role_Info.role_id = 1;
				con43.role_Info.role_name = "zhangsan";
				con43.role_Info.gender = 0;
				con43.role_Info.guild_name = "china";
				con43.role_Info.career = 1;
				con43.role_Info.level = 10;
				con43.role_Info.vip_level = 2;
				csMsg.content.push_back(con43);

				std::vector<Chat_Content *> contents;
				for (std::vector<Chat_Content>::iterator it = csMsg.content.begin(); it != csMsg.content.end(); ++it) {
					contents.push_back(&*it);
				}
				system_chat(contents);
			}else if (type == 5) {
				Chat_Content con5;

				csMsg.content.push_back(con5);

				std::vector<Chat_Content *> contents;
				for (std::vector<Chat_Content>::iterator it = csMsg.content.begin(); it != csMsg.content.end(); ++it) {
					contents.push_back(&*it);
				}
				system_chat(contents);
			}


		}
	}

	return 0;
}
