/*
* Generate by devtool
*/

#ifndef _MSG_STRUCT_H_
#define _MSG_STRUCT_H_


#include "Base_Struct.h"


/* Msg_Struct */
/*
人物出现信息
*/
struct Role_Appear_Info {
	int64_t role_id;/**/
	int32_t head_id;/*头像ID*/
	Coord coord;
	std::string role_name;/**/
	uint8_t gender;/*性别*/
	uint16_t level;/*等级*/
	uint16_t speed;/*速度*/
	int32_t career;/*职业*/
	int32_t title_id;/*称号*/
	double hp_curr;/**/
	double hp_max;/**/
	uint8_t vip;/*VIP*/
	std::string gang_name;/*公会名*/
	std::string server_name;/*服名*/
	int8_t state;/*（状态 0：正常，1：战斗中）*/
	std::vector<int32_t> avatar;/*外观 val_1:外观id   val_2:是否显示*/
	int8_t tmp_camp;/*临时阵营*/
	std::vector<Appear_Addition> addition_prop;
	Role_Appear_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(head_id);
		coord.serialize(w);
		w.write_string(role_name);
		w.write_uint8(gender);
		w.write_uint16(level);
		w.write_uint16(speed);
		w.write_int32(career);
		w.write_int32(title_id);
		w.write_double(hp_curr);
		w.write_double(hp_max);
		w.write_uint8(vip);
		w.write_string(gang_name);
		w.write_string(server_name);
		w.write_int8(state);
		uint16_t __avatar_vec_size = avatar.size();
		w.write_uint16(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				w.write_int32(avatar[i]);
		}

		w.write_int8(tmp_camp);
		uint16_t __addition_prop_vec_size = addition_prop.size();
		w.write_uint16(__addition_prop_vec_size);
		for(uint16_t i = 0; i < __addition_prop_vec_size; ++i) {
				addition_prop[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(head_id)  ||  coord.deserialize(r)  ||  r.read_string(role_name)  ||  r.read_uint8(gender)  ||  r.read_uint16(level)  ||  r.read_uint16(speed)  ||  r.read_int32(career)  ||  r.read_int32(title_id)  ||  r.read_double(hp_curr)  ||  r.read_double(hp_max)  ||  r.read_uint8(vip)  ||  r.read_string(gang_name)  ||  r.read_string(server_name)  ||  r.read_int8(state)  )
				return -1;
 		uint16_t __avatar_vec_size;
		if(r.read_uint16(__avatar_vec_size) )
				return -1;
		avatar.reserve(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				avatar.push_back(v);
		}

		if( r.read_int8(tmp_camp)  )
				return -1;
 		uint16_t __addition_prop_vec_size;
		if(r.read_uint16(__addition_prop_vec_size) )
				return -1;
		addition_prop.reserve(__addition_prop_vec_size);
		for(uint16_t i = 0; i < __addition_prop_vec_size; ++i) {
				Appear_Addition v;
				if(v.deserialize(r))
						return -1;
				addition_prop.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		role_id = 0;
		head_id = 0;
		coord.reset();
		role_name.clear();
		gender = 0;
		level = 0;
		speed = 0;
		career = 0;
		title_id = 0;
		hp_curr = 0.0;
		hp_max = 0.0;
		vip = 0;
		gang_name.clear();
		server_name.clear();
		state = 0;
		avatar.clear();
		tmp_camp = 0;
		addition_prop.clear();
	}
};

/*
卡魂信息
*/
struct Card_Info {
	int32_t index;/**/
	int32_t lv;/*等级*/
	int32_t id;/*配置id*/
	int32_t cur_exp;/*当前经验*/
	int32_t poten;/*潜力*/
	Int_Double main_attr;
	Int_Double sub_attr;
	int8_t star;/*星级*/
	int8_t part;/*部位*/
	int32_t item_id;/*装备id*/
	int8_t equip_loc;/*卡位*/
	int8_t embed_state;/*是否装备，0.未，1是*/
	Card_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int32(lv);
		w.write_int32(id);
		w.write_int32(cur_exp);
		w.write_int32(poten);
		main_attr.serialize(w);
		sub_attr.serialize(w);
		w.write_int8(star);
		w.write_int8(part);
		w.write_int32(item_id);
		w.write_int8(equip_loc);
		w.write_int8(embed_state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int32(lv)  ||  r.read_int32(id)  ||  r.read_int32(cur_exp)  ||  r.read_int32(poten)  ||  main_attr.deserialize(r)  ||  sub_attr.deserialize(r)  ||  r.read_int8(star)  ||  r.read_int8(part)  ||  r.read_int32(item_id)  ||  r.read_int8(equip_loc)  ||  r.read_int8(embed_state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		lv = 0;
		id = 0;
		cur_exp = 0;
		poten = 0;
		main_attr.reset();
		sub_attr.reset();
		star = 0;
		part = 0;
		item_id = 0;
		equip_loc = 0;
		embed_state = 0;
	}
};

/*
附魔属性信息
*/
struct Enchant_Prop_Info {
	int32_t prop_type;/*属性类型*/
	int32_t prop_val;/*属性值*/
	int8_t star;/*星数*/
	int8_t is_lock;/*是否锁，1是0否*/
	Enchant_Prop_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(prop_type);
		w.write_int32(prop_val);
		w.write_int8(star);
		w.write_int8(is_lock);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(prop_type)  ||  r.read_int32(prop_val)  ||  r.read_int8(star)  ||  r.read_int8(is_lock)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		prop_type = 0;
		prop_val = 0;
		star = 0;
		is_lock = 0;
	}
};

/*
附魔属性组信息
*/
struct Enchant_Prop_Group_Info {
	std::vector<Enchant_Prop_Info> group_info;
	int32_t score;/**/
	Enchant_Prop_Group_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __group_info_vec_size = group_info.size();
		w.write_uint16(__group_info_vec_size);
		for(uint16_t i = 0; i < __group_info_vec_size; ++i) {
				group_info[i].serialize(w);
		}

		w.write_int32(score);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __group_info_vec_size;
		if(r.read_uint16(__group_info_vec_size) )
				return -1;
		group_info.reserve(__group_info_vec_size);
		for(uint16_t i = 0; i < __group_info_vec_size; ++i) {
				Enchant_Prop_Info v;
				if(v.deserialize(r))
						return -1;
				group_info.push_back(v);
		}

		if( r.read_int32(score)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		group_info.clear();
		score = 0;
	}
};

/*
物品装备附加信息
*/
struct Item_Equip_Info {
	uint32_t index;/**/
	int32_t unban_lvl;/*解封等级*/
	int32_t force;/*装备战斗力*/
	int32_t score;/*装备评分*/
	uint8_t incr_level;/*精通等级*/
	int32_t incr_luck_value;/*当前精通幸运值*/
	int8_t incr_fail_add_rate;/*精通提升失败提升成功率(百分制)*/
	std::vector<Enchant_Prop_Group_Info> enchant_prop_groups;
	std::vector<Card_Info> card_info_vec;
	std::vector<Int_Int> gemstone_vec;
	Item_Equip_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_int32(unban_lvl);
		w.write_int32(force);
		w.write_int32(score);
		w.write_uint8(incr_level);
		w.write_int32(incr_luck_value);
		w.write_int8(incr_fail_add_rate);
		uint16_t __enchant_prop_groups_vec_size = enchant_prop_groups.size();
		w.write_uint16(__enchant_prop_groups_vec_size);
		for(uint16_t i = 0; i < __enchant_prop_groups_vec_size; ++i) {
				enchant_prop_groups[i].serialize(w);
		}

		uint16_t __card_info_vec_vec_size = card_info_vec.size();
		w.write_uint16(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				card_info_vec[i].serialize(w);
		}

		uint16_t __gemstone_vec_vec_size = gemstone_vec.size();
		w.write_uint16(__gemstone_vec_vec_size);
		for(uint16_t i = 0; i < __gemstone_vec_vec_size; ++i) {
				gemstone_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_int32(unban_lvl)  ||  r.read_int32(force)  ||  r.read_int32(score)  ||  r.read_uint8(incr_level)  ||  r.read_int32(incr_luck_value)  ||  r.read_int8(incr_fail_add_rate)  )
				return -1;
 		uint16_t __enchant_prop_groups_vec_size;
		if(r.read_uint16(__enchant_prop_groups_vec_size) )
				return -1;
		enchant_prop_groups.reserve(__enchant_prop_groups_vec_size);
		for(uint16_t i = 0; i < __enchant_prop_groups_vec_size; ++i) {
				Enchant_Prop_Group_Info v;
				if(v.deserialize(r))
						return -1;
				enchant_prop_groups.push_back(v);
		}

		uint16_t __card_info_vec_vec_size;
		if(r.read_uint16(__card_info_vec_vec_size) )
				return -1;
		card_info_vec.reserve(__card_info_vec_vec_size);
		for(uint16_t i = 0; i < __card_info_vec_vec_size; ++i) {
				Card_Info v;
				if(v.deserialize(r))
						return -1;
				card_info_vec.push_back(v);
		}

		uint16_t __gemstone_vec_vec_size;
		if(r.read_uint16(__gemstone_vec_vec_size) )
				return -1;
		gemstone_vec.reserve(__gemstone_vec_vec_size);
		for(uint16_t i = 0; i < __gemstone_vec_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				gemstone_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		index = 0;
		unban_lvl = 0;
		force = 0;
		score = 0;
		incr_level = 0;
		incr_luck_value = 0;
		incr_fail_add_rate = 0;
		enchant_prop_groups.clear();
		card_info_vec.clear();
		gemstone_vec.clear();
	}
};

/*
物品符石附加信息
*/
struct Item_Rune_Stone_Info {
	uint32_t index;/**/
	int32_t exp_lvl;/*经验等级*/
	int32_t exp;/*经验等级*/
	int8_t is_lock;/*是否锁定*/
	Item_Rune_Stone_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_int32(exp_lvl);
		w.write_int32(exp);
		w.write_int8(is_lock);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_int32(exp_lvl)  ||  r.read_int32(exp)  ||  r.read_int8(is_lock)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		exp_lvl = 0;
		exp = 0;
		is_lock = 0;
	}
};

/*
NPC出现信息
*/
struct NPC_Appear_Info {
	int64_t role_id;/**/
	int8_t type;/*怪物（0：被动 1：主动 2：友善 3：小动物（预设） 4：飞行类小动物（预设） 5：神秘商人 6：弹确定框的被动怪）采集物（0：宝箱 1：非宝箱）机关（0：点击触发，1：采集触发，其他：进入范围触发）*/
	int32_t npc_type;/**/
	std::string role_name;/**/
	int64_t master_direct;/*直接主人*/
	Coord coord;
	uint16_t speed;/*速度*/
	int32_t title_id;/*称号id，默认0*/
	double hp_curr;/**/
	double hp_max;/**/
	int8_t state;/*战斗状态（0：正常，1：战斗中）*/
	std::vector<int32_t> avatar;/**/
	int8_t tmp_camp;/*临时阵营*/
	std::vector<int64_t> add_id;/*附属id（不为空时，只有附属id的玩家可见）*/
	std::vector<Appear_Addition> addition_prop;
	int32_t awake_lvl;/*觉醒等级*/
	int32_t npc_jid;/*npc模型id*/
	NPC_Appear_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(type);
		w.write_int32(npc_type);
		w.write_string(role_name);
		w.write_int64(master_direct);
		coord.serialize(w);
		w.write_uint16(speed);
		w.write_int32(title_id);
		w.write_double(hp_curr);
		w.write_double(hp_max);
		w.write_int8(state);
		uint16_t __avatar_vec_size = avatar.size();
		w.write_uint16(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				w.write_int32(avatar[i]);
		}

		w.write_int8(tmp_camp);
		uint16_t __add_id_vec_size = add_id.size();
		w.write_uint16(__add_id_vec_size);
		for(uint16_t i = 0; i < __add_id_vec_size; ++i) {
				w.write_int64(add_id[i]);
		}

		uint16_t __addition_prop_vec_size = addition_prop.size();
		w.write_uint16(__addition_prop_vec_size);
		for(uint16_t i = 0; i < __addition_prop_vec_size; ++i) {
				addition_prop[i].serialize(w);
		}

		w.write_int32(awake_lvl);
		w.write_int32(npc_jid);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(type)  ||  r.read_int32(npc_type)  ||  r.read_string(role_name)  ||  r.read_int64(master_direct)  ||  coord.deserialize(r)  ||  r.read_uint16(speed)  ||  r.read_int32(title_id)  ||  r.read_double(hp_curr)  ||  r.read_double(hp_max)  ||  r.read_int8(state)  )
				return -1;
 		uint16_t __avatar_vec_size;
		if(r.read_uint16(__avatar_vec_size) )
				return -1;
		avatar.reserve(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				avatar.push_back(v);
		}

		if( r.read_int8(tmp_camp)  )
				return -1;
 		uint16_t __add_id_vec_size;
		if(r.read_uint16(__add_id_vec_size) )
				return -1;
		add_id.reserve(__add_id_vec_size);
		for(uint16_t i = 0; i < __add_id_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				add_id.push_back(v);
		}

		uint16_t __addition_prop_vec_size;
		if(r.read_uint16(__addition_prop_vec_size) )
				return -1;
		addition_prop.reserve(__addition_prop_vec_size);
		for(uint16_t i = 0; i < __addition_prop_vec_size; ++i) {
				Appear_Addition v;
				if(v.deserialize(r))
						return -1;
				addition_prop.push_back(v);
		}

		if( r.read_int32(awake_lvl)  ||  r.read_int32(npc_jid)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		type = 0;
		npc_type = 0;
		role_name.clear();
		master_direct = 0;
		coord.reset();
		speed = 0;
		title_id = 0;
		hp_curr = 0.0;
		hp_max = 0.0;
		state = 0;
		avatar.clear();
		tmp_camp = 0;
		add_id.clear();
		addition_prop.clear();
		awake_lvl = 0;
		npc_jid = 0;
	}
};

/*
Task_Info
value : 保存当前任务完成度 

status: 1不可接 2可接 3已接 4可交付 5已交付 6失败 

extra : 

is_auto_submit:  可直接交付的任务，在任务完成后(状态为4)置为1；其他情况下，全为0
task_quality: 悬赏任务品质。 0:白 1:绿  2:蓝  3：紫 4:橙
type: 任务类型。
                101:主线任务  102: 支线任务   103: 日常任务   104:周常任务    110:悬赏任务
                120:环任务
*/
struct Task_Info {
	uint16_t type;/*任务类型*/
	int32_t id;/*任务ID*/
	uint8_t status;/*任务状态*/
	uint32_t time;/*接任务时间*/
	std::vector<int32_t> value;/*任务值*/
	std::vector<int32_t> extra;/*扩展*/
	uint8_t is_auto_submit;/*是否自动提交。1：是，0：否*/
	uint8_t is_limit_time;/*是否限时任务。1：是，0：否*/
	uint32_t second;/*限时任务剩余秒数。(只对限时任务有用)*/
	uint32_t ring_num;/*环数*/
	Task_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(type);
		w.write_int32(id);
		w.write_uint8(status);
		w.write_uint32(time);
		uint16_t __value_vec_size = value.size();
		w.write_uint16(__value_vec_size);
		for(uint16_t i = 0; i < __value_vec_size; ++i) {
				w.write_int32(value[i]);
		}

		uint16_t __extra_vec_size = extra.size();
		w.write_uint16(__extra_vec_size);
		for(uint16_t i = 0; i < __extra_vec_size; ++i) {
				w.write_int32(extra[i]);
		}

		w.write_uint8(is_auto_submit);
		w.write_uint8(is_limit_time);
		w.write_uint32(second);
		w.write_uint32(ring_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(type)  ||  r.read_int32(id)  ||  r.read_uint8(status)  ||  r.read_uint32(time)  )
				return -1;
 		uint16_t __value_vec_size;
		if(r.read_uint16(__value_vec_size) )
				return -1;
		value.reserve(__value_vec_size);
		for(uint16_t i = 0; i < __value_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				value.push_back(v);
		}

		uint16_t __extra_vec_size;
		if(r.read_uint16(__extra_vec_size) )
				return -1;
		extra.reserve(__extra_vec_size);
		for(uint16_t i = 0; i < __extra_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				extra.push_back(v);
		}

		if( r.read_uint8(is_auto_submit)  ||  r.read_uint8(is_limit_time)  ||  r.read_uint32(second)  ||  r.read_uint32(ring_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		id = 0;
		status = 0;
		time = 0;
		value.clear();
		extra.clear();
		is_auto_submit = 0;
		is_limit_time = 0;
		second = 0;
		ring_num = 0;
	}
};

/*
Task_Status
*/
struct Task_Status {
	uint32_t task_id;/*任务ID*/
	uint16_t type;/*任务类型*/
	uint8_t status;/*任务状态*/
	Task_Status(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint16(type);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint16(type)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id = 0;
		type = 0;
		status = 0;
	}
};

/*
邮件信息
*/
struct Mail_Info {
	int32_t mail_id;/*邮件ID*/
	int8_t have_read;/*是否已读 1:已读, 0:未读*/
	int32_t time;/*发送时间*/
	int32_t gold;/*钻石*/
	std::string mail_title;/*邮件标题*/
	std::string mail_content;/*邮件内容*/
	std::string role_name;/*收到邮件时为发件人名称，发送邮件时为收件人名称*/
	int32_t mail_type;/*邮件类型1:普通邮件，0：系统邮件*/
	int8_t have_item;/*是否有附件*/
	std::vector<Item_Basic_Info> item_vector;
	Mail_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mail_id);
		w.write_int8(have_read);
		w.write_int32(time);
		w.write_int32(gold);
		w.write_string(mail_title);
		w.write_string(mail_content);
		w.write_string(role_name);
		w.write_int32(mail_type);
		w.write_int8(have_item);
		uint16_t __item_vector_vec_size = item_vector.size();
		w.write_uint16(__item_vector_vec_size);
		for(uint16_t i = 0; i < __item_vector_vec_size; ++i) {
				item_vector[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mail_id)  ||  r.read_int8(have_read)  ||  r.read_int32(time)  ||  r.read_int32(gold)  ||  r.read_string(mail_title)  ||  r.read_string(mail_content)  ||  r.read_string(role_name)  ||  r.read_int32(mail_type)  ||  r.read_int8(have_item)  )
				return -1;
 		uint16_t __item_vector_vec_size;
		if(r.read_uint16(__item_vector_vec_size) )
				return -1;
		item_vector.reserve(__item_vector_vec_size);
		for(uint16_t i = 0; i < __item_vector_vec_size; ++i) {
				Item_Basic_Info v;
				if(v.deserialize(r))
						return -1;
				item_vector.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		mail_id = 0;
		have_read = 0;
		time = 0;
		gold = 0;
		mail_title.clear();
		mail_content.clear();
		role_name.clear();
		mail_type = 0;
		have_item = 0;
		item_vector.clear();
	}
};

/*
单人副本信息
*/
struct Single_Dungeon_Info {
	int32_t dungeon_id;/*副本id*/
	uint16_t daily_counter;/*当天进入副本次数*/
	int8_t is_passed;/*0:已经通关 1：没有通关*/
	Single_Dungeon_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(dungeon_id);
		w.write_uint16(daily_counter);
		w.write_int8(is_passed);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(dungeon_id)  ||  r.read_uint16(daily_counter)  ||  r.read_int8(is_passed)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		dungeon_id = 0;
		daily_counter = 0;
		is_passed = 0;
	}
};

