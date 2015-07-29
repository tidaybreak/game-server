/*
 * Status_Struct.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: ti
 *
 *
 */

#include "Err_Code.h"
#include "Config_Cache_Buff.h"
#include "Pool_Manager.h"
#include "Status_Func.h"
#include "Status_Struct.h"

void Effect_Argv_Prop::reset(void) {
	this->effect_value	= 0.0;
	this->i_effect_val1 = 0;
	this->i_effect_val2 = 0;
	this->i_effect_val3 = 0;
	this->d_effect_val1 = 0.0;
	this->d_effect_val2 = 0.0;
	this->d_effect_val3 = 0.0;
	this->d_effect_val4 = 0.0;
	this->d_effect_val5 = 0.0;
	this->d_effect_val6 = 0.0;
	this->d_effect_val7 = 0.0;
	this->d_effect_val8 = 0.0;
	this->d_effect_val9 = 0.0;
}

void Effect_Argv_Prop::serialize(Block_Buffer &buf) const {
	buf.write_double(effect_value);
	buf.write_int32(i_effect_val1);
	buf.write_int32(i_effect_val2);
	buf.write_int32(i_effect_val3);
	buf.write_double(d_effect_val1);
	buf.write_double(d_effect_val2);
	buf.write_double(d_effect_val3);
	buf.write_double(d_effect_val4);
	buf.write_double(d_effect_val5);
	buf.write_double(d_effect_val6);
	buf.write_double(d_effect_val7);
	buf.write_double(d_effect_val8);
	buf.write_double(d_effect_val9);
}

void Effect_Argv_Prop::deserialize(Block_Buffer &buf) {
	buf.read_double(effect_value);
	buf.read_int32(i_effect_val1);
	buf.read_int32(i_effect_val2);
	buf.read_int32(i_effect_val3);
	buf.read_double(d_effect_val1);
	buf.read_double(d_effect_val2);
	buf.read_double(d_effect_val3);
	buf.read_double(d_effect_val4);
	buf.read_double(d_effect_val5);
	buf.read_double(d_effect_val6);
	buf.read_double(d_effect_val7);
	buf.read_double(d_effect_val8);
	buf.read_double(d_effect_val9);
}

void Status_Effect_Argv::reset(void) {
	status_id = 0;
	last_time_ms = 0;
	cfg_action_gap = 0;
	cfg_trigger_gap = 0;
	status_from = 0;
	caster_id = 0;
	overlay = 0;
	disappear_gap.clear();
	CLEAR_VECTOR(Effect_Argv_Prop_Vec, data);
	is_fight_buff = false;
}

void Status_Effect_Argv::dispath(void) {
	const Btl_Buff_Cfg *buff_config = CONFIG_CACHE_BUFF->btl_buff(this->status_id);
	if (NULL == buff_config
			|| buff_config->buff_disappear_vec.size() != this->disappear_gap.size()) {
		MSG_USER("ERROR: %d, buff_config->buff_disappear_vec.size() : this->disappear_gap.size() = %d : %d",
				this->status_id, buff_config->buff_disappear_vec.size(), this->disappear_gap.size());
		return ;
	}
	uint32_t i = 0;
	for (Int_Vec::const_iterator it = this->disappear_gap.begin();
			it != this->disappear_gap.end(); ++it) {
		switch(buff_config->buff_disappear_vec[i]) {
		case STATUS_TYPE_TIME:
			this->last_time_ms = *it;
			break;
		case STATUS_TYPE_SHOT:
			this->cfg_action_gap = *it;
			break;
		case STATUS_TYPE_PASSIVE:
			this->cfg_trigger_gap = *it;
			break;
		default:
			break;
		}
		++i;
	}
}

bool Status_Effect_Argv::check_last_time(void) {
	const Btl_Buff_Cfg *buff_config = CONFIG_CACHE_BUFF->btl_buff(this->status_id);
	if (NULL == buff_config) {
		MSG_USER("ERROR: %d", this->status_id);
		return false;
	}
	for (Int_Vec::const_iterator it = buff_config->buff_disappear_vec.begin();
			it != buff_config->buff_disappear_vec.end(); ++it) {
		switch(*it) {
		case STATUS_TYPE_TIME:
			if (this->last_time_ms < 0 && this->last_time_ms != -1) {
				return false;
			}
			break;
//		case STATUS_TYPE_SHOT:
//			this->cfg_action_gap = *it;
//			break;
//		case STATUS_TYPE_PASSIVE:
//			this->cfg_trigger_gap = *it;
//			break;
		default:
			break;
		}
	}
	return true;
}

