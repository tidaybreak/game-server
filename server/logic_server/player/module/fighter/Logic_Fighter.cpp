/*
 * Logic_Fighter.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: ti
 */

#include "task/Tasker.h"
#include "fighter/Fighter_Struct.h"
#include "Logic_Fighter.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Role.h"
#include "Config_Cache_Scene.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Fight_Struct.h"
#include "Msg_Inner_Struct.h"
#include "Fighter_Detail.h"
#include "Err_Code.h"
#include "Logic_Player_Struct.h"
#include "pack/Pack_Def.h"
#include "task/Task_Struct.h"
#include "Logic_Player.h"
#include "covenant/Covenant_Def.h"
#include "Msg_Active_Struct.h"
#include "equiper/Equiper_Func.h"
#include "knight_trials/Knight_Trials_Manager.h"
#include "Logic_Fighter_Struct.h"
#include "Config_Cache_Dungeon.h"
#include "reward/Reward_Def.h"

Logic_Fighter::Logic_Fighter(void) {
	reset();
}

Logic_Fighter::~Logic_Fighter() {
	// TODO Auto-generated destructor stub
}

void Logic_Fighter::reset(void) {
	fighter_detail_ = 0;

	sign_current_blood_ = 0.0;
	sign_current_morale_ = 0.0;
	sign_current_strength_ = 0.0;
	sign_current_blood_pool_ = 0.0;
}

int Logic_Fighter::create_init(Fighter_Detail &detail) {
	detail.blood_current.basic.basic_value = 999999;
	detail.blood_pool_current.basic.basic_value = 30000000;
	detail.strength_current.basic.basic_value = MAX_PHY_POWER;

	detail.modify_normal_property(PT_LEVEL, O_SET, CONFIG_CACHE_ROLE->player_init_cache().level);

	// 升级加技能点
	int skill_point_ = CONFIG_CACHE_ROLE->level_up_add_skill_points(CONFIG_CACHE_ROLE->player_init_cache().level);
	detail.ttl_skill_points_ = skill_point_;

	detail.update_force();
	KNIGHT_TRIALS_MANAGER->insert_force_role_id_match(detail.force, detail.role_id);
	return 0;
}

void Logic_Fighter::load_detail(Fighter_Detail &detail) {
	fighter_detail_ = &detail;
}

void Logic_Fighter::save_detail(Fighter_Detail &detail) {
	if (fighter_detail().is_change) {
		detail = fighter_detail();
		detail.ttl_force = detail.force + player_self()->get_fight_hero_force();
		if(detail.highest_force < detail.ttl_force) {
			detail.highest_force = detail.ttl_force;
		}
		fighter_detail_->is_change = false;
	}
}

void Logic_Fighter::serialize_fighter_detail(Block_Buffer& buf){
	if(fighter_detail_ != NULL){
		fighter_detail_->serialize(buf);
	}
}

int Logic_Fighter::sync_scene_module_info(Block_Buffer &buf) {
	fighter_detail().serialize(buf);
	return 0;
}

void Logic_Fighter::init_skill(const Player_Init_Cache& init_config) {
	int init_talent_id = 1;
	if (!fighter_detail_->talent_map.empty()) {
		init_talent_id = fighter_detail_->cur_used_talent_;
	}

	Player_Init_Cache::Role_Default_Skill_Map::const_iterator career_it =
			init_config.default_skill.find(career());
	if (career_it == init_config.default_skill.end()) return;
	Player_Init_Cache::Default_Skill_Map::const_iterator find_it =
			career_it->second.find(init_talent_id);
	if (find_it == career_it->second.end()) return;

	fighter_detail_->default_skill = find_it->second.val_1;

	// 技能初始化
	if (fighter_detail_->talent_map.empty()) {
		Talent talent;
		talent.reset();
		talent.talent_id = init_talent_id;
		talent.skill_point = fighter_detail_->ttl_skill_points_;

		Skill_DB_Info skill;
		skill.skill_id = find_it->second.val_1;
		skill.skill_level = 1;
		skill.skill_loc = 0;
		talent.skill_db_info_map[skill.skill_id] = skill;

		skill.reset();
		skill.skill_id = find_it->second.val_2;
		skill.skill_level = 1;
		skill.skill_loc = 1;
		talent.skill_db_info_map[skill.skill_id] = skill;

		fighter_detail_->talent_map[talent.talent_id] = talent;
		fighter_detail_->cur_used_talent_ = 1;
	}
}

void Logic_Fighter::init_phy_power(void){
	if(fighter_detail_->phy_power_recover_time != 0){
		Time_Value time = Time_Value(fighter_detail_->phy_power_recover_time);
		Time_Value now = Time_Value::gettimeofday();
		if(now.sec() - fighter_detail_->phy_power_recover_time > 0){
			int spend_time = now.sec() - fighter_detail_->phy_power_recover_time;
			int count = spend_time/PHY_POWER_RECOVER_TIME_VALUE;
			count += 1;
			int phy_power_cur = fighter_detail_->current_strength();
			phy_power_cur += count * PHY_POWER_RECOVER_VALUE;
			if(phy_power_cur >= fighter_detail_->max_strength()){
				phy_power_cur = fighter_detail_->max_strength();
				fighter_detail_->phy_power_recover_time = 0;
			}else{
				int last_time = PHY_POWER_RECOVER_TIME_VALUE - spend_time%PHY_POWER_RECOVER_TIME_VALUE;
				Time_Value next_recover_time = now + Time_Value(last_time);
				fighter_detail_->phy_power_recover_time = next_recover_time.sec();
			}
			set_phy_power(phy_power_cur);
		}
	}
}