/*
奖项
*/
struct Reward_Info {
	int32_t reward_id;/*奖品id(物品， 属性等）*/
	int32_t quantity;/*数量（未加上vip）*/
	uint8_t bind;/*是否绑定（1）不绑，（2）已绑 其他值非法*/
	int32_t vip_add;/*vip加成比例（千分制）*/
	int32_t world_level;/*世界等级加成（50%则为50）*/
	Reward_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(reward_id);
		w.write_int32(quantity);
		w.write_uint8(bind);
		w.write_int32(vip_add);
		w.write_int32(world_level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(reward_id)  ||  r.read_int32(quantity)  ||  r.read_uint8(bind)  ||  r.read_int32(vip_add)  ||  r.read_int32(world_level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		reward_id = 0;
		quantity = 0;
		bind = 0;
		vip_add = 0;
		world_level = 0;
	}
};

/*
战斗单位出现
*/
struct Fighter_Appear_Info {
	int64_t role_id;/**/
	std::string role_name;/*名字*/
	int8_t career;/*职业*/
	int8_t gender;/*性别*/
	int16_t type;/*0：英雄 300:小怪 301：boss*/
	int32_t role_type;/*类型id*/
	int64_t master_direct;/*直接主人*/
	Int_Int position;
	std::vector<int32_t> avatar;/**/
	int32_t appear_effect;/**/
	double hp_curr;/**/
	double hp_max;/**/
	int8_t fight_state;/*当时战斗中的状态*/
	std::vector<Prop_Change> init_props;
	int32_t role_jid;/*模型id*/
	int32_t awake_lvl;/*觉醒等级*/
	Fighter_Appear_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int8(career);
		w.write_int8(gender);
		w.write_int16(type);
		w.write_int32(role_type);
		w.write_int64(master_direct);
		position.serialize(w);
		uint16_t __avatar_vec_size = avatar.size();
		w.write_uint16(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				w.write_int32(avatar[i]);
		}

		w.write_int32(appear_effect);
		w.write_double(hp_curr);
		w.write_double(hp_max);
		w.write_int8(fight_state);
		uint16_t __init_props_vec_size = init_props.size();
		w.write_uint16(__init_props_vec_size);
		for(uint16_t i = 0; i < __init_props_vec_size; ++i) {
				init_props[i].serialize(w);
		}

		w.write_int32(role_jid);
		w.write_int32(awake_lvl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int8(career)  ||  r.read_int8(gender)  ||  r.read_int16(type)  ||  r.read_int32(role_type)  ||  r.read_int64(master_direct)  ||  position.deserialize(r)  )
				return -1;
 		uint16_t __avatar_vec_size;
		if(r.read_uint16(__avatar_vec_size) )
				return -1;
		avatar.reserve(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				avatar.push_back(v);
		}

		if( r.read_int32(appear_effect)  ||  r.read_double(hp_curr)  ||  r.read_double(hp_max)  ||  r.read_int8(fight_state)  )
				return -1;
 		uint16_t __init_props_vec_size;
		if(r.read_uint16(__init_props_vec_size) )
				return -1;
		init_props.reserve(__init_props_vec_size);
		for(uint16_t i = 0; i < __init_props_vec_size; ++i) {
				Prop_Change v;
				if(v.deserialize(r))
						return -1;
				init_props.push_back(v);
		}

		if( r.read_int32(role_jid)  ||  r.read_int32(awake_lvl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		career = 0;
		gender = 0;
		type = 0;
		role_type = 0;
		master_direct = 0;
		position.reset();
		avatar.clear();
		appear_effect = 0;
		hp_curr = 0.0;
		hp_max = 0.0;
		fight_state = 0;
		init_props.clear();
		role_jid = 0;
		awake_lvl = 0;
	}
};

/*
技能学习
*/
struct Skill_Info {
	int32_t skill_id;/*技能id*/
	int32_t skill_lv;/*技能等级*/
	int32_t skill_loc;/*技能位置(0，不在战斗的技能）*/
	Skill_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_int32(skill_lv);
		w.write_int32(skill_loc);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_int32(skill_lv)  ||  r.read_int32(skill_loc)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
		skill_lv = 0;
		skill_loc = 0;
	}
};

/*
好友信息
*/
struct Friend_Info {
	int64_t role_id;/*好友角色ID*/
	std::string role_name;/*好友角色名*/
	int8_t sex;/*好友性别*/
	int8_t friend_type;/*好友类型0－好友，1－陌生人，2－黑名单*/
	int32_t career;/*职业*/
	int32_t level;/*等级*/
	std::string faction_name;/*帮派名称*/
	int32_t scene;/*场景*/
	int32_t is_black;/*是否在黑名单*/
	int8_t is_on_line;/*是否在线 1:在线   16：不在线*/
	int64_t team_id;/*组队ID，如果没组队，ID为0*/
	int32_t friendship_degree;/*好友度*/
	int32_t head_photo_id;/*头像*/
	int8_t vip;/*VIP等级*/
	Friend_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int8(sex);
		w.write_int8(friend_type);
		w.write_int32(career);
		w.write_int32(level);
		w.write_string(faction_name);
		w.write_int32(scene);
		w.write_int32(is_black);
		w.write_int8(is_on_line);
		w.write_int64(team_id);
		w.write_int32(friendship_degree);
		w.write_int32(head_photo_id);
		w.write_int8(vip);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int8(sex)  ||  r.read_int8(friend_type)  ||  r.read_int32(career)  ||  r.read_int32(level)  ||  r.read_string(faction_name)  ||  r.read_int32(scene)  ||  r.read_int32(is_black)  ||  r.read_int8(is_on_line)  ||  r.read_int64(team_id)  ||  r.read_int32(friendship_degree)  ||  r.read_int32(head_photo_id)  ||  r.read_int8(vip)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		sex = 0;
		friend_type = 0;
		career = 0;
		level = 0;
		faction_name.clear();
		scene = 0;
		is_black = 0;
		is_on_line = 0;
		team_id = 0;
		friendship_degree = 0;
		head_photo_id = 0;
		vip = 0;
	}
};

/*
申请好友信息
*/
struct Friend_Apply_Info {
	int64_t apply_id;/*申请人ID*/
	std::string role_name;/**/
	int32_t level;/*等级*/
	int32_t ttl_force;/*战斗力*/
	int32_t apply_time;/*申请时间*/
	Friend_Apply_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(apply_id);
		w.write_string(role_name);
		w.write_int32(level);
		w.write_int32(ttl_force);
		w.write_int32(apply_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(apply_id)  ||  r.read_string(role_name)  ||  r.read_int32(level)  ||  r.read_int32(ttl_force)  ||  r.read_int32(apply_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		apply_id = 0;
		role_name.clear();
		level = 0;
		ttl_force = 0;
		apply_time = 0;
	}
};

/*
宝箱中物品信息
*/
struct Drops_Item {
	int32_t item_id;/*道具id*/
	int32_t item_amount;/*道具数量*/
	int8_t bind;/*是否绑定0：非绑定*/
	Drops_Item(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(item_id);
		w.write_int32(item_amount);
		w.write_int8(bind);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(item_id)  ||  r.read_int32(item_amount)  ||  r.read_int8(bind)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_id = 0;
		item_amount = 0;
		bind = 0;
	}
};

/*
神器信息
*/
struct Artifact_Info {
	int32_t artifact_id;/*神器ID*/
	int32_t artifact_lvl;/*神器等级*/
	Artifact_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(artifact_id);
		w.write_int32(artifact_lvl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(artifact_id)  ||  r.read_int32(artifact_lvl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		artifact_id = 0;
		artifact_lvl = 0;
	}
};

/*
人物信息
*/
struct Role_Info {
	int64_t role_id;/*人物id*/
	std::string role_name;/*人物姓名*/
	int32_t head_photo_id;/*人物头像*/
	uint8_t gender;/*性别（1男，2女）*/
	std::string guild_name;/*公会名称*/
	int32_t career;/*职业*/
	int32_t level;/*人物等级*/
	int8_t vip_level;/*vip等级*/
	int32_t combat;/*战斗力*/
	int32_t title_id;/*称号*/
	Role_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int32(head_photo_id);
		w.write_uint8(gender);
		w.write_string(guild_name);
		w.write_int32(career);
		w.write_int32(level);
		w.write_int8(vip_level);
		w.write_int32(combat);
		w.write_int32(title_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int32(head_photo_id)  ||  r.read_uint8(gender)  ||  r.read_string(guild_name)  ||  r.read_int32(career)  ||  r.read_int32(level)  ||  r.read_int8(vip_level)  ||  r.read_int32(combat)  ||  r.read_int32(title_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		head_photo_id = 0;
		gender = 0;
		guild_name.clear();
		career = 0;
		level = 0;
		vip_level = 0;
		combat = 0;
		title_id = 0;
	}
};

/*
商城商品信息
*/
struct Goods_Info {
	int32_t id;/*物品ID*/
	int32_t tabs;/*标签: 1:促销 2:粉钻 3:道具 4:宝石 5:时装 99:计时促销*/
	int32_t pay_type;/*支付类型 2:粉钻优先,3:钻石,4:粉钻*/
	int32_t price;/*售价*/
	int32_t vip_discounts;/*是否可以享受VIP打折 0:不可以 1:可以*/
	int32_t promo_price;/*促销价格*/
	int32_t sort;/*排序*/
	int32_t all_num;/*总销售数量*/
	int32_t per_num;/*每人剩余可购买*/
	int32_t bind;/*绑定状态*/
	int32_t per_all;/*每人总可购买*/
	Goods_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(tabs);
		w.write_int32(pay_type);
		w.write_int32(price);
		w.write_int32(vip_discounts);
		w.write_int32(promo_price);
		w.write_int32(sort);
		w.write_int32(all_num);
		w.write_int32(per_num);
		w.write_int32(bind);
		w.write_int32(per_all);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(tabs)  ||  r.read_int32(pay_type)  ||  r.read_int32(price)  ||  r.read_int32(vip_discounts)  ||  r.read_int32(promo_price)  ||  r.read_int32(sort)  ||  r.read_int32(all_num)  ||  r.read_int32(per_num)  ||  r.read_int32(bind)  ||  r.read_int32(per_all)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		tabs = 0;
		pay_type = 0;
		price = 0;
		vip_discounts = 0;
		promo_price = 0;
		sort = 0;
		all_num = 0;
		per_num = 0;
		bind = 0;
		per_all = 0;
	}
};

/*
好友id-类型
*/
struct Friend_Id_Style {
	int64_t role_id;/*角色id*/
	int8_t role_style;/*角色类型：1好友-2公会-3-最近-4-黑名单*/
	Friend_Id_Style(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(role_style);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(role_style)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_style = 0;
	}
};

/*
人物信息-组队
*/
struct Role_Team_Info {
	int64_t role_id;/*人物id*/
	std::string role_name;/*人物姓名*/
	int32_t head_photo_id;/*人物头像*/
	uint8_t gender;/*性别（1男，2女）*/
	std::string guild_name;/*公会名称*/
	int32_t career;/*职业*/
	int32_t level;/*人物等级*/
	int8_t vip_level;/*vip等级*/
	int32_t combat;/*战斗力*/
	int8_t ready;/*准备状态*/
	Role_Team_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int32(head_photo_id);
		w.write_uint8(gender);
		w.write_string(guild_name);
		w.write_int32(career);
		w.write_int32(level);
		w.write_int8(vip_level);
		w.write_int32(combat);
		w.write_int8(ready);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int32(head_photo_id)  ||  r.read_uint8(gender)  ||  r.read_string(guild_name)  ||  r.read_int32(career)  ||  r.read_int32(level)  ||  r.read_int8(vip_level)  ||  r.read_int32(combat)  ||  r.read_int8(ready)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		head_photo_id = 0;
		gender = 0;
		guild_name.clear();
		career = 0;
		level = 0;
		vip_level = 0;
		combat = 0;
		ready = 0;
	}
};

/*
队伍基本信息
*/
struct Team_Basic_Info {
	uint32_t index;/*房间索引号*/
	int64_t team_id;/*队伍id*/
	int8_t type;/*区分是组队(0)还是竞技场(1)的或者是挂机(2)*/
	Role_Team_Info leader;
	int8_t teamer_nums;/*队员人数*/
	int32_t teamer_nums_max;/*队员人数上限*/
	std::vector<int8_t> index_list;/*关闭的位置索引(空无设置)*/
	int32_t scene_id;/*副本id(可根据配置取得副本信息)*/
	std::string password;/*密码(6位以内数字)*/
	int8_t scene_degree;/*副本难度0普通1噩梦*/
	std::string words;/*队伍备注*/
	int8_t battle;/*是否进入战斗、副本*/
	int32_t level;/*组队竞技：战队等级*/
	Team_Basic_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(index);
		w.write_int64(team_id);
		w.write_int8(type);
		leader.serialize(w);
		w.write_int8(teamer_nums);
		w.write_int32(teamer_nums_max);
		uint16_t __index_list_vec_size = index_list.size();
		w.write_uint16(__index_list_vec_size);
		for(uint16_t i = 0; i < __index_list_vec_size; ++i) {
				w.write_int8(index_list[i]);
		}

		w.write_int32(scene_id);
		w.write_string(password);
		w.write_int8(scene_degree);
		w.write_string(words);
		w.write_int8(battle);
		w.write_int32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(index)  ||  r.read_int64(team_id)  ||  r.read_int8(type)  ||  leader.deserialize(r)  ||  r.read_int8(teamer_nums)  ||  r.read_int32(teamer_nums_max)  )
				return -1;
 		uint16_t __index_list_vec_size;
		if(r.read_uint16(__index_list_vec_size) )
				return -1;
		index_list.reserve(__index_list_vec_size);
		for(uint16_t i = 0; i < __index_list_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				index_list.push_back(v);
		}

		if( r.read_int32(scene_id)  ||  r.read_string(password)  ||  r.read_int8(scene_degree)  ||  r.read_string(words)  ||  r.read_int8(battle)  ||  r.read_int32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		team_id = 0;
		type = 0;
		leader.reset();
		teamer_nums = 0;
		teamer_nums_max = 0;
		index_list.clear();
		scene_id = 0;
		password.clear();
		scene_degree = 0;
		words.clear();
		battle = 0;
		level = 0;
	}
};

/*
商城促销商品信息
*/
struct Mall_Promo_Info {
	int32_t start_time;/*开始时间*/
	int32_t end_time;/*结束时间*/
	int32_t day;/*循环天*/
	std::vector<Goods_Info> goods_list;
	Mall_Promo_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(start_time);
		w.write_int32(end_time);
		w.write_int32(day);
		uint16_t __goods_list_vec_size = goods_list.size();
		w.write_uint16(__goods_list_vec_size);
		for(uint16_t i = 0; i < __goods_list_vec_size; ++i) {
				goods_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(start_time)  ||  r.read_int32(end_time)  ||  r.read_int32(day)  )
				return -1;
 		uint16_t __goods_list_vec_size;
		if(r.read_uint16(__goods_list_vec_size) )
				return -1;
		goods_list.reserve(__goods_list_vec_size);
		for(uint16_t i = 0; i < __goods_list_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				goods_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		start_time = 0;
		end_time = 0;
		day = 0;
		goods_list.clear();
	}
};

/*
人物基础信息
*/
struct Role_Base_Info {
	int64_t role_id;/*人物id*/
	std::string role_name;/*人物姓名*/
	uint8_t gender;/*性别（1男，2女）*/
	std::string guild_name;/*公会名称*/
	int32_t career;/*职业*/
	int32_t level;/*人物等级*/
	int8_t vip_level;/*vip等级*/
	int32_t combat;/*战斗力*/
	Role_Base_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_uint8(gender);
		w.write_string(guild_name);
		w.write_int32(career);
		w.write_int32(level);
		w.write_int8(vip_level);
		w.write_int32(combat);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_uint8(gender)  ||  r.read_string(guild_name)  ||  r.read_int32(career)  ||  r.read_int32(level)  ||  r.read_int8(vip_level)  ||  r.read_int32(combat)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		gender = 0;
		guild_name.clear();
		career = 0;
		level = 0;
		vip_level = 0;
		combat = 0;
	}
};

/*
公会列信息
*/
struct Gang_Row_Info {
	int64_t gang_id;/*公会id*/
	int32_t rank;/*排名*/
	int32_t level;/**/
	int32_t cur_members;/*当前成员数*/
	int32_t max_members;/*最大成员数*/
	int8_t join_block;/*是否允许所有人加入 0-允许所有加入，1-需要批准进入*/
	uint8_t logo;/**/
	std::string gang_name;/*公会名*/
	Role_Info leader_info;
	int8_t apply_status;/*0无需显示 1申请加入 2取消申请*/
	Gang_Row_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int32(rank);
		w.write_int32(level);
		w.write_int32(cur_members);
		w.write_int32(max_members);
		w.write_int8(join_block);
		w.write_uint8(logo);
		w.write_string(gang_name);
		leader_info.serialize(w);
		w.write_int8(apply_status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int32(rank)  ||  r.read_int32(level)  ||  r.read_int32(cur_members)  ||  r.read_int32(max_members)  ||  r.read_int8(join_block)  ||  r.read_uint8(logo)  ||  r.read_string(gang_name)  ||  leader_info.deserialize(r)  ||  r.read_int8(apply_status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		rank = 0;
		level = 0;
		cur_members = 0;
		max_members = 0;
		join_block = 0;
		logo = 0;
		gang_name.clear();
		leader_info.reset();
		apply_status = 0;
	}
};

/*
公会成员列信息
*/
struct Ganger_Row_Info {
	int64_t role_id;/**/
	uint32_t week_contribute;/*周贡献*/
	uint32_t total_contribute;/*总贡献*/
	uint32_t fight;/*战斗力*/
	int32_t amplify_times;/*附魔次数*/
	int32_t level;/**/
	int8_t vip;/**/
	int8_t headship;/*公会职务*/
	std::vector<int8_t> authority;/**/
	int8_t career;/**/
	int8_t line_stat;/*在线状态*/
	int8_t gender;/**/
	std::string ganger_name;/*成员名称*/
	int32_t head_photo_id;/*头像id*/
	int8_t is_zhizunbao;/*是否是至尊宝*/
	int32_t last_sign_out_time;/*最后下线时间*/
	Ganger_Row_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint32(week_contribute);
		w.write_uint32(total_contribute);
		w.write_uint32(fight);
		w.write_int32(amplify_times);
		w.write_int32(level);
		w.write_int8(vip);
		w.write_int8(headship);
		uint16_t __authority_vec_size = authority.size();
		w.write_uint16(__authority_vec_size);
		for(uint16_t i = 0; i < __authority_vec_size; ++i) {
				w.write_int8(authority[i]);
		}

		w.write_int8(career);
		w.write_int8(line_stat);
		w.write_int8(gender);
		w.write_string(ganger_name);
		w.write_int32(head_photo_id);
		w.write_int8(is_zhizunbao);
		w.write_int32(last_sign_out_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint32(week_contribute)  ||  r.read_uint32(total_contribute)  ||  r.read_uint32(fight)  ||  r.read_int32(amplify_times)  ||  r.read_int32(level)  ||  r.read_int8(vip)  ||  r.read_int8(headship)  )
				return -1;
 		uint16_t __authority_vec_size;
		if(r.read_uint16(__authority_vec_size) )
				return -1;
		authority.reserve(__authority_vec_size);
		for(uint16_t i = 0; i < __authority_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				authority.push_back(v);
		}

		if( r.read_int8(career)  ||  r.read_int8(line_stat)  ||  r.read_int8(gender)  ||  r.read_string(ganger_name)  ||  r.read_int32(head_photo_id)  ||  r.read_int8(is_zhizunbao)  ||  r.read_int32(last_sign_out_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		week_contribute = 0;
		total_contribute = 0;
		fight = 0;
		amplify_times = 0;
		level = 0;
		vip = 0;
		headship = 0;
		authority.clear();
		career = 0;
		line_stat = 0;
		gender = 0;
		ganger_name.clear();
		head_photo_id = 0;
		is_zhizunbao = 0;
		last_sign_out_time = 0;
	}
};

/*
公会动态结构
备注：
打包方式
0 - uint32 
1 - uint8 
2 - role_info 
3 - text
*/
struct Gang_Passed_Event_Data {
	int8_t pack_type;/**/
	uint32_t value32;/**/
	uint8_t value8;/**/
	std::string text;
	Role_Info role_info;
	Gang_Passed_Event_Data(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(pack_type);
		if ( pack_type == 0 ) {
			w.write_uint32(value32);
		}
		else if ( pack_type == 1 ) {
			w.write_uint8(value8);
		}
		else if ( pack_type == 2 ) {
			role_info.serialize(w);
		}
		else if ( pack_type == 3 ) {
			w.write_string(text);
		}
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(pack_type) )
				return -1;
		if ( pack_type == 0 ) {
			if ( r.read_uint32(value32) ) return -1;
		}
		else if ( pack_type == 1 ) {
			if ( r.read_uint8(value8) ) return -1;
		}
		else if ( pack_type == 2 ) {
			if ( role_info.deserialize(r) ) return -1;
		}
		else if ( pack_type == 3 ) {
			if ( r.read_string(text) ) return -1;
		}
 		return 0;
	}

