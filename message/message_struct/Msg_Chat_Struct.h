/*
* Generate by devtool
*/

#ifndef _MSG_CHAT_H_
#define _MSG_CHAT_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
本服聊天请求
*/
struct MSG_10300000  {
	uint8_t chat_type;
	int32_t system_type;
	std::string dest_name;
	int64_t dest_id;
	std::vector<Chat_Content> content;
	MSG_10300000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(chat_type);
		w.write_int32(system_type);
		w.write_string(dest_name);
		w.write_int64(dest_id);
		uint16_t __content_vec_size = content.size();
		w.write_uint16(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				content[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(chat_type)  ||  r.read_int32(system_type)  ||  r.read_string(dest_name)  ||  r.read_int64(dest_id)  )
				return -1;
 		uint16_t __content_vec_size;
		if(r.read_uint16(__content_vec_size)  )
				return -1;
		content.reserve(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				Chat_Content v;
				if(v.deserialize(r))
						return -1;
				content.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		chat_type = 0;
		system_type = 0;
		dest_name.clear();
		dest_id = 0;
		content.clear();
	}
};

/*
私聊聊天记录请求
*/
struct MSG_10300001  {
	int64_t role_id;
	MSG_10300001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
聊天记录请求（返回）
*/
struct MSG_50300001 : public Base_Msg  {
	std::vector<Chat_Record> chat_record;
	int64_t role_id;
	MSG_50300001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __chat_record_vec_size = chat_record.size();
		w.write_uint16(__chat_record_vec_size);
		for(uint16_t i = 0; i < __chat_record_vec_size; ++i) {
				chat_record[i].serialize(w);
		}

		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __chat_record_vec_size;
		if(r.read_uint16(__chat_record_vec_size)  )
				return -1;
		chat_record.reserve(__chat_record_vec_size);
		for(uint16_t i = 0; i < __chat_record_vec_size; ++i) {
				Chat_Record v;
				if(v.deserialize(r))
						return -1;
				chat_record.push_back(v);
		}

		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50300001;
		chat_record.clear();
		role_id = 0;
	}
};

/*
公会聊天记录请求
*/
struct MSG_10300002  {
	MSG_10300002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
发起输入状态
*/
struct MSG_10300003  {
	int64_t role_id;
	int8_t status;
	MSG_10300003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		status = 0;
	}
};

/*
推送输入状态(返回)
*/
struct MSG_50300003 : public Base_Msg  {
	int64_t role_id;
	int8_t status;
	MSG_50300003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50300003;
		role_id = 0;
		status = 0;
	}
};

/*
请求同场景聊天
*/
struct MSG_10200011  {
	std::vector<Chat_Content> content;
	MSG_10200011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __content_vec_size = content.size();
		w.write_uint16(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				content[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __content_vec_size;
		if(r.read_uint16(__content_vec_size)  )
				return -1;
		content.reserve(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				Chat_Content v;
				if(v.deserialize(r))
						return -1;
				content.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		content.clear();
	}
};



#endif