int Logic_Fighter::trigger_daily_zero(const Time_Value &now, bool logic_check/* = false*/){
	return 0;
}

int Logic_Fighter::trigger_daily_six(const Time_Value &now, bool logic_check/* = false*/){
	// 每天6点恢复50点体力 重置购买次数
	if(!fighter_detail_ || player_self() == NULL){
		return -1;
	}
	bool b_change = false;
	if(fighter_detail_->current_strength() < fighter_detail_->max_strength()){
		int rc_strength = PHY_POWER_DAILY_RECOVER;
		if(logic_check){
			int spand_time = now.sec() - player_self()->base_detail().last_trigger_daily_six.sec();
			if(spand_time >= Time_Value::ONE_DAY_IN_SECS*2){
				rc_strength = PHY_POWER_DAILY_RECOVER*2;
			}
		}
		int new_strength = rc_strength + fighter_detail_->current_strength();
		if(new_strength > fighter_detail_->max_strength()){
			new_strength = fighter_detail_->max_strength();
		}
		fighter_detail_->strength_current.basic.basic_value = new_strength;
		fighter_detail_->detail_change();
		b_change = true;
	}
	if(fighter_detail_->phy_power_buy_times != 0){
		fighter_detail_->phy_power_buy_times = 0;
		fighter_detail_->detail_change();
		b_change = true;
	}
	if(b_change && !logic_check){
		//sync scene
		MSG_20100030 msg;
		msg.phy_power_buy_times = 0;
		msg.phy_power_cur = fighter_detail_->current_strength();
		msg.phy_power_recover_time = fighter_detail_->phy_power_recover_time;
		THIS_SEND_TO_SCENE(msg);
	}
	return 0;
}

void Logic_Fighter::module_init(void) {
	sign_current_blood_ = fighter_detail_->current_blood();
	sign_current_morale_ = fighter_detail_->current_morale();
	sign_current_strength_ = fighter_detail_->current_strength();
	sign_current_blood_pool_ = fighter_detail_->current_blood_pool();

	if (level() <= 0) {
		fighter_detail_->modify_normal_property(PT_LEVEL, O_SET, CONFIG_CACHE_ROLE->player_init_cache().level);
		MSG_USER("error level:%d, role:%ld", level(), role_id());
	}

	if (level() > MAX_LEVEL) {
		MSG_USER("error level:%d, role:%ld", level(), role_id());
		fighter_detail_->modify_normal_property(PT_LEVEL, O_SET, MAX_LEVEL);
	}

	// 等级基础属性
	const Prop_Value_Vec *prop_base_value = CONFIG_CACHE_ROLE->level_fighter_prop_cache(level());
	if (prop_base_value) {
		for (Prop_Value_Vec::const_iterator it = prop_base_value->begin(); it != prop_base_value->end(); ++it) {
			Property_Type prop_type = Property_Type((*it).id);
			if (prop_type == PT_SKILL_POINT) continue;
			fighter_detail_->modify_basic_property(prop_type, AT_BASIC, O_SET, (*it).value);
		}
	} else {
		MSG_USER("base property config error, level:%d", level());
	}

	// 一二级转化率
	const Player_Init_Cache &init_config = CONFIG_CACHE_ROLE->player_init_cache();
	Player_Init_Cache::Prop_Conversion::const_iterator find_it = init_config.prop_conversion.find(career());
	if (find_it != init_config.prop_conversion.end()) {
		for (Player_Init_Cache::Prop_Value_Vec::const_iterator it = find_it->second.begin(); it != find_it->second.end(); ++it) {
			Property_Type prop_type = Property_Type((*it).id);
			fighter_detail_->modify_percent_property(prop_type, AT_CONVERSION, O_SET, (*it).value);
		}
	}

	// 基础属性
	for (Player_Init_Cache::Prop_Value_Vec::const_iterator it = init_config.initial_attribute.begin(); it != init_config.initial_attribute.end(); ++it) {
		Property_Type prop_type = Property_Type((*it).id);
		fighter_detail_->modify_basic_property(prop_type, AT_BASIC, O_SET, (*it).value);
	}

	// 预防升级经验减少
	double exp_max = CONFIG_CACHE_ROLE->level_up_need_exp_map(level());
	if (exp_max > 1.0 && fighter_detail_->fetch_normal_property(PT_EXP_CURRENT) > exp_max) {
		fighter_detail_->modify_normal_property(PT_EXP_CURRENT, O_SET, exp_max - 1);
	}

	init_skill(init_config);
}

void Logic_Fighter::sign_in(void) {
	if(!fighter_detail_->ttl_force) {
		fighter_detail_->ttl_force = fighter_detail_->force + player_self()->get_fight_hero_force();
	}
	if(fighter_detail_->highest_force < fighter_detail_->ttl_force) {
		fighter_detail_->highest_force = fighter_detail_->ttl_force;
	}
}

