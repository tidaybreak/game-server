/////////////////////////Friend start///////////////////////////
// 好友申请
#ifndef FRIEND_STRUCT_H_
#define FRIEND_STRUCT_H_
#include "Server_Struct.h"
#include "boost/unordered_map.hpp"
#include "DB_Manager.h"
#include "Msg_Struct.h"

typedef boost::unordered_map<role_id_t, Friend_Info> Friend_Info_Map;
typedef boost::unordered_map<role_id_t, int32_t> RoleID_Friend_Map;
// 好友详细
struct Friend_Detail: public Detail {

	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);


	Friend_Detail(void) : role_id(0) { reset(); }

	role_id_t role_id;
	Friend_Info_Map firend_list_;
	Friend_Info_Map stranger_list_;
	Friend_Info_Map black_list_;
	Role_Id_Time_Map invite_list_;
	Role_Id_Time_Map apply_list_;
	int32_t head_photo_id;

	RoleID_Friend_Map del_firend_list_;

};

struct Friend_DB_View {
	void reset(void);
	Friend_DB_View(void) { reset(); }
	//Friend_Info_Map firend_list_;
	Role_Id_Set firend_list_;
	uint16_t apply_list_size;
	uint16_t invite_list_size;
	int32_t head_photo_id;
};

#endif
/////////////////////////////Friend end////////////////////////

