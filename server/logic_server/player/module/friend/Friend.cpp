
#include "Logic_Module_Head.h"
#include "Friend_Struct.h"
#include "Friend.h"
#include "Msg_Struct.h"
#include "Logic_Monitor.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Msg_Friend_Struct.h"
#include "Msg_Active_Struct.h"

#include "Configurator.h"
#include "DB_Operator.h"
#include "DB_Manager.h"
#include "Config_Cache.h"
#include <queue>
#include "Fighter_Detail.h"
#include "Logic_Player_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Err_Code.h"
#include "Config_Cache_Friend.h"
#include "Friend_Def.h"

Friend::Friend(void) {
	reset();
}

Friend::~Friend() {
}

void Friend::reset(void) {
	friend_detail_ = 0;
}

int Friend::create_init(Friend_Detail &detail) {
	return 0;
}

void Friend::load_detail(Friend_Detail &detail) {
	friend_detail_ = &detail;
}

void Friend::save_detail(Friend_Detail &detail) {
	if (friend_detail_->is_change) {
		detail = *friend_detail_;
		friend_detail_->is_change = false;
	}
	//LOGIC_PUBLIC->sync_friend_to_db(this->friend_detail_);
}

void Friend::module_init(void) {
	this->syn_to_dbview();
	this->get_head_photo_id();
	return;
}

void Friend::sign_in(void) {

}

void Friend::sign_out(void) {

}

int Friend::move_scene_completed(void) {
	return 0;
}

int Friend::client_ready_now(void) {
	int count = friend_detail_->invite_list_.size();
	if(count > 0){
		MSG_82200001 invite_msg;
		invite_msg.reset();
		invite_msg.friend_apply_in = count;
		THIS_SEND_TO_CLIENT(invite_msg);
	}
	return 0;
}

