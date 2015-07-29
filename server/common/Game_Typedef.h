/*
 * custom_type.h
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 */

#ifndef GAME_TYPEDEF_H_
#define GAME_TYPEDEF_H_

#include "Game_Header.h"

class Mover;
class Fighter;
class Scene_Player;
class NPC;

struct Coord;
struct Item_Info;
struct Property;
struct Prop_Change;
struct Int_Double;
struct Int_Role_Id;
struct Int_Int;

typedef int64_t role_id_t;				/// 角色id
typedef int64_t long_id_t;				/// 角色id
typedef int64_t team_id_t;				/// 角色id
typedef int64_t faction_id_t;			/// 帮派id
typedef int64_t gang_id_t;			/// 帮派id

typedef int64_t mail_id_t;              //邮件id
typedef uint16_t icon_id_t;             //图标id

typedef Block_List<Thread_Mutex> Data_Mutex_List;
typedef List<int, Thread_Mutex> Int_Mutex_List;
typedef Block_List<Thread_Mutex> Data_Spin_List;
typedef List<int, Thread_Mutex> Int_Spin_List;
typedef List<Block_Buffer *, Thread_Mutex> Block_Mutex_List;

typedef std::set<int8_t> Int8_Set;
typedef std::set<int16_t> Int16_Set;
typedef std::set<int32_t> Int_Set;
typedef std::set<uint32_t> UInt_Set;
typedef std::set<role_id_t> Role_Id_Set;
typedef std::set<long_id_t> Long_Id_Set;
typedef boost::unordered_set<std::string> String_Set;

typedef boost::unordered_set<role_id_t> Role_Hash_Set;
typedef boost::unordered_set<int> Int_Hash_Set;

typedef std::vector<int8_t> Int8_Vec;
typedef std::vector<int32_t> Int_Vec;
typedef std::vector<uint32_t> UInt_Vec;
typedef std::vector<role_id_t> Role_Vec;
typedef std::vector<std::string> String_Vec;
typedef std::vector<double> Double_Vec;
typedef std::vector<Int_Int> Int_Int_Vec;
typedef std::vector<Int_Double> Int_Double_Vec;
typedef std::vector<Int_Role_Id> Int_Role_Id_Vec;
typedef boost::unordered_map<int, Int_Vec> Int_IntVec_Map;
typedef boost::unordered_map<uint32_t, Int_Vec> UInt_IntVec_Map;
typedef boost::unordered_map<uint32_t, UInt_Vec> UInt_UIntVec_Map;
typedef boost::unordered_map<uint32_t, Int_Set> UInt_IntSet_Map;
typedef boost::unordered_map<int, Role_Hash_Set> Int_RoleHashSet_Map;
typedef std::vector<Int_Vec> Int_Vec_Vec;

typedef std::list<int> Int_List;
typedef std::list<role_id_t> Role_Id_List;

typedef std::deque<int32_t> Int_Deque;
typedef std::deque<uint32_t> UInt_Deque;
typedef std::deque<role_id_t> Role_Deque;
typedef std::deque<int32_t> Int_Deque;
typedef std::deque<uint32_t> UInt_Deque;
typedef std::queue<role_id_t> Role_Queue;

typedef boost::unordered_map<int8_t, int> Int8_Int_Map;
typedef boost::unordered_map<int, Int_Set> Int_Int_Set_Map;
typedef boost::unordered_map<int, int> Int_Int_Map;
typedef boost::unordered_map<uint32_t, int> UInt_Int_Map;
typedef boost::unordered_map<int, std::string> Int_String_Map;
typedef boost::unordered_map<int, int8_t> Int_Bool_Map;
typedef boost::unordered_map<int, double> Int_Double_Map;
typedef boost::unordered_map<int, Time_Value> Int_Time_Map;
typedef boost::unordered_map<int, role_id_t> Int_Uid;
typedef boost::unordered_map<int, Role_Id_Set> Int_Role_Set_Map;
typedef boost::unordered_map<uint32_t, Role_Id_Set> UInt_Role_Set_Map;
typedef boost::unordered_map<role_id_t, Role_Id_Set> Role_RoleIdSet_Map;
typedef boost::unordered_map<role_id_t, Role_Vec> Role_RoleVec_Map;
typedef boost::unordered_map<uint32_t, role_id_t> UInt_Uid;
typedef boost::unordered_map<std::string, int> String_Int_Map;
typedef boost::unordered_map<std::string, Time_Value> String_Time_Map;
typedef boost::unordered_map<role_id_t, Time_Value> Role_Id_Time_Map;
typedef boost::unordered_map<std::string, std::string> String_String_Map;
typedef std::map<uint32_t, uint32_t> UInt_UInt_Map;
typedef std::map<uint32_t, UInt_UInt_Map> UInt_UIntUIntMap_Map;
typedef std::map<int32_t, int32_t> Int_Int_Std_Map;
typedef boost::unordered_map<int, Int_Hash_Set> Int_IntHashSet_Map;

typedef boost::unordered_map<role_id_t, double> RoleID_Double_Map;
typedef boost::unordered_map<role_id_t, int8_t> RoleID_Int8_Map;
typedef boost::unordered_map<role_id_t, Int_Int_Map> Role_IntIntMap_Map;
typedef boost::unordered_map<role_id_t, int> Role_Int_Map;
typedef boost::unordered_map<role_id_t, Role_Int_Map> Role_RoleIntMap_Map;
typedef boost::unordered_map<role_id_t, role_id_t> RoleID_RoleID_Map;
typedef boost::unordered_map<role_id_t, NPC*> RoleID_NPC_Map;
typedef boost::unordered_map<role_id_t, Int8_Set> RoleID_Int8Set_Map;
typedef boost::unordered_map<role_id_t, std::string> RoleID_String_Map;
typedef boost::unordered_map<role_id_t, uint32_t> Role_UInt_Map;
typedef boost::unordered_map<role_id_t, Role_UInt_Map> Role_RoleUIntMap_Map;
typedef boost::unordered_map<role_id_t, Int_Hash_Set> Role_IntHashSet_Map;
typedef boost::unordered_map<role_id_t, bool> Role_Bool_Map;
typedef boost::unordered_map<role_id_t, Role_Hash_Set> Role_RoleHashSet_Map;

