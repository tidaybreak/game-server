/*
 * Status_Func.h
 *
 *  Created on: Mar 24, 2014
 *      Author: ti
 */

#ifndef STATUS_FUNC_H_
#define STATUS_FUNC_H_

#include "Status_Struct.h"
#include "Config_Cache_Buff.h"

struct Fighter;
struct Status;
struct Status_Info_Msg;

inline int get_status_type(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return 0;
	} else {
		return status_config->buff_id;
	}
}

inline bool check_int_in_vec(const int val, const Int_Vec vec) {
	for (Int_Vec::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		if (*it == val) {
			return true;
		}
	}
	return false;
}

inline bool is_status_prior(const Status_Effect_Detail &status_info1, const Status_Effect_Detail &status_info2) {
	if (status_info1.next_opt > status_info2.next_opt) {
		return false;
	} else {
		return true;
	}
}

inline int insert_status_and_sort(Status_Info *status_info, Status_Detail::Status_Info_List &status_list) {
	if (!status_info || -1 == status_info->last_time_ms) {
		return 0;// 永久buff
	}
	Status_Detail::Status_Info_List::iterator iter = status_list.begin();
	for (; iter != status_list.end(); ++iter) {
		if (status_info->last_end_time < (**iter).last_end_time) {
			status_list.insert(iter, status_info);
			break;
		}
	}

	if (iter == status_list.end()) {
		status_list.push_back(status_info);
	}

	return 0;
}

inline int remove_status_and_sort(Status_Info *status_info, Status_Detail::Status_Info_List &status_list) {
	if (!status_info || -1 == status_info->last_time_ms) {
		return 0;// 永久buff
	}
	status_list.remove(status_info);

	return 0;
}

inline int replace_status_and_sort(Status_Info *status_info, Status_Detail::Status_Info_List &status_list) {
	if (!status_info || -1 == status_info->last_time_ms) {
		return 0;// 永久buff
	}
	status_list.remove(status_info);
	Status_Detail::Status_Info_List::iterator iter = status_list.begin();
	for (; iter != status_list.end(); ++iter) {
		if (status_info->last_end_time < (**iter).last_end_time) {
			status_list.insert(iter, status_info);
			break;
		}
	}

	if (iter == status_list.end()) {
		status_list.push_back(status_info);
	}

	return 0;
}

inline int insert_status_and_sort_timepri(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	if (status_info
			&& !status_info->heat_type.empty()
			&& status_info->heat_type.count(EFFECT_TYPE_TIME)
			&& status_info->interval != Time_Value::zero) { // interval 为0，表示永久生效且只生效一次
		Status_Detail::Status_Effect_List::iterator iter = timepri_list.begin();
		for (; iter != timepri_list.end(); ++iter) {
			if (is_status_prior(*status_info, **iter)) {
				timepri_list.insert(iter, status_info);
				break;
			}
		}
		if (iter == timepri_list.end()) {
			timepri_list.push_back(status_info);
		}
	}

	return 0;
}

inline int insert_status_and_sort_timepri(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		insert_status_and_sort_timepri(&(*it), timepri_list);
	}

	return 0;
}

inline int remove_status_and_sort_timepri(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	if (status_info->heat_type.count(EFFECT_TYPE_TIME) && status_info->interval != Time_Value::zero) {
		timepri_list.remove(status_info);
	}

	return 0;
}

inline int remove_status_and_sort_timepri(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		remove_status_and_sort_timepri(&(*it), timepri_list);
	}

	return 0;
}

inline int insert_status_and_sort_once_timepri(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	if (status_info->heat_type.count(EFFECT_TYPE_TIME) && status_info->interval == Time_Value::zero
			&& Time_Value::zero != status_info->delay_time_tick) { // interval 为0，表示永久生效且只生效一次
		status_info->delay_time_ms = status_info->delay_time_tick + Time_Value::gettimeofday();
		Status_Detail::Status_Effect_List::iterator iter = timepri_list.begin();
		for (; iter != timepri_list.end(); ++iter) {
			if (status_info->delay_time_ms <= (**iter).delay_time_ms) {
				timepri_list.insert(iter, status_info);
				break;
			}
		}
		if (iter == timepri_list.end()) {
			timepri_list.push_back(status_info);
		}
	}

	return 0;
}

inline int insert_status_and_sort_once_timepri(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		insert_status_and_sort_once_timepri(&(*it), timepri_list);
	}

	return 0;
}

inline int remove_status_and_sort_once_timepri(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	if (status_info->heat_type.count(EFFECT_TYPE_TIME) && status_info->interval == Time_Value::zero
				&& Time_Value::zero != status_info->delay_time_tick) {
		timepri_list.remove(status_info);
	}

	return 0;
}