int Friend::fetch_friend_list(){
	MSG_50160000 msg;
	msg.reset();
	typedef std::map<unsigned long long int, Friend_Info, std::greater<unsigned long long int> > Firend_Map; //排序: 1、是否在线 	2、好友度 3、是否是VIP	4、等级
	/********firend_list_**********/
	//排序
	Firend_Map friend_map_;
	friend_map_.clear();
	int index = 0;
	for(Friend_Info_Map::iterator it = friend_detail_->firend_list_.begin(); it != friend_detail_->firend_list_.end(); it++) {
		role_id_t role_id = it->first;
		//填充好友信息
		fill_friend_info(role_id, it->second);
		unsigned long long int sort = (it->second.is_on_line) * pow(10, 14) + it->second.friendship_degree * pow(10, 8) + it->second.vip * pow(10, 7) + it->second.level*pow(10, 4) + index;
		friend_map_.insert(std::make_pair(sort,it->second));
		++index;
	}
	//make the message
	for(Firend_Map::iterator it = friend_map_.begin(); it != friend_map_.end(); it++) {
		msg.friend_info_list.push_back(it->second);
	}
	friend_map_.clear();

	/*******stranger_list_*******/
	for(Friend_Info_Map::iterator it = friend_detail_->stranger_list_.begin(); it != friend_detail_->stranger_list_.end(); it++) {
		role_id_t role_id = it->first;
		//填充好友信息
		fill_friend_info(role_id, it->second);
		msg.stranger_info_list.push_back(it->second);

	}
	/*****black_list_*********/
	for(Friend_Info_Map::iterator it = friend_detail_->black_list_.begin(); it != friend_detail_->black_list_.end(); it++) {
		role_id_t role_id = it->first;
		//填充好友信息
		fill_friend_info(role_id, it->second);
		msg.black_info_list.push_back(it->second);
	}
	/***********/
	// start 处理自己的头像
	msg.head_photo_id = get_head_photo_id();
	// end 处理自己的头像
	//回应客户端
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int  Friend::get_friend_info(role_id_t role_id) {
	MSG_50160001 msg;
	msg.reset();
	bool isfind = false;
	Friend_Info_Map::iterator friend_info_it;
	do{
		friend_info_it = friend_detail_->firend_list_.find(role_id);
		if(friend_info_it !=  friend_detail_->firend_list_.end()) {
			isfind = true;
			break;
		}
		friend_info_it = friend_detail_->stranger_list_.find(role_id);
		if(friend_info_it !=  friend_detail_->stranger_list_.end()) {
			isfind = true;
			break;
		}
		friend_info_it = friend_detail_->black_list_.find(role_id);
		if(friend_info_it !=  friend_detail_->black_list_.end()) {
			isfind = true;
			break;
		}
	}while(false);
	if(!isfind){ return -1;};
	//填充好友信息
	fill_friend_info(role_id, friend_info_it->second);
	msg.friend_info = friend_info_it->second;
	//回应客户端
	Block_Buffer buf;
	msg.serialize(buf);
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Friend::fetch_apply_list() {
	MSG_50160002 msg;
	msg.reset();
	//init the struct by Friend_Apply_Info
	Friend_Apply_Info fai;
	for(Role_Id_Time_Map::iterator it = friend_detail_->invite_list_.begin(); it != friend_detail_->invite_list_.end(); it++) {
		fai.reset();
		fai.apply_id = it->first;
		fai.role_name = LOGIC_MONITOR->get_role_name_by_id(it->first);
		fai.apply_time = it->second.sec();

		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->first);
		if (iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			fai.level = iter->second->level;
			fai.ttl_force = iter->second->ttl_force;
		} else {
			MSG_USER("player(role_id=%ld) is not in db_view", it->first);
		}


		msg.apply_lists.push_back(fai);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Friend::remove_friend(role_id_t role_id, int8_t type) {
	MSG_50160003 msg;
	msg.reset();
	Friend_Info_Map::iterator friend_info_it;
	switch(type) {
		case 1: {
			friend_info_it = friend_detail_->firend_list_.find(role_id);
			if(friend_info_it !=  friend_detail_->firend_list_.end()) {
				//如果有好友度;存起来;方便以后加上好友而不丢失好友度
				if(friend_info_it->second.friendship_degree > 1) {
					friend_detail_->del_firend_list_.insert(std::make_pair(role_id,friend_info_it->second.friendship_degree));
				}

				friend_detail_->firend_list_.erase(friend_info_it);
			} else {
				return ERROR_CLIENT_PARAM;
			}
			break;
		}
		case 2: {
			friend_info_it = friend_detail_->stranger_list_.find(role_id);
			if(friend_info_it !=  friend_detail_->stranger_list_.end()) {
				friend_detail_->stranger_list_.erase(friend_info_it);
			} else {
				return ERROR_CLIENT_PARAM;
			}
			break;
		}
		case 3: {
			friend_info_it = friend_detail_->black_list_.find(role_id);
			if(friend_info_it !=  friend_detail_->black_list_.end()) {
				friend_detail_->black_list_.erase(friend_info_it);
				this->syn_black_list_to_chat_server(0, role_id); //syn to Chat Server
			} else {
				return ERROR_CLIENT_PARAM;
			}
			break;
		}
	}

	friend_detail_->detail_change();

	this->syn_to_dbview();
	// send to client
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Friend::multi_add_friend(String_Vec &role_name_vec) {
	MSG_50160013 msg;
	Int_String is;
	for(String_Vec::iterator it = role_name_vec.begin(); it != role_name_vec.end(); ++it) {
		is.reset();
		int status = 1;
		int res = add_friend(*it, status);
		if(res) {
			is.int_value = res;
		} else {
			is.int_value = status;
		}
		is.str_value = *it;
		msg.role_name_status.push_back(is);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
int Friend::append_add_friend(std::string role_name) {
	MSG_50160004 msg;
	int status = 1; //客户端要求知道是不是直接添加的(1.申请添加的; 6.对方好友列表有自己,直接添加的)
	int res = add_friend(role_name, status);
	if(res) {
		return res;
	}
	// send to client
	msg.status = status;
	msg.role_name = role_name;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Friend::add_friend(std::string &role_name, int &status) {
	//判断自己好友数量
	if(friend_detail_->firend_list_.size() + friend_detail_->apply_list_.size() + friend_detail_->invite_list_.size()
			>= CONFIG_CACHE_FRIEND->find_common_config_val(FRIEND_COMM_CONF_FRIEND_MAX_NUM)) {
		return ERROR_FRIEND_NUM_MAX;
	}
	role_id_t role_id = LOGIC_MONITOR->get_role_id_by_name(role_name);
	//判断玩家是否存在
	if(!role_id){
		return ERROR_FRIEND_ROLE_NOT_EXIST;
	}
	//判断是否自己
	if(role_id == this->role_id()) {
		return ERROR_ADD_SELF_FRIEND;
	}
	//判断是否好友
	int type = is_friend(role_id);
	if(type == 0) {
		return ERROR_IS_FRIEND;
	}
	//判断是否在申请
	if(friend_detail_->apply_list_.find(role_id) != friend_detail_->apply_list_.end()) {
		return ERROR_IN_APPLY_LIST;
	}
	//如果已经在对方好友列表直接加
	DB_View::ID_Player_View_Map::iterator find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		Role_Id_Set::iterator friend_iter = find_it->second->friend_.firend_list_.find(this->role_id());
		if (friend_iter != find_it->second->friend_.firend_list_.end()) {
			Friend_Info_Map::iterator it;
			//如果在黑名单;删除
			it = friend_detail_->black_list_.find(role_id);
			if(it != friend_detail_->black_list_.end()) {
				friend_detail_->black_list_.erase(it);
				this->syn_black_list_to_chat_server(0, role_id); //syn to Chat Server
			}
			//如果在陌生人;删除
			it = friend_detail_->stranger_list_.find(role_id);
			if(it != friend_detail_->stranger_list_.end()) {
				friend_detail_->stranger_list_.erase(it);
			}
			Friend_Info fi;
			fill_friend_info(role_id, fi);
			fi.role_id = role_id;
			fi.role_name = role_name;
			fi.friend_type = 0;
			fi.friendship_degree = 1;
			//如果之前有删除过好友;并且有好友度
			RoleID_Friend_Map::iterator diter = friend_detail_->del_firend_list_.find(role_id);
			if(diter != friend_detail_->del_firend_list_.end()) {
				fi.friendship_degree = diter->second;
				friend_detail_->del_firend_list_.erase(diter);
			}

			friend_detail_->firend_list_.insert(std::make_pair(role_id, fi));
			//已经在对方好友列表直接加,同步到DBVIEW
			this->syn_to_dbview();
			status = 6; //在对方好友列表的状态
			return 0;
		}
	}
	//申请添加
	friend_detail_->apply_list_.insert(std::make_pair(role_id, Time_Value::gettimeofday()));
	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (receiver) {
		if(receiver->friend_detail_->firend_list_.size() + receiver->friend_detail_->apply_list_.size() + receiver->friend_detail_->invite_list_.size()
				>= CONFIG_CACHE_FRIEND->find_common_config_val(FRIEND_COMM_CONF_FRIEND_MAX_NUM)) {
			return ERROR_PER_FRIEND_NUM_MAX;
		}
		receiver->friend_detail_->invite_list_.insert(std::make_pair(this->role_id(), Time_Value::gettimeofday()));
		MSG_82200001 invite_msg;
		invite_msg.reset();
		invite_msg.friend_apply_in = receiver->friend_detail_->invite_list_.size();
		OBJ_SEND_TO_CLIENT(invite_msg, (*receiver));
		receiver->friend_detail_->detail_change();
	} else {
		if (find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			if(find_it->second->friend_.firend_list_.size()+find_it->second->friend_.apply_list_size+find_it->second->friend_.invite_list_size
					>= CONFIG_CACHE_FRIEND->find_common_config_val(FRIEND_COMM_CONF_FRIEND_MAX_NUM)) {
				return ERROR_PER_FRIEND_NUM_MAX;
			}
		}
		//对方不在线情况
		Logic_Player::offline_firend_add(this->role_id(), role_id,Time_Value::gettimeofday());
	}
	status = 1;//不在对方好友列表的状态
	friend_detail_->detail_change();
	this->syn_to_dbview();
	return 0;
}

int Friend::handel_handel_apply(int8_t handel, role_id_t role_id, std::string role_name) {
	Role_Id_Time_Map::iterator a_it = friend_detail_->apply_list_.find(role_id);
	if(a_it != friend_detail_->apply_list_.end()) {
		friend_detail_->apply_list_.erase(a_it);
	}

	if(handel == 1) {
		Friend_Info_Map::iterator it;
		//如果在黑名单;删除
		it = friend_detail_->black_list_.find(role_id);
		if(it != friend_detail_->black_list_.end()) {
			friend_detail_->black_list_.erase(it);
			this->syn_black_list_to_chat_server(0, role_id); //syn to Chat Server
		}
		//如果在陌生人;删除
		it = friend_detail_->stranger_list_.find(role_id);
		if(it != friend_detail_->stranger_list_.end()) {
			friend_detail_->stranger_list_.erase(it);
		}
		Friend_Info fi;
		fill_friend_info(role_id, fi);
		fi.role_id = role_id;
		fi.role_name = role_name;
		fi.friend_type = 0;
		fi.friendship_degree = 1;
		//如果之前有删除过好友;并且有好友度
		RoleID_Friend_Map::iterator diter = friend_detail_->del_firend_list_.find(role_id);
		if(diter != friend_detail_->del_firend_list_.end()) {
			fi.friendship_degree = diter->second;
			friend_detail_->del_firend_list_.erase(diter);
		}
		friend_detail_->firend_list_.insert(std::make_pair(role_id, fi));
		friend_detail_->detail_change();
		this->syn_to_dbview();
	}
	//通知客户端
	MSG_82200002 handel_msg;
	handel_msg.reset();
	handel_msg.handel_friend_apply = handel;
	handel_msg.role_name = role_name;
	THIS_SEND_TO_CLIENT(handel_msg);
	return 0;
}

int Friend::handle_friend_apply(int8_t handel, role_id_t role_id) {
	if(handel == 1) {
		//如果是同意,
		Friend_Info_Map::iterator it;
			//自己
		//如果在黑名单;删除
		it = friend_detail_->black_list_.find(role_id);
		if(it != friend_detail_->black_list_.end()) {
			friend_detail_->black_list_.erase(it);
			this->syn_black_list_to_chat_server(0, role_id); //syn to Chat Server
		}
		//如果在陌生人;删除
		it = friend_detail_->stranger_list_.find(role_id);
		if(it != friend_detail_->stranger_list_.end()) {
			friend_detail_->stranger_list_.erase(it);
		}
		Friend_Info fi;
		fill_friend_info(role_id, fi);
		fi.role_id = role_id;
		fi.role_name = LOGIC_MONITOR->get_role_name_by_id(role_id);
		fi.friend_type = 0;
		fi.friendship_degree = 1;
		//如果之前有删除过好友;并且有好友度
		RoleID_Friend_Map::iterator diter = friend_detail_->del_firend_list_.find(role_id);
		if(diter != friend_detail_->del_firend_list_.end()) {
			fi.friendship_degree = diter->second;
			friend_detail_->del_firend_list_.erase(diter);
		}

		friend_detail_->firend_list_.insert(std::make_pair(role_id, fi));
	}
	Logic_Player *receiver = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (receiver) {
		receiver->handel_handel_apply(handel, this->role_id(), this->role_name());
	} else {
		Logic_Player::offline_firend_handel(this->role_id(), role_id, handel);
	}
	friend_detail_->detail_change();
	this->syn_to_dbview();
	return 0;
}

int Friend::handle_friend_apply(int8_t handel, std::string &role_name) {
	if(handel >= 2) {	//全部拒绝或者同意
		handel = handel - 2;
		for(Role_Id_Time_Map::iterator it = friend_detail_->invite_list_.begin(); it != friend_detail_->invite_list_.end(); ++it) {
			handle_friend_apply(handel, it->first);
		}
		friend_detail_->invite_list_.clear();
		return 0;
	} else {//同意或者拒绝一个
		role_id_t role_id = LOGIC_MONITOR->get_role_id_by_name(role_name);
		Role_Id_Time_Map::iterator it = friend_detail_->invite_list_.find(role_id);
		if(it != friend_detail_->invite_list_.end()){
			friend_detail_->invite_list_.erase(it);
		}
		return handle_friend_apply(handel, role_id);
	}
}

int Friend::add_to_black(std::string role_name) {
	if(friend_detail_->black_list_.size() >= CONFIG_CACHE_FRIEND->find_common_config_val(FRIEND_COMM_CONF_BLACK_MAX_NUM)) {
		return ERROR_FRIEND_BLACK_MAX_NUM;
	}
	int status = 0;
	role_id_t role_id = LOGIC_MONITOR->get_role_id_by_name(role_name);
	if(role_id) {
		Friend_Info_Map::iterator it;
				//自己
		//如果在好友;删除
		it = friend_detail_->firend_list_.find(role_id);
		if(it != friend_detail_->firend_list_.end()) {
			//如果有好友度;存起来;方便以后加上好友而不丢失好友度
			if(it->second.friendship_degree > 1) {
				friend_detail_->del_firend_list_.insert(std::make_pair(role_id,it->second.friendship_degree));
			}

			friend_detail_->firend_list_.erase(it);
		}
		//如果在陌生人;删除
		it = friend_detail_->stranger_list_.find(role_id);
		if(it != friend_detail_->stranger_list_.end()) {
			friend_detail_->stranger_list_.erase(it);
		}
		Friend_Info fi;
		fill_friend_info(role_id, fi);
		fi.role_id = role_id;
		fi.role_name = role_name;
		fi.friend_type = 2;
		friend_detail_->black_list_.insert(std::make_pair(role_id, fi));
		friend_detail_->detail_change();
		this->syn_black_list_to_chat_server(1,role_id); //syn to Chat Server
		this->syn_to_dbview();
	}else{
		status = 1;
	}

	MSG_50160005 msg;
	msg.status = status;
	msg.role_name = role_name;
	Block_Buffer buf;
	msg.serialize(buf);
	THIS_SEND_TO_CLIENT(msg);

	return 0;
}

void Friend::syn_to_dbview(void) {
	for(Friend_Info_Map::iterator it = friend_detail_->firend_list_.begin(); it != friend_detail_->firend_list_.end(); ++it) {
		fill_friend_info(it->first, it->second);
	}
	DB_View::ID_Player_View_Map::iterator find_it = LOGIC_MONITOR->db_view()->id_player_view_map.find(friend_detail_->role_id);
	if (find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		find_it->second->friend_.firend_list_.clear();
		for(Friend_Info_Map::iterator fit = friend_detail_->firend_list_.begin(); fit != friend_detail_->firend_list_.end(); ++fit) {
			find_it->second->friend_.firend_list_.insert(fit->second.role_id);
		}
		find_it->second->friend_.apply_list_size = friend_detail_->apply_list_.size();
		find_it->second->friend_.invite_list_size = friend_detail_->invite_list_.size();
	}
}

void Friend::syn_black_list_to_chat_server(int8_t type, role_id_t role_id) {
	MSG_20160000 msg;

	msg.role_id = role_id;
	msg.type = type;

	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	send_to_local_chat( buf );
}

void Friend::syn_black_list_to_chat_server(Block_Buffer &buf) {
	uint16_t size = friend_detail_->black_list_.size();
	buf.write_uint16(size);
	for(Friend_Info_Map::iterator iter = friend_detail_->black_list_.begin(); iter != friend_detail_->black_list_.end(); iter++) {
		buf.write_int64(iter->first);
	}
}

int Friend::offline_add_invite(Role_Id_Time_Map &invite_list) {
	for(Role_Id_Time_Map::iterator it = invite_list.begin(); it != invite_list.end(); it++) {
		Role_Id_Time_Map::iterator my_invite_it = friend_detail_->invite_list_.find(it->first);
		if(my_invite_it != friend_detail_->invite_list_.end()){
			friend_detail_->invite_list_.erase(my_invite_it);
		}
		friend_detail_->invite_list_.insert(*it);
	}
	friend_detail_->detail_change();
	return 0;
}

int Friend::offline_handel_apply(RoleID_Int8_Map &apply_list) {
	Role_Id_Time_Map::iterator my_apply_it;
	Friend_Info_Map::iterator fit;
	for(RoleID_Int8_Map::iterator it = apply_list.begin(); it != apply_list.end(); it++) {
		role_id_t role_id = it->first;
		my_apply_it = friend_detail_->apply_list_.find(role_id);
		if(my_apply_it != friend_detail_->apply_list_.end()){
			friend_detail_->apply_list_.erase(my_apply_it);
		}
		fit = friend_detail_->firend_list_.find(role_id);
		if(it->second == 1 && fit == friend_detail_->firend_list_.end()){
			//如果在黑名单;删除
			fit = friend_detail_->black_list_.find(role_id);
			if(fit != friend_detail_->black_list_.end()) {
				friend_detail_->black_list_.erase(fit);
				this->syn_black_list_to_chat_server(0, fit->first); //syn to Chat Server
			}
			//如果在陌生人;删除
			fit = friend_detail_->stranger_list_.find(role_id);
			if(fit != friend_detail_->stranger_list_.end()) {
				friend_detail_->stranger_list_.erase(fit);
			}
			Friend_Info fi;
			fill_friend_info(role_id, fi);
			fi.role_id = role_id;
			fi.friend_type = 0;
			fi.friendship_degree = 1;
			//如果之前有删除过好友;并且有好友度
			RoleID_Friend_Map::iterator diter = friend_detail_->del_firend_list_.find(role_id);
			if(diter != friend_detail_->del_firend_list_.end()) {
				fi.friendship_degree = diter->second;
				friend_detail_->del_firend_list_.erase(diter);
			}

			friend_detail_->firend_list_.insert(std::make_pair(role_id, fi));
		}
	}
	friend_detail_->detail_change();
	this->syn_to_dbview();
	return 0;
}

int Friend::is_friend(role_id_t role_id) {
	Friend_Info_Map::iterator friend_info_it = friend_detail_->firend_list_.find(role_id);
	if(friend_info_it != friend_detail_->firend_list_.end()) {
		return friend_info_it->second.friend_type;
	}
	friend_info_it = friend_detail_->stranger_list_.find(role_id);
	if(friend_info_it != friend_detail_->stranger_list_.end()) {
		return friend_info_it->second.friend_type;
	}
	friend_info_it = friend_detail_->black_list_.find(role_id);
	if(friend_info_it != friend_detail_->black_list_.end()) {
		return friend_info_it->second.friend_type;
	}
	return -1;
}

int Friend::req_friend_set_photo(void) {
	const Friend_Head_Photo_Map &set_map = CONFIG_CACHE_FRIEND->get_set_head_photo_map();
	MSG_50160011 msg;
	msg.reset();
	Int_Int ii;
	//default by  self
	ii.reset();
	const Friend_Default_Head_Photo_Config *default_set = CONFIG_CACHE_FRIEND->find_default_head_config(player_self()->base_detail().career, player_self()->base_detail().gender);
	if(!default_set) {
		return ERROR_CONFIG_NOT_EXIST;
	}
	ii.val_1 = default_set->head_photo_id;
	ii.val_2 = 1;
	msg.photo_list.push_back(ii);

	//all set
	for(Friend_Head_Photo_Map::const_iterator iter = set_map.begin(); iter != set_map.end(); ++iter){
		ii.reset();
		ii.val_1 = iter->second.head_photo_id;
		ii.val_2 = iter->second.open_status?iter->second.open_status:(player_self()->has_hero_id(iter->second.hero_id)?1:0);
		msg.photo_list.push_back(ii);
	}
	//send to client
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Friend::req_set_head_photo(int32_t photo_id) {
	//判断是否是缺省头像
	const Friend_Default_Head_Photo_Config *default_set = CONFIG_CACHE_FRIEND->find_default_head_config(player_self()->base_detail().career, player_self()->base_detail().gender);
	if(!default_set) {
		return ERROR_CONFIG_NOT_EXIST;
	}

	//不是缺省头像
	if(photo_id != default_set->head_photo_id) {
		const Friend_Head_Photo_Config *set_map = CONFIG_CACHE_FRIEND->find_set_head_config_by_photo_id(photo_id);
		if(!set_map) {
			return ERROR_CLIENT_PARAM;
		}
		//判断是否默认可用或已经招募相应英雄
		if((set_map->open_status == 0) && !player_self()->has_hero_id(set_map->hero_id)) {
			return ERROR_CLIENT_PARAM;
		}
	}
	friend_detail_->head_photo_id = photo_id;
	// 设置头像
	player_self()->set_head_id(photo_id);
	friend_detail_->detail_change();
	{//逻辑服同步头像到场景服
		MSG_20200023 syn_msg;
		syn_msg.reset();
		syn_msg.head_id = photo_id;
		THIS_SEND_TO_SCENE(syn_msg);
	}
	//同步到DB_VIEW
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->base_detail().role_id);
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view", player_self()->base_detail().role_id);
		return ERROR_INNER_SERVER;
	}
	iter->second->friend_.head_photo_id = photo_id;
	//res client
	MSG_50160012 msg;
	msg.reset();
	msg.photo_id = photo_id;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Friend::req_role_online_status(Role_Vec stranger_id_list, Role_Vec recent_id_list) {
	MSG_50160010 msg;
	msg.reset();
	Int_Role_Id role_online_status;
	for(Role_Vec::iterator it = stranger_id_list.begin(); it != stranger_id_list.end(); ++it) {
		role_id_t role_id = *it;
		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			MSG_USER("player(role_id=%ld) is not in db_view", role_id);
			return ERROR_FRIEND_ROLE_NOT_EXIST;
		}
		role_online_status.reset();
		role_online_status.role_id = role_id;
		role_online_status.int_value = iter->second->account_mode;
		msg.stranger_online.push_back(role_online_status);
	}

	for(Role_Vec::iterator it = recent_id_list.begin(); it != recent_id_list.end(); ++it) {
		role_id_t role_id = *it;
		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			MSG_USER("player(role_id=%ld) is not in db_view", role_id);
			return ERROR_FRIEND_ROLE_NOT_EXIST;
		}
		role_online_status.reset();
		role_online_status.role_id = role_id;
		role_online_status.int_value = iter->second->account_mode;
		msg.recent_online.push_back(role_online_status);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Friend::fill_friend_info(role_id_t role_id, Friend_Info& fi) {
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		MSG_USER("player(role_id=%ld) is not in db_view", role_id);
		return;
	}
	fi.role_name = iter->second->role_name;
	fi.sex = iter->second->gender;
	fi.career = iter->second->career;/*职业*/
	fi.level = iter->second->level;/*等级*/
	fi.faction_name = iter->second->gang_name;/*帮派名称*/
	fi.is_black = (fi.friend_type == 2)?1:0; /*是否在黑名单*/
	//fi.is_on_line = iter->second->account_mode;/*是否在线*/ //不知道是谁的坑;也不知道从什么时候开始;这个值获取不了在线状态了
	fi.is_on_line = 0;
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if (player) {
		fi.is_on_line = 1;
	}

	fi.team_id = 0;/*组队ID，如果没组队，ID为0*/
	fi.vip = iter->second->vip();/*VIP等级*/
	fi.head_photo_id = iter->second->friend_.head_photo_id;
	if(fi.head_photo_id == 0) {
		const Friend_Default_Head_Photo_Config *default_set = CONFIG_CACHE_FRIEND->find_default_head_config(iter->second->career, iter->second->gender);
		if(default_set) {
			fi.head_photo_id = default_set->head_photo_id;
		}
	}
}

int Friend::req_friend_base_info_list(void) {
	MSG_50160014 msg;
	msg.reset();
	Friend_Base_Info friend_base;
	for(Friend_Info_Map::iterator it = friend_detail_->firend_list_.begin(); it != friend_detail_->firend_list_.end(); ++it) {
		friend_base.reset();
		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(it->second.role_id);
		if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			MSG_USER("player(role_id=%ld) is not in db_view", it->second.role_id);
			return ERROR_INNER_SERVER;
		}

		friend_base.role_id = iter->second->role_id;
		friend_base.role_name = iter->second->role_name;

		msg.friend_list.push_back(friend_base);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Friend::get_head_photo_id(void) {
	int32_t head_photo_id = player_self()->friend_detail().head_photo_id;
	if(head_photo_id == 0) {
		//缺省头像
		const Friend_Default_Head_Photo_Config *default_set = CONFIG_CACHE_FRIEND->find_default_head_config(player_self()->base_detail().career, player_self()->base_detail().gender);
		if(!default_set) {
			return 0;
		}
		friend_detail_->head_photo_id = default_set->head_photo_id;
		//同步到DB_VIEW
		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->base_detail().role_id);
		if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			MSG_USER("player(role_id=%ld) is not in db_view", player_self()->base_detail().role_id);
		} else {
			iter->second->friend_.head_photo_id = default_set->head_photo_id;
		}
		//update head_photo_id
		head_photo_id = default_set->head_photo_id;
		// 设置头像
		player_self()->set_head_id(head_photo_id);
	} else {
		//缺省头像
		const Friend_Default_Head_Photo_Config *default_set = CONFIG_CACHE_FRIEND->find_default_head_config(player_self()->base_detail().career, player_self()->base_detail().gender);
		if(!default_set) {
			return head_photo_id;
		}
		//不是缺省头像
		if(head_photo_id != default_set->head_photo_id) {
			const Friend_Head_Photo_Config *set_map = CONFIG_CACHE_FRIEND->find_set_head_config_by_photo_id(head_photo_id);
			if(!set_map) {
				head_photo_id = default_set->head_photo_id;
			} else if((set_map->open_status == 0) && !player_self()->has_hero_id(set_map->hero_id)) {//判断是否默认可用或已经招募相应英雄
				head_photo_id = default_set->head_photo_id;
			}
		}
		if(player_self()->friend_detail().head_photo_id != head_photo_id) {
			friend_detail_->head_photo_id = head_photo_id;
			//同步到DB_VIEW
			DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(player_self()->base_detail().role_id);
			if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
				MSG_USER("player(role_id=%ld) is not in db_view", player_self()->base_detail().role_id);
			} else {
				iter->second->friend_.head_photo_id = head_photo_id;
			}
			// 设置头像
			player_self()->set_head_id(head_photo_id);
		}
	}
	if(player_self()->head_id() != (uint32_t)head_photo_id) {
		player_self()->set_head_id(head_photo_id);
	}
	return head_photo_id;
}

void Friend::friend_listen_level_up(const int level) {
	if(level > 30) {
		return;
	}
	if((level%10 == 0 && level != 10) || level == 5) {
		MSG_82200003 msg;
		int num = rand()%6 + 5;
		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.begin();
		for (;iter != LOGIC_MONITOR->db_view()->id_player_view_map.end() && num > 0; ++iter) {
			Player_DB_View *player = iter->second;
			if(player
			   && abs(player->level - player_self()->level()) <= 3  //等级差
			   && player->role_id != player_self()->role_id()		  //不是自己
			   && player_self()->is_friend(player->role_id)){		  //还不是好友
				Role_Info role;
				role.role_id = player->role_id;
				role.role_name = player->role_name;
				role.career = player->career;
				role.combat = player->force;
				role.gender = player->gender;
				role.head_photo_id = player->friend_.head_photo_id;
				role.level = player->level;
				role.vip_level = player->vip_level;
				role.guild_name = player->gang_name;
				msg.roles.push_back(role);
				--num;
			}
		}
		THIS_SEND_TO_CLIENT(msg);
	}
}

void Friend::add_friendship_degree(role_id_t role_id, uint32_t num) {
	Friend_Info_Map::iterator it = friend_detail_->firend_list_.find(role_id);
	if(it != friend_detail_->firend_list_.end()) {
		it->second.friendship_degree += num;
		friend_detail_->detail_change();
	}
}
