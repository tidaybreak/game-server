/*
* Generate by devtool
*/

#ifndef _MSG_OTHER_H_
#define _MSG_OTHER_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
购买体力
*/
struct MSG_10120000  {
	int8_t bind_gold_first;
	MSG_10120000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(bind_gold_first);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(bind_gold_first)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		bind_gold_first = 0;
	}
};

/*
购买体力返回
*/
struct MSG_50120000 : public Base_Msg  {
	uint8_t result;
	MSG_50120000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120000;
		result = 0;
	}
};

/*
请求体力信息
*/
struct MSG_10120001  {
	MSG_10120001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求体力信息返回
*/
struct MSG_50120001 : public Base_Msg  {
	uint16_t phy_power_cur;
	uint16_t phy_power_max;
	uint8_t phy_power_buy_times;
	int32_t phy_power_recover_time;
	MSG_50120001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(phy_power_cur);
		w.write_uint16(phy_power_max);
		w.write_uint8(phy_power_buy_times);
		w.write_int32(phy_power_recover_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(phy_power_cur)  ||  r.read_uint16(phy_power_max)  ||  r.read_uint8(phy_power_buy_times)  ||  r.read_int32(phy_power_recover_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120001;
		phy_power_cur = 0;
		phy_power_max = 0;
		phy_power_buy_times = 0;
		phy_power_recover_time = 0;
	}
};

/*
请求小喇叭聊天
*/
struct MSG_10120002  {
	std::vector<Chat_Content> content;
	MSG_10120002(void) { reset(); };

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

/*
请求获取系统设置
*/
struct MSG_10120003  {
	MSG_10120003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求获取系统设置(返回)
*/
struct MSG_50120003 : public Base_Msg  {
	uint8_t is_open_music;
	uint8_t music_num;
	uint8_t is_open_voice;
	uint8_t voice_num;
	uint8_t is_hide_role_model;
	uint8_t is_hide_hero_model;
	uint8_t is_hide_title;
	uint32_t player_count;
	uint8_t is_total_open;
	uint8_t is_skip_plot;
	MSG_50120003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_open_music);
		w.write_uint8(music_num);
		w.write_uint8(is_open_voice);
		w.write_uint8(voice_num);
		w.write_uint8(is_hide_role_model);
		w.write_uint8(is_hide_hero_model);
		w.write_uint8(is_hide_title);
		w.write_uint32(player_count);
		w.write_uint8(is_total_open);
		w.write_uint8(is_skip_plot);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_open_music)  ||  r.read_uint8(music_num)  ||  r.read_uint8(is_open_voice)  ||  r.read_uint8(voice_num)  ||  r.read_uint8(is_hide_role_model)  ||  r.read_uint8(is_hide_hero_model)  ||  r.read_uint8(is_hide_title)  ||  r.read_uint32(player_count)  ||  r.read_uint8(is_total_open)  ||  r.read_uint8(is_skip_plot)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120003;
		is_open_music = 0;
		music_num = 0;
		is_open_voice = 0;
		voice_num = 0;
		is_hide_role_model = 0;
		is_hide_hero_model = 0;
		is_hide_title = 0;
		player_count = 0;
		is_total_open = 0;
		is_skip_plot = 0;
	}
};

/*
请求更改系统设置
*/
struct MSG_10120004  {
	uint8_t is_open_music;
	uint8_t music_num;
	uint8_t is_open_voice;
	uint8_t voice_num;
	uint8_t is_hide_role_model;
	uint8_t is_hide_hero_model;
	uint8_t is_hide_title;
	uint32_t player_count;
	uint8_t is_total_open;
	uint8_t is_skip_plot;
	MSG_10120004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_open_music);
		w.write_uint8(music_num);
		w.write_uint8(is_open_voice);
		w.write_uint8(voice_num);
		w.write_uint8(is_hide_role_model);
		w.write_uint8(is_hide_hero_model);
		w.write_uint8(is_hide_title);
		w.write_uint32(player_count);
		w.write_uint8(is_total_open);
		w.write_uint8(is_skip_plot);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_open_music)  ||  r.read_uint8(music_num)  ||  r.read_uint8(is_open_voice)  ||  r.read_uint8(voice_num)  ||  r.read_uint8(is_hide_role_model)  ||  r.read_uint8(is_hide_hero_model)  ||  r.read_uint8(is_hide_title)  ||  r.read_uint32(player_count)  ||  r.read_uint8(is_total_open)  ||  r.read_uint8(is_skip_plot)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_open_music = 0;
		music_num = 0;
		is_open_voice = 0;
		voice_num = 0;
		is_hide_role_model = 0;
		is_hide_hero_model = 0;
		is_hide_title = 0;
		player_count = 0;
		is_total_open = 0;
		is_skip_plot = 0;
	}
};