void Logic_Fighter::sign_out(void) {
	fighter_detail_->ttl_force = fighter_detail_->force + player_self()->get_fight_hero_force();
	if(fighter_detail_->highest_force < fighter_detail_->ttl_force) {
		fighter_detail_->highest_force = fighter_detail_->ttl_force;
		fighter_detail_->detail_change();
	}
}

void Logic_Fighter::reset_current_prop(void) {

	// 刷新被动技能加属性
	fighter_detail_->set_be_skill_attr();

	fighter_detail_->modify_basic_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, sign_current_blood_);
	fighter_detail_->modify_basic_property(PT_MORALE_CURRENT, AT_BASIC, O_SET, sign_current_morale_);
	fighter_detail_->modify_basic_property(PT_STRENGTH_CURRENT, AT_BASIC, O_SET, sign_current_strength_);
	fighter_detail_->modify_basic_property(PT_BLOOD_POOL_CURRENT, AT_BASIC, O_SET, sign_current_blood_pool_);

	// 预防空血
	double current_blood = fighter_detail_->current_blood();
	double max_blood = fighter_detail_->fetch_fight_property(PT_BLOOD_CURRENT);
	if (current_blood < 0.0001 || isnan(current_blood) || isinf(current_blood)) {
		double survive_blood = fighter_detail_->blood_max.total() * 0.1;
		if (survive_blood < 1.0) { /// 最少回1滴血
			survive_blood = 1.0;
		}
		fighter_detail_->modify_basic_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, survive_blood);
	} else if (current_blood < max_blood) {
		double cur_blood_pool = fighter_detail_->fetch_fight_property(PT_BLOOD_POOL_CURRENT);
		double delta = max_blood - current_blood;
		delta = delta < cur_blood_pool ? delta : cur_blood_pool;

		fighter_detail_->modify_basic_property(PT_BLOOD_CURRENT, AT_BASIC, O_SET, current_blood+delta);
		fighter_detail_->modify_basic_property(PT_BLOOD_POOL_CURRENT, AT_BASIC, O_SET, cur_blood_pool - delta);
	}
	//
	//刷新体力
	//init_phy_power();

	// 刷新战力
	fighter_detail_->update_force();
}

void Logic_Fighter::refresh_fighter_detail(Block_Buffer &detail_buf) {
	fighter_detail_->deserialize(detail_buf);
}

void Logic_Fighter::hook_fighter_level(const int new_level, const int src_level) {
	fighter_detail_->modify_normal_property(PT_LEVEL, O_SET, new_level);

	// 玩家等级更新--xiaoliang
	DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(this->role_id());
	if (iter != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		iter->second->level = new_level;
	}
	{//通知聊天服
		MSG_20200025 inner_msg;
		inner_msg.level = new_level;
		Block_Buffer buf;
		buf.make_message(inner_msg.msg_id);
		inner_msg.serialize(buf);
		buf.finish_message();
		this->send_to_local_chat(buf);
	}
	//监听等级;推荐好友
	player_self()->friend_listen_level_up(new_level);

	// 符石
	player_self()->update_rune_stone_equip_cap();

	//在线奖励
	player_self()->reward_back_listen_level_up(new_level);

	//VIP
	player_self()->vip_listen_level_up(new_level);

	//开服活动
	player_self()->active_content_listen_level(new_level);

	player_self()->active_listen_level_up(new_level);

	player_self()->public_listen_level_up(new_level);

	/*******************流水放到最后********************/
//	CREATE TABLE `stream_level` (
//	  `player_id` bigint(20) unsigned NOT NULL COMMENT '角色ID',
//	  `level` tinyint(3) unsigned NOT NULL COMMENT '玩家等级',
//	  `scene_id` int(10) unsigned NOT NULL COMMENT '场景ID',
//	  `time` int(10) unsigned NOT NULL COMMENT '发生时间戳',
//	  KEY `idx_player_id` (`player_id`),
//	  KEY `idx_scene_id` (`scene_id`),
//	  KEY `idx_time` (`time`)
//	) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='等级流水';
	MSG_STREAM("`stream_level`(`player_id`, `level`, `scene_id`, `time`) VALUES (%ld,%d,%d,%d)",
				player_self()->role_id(), new_level,player_self()->scene_id() ,Time_Value::gettimeofday().sec());
}

void Logic_Fighter::hook_kill_player(role_id_t role_id) {
	task_listen_win_player(role_id);
}

void Logic_Fighter::hook_kill_monster(int monster_id, int amount) {
	task_listen_kill_monster(monster_id, amount);
//	task_listen_limit_time_kill_boss(monster_id, amount);
	player_self()->achievement_listen_kill_monster_nums(amount);
	player_self()->achievement_listen_kill_boss(monster_id);
	player_self()->task_listen_main_scene_npc_battle(monster_id, amount);
}

void Logic_Fighter::hook_kill_monster_point(int scene_id, int id) {
	task_listen_kill_monster_group(scene_id, id);
	task_listen_fight(scene_id, id);
}

void Logic_Fighter::hook_kill_monster_group(int scene_id, int id){
	player_self()->task_listen_kill_group_num(id);
}

