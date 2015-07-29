
#ifndef FRIEND_H_
#define FRIEND_H_

#include "Logic_Player_Base.h"
#include "Friend_Struct.h"

struct Friend_Detail;
struct Friend_Info;

class Logic_Player;

class Friend : virtual public Logic_Player_Base {
public:

	Friend(void);
	virtual ~Friend();
	void reset(void);
	static int create_init(Friend_Detail &detail);
	void load_detail(Friend_Detail &detail);
	void save_detail(Friend_Detail &data);

	Friend_Detail const &friend_detail(void) const;

	void module_init(void);
	void sign_in(void);
	void sign_out(void);
	// 场景加载完成
	int move_scene_completed(void);
	// 客户端初始化完成(登录后客户端准备完成调用,通知客户端模块信息)
	int client_ready_now(void);

	int fetch_friend_list(); // get friend list for client
	int get_friend_info(role_id_t role_id); //get friend info by role_id for client
	int fetch_apply_list(); //get invite be friend for client
	int remove_friend(role_id_t role_id, int8_t type); //delete friend on list for client
	int append_add_friend(std::string role_name); //add friend for client
	int multi_add_friend(String_Vec &role_name_vec); //multi add friend for client
	int handel_handel_apply(int8_t handel, role_id_t role_id, std::string role_name); //对方处理请求后的处理函数
	int handle_friend_apply(int8_t handel, std::string &role_name);
	int handle_friend_apply(int8_t handel, role_id_t role_id);
	int add_to_black(std::string role_name);

	void syn_to_dbview(void);

	void syn_black_list_to_chat_server(int8_t type, role_id_t role_id);
	void syn_black_list_to_chat_server(Block_Buffer &buf);

	int offline_add_invite(Role_Id_Time_Map &invite_list);
	int offline_handel_apply(RoleID_Int8_Map &apply_list);

	int is_friend(role_id_t role_id);

	int req_role_online_status(Role_Vec stranger_id_list, Role_Vec recent_id_list);

	int req_friend_set_photo(void);
	int req_set_head_photo(int32_t photo_id);

	void friend_listen_level_up(const int level);

	int get_head_photo_id(void);

	int req_friend_base_info_list(void);

	//interface;
	void add_friendship_degree(role_id_t role_id, uint32_t num);

private:
	int add_friend(std::string &role_name, int &status);
	void fill_friend_info(role_id_t role_id, Friend_Info &fi);

	Friend_Detail *friend_detail_;
};

inline Friend_Detail const &Friend::friend_detail(void) const {
	return *friend_detail_;
}


#endif /* FRIEND_H_ */