inline int remove_status_and_sort_once_timepri(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &timepri_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		remove_status_and_sort_once_timepri(&(*it), timepri_list);
	}

	return 0;
}

inline int insert_status_and_sort_action(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	if (status_info
			&& !status_info->heat_type.empty()
			&& status_info->heat_type.count(EFFECT_TYPE_SHOT)
			&& status_info->action_tick != -1) {
		action_list.push_back(status_info);
	}

	return 0;
}

inline int insert_status_and_sort_action(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		insert_status_and_sort_action(&(*it), action_list);
	}

	return 0;
}

inline int remove_status_and_sort_action(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	if (status_info->heat_type.count(EFFECT_TYPE_SHOT)) {
		action_list.remove(status_info);
	}

	return 0;
}

inline int remove_status_and_sort_action(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		remove_status_and_sort_action(&(*it), action_list);
	}

	return 0;
}

inline int insert_status_and_sort_passive(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	if (status_info
			&& !status_info->heat_type.empty()
			&& status_info->heat_type.count(EFFECT_TYPE_PASSIVE)
			&& status_info->passive_tick != -1) {
		action_list.push_back(status_info);
	}

	return 0;
}

inline int insert_status_and_sort_passive(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		insert_status_and_sort_passive(&(*it), action_list);
	}

	return 0;
}

inline int remove_status_and_sort_passive(Status_Effect_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	if (status_info->heat_type.count(EFFECT_TYPE_PASSIVE)) {
		action_list.remove(status_info);
	}

	return 0;
}

inline int remove_status_and_sort_passive(Status_Info_Detail *status_info, Status_Detail::Status_Effect_List &action_list) {
	for (Status_Info_Detail::Effect_Vec::iterator it = status_info->effect_map.begin();
			it != status_info->effect_map.end(); ++it) {
		remove_status_and_sort_passive(&(*it), action_list);
	}

	return 0;
}

inline bool is_dead_remove(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return (bool)status_config->buff_disappear.count(1);
	}
}

inline bool is_timepri_remove(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return (bool)status_config->buff_disappear.count(STATUS_TYPE_TIME);
	}
}

inline bool is_action_remove(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return (bool)status_config->buff_disappear.count(STATUS_TYPE_SHOT);
	}
}

inline bool is_passive_remove(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return (bool)status_config->buff_disappear.count(STATUS_TYPE_PASSIVE);
	}
}

inline bool is_overlay(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return status_config->overlay > 0;
	}
}

inline bool is_dispel(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return status_config->is_dispel;
	}
}

inline bool is_pured(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return status_config->is_pure;
	}
}

inline bool is_stoic(int status_id) {
	const Btl_Buff_Cfg *status_config = CONFIG_CACHE_BUFF->btl_buff(status_id);
	if (NULL == status_config) {
		return false;
	} else {
		return status_config->is_stoic;
	}
}

inline void attribute_set_prop(const int attribute_id, const double value, Effect_Argv_Prop& prop) {
	const Attribute_Number *an = CONFIG_CACHE_BUFF->attribute_number(attribute_id);
	if (an) {
		prop.i_effect_val1 = an->attr_id;
		if (an->add) {
			prop.d_effect_val1 = value;
		} else if (an->percent) {
			prop.d_effect_val2 = value;
		} else if (an->cover) {
			prop.d_effect_val3 = value;
		}
	}
	if (prop.i_effect_val1 == 0) {
		prop.i_effect_val1 = attribute_id;
		prop.d_effect_val1 = value;
	}
}

inline void attribute_set_prop(Effect_Argv_Prop_Vec& prop) {
	for (Effect_Argv_Prop_Vec::iterator it = prop.begin(); it != prop.end(); ++it) {
		const Attribute_Number *an = CONFIG_CACHE_BUFF->attribute_number(it->i_effect_val1);
		if (an) {
			it->i_effect_val1 = an->attr_id;
			if (an->add) {
			} else if (an->percent) {
				if (0 == it->d_effect_val2) {
					it->d_effect_val2 = it->d_effect_val1;
					it->d_effect_val1 = 0.0;
				}
			} else if (an->cover) {
				if (0 == it->d_effect_val3) {
					it->d_effect_val3 = it->d_effect_val1;
					it->d_effect_val1 = 0.0;
				}
			}
		}
	}
}

void handle_poison_trigger(Fighter & fighter, Status_Effect_Detail & info, int skill_id = 0);
void handle_poison_by_expression_trigger(Fighter & fighter, Status_Effect_Detail & info, int skill_id = 0);

#endif /* STATUS_FUNC_H_ */
