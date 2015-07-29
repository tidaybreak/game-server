/*
* Generate by devtool
*/

#ifndef _MSG_MAIL_H_
#define _MSG_MAIL_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取邮件列表
*/
struct MSG_10150000  {
	int8_t page_count;
	int8_t require_mail_state;
	MSG_10150000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(page_count);
		w.write_int8(require_mail_state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(page_count)  ||  r.read_int8(require_mail_state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		page_count = 0;
		require_mail_state = 0;
	}
};

/*
查看邮件
*/
struct MSG_10150001  {
	int32_t mail_id;
	MSG_10150001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mail_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mail_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mail_id = 0;
	}
};

/*
发送邮件
*/
struct MSG_10150002  {
	Mail_Info mail_info;
	MSG_10150002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		mail_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( mail_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mail_info.reset();
	}
};

/*
获取邮件列表返回
*/
struct MSG_50150000 : public Base_Msg  {
	std::vector<Mail_Info> mail_info_list;
	int8_t current_page;
	int8_t total_page_num;
	int8_t one_key_pickup;
	MSG_50150000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __mail_info_list_vec_size = mail_info_list.size();
		w.write_uint16(__mail_info_list_vec_size);
		for(uint16_t i = 0; i < __mail_info_list_vec_size; ++i) {
				mail_info_list[i].serialize(w);
		}

		w.write_int8(current_page);
		w.write_int8(total_page_num);
		w.write_int8(one_key_pickup);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __mail_info_list_vec_size;
		if(r.read_uint16(__mail_info_list_vec_size)  )
				return -1;
		mail_info_list.reserve(__mail_info_list_vec_size);
		for(uint16_t i = 0; i < __mail_info_list_vec_size; ++i) {
				Mail_Info v;
				if(v.deserialize(r))
						return -1;
				mail_info_list.push_back(v);
		}

		if( r.read_int8(current_page)  ||  r.read_int8(total_page_num)  ||  r.read_int8(one_key_pickup)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50150000;
		mail_info_list.clear();
		current_page = 0;
		total_page_num = 0;
		one_key_pickup = 0;
	}
};

/*
查看邮件返回
*/
struct MSG_50150001 : public Base_Msg  {
	int32_t check_back_id;
	MSG_50150001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(check_back_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(check_back_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50150001;
		check_back_id = 0;
	}
};

/*
发送邮件返回
*/
struct MSG_50150002 : public Base_Msg  {
	int32_t verify_code;
	MSG_50150002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(verify_code);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(verify_code)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50150002;
		verify_code = 0;
	}
};

/*
批量删除邮件
*/
struct MSG_10150005  {
	std::vector<int32_t> muti_mail_id;
	MSG_10150005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __muti_mail_id_vec_size = muti_mail_id.size();
		w.write_uint16(__muti_mail_id_vec_size);
		for(uint16_t i = 0; i < __muti_mail_id_vec_size; ++i) {
				w.write_int32(muti_mail_id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __muti_mail_id_vec_size;
		if(r.read_uint16(__muti_mail_id_vec_size)  )
				return -1;
		muti_mail_id.reserve(__muti_mail_id_vec_size);
		for(uint16_t i = 0; i < __muti_mail_id_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				muti_mail_id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		muti_mail_id.clear();
	}
};

/*
批量删除邮件列表返回
*/
struct MSG_50150005 : public Base_Msg  {
	std::vector<int32_t> mutil_mail_id;
	MSG_50150005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __mutil_mail_id_vec_size = mutil_mail_id.size();
		w.write_uint16(__mutil_mail_id_vec_size);
		for(uint16_t i = 0; i < __mutil_mail_id_vec_size; ++i) {
				w.write_int32(mutil_mail_id[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __mutil_mail_id_vec_size;
		if(r.read_uint16(__mutil_mail_id_vec_size)  )
				return -1;
		mutil_mail_id.reserve(__mutil_mail_id_vec_size);
		for(uint16_t i = 0; i < __mutil_mail_id_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				mutil_mail_id.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50150005;
		mutil_mail_id.clear();
	}
};

/*
请求收取附件
*/
struct MSG_10150003  {
	int32_t mail_id;
	std::vector<uint32_t> item_vector_id;
	uint8_t type;
	MSG_10150003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mail_id);
		uint16_t __item_vector_id_vec_size = item_vector_id.size();
		w.write_uint16(__item_vector_id_vec_size);
		for(uint16_t i = 0; i < __item_vector_id_vec_size; ++i) {
				w.write_uint32(item_vector_id[i]);
		}

		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mail_id)  )
				return -1;
 		uint16_t __item_vector_id_vec_size;
		if(r.read_uint16(__item_vector_id_vec_size)  )
				return -1;
		item_vector_id.reserve(__item_vector_id_vec_size);
		for(uint16_t i = 0; i < __item_vector_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				item_vector_id.push_back(v);
		}

		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mail_id = 0;
		item_vector_id.clear();
		type = 0;
	}
};

/*
请求收取附件返回
*/
struct MSG_50150003 : public Base_Msg  {
	std::vector<uint32_t> item_vector_id;
	uint8_t type;
	MSG_50150003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __item_vector_id_vec_size = item_vector_id.size();
		w.write_uint16(__item_vector_id_vec_size);
		for(uint16_t i = 0; i < __item_vector_id_vec_size; ++i) {
				w.write_uint32(item_vector_id[i]);
		}

		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __item_vector_id_vec_size;
		if(r.read_uint16(__item_vector_id_vec_size)  )
				return -1;
		item_vector_id.reserve(__item_vector_id_vec_size);
		for(uint16_t i = 0; i < __item_vector_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				item_vector_id.push_back(v);
		}

		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50150003;
		item_vector_id.clear();
		type = 0;
	}
};

/*
请求一键收取附件
*/
struct MSG_10150004  {
	MSG_10150004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求一键收取附件(返回)
*/
struct MSG_50150004 : public Base_Msg  {
	std::vector<int32_t> mail_id;
	int8_t keep;
	MSG_50150004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __mail_id_vec_size = mail_id.size();
		w.write_uint16(__mail_id_vec_size);
		for(uint16_t i = 0; i < __mail_id_vec_size; ++i) {
				w.write_int32(mail_id[i]);
		}

		w.write_int8(keep);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __mail_id_vec_size;
		if(r.read_uint16(__mail_id_vec_size)  )
				return -1;
		mail_id.reserve(__mail_id_vec_size);
		for(uint16_t i = 0; i < __mail_id_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				mail_id.push_back(v);
		}

		if( r.read_int8(keep)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50150004;
		mail_id.clear();
		keep = 0;
	}
};



#endif