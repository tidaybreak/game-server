/*
* Generate by devtool
*/

#ifndef _MSG_MALL_H_
#define _MSG_MALL_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取商品列表
*/
struct MSG_10151000  {
	MSG_10151000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
返回商品列表
*/
struct MSG_50151000 : public Base_Msg  {
	std::vector<Goods_Info> goods_info;
	std::vector<Goods_Info> promo_goods_info;
	int32_t promo_start_time;
	int32_t promo_end_time;
	std::vector<Goods_Info> banner_goods_info;
	int32_t banner_start_time;
	int32_t banner_end_time;
	std::vector<uint32_t> act_ids;
	MSG_50151000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __goods_info_vec_size = goods_info.size();
		w.write_uint16(__goods_info_vec_size);
		for(uint16_t i = 0; i < __goods_info_vec_size; ++i) {
				goods_info[i].serialize(w);
		}

		uint16_t __promo_goods_info_vec_size = promo_goods_info.size();
		w.write_uint16(__promo_goods_info_vec_size);
		for(uint16_t i = 0; i < __promo_goods_info_vec_size; ++i) {
				promo_goods_info[i].serialize(w);
		}

		w.write_int32(promo_start_time);
		w.write_int32(promo_end_time);
		uint16_t __banner_goods_info_vec_size = banner_goods_info.size();
		w.write_uint16(__banner_goods_info_vec_size);
		for(uint16_t i = 0; i < __banner_goods_info_vec_size; ++i) {
				banner_goods_info[i].serialize(w);
		}

		w.write_int32(banner_start_time);
		w.write_int32(banner_end_time);
		uint16_t __act_ids_vec_size = act_ids.size();
		w.write_uint16(__act_ids_vec_size);
		for(uint16_t i = 0; i < __act_ids_vec_size; ++i) {
				w.write_uint32(act_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __goods_info_vec_size;
		if(r.read_uint16(__goods_info_vec_size)  )
				return -1;
		goods_info.reserve(__goods_info_vec_size);
		for(uint16_t i = 0; i < __goods_info_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				goods_info.push_back(v);
		}

		uint16_t __promo_goods_info_vec_size;
		if(r.read_uint16(__promo_goods_info_vec_size)  )
				return -1;
		promo_goods_info.reserve(__promo_goods_info_vec_size);
		for(uint16_t i = 0; i < __promo_goods_info_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				promo_goods_info.push_back(v);
		}

		if( r.read_int32(promo_start_time)  ||  r.read_int32(promo_end_time)  )
				return -1;
 		uint16_t __banner_goods_info_vec_size;
		if(r.read_uint16(__banner_goods_info_vec_size)  )
				return -1;
		banner_goods_info.reserve(__banner_goods_info_vec_size);
		for(uint16_t i = 0; i < __banner_goods_info_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				banner_goods_info.push_back(v);
		}

		if( r.read_int32(banner_start_time)  ||  r.read_int32(banner_end_time)  )
				return -1;
 		uint16_t __act_ids_vec_size;
		if(r.read_uint16(__act_ids_vec_size)  )
				return -1;
		act_ids.reserve(__act_ids_vec_size);
		for(uint16_t i = 0; i < __act_ids_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				act_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50151000;
		goods_info.clear();
		promo_goods_info.clear();
		promo_start_time = 0;
		promo_end_time = 0;
		banner_goods_info.clear();
		banner_start_time = 0;
		banner_end_time = 0;
		act_ids.clear();
	}
};

/*
购买商品
*/
struct MSG_10151001  {
	int32_t id;
	int32_t tabs;
	int32_t amount;
	std::string role_name;
	int32_t buy_type;
	int32_t helper_fast_use;
	MSG_10151001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(tabs);
		w.write_int32(amount);
		w.write_string(role_name);
		w.write_int32(buy_type);
		w.write_int32(helper_fast_use);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(tabs)  ||  r.read_int32(amount)  ||  r.read_string(role_name)  ||  r.read_int32(buy_type)  ||  r.read_int32(helper_fast_use)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		tabs = 0;
		amount = 0;
		role_name.clear();
		buy_type = 0;
		helper_fast_use = 0;
	}
};

/*
购买商品(返回)
*/
struct MSG_50151001 : public Base_Msg  {
	int32_t status;
	int16_t helper_fast_use;
	int32_t id;
	MSG_50151001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(status);
		w.write_int16(helper_fast_use);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(status)  ||  r.read_int16(helper_fast_use)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50151001;
		status = 0;
		helper_fast_use = 0;
		id = 0;
	}
};

/*
一键购买商品
*/
struct MSG_10151002  {
	int32_t tabs;
	std::vector<int32_t> ids;
	MSG_10151002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(tabs);
		uint16_t __ids_vec_size = ids.size();
		w.write_uint16(__ids_vec_size);
		for(uint16_t i = 0; i < __ids_vec_size; ++i) {
				w.write_int32(ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(tabs)  )
				return -1;
 		uint16_t __ids_vec_size;
		if(r.read_uint16(__ids_vec_size)  )
				return -1;
		ids.reserve(__ids_vec_size);
		for(uint16_t i = 0; i < __ids_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		tabs = 0;
		ids.clear();
	}
};

/*
一键购买商品(返回)
*/
struct MSG_50151002 : public Base_Msg  {
	int32_t status;
	MSG_50151002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50151002;
		status = 0;
	}
};

/*
请求快速购买物品
*/
struct MSG_10151003  {
	int32_t item_id;
	uint32_t amount;
	uint8_t buy_type;
	int8_t helper_fast_use;
	MSG_10151003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_uint32(amount);
		w.write_uint8(buy_type);
		w.write_int8(helper_fast_use);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_uint32(amount)  ||  r.read_uint8(buy_type)  ||  r.read_int8(helper_fast_use)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
		amount = 0;
		buy_type = 0;
		helper_fast_use = 0;
	}
};

/*
请求快速购买物品(返回)
*/
struct MSG_50151003 : public Base_Msg  {
	uint8_t status;
	int8_t helper_fast_use;
	int32_t id;
	MSG_50151003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(status);
		w.write_int8(helper_fast_use);
		w.write_int32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(status)  ||  r.read_int8(helper_fast_use)  ||  r.read_int32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50151003;
		status = 0;
		helper_fast_use = 0;
		id = 0;
	}
};



#endif