	void reset(void) {
		pack_type = 0;
		value32 = 0;
		value8 = 0;
		text.clear();
		role_info.reset();
	}
};
/*
公会动态信息
备注：
101900 加入公会：roleInfo1批准roleInfo2加入公会。数据:[roleInfo1,roleInfo2]
101901 职位变化：roleInfo1任命roleInfo2为公会官员/副会长/成员  数据:[roleInfo1,roleInfo2,职位id]
101902 退出公会：roleInfo退出了公会。   数据:[roleInfo]
101903 踢出公会：roleInfo1把roleInfo2踢出了公会   data:[roleInfo1,roleInfo2]
101904 公会升级：XXX消耗XXX公会财富，将公会等级升到了X级  data:[roleInfo,int8,int8]

101905 建筑升级：XXX消耗XXX公会财富，将XXX升到了X级。  data:[roleInfo,int8,text,int8] text=功能的名称

101906 公会转让：会长roleInfo1将公会转让给roleInfo2管理。 data:[roleInfo1,roleInfo2] 
101907 公会夺权：roleInfo1发动了公会夺权，成为公会新的会长，roleInfo2职位由会长变为会员。data:[roleInfo1,roleInfo2] 
101908 公会排名：公会排名变为X。data:[int8]
101909 公会维护费用不足：公会维护费用不足，X日后仍不足扣取，公会将解散。 data:[int8]
101910 公告修改：XXX修改了公会公告  data:[roleInfo]

公会战报名：XXX报名了公会战，将在XX日XX时开启，请如期参战。event_id:101911  data:[roleInfo],int32]
公会战开启：公会战已开启。event_id:101912  data:[]
公会战结果：本公会在此次公会战中胜利了/失败了。event_id:101913  data:[text] text = "失败了"or"胜利了"
公会战排名：本公会在此次公会战中，排名第X。event_id:101914  data:[int8]
公会boss开启：XXX开启了公会boss。event_id:1019145  data:[roleInfo]
*/
struct Gang_Passed_Event {
	int32_t event_id;/**/
	uint32_t time;/**/
	std::vector<Gang_Passed_Event_Data> event_data;
	Gang_Passed_Event(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(event_id);
		w.write_uint32(time);
		uint16_t __event_data_vec_size = event_data.size();
		w.write_uint16(__event_data_vec_size);
		for(uint16_t i = 0; i < __event_data_vec_size; ++i) {
				event_data[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(event_id)  ||  r.read_uint32(time)  )
				return -1;
 		uint16_t __event_data_vec_size;
		if(r.read_uint16(__event_data_vec_size) )
				return -1;
		event_data.reserve(__event_data_vec_size);
		for(uint16_t i = 0; i < __event_data_vec_size; ++i) {
				Gang_Passed_Event_Data v;
				if(v.deserialize(r))
						return -1;
				event_data.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		event_id = 0;
		time = 0;
		event_data.clear();
	}
};

/*
任务随身怪
*/
struct NPC_Appear_Info_Task {
	NPC_Appear_Info npc_info;
	std::string funcName;/**/
	int32_t direction;/*静态npc朝向*/
	std::string sceneId;/*静态npc场景*/
	std::string open;/**/
	NPC_Appear_Info_Task(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		npc_info.serialize(w);
		w.write_string(funcName);
		w.write_int32(direction);
		w.write_string(sceneId);
		w.write_string(open);
	}

	int deserialize(Block_Buffer & r) {
		if( npc_info.deserialize(r)  ||  r.read_string(funcName)  ||  r.read_int32(direction)  ||  r.read_string(sceneId)  ||  r.read_string(open)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		npc_info.reset();
		funcName.clear();
		direction = 0;
		sceneId.clear();
		open.clear();
	}
};

/*
公会建筑信息
*/
struct Gang_Building_Info {
	int32_t building_id;/*建筑ID*/
	int32_t building_lvl;/*建筑等级*/
	int32_t upgrade_cd;/*下次可升级时间戳*/
	Gang_Building_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(building_id);
		w.write_int32(building_lvl);
		w.write_int32(upgrade_cd);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(building_id)  ||  r.read_int32(building_lvl)  ||  r.read_int32(upgrade_cd)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		building_id = 0;
		building_lvl = 0;
		upgrade_cd = 0;
	}
};

/*
公会技能信息
*/
struct Gang_Skill_Info {
	int32_t skill_id;/*公会技能ID*/
	Gang_Skill_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
	}
};

/*
公会信息
*/
struct Gang_Info {
	int64_t gang_id;/**/
	int64_t leader;/**/
	uint32_t fund;/*财富*/
	uint32_t maintain_cost;/*维护费用*/
	uint32_t cd_upgrade;/*下次可升级时间*/
	int32_t level;/**/
	int32_t cur_members;/**/
	int32_t max_members;/**/
	int32_t rank;/*排名*/
	int8_t logo;/**/
	int8_t join_block;/*0-允许所有加入，1-需要批准进入*/
	std::string leader_name;/**/
	std::string gang_name;/**/
	std::string announce;/*公告*/
	int8_t can_seize_power;/*是否可夺权*/
	std::vector<Gang_Skill_Info> researched_skills;
	std::vector<Gang_Building_Info> building_infos;
	int32_t force;/*公会战力*/
	Gang_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int64(leader);
		w.write_uint32(fund);
		w.write_uint32(maintain_cost);
		w.write_uint32(cd_upgrade);
		w.write_int32(level);
		w.write_int32(cur_members);
		w.write_int32(max_members);
		w.write_int32(rank);
		w.write_int8(logo);
		w.write_int8(join_block);
		w.write_string(leader_name);
		w.write_string(gang_name);
		w.write_string(announce);
		w.write_int8(can_seize_power);
		uint16_t __researched_skills_vec_size = researched_skills.size();
		w.write_uint16(__researched_skills_vec_size);
		for(uint16_t i = 0; i < __researched_skills_vec_size; ++i) {
				researched_skills[i].serialize(w);
		}

		uint16_t __building_infos_vec_size = building_infos.size();
		w.write_uint16(__building_infos_vec_size);
		for(uint16_t i = 0; i < __building_infos_vec_size; ++i) {
				building_infos[i].serialize(w);
		}

		w.write_int32(force);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int64(leader)  ||  r.read_uint32(fund)  ||  r.read_uint32(maintain_cost)  ||  r.read_uint32(cd_upgrade)  ||  r.read_int32(level)  ||  r.read_int32(cur_members)  ||  r.read_int32(max_members)  ||  r.read_int32(rank)  ||  r.read_int8(logo)  ||  r.read_int8(join_block)  ||  r.read_string(leader_name)  ||  r.read_string(gang_name)  ||  r.read_string(announce)  ||  r.read_int8(can_seize_power)  )
				return -1;
 		uint16_t __researched_skills_vec_size;
		if(r.read_uint16(__researched_skills_vec_size) )
				return -1;
		researched_skills.reserve(__researched_skills_vec_size);
		for(uint16_t i = 0; i < __researched_skills_vec_size; ++i) {
				Gang_Skill_Info v;
				if(v.deserialize(r))
						return -1;
				researched_skills.push_back(v);
		}

		uint16_t __building_infos_vec_size;
		if(r.read_uint16(__building_infos_vec_size) )
				return -1;
		building_infos.reserve(__building_infos_vec_size);
		for(uint16_t i = 0; i < __building_infos_vec_size; ++i) {
				Gang_Building_Info v;
				if(v.deserialize(r))
						return -1;
				building_infos.push_back(v);
		}

		if( r.read_int32(force)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		leader = 0;
		fund = 0;
		maintain_cost = 0;
		cd_upgrade = 0;
		level = 0;
		cur_members = 0;
		max_members = 0;
		rank = 0;
		logo = 0;
		join_block = 0;
		leader_name.clear();
		gang_name.clear();
		announce.clear();
		can_seize_power = 0;
		researched_skills.clear();
		building_infos.clear();
		force = 0;
	}
};

/*
玩家公会技能信息
*/
struct Ganger_Skill_Info {
	int32_t skill_id;/*公会技能ID*/
	Ganger_Skill_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
	}
};

/*
玩家公会信息
*/
struct Ganger_Info {
	int64_t gang_id;/**/
	int64_t role_id;/**/
	int32_t current_contribute;/*当前剩余贡献*/
	uint32_t total_contribute;/*历史总贡献*/
	uint32_t week_contribute;/*本周贡献*/
	uint32_t day_contribute;/*今日贡献*/
	int32_t gold_contrib_times;/**/
	int32_t diam_contrib_times;/**/
	int32_t rank;/**/
	int8_t headship;/*公会职务，0-普通成员，4-会长，3-副会长，2-长老，1-官员*/
	std::string gang_name;/**/
	std::vector<int8_t> authority;/**/
	int8_t receive_bless_times;/*已接受祝福buf次数*/
	std::vector<Ganger_Skill_Info> skill_infos;
	int32_t remain_draw_times;/*剩余抽奖次数*/
	std::vector<int32_t> draw_awards;/*抽奖抽到但未领取的奖品*/
	int32_t phy_power_ttl;/*当天还可获利体力值*/
	Ganger_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_int64(role_id);
		w.write_int32(current_contribute);
		w.write_uint32(total_contribute);
		w.write_uint32(week_contribute);
		w.write_uint32(day_contribute);
		w.write_int32(gold_contrib_times);
		w.write_int32(diam_contrib_times);
		w.write_int32(rank);
		w.write_int8(headship);
		w.write_string(gang_name);
		uint16_t __authority_vec_size = authority.size();
		w.write_uint16(__authority_vec_size);
		for(uint16_t i = 0; i < __authority_vec_size; ++i) {
				w.write_int8(authority[i]);
		}

		w.write_int8(receive_bless_times);
		uint16_t __skill_infos_vec_size = skill_infos.size();
		w.write_uint16(__skill_infos_vec_size);
		for(uint16_t i = 0; i < __skill_infos_vec_size; ++i) {
				skill_infos[i].serialize(w);
		}

		w.write_int32(remain_draw_times);
		uint16_t __draw_awards_vec_size = draw_awards.size();
		w.write_uint16(__draw_awards_vec_size);
		for(uint16_t i = 0; i < __draw_awards_vec_size; ++i) {
				w.write_int32(draw_awards[i]);
		}

		w.write_int32(phy_power_ttl);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_int64(role_id)  ||  r.read_int32(current_contribute)  ||  r.read_uint32(total_contribute)  ||  r.read_uint32(week_contribute)  ||  r.read_uint32(day_contribute)  ||  r.read_int32(gold_contrib_times)  ||  r.read_int32(diam_contrib_times)  ||  r.read_int32(rank)  ||  r.read_int8(headship)  ||  r.read_string(gang_name)  )
				return -1;
 		uint16_t __authority_vec_size;
		if(r.read_uint16(__authority_vec_size) )
				return -1;
		authority.reserve(__authority_vec_size);
		for(uint16_t i = 0; i < __authority_vec_size; ++i) {
				int8_t v;
				if(r.read_int8(v) )
						return -1;
				authority.push_back(v);
		}

		if( r.read_int8(receive_bless_times)  )
				return -1;
 		uint16_t __skill_infos_vec_size;
		if(r.read_uint16(__skill_infos_vec_size) )
				return -1;
		skill_infos.reserve(__skill_infos_vec_size);
		for(uint16_t i = 0; i < __skill_infos_vec_size; ++i) {
				Ganger_Skill_Info v;
				if(v.deserialize(r))
						return -1;
				skill_infos.push_back(v);
		}

		if( r.read_int32(remain_draw_times)  )
				return -1;
 		uint16_t __draw_awards_vec_size;
		if(r.read_uint16(__draw_awards_vec_size) )
				return -1;
		draw_awards.reserve(__draw_awards_vec_size);
		for(uint16_t i = 0; i < __draw_awards_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				draw_awards.push_back(v);
		}

		if( r.read_int32(phy_power_ttl)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		role_id = 0;
		current_contribute = 0;
		total_contribute = 0;
		week_contribute = 0;
		day_contribute = 0;
		gold_contrib_times = 0;
		diam_contrib_times = 0;
		rank = 0;
		headship = 0;
		gang_name.clear();
		authority.clear();
		receive_bless_times = 0;
		skill_infos.clear();
		remain_draw_times = 0;
		draw_awards.clear();
		phy_power_ttl = 0;
	}
};

/*
符文孔信息
*/
struct Rune_Hole_Info {
	int32_t index;/*索引[1开始]*/
	int32_t level;/*等级*/
	int32_t energy;/*能量*/
	Rune_Hole_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int32(level);
		w.write_int32(energy);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int32(level)  ||  r.read_int32(energy)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		level = 0;
		energy = 0;
	}
};

/*
符文部位信息
*/
struct Rune_Part_Info {
	int32_t part;/*符文部位[默认值是身上装备index,属性类型配置是装备部位]*/
	std::vector<Rune_Hole_Info> hole_infos;
	Rune_Part_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(part);
		uint16_t __hole_infos_vec_size = hole_infos.size();
		w.write_uint16(__hole_infos_vec_size);
		for(uint16_t i = 0; i < __hole_infos_vec_size; ++i) {
				hole_infos[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(part)  )
				return -1;
 		uint16_t __hole_infos_vec_size;
		if(r.read_uint16(__hole_infos_vec_size) )
				return -1;
		hole_infos.reserve(__hole_infos_vec_size);
		for(uint16_t i = 0; i < __hole_infos_vec_size; ++i) {
				Rune_Hole_Info v;
				if(v.deserialize(r))
						return -1;
				hole_infos.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		part = 0;
		hole_infos.clear();
	}
};

/*
竞技场排行榜信息
*/
struct Arena_Rank_Info {
	uint32_t fight;/*战斗力*/
	uint16_t score;/*积分*/
	uint8_t seg;/*段（1段，2段....）*/
	uint8_t group;/*组（青铜...)*/
	uint32_t rank;/*排名*/
	uint16_t level;/*等级*/
	int64_t role_id;/*role_id*/
	std::string name;/*名称*/
	std::string server_name;/*服务器名称*/
	Arena_Rank_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(fight);
		w.write_uint16(score);
		w.write_uint8(seg);
		w.write_uint8(group);
		w.write_uint32(rank);
		w.write_uint16(level);
		w.write_int64(role_id);
		w.write_string(name);
		w.write_string(server_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(fight)  ||  r.read_uint16(score)  ||  r.read_uint8(seg)  ||  r.read_uint8(group)  ||  r.read_uint32(rank)  ||  r.read_uint16(level)  ||  r.read_int64(role_id)  ||  r.read_string(name)  ||  r.read_string(server_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		fight = 0;
		score = 0;
		seg = 0;
		group = 0;
		rank = 0;
		level = 0;
		role_id = 0;
		name.clear();
		server_name.clear();
	}
};

/*
翻牌结果
*/
struct Reward_Info_List {
	int32_t index;/**/
	Role_Team_Info role_info;
	std::vector<Reward_Info> award;
	Reward_Info_List(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		role_info.serialize(w);
		uint16_t __award_vec_size = award.size();
		w.write_uint16(__award_vec_size);
		for(uint16_t i = 0; i < __award_vec_size; ++i) {
				award[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  role_info.deserialize(r)  )
				return -1;
 		uint16_t __award_vec_size;
		if(r.read_uint16(__award_vec_size) )
				return -1;
		award.reserve(__award_vec_size);
		for(uint16_t i = 0; i < __award_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				award.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		index = 0;
		role_info.reset();
		award.clear();
	}
};

/*
功能图标开启信息
*/
struct Icon_Info {
	uint16_t id;/*图标id*/
	uint8_t status;/*开启状态。1-开启，0-未开启*/
	Icon_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(id);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(id)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		status = 0;
	}
};

/*
竞技场战报
*/
struct Arena_Battle_Report {
	uint8_t result;/*战斗结果 0：挑战成功 1：挑战失败 2：晋级成功 3：晋级失败 4：连胜 6:排名提升 7：排名下降*/
	int16_t score;/*积分*/
	uint16_t gold;/*普通是金钱，如果是晋级成功是英魂*/
	std::vector<Reward_Info> item_vec;
	int16_t val1;/*晋级成功的为组值；连胜为连胜次数；排名提升、下降为名次*/
	int16_t val2;/*晋级成功的为段值*/
	std::string role_name;/*挑战玩家名称*/
	Arena_Battle_Report(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(result);
		w.write_int16(score);
		w.write_uint16(gold);
		uint16_t __item_vec_vec_size = item_vec.size();
		w.write_uint16(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				item_vec[i].serialize(w);
		}

		w.write_int16(val1);
		w.write_int16(val2);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(result)  ||  r.read_int16(score)  ||  r.read_uint16(gold)  )
				return -1;
 		uint16_t __item_vec_vec_size;
		if(r.read_uint16(__item_vec_vec_size) )
				return -1;
		item_vec.reserve(__item_vec_vec_size);
		for(uint16_t i = 0; i < __item_vec_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				item_vec.push_back(v);
		}

		if( r.read_int16(val1)  ||  r.read_int16(val2)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		result = 0;
		score = 0;
		gold = 0;
		item_vec.clear();
		val1 = 0;
		val2 = 0;
		role_name.clear();
	}
};

/*
排行榜信息
*/
struct Ranking_Info {
	int32_t rank;/*名次*/
	int64_t role_id;/*角色ID*/
	std::string role_name;/*角色名称*/
	int32_t career;/*职业 1-4	([1,4])*/
	std::string server;/*所在地*/
	double ext_1;/*经验*/
	std::string ext_2;/*装备名 or 公会名 or 英雄名*/
	int32_t ext_3;/*装备评分	角色等级	积分	公会等级	英雄等级 性别*/
	int32_t ext_4;/*金币	英雄战斗力	公会贡献	角色战斗力	通关数	层数   公会人数  魅力值   友情值*/
	int32_t ext_5;/*VIP等级*/
	int32_t ext_6;/*英雄ID  翅膀阶级  坐骑总星数*/
	int32_t ext_7;/*英雄索引*/
	int64_t ext_8;/*保留*/
	Ranking_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(rank);
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int32(career);
		w.write_string(server);
		w.write_double(ext_1);
		w.write_string(ext_2);
		w.write_int32(ext_3);
		w.write_int32(ext_4);
		w.write_int32(ext_5);
		w.write_int32(ext_6);
		w.write_int32(ext_7);
		w.write_int64(ext_8);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(rank)  ||  r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int32(career)  ||  r.read_string(server)  ||  r.read_double(ext_1)  ||  r.read_string(ext_2)  ||  r.read_int32(ext_3)  ||  r.read_int32(ext_4)  ||  r.read_int32(ext_5)  ||  r.read_int32(ext_6)  ||  r.read_int32(ext_7)  ||  r.read_int64(ext_8)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		rank = 0;
		role_id = 0;
		role_name.clear();
		career = 0;
		server.clear();
		ext_1 = 0.0;
		ext_2.clear();
		ext_3 = 0;
		ext_4 = 0;
		ext_5 = 0;
		ext_6 = 0;
		ext_7 = 0;
		ext_8 = 0;
	}
};

/*
公会邀请信息
*/
struct Gang_Invite_Info {
	int64_t gang_id;/*公会ID*/
	std::string gang_name;/*公会名称*/
	std::string leader;/*会长角色名*/
	int32_t level;/*公会等级*/
	int32_t cur_mem_num;/*当前公会人数*/
	int32_t max_mem_num;/*公会人数上限*/
	int32_t time_sec;/*邀请时间*/
	Gang_Invite_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_string(gang_name);
		w.write_string(leader);
		w.write_int32(level);
		w.write_int32(cur_mem_num);
		w.write_int32(max_mem_num);
		w.write_int32(time_sec);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_string(gang_name)  ||  r.read_string(leader)  ||  r.read_int32(level)  ||  r.read_int32(cur_mem_num)  ||  r.read_int32(max_mem_num)  ||  r.read_int32(time_sec)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		gang_name.clear();
		leader.clear();
		level = 0;
		cur_mem_num = 0;
		max_mem_num = 0;
		time_sec = 0;
	}
};

/*
公会申请信息
*/
struct Gang_Apply_Info {
	int64_t role_id;/*角色ID*/
	std::string role_name;/*角色名*/
	int32_t level;/*角色等级*/
	int8_t gender;/*性别*/
	int8_t career;/*职业*/
	int8_t vip;/*VIP类型*/
	int32_t combat_power;/*战力*/
	int32_t time_sec;/*申请时间*/
	Gang_Apply_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int32(level);
		w.write_int8(gender);
		w.write_int8(career);
		w.write_int8(vip);
		w.write_int32(combat_power);
		w.write_int32(time_sec);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int32(level)  ||  r.read_int8(gender)  ||  r.read_int8(career)  ||  r.read_int8(vip)  ||  r.read_int32(combat_power)  ||  r.read_int32(time_sec)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		level = 0;
		gender = 0;
		career = 0;
		vip = 0;
		combat_power = 0;
		time_sec = 0;
	}
};