void Logic_Fighter::hook_compelte_scene(int scene_id) {
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(scene_id);
	if (scene_config) {
		if(scene_config->dungeon.first_layer_id != 0){
			task_listen_fb(scene_config->dungeon.first_layer_id);
		}else{
			task_listen_fb(scene_config->dungeon.id);
		}
		if(scene_config->type == FuBen_Scene_Type || scene_config->type == Elite_Scene_Type){
			task_listen_first_pass_scene(scene_id);
		}
		if(scene_config->type == Elite_Scene_Type){
			//精英副本完成，通知奖励找回
			if(scene_config->dungeon.difficulty == 1){
				player_self()->reward_back_listen_fulfill(scene_id);
				if(scene_config->dungeon.is_cost_times()){
					player_self()->reward_back_listen_fulfill(REWARD_BACK_PRO_ELITE_EXPLORE);
				}
				MSG_STREAM("`stream_player_npc`(`type`, `player_id`, `time`,`level`) VALUES (%d,%ld,%ld,%d)",  9,  player_self()->role_id(), Time_Value::gettimeofday().sec(),
						player_self()->level());
				player_self()->task_listen_remote_scene_any_pass();
			}else if(scene_config->dungeon.difficulty == 2){

			}
		}
		if(scene_config->type == Elite_Scene_Type && scene_config->dungeon.difficulty == 2){

		}else{
			task_listen_fb_type(scene_config->type);
		}
	}
}

int Logic_Fighter::fight_basic_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic, int from) {
	if (player_has_init()) {
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		setter.prop_type = pen;
		setter.operate = oen;
		setter.addi_type = aen;
		setter.basic = basic;
		setter_vec.push_back(setter);

		Block_Buffer buf;
		buf.make_message(INNER_CHANGE_FIGHT_PROP);
		buf.write_int32(from);

		uint16_t vec_size = setter_vec.size();
		buf.write_uint16(vec_size);
		for(uint16_t i = 0; i < vec_size; ++i) {
			setter_vec[i].serialize(buf);
		}

		buf.finish_message();
		this->send_to_scene(buf);
	} else {
		fighter_detail_->modify_basic_property(pen, aen, oen, basic);
	}

	return 0;
}

int Logic_Fighter::fight_fixed_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double fixed, int from) {
	if (player_has_init()) {
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		setter.prop_type = pen;
		setter.operate = oen;
		setter.addi_type = aen;
		setter.fixed = fixed;
		setter_vec.push_back(setter);

		Block_Buffer buf;
		buf.make_message(INNER_CHANGE_FIGHT_PROP);
		buf.write_int32(from);

		uint16_t vec_size = setter_vec.size();
		buf.write_uint16(vec_size);
		for(uint16_t i = 0; i < vec_size; ++i) {
			setter_vec[i].serialize(buf);
		}
		buf.finish_message();
		this->send_to_scene(buf);
	} else {
		fighter_detail_->modify_fixed_property(pen, aen, oen, fixed);
	}

	return 0;
}

int Logic_Fighter::fight_percent_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double percent, int from) {
	if (player_has_init()) {
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		setter.prop_type = pen;
		setter.operate = oen;
		setter.addi_type = aen;
		setter.percent = percent;
		setter_vec.push_back(setter);

		Block_Buffer buf;
		buf.make_message(INNER_CHANGE_FIGHT_PROP);
		buf.write_int32(from);

		uint16_t vec_size = setter_vec.size();
		buf.write_uint16(vec_size);
		for(uint16_t i = 0; i < vec_size; ++i) {
			setter_vec[i].serialize(buf);
		}
		buf.finish_message();
		this->send_to_scene(buf);
	} else {
		fighter_detail_->modify_percent_property(pen, aen, oen, percent);
	}

	return 0;
}

int Logic_Fighter::fight_prop_modify(Property_Type pen, Addition_Type aen, Operate_Type oen, double basic, double fixed, double percent, int from) {
	if (player_has_init()) {
		Prop_Setter_Vec setter_vec;
		Prop_Setter setter;
		setter.prop_type = pen;
		setter.operate = oen;
		setter.addi_type = aen;
		setter.basic = basic;
		setter.fixed = fixed;
		setter.percent = percent;
		setter_vec.push_back(setter);

		Block_Buffer buf;
		buf.make_message(INNER_CHANGE_FIGHT_PROP);
		buf.write_int32(from);

		uint16_t vec_size = setter_vec.size();
		buf.write_uint16(vec_size);
		for(uint16_t i = 0; i < vec_size; ++i) {
			setter_vec[i].serialize(buf);
		}
		buf.finish_message();
		this->send_to_scene(buf);
	} else {
		fighter_detail_->modify_fight_property(pen, aen, oen, basic, fixed, percent);
	}

	return 0;
}