int Status_Effect_Argv::add_last_time_ms(const int &last_ms) {
	const Btl_Buff_Cfg *buff_config = CONFIG_CACHE_BUFF->btl_buff(this->status_id);
	if (NULL == buff_config
			|| buff_config->buff_disappear_vec.size() != this->disappear_gap.size()) {
		MSG_USER("ERROR: %d, buff_config->buff_disappear_vec.size() : this->disappear_gap.size() = %d : %d",
				this->status_id, buff_config->buff_disappear_vec.size(), this->disappear_gap.size());
		return -1;
	}
	uint32_t i = 0;
	for (Int_Vec::iterator it = this->disappear_gap.begin();
			it != this->disappear_gap.end(); ++it) {
		switch(buff_config->buff_disappear_vec[i]) {
		case STATUS_TYPE_TIME:
			(*it) += last_ms;
			break;
//		case STATUS_TYPE_SHOT:
//			(*it) += last_ms;
//			break;
//		case STATUS_TYPE_PASSIVE:
//			(*it) += last_ms;
//			break;
		default:
			break;
		}
		++i;
	}
	return 0;
}

void Status_Effect_Argv::serialize(Block_Buffer &buf) const {
	buf.write_int32(status_id);
	buf.write_int64(last_time_ms);
	buf.write_int32(cfg_action_gap);
	buf.write_int32(cfg_trigger_gap);
	buf.write_int32(status_from);
	buf.write_int64(caster_id);
	buf.write_int32(overlay);
	buf.write_uint16(disappear_gap.size());
	for (Int_Vec::const_iterator it = disappear_gap.begin(); it != disappear_gap.end(); ++it) {
		buf.write_int32(*it);
	}
	buf.write_uint16(data.size());
	for (Effect_Argv_Prop_Vec::const_iterator it = data.begin(); it != data.end(); ++it) {
		(*it).serialize(buf);
	}
	buf.write_bool(is_fight_buff);
}

void Status_Effect_Argv::deserialize(Block_Buffer &buf) {
	buf.read_int32(status_id);
	buf.read_int64(last_time_ms);
	buf.read_int32(cfg_action_gap);
	buf.read_int32(cfg_trigger_gap);
	buf.read_int32(status_from);
	buf.read_int64(caster_id);
	buf.read_int32(overlay);
	uint16_t len = 0;
	int32_t tmp = 0;
	buf.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int32(tmp);
		disappear_gap.push_back(tmp);
	}
	buf.read_uint16(len);
	Effect_Argv_Prop prop;
	for (uint16_t i = 0; i < len; ++i) {
		prop.reset();
		prop.deserialize(buf);
		data.push_back(prop);
	}
	buf.read_bool(is_fight_buff);
}

int Status_Effect_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(effect_id);
	buffer.write_double(effect_value);
	buffer.write_int32(i_effect_value1);
	buffer.write_int32(i_effect_value2);
	buffer.write_int32(i_effect_value3);
	buffer.write_double(d_effect_value1);
	buffer.write_double(d_effect_value2);
	buffer.write_double(d_effect_value3);
	buffer.write_double(d_effect_value4);
	buffer.write_double(d_effect_value5);
	buffer.write_double(d_effect_value6);
	buffer.write_double(d_effect_value7);
	buffer.write_double(d_effect_value8);
	buffer.write_double(d_effect_value9);

	buffer.write_int32(effect_type);
	buffer.write_int32(status_id);
	buffer.write_uint16(heat_type.size());
	for (Int_Hash_Set::iterator it = heat_type.begin(); it != heat_type.end(); ++it) {
		buffer.write_int32(*it);
	}
	buffer.write_int32(effect_times);
	buffer.write_int32(overlay);
	buffer.write_bool(is_overlay);
	buffer.write_bool(is_hit);
	buffer.write_int32(skill_id);

	trigger_buff.serialize(buffer);
	buffer.write_double(buff_chance);
	buffer.write_int64(caster_id);

	buffer.write_int32(action_times);
	buffer.write_int32(action_tick);
	buffer.write_int32(passive_times);
	buffer.write_int32(passive_tick);
	buffer.write_int64(caster_id);
	next_opt.serialize(buffer);
	interval.serialize(buffer);

	buffer.write_bool(is_delay);
	buffer.write_int32(delay_times);
	delay_time_ms.serialize(buffer);
	buffer.write_int32(delay_action_times);
	buffer.write_int32(delay_passive_times);
	delay_time_tick.serialize(buffer);
	buffer.write_int32(delay_action_tick);
	buffer.write_int32(delay_passive_tick);

	buffer.write_double(last_cure_value);
	buffer.write_double(last_cure_rate);

	buffer.write_uint16(buff_scene.size());
	for (Int_Vec::const_iterator it = buff_scene.begin(); it != buff_scene.end(); ++it) {
		buffer.write_int32(*it);
	}
	return 0;
}