/*
精英副本信息
*/
struct Elite_Dungeon_Info {
	int32_t id;/*//副本id*/
	int8_t status;/*//副本状态（0：未解锁 1:激活 2：未激活 3：打通的 4：打过的）*/
	int32_t best_force;/*//最优通关战斗力（0：没有人通关过该副本）*/
	int64_t best_role_id;/*//最优通关玩家*/
	std::string best_role_name;/*//最优通关玩家*/
	uint8_t add_num;/*//额外购买次数*/
	Elite_Dungeon_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int8(status);
		w.write_int32(best_force);
		w.write_int64(best_role_id);
		w.write_string(best_role_name);
		w.write_uint8(add_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int8(status)  ||  r.read_int32(best_force)  ||  r.read_int64(best_role_id)  ||  r.read_string(best_role_name)  ||  r.read_uint8(add_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		status = 0;
		best_force = 0;
		best_role_id = 0;
		best_role_name.clear();
		add_num = 0;
	}
};

/*
聊天物品信息结构
备注：type 1:装备 4:符石 5:卡牌
*/
struct Chat_Item_Info {
	int8_t type;/*物品类型 1:装备 4:符石 5:卡牌*/
	Item_Basic_Info item_base_info;
	Item_Equip_Info equip_info;
	Rune_Part_Info rune_part_info;
	Item_Rune_Stone_Info rune_stone_info;
	Card_Info card_info;
	Chat_Item_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
		item_base_info.serialize(w);

		if (type == 1) {
			equip_info.serialize(w);
			rune_part_info.serialize(w);
		} else if (type == 4) {
			rune_stone_info.serialize(w);
		} else if (type == 5) {
			card_info.serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type)  ||  item_base_info.deserialize(r) )
				return -1;
		if (type == 1) {
			if (equip_info.deserialize(r) || rune_part_info.deserialize(r))
				return -1;
		} else if (type == 4) {
			rune_stone_info.deserialize(r);
		} else if (type == 5) {
			card_info.deserialize(r);
		}

 		return 0;
	}

	void reset(void) {
		type = 0;
		item_base_info.reset();
		equip_info.reset();
		rune_part_info.reset();
		rune_stone_info.reset();
		card_info.reset();
	}
};
/*
聊天结构
备注：
type:
0 纯文字
1 道具
*/
struct Chat_Content {
	int8_t type;/**/
	std::string text;/*纯文字*/
	Chat_Item_Info item;
        Role_Info role_Info;
	int32_t prop_id;/*属性ID*/
	Chat_Content(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(type);
                if(type==0){
		    w.write_string(text);
                }else if(type==1){
		    item.serialize(w);
                }else if(type==2){
		    role_Info.serialize(w);
                }else if(type==3){
		   w.write_int32(prop_id);
                }
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(type) )
				return -1;
                if(type==0){
                      if(r.read_string(text) ) return -1;
                }else if(type==1){
                      if(item.deserialize(r)) return -1;
                }else if(type==2){
                      if(role_Info.deserialize(r)) return -1;
                }else if(type==3){
                      if(r.read_int32(prop_id)) return -1;
                }
 		return 0;
	}

	void reset(void) {
		type = 0;
		text.clear();
		item.reset();
                role_Info.reset();
                prop_id = 0;
	}
};
/*
聊天记录
*/
struct Chat_Record {
	std::vector<Chat_Content> content;
	uint8_t chat_type;/*聊天类型（1：世界 2：公会；3:组队；4：系统 5：喇叭 6:私聊）*/
	uint32_t send_time;/*发送时间*/
	Chat_Record(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __content_vec_size = content.size();
		w.write_uint16(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				content[i].serialize(w);
		}

		w.write_uint8(chat_type);
		w.write_uint32(send_time);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __content_vec_size;
		if(r.read_uint16(__content_vec_size) )
				return -1;
		content.reserve(__content_vec_size);
		for(uint16_t i = 0; i < __content_vec_size; ++i) {
				Chat_Content v;
				if(v.deserialize(r))
						return -1;
				content.push_back(v);
		}

		if( r.read_uint8(chat_type)  ||  r.read_uint32(send_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		content.clear();
		chat_type = 0;
		send_time = 0;
	}
};

/*
匹配面板信息
*/
struct Team_Match_Info {
	int64_t role_id;/**/
	std::string role_name;/**/
	int32_t level;/**/
	int32_t head_photo_id;/**/
	int32_t hero_photo_id;/*英雄头像id*/
	std::string zone;/*区服*/
	Team_Match_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int32(level);
		w.write_int32(head_photo_id);
		w.write_int32(hero_photo_id);
		w.write_string(zone);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int32(level)  ||  r.read_int32(head_photo_id)  ||  r.read_int32(hero_photo_id)  ||  r.read_string(zone)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		level = 0;
		head_photo_id = 0;
		hero_photo_id = 0;
		zone.clear();
	}
};

/*
多人竞技积分信息
*/
struct Team_Sports_Integrate {
	int64_t role_id;/**/
	int32_t integrate;/*积分*/
	int32_t level;/*等级*/
	std::string name;/*角色名*/
	int32_t index;/*名次*/
	Team_Sports_Integrate(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int32(integrate);
		w.write_int32(level);
		w.write_string(name);
		w.write_int32(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int32(integrate)  ||  r.read_int32(level)  ||  r.read_string(name)  ||  r.read_int32(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		integrate = 0;
		level = 0;
		name.clear();
		index = 0;
	}
};

/*
战场单个信息
*/
struct War_Pro {
	int64_t role_id;/**/
	uint16_t score;/*积分*/
	uint16_t kill_num;/*击杀数*/
	uint16_t max_streak_kill_num;/*最大连杀数*/
	uint16_t assists_num;/*助攻数*/
	uint16_t merit;/*功勋*/
	uint16_t total_merit;/*累计功勋*/
	uint16_t heal_num;/*治疗次数*/
	uint16_t revive_time;/*复活倒计时*/
	int8_t league;/*玩家阵营 1：光明 2：黑暗*/
	uint16_t reputation;/*声望值*/
	War_Pro(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint16(score);
		w.write_uint16(kill_num);
		w.write_uint16(max_streak_kill_num);
		w.write_uint16(assists_num);
		w.write_uint16(merit);
		w.write_uint16(total_merit);
		w.write_uint16(heal_num);
		w.write_uint16(revive_time);
		w.write_int8(league);
		w.write_uint16(reputation);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint16(score)  ||  r.read_uint16(kill_num)  ||  r.read_uint16(max_streak_kill_num)  ||  r.read_uint16(assists_num)  ||  r.read_uint16(merit)  ||  r.read_uint16(total_merit)  ||  r.read_uint16(heal_num)  ||  r.read_uint16(revive_time)  ||  r.read_int8(league)  ||  r.read_uint16(reputation)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		score = 0;
		kill_num = 0;
		max_streak_kill_num = 0;
		assists_num = 0;
		merit = 0;
		total_merit = 0;
		heal_num = 0;
		revive_time = 0;
		league = 0;
		reputation = 0;
	}
};

/*
战场排名信息
*/
struct War_Player_Rank_Info {
	std::string role_name;/*角色名称*/
	int64_t role_id;/*角色role_id*/
	int8_t league;/*1:光明 2：黑暗*/
	uint16_t score;/*积分*/
	uint16_t kill_num;/*击杀数*/
	uint16_t assists_num;/*助攻数*/
	uint16_t merit;/*功勋*/
	uint8_t rank;/*名次*/
	std::string server_name;/*服务器名称*/
	uint8_t max_kill;/*最大连杀数*/
	uint16_t reputation;/*声望值*/
	War_Player_Rank_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
		w.write_int64(role_id);
		w.write_int8(league);
		w.write_uint16(score);
		w.write_uint16(kill_num);
		w.write_uint16(assists_num);
		w.write_uint16(merit);
		w.write_uint8(rank);
		w.write_string(server_name);
		w.write_uint8(max_kill);
		w.write_uint16(reputation);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  ||  r.read_int64(role_id)  ||  r.read_int8(league)  ||  r.read_uint16(score)  ||  r.read_uint16(kill_num)  ||  r.read_uint16(assists_num)  ||  r.read_uint16(merit)  ||  r.read_uint8(rank)  ||  r.read_string(server_name)  ||  r.read_uint8(max_kill)  ||  r.read_uint16(reputation)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_name.clear();
		role_id = 0;
		league = 0;
		score = 0;
		kill_num = 0;
		assists_num = 0;
		merit = 0;
		rank = 0;
		server_name.clear();
		max_kill = 0;
		reputation = 0;
	}
};

/*
龙谷内建筑信息
备注： status: 建筑状态     
                      0:不是升级中，也不是升级完成，统一填0   1:升级中  2:升级完成
*/
struct Building_Info {
	uint32_t id;/*建筑id*/
	uint8_t type;/*建筑类型  1:金矿  2:月亮井 3:图腾*/
	uint16_t level;/*建筑等级*/
	uint8_t status;/*建筑状态*/
	uint32_t production;/*当前资源产出量*/
	uint32_t production_per_minute;/*每分钟产量*/
	uint8_t is_gain;/*是否可收获资源   1:可收获  0: 不可收获(客户端显示用)*/
	uint32_t left_time;/*升级剩余时间   单位：秒*/
	uint8_t order;/*建造顺序*/
	uint8_t consume_type;/*消耗类型*/
	Building_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(type);
		w.write_uint16(level);
		w.write_uint8(status);
		w.write_uint32(production);
		w.write_uint32(production_per_minute);
		w.write_uint8(is_gain);
		w.write_uint32(left_time);
		w.write_uint8(order);
		w.write_uint8(consume_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(type)  ||  r.read_uint16(level)  ||  r.read_uint8(status)  ||  r.read_uint32(production)  ||  r.read_uint32(production_per_minute)  ||  r.read_uint8(is_gain)  ||  r.read_uint32(left_time)  ||  r.read_uint8(order)  ||  r.read_uint8(consume_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		type = 0;
		level = 0;
		status = 0;
		production = 0;
		production_per_minute = 0;
		is_gain = 0;
		left_time = 0;
		order = 0;
		consume_type = 0;
	}
};

/*
技能天赋
*/
struct Skill_Talent_Info {
	int32_t talent_id;/*天赋id*/
	int32_t skill_point;/*技能点*/
	std::vector<Skill_Info> skill_info_vec;
	Skill_Talent_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(talent_id);
		w.write_int32(skill_point);
		uint16_t __skill_info_vec_vec_size = skill_info_vec.size();
		w.write_uint16(__skill_info_vec_vec_size);
		for(uint16_t i = 0; i < __skill_info_vec_vec_size; ++i) {
				skill_info_vec[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(talent_id)  ||  r.read_int32(skill_point)  )
				return -1;
 		uint16_t __skill_info_vec_vec_size;
		if(r.read_uint16(__skill_info_vec_vec_size) )
				return -1;
		skill_info_vec.reserve(__skill_info_vec_vec_size);
		for(uint16_t i = 0; i < __skill_info_vec_vec_size; ++i) {
				Skill_Info v;
				if(v.deserialize(r))
						return -1;
				skill_info_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		talent_id = 0;
		skill_point = 0;
		skill_info_vec.clear();
	}
};

/*
实力提升获取方式数据
*/
struct Upgrade_Get_Info {
	uint32_t id;/*获取方式ID*/
	uint32_t time;/*时间*/
	uint8_t status;/*0.不可参与(已过期);1.可参与(未开启);2.进行中*/
	uint32_t cur_num;/*当前次数*/
	uint32_t all_num;/*总次数*/
	Upgrade_Get_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(time);
		w.write_uint8(status);
		w.write_uint32(cur_num);
		w.write_uint32(all_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(time)  ||  r.read_uint8(status)  ||  r.read_uint32(cur_num)  ||  r.read_uint32(all_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		time = 0;
		status = 0;
		cur_num = 0;
		all_num = 0;
	}
};

/*
龙谷脚印信息
*/
struct Dragon_Vale_Foot_Info {
	uint8_t sex;/*性别    1:男   2:女*/
	std::string name;/*角色 名*/
	Dragon_Vale_Foot_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(sex);
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(sex)  ||  r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		sex = 0;
		name.clear();
	}
};

/*
龙谷头像区信息
*/
struct Dragon_Vale_Head_Info {
	int64_t role_id;/*龙谷的主人角色id*/
	std::string role_name;/*龙谷的主人角色名*/
	uint8_t star_num;/*当前星的个数  ：  0 - 5*/
	uint32_t head_photo_id;/*头像id*/
	uint16_t star_add_speed;/*星级产速提升(如：提升20%  ，值为20)*/
	uint16_t thief_sub_speed;/*沙鼠偷猎者降低产速(如：降低20%， 值为20)*/
	std::vector<Dragon_Vale_Foot_Info> foot_list;
	Dragon_Vale_Head_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_uint8(star_num);
		w.write_uint32(head_photo_id);
		w.write_uint16(star_add_speed);
		w.write_uint16(thief_sub_speed);
		uint16_t __foot_list_vec_size = foot_list.size();
		w.write_uint16(__foot_list_vec_size);
		for(uint16_t i = 0; i < __foot_list_vec_size; ++i) {
				foot_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_uint8(star_num)  ||  r.read_uint32(head_photo_id)  ||  r.read_uint16(star_add_speed)  ||  r.read_uint16(thief_sub_speed)  )
				return -1;
 		uint16_t __foot_list_vec_size;
		if(r.read_uint16(__foot_list_vec_size) )
				return -1;
		foot_list.reserve(__foot_list_vec_size);
		for(uint16_t i = 0; i < __foot_list_vec_size; ++i) {
				Dragon_Vale_Foot_Info v;
				if(v.deserialize(r))
						return -1;
				foot_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		star_num = 0;
		head_photo_id = 0;
		star_add_speed = 0;
		thief_sub_speed = 0;
		foot_list.clear();
	}
};

/*
有龙谷的好友或者公会成员信息
*/
struct Dragon_Vale_Member {
	int64_t role_id;/*角色id*/
	uint32_t head_photo_id;/*头像id*/
	uint8_t vip_level;/*vip等级*/
	std::string name;/*角色名*/
	uint8_t is_special;/*是否至尊宝 1：是， 0：否*/
	uint8_t headship;/*公会职务 0:普通会员 1:官员 2:副会长 3：会长*/
	uint8_t flag;/*标志  1：好友成员  2：公会成员*/
	uint8_t op;/*玩家操作  0: 无操作  1:踩脚印 2：逗龙 3：驱赶*/
	Dragon_Vale_Member(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint32(head_photo_id);
		w.write_uint8(vip_level);
		w.write_string(name);
		w.write_uint8(is_special);
		w.write_uint8(headship);
		w.write_uint8(flag);
		w.write_uint8(op);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint32(head_photo_id)  ||  r.read_uint8(vip_level)  ||  r.read_string(name)  ||  r.read_uint8(is_special)  ||  r.read_uint8(headship)  ||  r.read_uint8(flag)  ||  r.read_uint8(op)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		head_photo_id = 0;
		vip_level = 0;
		name.clear();
		is_special = 0;
		headship = 0;
		flag = 0;
		op = 0;
	}
};

/*
龙信息
*/
struct Dragon_Info {
	uint8_t progress;/*进化程度*/
	uint8_t feeling;/*当前心情值  (最大值为5，这里用uint8_t足够)*/
	uint32_t power;/*当前主角斗力值*/
	uint32_t armor;/*当前主角护甲*/
	uint32_t stamina;/*当前主角耐力*/
	uint32_t power_level;/*当前主角斗力等级*/
	uint32_t armor_level;/*当前主角护甲等级*/
	uint32_t stamina_level;/*当前主角耐力等级*/
	uint32_t hero_power;/*当前英雄斗力值*/
	uint32_t hero_armor;/*当前英雄护甲*/
	uint32_t hero_stamina;/*当前英雄耐力*/
	uint32_t hero_power_level;/*当前英雄斗力等级*/
	int32_t hero_armor_level;/*当前英雄护甲等级*/
	int32_t hero_stamina_level;/*当前英雄耐力等级*/
	uint8_t is_play_dragon;/*是否可以逗龙 1:可以 0 :不可以*/
	uint32_t energy;/*当前能量值*/
	uint32_t crit;/*主角暴击值*/
	uint32_t prevent_crit;/*主角抗暴值*/
	uint32_t crit_level;/*主角暴击等级*/
	uint32_t prevent_crit_level;/*主角抗暴等级*/
	uint32_t hero_crit;/*英雄暴击值*/
	uint32_t hero_prevent_crit;/*英雄抗暴值*/
	uint32_t hero_crit_level;/*英雄暴击等级*/
	uint32_t hero_prevent_crit_level;/*英雄抗暴等级*/
	Dragon_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(progress);
		w.write_uint8(feeling);
		w.write_uint32(power);
		w.write_uint32(armor);
		w.write_uint32(stamina);
		w.write_uint32(power_level);
		w.write_uint32(armor_level);
		w.write_uint32(stamina_level);
		w.write_uint32(hero_power);
		w.write_uint32(hero_armor);
		w.write_uint32(hero_stamina);
		w.write_uint32(hero_power_level);
		w.write_int32(hero_armor_level);
		w.write_int32(hero_stamina_level);
		w.write_uint8(is_play_dragon);
		w.write_uint32(energy);
		w.write_uint32(crit);
		w.write_uint32(prevent_crit);
		w.write_uint32(crit_level);
		w.write_uint32(prevent_crit_level);
		w.write_uint32(hero_crit);
		w.write_uint32(hero_prevent_crit);
		w.write_uint32(hero_crit_level);
		w.write_uint32(hero_prevent_crit_level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(progress)  ||  r.read_uint8(feeling)  ||  r.read_uint32(power)  ||  r.read_uint32(armor)  ||  r.read_uint32(stamina)  ||  r.read_uint32(power_level)  ||  r.read_uint32(armor_level)  ||  r.read_uint32(stamina_level)  ||  r.read_uint32(hero_power)  ||  r.read_uint32(hero_armor)  ||  r.read_uint32(hero_stamina)  ||  r.read_uint32(hero_power_level)  ||  r.read_int32(hero_armor_level)  ||  r.read_int32(hero_stamina_level)  ||  r.read_uint8(is_play_dragon)  ||  r.read_uint32(energy)  ||  r.read_uint32(crit)  ||  r.read_uint32(prevent_crit)  ||  r.read_uint32(crit_level)  ||  r.read_uint32(prevent_crit_level)  ||  r.read_uint32(hero_crit)  ||  r.read_uint32(hero_prevent_crit)  ||  r.read_uint32(hero_crit_level)  ||  r.read_uint32(hero_prevent_crit_level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		progress = 0;
		feeling = 0;
		power = 0;
		armor = 0;
		stamina = 0;
		power_level = 0;
		armor_level = 0;
		stamina_level = 0;
		hero_power = 0;
		hero_armor = 0;
		hero_stamina = 0;
		hero_power_level = 0;
		hero_armor_level = 0;
		hero_stamina_level = 0;
		is_play_dragon = 0;
		energy = 0;
		crit = 0;
		prevent_crit = 0;
		crit_level = 0;
		prevent_crit_level = 0;
		hero_crit = 0;
		hero_prevent_crit = 0;
		hero_crit_level = 0;
		hero_prevent_crit_level = 0;
	}
};

/*
精灵盟约状态
*/
struct Covenant_Status {
	uint32_t id;/*盟约ID*/
	uint8_t init_status;/*初始状态,0未开通过, 1开通过*/
	uint32_t over_time;/*过期时间*/
	uint32_t today_become_time;/*特殊精灵,在当天有持续时间, 表示当天开启后持续时间过期时间*/
	Covenant_Status(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint8(init_status);
		w.write_uint32(over_time);
		w.write_uint32(today_become_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint8(init_status)  ||  r.read_uint32(over_time)  ||  r.read_uint32(today_become_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		init_status = 0;
		over_time = 0;
		today_become_time = 0;
	}
};

/*
好友基本信息
*/
struct Friend_Base_Info {
	int64_t role_id;/*好友角色ID*/
	std::string role_name;/*好友角色名*/
	Friend_Base_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
	}
};

/*
精通部位信息
*/
struct Master_Part_Info {
	int32_t index;/*部位索引主角10001-10008 英雄1-8*/
	int32_t level;/*等级*/
	int32_t luck_value;/*当前幸运值*/
	int32_t fail_add_rate;/*失败提升成功率(百分制)*/
	Master_Part_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_int32(level);
		w.write_int32(luck_value);
		w.write_int32(fail_add_rate);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_int32(level)  ||  r.read_int32(luck_value)  ||  r.read_int32(fail_add_rate)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		level = 0;
		luck_value = 0;
		fail_add_rate = 0;
	}
};

/*
VIP世界之树累计奖励
*/
struct World_Tree_Flock {
	uint32_t id;/*项目ID*/
	uint32_t souls;/*英灵*/
	uint32_t copper;/*金币*/
	uint32_t dragon_soul;/*龙魂*/
	int32_t receive_souls;/*已领取的英灵*/
	int32_t receive_copper;/*已领取的金币*/
	int32_t receive_dragon_soul;/*已领取的龙魂*/
	World_Tree_Flock(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(souls);
		w.write_uint32(copper);
		w.write_uint32(dragon_soul);
		w.write_int32(receive_souls);
		w.write_int32(receive_copper);
		w.write_int32(receive_dragon_soul);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(souls)  ||  r.read_uint32(copper)  ||  r.read_uint32(dragon_soul)  ||  r.read_int32(receive_souls)  ||  r.read_int32(receive_copper)  ||  r.read_int32(receive_dragon_soul)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		souls = 0;
		copper = 0;
		dragon_soul = 0;
		receive_souls = 0;
		receive_copper = 0;
		receive_dragon_soul = 0;
	}
};

/*
掠夺匹配成员信息
*/
struct Rob_Match_Member {
	std::string name;/*名称*/
	uint32_t level;/*等级*/
	uint32_t hero_id;/**/
	uint8_t career;/**/
	uint8_t gender;/**/
	Rob_Match_Member(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(name);
		w.write_uint32(level);
		w.write_uint32(hero_id);
		w.write_uint8(career);
		w.write_uint8(gender);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(name)  ||  r.read_uint32(level)  ||  r.read_uint32(hero_id)  ||  r.read_uint8(career)  ||  r.read_uint8(gender)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		name.clear();
		level = 0;
		hero_id = 0;
		career = 0;
		gender = 0;
	}
};

/*
坐骑数据
*/
struct Mount_Info {
	int32_t mount_id;/*坐骑id*/
	uint32_t deadline;/*有效时间（0：永久有效）*/
	uint32_t activate_time;/*激活时间*/
	int8_t invalid;/*是否无效的（0：有效的 1：无效的)*/
	int8_t train_lv;/*驯养等级  星级*/
	int8_t deploy;/*是否上阵 （1：上阵   0：下阵）*/
	std::vector<int32_t> cur_attr;/*当前的驯养属性值*/
	std::vector<int32_t> max_attr;/*升星所需属性值*/
	int32_t luck_value;/*幸运值*/
	int32_t skill_1;/*当前坐骑技能1  （0：没有技能）*/
	int32_t skill_2;/*当前坐骑技能2  （0：没有技能）*/
	int32_t skill_3;/*当前坐骑技能3  （0：没有技能）*/
	int8_t evolve;/*0：非进阶 1：进阶中*/
	int8_t train_rate;/*升星进度（0-->100)*/
	int8_t lv;/*等级*/
	Mount_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(mount_id);
		w.write_uint32(deadline);
		w.write_uint32(activate_time);
		w.write_int8(invalid);
		w.write_int8(train_lv);
		w.write_int8(deploy);
		uint16_t __cur_attr_vec_size = cur_attr.size();
		w.write_uint16(__cur_attr_vec_size);
		for(uint16_t i = 0; i < __cur_attr_vec_size; ++i) {
				w.write_int32(cur_attr[i]);
		}