int Logic_Fighter::fight_prop_modify(const Prop_Setter_Vec &setter_vec, int from) {
	// modify fighter_detail @ logic
	for(uint16_t i = 0; i < setter_vec.size(); ++i) {
		fighter_detail_->modify_fight_property(setter_vec[i].prop_type, setter_vec[i].addi_type,
				setter_vec[i].operate, setter_vec[i].basic, setter_vec[i].fixed, setter_vec[i].percent);
	}
	// modify fighter_detail @ scene
	if (player_has_init()) {
		Block_Buffer buf;
		buf.make_message(INNER_CHANGE_FIGHT_PROP);
		buf.write_int32(from);

		uint16_t vec_size = setter_vec.size();
		buf.write_uint16(vec_size);
		for(uint16_t i = 0; i < vec_size; ++i) {
			setter_vec[i].serialize(buf);
		}
		buf.finish_message();
		this->send_to_scene(buf);
	}

	return 0;
}

const Exp_All Logic_Fighter::modify_experience_inter(int exp,
		const bool check_vip/* = false*/, const bool check_buff/* = false*/,
		const bool check_vip_team/* = false*/, const bool check_world_level/* = true*/,
		const int double_exp/* = 1*/,
		const int scene_id,
		Operate_Type oen/* = O_ADD*/) {
	Exp_All exp_all;
	exp_all.exp = exp;
	if (exp <= 0) {
		exp_all.result = -1;
		return exp_all;
	}
	if (check_vip) {
		int vip_level = player_self()->vip();
		int vip_rate = CONFIG_CACHE_DUNGEON->get_single_copy_vip_rate(vip_level);
		double vip_exp = exp_all.exp * (double)vip_rate/100;
		exp_all.vip_exp = vip_exp;
	}
	if (check_buff) {
		double prop_exp = exp_all.exp * player_self()->exp_sum(scene_id);
		exp_all.buff_exp = prop_exp;
	}
	if (check_vip_team) {
		int vip_exp = exp_all.exp * (player_self()->get_vip_addition(1) / 100.0);
		exp_all.vip_exp += vip_exp;
	}
	exp = exp_all.exp_sum();
	if (check_world_level) {
		exp *= player_self()->get_world_level_add_percent();
		exp_all.all *= player_self()->get_world_level_add_percent();
	}
	if (double_exp >= 1) {
		exp *= double_exp;
		exp_all.all *= double_exp;
	}
	exp_all.result = modify_experience(exp);
	return exp_all;
}
int Logic_Fighter::modify_experience(int exp, Operate_Type oen, int from) {
	if (exp <= 0) {
		MSG_USER_TRACE("exp:%ld", exp);
		return -1;
	}

	Prop_Setter_Vec setter_vec;
	Prop_Setter setter(PT_EXP_CURRENT, AT_BASIC, oen, exp, 0.0, 0.0);
	setter_vec.push_back(setter);

	Block_Buffer buf;
	buf.make_message(INNER_CHANGE_FIGHT_PROP);
	buf.write_int32(from);

	uint16_t vec_size = setter_vec.size();
	buf.write_uint16(vec_size);
	for(uint16_t i = 0; i < vec_size; ++i) {
		setter_vec[i].serialize(buf);
	}
	buf.finish_message();
	return this->send_to_scene(buf);
}

int Logic_Fighter::addition_prop_refresh(Addition_Type addi_type, Prop_Setter_Type pst, const std::map<int, double> &pro_val_map, int from) {
	/*
	 * 人物属性加成公共接口
	 */
	Prop_Setter setter;
	Prop_Setter_Vec setter_vec;
	bool pst_basic = false;
	bool pst_fixed = false;
	bool pst_percent = false;
	switch (pst) {
	case PST_BASIC:
		pst_basic = true;
		break;
	case PST_FIXED:
		pst_fixed = true;
		break;
	case PST_PERCENT:
		pst_percent = true;
		break;
	default:
		return 0;
	}

	for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
		setter.reset();
		setter.prop_type = static_cast<Property_Type>(*it_prop);
		setter.operate = O_SET;
		setter.addi_type = addi_type;
		setter.basic = pst_basic ? get_map_second_by_key(*it_prop, pro_val_map) : 0.0;
		setter.fixed = pst_fixed  ? get_map_second_by_key(*it_prop, pro_val_map) : 0.0;
		setter.percent = pst_percent  ? get_map_second_by_key(*it_prop, pro_val_map) : 0.0;
		setter_vec.push_back(setter);
	}
	if (! setter_vec.empty()) {
		fight_prop_modify(setter_vec, from);
	}

	return 0;
}

