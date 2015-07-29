/*
* Generate by devtool
*/

#ifndef _MSG_PHP_H_
#define _MSG_PHP_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
 账号和角色封禁
*/
struct MSG_30100001  {
	std::string account;
	int64_t role_id;
	uint32_t ban_type;
	uint32_t unban_time;
	MSG_30100001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(account);
		w.write_int64(role_id);
		w.write_uint32(ban_type);
		w.write_uint32(unban_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(account)  ||  r.read_int64(role_id)  ||  r.read_uint32(ban_type)  ||  r.read_uint32(unban_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		account.clear();
		role_id = 0;
		ban_type = 0;
		unban_time = 0;
	}
};

/*
 IP地址封禁或解除
*/
struct MSG_30100010  {
	std::string ip_address;
	int8_t type;
	int32_t unban_time;
	MSG_30100010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(ip_address);
		w.write_int8(type);
		w.write_int32(unban_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(ip_address)  ||  r.read_int8(type)  ||  r.read_int32(unban_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		ip_address.clear();
		type = 0;
		unban_time = 0;
	}
};

/*
PHP通知处理结果(返回)
*/
struct MSG_30100000  {
	int32_t status;
	MSG_30100000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		status = 0;
	}
};

/*
PHP通知商城信息变动
备注：商城物品列表信息变动
*/
struct MSG_30100002  {
	std::vector<int32_t> diff_tabs;
	std::vector<Goods_Info> goods_list;
	std::vector<Mall_Promo_Info> promo_goods_list;
	std::vector<Mall_Promo_Info> banner_goods_list;
	MSG_30100002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __diff_tabs_vec_size = diff_tabs.size();
		w.write_uint16(__diff_tabs_vec_size);
		for(uint16_t i = 0; i < __diff_tabs_vec_size; ++i) {
				w.write_int32(diff_tabs[i]);
		}

		uint16_t __goods_list_vec_size = goods_list.size();
		w.write_uint16(__goods_list_vec_size);
		for(uint16_t i = 0; i < __goods_list_vec_size; ++i) {
				goods_list[i].serialize(w);
		}

		uint16_t __promo_goods_list_vec_size = promo_goods_list.size();
		w.write_uint16(__promo_goods_list_vec_size);
		for(uint16_t i = 0; i < __promo_goods_list_vec_size; ++i) {
				promo_goods_list[i].serialize(w);
		}

		uint16_t __banner_goods_list_vec_size = banner_goods_list.size();
		w.write_uint16(__banner_goods_list_vec_size);
		for(uint16_t i = 0; i < __banner_goods_list_vec_size; ++i) {
				banner_goods_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __diff_tabs_vec_size;
		if(r.read_uint16(__diff_tabs_vec_size)  )
				return -1;
		diff_tabs.reserve(__diff_tabs_vec_size);
		for(uint16_t i = 0; i < __diff_tabs_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				diff_tabs.push_back(v);
		}

		uint16_t __goods_list_vec_size;
		if(r.read_uint16(__goods_list_vec_size)  )
				return -1;
		goods_list.reserve(__goods_list_vec_size);
		for(uint16_t i = 0; i < __goods_list_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				goods_list.push_back(v);
		}

		uint16_t __promo_goods_list_vec_size;
		if(r.read_uint16(__promo_goods_list_vec_size)  )
				return -1;
		promo_goods_list.reserve(__promo_goods_list_vec_size);
		for(uint16_t i = 0; i < __promo_goods_list_vec_size; ++i) {
				Mall_Promo_Info v;
				if(v.deserialize(r))
						return -1;
				promo_goods_list.push_back(v);
		}

		uint16_t __banner_goods_list_vec_size;
		if(r.read_uint16(__banner_goods_list_vec_size)  )
				return -1;
		banner_goods_list.reserve(__banner_goods_list_vec_size);
		for(uint16_t i = 0; i < __banner_goods_list_vec_size; ++i) {
				Mall_Promo_Info v;
				if(v.deserialize(r))
						return -1;
				banner_goods_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		diff_tabs.clear();
		goods_list.clear();
		promo_goods_list.clear();
		banner_goods_list.clear();
	}
};

/*
PHP通知在线充值
*/
struct MSG_30100003  {
	int32_t pay_num;
	MSG_30100003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(pay_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(pay_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		pay_num = 0;
	}
};

/*
PHP通知发送邮件
*/
struct MSG_30100004  {
	std::vector<int64_t> role_ids;
	int32_t type;
	int32_t career;
	int32_t start_level;
	int32_t end_level;
	std::string sender;
	std::string title;
	std::string contents;
	std::vector<Item_Info> item;
	int32_t gold;
	MSG_30100004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __role_ids_vec_size = role_ids.size();
		w.write_uint16(__role_ids_vec_size);
		for(uint16_t i = 0; i < __role_ids_vec_size; ++i) {
				w.write_int64(role_ids[i]);
		}

		w.write_int32(type);
		w.write_int32(career);
		w.write_int32(start_level);
		w.write_int32(end_level);
		w.write_string(sender);
		w.write_string(title);
		w.write_string(contents);
		uint16_t __item_vec_size = item.size();
		w.write_uint16(__item_vec_size);
		for(uint16_t i = 0; i < __item_vec_size; ++i) {
				item[i].serialize(w);
		}