		uint16_t __max_attr_vec_size = max_attr.size();
		w.write_uint16(__max_attr_vec_size);
		for(uint16_t i = 0; i < __max_attr_vec_size; ++i) {
				w.write_int32(max_attr[i]);
		}

		w.write_int32(luck_value);
		w.write_int32(skill_1);
		w.write_int32(skill_2);
		w.write_int32(skill_3);
		w.write_int8(evolve);
		w.write_int8(train_rate);
		w.write_int8(lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(mount_id)  ||  r.read_uint32(deadline)  ||  r.read_uint32(activate_time)  ||  r.read_int8(invalid)  ||  r.read_int8(train_lv)  ||  r.read_int8(deploy)  )
				return -1;
 		uint16_t __cur_attr_vec_size;
		if(r.read_uint16(__cur_attr_vec_size) )
				return -1;
		cur_attr.reserve(__cur_attr_vec_size);
		for(uint16_t i = 0; i < __cur_attr_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				cur_attr.push_back(v);
		}

		uint16_t __max_attr_vec_size;
		if(r.read_uint16(__max_attr_vec_size) )
				return -1;
		max_attr.reserve(__max_attr_vec_size);
		for(uint16_t i = 0; i < __max_attr_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				max_attr.push_back(v);
		}

		if( r.read_int32(luck_value)  ||  r.read_int32(skill_1)  ||  r.read_int32(skill_2)  ||  r.read_int32(skill_3)  ||  r.read_int8(evolve)  ||  r.read_int8(train_rate)  ||  r.read_int8(lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		mount_id = 0;
		deadline = 0;
		activate_time = 0;
		invalid = 0;
		train_lv = 0;
		deploy = 0;
		cur_attr.clear();
		max_attr.clear();
		luck_value = 0;
		skill_1 = 0;
		skill_2 = 0;
		skill_3 = 0;
		evolve = 0;
		train_rate = 0;
		lv = 0;
	}
};

/*
龙谷布告栏
备注： 
        type :类型。   1：拜访  2:驱逐  3：沙鼠偷猎者出现  4:掠夺  5:逗龙
               每个布告栏最多50条，超过50条，时间最早的删除。
        param1: 
           如果type=2,   param1 =>     怪物品质
           如果type=3,   param1 =>     怪物品质
           如果type=4,  param1 =>   1: 龙谷主人战胜    2:龙谷主人失败
           如果type=5,  param1=>  龙宠心情值提升点数 
*/
struct Dragon_Vale_Notice {
	uint8_t type;/*类型*/
	int32_t time;/*时间*/
	std::string host;/*主语*/
	std::string target;/*宾语*/
	uint32_t dragon_soul;/*龙魂*/
	uint32_t coin;/*金币*/
	uint32_t soul;/*英灵*/
	uint8_t is_get_dragon_heart;/*是否获得龙心  1：是， 0：否*/
	uint8_t param1;/*附加参数1*/
	Dragon_Vale_Notice(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
		w.write_int32(time);
		w.write_string(host);
		w.write_string(target);
		w.write_uint32(dragon_soul);
		w.write_uint32(coin);
		w.write_uint32(soul);
		w.write_uint8(is_get_dragon_heart);
		w.write_uint8(param1);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  ||  r.read_int32(time)  ||  r.read_string(host)  ||  r.read_string(target)  ||  r.read_uint32(dragon_soul)  ||  r.read_uint32(coin)  ||  r.read_uint32(soul)  ||  r.read_uint8(is_get_dragon_heart)  ||  r.read_uint8(param1)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
		time = 0;
		host.clear();
		target.clear();
		dragon_soul = 0;
		coin = 0;
		soul = 0;
		is_get_dragon_heart = 0;
		param1 = 0;
	}
};

/*
组队阵型信息
*/
struct Teamer_Lineup_Info {
	int8_t index;/*阵型索引*/
	int64_t role_id;/*角色id*/
	int8_t hero_fight;/*0未出战1出战*/
	Teamer_Lineup_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
		w.write_int64(role_id);
		w.write_int8(hero_fight);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  ||  r.read_int64(role_id)  ||  r.read_int8(hero_fight)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		role_id = 0;
		hero_fight = 0;
	}
};

/*
坐骑组合信息
*/
struct Mount_Group_Info {
	int32_t id;/*组合id*/
	int8_t status;/*0：未激活 1：已激活 2：新激活*/
	Mount_Group_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		status = 0;
	}
};

/*
送花信息
*/
struct Flowers_Info {
	std::string role_name;/*赠送方角色名*/
	int64_t role_id;/*赠送方角色ID*/
	uint32_t item_id;/*道具ID*/
	uint32_t charm;/*增加魅力值数*/
	uint32_t friendship;/*增加友好度数*/
	std::string msg;/*寄语*/
	Flowers_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
		w.write_int64(role_id);
		w.write_uint32(item_id);
		w.write_uint32(charm);
		w.write_uint32(friendship);
		w.write_string(msg);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  ||  r.read_int64(role_id)  ||  r.read_uint32(item_id)  ||  r.read_uint32(charm)  ||  r.read_uint32(friendship)  ||  r.read_string(msg)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_name.clear();
		role_id = 0;
		item_id = 0;
		charm = 0;
		friendship = 0;
		msg.clear();
	}
};

/*
熔炉信息
*/
struct Smelter_Info {
	int8_t smelter_idx;/*熔炉index[1-5]*/
	int8_t is_open;/*是否开启*/
	Smelter_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(smelter_idx);
		w.write_int8(is_open);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(smelter_idx)  ||  r.read_int8(is_open)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		smelter_idx = 0;
		is_open = 0;
	}
};

/*
公会战资源塔状态
采集点从左往右，从上往下排

*/
struct Gang_War_Tower_Detail {
	int8_t state;/*0完成（未占领）  1红色占领过程中 2蓝色占领过程中*/
	int8_t color;/*0未被占领 1红色 2蓝色*/
	std::string gang_name_state;/*突袭公会*/
	std::string gang_name_color;/*占领公会*/
	Gang_War_Tower_Detail(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(state);
		w.write_int8(color);
		w.write_string(gang_name_state);
		w.write_string(gang_name_color);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(state)  ||  r.read_int8(color)  ||  r.read_string(gang_name_state)  ||  r.read_string(gang_name_color)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		state = 0;
		color = 0;
		gang_name_state.clear();
		gang_name_color.clear();
	}
};

/*
公会战排行榜信息
*/
struct Gang_War_Rank_Info {
	int64_t role_id;/**/
	int64_t gang_id;/**/
	std::string role_name;/**/
	int32_t kill_num;/*击杀数*/
	int32_t die_num;/*死亡次数*/
	int32_t hold_spot;/*抢点*/
	int8_t headship;/*公会职务，0-普通成员，4-会长，3-副会长，2-长老，1-官员*/
	int32_t force;/*战斗力*/
	std::string gang_name;/*公会名*/
	int8_t camp;/*1红2蓝*/
	int8_t index;/*否今日已经分配过箱子*/
	Gang_War_Rank_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int64(gang_id);
		w.write_string(role_name);
		w.write_int32(kill_num);
		w.write_int32(die_num);
		w.write_int32(hold_spot);
		w.write_int8(headship);
		w.write_int32(force);
		w.write_string(gang_name);
		w.write_int8(camp);
		w.write_int8(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int64(gang_id)  ||  r.read_string(role_name)  ||  r.read_int32(kill_num)  ||  r.read_int32(die_num)  ||  r.read_int32(hold_spot)  ||  r.read_int8(headship)  ||  r.read_int32(force)  ||  r.read_string(gang_name)  ||  r.read_int8(camp)  ||  r.read_int8(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		gang_id = 0;
		role_name.clear();
		kill_num = 0;
		die_num = 0;
		hold_spot = 0;
		headship = 0;
		force = 0;
		gang_name.clear();
		camp = 0;
		index = 0;
	}
};

/*
公会最简信息-公会战用
*/
struct Gang_Basic_Detail {
	int64_t gang_id;/**/
	std::string gang_name;/**/
	int8_t index;/*0上面1下面*/
	std::string zone;/*所属服*/
	int8_t rank;/**/
	Gang_Basic_Detail(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_string(gang_name);
		w.write_int8(index);
		w.write_string(zone);
		w.write_int8(rank);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_string(gang_name)  ||  r.read_int8(index)  ||  r.read_string(zone)  ||  r.read_int8(rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		gang_name.clear();
		index = 0;
		zone.clear();
		rank = 0;
	}
};

/*
公会战面板结点信息
index:16进八（0-7），8进4（8-11），4进2（12-13），2进1（14）
*/
struct Gang_War_Spot {
	int8_t index;/**/
	std::vector<Gang_Basic_Detail> gang_vec;
	int8_t result;/*0未打完1上面胜2下面胜*/
	Gang_War_Spot(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
		uint16_t __gang_vec_vec_size = gang_vec.size();
		w.write_uint16(__gang_vec_vec_size);
		for(uint16_t i = 0; i < __gang_vec_vec_size; ++i) {
				gang_vec[i].serialize(w);
		}

		w.write_int8(result);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  )
				return -1;
 		uint16_t __gang_vec_vec_size;
		if(r.read_uint16(__gang_vec_vec_size) )
				return -1;
		gang_vec.reserve(__gang_vec_vec_size);
		for(uint16_t i = 0; i < __gang_vec_vec_size; ++i) {
				Gang_Basic_Detail v;
				if(v.deserialize(r))
						return -1;
				gang_vec.push_back(v);
		}

		if( r.read_int8(result)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		gang_vec.clear();
		result = 0;
	}
};

/*
神秘商店物品信息
*/
struct Secret_Item_Info {
	int8_t shop_type;/*0：金币商品 1：钻石商品 2：神秘商品*/
	int32_t item_id;/*道具id*/
	int32_t remain_num;/*剩余数量*/
	int16_t buy_num;/*已经购买的数量*/
	Secret_Item_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(shop_type);
		w.write_int32(item_id);
		w.write_int32(remain_num);
		w.write_int16(buy_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(shop_type)  ||  r.read_int32(item_id)  ||  r.read_int32(remain_num)  ||  r.read_int16(buy_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		shop_type = 0;
		item_id = 0;
		remain_num = 0;
		buy_num = 0;
	}
};

/*
公会战统计信息
*/
struct Gang_War_Total_Info {
	int32_t tower_num;/*拥有的资源塔数*/
	int32_t camp;/*1红方2蓝方*/
	int32_t member_num;/*人数*/
	int32_t resource_num;/*产生的资源点*/
	std::string gang_name;/**/
	Gang_War_Total_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(tower_num);
		w.write_int32(camp);
		w.write_int32(member_num);
		w.write_int32(resource_num);
		w.write_string(gang_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(tower_num)  ||  r.read_int32(camp)  ||  r.read_int32(member_num)  ||  r.read_int32(resource_num)  ||  r.read_string(gang_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		tower_num = 0;
		camp = 0;
		member_num = 0;
		resource_num = 0;
		gang_name.clear();
	}
};

/*
副本扫荡信息
*/
struct Wipe_Info {
	int8_t index;/*序号*/
	int32_t scene_id;/*副本id*/
	int8_t status;/*0：未开始 1：扫荡中 2：扫荡结束*/
	int32_t exp;/*获得的经验*/
	int32_t copper;/*获得的金币*/
	std::vector<Item_Info> items;
	Wipe_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
		w.write_int32(scene_id);
		w.write_int8(status);
		w.write_int32(exp);
		w.write_int32(copper);
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  ||  r.read_int32(scene_id)  ||  r.read_int8(status)  ||  r.read_int32(exp)  ||  r.read_int32(copper)  )
				return -1;
 		uint16_t __items_vec_size;
		if(r.read_uint16(__items_vec_size) )
				return -1;
		items.reserve(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				items.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		index = 0;
		scene_id = 0;
		status = 0;
		exp = 0;
		copper = 0;
		items.clear();
	}
};

/*
奖励找回信息
*/
struct Reward_Back_Info {
	uint32_t id;/*项目ID*/
	std::vector<Uint_Uint> info_item;
	std::vector<Uint_Uint> info_money;
	uint32_t exp;/*经验奖励值*/
	uint32_t copper_cost;/*金币追回消耗金币数*/
	uint32_t gold_cost;/*钻石追回消耗钻石数*/
	uint32_t back_num;/*找回次数*/
	Reward_Back_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		uint16_t __info_item_vec_size = info_item.size();
		w.write_uint16(__info_item_vec_size);
		for(uint16_t i = 0; i < __info_item_vec_size; ++i) {
				info_item[i].serialize(w);
		}

		uint16_t __info_money_vec_size = info_money.size();
		w.write_uint16(__info_money_vec_size);
		for(uint16_t i = 0; i < __info_money_vec_size; ++i) {
				info_money[i].serialize(w);
		}