int Status_Effect_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(effect_id);
	buffer.read_double(effect_value);
	buffer.read_int32(i_effect_value1);
	buffer.read_int32(i_effect_value2);
	buffer.read_int32(i_effect_value3);
	buffer.read_double(d_effect_value1);
	buffer.read_double(d_effect_value2);
	buffer.read_double(d_effect_value3);
	buffer.read_double(d_effect_value4);
	buffer.read_double(d_effect_value5);
	buffer.read_double(d_effect_value6);
	buffer.read_double(d_effect_value7);
	buffer.read_double(d_effect_value8);
	buffer.read_double(d_effect_value9);

	buffer.read_int32(effect_type);
	buffer.read_int32(status_id);
	uint16_t len = 0;
	int32_t tmp = 0;
	buffer.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(tmp);
		heat_type.insert(tmp);
	}
	buffer.read_int32(effect_times);
	buffer.read_int32(overlay);
	buffer.read_bool(is_overlay);
	buffer.read_bool(is_hit);
	buffer.read_int32(skill_id);

	trigger_buff.deserialize(buffer);
	buffer.read_double(buff_chance);
	buffer.read_int64(caster_id);

	buffer.read_int32(action_times);
	buffer.read_int32(action_tick);
	buffer.read_int32(passive_times);
	buffer.read_int32(passive_tick);
	buffer.read_int64(caster_id);
	next_opt.deserialize(buffer);
	interval.deserialize(buffer);

	buffer.read_bool(is_delay);
	buffer.read_int32(delay_times);
	delay_time_ms.deserialize(buffer);
	buffer.read_int32(delay_action_times);
	buffer.read_int32(delay_passive_times);
	delay_time_tick.deserialize(buffer);
	buffer.read_int32(delay_action_tick);
	buffer.read_int32(delay_passive_tick);

	buffer.read_double(last_cure_value);
	buffer.read_double(last_cure_rate);

	buffer.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buffer.read_int32(tmp);
		buff_scene.push_back(tmp);
	}

	return 0;
}

void Status_Effect_Detail::reset(void) {
	effect_id = 0;
	effect_value = 0;
	i_effect_value1 = 0;
	i_effect_value2 = 0;
	i_effect_value3 = 0;
	d_effect_value1 = 0.0;
	d_effect_value2 = 0.0;
	d_effect_value3 = 0.0;
	d_effect_value4 = 0.0;
	d_effect_value5 = 0.0;
	d_effect_value6 = 0.0;
	d_effect_value7 = 0.0;
	d_effect_value8 = 0.0;
	d_effect_value9 = 0.0;
//	fighter_prop_modify.clear();

	effect_type = 0;
	status_id = 0;
	heat_type.clear();
	effect_times = 0;
	overlay = 0;
	is_overlay = false;
	is_hit = false;
	skill_id = 0;
	trigger_buff.reset();
	buff_chance = 0;

	action_times = 0;
	action_tick = 0;
	passive_times = 0;
	passive_tick = 0;
	next_opt = Time_Value::zero;
	interval = Time_Value::zero;

	is_delay = false;
	delay_times = 0;
	delay_time_ms = Time_Value::zero;
	delay_action_times = 0;
	delay_passive_times = 0;
	delay_time_tick = Time_Value::zero;
	delay_action_tick = 0;
	delay_passive_tick = 0;

	last_cure_value = 0.0;
	last_cure_rate = 0.0;

	buff_scene.clear();
}