int Logic_Fighter::get_all_learned_skills(int talent_id) {

	if (0 == fighter_detail_) return 0;
	MSG_50100140 msg;
	msg.cur_used_talent = fighter_detail_->cur_used_talent_;
	msg.reset_skill_cost = fighter_detail_->reset_skill_cost();
	msg.ttl_skill_point = fighter_detail_->ttl_skill_points_;

	Skill_Talent_Info talent_info;
	Skill_Info skill_info;

	for (Talent_Map::iterator it = fighter_detail_->talent_map.begin();
			it != fighter_detail_->talent_map.end(); ++it) {
		talent_info.reset();
		talent_info.talent_id = it->second.talent_id;
		talent_info.skill_point = it->second.skill_point;
		for (Skill_DB_Info_Map::iterator skill_it = it->second.skill_db_info_map.begin();
				skill_it != it->second.skill_db_info_map.end(); ++skill_it) {
			skill_info.reset();
			skill_info.skill_id = skill_it->second.skill_id;
			skill_info.skill_lv = skill_it->second.skill_level;
			skill_info.skill_loc = skill_it->second.skill_loc;
			talent_info.skill_info_vec.push_back(skill_info);
		}
		msg.skill_talent_info_vec.push_back(talent_info);
	}

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Logic_Fighter::normal_check(int career, int talent_id, int series, const Skill_Detail& config_detail) {
	//Skill_DB_Info_Map &skill_db_info_map_ = fighter_detail_->skill_db_info_map;
	// 检查等级条件
	if (0 == player_self()) return ERROR_INNER_SERVER;
	if (config_detail.lv_lim > this->level()) {
		return ERROR_ROLE_LV_LIM;
	}
	if (player_self()->awake_lvl() < config_detail.wake_lv) return ERROR_WAKE_LV_LIMIT;
	// 检查职业条件
	if (this->career() != career) {
		return ERROR_CAREER_LIM;
	}
	return 0;
}

int Logic_Fighter::manual_learn_skill(int talent_id, int skill_id, int skill_lv) {
	const Skill_Config *skill_config = CONFIG_CACHE_FIGHTER->skill_config_cache(skill_id);
	if (!skill_config) {
		return ERROR_NO_SUCH_SKILL;
	}
	const Skill_Detail *detail = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lv);
	if (0 == detail) {
		return ERROR_NO_SUCH_SKILL;
	}

	if (0 != detail->goods_cost && 0 != detail->goods_cost_num) {
		Id_Amount item(detail->goods_cost, detail->goods_cost_num);
		int ret = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX, item);
		if (0 != ret) return ret;
	}

	int ret = manual_learn_checker(skill_config->skill_career, talent_id,
			skill_config->skill_series, *detail);
	if (0 != ret) {
		return ret;
	}
	return learn_skill(talent_id, *detail);
}

int Logic_Fighter::manual_learn_checker(int career, int talent_id, int series, const Skill_Detail& config_detail) {
	int ret = normal_check(career, talent_id, series, config_detail);
	if (0 != ret) {
		return ret;
	}

	//检查物品, 暂时不检查
	return 0;
}

int Logic_Fighter::learn_skill(int talent_id, const Skill_Detail &detail) {

	MSG_20100018 msg;
	msg.skill_id = detail.skill_id;
	msg.skill_lv = detail.skill_level;
	msg.page_id = talent_id;
	THIS_SEND_TO_SCENE(msg);
	return 0;
}

int Logic_Fighter::learn_skill_cost_item(int skill_id, int skill_lv) {

	const Skill_Detail *detail_cfg = CONFIG_CACHE_FIGHTER->skill_detail_cache(skill_id, skill_lv);
	if (0 == detail_cfg) {
		MSG_USER("cannot find skill detail config, skill_id=%d, skill_lv=%d", skill_id, skill_lv);
		return 0;
	}
	if (0 == detail_cfg->goods_cost || 0 == detail_cfg->goods_cost_num)
		return 0;
	Id_Amount item(detail_cfg->goods_cost, detail_cfg->goods_cost_num);
	player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, item,
			Lose_Item_DM_Info(ITEM_LOSE_LEARN_SKILL));

	return 0;
}

int Logic_Fighter::erase_skill(int talent_id, int type) {
	// todo 基本判断，主要是消耗品数量的统计和钱的消耗
	Money_Sub_Info sub_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY),
			10, MONEY_SUB_OPEN_TALENT_PAGE));
	int sub_result = player_self()->pack_try_sub_money(sub_info);
	if (sub_result) return sub_result;

	MSG_20100019 msg;
	msg.type =type;
	msg.page_id = talent_id;
	THIS_SEND_TO_SCENE(msg);
	return 0;
}

int Logic_Fighter::move_skill(int skill_id, int new_loc, int talent_id) {
	if (0 == skill_id) return ERROR_SKILL_ID_NULL;
	if (skill_id == this->fighter_detail_->default_skill) return ERROR_DEFAULT_SKILL_NOT_MOVE;
	MSG_20100020 msg;
	msg.skill_id = skill_id;
	msg.new_loc = new_loc;
	msg.page_id = talent_id;
	THIS_SEND_TO_SCENE(msg);
	return 0;
}

//int Logic_Fighter::open_a_skill_talent(int page_id, int talent_id) {
//	// 做常规检查，扣物品之类
//	const Skill_Talent* talent_base = CONFIG_CACHE_FIGHTER->skill_talent(career(), talent_id);
//	if (0 == talent_base) return ERROR_INNER_SERVER;
//	if (talent_base->lv_cond > level()) return ERROR_ROLE_LV_LIM;
//	if ( 0 != talent_base->task_cond && TaskInfo::TASK_STATUS_FINISH != task_state(talent_base->task_cond)) return ERROR_TASK_LIMIT;
//	if (0 < talent_base->goods_id) {
//		int goods_num = this->player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, talent_base->goods_id);
//		if (goods_num < talent_base->goods_cost) return ERROR_GOODS_NUM_LIMIT;
//	}
//	MSG_20100021 msg;
//	msg.page_id = page_id;
//	msg.talent_id = talent_id;
//	THIS_SEND_TO_SCENE(msg);
//	return 0;
//}