		w.write_uint32(exp);
		w.write_uint32(copper_cost);
		w.write_uint32(gold_cost);
		w.write_uint32(back_num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		uint16_t __info_item_vec_size;
		if(r.read_uint16(__info_item_vec_size) )
				return -1;
		info_item.reserve(__info_item_vec_size);
		for(uint16_t i = 0; i < __info_item_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				info_item.push_back(v);
		}

		uint16_t __info_money_vec_size;
		if(r.read_uint16(__info_money_vec_size) )
				return -1;
		info_money.reserve(__info_money_vec_size);
		for(uint16_t i = 0; i < __info_money_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				info_money.push_back(v);
		}

		if( r.read_uint32(exp)  ||  r.read_uint32(copper_cost)  ||  r.read_uint32(gold_cost)  ||  r.read_uint32(back_num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		info_item.clear();
		info_money.clear();
		exp = 0;
		copper_cost = 0;
		gold_cost = 0;
		back_num = 0;
	}
};

/*
剧情数据
已经有过的剧情数据
*/
struct Plot_Info {
	std::string plot_id;/**/
	int32_t num;/*已进入次数*/
	Plot_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(plot_id);
		w.write_int32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(plot_id)  ||  r.read_int32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		plot_id.clear();
		num = 0;
	}
};

/*
限时抢购商品结构配置
*/
struct Limit_Buy_Config {
	uint32_t id;/*id*/
	uint32_t item_id;/*道具ID*/
	uint32_t price;/*原价*/
	uint32_t pro_price;/*现价*/
	uint32_t start_time;/*相对开始时间*/
	uint32_t continue_time;/*持续时长*/
	uint32_t num;/*抢购数量*/
	Limit_Buy_Config(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(item_id);
		w.write_uint32(price);
		w.write_uint32(pro_price);
		w.write_uint32(start_time);
		w.write_uint32(continue_time);
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(item_id)  ||  r.read_uint32(price)  ||  r.read_uint32(pro_price)  ||  r.read_uint32(start_time)  ||  r.read_uint32(continue_time)  ||  r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		item_id = 0;
		price = 0;
		pro_price = 0;
		start_time = 0;
		continue_time = 0;
		num = 0;
	}
};

/*
篝火信息
*/
struct Campfire_Info {
	int16_t level;/*等级（0：公会没占有篝火）*/
	int32_t lv_cur_num;/*等级篝火数量*/
	int32_t lv_max_num;/*等级提升的篝火数量*/
	int32_t total_num;/*篝火累计数量*/
	double incom_rate;/*收益倍数*/
	double fs_up_rate;/*符石提示概率*/
	int8_t firewood;/*是否添加过柴火（0：没添加过柴火， 1：有添加过柴火）*/
	int8_t burning;/*是否点燃（0：没有点燃的 1：点燃的）*/
	Campfire_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(level);
		w.write_int32(lv_cur_num);
		w.write_int32(lv_max_num);
		w.write_int32(total_num);
		w.write_double(incom_rate);
		w.write_double(fs_up_rate);
		w.write_int8(firewood);
		w.write_int8(burning);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(level)  ||  r.read_int32(lv_cur_num)  ||  r.read_int32(lv_max_num)  ||  r.read_int32(total_num)  ||  r.read_double(incom_rate)  ||  r.read_double(fs_up_rate)  ||  r.read_int8(firewood)  ||  r.read_int8(burning)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
		lv_cur_num = 0;
		lv_max_num = 0;
		total_num = 0;
		incom_rate = 0.0;
		fs_up_rate = 0.0;
		firewood = 0;
		burning = 0;
	}
};

/*
公会战奖励分配
*/
struct Reward_Info_Gang_War {
	int64_t role_id;/**/
	std::string role_name;/**/
	Reward_Info award;
	int64_t index;/*标识位置*/
	Reward_Info_Gang_War(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		award.serialize(w);
		w.write_int64(index);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  award.deserialize(r)  ||  r.read_int64(index)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		award.reset();
		index = 0;
	}
};

/*
PHP发放物品货币结构
*/
struct PHP_Send_Item_Info {
	uint8_t item_type;/*道具类型, 1.货币, 2.物品*/
	uint32_t item_id;/*货币:501502//金币 GOLD = 501602//钻石 501702//粉钻 502803//龙魂 502903//英灵 503403//友情值; 物品直接写物品ID*/
	uint32_t num;/*数量*/
	uint32_t bind;/*绑定类型: 0.根据配置; 1.不绑定; 2.绑定*/
	int64_t role_id;/*角色ID*/
	PHP_Send_Item_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(item_type);
		w.write_uint32(item_id);
		w.write_uint32(num);
		w.write_uint32(bind);
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(item_type)  ||  r.read_uint32(item_id)  ||  r.read_uint32(num)  ||  r.read_uint32(bind)  ||  r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		item_type = 0;
		item_id = 0;
		num = 0;
		bind = 0;
		role_id = 0;
	}
};

/*
公会战排名奖励
*/
struct Reward_Info_Gang_War_Rank_Award {
	std::vector<Reward_Info> gang;
	std::vector<Reward_Info> self;
	int64_t gang_id;/**/
	int8_t state;/*0未领1已领*/
	std::string gang_name;/**/
	int32_t rank;/*排名*/
	int32_t win_nums;/*胜场数*/
	std::string zone;/*服务器名*/
	Reward_Info_Gang_War_Rank_Award(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __gang_vec_size = gang.size();
		w.write_uint16(__gang_vec_size);
		for(uint16_t i = 0; i < __gang_vec_size; ++i) {
				gang[i].serialize(w);
		}

		uint16_t __self_vec_size = self.size();
		w.write_uint16(__self_vec_size);
		for(uint16_t i = 0; i < __self_vec_size; ++i) {
				self[i].serialize(w);
		}

		w.write_int64(gang_id);
		w.write_int8(state);
		w.write_string(gang_name);
		w.write_int32(rank);
		w.write_int32(win_nums);
		w.write_string(zone);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __gang_vec_size;
		if(r.read_uint16(__gang_vec_size) )
				return -1;
		gang.reserve(__gang_vec_size);
		for(uint16_t i = 0; i < __gang_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				gang.push_back(v);
		}

		uint16_t __self_vec_size;
		if(r.read_uint16(__self_vec_size) )
				return -1;
		self.reserve(__self_vec_size);
		for(uint16_t i = 0; i < __self_vec_size; ++i) {
				Reward_Info v;
				if(v.deserialize(r))
						return -1;
				self.push_back(v);
		}

		if( r.read_int64(gang_id)  ||  r.read_int8(state)  ||  r.read_string(gang_name)  ||  r.read_int32(rank)  ||  r.read_int32(win_nums)  ||  r.read_string(zone)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang.clear();
		self.clear();
		gang_id = 0;
		state = 0;
		gang_name.clear();
		rank = 0;
		win_nums = 0;
		zone.clear();
	}
};

/*
时装信息
*/
struct Fashion_Info {
	int32_t id;/*id*/
	int8_t lv;/*level*/
	int8_t state;/*状态，0.未穿上，1.穿上*/
	int16_t exp;/*当前等级经验*/
	Fashion_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int8(lv);
		w.write_int8(state);
		w.write_int16(exp);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int8(lv)  ||  r.read_int8(state)  ||  r.read_int16(exp)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		lv = 0;
		state = 0;
		exp = 0;
	}
};

/*
拍卖行商品信息
*/
struct Auction_Goods_Info {
	uint32_t id;/*商品ID*/
	uint32_t item_id;/*道具ID*/
	uint32_t num;/*数量*/
	uint32_t auction_price;/*默认竞拍价*/
	uint32_t ones_price;/*一口价*/
	uint32_t lave_time;/*剩余时间(秒)*/
	std::string auction_role_name;/*发布拍卖信息的玩家名字*/
	int64_t auction_role_id;/*发布拍卖信息的玩家id*/
	std::string last_auction_role_name;/*最后一次竞拍的玩家名字*/
	int64_t last_auction_role_id;/*最后一次竞拍的玩家id*/
	uint32_t last_auction_price;/*最后一次给出的竞拍价*/
	std::vector<int64_t> auction_fail_role_id;/*所有竞拍过,但又失败的角色ID*/
	uint32_t start_time;/*开始拍卖时间*/
	Auction_Goods_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(item_id);
		w.write_uint32(num);
		w.write_uint32(auction_price);
		w.write_uint32(ones_price);
		w.write_uint32(lave_time);
		w.write_string(auction_role_name);
		w.write_int64(auction_role_id);
		w.write_string(last_auction_role_name);
		w.write_int64(last_auction_role_id);
		w.write_uint32(last_auction_price);
		uint16_t __auction_fail_role_id_vec_size = auction_fail_role_id.size();
		w.write_uint16(__auction_fail_role_id_vec_size);
		for(uint16_t i = 0; i < __auction_fail_role_id_vec_size; ++i) {
				w.write_int64(auction_fail_role_id[i]);
		}

		w.write_uint32(start_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(item_id)  ||  r.read_uint32(num)  ||  r.read_uint32(auction_price)  ||  r.read_uint32(ones_price)  ||  r.read_uint32(lave_time)  ||  r.read_string(auction_role_name)  ||  r.read_int64(auction_role_id)  ||  r.read_string(last_auction_role_name)  ||  r.read_int64(last_auction_role_id)  ||  r.read_uint32(last_auction_price)  )
				return -1;
 		uint16_t __auction_fail_role_id_vec_size;
		if(r.read_uint16(__auction_fail_role_id_vec_size) )
				return -1;
		auction_fail_role_id.reserve(__auction_fail_role_id_vec_size);
		for(uint16_t i = 0; i < __auction_fail_role_id_vec_size; ++i) {
				int64_t v;
				if(r.read_int64(v) )
						return -1;
				auction_fail_role_id.push_back(v);
		}

		if( r.read_uint32(start_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		item_id = 0;
		num = 0;
		auction_price = 0;
		ones_price = 0;
		lave_time = 0;
		auction_role_name.clear();
		auction_role_id = 0;
		last_auction_role_name.clear();
		last_auction_role_id = 0;
		last_auction_price = 0;
		auction_fail_role_id.clear();
		start_time = 0;
	}
};

/*
骑士试炼人物信息
*/
struct Knight_Trials_Role_Info {
	int64_t role_id;/*角色ID*/
	uint8_t points;/*挑战关数*/
	std::string role_name;/*角色名*/
	int32_t head_id;/*头像ID*/
	uint8_t gender;/*性别*/
	int32_t career;/*职业*/
	int32_t level;/*角色等级*/
	int32_t vip;/*vip等级*/
	std::string server_name;/*服务器名*/
	int32_t force;/*角色战力*/
	int32_t hero_id;/*英雄ID*/
	int32_t hero_force;/*英雄战力*/
	int32_t hero_level;/*英雄等级*/
	std::string hero_name;/*英雄名字*/
	int32_t mount_id;/*坐骑ID*/
	std::vector<int32_t> fashion;/*时装*/
	std::string gang_name;/*公会名字*/
	Knight_Trials_Role_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint8(points);
		w.write_string(role_name);
		w.write_int32(head_id);
		w.write_uint8(gender);
		w.write_int32(career);
		w.write_int32(level);
		w.write_int32(vip);
		w.write_string(server_name);
		w.write_int32(force);
		w.write_int32(hero_id);
		w.write_int32(hero_force);
		w.write_int32(hero_level);
		w.write_string(hero_name);
		w.write_int32(mount_id);
		uint16_t __fashion_vec_size = fashion.size();
		w.write_uint16(__fashion_vec_size);
		for(uint16_t i = 0; i < __fashion_vec_size; ++i) {
				w.write_int32(fashion[i]);
		}

		w.write_string(gang_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint8(points)  ||  r.read_string(role_name)  ||  r.read_int32(head_id)  ||  r.read_uint8(gender)  ||  r.read_int32(career)  ||  r.read_int32(level)  ||  r.read_int32(vip)  ||  r.read_string(server_name)  ||  r.read_int32(force)  ||  r.read_int32(hero_id)  ||  r.read_int32(hero_force)  ||  r.read_int32(hero_level)  ||  r.read_string(hero_name)  ||  r.read_int32(mount_id)  )
				return -1;
 		uint16_t __fashion_vec_size;
		if(r.read_uint16(__fashion_vec_size) )
				return -1;
		fashion.reserve(__fashion_vec_size);
		for(uint16_t i = 0; i < __fashion_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				fashion.push_back(v);
		}

		if( r.read_string(gang_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		points = 0;
		role_name.clear();
		head_id = 0;
		gender = 0;
		career = 0;
		level = 0;
		vip = 0;
		server_name.clear();
		force = 0;
		hero_id = 0;
		hero_force = 0;
		hero_level = 0;
		hero_name.clear();
		mount_id = 0;
		fashion.clear();
		gang_name.clear();
	}
};

/*
竞技场匹配信息
*/
struct Arena_Match_Info {
	std::string server_name;/*服务器名称*/
	int64_t role_id;/**/
	std::string role_name;/*名称*/
	uint32_t head_id;/*头像*/
	uint32_t fight;/*战斗力*/
	uint16_t level;/*等级*/
	uint16_t arousal;/*觉醒等级*/
	uint16_t score;/*竞技场积分*/
	uint8_t group;/*组*/
	uint8_t seg;/*段*/
	int32_t hero_id;/*英雄id*/
	std::string hero_name;/*英雄名称*/
	uint16_t hero_level;/*英雄等级*/
	uint16_t hero_arousal;/*英雄觉醒等级*/
	int16_t rank;/*排名*/
	uint8_t gender;/*性别*/
	uint8_t career;/*职业*/
	int32_t title_id;/*称号*/
	std::vector<int32_t> avatar_vec;/*时装*/
	int8_t difficult_lv;/*0:普通 1：困难 2：神级*/
	Arena_Match_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(server_name);
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_uint32(head_id);
		w.write_uint32(fight);
		w.write_uint16(level);
		w.write_uint16(arousal);
		w.write_uint16(score);
		w.write_uint8(group);
		w.write_uint8(seg);
		w.write_int32(hero_id);
		w.write_string(hero_name);
		w.write_uint16(hero_level);
		w.write_uint16(hero_arousal);
		w.write_int16(rank);
		w.write_uint8(gender);
		w.write_uint8(career);
		w.write_int32(title_id);
		uint16_t __avatar_vec_vec_size = avatar_vec.size();
		w.write_uint16(__avatar_vec_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_vec_size; ++i) {
				w.write_int32(avatar_vec[i]);
		}

		w.write_int8(difficult_lv);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(server_name)  ||  r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_uint32(head_id)  ||  r.read_uint32(fight)  ||  r.read_uint16(level)  ||  r.read_uint16(arousal)  ||  r.read_uint16(score)  ||  r.read_uint8(group)  ||  r.read_uint8(seg)  ||  r.read_int32(hero_id)  ||  r.read_string(hero_name)  ||  r.read_uint16(hero_level)  ||  r.read_uint16(hero_arousal)  ||  r.read_int16(rank)  ||  r.read_uint8(gender)  ||  r.read_uint8(career)  ||  r.read_int32(title_id)  )
				return -1;
 		uint16_t __avatar_vec_vec_size;
		if(r.read_uint16(__avatar_vec_vec_size) )
				return -1;
		avatar_vec.reserve(__avatar_vec_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				avatar_vec.push_back(v);
		}

		if( r.read_int8(difficult_lv)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		server_name.clear();
		role_id = 0;
		role_name.clear();
		head_id = 0;
		fight = 0;
		level = 0;
		arousal = 0;
		score = 0;
		group = 0;
		seg = 0;
		hero_id = 0;
		hero_name.clear();
		hero_level = 0;
		hero_arousal = 0;
		rank = 0;
		gender = 0;
		career = 0;
		title_id = 0;
		avatar_vec.clear();
		difficult_lv = 0;
	}
};

/*
竞技场信息
*/
struct Arena_Info {
	uint16_t season_time_remain;/*赛季结束时间剩余天数*/
	std::vector<Uid_Name> the_first_three;
	Arena_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(season_time_remain);
		uint16_t __the_first_three_vec_size = the_first_three.size();
		w.write_uint16(__the_first_three_vec_size);
		for(uint16_t i = 0; i < __the_first_three_vec_size; ++i) {
				the_first_three[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(season_time_remain)  )
				return -1;
 		uint16_t __the_first_three_vec_size;
		if(r.read_uint16(__the_first_three_vec_size) )
				return -1;
		the_first_three.reserve(__the_first_three_vec_size);
		for(uint16_t i = 0; i < __the_first_three_vec_size; ++i) {
				Uid_Name v;
				if(v.deserialize(r))
						return -1;
				the_first_three.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		season_time_remain = 0;
		the_first_three.clear();
	}
};

/*
竞技场个人信息
*/
struct Arena_Personal_Info {
	uint8_t group;/*组*/
	uint8_t seg;/*段*/
	uint16_t local_rank;/*本服排名*/
	uint16_t inter_rank;/*跨服排名*/
	uint16_t score;/*积分*/
	uint8_t free_times;/*剩余免费挑战次数*/
	uint8_t pay_times;/*剩余付费挑战次数*/
	uint8_t max_pay_times;/*最多能购买的付费挑战次数*/
	uint8_t buy_pay_times;/*已经购买的付费挑战次数*/
	uint8_t total_free_times;/*免费次数*/
	uint32_t reward_times1;/*3天奖励时间*/
	std::vector<Arena_Battle_Report> report_vec;
	uint16_t three_day_rank;/*每天奖励排名，如果不为0，则是有奖励可以领取的*/
	uint16_t season_rank;/*赛季奖励排名，如果不为0，则是有奖励可以领取的*/
	uint8_t rise;/*是否在晋级赛 0：非晋级赛， 1：晋级赛*/
	std::vector<uint8_t> rise_result_vec;/*晋级赛胜负结果（0：失败，1：胜利*/
	int32_t remain_cd_time;/*剩余cd时间*/
	Arena_Personal_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(group);
		w.write_uint8(seg);
		w.write_uint16(local_rank);
		w.write_uint16(inter_rank);
		w.write_uint16(score);
		w.write_uint8(free_times);
		w.write_uint8(pay_times);
		w.write_uint8(max_pay_times);
		w.write_uint8(buy_pay_times);
		w.write_uint8(total_free_times);
		w.write_uint32(reward_times1);
		uint16_t __report_vec_vec_size = report_vec.size();
		w.write_uint16(__report_vec_vec_size);
		for(uint16_t i = 0; i < __report_vec_vec_size; ++i) {
				report_vec[i].serialize(w);
		}

		w.write_uint16(three_day_rank);
		w.write_uint16(season_rank);
		w.write_uint8(rise);
		uint16_t __rise_result_vec_vec_size = rise_result_vec.size();
		w.write_uint16(__rise_result_vec_vec_size);
		for(uint16_t i = 0; i < __rise_result_vec_vec_size; ++i) {
				w.write_uint8(rise_result_vec[i]);
		}

		w.write_int32(remain_cd_time);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(group)  ||  r.read_uint8(seg)  ||  r.read_uint16(local_rank)  ||  r.read_uint16(inter_rank)  ||  r.read_uint16(score)  ||  r.read_uint8(free_times)  ||  r.read_uint8(pay_times)  ||  r.read_uint8(max_pay_times)  ||  r.read_uint8(buy_pay_times)  ||  r.read_uint8(total_free_times)  ||  r.read_uint32(reward_times1)  )
				return -1;
 		uint16_t __report_vec_vec_size;
		if(r.read_uint16(__report_vec_vec_size) )
				return -1;
		report_vec.reserve(__report_vec_vec_size);
		for(uint16_t i = 0; i < __report_vec_vec_size; ++i) {
				Arena_Battle_Report v;
				if(v.deserialize(r))
						return -1;
				report_vec.push_back(v);
		}

		if( r.read_uint16(three_day_rank)  ||  r.read_uint16(season_rank)  ||  r.read_uint8(rise)  )
				return -1;
 		uint16_t __rise_result_vec_vec_size;
		if(r.read_uint16(__rise_result_vec_vec_size) )
				return -1;
		rise_result_vec.reserve(__rise_result_vec_vec_size);
		for(uint16_t i = 0; i < __rise_result_vec_vec_size; ++i) {
				uint8_t v;
				if(r.read_uint8(v) )
						return -1;
				rise_result_vec.push_back(v);
		}

		if( r.read_int32(remain_cd_time)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		group = 0;
		seg = 0;
		local_rank = 0;
		inter_rank = 0;
		score = 0;
		free_times = 0;
		pay_times = 0;
		max_pay_times = 0;
		buy_pay_times = 0;
		total_free_times = 0;
		reward_times1 = 0;
		report_vec.clear();
		three_day_rank = 0;
		season_rank = 0;
		rise = 0;
		rise_result_vec.clear();
		remain_cd_time = 0;
	}
};

/*
上古神器宝石
cond_list:
  val1-条件id 
  val2-0不可领1可领2已领
*/
struct Anci_Artifact_Gem {
	int32_t id;/*宝石id*/
	int8_t state;/*0未激活1已激活*/
	std::vector<Int_Int> cond_list;
	Anci_Artifact_Gem(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int8(state);
		uint16_t __cond_list_vec_size = cond_list.size();
		w.write_uint16(__cond_list_vec_size);
		for(uint16_t i = 0; i < __cond_list_vec_size; ++i) {
				cond_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int8(state)  )
				return -1;
 		uint16_t __cond_list_vec_size;
		if(r.read_uint16(__cond_list_vec_size) )
				return -1;
		cond_list.reserve(__cond_list_vec_size);
		for(uint16_t i = 0; i < __cond_list_vec_size; ++i) {
				Int_Int v;
				if(v.deserialize(r))
						return -1;
				cond_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		id = 0;
		state = 0;
		cond_list.clear();
	}
};

/*
上古神器信息
cond_list:
  val1-条件id 
  val2-0不可领1可领2已领
*/
struct Anci_Artifact_Info {
	int32_t id;/*神器id*/
	int32_t state;/*0不可1可2已*/
	std::vector<Anci_Artifact_Gem> gem_list;
	int8_t box_state;/*0不可领1可领2已领*/
	int32_t perfect_degree;/*完美度*/
	Anci_Artifact_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(state);
		uint16_t __gem_list_vec_size = gem_list.size();
		w.write_uint16(__gem_list_vec_size);
		for(uint16_t i = 0; i < __gem_list_vec_size; ++i) {
				gem_list[i].serialize(w);
		}

		w.write_int8(box_state);
		w.write_int32(perfect_degree);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(state)  )
				return -1;
 		uint16_t __gem_list_vec_size;
		if(r.read_uint16(__gem_list_vec_size) )
				return -1;
		gem_list.reserve(__gem_list_vec_size);
		for(uint16_t i = 0; i < __gem_list_vec_size; ++i) {
				Anci_Artifact_Gem v;
				if(v.deserialize(r))
						return -1;
				gem_list.push_back(v);
		}