void Status_Effect_Detail::set_time(int ms_interval) {
	if (ms_interval < 0) {
//		MSG_USER("ms_interval < 0");
		ms_interval = 0;
	}

	if (ms_interval > 0) {
		interval.set(ms_interval / 1000, ms_interval % 1000 * 1000);
		next_opt = Time_Value::gettimeofday() + interval;
	} else {
		interval = Time_Value::zero;
		next_opt = Time_Value::max;
	}
}

void Status_Effect_Detail::set_delay_time(int ms_interval) {
	if (ms_interval < 0) {
		MSG_USER("ms_interval < 0");
		ms_interval = 0;
	}

	if (ms_interval > 0) {
		delay_time_tick.set(ms_interval / 1000, ms_interval % 1000 * 1000);
		delay_time_ms = Time_Value::gettimeofday() + delay_time_tick;
	} else {
		delay_time_tick = Time_Value::zero;
		delay_time_ms = Time_Value::zero;
	}
}

int Status_Effect_Detail::ms_interval(void) const {
	int ms_interval = interval.sec() * 1000;
	ms_interval += interval.usec() / 1000;
	return ms_interval;
}

int Status_Effect_Detail::delay_ms_interval(void) const {
	int ms_interval = delay_time_tick.sec() * 1000;
	ms_interval += delay_time_tick.usec() / 1000;
	return ms_interval;
}

int Status_Info_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(status_id);
	buffer.write_int32(status_from);
	buffer.write_int64(caster_role_id);
	buffer.write_int64(caster_master_id);
	buffer.write_int32(overlay);

	buffer.write_uint16(effect_map.size());
	for (Effect_Vec::const_iterator it = effect_map.begin(); it != effect_map.end(); ++it) {
		(*it).serialize(buffer);
	}

	return 0;
}

int Status_Info_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(status_id);
	buffer.read_int32(status_from);
	buffer.read_int64(caster_role_id);
	buffer.read_int64(caster_master_id);
	buffer.read_int32(overlay);

	uint16_t size = 0;
	buffer.read_uint16(size);
	Status_Effect_Detail status_effect;
	for (uint16_t i = 0; i < size; ++i) {
		status_effect.reset();
		status_effect.deserialize(buffer);
		effect_map.push_back(status_effect);
	}

	return 0;
}

void Status_Info_Detail::reset(void) {
	for (Effect_Vec::iterator it = effect_map.begin(); it != effect_map.end(); ++it) {
		(*it).reset();
//		POOL_MANAGER->push_status_effect_pool(it->second);
	}
	CLEAR_VECTOR(Effect_Vec, effect_map);

	status_id = 0;
}

int Status_Info_Detail::set(int status_id_, role_id_t caster_role_id_, int status_from_, role_id_t caster_master_id_, int overlay_) {
	status_id 			= status_id_;
	caster_role_id		= caster_role_id_;
	caster_master_id 	= caster_master_id_ ? caster_master_id_ : caster_role_id;
	status_from			= status_from_;
	overlay				= overlay_;

	const Btl_Buff_Cfg *cache = CONFIG_CACHE_BUFF->btl_buff(status_id_);
	if (NULL == cache) {
		MSG_USER("status %d configure file error.", status_id_);
		return ERROR_CONFIG_NOT_EXIST;
	}

	return 0;
}

Status_Info::Status_Info(void) : status_id_(0), overlay_(0),
		action_times(0), trigger_times(0), cfg_action_times(0), cfg_trigger_times(0),
		last_status_from_(0), last_caster_role_id_(0), last_caster_master_id_(0),
		is_fight_buff(false), last_time_ms(0), other_id(0) {
	this->reset();
}

Status_Info::~Status_Info(void) {
}