int Logic_Fighter::open_a_talent_page(int talent_id) {
	const Skill_Talent* talent_base = CONFIG_CACHE_FIGHTER->skill_talent(career(), talent_id);
	if (0 == talent_base) return ERROR_INNER_SERVER;
	if (talent_base->lv_cond > level()) return ERROR_ROLE_LV_LIM;
//	if (!player_self()->is_finish_last_talent_task(talent_id)) return ERROR_TASK_NOT_FINISH;
	if (0 != talent_base->task_cond && TaskInfo::TASK_STATUS_FINISH != task_state(talent_base->task_cond)) return ERROR_TASK_LIMIT;
	if (0 < talent_base->goods_id) {
		int goods_num = this->player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, talent_base->goods_id);
		if (goods_num < talent_base->goods_cost) return ERROR_GOODS_NUM_LIMIT;
	}
	MSG_20100022 msg;
	msg.page_id = talent_id;
	THIS_SEND_TO_SCENE(msg);
	return 0;
}

//int Logic_Fighter::open_a_talent_page(int page_id) {
//	Money_Sub_Info sub_info(Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST),
//			100, MONEY_SUB_OPEN_TALENT_PAGE));
//	int sub_result = player_self()->pack_try_sub_money(sub_info);
//	if (sub_result) return sub_result;
//
//	MSG_20100022 msg;
//	msg.page_id = page_id;
//	THIS_SEND_TO_SCENE(msg);
//	return 0;
//}

int Logic_Fighter::use_a_talent_page(int page_id) {
	MSG_20100023 msg;
	msg.page_id = page_id;
	THIS_SEND_TO_SCENE(msg);
	return 0;
}

int Logic_Fighter::skill_cost(int type, int arg1, int arg2, int cost) {
	switch(type) {
	case 1:
	{
		if (player_self()->get_covenant_status(RESET_POINT_COVENANT)) break;	//洗点精灵

		Money_Sub_Info sub_info(Money_Sub_Info(Money_Sub_Type(COPPER_ONLY),
				cost, MONEY_SUB_ROLE_ERASE_SKILL));
		int sub_result = player_self()->pack_try_sub_money(sub_info);
		if (sub_result) {
			return sub_result;
		}
		player_self()->pack_sub_money(sub_info);
		break;
	}
	case 2:
	{
		const Skill_Talent* talent_base = CONFIG_CACHE_FIGHTER->skill_talent(career(), arg1);
		if (0 == talent_base) {
			return ERROR_INNER_SERVER;
		}
		if (0 < talent_base->goods_id) {
			int goods_num = this->player_self()->pack_calc_item(Pack::PACK_T_PACKAGE_INDEX, talent_base->goods_id);
			if (goods_num < talent_base->goods_cost) {
				return ERROR_GOODS_NUM_LIMIT;
			}
			if (player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX,
					Id_Amount(talent_base->goods_id, talent_base->goods_cost), Lose_Item_DM_Info(ITEM_LOSE_OPEN_TALENT))) {
				MSG_USER("error erase items, goods_id=%d, num=%d", talent_base->goods_id, talent_base->goods_cost);
			}
		}
		break;
	}
	case 3:
	{
		Money_Sub_Info sub_info(Money_Sub_Info(Money_Sub_Type(BIND_GOLD_FIRST),
				cost, MONEY_SUB_OPEN_TALENT_PAGE));
		int sub_result = player_self()->pack_try_sub_money(sub_info);
		if (sub_result) {
			return sub_result;
		}
		player_self()->pack_sub_money(sub_info);
		break;
	}
	default:
		return 0;
	}
	MSG_20100025 msg;
	msg.type = type;
	msg.arg1 = arg1;
	msg.arg2 = arg2;
	THIS_SEND_TO_SCENE(msg);

	return 0;
}

void Logic_Fighter::sync_phy_power_info(uint16_t cur, int32_t recover_time, uint8_t buy_times){
	fighter_detail_->strength_current.basic.basic_value = cur;
	fighter_detail_->phy_power_recover_time = recover_time;
	fighter_detail_->phy_power_buy_times = buy_times;
	fighter_detail_->detail_change();
}