		if( r.read_int8(box_state)  ||  r.read_int32(perfect_degree)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		state = 0;
		gem_list.clear();
		box_state = 0;
		perfect_degree = 0;
	}
};

/*
上古神器条件信息
*/
struct Anci_Artifact_Cond {
	int32_t id;/*条件id*/
	int8_t state;/*0不可领1可领2已领*/
	int32_t data;/*具体完成度*/
	Anci_Artifact_Cond(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int8(state);
		w.write_int32(data);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int8(state)  ||  r.read_int32(data)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		state = 0;
		data = 0;
	}
};

/*
月卡投资领取按钮信息
备注：day：第几天 。day=0，表示投资后，立即领取的按钮
*/
struct Month_Card_Invest_Info {
	uint8_t day;/*第几天*/
	uint8_t status;/*按钮状态  0:不可领取，1：可领取，2：已领取*/
	Month_Card_Invest_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(day);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(day)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		day = 0;
		status = 0;
	}
};

/*
升级投资领取按钮信息
备注：level=0，表示升级投资后，立即领取
*/
struct Upgrade_Invest_Info {
	uint16_t level;/*等级*/
	uint8_t status;/*按钮状态  0:不可领取，1：可领取，2：已领取*/
	Upgrade_Invest_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(level);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(level)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
		status = 0;
	}
};

/*
升级投资已领取收益结构信息
*/
struct Upgrade_Invest_Profit_Info {
	uint32_t level;/*等级*/
	uint32_t invest_money;/*投资额度*/
	Upgrade_Invest_Profit_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
		w.write_uint32(invest_money);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  ||  r.read_uint32(invest_money)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
		invest_money = 0;
	}
};

/*
投资计划历史记录
*/
struct INVEST_PLAN_HISTORY_RECORD {
	uint8_t invest_type;/*投资计划的类型 1:月卡投资 2:升级投资*/
	uint8_t history_type;/*记录类型。1：存入  2：领取*/
	uint32_t money;/*存入/领取的钻石/粉钻数目*/
	std::string player_name;/*玩家姓名*/
	INVEST_PLAN_HISTORY_RECORD(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(invest_type);
		w.write_uint8(history_type);
		w.write_uint32(money);
		w.write_string(player_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(invest_type)  ||  r.read_uint8(history_type)  ||  r.read_uint32(money)  ||  r.read_string(player_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		invest_type = 0;
		history_type = 0;
		money = 0;
		player_name.clear();
	}
};

/*
远征公会排名信息
*/
struct Expedition_Gang_Rank_Info {
	uint32_t rank;/*当前排名*/
	int64_t uuid;/*公会所在服ID*/
	std::string server_name;/*公会所在服名*/
	int64_t gang_id;/*公会ID*/
	std::string gang_name;/*公会名称*/
	int64_t gang_leader_id;/*公会会长ID*/
	std::string gang_leader_name;/*会长名字*/
	uint32_t force;/*公会战力*/
	uint32_t num;/*公会人数*/
	uint32_t asset;/*公会财富*/
	uint32_t level;/*公会等级*/
	uint32_t create_time;/*公会创建时间*/
	uint32_t expedition_rank;/*本次远征排名*/
	Expedition_Gang_Rank_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(rank);
		w.write_int64(uuid);
		w.write_string(server_name);
		w.write_int64(gang_id);
		w.write_string(gang_name);
		w.write_int64(gang_leader_id);
		w.write_string(gang_leader_name);
		w.write_uint32(force);
		w.write_uint32(num);
		w.write_uint32(asset);
		w.write_uint32(level);
		w.write_uint32(create_time);
		w.write_uint32(expedition_rank);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(rank)  ||  r.read_int64(uuid)  ||  r.read_string(server_name)  ||  r.read_int64(gang_id)  ||  r.read_string(gang_name)  ||  r.read_int64(gang_leader_id)  ||  r.read_string(gang_leader_name)  ||  r.read_uint32(force)  ||  r.read_uint32(num)  ||  r.read_uint32(asset)  ||  r.read_uint32(level)  ||  r.read_uint32(create_time)  ||  r.read_uint32(expedition_rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		rank = 0;
		uuid = 0;
		server_name.clear();
		gang_id = 0;
		gang_name.clear();
		gang_leader_id = 0;
		gang_leader_name.clear();
		force = 0;
		num = 0;
		asset = 0;
		level = 0;
		create_time = 0;
		expedition_rank = 0;
	}
};

/*
 追加技触发
*/
struct Addition_Skill_Trigger_Info {
	int32_t index;/**/
	std::vector<int32_t> effect_vec;/**/
	Addition_Skill_Trigger_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		uint16_t __effect_vec_vec_size = effect_vec.size();
		w.write_uint16(__effect_vec_vec_size);
		for(uint16_t i = 0; i < __effect_vec_vec_size; ++i) {
				w.write_int32(effect_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  )
				return -1;
 		uint16_t __effect_vec_vec_size;
		if(r.read_uint16(__effect_vec_vec_size) )
				return -1;
		effect_vec.reserve(__effect_vec_vec_size);
		for(uint16_t i = 0; i < __effect_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				effect_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		index = 0;
		effect_vec.clear();
	}
};

/*
远征系统据点数据
*/
struct Strong_Hold_Data {
	uint32_t point_id;/*建筑ID*/
	uint32_t type;/*类型	1.基地  2.关卡	3.大城堡	4.中城堡	5.小城堡	6.宝箱区	7.野兽区*/
	std::vector<uint32_t> abut;/*邻近的建筑ID*/
	uint32_t attribution;/*占领所属建筑ID*/
	std::vector<uint32_t> territory;/*建筑领土*/
	uint32_t status;/*据点状态 0.未被初始化状态, 1.中立(和平状态), 2.中立(攻占状态), 11.占领(和平状态), 12.占领(攻占状态)*/
	std::string server_name;/*占领该据点的服务器名字*/
	int64_t uuid;/*占领该据点的服务器ID*/
	int64_t gang_id;/*占领该据点的公会ID*/
	std::string gang_name;/*占领该据点的公会名字*/
	Strong_Hold_Data(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(point_id);
		w.write_uint32(type);
		uint16_t __abut_vec_size = abut.size();
		w.write_uint16(__abut_vec_size);
		for(uint16_t i = 0; i < __abut_vec_size; ++i) {
				w.write_uint32(abut[i]);
		}

		w.write_uint32(attribution);
		uint16_t __territory_vec_size = territory.size();
		w.write_uint16(__territory_vec_size);
		for(uint16_t i = 0; i < __territory_vec_size; ++i) {
				w.write_uint32(territory[i]);
		}

		w.write_uint32(status);
		w.write_string(server_name);
		w.write_int64(uuid);
		w.write_int64(gang_id);
		w.write_string(gang_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(point_id)  ||  r.read_uint32(type)  )
				return -1;
 		uint16_t __abut_vec_size;
		if(r.read_uint16(__abut_vec_size) )
				return -1;
		abut.reserve(__abut_vec_size);
		for(uint16_t i = 0; i < __abut_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				abut.push_back(v);
		}

		if( r.read_uint32(attribution)  )
				return -1;
 		uint16_t __territory_vec_size;
		if(r.read_uint16(__territory_vec_size) )
				return -1;
		territory.reserve(__territory_vec_size);
		for(uint16_t i = 0; i < __territory_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				territory.push_back(v);
		}

		if( r.read_uint32(status)  ||  r.read_string(server_name)  ||  r.read_int64(uuid)  ||  r.read_int64(gang_id)  ||  r.read_string(gang_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
		type = 0;
		abut.clear();
		attribution = 0;
		territory.clear();
		status = 0;
		server_name.clear();
		uuid = 0;
		gang_id = 0;
		gang_name.clear();
	}
};

/*
远征建筑简略信息
*/
struct Expedition_Build_Brief_Info {
	int32_t point_id;/*建筑id*/
	int8_t type;/*类型	1.基地 2.关卡	3.大城堡	4.中城堡	5.小城堡	6.宝箱区	7.野兽区*/
	int8_t state;/*据点状态 0.未被初始化状态, 1.中立(和平状态), 2.中立(攻占状态), 11.占领(和平状态), 12.占领(攻占状态)*/
	int64_t gang_id;/*公会id(未被占为0）*/
	int16_t val1;/*野怪区域，怪物剩余数；对战时，攻方人数；*/
	int16_t val2;/*对战时：守方人数(区域:)*/
	int8_t val3;/*野怪区、宝箱：0不可领；1可领；2已领;刷新怪点：0可见1不可见*/
	Expedition_Build_Brief_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(point_id);
		w.write_int8(type);
		w.write_int8(state);
		w.write_int64(gang_id);
		w.write_int16(val1);
		w.write_int16(val2);
		w.write_int8(val3);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(point_id)  ||  r.read_int8(type)  ||  r.read_int8(state)  ||  r.read_int64(gang_id)  ||  r.read_int16(val1)  ||  r.read_int16(val2)  ||  r.read_int8(val3)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		point_id = 0;
		type = 0;
		state = 0;
		gang_id = 0;
		val1 = 0;
		val2 = 0;
		val3 = 0;
	}
};

/*
远征当前线存在的公会数据
*/
struct Expedition_Gang_Brief_Info {
	int64_t uuid;/*服务器ID*/
	std::string server_name;/*服务器名字*/
	int64_t gang_id;/*公会ID*/
	std::string gang_name;/*公会名字*/
	Expedition_Gang_Brief_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(uuid);
		w.write_string(server_name);
		w.write_int64(gang_id);
		w.write_string(gang_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(uuid)  ||  r.read_string(server_name)  ||  r.read_int64(gang_id)  ||  r.read_string(gang_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		uuid = 0;
		server_name.clear();
		gang_id = 0;
		gang_name.clear();
	}
};

/*
远征驻守玩家数据
*/
struct Expedition_Defend_Info {
	int64_t role_id;/**/
	std::string name;/**/
	int32_t level;/*人物头像*/
	int32_t force;/*战斗力*/
	int32_t point_id;/**/
	Expedition_Defend_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(name);
		w.write_int32(level);
		w.write_int32(force);
		w.write_int32(point_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(name)  ||  r.read_int32(level)  ||  r.read_int32(force)  ||  r.read_int32(point_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		name.clear();
		level = 0;
		force = 0;
		point_id = 0;
	}
};

/*
远征非公会基地信息
x.	城堡+野兽区+宝箱区信息
*/
struct Expedition_Other_Info {
	int64_t defend_gang_id;/*守方*/
	std::string defend_gang_name;/*守方*/
	int64_t attack_gang_id;/*攻方*/
	std::string attack_gang_name;/*攻方*/
	int16_t attack_num;/*攻方人数*/
	int32_t defend_list;/*驻守玩家列表（守方人数）*/
	Expedition_Other_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(defend_gang_id);
		w.write_string(defend_gang_name);
		w.write_int64(attack_gang_id);
		w.write_string(attack_gang_name);
		w.write_int16(attack_num);
		w.write_int32(defend_list);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(defend_gang_id)  ||  r.read_string(defend_gang_name)  ||  r.read_int64(attack_gang_id)  ||  r.read_string(attack_gang_name)  ||  r.read_int16(attack_num)  ||  r.read_int32(defend_list)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		defend_gang_id = 0;
		defend_gang_name.clear();
		attack_gang_id = 0;
		attack_gang_name.clear();
		attack_num = 0;
		defend_list = 0;
	}
};

/*
远征公会基地信息
*/
struct Expedition_Gang_Base_Place_Info {
	int64_t gang_id;/*守方*/
	std::string gang_name;/*守方*/
	int16_t hold_nums;/*占领城池数*/
	std::vector<Item_Info> daily_award;
	int32_t time_army;/*下一批远征军出击时间*/
	int32_t time_knightage;/*下一批公会骑士团出击时间*/
	Expedition_Gang_Base_Place_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(gang_id);
		w.write_string(gang_name);
		w.write_int16(hold_nums);
		uint16_t __daily_award_vec_size = daily_award.size();
		w.write_uint16(__daily_award_vec_size);
		for(uint16_t i = 0; i < __daily_award_vec_size; ++i) {
				daily_award[i].serialize(w);
		}

		w.write_int32(time_army);
		w.write_int32(time_knightage);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(gang_id)  ||  r.read_string(gang_name)  ||  r.read_int16(hold_nums)  )
				return -1;
 		uint16_t __daily_award_vec_size;
		if(r.read_uint16(__daily_award_vec_size) )
				return -1;
		daily_award.reserve(__daily_award_vec_size);
		for(uint16_t i = 0; i < __daily_award_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				daily_award.push_back(v);
		}

		if( r.read_int32(time_army)  ||  r.read_int32(time_knightage)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		gang_id = 0;
		gang_name.clear();
		hold_nums = 0;
		daily_award.clear();
		time_army = 0;
		time_knightage = 0;
	}
};

/*
远征对战英雄信息
*/
struct Expedition_Battle_Hero_Info {
	int32_t facade_id;/*英雄形象id*/
	int8_t type;/*0英雄1怪物*/
	std::string name;/**/
	int16_t level;/**/
	int32_t force;/**/
	Expedition_Battle_Hero_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(facade_id);
		w.write_int8(type);
		w.write_string(name);
		w.write_int16(level);
		w.write_int32(force);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(facade_id)  ||  r.read_int8(type)  ||  r.read_string(name)  ||  r.read_int16(level)  ||  r.read_int32(force)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		facade_id = 0;
		type = 0;
		name.clear();
		level = 0;
		force = 0;
	}
};

/*
远征对战双方信息
*/
struct Expedition_Battle_Role_Info {
	int64_t role_id;/**/
	int8_t type;/*0人1怪物*/
	int32_t head_photo_id;/**/
	std::string name;/**/
	int16_t level;/**/
	int32_t force;/**/
	int8_t monster_num;/*为怪物时，怪物数*/
	Expedition_Battle_Hero_Info hero_info;
	Expedition_Battle_Role_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_int8(type);
		w.write_int32(head_photo_id);
		w.write_string(name);
		w.write_int16(level);
		w.write_int32(force);
		w.write_int8(monster_num);
		hero_info.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_int8(type)  ||  r.read_int32(head_photo_id)  ||  r.read_string(name)  ||  r.read_int16(level)  ||  r.read_int32(force)  ||  r.read_int8(monster_num)  ||  hero_info.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		type = 0;
		head_photo_id = 0;
		name.clear();
		level = 0;
		force = 0;
		monster_num = 0;
		hero_info.reset();
	}
};

/*
新手引导结构信息
*/
struct Guide_Info {
	uint16_t guide_id;/*新手引导id*/
	uint8_t step;/*引导步骤*/
	Guide_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint16(guide_id);
		w.write_uint8(step);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint16(guide_id)  ||  r.read_uint8(step)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		guide_id = 0;
		step = 0;
	}
};

/*
雷神之锤奖品
*/
struct ThorLottery_Reward_Item {
	int8_t index;/*第几次抽到*/
	int32_t item_index;/**/
	int32_t id;/*物品id*/
	int32_t num;/*物品数量*/
	int8_t state;/*绑定状态*/
	ThorLottery_Reward_Item(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(index);
		w.write_int32(item_index);
		w.write_int32(id);
		w.write_int32(num);
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(index)  ||  r.read_int32(item_index)  ||  r.read_int32(id)  ||  r.read_int32(num)  ||  r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		item_index = 0;
		id = 0;
		num = 0;
		state = 0;
	}
};

/*
雷神之锤日志
*/
struct ThorLottery_Log {
	int32_t time;/*secs*/
	std::string role_name;/**/
	int64_t role_id;/**/
	int32_t item_id;/**/
	int32_t item_num;/**/
	int8_t state;/**/
	ThorLottery_Log(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(time);
		w.write_string(role_name);
		w.write_int64(role_id);
		w.write_int32(item_id);
		w.write_int32(item_num);
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(time)  ||  r.read_string(role_name)  ||  r.read_int64(role_id)  ||  r.read_int32(item_id)  ||  r.read_int32(item_num)  ||  r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		time = 0;
		role_name.clear();
		role_id = 0;
		item_id = 0;
		item_num = 0;
		state = 0;
	}
};

/*
邀请好友踩脚印的好友信息
*/
struct Dragon_Vale_Invite_Step_Foot_Friend {
	int64_t role_id;/*角色id*/
	std::string role_name;/*角色名*/
	Dragon_Vale_Invite_Step_Foot_Friend(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
	}
};

/*
智慧试练排行榜人物信息
*/
struct Answer_Rank_Role_Info {
	int64_t role_id;/*角色ID*/
	uint32_t sorce;/*积分*/
	std::string role_name;/*角色名*/
	int32_t level;/*角色等级*/
	Answer_Rank_Role_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint32(sorce);
		w.write_string(role_name);
		w.write_int32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint32(sorce)  ||  r.read_string(role_name)  ||  r.read_int32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		sorce = 0;
		role_name.clear();
		level = 0;
	}
};

/*
调查问卷答案信息
*/
struct Questionnaire_Answer_Info {
	uint8_t qno;/*题号*/
	uint8_t radio;/*单选题答案*/
	std::vector<uint8_t> mult;/*多选题答案*/
	std::string contents;/*问答题答案*/
	Questionnaire_Answer_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(qno);
		w.write_uint8(radio);
		uint16_t __mult_vec_size = mult.size();
		w.write_uint16(__mult_vec_size);
		for(uint16_t i = 0; i < __mult_vec_size; ++i) {
				w.write_uint8(mult[i]);
		}

		w.write_string(contents);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(qno)  ||  r.read_uint8(radio)  )
				return -1;
 		uint16_t __mult_vec_size;
		if(r.read_uint16(__mult_vec_size) )
				return -1;
		mult.reserve(__mult_vec_size);
		for(uint16_t i = 0; i < __mult_vec_size; ++i) {
				uint8_t v;
				if(r.read_uint8(v) )
						return -1;
				mult.push_back(v);
		}

		if( r.read_string(contents)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		qno = 0;
		radio = 0;
		mult.clear();
		contents.clear();
	}
};

/*
战斗ai预告信息
*/
struct AI_Foreshow_Info {
	int64_t monster_role_id;/*怪物role_id*/
	int32_t ai_id;/*ai id*/
	int32_t skill_id;/*技能id*/
	int32_t buffer_id;/*buffer id*/
	int32_t value;/*回合释放：剩余回合；时间释放：剩余时间；血量百分比：百分比100*/
	int8_t type;/*0：即将释放 1：回合释放 2：时间释放 3：血量小于百分比 4：血量大于百分比 5：血量小于值 6：血量大于值*/
	AI_Foreshow_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(monster_role_id);
		w.write_int32(ai_id);
		w.write_int32(skill_id);
		w.write_int32(buffer_id);
		w.write_int32(value);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(monster_role_id)  ||  r.read_int32(ai_id)  ||  r.read_int32(skill_id)  ||  r.read_int32(buffer_id)  ||  r.read_int32(value)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		monster_role_id = 0;
		ai_id = 0;
		skill_id = 0;
		buffer_id = 0;
		value = 0;
		type = 0;
	}
};

/*
场景npc
*/
struct Scene_Npc {
	uint32_t scene_id;/*场景id*/
	uint32_t npc_id;/*npc id*/
	Scene_Npc(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(scene_id);
		w.write_uint32(npc_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(scene_id)  ||  r.read_uint32(npc_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		scene_id = 0;
		npc_id = 0;
	}
};

/*
兑换数据结构
*/
struct Exchange_Info_Msg {
	int32_t id;/*物品id*/
	int32_t amount_left;/*累计兑换*/
	int32_t amount_daily;/*每日限制*/
	Exchange_Info_Msg(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(id);
		w.write_int32(amount_left);
		w.write_int32(amount_daily);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(id)  ||  r.read_int32(amount_left)  ||  r.read_int32(amount_daily)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		amount_left = 0;
		amount_daily = 0;
	}
};

/*
世界boss伤害排名信息
*/
struct World_Boss_Rank_Info {
	int64_t role_id;/**/
	std::string role_name;/**/
	int32_t damage;/*伤害*/
	double damage_rate;/*伤害百分比*/
	int32_t rank;/*排名*/
	int8_t career;/*职业 1战士 2刺客 3法师*/
	World_Boss_Rank_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int32(damage);
		w.write_double(damage_rate);
		w.write_int32(rank);
		w.write_int8(career);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int32(damage)  ||  r.read_double(damage_rate)  ||  r.read_int32(rank)  ||  r.read_int8(career)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		damage = 0;
		damage_rate = 0.0;
		rank = 0;
		career = 0;
	}
};

/*
援兵信息
*/
struct Expedition_Help_Info {
	int32_t index;/*排序*/
	std::string role_name;/**/
	int32_t force;/**/
	int8_t state;/**/
	Expedition_Help_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(index);
		w.write_string(role_name);
		w.write_int32(force);
		w.write_int8(state);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(index)  ||  r.read_string(role_name)  ||  r.read_int32(force)  ||  r.read_int8(state)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		index = 0;
		role_name.clear();
		force = 0;
		state = 0;
	}
};