typedef boost::unordered_set<Mover*> Mover_Set;
typedef boost::unordered_set<Fighter*> Fighter_Set;
typedef boost::unordered_set<Scene_Player*> Scene_Player_Set;

typedef std::vector<Coord> Coord_Vec;
typedef std::vector<Coord_Vec> Coord_Vec_Vec;
typedef std::vector<Item_Info> Item_Info_Vec;
typedef std::vector<Property> Property_Vec;
typedef std::vector<Prop_Change> Prop_Change_Vec;

typedef boost::unordered_map<int, Item_Info> Id_ItemInfo_Map;
typedef boost::unordered_map<int, Item_Info_Vec> Int_Item_Info_Vec_Map;
typedef boost::unordered_map<int64_t, Item_Info_Vec> Int64_Item_Info_Vec_Map;

////--Json_Value
typedef RE_MUTEX Config_Lock;
typedef void (*JsonKey_Func)(const Json::Value &json, int &key);
typedef boost::unordered_map<std::string, Json::Value> Config_Map;
typedef boost::unordered_map<int, Json::Value> Int_Json_Map;
typedef boost::unordered_map<std::string, Json::Value> String_Json_Map;
////--Json_Value

#define	LOGIN_VALIDATE_TIMEOUT	120

#define CALL_RETURN		1000
#define CALL_CONTINUE	1001

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

#define CLEAR_VECTOR(VECTOR, ARG) \
	{\
		VECTOR tmp;\
		ARG.swap((VECTOR&&)tmp);\
	}


/*
 * if CONDITION false then continue
 */


// 一般用在启动时配制检查
#define JUDGE_ERROR_ABORT(CONDITION) \
    if (CONDITION) {\
        abort();\
    }

// 检查buf是否合法(长度)
#define CHECK_BUFFER_LEN(BUFFER) \
    if (!BUFFER.check_pack_len()) {\
    	LOG_USER_TRACE("buffer error");\
    	 abort();\
    }

// INT空值CORE
#define ABORT_INT_ZERO(CONDITION) \
    if (CONDITION == 0) {\
    	abort();\
    }

// 空字符串CORE
#define ABORT_STRING_NULL(CONDITION) \
    if (CONDITION == "") {\
    	abort();\
    }

// 遇到CALL_RETURN返回
#define JUDGE_CALL_RETURN(CONDITION) \
    if (CONDITION == CALL_RETURN) {\
        return CALL_RETURN;\
    }

// 解包错误返回
#define JUDGE_MSG_ERROR_RETURN(MSG) \
	buf.set_read_abort(false);\
    if (MSG.deserialize(buf)) {\
    	buf.set_read_abort(true);\
    	buf.set_read_idx(buf.get_read_idx() - 4);\
    	int msg_id = buf.get_msg_id();\
    	buf.set_read_idx(buf.get_read_idx() + 4);\
    	LOG_DEBUG("depack error, msg:%d", msg_id);\
        return 10000101;\
    }\
    buf.set_read_abort(true);

// 遇到错误返回
#define JUDGE_ERROR_RETURN(CONDITION) \
    if (CONDITION) {\
    	MSG_USER_TRACE("something error");\
        return CALL_RETURN;\
    }

// 角色空指针
#define JUDGE_PLAYER_NULL(CONDITION, ID) \
    if (! CONDITION) {\
    	MSG_USER_TRACE("player null, role:%ld", ID);\
        return 0;\
    }


#define MAKE_MSG_BUF(MSG, BUF) \
	BUF.make_message(MSG.msg_id); \
    MSG.serialize(BUF); \
    BUF.finish_message();

// 发给客户端(player内使用)
#define THIS_SEND_TO_CLIENT(MSG) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    this->send_to_client(buf__, true); \
	}

// 发给客户端(player外使用)
#define OBJ_SEND_TO_CLIENT(MSG, OBJ) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    OBJ.send_to_client(buf__, true); \
	}

// 发给逻辑服(player内使用)
#define THIS_SEND_TO_LOGIC(MSG) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    this->send_to_logic(buf__); \
	}

// 发给逻辑服(player外使用)
#define OBJ_SEND_TO_LOGIC(MSG, OBJ) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    OBJ.send_to_logic(buf__); \
    }

// 发给场景服(player内使用)
#define THIS_SEND_TO_SCENE(MSG) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    this->send_to_scene(buf__, true); \
	}

// 发给场景服(player外使用)
#define THIS_SEND_TO_GATE(MSG) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    this->send_to_gate(buf__); \
	}

// 发给聊天服(player内使用)
#define THIS_SEND_TO_LOCAL_CHAT(MSG) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    this->send_to_local_chat(buf__); \
	}

// 发给聊天服(player外使用)
#define OBJ_SEND_TO_LOCAL_CHAT(MSG, OBJ) \
	{ \
	Block_Buffer buf__; \
	buf__.make_message(MSG.msg_id); \
    MSG.serialize(buf__); \
    buf__.finish_message(); \
    OBJ.send_to_local_chat(buf__); \
	}


#endif /* GAME_TYPEDEF_H_ */