int Logic_Fighter::restore_full_prop(int32_t type, int32_t per) {
	Block_Buffer buf;
	buf.make_message(INNER_FIGHTER_RESTORE_FULL_PROP);
	buf.write_int32(type);
	buf.write_int32(per);
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

int Logic_Fighter::add_be_skills(const Skill_Info_Vec &skill_infos) {
	if (skill_infos.empty()) {
		return -1;
	}
	Block_Buffer buf;
	buf.make_message(INNER_NOTIFY_SCENE_PASSIVE);
	buf.write_int8(NPT_ADD);
	uint16_t vec_size = skill_infos.size();
	buf.write_uint16(vec_size);
	for (uint8_t i = 0; i < skill_infos.size(); ++i) {
		skill_infos[i].serialize(buf);
	}
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

int Logic_Fighter::remove_be_skills(const std::vector<Skill_Info> &skill_infos) {
	if (skill_infos.empty()) {
		return -1;
	}
	Block_Buffer buf;
	buf.make_message(INNER_NOTIFY_SCENE_PASSIVE);
	buf.write_int8(NPT_REMOVE);
	uint16_t vec_size = skill_infos.size();
	buf.write_uint16(vec_size);
	for (std::vector<Skill_Info>::const_iterator it = skill_infos.begin(); it != skill_infos.end(); ++it) {
		it->serialize(buf);
	}
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

void Logic_Fighter::init_be_skill_extra(int skill_id, int skill_lv) {
	Skill_DB_Info skill;
	skill.skill_id = skill_id;
	skill.skill_level = skill_lv;
	fighter_detail_->extra_skill_vec.push_back(skill);
}

//int Logic_Fighter::module_init_add_be_skills(const Skill_Info_Vec &skill_infos) {
//	if (skill_infos.empty()) return 0;
//	if (0 == fighter_detail_) return 0;
//	for (Skill_Info_Vec::const_iterator it = skill_infos.begin(); it != skill_infos.end(); ++it) {
////		Skill_DB_Info_Map::iterator find_it = fighter_detail_->extra_skill_map.find(it->skill_id);
////		if (find_it == fighter_detail_->extra_skill_map.end()) {
////			Skill_DB_Info info;
////			info.skill_id = it->skill_id;
////			info.skill_level = it->skill_lv;
////			info.skill_loc = it->skill_loc;
////			fighter_detail_->extra_skill_map.insert(std::make_pair(it->skill_id, info));
////		} else  if (find_it->second.skill_level < it->skill_lv) {
////			find_it->second.skill_level = it->skill_lv;
////		}
//		Skill_DB_Info info;
//		info.skill_id = it->skill_id;
//		info.skill_level = it->skill_lv;
//		fighter_detail_->extra_skill_vec.push_back(info);
//	}
//	return 0;
//}

int Logic_Fighter::add_skill_pointer(const int skill_points) {
	if (skill_points <= 0) {
		return -1;
	}
	Block_Buffer buf;
	buf.make_message(INNER_LOGIC_ADD_SKILL_POINT);
	buf.write_int32(skill_points);
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

void Logic_Fighter::set_phy_power_recover_time(int time){
	if(fighter_detail_){
		fighter_detail_->phy_power_recover_time = time;
		fighter_detail_->detail_change();
	}
}

void Logic_Fighter::set_phy_power(double value){
	if(fighter_detail_){
		fighter_detail_->strength_current.basic.basic_value = value;
		fighter_detail_->detail_change();
	}
}

void Logic_Fighter::sync_phy_power_to_client(void){
	MSG_81000063 msg;
	msg.phy_power_buy_times = fighter_detail_->phy_power_buy_times;
	msg.phy_power_cur = fighter_detail_->current_strength();
	msg.phy_power_max = fighter_detail_->max_strength();
	int remain_time = 0;
	if(fighter_detail_->phy_power_recover_time > 0){
		remain_time = fighter_detail_->phy_power_recover_time - Time_Value::gettimeofday().sec();
	}
	msg.phy_power_recover_time = remain_time;
	THIS_SEND_TO_CLIENT(msg);
}

int Logic_Fighter::cost_strength(int value, bool to_scene/* = true*/){
	if(!fighter_detail_){
		return -1;
	}
	if(value <= 0){
		return -1;
	}
	int cur_str = fighter_detail_->current_strength();
	if(value > cur_str){
		return -1;
	}
	double new_str = cur_str - value;
	fighter_detail_->strength_current.basic.basic_value = new_str;
//	if(fighter_detail_->phy_power_recover_time == 0){
//		Time_Value next_recover_time = Time_Value::gettimeofday() + Time_Value(PHY_POWER_RECOVER_TIME_VALUE);
//		fighter_detail_->phy_power_recover_time = next_recover_time.sec();
//	}
	sign_current_strength_ -= value;
	if(sign_current_strength_ < 0){
		sign_current_strength_ = 0;
	}
	fighter_detail_->detail_change();
	if(to_scene){
		MSG_20100030 msg;
		msg.phy_power_buy_times = fighter_detail_->phy_power_buy_times;
		msg.phy_power_cur = fighter_detail_->current_strength();
		msg.phy_power_recover_time = fighter_detail_->phy_power_recover_time;
		THIS_SEND_TO_SCENE(msg);
	}
	return 0;
}

void Logic_Fighter::update_force_by_sync_data(int force) {
	int force_before = fighter_detail_->force + player_self()->get_fight_hero_force();

	fighter_detail_->force = force;
	fighter_detail_->detail_change();

	player_self()->update_ttl_force();

	int force_now = fighter_detail_->force + player_self()->get_fight_hero_force();
	if (force_before > force_now) {
		MSG_DEBUG("force change role:%ld, %d   %d   %d", role_id(), force, force_before, force_now);
	}

}

int Logic_Fighter::get_player_force(void) {
	return fighter_detail_->force + player_self()->get_fight_hero_force();
}

int Logic_Fighter::get_ttl_force(void){
	if(fighter_detail_){
		return fighter_detail_->ttl_force;
	}
	return 0;
}

int Logic_Fighter::get_highest_force(void){
	if(fighter_detail_){
		if(fighter_detail_->highest_force <= fighter_detail_->ttl_force){
			return fighter_detail_->ttl_force;
		}
		return fighter_detail_->highest_force;
	}
	return 0;
}