/*
R2平台邮件邀请信息
*/
struct R2_Mail_Invite_Info {
	int64_t role_id;/*邀请人创号的角色ID*/
	uint32_t time;/*邀请时间*/
	uint8_t status;/*邀请状态: 0.未处理, 1.已注册, 2.升级中, 3.处理失败*/
	std::string mail;/*邀请邮件帐号*/
	R2_Mail_Invite_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_uint32(time);
		w.write_uint8(status);
		w.write_string(mail);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_uint32(time)  ||  r.read_uint8(status)  ||  r.read_string(mail)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		time = 0;
		status = 0;
		mail.clear();
	}
};

/*
助阵英雄信息
*/
struct Hero_Assist_Info {
	std::vector<Int_Double> props;
	int32_t skill_id;/*助阵技能*/
	int32_t skill_lv;/*助阵技能等级*/
	int32_t intimacy_lv;/*亲密度等级*/
	int32_t intimacy_val;/*当前亲密度值*/
	int32_t state;/*当前助阵英雄状态*/
	int32_t communite_times;/*对话次数*/
	int32_t gift_times;/*送礼品次数*/
	Hero_Assist_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __props_vec_size = props.size();
		w.write_uint16(__props_vec_size);
		for(uint16_t i = 0; i < __props_vec_size; ++i) {
				props[i].serialize(w);
		}

		w.write_int32(skill_id);
		w.write_int32(skill_lv);
		w.write_int32(intimacy_lv);
		w.write_int32(intimacy_val);
		w.write_int32(state);
		w.write_int32(communite_times);
		w.write_int32(gift_times);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __props_vec_size;
		if(r.read_uint16(__props_vec_size) )
				return -1;
		props.reserve(__props_vec_size);
		for(uint16_t i = 0; i < __props_vec_size; ++i) {
				Int_Double v;
				if(v.deserialize(r))
						return -1;
				props.push_back(v);
		}

		if( r.read_int32(skill_id)  ||  r.read_int32(skill_lv)  ||  r.read_int32(intimacy_lv)  ||  r.read_int32(intimacy_val)  ||  r.read_int32(state)  ||  r.read_int32(communite_times)  ||  r.read_int32(gift_times)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		props.clear();
		skill_id = 0;
		skill_lv = 0;
		intimacy_lv = 0;
		intimacy_val = 0;
		state = 0;
		communite_times = 0;
		gift_times = 0;
	}
};

/*
组队竞技积分信息
*/
struct Team_Score_Info {
	int32_t rank;/*排名*/
	std::string role_name;/*角色名*/
	std::string role_area;/*区服名*/
	int32_t score;/*积分*/
	int32_t force;/*战力*/
	Team_Score_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(rank);
		w.write_string(role_name);
		w.write_string(role_area);
		w.write_int32(score);
		w.write_int32(force);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(rank)  ||  r.read_string(role_name)  ||  r.read_string(role_area)  ||  r.read_int32(score)  ||  r.read_int32(force)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		rank = 0;
		role_name.clear();
		role_area.clear();
		score = 0;
		force = 0;
	}
};

/*
活动兑换商店物品信息
*/
struct Activity_Exchange_Mall_Goods_Info {
	uint32_t id;/*商品唯一ID*/
	uint32_t item_id;/*兑换道具ID*/
	uint32_t already_exchage;/*已经兑换道具数量*/
	uint32_t exchange_id;/*使用兑换的ID,可以为道具ID或资源ID*/
	uint32_t exchange_num;/*兑换所需数量*/
	uint32_t limit_lv;/*等级限制*/
	uint32_t limit_exchange;/*限制兑换次数*/
	uint32_t classify;/*大分类页签*/
	int32_t subclass;/*小分类页签,-1表示没有小分类*/
	uint32_t sort;/*显示顺序*/
	uint32_t act_id;/*活动ID*/
	Activity_Exchange_Mall_Goods_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(item_id);
		w.write_uint32(already_exchage);
		w.write_uint32(exchange_id);
		w.write_uint32(exchange_num);
		w.write_uint32(limit_lv);
		w.write_uint32(limit_exchange);
		w.write_uint32(classify);
		w.write_int32(subclass);
		w.write_uint32(sort);
		w.write_uint32(act_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(item_id)  ||  r.read_uint32(already_exchage)  ||  r.read_uint32(exchange_id)  ||  r.read_uint32(exchange_num)  ||  r.read_uint32(limit_lv)  ||  r.read_uint32(limit_exchange)  ||  r.read_uint32(classify)  ||  r.read_int32(subclass)  ||  r.read_uint32(sort)  ||  r.read_uint32(act_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		item_id = 0;
		already_exchage = 0;
		exchange_id = 0;
		exchange_num = 0;
		limit_lv = 0;
		limit_exchange = 0;
		classify = 0;
		subclass = 0;
		sort = 0;
		act_id = 0;
	}
};

/*
远征伤害排名
*/
struct Expedition_Role_Rank {
	std::string role_name;/*角色*/
	std::string gang_name;/*公会*/
	int32_t demage;/*伤害*/
	int32_t force;/*战力*/
	std::vector<Item_Info> award_list;
	int32_t rankId;/*客户端用*/
	Expedition_Role_Rank(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(role_name);
		w.write_string(gang_name);
		w.write_int32(demage);
		w.write_int32(force);
		uint16_t __award_list_vec_size = award_list.size();
		w.write_uint16(__award_list_vec_size);
		for(uint16_t i = 0; i < __award_list_vec_size; ++i) {
				award_list[i].serialize(w);
		}

		w.write_int32(rankId);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(role_name)  ||  r.read_string(gang_name)  ||  r.read_int32(demage)  ||  r.read_int32(force)  )
				return -1;
 		uint16_t __award_list_vec_size;
		if(r.read_uint16(__award_list_vec_size) )
				return -1;
		award_list.reserve(__award_list_vec_size);
		for(uint16_t i = 0; i < __award_list_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				award_list.push_back(v);
		}

		if( r.read_int32(rankId)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_name.clear();
		gang_name.clear();
		demage = 0;
		force = 0;
		award_list.clear();
		rankId = 0;
	}
};

/*
时装部位信息
*/
struct Fashion_Part_Info {
	int8_t part_id;/*部位*/
	int16_t lv;/*强化等级*/
	int32_t exp;/*当前能量值*/
	int32_t used_fashion_id;/*当前装戴的时装id*/
	std::vector<int32_t> fashion_id_vec;/**/
	Fashion_Part_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(part_id);
		w.write_int16(lv);
		w.write_int32(exp);
		w.write_int32(used_fashion_id);
		uint16_t __fashion_id_vec_vec_size = fashion_id_vec.size();
		w.write_uint16(__fashion_id_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_id_vec_vec_size; ++i) {
				w.write_int32(fashion_id_vec[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(part_id)  ||  r.read_int16(lv)  ||  r.read_int32(exp)  ||  r.read_int32(used_fashion_id)  )
				return -1;
 		uint16_t __fashion_id_vec_vec_size;
		if(r.read_uint16(__fashion_id_vec_vec_size) )
				return -1;
		fashion_id_vec.reserve(__fashion_id_vec_vec_size);
		for(uint16_t i = 0; i < __fashion_id_vec_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				fashion_id_vec.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		part_id = 0;
		lv = 0;
		exp = 0;
		used_fashion_id = 0;
		fashion_id_vec.clear();
	}
};

/*
生活技能
*/
struct Life_Skill_Unit {
	int32_t skill_id;/*技能id*/
	uint8_t skill_lv;/*技能等级*/
	uint16_t skilled_degree;/*技能熟练度*/
	Life_Skill_Unit(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(skill_id);
		w.write_uint8(skill_lv);
		w.write_uint16(skilled_degree);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(skill_id)  ||  r.read_uint8(skill_lv)  ||  r.read_uint16(skilled_degree)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		skill_id = 0;
		skill_lv = 0;
		skilled_degree = 0;
	}
};

/*
荣誉竞技场战报
type  公告类型   val_1: 
0： XXX战胜了XXX，获得
1：XXX战胜了XXX，赢得了X连胜，获得
2：XXX终结了XXX的X连胜，获得
3：XXX过关斩将，杀入前100名，大家都说好厉害！
4：XXX过关斩将，杀入前50名，杀神降临！
5：XXX过关斩将，杀入前10名，已经无人可挡！
6：XXX取代了XXX，成为新晋的第一名！！勇者无畏！
*/
struct Honor_Arena_Report {
	int64_t win_role_id;/**/
	int64_t lose_role_id;/**/
	std::string win_role_name;/**/
	std::string lose_role_name;/**/
	Int_Int type;
	std::vector<Item_Info> items;
	Honor_Arena_Report(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(win_role_id);
		w.write_int64(lose_role_id);
		w.write_string(win_role_name);
		w.write_string(lose_role_name);
		type.serialize(w);
		uint16_t __items_vec_size = items.size();
		w.write_uint16(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				items[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(win_role_id)  ||  r.read_int64(lose_role_id)  ||  r.read_string(win_role_name)  ||  r.read_string(lose_role_name)  ||  type.deserialize(r)  )
				return -1;
 		uint16_t __items_vec_size;
		if(r.read_uint16(__items_vec_size) )
				return -1;
		items.reserve(__items_vec_size);
		for(uint16_t i = 0; i < __items_vec_size; ++i) {
				Item_Info v;
				if(v.deserialize(r))
						return -1;
				items.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		win_role_id = 0;
		lose_role_id = 0;
		win_role_name.clear();
		lose_role_name.clear();
		type.reset();
		items.clear();
	}
};

/*
荣誉竞技场排名信息
*/
struct Honor_Arena_Rank_Info {
	int64_t role_id;/**/
	std::string role_name;/**/
	std::string guild_name;/*公会*/
	int32_t rank;/*排名*/
	int32_t force;/*战力*/
	int32_t score;/*积分*/
	Honor_Arena_Rank_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_string(guild_name);
		w.write_int32(rank);
		w.write_int32(force);
		w.write_int32(score);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_string(guild_name)  ||  r.read_int32(rank)  ||  r.read_int32(force)  ||  r.read_int32(score)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		guild_name.clear();
		rank = 0;
		force = 0;
		score = 0;
	}
};

/*
带时装信息的玩家基本信息
*/
struct Role_Info_With_Avatar {
	Role_Info base_info;
	std::vector<int32_t> avatar;/*avatar*/
	Role_Info_With_Avatar(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		base_info.serialize(w);
		uint16_t __avatar_vec_size = avatar.size();
		w.write_uint16(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				w.write_int32(avatar[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( base_info.deserialize(r)  )
				return -1;
 		uint16_t __avatar_vec_size;
		if(r.read_uint16(__avatar_vec_size) )
				return -1;
		avatar.reserve(__avatar_vec_size);
		for(uint16_t i = 0; i < __avatar_vec_size; ++i) {
				int32_t v;
				if(r.read_int32(v) )
						return -1;
				avatar.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		base_info.reset();
		avatar.clear();
	}
};

/*
运营活动标签项信息
*/
struct Oper_Activity_Item_Info {
	uint32_t act_id;/*活动ID*/
	uint32_t end_time;/*结束时间*/
	uint32_t count_time;/*结算时间*/
	std::vector<Uint_Uint> uu1;
	std::vector<Uint_Uint> uu2;
	std::vector<Uint_Uint> uu3;
	std::vector<Uint_Uint> uu4;
	std::vector<uint32_t> vu32;/*预留*/
	Oper_Activity_Item_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(act_id);
		w.write_uint32(end_time);
		w.write_uint32(count_time);
		uint16_t __uu1_vec_size = uu1.size();
		w.write_uint16(__uu1_vec_size);
		for(uint16_t i = 0; i < __uu1_vec_size; ++i) {
				uu1[i].serialize(w);
		}

		uint16_t __uu2_vec_size = uu2.size();
		w.write_uint16(__uu2_vec_size);
		for(uint16_t i = 0; i < __uu2_vec_size; ++i) {
				uu2[i].serialize(w);
		}

		uint16_t __uu3_vec_size = uu3.size();
		w.write_uint16(__uu3_vec_size);
		for(uint16_t i = 0; i < __uu3_vec_size; ++i) {
				uu3[i].serialize(w);
		}

		uint16_t __uu4_vec_size = uu4.size();
		w.write_uint16(__uu4_vec_size);
		for(uint16_t i = 0; i < __uu4_vec_size; ++i) {
				uu4[i].serialize(w);
		}

		uint16_t __vu32_vec_size = vu32.size();
		w.write_uint16(__vu32_vec_size);
		for(uint16_t i = 0; i < __vu32_vec_size; ++i) {
				w.write_uint32(vu32[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(act_id)  ||  r.read_uint32(end_time)  ||  r.read_uint32(count_time)  )
				return -1;
 		uint16_t __uu1_vec_size;
		if(r.read_uint16(__uu1_vec_size) )
				return -1;
		uu1.reserve(__uu1_vec_size);
		for(uint16_t i = 0; i < __uu1_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				uu1.push_back(v);
		}

		uint16_t __uu2_vec_size;
		if(r.read_uint16(__uu2_vec_size) )
				return -1;
		uu2.reserve(__uu2_vec_size);
		for(uint16_t i = 0; i < __uu2_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				uu2.push_back(v);
		}

		uint16_t __uu3_vec_size;
		if(r.read_uint16(__uu3_vec_size) )
				return -1;
		uu3.reserve(__uu3_vec_size);
		for(uint16_t i = 0; i < __uu3_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				uu3.push_back(v);
		}

		uint16_t __uu4_vec_size;
		if(r.read_uint16(__uu4_vec_size) )
				return -1;
		uu4.reserve(__uu4_vec_size);
		for(uint16_t i = 0; i < __uu4_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				uu4.push_back(v);
		}

		uint16_t __vu32_vec_size;
		if(r.read_uint16(__vu32_vec_size) )
				return -1;
		vu32.reserve(__vu32_vec_size);
		for(uint16_t i = 0; i < __vu32_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				vu32.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		act_id = 0;
		end_time = 0;
		count_time = 0;
		uu1.clear();
		uu2.clear();
		uu3.clear();
		uu4.clear();
		vu32.clear();
	}
};

/*
远征占城榜数据
*/
struct Expedition_Occupy_Castle {
	int16_t rank;/**/
	std::string role_name;/**/
	std::string gang_name;/**/
	int32_t level;/**/
	int32_t force;/**/
	int16_t nums;/*占领城堡数*/
	Expedition_Occupy_Castle(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int16(rank);
		w.write_string(role_name);
		w.write_string(gang_name);
		w.write_int32(level);
		w.write_int32(force);
		w.write_int16(nums);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int16(rank)  ||  r.read_string(role_name)  ||  r.read_string(gang_name)  ||  r.read_int32(level)  ||  r.read_int32(force)  ||  r.read_int16(nums)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		rank = 0;
		role_name.clear();
		gang_name.clear();
		level = 0;
		force = 0;
		nums = 0;
	}
};

/*
篝火收集信息
*/
struct Campfire_Collect_Info {
	Coord f_coord;
	int8_t result;/*激活状态0：未激活，1：激活*/
	uint32_t role_id;/*火柴人类型id*/
	std::string name;/*火柴人名字*/
	Campfire_Collect_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		f_coord.serialize(w);
		w.write_int8(result);
		w.write_uint32(role_id);
		w.write_string(name);
	}

	int deserialize(Block_Buffer & r) {
		if( f_coord.deserialize(r)  ||  r.read_int8(result)  ||  r.read_uint32(role_id)  ||  r.read_string(name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		f_coord.reset();
		result = 0;
		role_id = 0;
		name.clear();
	}
};

/*
篝火排名
*/
struct campfire_rank_info {
	int64_t role_id;/*角色id*/
	std::string role_name;/*角色*/
	int32_t time;/*收集花费时间*/
	uint32_t rank;/*玩家名次*/
	campfire_rank_info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
		w.write_string(role_name);
		w.write_int32(time);
		w.write_uint32(rank);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  ||  r.read_string(role_name)  ||  r.read_int32(time)  ||  r.read_uint32(rank)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
		role_name.clear();
		time = 0;
		rank = 0;
	}
};

/*
英雄信息
*/
struct Hero_Info {
	int32_t hero_id;/*英雄ID*/
	int32_t hero_index;/*英雄index*/
	int32_t hero_facade_id;/*英雄外观ID*/
	std::string hero_name;/*英雄名字*/
	int8_t is_outer;/*是否跟随英雄*/
	int8_t is_fighter;/*是否战斗英雄*/
	int32_t level;/*英雄等级*/
	int32_t formation;/*英雄阵型*/
	int32_t awake_lvl;/*觉醒等级*/
	double exp;/*英雄经验*/
	std::vector<Skill_Info> skill_info_vec;
	std::vector<Int_Double> prop_value;
	std::vector<Artifact_Info> artifact_vec;
	Hero_Assist_Info hero_assist_info;
	int32_t fight_date;/*出战时间戳*/
	Hero_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(hero_id);
		w.write_int32(hero_index);
		w.write_int32(hero_facade_id);
		w.write_string(hero_name);
		w.write_int8(is_outer);
		w.write_int8(is_fighter);
		w.write_int32(level);
		w.write_int32(formation);
		w.write_int32(awake_lvl);
		w.write_double(exp);
		uint16_t __skill_info_vec_vec_size = skill_info_vec.size();
		w.write_uint16(__skill_info_vec_vec_size);
		for(uint16_t i = 0; i < __skill_info_vec_vec_size; ++i) {
				skill_info_vec[i].serialize(w);
		}

		uint16_t __prop_value_vec_size = prop_value.size();
		w.write_uint16(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				prop_value[i].serialize(w);
		}

		uint16_t __artifact_vec_vec_size = artifact_vec.size();
		w.write_uint16(__artifact_vec_vec_size);
		for(uint16_t i = 0; i < __artifact_vec_vec_size; ++i) {
				artifact_vec[i].serialize(w);
		}

		hero_assist_info.serialize(w);
		w.write_int32(fight_date);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(hero_id)  ||  r.read_int32(hero_index)  ||  r.read_int32(hero_facade_id)  ||  r.read_string(hero_name)  ||  r.read_int8(is_outer)  ||  r.read_int8(is_fighter)  ||  r.read_int32(level)  ||  r.read_int32(formation)  ||  r.read_int32(awake_lvl)  ||  r.read_double(exp)  )
				return -1;
 		uint16_t __skill_info_vec_vec_size;
		if(r.read_uint16(__skill_info_vec_vec_size) )
				return -1;
		skill_info_vec.reserve(__skill_info_vec_vec_size);
		for(uint16_t i = 0; i < __skill_info_vec_vec_size; ++i) {
				Skill_Info v;
				if(v.deserialize(r))
						return -1;
				skill_info_vec.push_back(v);
		}

		uint16_t __prop_value_vec_size;
		if(r.read_uint16(__prop_value_vec_size) )
				return -1;
		prop_value.reserve(__prop_value_vec_size);
		for(uint16_t i = 0; i < __prop_value_vec_size; ++i) {
				Int_Double v;
				if(v.deserialize(r))
						return -1;
				prop_value.push_back(v);
		}

		uint16_t __artifact_vec_vec_size;
		if(r.read_uint16(__artifact_vec_vec_size) )
				return -1;
		artifact_vec.reserve(__artifact_vec_vec_size);
		for(uint16_t i = 0; i < __artifact_vec_vec_size; ++i) {
				Artifact_Info v;
				if(v.deserialize(r))
						return -1;
				artifact_vec.push_back(v);
		}

		if( hero_assist_info.deserialize(r)  ||  r.read_int32(fight_date)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		hero_id = 0;
		hero_index = 0;
		hero_facade_id = 0;
		hero_name.clear();
		is_outer = 0;
		is_fighter = 0;
		level = 0;
		formation = 0;
		awake_lvl = 0;
		exp = 0.0;
		skill_info_vec.clear();
		prop_value.clear();
		artifact_vec.clear();
		hero_assist_info.reset();
		fight_date = 0;
	}
};

/*
在线队员信息
*/
struct Online_Teamer_Info {
	Role_Team_Info role;
	int8_t online;/*0不在线 1 在线*/
	int8_t level;/*段位*/
	Hero_Info hero;
	Online_Teamer_Info(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		role.serialize(w);
		w.write_int8(online);
		w.write_int8(level);
		hero.serialize(w);
	}

	int deserialize(Block_Buffer & r) {
		if( role.deserialize(r)  ||  r.read_int8(online)  ||  r.read_int8(level)  ||  hero.deserialize(r)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role.reset();
		online = 0;
		level = 0;
		hero.reset();
	}
};



#endif