void Status_Info::serialize(Block_Buffer &buf) const {
	buf.write_int32(status_id_);
	buf.write_int32(overlay_);
	buf.write_int32(action_times);
	buf.write_int32(trigger_times);
	buf.write_int32(cfg_action_times);
	buf.write_int32(cfg_trigger_times);
	buf.write_int32(last_status_from_);
	buf.write_int64(last_caster_role_id_);
	buf.write_int64(last_caster_master_id_);
	buf.write_bool(is_fight_buff);

	buf.write_uint16(buff_scene.size());
	for (Int_Vec::const_iterator it = buff_scene.begin(); it != buff_scene.end(); ++it) {
		buf.write_int32(*it);
	}

	buf.write_uint16(scene_visible.size());
	for (Int_Vec::const_iterator it = scene_visible.begin(); it != scene_visible.end(); ++it) {
		buf.write_int32(*it);
	}

	buf.write_int64(last_time_ms);
	last_start_time.serialize(buf);
	last_end_time.serialize(buf);

	buf.write_uint16(status_fifo.size());
	for (std::deque<role_id_t>::const_iterator it = status_fifo.begin(); it != status_fifo.end(); ++it) {
		buf.write_int64(*it);
	}
	buf.write_uint16(status_info_map.size());
	for (Status_Info_Map::const_iterator it = status_info_map.begin(); it != status_info_map.end(); ++it) {
		buf.write_int64(it->first);
		it->second.serialize(buf);
	}

	buf.write_int32(other_id);
}

void Status_Info::deserialize(Block_Buffer &buf) {
	buf.read_int32(status_id_);
	buf.read_int32(overlay_);
	buf.read_int32(action_times);
	buf.read_int32(trigger_times);
	buf.read_int32(cfg_action_times);
	buf.read_int32(cfg_trigger_times);
	buf.read_int32(last_status_from_);
	buf.read_int64(last_caster_role_id_);
	buf.read_int64(last_caster_master_id_);
	buf.read_bool(is_fight_buff);

	uint16_t len = 0;
	int32_t val = 0;
	buf.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int32(val);
		buff_scene.push_back(val);
	}
	buf.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int32(val);
		scene_visible.push_back(val);
	}

	buf.read_int64(last_time_ms);
	last_start_time.deserialize(buf);
	last_end_time.deserialize(buf);

	role_id_t role_id = 0;
	buf.read_uint16(len);
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int64(role_id);
		status_fifo.push_back(role_id);
	}
	buf.read_uint16(len);
	Status_Info_Detail status_info_detail;
	for (uint16_t i = 0; i < len; ++i) {
		buf.read_int64(role_id);
		status_info_detail.reset();
		status_info_detail.deserialize(buf);
		status_info_map.insert(std::make_pair(role_id, status_info_detail));
	}

	buf.read_int32(other_id);
}

void Status_Info::reset(void) {
	status_id_ = 0;
	overlay_ = 0;
	action_times = 0;
	trigger_times = 0;
	cfg_action_times = 0;
	cfg_trigger_times = 0;
	status_info_map.clear();
	status_fifo.clear();
	last_status_from_ = 0;
	last_caster_role_id_ = 0;
	last_caster_master_id_ = 0;
	is_fight_buff = false;
	buff_scene.clear();
	scene_visible.clear();

	last_time_ms = 0;
	last_start_time = Time_Value::zero;
	last_end_time = Time_Value::zero;

	other_id = 0;
}

int Status_Info::set(int status_id, role_id_t caster_role_id, int status_from, role_id_t caster_master_id) {
	this->status_id_ = status_id;
	this->last_caster_master_id_ = caster_master_id;
	this->last_caster_role_id_ = caster_role_id;
	this->last_status_from_ = status_from;
	return 0;
}