/*
获取玩家觉醒等级
*/
struct MSG_10120020  {
	MSG_10120020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取玩家觉醒等级（返回）
*/
struct MSG_50120020 : public Base_Msg  {
	int32_t awake_lvl;
	MSG_50120020(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(awake_lvl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(awake_lvl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120020;
		awake_lvl = 0;
	}
};

/*
玩家觉醒等级升级
*/
struct MSG_10120021  {
	MSG_10120021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
玩家觉醒等级升级（返回）
*/
struct MSG_50120021 : public Base_Msg  {
	int8_t result;
	int32_t new_awake_skill;
	MSG_50120021(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(result);
		w.write_int32(new_awake_skill);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(result)  ||  r.read_int32(new_awake_skill)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120021;
		result = 0;
		new_awake_skill = 0;
	}
};

/*
获取玩家神器信息
*/
struct MSG_10120030  {
	MSG_10120030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取玩家神器信息（返回）
*/
struct MSG_50120030 : public Base_Msg  {
	std::vector<Artifact_Info> artifact_vec;
	MSG_50120030(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __artifact_vec_vec_size = artifact_vec.size();
		w.write_uint16(__artifact_vec_vec_size);
		for(uint16_t i = 0; i < __artifact_vec_vec_size; ++i) {
				artifact_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __artifact_vec_vec_size;
		if(r.read_uint16(__artifact_vec_vec_size)  )
				return -1;
		artifact_vec.reserve(__artifact_vec_vec_size);
		for(uint16_t i = 0; i < __artifact_vec_vec_size; ++i) {
				Artifact_Info v;
				if(v.deserialize(r))
						return -1;
				artifact_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50120030;
		artifact_vec.clear();
	}
};

/*
请求领取等级礼包
*/
struct MSG_10120040  {
	int32_t gift_lvl;
	MSG_10120040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(gift_lvl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(gift_lvl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gift_lvl = 0;
	}
};

/*
请求领取等级礼包（返回）
*/
struct MSG_50120040 : public Base_Msg  {
	int32_t gift_lvl;
	int8_t result;
	MSG_50120040(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(gift_lvl);
		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(gift_lvl)  ||  r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120040;
		gift_lvl = 0;
		result = 0;
	}
};

/*
请求已领过的等级礼包
*/
struct MSG_10120041  {
	MSG_10120041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求已领过的等级礼包（返回）
*/
struct MSG_50120041 : public Base_Msg  {
	std::vector<int32_t> gift_lvls;
	MSG_50120041(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gift_lvls_vec_size = gift_lvls.size();
		w.write_uint16(__gift_lvls_vec_size);
		for(uint16_t i = 0; i < __gift_lvls_vec_size; ++i) {
				w.write_int32(gift_lvls[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gift_lvls_vec_size;
		if(r.read_uint16(__gift_lvls_vec_size)  )
				return -1;
		gift_lvls.reserve(__gift_lvls_vec_size);
		for(uint16_t i = 0; i < __gift_lvls_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				gift_lvls.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50120041;
		gift_lvls.clear();
	}
};

/*
请求送花
*/
struct MSG_10120050  {
	uint32_t item_id;
	std::string role_name;
	std::string msg;
	MSG_10120050(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
		w.write_string(role_name);
		w.write_string(msg);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  ||  r.read_string(role_name)  ||  r.read_string(msg)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
		role_name.clear();
		msg.clear();
	}
};

/*
请求神秘商店信息
*/
struct MSG_10120010  {
	MSG_10120010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求神秘商店信息返回
*/
struct MSG_50120010 : public Base_Msg  {
	int32_t remain_time;
	std::vector<Secret_Item_Info> items;
	MSG_50120010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(remain_time);
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(remain_time)  )
				return -1;
 		uint16_t __items_vec_size;
		if(r.read_uint16(__items_vec_size)  )
				return -1;
		items.reserve(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				Secret_Item_Info v;
				if(v.deserialize(r))
						return -1;
				items.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50120010;
		remain_time = 0;
		items.clear();
	}
};

/*
请求神秘商店购买物品
*/
struct MSG_10120011  {
	int8_t index;
	MSG_10120011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
	}
};

/*
请求神秘商店购买物品返回
*/
struct MSG_50120011 : public Base_Msg  {
	int8_t shop_type;
	int32_t item_id;
	MSG_50120011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(shop_type);
		w.write_int32(item_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(shop_type)  ||  r.read_int32(item_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120011;
		shop_type = 0;
		item_id = 0;
	}
};

/*
请求查看鲜花
*/
struct MSG_10120051  {
	MSG_10120051(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求送花(返回)
*/
struct MSG_50120050 : public Base_Msg  {
	uint32_t item_id;
	uint32_t charm;
	uint32_t friendship;
	MSG_50120050(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
		w.write_uint32(charm);
		w.write_uint32(friendship);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  ||  r.read_uint32(charm)  ||  r.read_uint32(friendship)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120050;
		item_id = 0;
		charm = 0;
		friendship = 0;
	}
};

/*
请求发送你提我改
*/
struct MSG_10120052  {
	uint8_t type;
	std::string content;
	MSG_10120052(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_string(content);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_string(content)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		content.clear();
	}
};

/*
请求发送你提我改(返回)
*/
struct MSG_50120052 : public Base_Msg  {
	uint8_t type;
	MSG_50120052(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120052;
		type = 0;
	}
};

/*
获取玩家功勋信息
*/
struct MSG_10120080  {
	MSG_10120080(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取玩家功勋信息（返回）
*/
struct MSG_50120080 : public Base_Msg  {
	int32_t exploit_val;
	int32_t exploit_lvl;
	int64_t target_role_id;
	MSG_50120080(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(exploit_val);
		w.write_int32(exploit_lvl);
		w.write_int64(target_role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(exploit_val)  ||  r.read_int32(exploit_lvl)  ||  r.read_int64(target_role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120080;
		exploit_val = 0;
		exploit_lvl = 0;
		target_role_id = 0;
	}
};

/*
请求打开首充面板
*/
struct MSG_10120090  {
	MSG_10120090(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开首充面板(返回)
*/
struct MSG_50120090 : public Base_Msg  {
	uint8_t status;
	uint32_t open_time;
	MSG_50120090(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(status);
		w.write_uint32(open_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(status)  ||  r.read_uint32(open_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120090;
		status = 0;
		open_time = 0;
	}
};

/*
请求领取首充礼包
*/
struct MSG_10120091  {
	MSG_10120091(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取首充礼包(返回)
*/
struct MSG_50120091 : public Base_Msg  {
	MSG_50120091(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50120091;
	}
};

/*
请求打开充值返利面板
*/
struct MSG_10120092  {
	MSG_10120092(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开充值返利面板(返回)
*/
struct MSG_50120092 : public Base_Msg  {
	uint32_t addup_money;
	int32_t end_time;
	std::vector<Uint_Uint> status;
	uint32_t act_id;
	MSG_50120092(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(addup_money);
		w.write_int32(end_time);
		uint16_t __status_vec_size = status.size();
		w.write_uint16(__status_vec_size);
		for(uint16_t i = 0; i < __status_vec_size; ++i) {
				status[i].serialize(w);
		}

		w.write_uint32(act_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(addup_money)  ||  r.read_int32(end_time)  )
				return -1;
 		uint16_t __status_vec_size;
		if(r.read_uint16(__status_vec_size)  )
				return -1;
		status.reserve(__status_vec_size);
		for(uint16_t i = 0; i < __status_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				status.push_back(v);
		}

		if( r.read_uint32(act_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120092;
		addup_money = 0;
		end_time = 0;
		status.clear();
		act_id = 0;
	}
};

/*
请求领取充值返利奖励
*/
struct MSG_10120093  {
	uint32_t id;
	MSG_10120093(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取充值返利奖励(返回)
*/
struct MSG_50120093 : public Base_Msg  {
	uint32_t id;
	MSG_50120093(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120093;
		id = 0;
	}
};

/*
请求充值返利是否可领取
*/
struct MSG_10120094  {
	MSG_10120094(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
新手引导，剧情中采集道具
备注：如果背包中已有足够数量的道具不再增加，否则，增加道具到背包中
*/
struct MSG_10120095  {
	uint32_t item_id;
	uint32_t item_num;
	MSG_10120095(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(item_id);
		w.write_uint32(item_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(item_id)  ||  r.read_uint32(item_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
		item_num = 0;
	}
};

/*
请求发送擂台结果到公会聊天
*/
struct MSG_10120096  {
	std::string role_name;
	int64_t role_id;
	MSG_10120096(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_name.clear();
		role_id = 0;
	}
};

/*
请求发送擂台结果到公会聊天(返回)
*/
struct MSG_50120096 : public Base_Msg  {
	MSG_50120096(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50120096;
	}
};

/*
请求调查问卷信息
*/
struct MSG_10120097  {
	MSG_10120097(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求调查问卷信息(返回)
*/
struct MSG_50120097 : public Base_Msg  {
	uint8_t qid;
	MSG_50120097(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(qid);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(qid)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120097;
		qid = 0;
	}
};

/*
请求提交调查问卷信息
*/
struct MSG_10120098  {
	uint8_t qid;
	std::vector<Questionnaire_Answer_Info> ans;
	MSG_10120098(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(qid);
		uint16_t __ans_vec_size = ans.size();
		w.write_uint16(__ans_vec_size);
		for(uint16_t i = 0; i < __ans_vec_size; ++i) {
				ans[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(qid)  )
				return -1;
 		uint16_t __ans_vec_size;
		if(r.read_uint16(__ans_vec_size)  )
				return -1;
		ans.reserve(__ans_vec_size);
		for(uint16_t i = 0; i < __ans_vec_size; ++i) {
				Questionnaire_Answer_Info v;
				if(v.deserialize(r))
						return -1;
				ans.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		qid = 0;
		ans.clear();
	}
};

/*
请求打开邀请奖励信息
*/
struct MSG_10120060  {
	MSG_10120060(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开邀请奖励信息(返回)
*/
struct MSG_50120060 : public Base_Msg  {
	uint8_t ones;
	uint8_t daily;
	uint8_t like;
	MSG_50120060(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(ones);
		w.write_uint8(daily);
		w.write_uint8(like);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(ones)  ||  r.read_uint8(daily)  ||  r.read_uint8(like)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120060;
		ones = 0;
		daily = 0;
		like = 0;
	}
};

/*
请求领取邀请奖励信息(返回)
*/
struct MSG_50120061 : public Base_Msg  {
	uint8_t type;
	MSG_50120061(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120061;
		type = 0;
	}
};

/*
请求领取邀请奖励信息
*/
struct MSG_10120061  {
	uint8_t type;
	MSG_10120061(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
请求每月英雄信息
*/
struct MSG_10120070  {
	MSG_10120070(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求每月英雄信息返回
*/
struct MSG_50120070 : public Base_Msg  {
	int32_t cfg_index;
	int32_t remain_day;
	MSG_50120070(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(cfg_index);
		w.write_int32(remain_day);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(cfg_index)  ||  r.read_int32(remain_day)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120070;
		cfg_index = 0;
		remain_day = 0;
	}
};

/*
请求打开二次(全民)充值面板
*/
struct MSG_10120099  {
	MSG_10120099(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开二次(全民)充值面板(返回)
*/
struct MSG_50120099 : public Base_Msg  {
	uint8_t status;
	uint32_t over_time;
	uint32_t rec_num;
	MSG_50120099(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(status);
		w.write_uint32(over_time);
		w.write_uint32(rec_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(status)  ||  r.read_uint32(over_time)  ||  r.read_uint32(rec_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120099;
		status = 0;
		over_time = 0;
		rec_num = 0;
	}
};

/*
请求领取二次(全民)充值面板
*/
struct MSG_10120100  {
	MSG_10120100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取二次(全民)充值面板(返回)
*/
struct MSG_50120100 : public Base_Msg  {
	uint32_t rec_num;
	MSG_50120100(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(rec_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(rec_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120100;
		rec_num = 0;
	}
};

/*
请求打开限时充值面板
*/
struct MSG_10120101  {
	MSG_10120101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开限时充值面板(返回)
*/
struct MSG_50120101 : public Base_Msg  {
	uint8_t status;
	MSG_50120101(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50120101;
		status = 0;
	}
};

/*
请求领取限时充值面板
*/
struct MSG_10120102  {
	MSG_10120102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求领取限时充值面板(返回)
*/
struct MSG_50120102 : public Base_Msg  {
	MSG_50120102(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50120102;
	}
};

/*
剧情数群加1
*/
struct MSG_10120110  {
	int8_t type;
	MSG_10120110(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};



#endif