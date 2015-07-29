/*
* Generate by devtool
*/

#ifndef _DEFAULT_H_
#define _DEFAULT_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求添加R2邀请邮箱
*/
struct MSG_10106800  {
	std::vector<std::string> mails;
	MSG_10106800(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __mails_vec_size = mails.size();
		w.write_uint16(__mails_vec_size);
		for(uint16_t i = 0; i < __mails_vec_size; ++i) {
				w.write_string(mails[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __mails_vec_size;
		if(r.read_uint16(__mails_vec_size)  )
				return -1;
		mails.reserve(__mails_vec_size);
		for(uint16_t i = 0; i < __mails_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				mails.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		mails.clear();
	}
};

/*
请求添加R2邀请邮箱(返回)
*/
struct MSG_50106800 : public Base_Msg  {
	std::vector<std::string> mails;
	MSG_50106800(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __mails_vec_size = mails.size();
		w.write_uint16(__mails_vec_size);
		for(uint16_t i = 0; i < __mails_vec_size; ++i) {
				w.write_string(mails[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __mails_vec_size;
		if(r.read_uint16(__mails_vec_size)  )
				return -1;
		mails.reserve(__mails_vec_size);
		for(uint16_t i = 0; i < __mails_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				mails.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106800;
		mails.clear();
	}
};

/*
请求获取R2邀请邮箱信息(返回)
*/
struct MSG_50106801 : public Base_Msg  {
	std::vector<R2_Mail_Invite_Info> info;
	MSG_50106801(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __info_vec_size = info.size();
		w.write_uint16(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __info_vec_size;
		if(r.read_uint16(__info_vec_size)  )
				return -1;
		info.reserve(__info_vec_size);
		for(uint16_t i = 0; i < __info_vec_size; ++i) {
				R2_Mail_Invite_Info v;
				if(v.deserialize(r))
						return -1;
				info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106801;
		info.clear();
	}
};

/*
请求获取R2邀请邮箱信息
*/
struct MSG_10106801  {
	MSG_10106801(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};



#endif