int Status_Running_Effect::serialize(Block_Buffer &buffer) const {
	buffer.write_uint16(hurt_link_map.size());
	for (Int_Int_Map::const_iterator it = hurt_link_map.begin(); it != hurt_link_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(cure_link_map.size());
	for (Int_Int_Map::const_iterator it = cure_link_map.begin(); it != cure_link_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(stun_map.size());
	for (Int_Int_Map::const_iterator it = stun_map.begin(); it != stun_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(stoic_map.size());
	for (Int_Int_Map::const_iterator it = stoic_map.begin(); it != stoic_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(taunt_map.size());
	for (Int_Int_Map::const_iterator it = taunt_map.begin(); it != taunt_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(lock_map.size());
	for (Int_Int_Map::const_iterator it = lock_map.begin(); it != lock_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(invincible_map.size());
	for (Int_Int_Map::const_iterator it = invincible_map.begin(); it != invincible_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(silent_map.size());
	for (Int_Int_Map::const_iterator it = silent_map.begin(); it != silent_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(confusion_map.size());
	for (Int_Int_Map::const_iterator it = confusion_map.begin(); it != confusion_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(exp_map.size());
	for (Int_Double_Map::const_iterator it = exp_map.begin(); it != exp_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_double(it->second);
	}

	buffer.write_uint16(shape_sheep_map.size());
	for (Int_Int_Map::const_iterator it = shape_sheep_map.begin(); it != shape_sheep_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(rebound_map.size());
	for (Int_Double_Map::const_iterator it = rebound_map.begin(); it != rebound_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_double(it->second);
	}

	buffer.write_uint16(change_fashion_map.size());
	for (Int_Int_Map::const_iterator it = change_fashion_map.begin(); it != change_fashion_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_int32(it->second);
	}

	buffer.write_uint16(exp_two_map.size());
	for (Int_Double_Map::const_iterator it = exp_two_map.begin(); it != exp_two_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_double(it->second);
	}

	buffer.write_uint16(exp_activity_map.size());
	for (Int_Double_Map::const_iterator it = exp_activity_map.begin(); it != exp_activity_map.end(); ++it) {
		buffer.write_int32(it->first);
		buffer.write_double(it->second);
	}
	return 0;
}

int Status_Running_Effect::deserialize(Block_Buffer &buffer) {
	uint16_t size = 0;
	int32_t key = 0, val = 0;

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		hurt_link_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		cure_link_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		stun_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		stoic_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		taunt_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		lock_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		invincible_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		silent_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		confusion_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	double double_val = 0.0;
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_double(double_val);
		exp_map.insert(std::make_pair(key, double_val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		shape_sheep_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_double(double_val);
		rebound_map.insert(std::make_pair(key, double_val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_int32(val);
		change_fashion_map.insert(std::make_pair(key, val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_double(double_val);
		exp_two_map.insert(std::make_pair(key, double_val));
	}

	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		buffer.read_int32(key);
		buffer.read_double(double_val);
		exp_activity_map.insert(std::make_pair(key, double_val));
	}

	return 0;
}

void Status_Running_Effect::reset(void) {
	hurt_link_map.clear();
	cure_link_map.clear();
	stun_map.clear();
	stoic_map.clear();
	taunt_map.clear();
	lock_map.clear();
	invincible_map.clear();
	silent_map.clear();
	confusion_map.clear();
	exp_map.clear();
	shape_sheep_map.clear();
	rebound_map.clear();
	change_fashion_map.clear();
	exp_two_map.clear();
	exp_activity_map.clear();
}

Status_Detail::Status_Detail(void) : status_map(STATUS_BUCKET_SIZE) {
	reset();
}

int Status_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);

	buffer.write_uint16(status_map.size());
	for (Status_Map::const_iterator it = status_map.begin(); it != status_map.end(); ++it) {
		it->second->serialize(buffer);
	}

	running_effect.serialize(buffer);

	return 0;
}

int Status_Detail::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);

	uint16_t size = 0;
	buffer.read_uint16(size);
	for (uint16_t i = 0; i < size; ++i) {
		Status_Info* status_info = POOL_MANAGER->pop_status_info_pool();
		status_info->deserialize(buffer);
		status_map.insert(std::make_pair(status_info->status_id_, status_info));
	}

	running_effect.deserialize(buffer);

	return 0;
}

void Status_Detail::reset(void) {
	role_id = 0;
	for (Status_Map::iterator it = status_map.begin(); it != status_map.end(); ++it) {
		POOL_MANAGER->push_status_info_pool(it->second);
	}
	status_map.clear();
	status_dispel_set.clear();
	status_pure_set.clear();
	status_stoic_set.clear();
	status_shield_set.clear();
	status_evil_set.clear();

	shield_cfg = 0.0;
	shield_hurt = 0.0;
	hurt_link.clear();
	cure_link.clear();

	status_timepri_list.clear();
	status_action_list.clear();
	status_passive_list.clear();
	out_status_timepri_list.clear();
	out_status_action_list.clear();
	out_status_passive_list.clear();
	timepri_list.clear();
	action_list.clear();
	passive_list.clear();
	once_timepri_list.clear();

	running_effect.reset();
//	speed_resis_stop_tv = Time_Value::zero;
//	silence_resis_stop_tv = Time_Value::zero;
//	stun_resis_stop_tv = Time_Value::zero;
}
