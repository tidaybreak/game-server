/*
 * Logic_Player_Struct.h
 *
 *  Created on: 2014年2月18日
 *      Author: linqiyou
 */

#ifndef LOGIC_PLAYER_STRUCT_H_
#define LOGIC_PLAYER_STRUCT_H_

#include "Server_Struct.h"
#include "Base_Struct.h"
#include "friend/Friend_Struct.h"
#include "dragon/Dragon_Vale_Struct.h"

//////////////////////////////////////////////////////////////////////////////

struct Player_DB_View {
    enum ACCOUNT_MODE {
    	ACCOUNT_DOING_LOGIN			= 0x02,		// 登录中
    	ACCOUNT_DOING_LOGOUT		= 0X04,		// 下线中
    	ACCOUNT_STATUS_TRUSTEESHIP	= 0x20		// 托管状态
    };

    Player_DB_View(void);
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	void set_account_status(int status, int val);
	void cancel_account_status(int status);

	role_id_t role_id;
	Account_Key account_info;
	Location location;
	std::string role_name;			/// 角色名
	int level;     			   		/// 等级[由战斗模块初始化]
	uint8_t gender; 					/// 性别0 = 女(female), 1 = 男(male)
	uint8_t career; 					/// 职业 1-4	([1,4])
	uint32_t head_id;				// 头像
	int force;						// 战斗力
	int ttl_force; 					// 历史最高战力
	uint8_t vip_level;				/// VIP
	Time_Value vip_over_time;       /// VIP 过期时间
	uint8_t country;				//  国家(0:不分 1:蓬莱 2:星宫 3:昆仑)
	std::string gang_name;			// 	公会名
	gang_id_t gang_id;				//	公会id
	uint16_t gang_war_award_get_times_by_day;
	Time_Value last_sign_in_time;			// 最后登录时间
	Time_Value last_sign_out_time;			// 最后登出时间
	int add_up_online_time;                 // 累计在线时间(秒)
	uint8_t fcm;
	std::string ip;					//IP地址
//	int be_worship;                 // 被膜拜次数

	Friend_DB_View friend_;

	int account_mode;
	int mode_value;
	int title_id;

	Int_Vec avatar_vec;						//时装

	uint32_t vip(void);

	Dragon_Vale_DB_View dragon_vale_view;

	Time_Value sign_out_flag_;			// 登出标识

	bool is_dragon_vale_special();
	void hook_dragon_vale_special_change();

	int db_view_size(void);
};

//////////////////////////////////////////////////////////////////////////////

struct DB_View {
	typedef boost::unordered_map<role_id_t, Player_DB_View *> ID_Player_View_Map;
	typedef boost::unordered_map<std::string, Player_DB_View *> Name_Player_View_Map;
	typedef boost::unordered_map<Account_Key, Player_DB_View *> Account_Player_View_Map;
	typedef boost::unordered_map<std::string, std::vector<Player_DB_View *> > Account_Role_View_Map;
	typedef boost::unordered_map<Server_Key, role_id_t > Server_Key_Amount_Map;
	typedef boost::unordered_map<Role_Key, int32_t> Role_Num_Map;
	typedef boost::unordered_map<role_id_t, std::string> Role_Session_Map;

	const static int player_view_map_bucket_num = 50000;

	DB_View(void);
	void reset(void);
	int insert_new_player(Player_DB_View *player);
	Role_Key get_min_career(void);
	void abnormal_account_add(role_id_t role_id, int delay_sec = 5);

	ID_Player_View_Map id_player_view_map;
	Name_Player_View_Map name_player_view_map;
	Account_Player_View_Map account_player_view_map;
	Account_Role_View_Map account_role_view_map;
	Server_Key_Amount_Map server_amount_map;
	Role_Num_Map role_num_map;

	Role_Session_Map role_session_map;
	Int_Uid login_cid_role_id_map;			// 正在登陆的玩家
	Role_Id_Time_Map abnormal_account;		// 正在退出的玩家，core之类会影响下线流程，这里做下预防
};


#endif /* LOGIC_PLAYER_STRUCT_H_ */
