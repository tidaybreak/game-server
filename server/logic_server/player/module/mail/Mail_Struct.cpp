/*
 * Mail_Struct.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: zhouyajun
 */

#include "Mail_Struct.h"
#include "DB_Operator.h"

Mail_Send_Info::Mail_Send_Info(role_id_t r_id, role_id_t s_id, int s_type, std::string &s_name, std::string &title, std::string &content, int s_gold) :
receiver_id(r_id), sender_id(s_id), send_type(s_type), sender_name(s_name), title(title), content(content), gold(s_gold) {};

Mail_Send_Info::Mail_Send_Info(role_id_t r_id, role_id_t s_id, int s_type, std::string &s_name, std::string &title, std::string &content) :
receiver_id(r_id), sender_id(s_id), send_type(s_type), sender_name(s_name), title(title), content(content), gold(0) {};

//Mail_Send_Info::Mail_Send_Info(role_id_t r_id, role_id_t s_id, int s_type, std::string &s_name, std::string &title, std::string &content, Item_Vec &item_vector) :
//receiver_id(r_id), sender_id(s_id), send_type(s_type), sender_name(s_name), title(title), content(content), gold(0), item_vector(item_vector) {};

void Mail_Send_Info::serialize(Block_Buffer & w) const {
	w.write_int64(receiver_id);
	w.write_int64(sender_id);
	w.write_int32(send_type);
	w.write_string(sender_name);
	w.write_string(title);
	w.write_string(content);
	w.write_int32(show_send_time.sec());
	uint16_t __item_vector_vec_size = item_vector.size();
	w.write_uint16(__item_vector_vec_size);
	for(uint16_t i = 0; i < __item_vector_vec_size; ++i) {
		item_vector[i].serialize(w);
	}
	w.write_int32(gold);
}

int Mail_Send_Info::deserialize(Block_Buffer & r) {
	int send_time = 0;
	if( r.read_int64(receiver_id)  ||  r.read_int64(sender_id)  ||  r.read_int32(send_type)  ||  r.read_string(sender_name)  ||  r.read_string(title)  ||  r.read_string(content) || r.read_int32(send_time) )
			return -1;
	show_send_time.sec(send_time);

 	uint16_t __item_vector_vec_size;
	if(r.read_uint16(__item_vector_vec_size) )
			return -1;
	item_vector.reserve(__item_vector_vec_size);
	for(uint16_t i = 0; i < __item_vector_vec_size; ++i) {
			Item_Detail v;
			if(v.deserialize(r))
					return -1;
			item_vector.push_back(v);
	}
	r.read_int32(gold);
	return 0;
}

void Mail_Send_Info::reset(void) {
	receiver_id = 0;
	sender_id = 0;
	send_type = 0;
	sender_name.clear();
	title.clear();
	content.clear();
	item_vector.clear();
	show_send_time = Time_Value::zero;
	gold = 0;
}

void Mail_Data::reset() {
	mail_type = 0;
	mail_id = 0;
	have_read = 0;
	gold = 0;
	mail_title.clear();
	mail_content.clear();
	sender_name.clear();
	show_send_time = Time_Value::zero;
	real_send_time = Time_Value::zero;
}

Mail_Detail::Mail_Detail() {
    reset();
}

Mail_Detail::~Mail_Detail() {

}

void Mail_Detail::reset() {
//	role_id = 0;
	mail_page_count = 0;
	mail_current_page = 1;
	role_id = 0;
	mail_all_count = 0;
	mail_state_page = 0;
	mail_map.clear();
	sys_mail_map.clear();
	last_read_sys_mail_time = Time_Value::zero;
	max_gold = 0;
	//not_read_count = 0;
}

int Mail_Detail::serialize(Block_Buffer &buffer) const {

	return 0;
}

int Mail_Detail::deserialize(Block_Buffer &buffer) {

	return 0;
}

int Mail_Detail::save(void) {
	if (role_id)
		MONGO_OPERATOR->save_mail_detail(*this);
	return 0;
}

int Mail_Detail::load(void) {
	return MONGO_OPERATOR->load_mail_detail(*this);
}
