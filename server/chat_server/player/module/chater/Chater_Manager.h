/*
 * Chater_Manager.h
 *
 *  Created on: 2014-06-19
 *      Author: xiaoliang
 */

#ifndef CHATER_MANAGER_H_
#define CHATER_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Msg_Struct.h"

struct Chat_Record;
class Chat_Player;

class Chater_Manager {
	typedef boost::unordered_map<role_id_t, UInt_Vec> RoleID_UInt_Vec_Map;
	typedef boost::unordered_map<role_id_t, String_Int_Map> RoleID_StringIntMap_Map;

	struct Chat_Check_Info {
		Chat_Check_Info(void) { reset(); }
		void reset(void) {
			role_id_last_chat_time.clear();
			role_id_gap_num.clear();
			//role_id_contents_num.clear();
			contets.clear();
			contents_num.clear();
			verification = true;
			last_tick_time = Time_Value::zero;
			ban_role_id.clear();
			ban_contents.clear();
		}
		Role_Id_Time_Map role_id_last_chat_time;//玩家最后一次聊天时间
		RoleID_UInt_Vec_Map role_id_gap_num;//玩家聊天间隔发送次数
		//RoleID_StringIntMap_Map role_id_contents_num;//玩家聊天内容相同个数
		String_Vec contets;//最近N条聊天内容记录
		String_Int_Map contents_num;//最近N条聊天记录中的内容对应次数
		String_Set ban_contents;//被封禁的聊天
		Role_Id_Set ban_role_id;//封禁列表
		bool verification;//检测开关,true 检测
		Time_Value last_tick_time;
	};
public:
	Chater_Manager();
	~Chater_Manager();
	void reset();

	void tick(const Time_Value &now);

	//公告
	int send_announce(int8_t type, int32_t ann_id, int64_t gang_id, const std::vector<role_id_t>& recievers, const std::vector<Chat_Content>& contents);

	//0dian
	int player_trigger_daily_zero(void);

	//system do
	void get_chat_contents_to_string(const std::vector<Chat_Content>& contents, std::string &str_res);
	bool check_chat_contents_interface(Chat_Player *player, const std::vector<Chat_Content>& contents);
	void switch_verification_chat(void);
	void not_verification_chat(void);

	//平台相关接口
	void send_chat_contents_to_pl(Chat_Player *player, int64_t target, std::string &target_name, const std::vector<Chat_Content>& contents);
private:
	//系统发出公告
	int announce_in_role_vec(const std::vector<role_id_t>& recievers, const std::vector<Chat_Content>& contents, uint8_t district, int32_t ann_id);
	int announce_in_gang(int64_t gang_id, int ann_id, const std::vector<Chat_Content>& contents);
	int announce_in_broadcast(int8_t type, int32_t ann_id, const std::vector<Chat_Content>& contents);

	void md5_string(const std::string &str_src, std::string &str_res);
	void record_chat_contents_text_num(role_id_t role_id, const std::string &str);
	bool verification_chat(const role_id_t role_id, const std::string & str);
	bool verification_chat_white_list(const std::string & str);
	bool verification_chat_black_list(const std::string & str);

	Chat_Check_Info chat_info;
};

typedef Singleton<Chater_Manager> Chater_Manager_Single;
#define CHATER_MANAGER Chater_Manager_Single::instance()

#endif /* CHATER_MANAGER_H_ */