		w.write_int32(gold);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __role_ids_vec_size;
		if(r.read_uint16(__role_ids_vec_size)  )
				return -1;
		role_ids.reserve(__role_ids_vec_size);
		for(uint16_t i = 0; i < __role_ids_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				role_ids.push_back(v);
		}

		if( r.read_int32(type)  ||  r.read_int32(career)  ||  r.read_int32(start_level)  ||  r.read_int32(end_level)  ||  r.read_string(sender)  ||  r.read_string(title)  ||  r.read_string(contents)  )
				return -1;
 		uint16_t __item_vec_size;
		if(r.read_uint16(__item_vec_size)  )
				return -1;
		item.reserve(__item_vec_size);
		for(uint16_t i = 0; i < __item_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				item.push_back(v);
		}

		if( r.read_int32(gold)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_ids.clear();
		type = 0;
		career = 0;
		start_level = 0;
		end_level = 0;
		sender.clear();
		title.clear();
		contents.clear();
		item.clear();
		gold = 0;
	}
};

/*
账号和角色禁言
*/
struct MSG_30100005  {
	std::string account;
	int64_t role_id;
	uint8_t ban_type;
	uint32_t unban_time;
	MSG_30100005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(account);
		w.write_int64(role_id);
		w.write_uint8(ban_type);
		w.write_uint32(unban_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(account)  ||  r.read_int64(role_id)  ||  r.read_uint8(ban_type)  ||  r.read_uint32(unban_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		account.clear();
		role_id = 0;
		ban_type = 0;
		unban_time = 0;
	}
};

/*
PHP请求获取统计数据
*/
struct MSG_30100006  {
	uint32_t type;
	MSG_30100006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
PHP请求获取统计数据(返回)
*/
struct MSG_30500006  {
	uint32_t num;
	MSG_30500006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		num = 0;
	}
};

/*
PHP发送你提我改回复
*/
struct MSG_30100007  {
	std::string sender;
	std::string title;
	std::string contents;
	int32_t gold;
	int64_t role_id;
	MSG_30100007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(sender);
		w.write_string(title);
		w.write_string(contents);
		w.write_int32(gold);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(sender)  ||  r.read_string(title)  ||  r.read_string(contents)  ||  r.read_int32(gold)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		sender.clear();
		title.clear();
		contents.clear();
		gold = 0;
		role_id = 0;
	}
};

/*
PHP通知GM公告
*/
struct MSG_30100008  {
	int32_t id;
	int8_t type;
	int32_t stime;
	int32_t etime;
	int32_t step;
	std::string contents;
	MSG_30100008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int8(type);
		w.write_int32(stime);
		w.write_int32(etime);
		w.write_int32(step);
		w.write_string(contents);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int8(type)  ||  r.read_int32(stime)  ||  r.read_int32(etime)  ||  r.read_int32(step)  ||  r.read_string(contents)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
		stime = 0;
		etime = 0;
		step = 0;
		contents.clear();
	}
};

/*
PHP请求增加激活码解码KEY
*/
struct MSG_30100011  {
	std::vector<std::string> key;
	MSG_30100011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __key_vec_size = key.size();
		w.write_uint16(__key_vec_size);
		for(uint16_t i = 0; i < __key_vec_size; ++i) {
				w.write_string(key[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __key_vec_size;
		if(r.read_uint16(__key_vec_size)  )
				return -1;
		key.reserve(__key_vec_size);
		for(uint16_t i = 0; i < __key_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				key.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		key.clear();
	}
};

/*
PHP通知Facebook分享成功
*/
struct MSG_30100012  {
	uint16_t type;
	MSG_30100012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
PHP通知内部充值
*/
struct MSG_30100009  {
	int32_t num;
	MSG_30100009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		num = 0;
	}
};

/*
PHP后台通知活动开启
*/
struct MSG_30100013  {
	uint32_t id;
	uint32_t start_time;
	uint32_t end_time;
	MSG_30100013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(start_time);
		w.write_uint32(end_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(start_time)  ||  r.read_uint32(end_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		start_time = 0;
		end_time = 0;
	}
};

/*
PHP后台通知活动结束
*/
struct MSG_30100014  {
	uint32_t id;
	uint32_t end_time;
	MSG_30100014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(end_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(end_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		end_time = 0;
	}
};

/*
PHP通知活动兑换商店道具变动
*/
struct MSG_30100015  {
	std::vector<Activity_Exchange_Mall_Goods_Info> goods;
	MSG_30100015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __goods_vec_size = goods.size();
		w.write_uint16(__goods_vec_size);
		for(uint16_t i = 0; i < __goods_vec_size; ++i) {
				goods[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __goods_vec_size;
		if(r.read_uint16(__goods_vec_size)  )
				return -1;
		goods.reserve(__goods_vec_size);
		for(uint16_t i = 0; i < __goods_vec_size; ++i) {
				Activity_Exchange_Mall_Goods_Info v;
				if(v.deserialize(r))
						return -1;
				goods.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		goods.clear();
	}
};

/*
PHP后台通知坐骑融合时间
*/
struct MSG_30100016  {
	int32_t mount_id;
	int64_t begin_time;
	int64_t end_time;
	MSG_30100016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_int64(begin_time);
		w.write_int64(end_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_int64(begin_time)  ||  r.read_int64(end_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mount_id = 0;
		begin_time = 0;
		end_time = 0;
	}
};



#endif