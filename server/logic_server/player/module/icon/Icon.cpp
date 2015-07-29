/*
 * Icon.cpp
 *
 *  Created on: 2014年5月22日
 *      Author: gaojianshan
 */

#include "Icon.h"
#include "Icon_Struct.h"
#include "Logic_Module_Head.h"
#include "Base_Detail.h"
#include "Config_Cache_Icon.h"
#include "Msg_Active_Struct.h"
#include "task/Task_Struct.h"
#include "Logic_Player.h"
#include "gang/Gang_Struct.h"
#include "Icon_Def.h"
Icon::Icon(void) {
	reset();
}

Icon::~Icon() {
}

void Icon::reset(void) {
	icon_detail_ = 0;
}

int Icon::create_init(Icon_Detail &iconDetail, const Base_Detail &baseDetail) {
	//创建角色时，初始化功能图标开启信息
	const Icon_Config_Map &icon_cfg_map =CONFIG_CACHE_ICON->icon_config();
	for(Icon_Config_Map::const_iterator it = icon_cfg_map.begin(); it != icon_cfg_map.end(); ++it) {
		if(it->second.type != ICON_TYPE_DOWN) {
			continue;
		}

		Svr_Icon_Info svr_icon_info;
		svr_icon_info.id = it->second.icon_id;
		//是否加入公会、  使用道具开启图标、 任务状态开启条件
		const Icon_Base_Config &icon_cfg = it->second;
		if(is_join_gang(icon_cfg) || is_use_item(icon_cfg) || is_task(icon_cfg)) {
			svr_icon_info.status = 0;
			iconDetail.icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
			continue;
		}

		//比较创建角色时的等级 与等级开启条件
		if(it->second.level && baseDetail.level < it->second.level) {
			svr_icon_info.status = 0;
			iconDetail.icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
			continue;
		}

	    svr_icon_info.status = 1;
	    svr_icon_info.is_ever_opened = true;
		iconDetail.icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
	}

	return 0;
}

void Icon::load_detail(Icon_Detail &detail) {
	icon_detail_ = &detail;
}

void Icon::save_detail(Icon_Detail &detail) {
	if (icon_detail().is_change) {
		detail = icon_detail();
		icon_detail_->is_change = false;
	}
}

int Icon::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Icon::move_scene_completed(void) {

	return 0;
}

int Icon::client_ready_now(void) {
	if(!icon_detail_) {
		return -1;
	}
	//通知客户端所有已开启的图标
	MSG_80100400  icon_msg;
	icon_msg.reset();
	const Icon_Info_Map &iconMap = icon_detail_->icon_map;
	for(Icon_Info_Map::const_iterator it = iconMap.begin(); it != iconMap.end(); ++it) {
		Icon_Info iconInfo;
		if(it->second.status == 1) {
			const Icon_Base_Config *icon_cfg = CONFIG_CACHE_ICON->find_config(it->second.id);
			if(!icon_cfg) {
				continue;
			}

			iconInfo.id = it->second.id;
			iconInfo.status = it->second.status;
			icon_msg.icon_list.push_back(iconInfo);
		}
	}
	THIS_SEND_TO_CLIENT(icon_msg);
	return 0;
}

void Icon::module_init(void) {
	if(!icon_detail_) {
		return;
	}

	//同步图标的策划配置与数据库数据
	const Icon_Config_Map &icon_cfg_map = CONFIG_CACHE_ICON->icon_config();
	for(Icon_Config_Map::const_iterator it = icon_cfg_map.begin(); it != icon_cfg_map.end(); ++it) {
		if(find_icon(it->first)) {
			continue;
		}

		if(it->second.type != ICON_TYPE_DOWN) {
			continue;
		}

		Svr_Icon_Info svr_icon_info;
		svr_icon_info.id = it->second.icon_id;
		//是否加入公会、  使用道具开启图标、 任务状态开启条件
		const Icon_Base_Config &icon_cfg = it->second;
		if(is_join_gang(icon_cfg) || is_use_item(icon_cfg) || is_task(icon_cfg)) {
			svr_icon_info.status = 0;
			icon_detail_->icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
			icon_detail_->detail_change();
			continue;
		}

		//比较玩家等级 与等级开启条件
		if(it->second.level && player_self()->level() < it->second.level) {
			svr_icon_info.status = 0;
			icon_detail_->icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
			icon_detail_->detail_change();
			continue;
		}

	    svr_icon_info.status = 1;
	    svr_icon_info.is_ever_opened = true;

	    icon_detail_->icon_map.insert(std::make_pair(svr_icon_info.id, svr_icon_info));
	    icon_detail_->detail_change();
	}
}

void Icon::sign_in(void) {

}

void Icon::sign_out(void) {

}

int Icon::sign_in_repeat(void) {
	return 0;
}

void Icon::notify_open_icon_by_level(int new_level) {
	for(Icon_Info_Map::iterator it = icon_detail_->icon_map.begin(); it != icon_detail_->icon_map.end(); ++it) {
		const Icon_Base_Config *icon_cfg = CONFIG_CACHE_ICON->find_config(it->first);
		if(!icon_cfg) {
			continue;
		}

		if(icon_cfg->type != ICON_TYPE_DOWN) {
			continue;
		}

		if(icon_cfg->is_forever_close && it->second.is_ever_opened && it->second.status == 0) {
			continue;
		}

		if(icon_cfg->is_forever_close && icon_cfg->forver_close_level && new_level >= (int)icon_cfg->forver_close_level) {
			if(1 == it->second.status) {
				MSG_80100401 msg;
				msg.id = it->second.id;
				msg.status = 0;
				THIS_SEND_TO_CLIENT(msg);

				it->second.is_ever_opened = true;
				it->second.status = 0;
				icon_detail_->detail_change();
				continue;
			}
		}

		if(it->second.status == 0) {
			//使用道具开启图标
			if(is_use_item(*icon_cfg) && !it->second.is_used_item) {
				continue;
			}

			//加入公会开启图标
			if(is_join_gang(*icon_cfg) && !player_self()->ganger_detail().gang_id) {
				continue;
			}

			//玩家等级开启图标
			if(is_level(*icon_cfg) && new_level < icon_cfg->level) {
				continue;
			}

			//任务状态开启条件列表
			if(is_task(*icon_cfg) && !is_all_task_pass(*icon_cfg)) {
				continue;
			}//end if

			if(11701 == it->first && player_self()->has_hero_id(60220001)) {
				it->second.status = 0;
				it->second.is_ever_opened = true;
				icon_detail_->detail_change();
				continue;
			}

			//推送开启图标消息
			MSG_80100401 icon_msg;
			icon_msg.id = it->first;
			icon_msg.status = 1;
			THIS_SEND_TO_CLIENT(icon_msg);
			//更改图标状态为开启
			it->second.status = 1;
			it->second.is_ever_opened = true;
			icon_detail_->detail_change();
			hook_icon_open(it->first);
		}
	}
}

bool Icon::is_level(const Icon_Base_Config &cfg) {
	return cfg.level ? true : false;
}

bool Icon::is_task(const Icon_Base_Config &cfg) {
	return !cfg.task_open_condition_list.empty();
}

bool Icon::is_use_item(const Icon_Base_Config &cfg) {
	return cfg.item_id ? true : false;
}

bool Icon::is_join_gang(const Icon_Base_Config &cfg) {
	return cfg.is_join_gang;
}

bool Icon::is_all_task_pass(const Icon_Base_Config &cfg) {
	for(Task_Open_Condition_List::const_iterator it = cfg.task_open_condition_list.begin();
			it != cfg.task_open_condition_list.end(); ++it) {
		//任务提交开启条件
		if(it->task_status == TaskInfo::TASK_STATUS_SUBMIT) {
			//如果任务未提交，不开启图标
			if(player_self()->tasker_detail().task_submited_once_.find(it->task_id)
					== player_self()->tasker_detail().task_submited_once_.end()) {
				return false;
			}

			continue;
		}

		//其他任务状态开启条件
		TaskMap::const_iterator task_it = player_self()->tasker_detail().task_map_.find(it->task_id);
		if(task_it == player_self()->tasker_detail().task_map_.end()) {
			return false;
		}

		if(it->task_status == TaskInfo::TASK_STATUS_ACCEPTED
				&& !(task_it->second.is_accepted())
				&& !(task_it->second.is_finish())
				&& player_self()->task_state(task_it->second.__task_id) != TaskInfo::TASK_STATUS_SUBMIT) {
			return false;
		}

		if(it->task_status == TaskInfo::TASK_STATUS_FINISH
				&& !(task_it->second.is_finish())
				&& player_self()->task_state(task_it->second.__task_id) != TaskInfo::TASK_STATUS_SUBMIT) {
			return false;
		}

	}//end for

	return true;
}

void Icon::notify_open_icon_by_join_gang() {
	for(Icon_Info_Map::iterator it = icon_detail_->icon_map.begin(); it != icon_detail_->icon_map.end(); ++it) {
		const Icon_Base_Config *icon_cfg = CONFIG_CACHE_ICON->find_config(it->first);
		if(!icon_cfg) {
			continue;
		}

		if(icon_cfg->type != ICON_TYPE_DOWN) {
			continue;
		}

		if(icon_cfg->is_forever_close && it->second.is_ever_opened && it->second.status == 0) {
			continue;
		}

		if(it->second.status == 0) {
			//使用道具开启图标
			if(is_use_item(*icon_cfg) && !it->second.is_used_item) {
				continue;
			}

			//加入公会开启图标
			if(is_join_gang(*icon_cfg) && !player_self()->ganger_detail().gang_id) {
				continue;
			}

			//玩家等级开启图标
			if(is_level(*icon_cfg) && player_self()->level() < icon_cfg->level) {
				continue;
			}

			//任务状态开启条件列表
			if(is_task(*icon_cfg) && !is_all_task_pass(*icon_cfg)) {
				continue;
			}//end if

			if(11701 == it->first && player_self()->has_hero_id(60220001)) {
				it->second.status = 0;
				it->second.is_ever_opened = true;
				icon_detail_->detail_change();
				continue;
			}

			//推送开启图标消息
			MSG_80100401 icon_msg;
			icon_msg.id = it->first;
			icon_msg.status = 1;
			THIS_SEND_TO_CLIENT(icon_msg);
			//更改图标状态为开启
			it->second.status = 1;
			it->second.is_ever_opened = true;
			icon_detail_->detail_change();
			hook_icon_open(it->first);
		}
	}
}

void Icon::notify_open_icon_by_task_status() {
	for(Icon_Info_Map::iterator it = icon_detail_->icon_map.begin(); it != icon_detail_->icon_map.end(); ++it) {
		const Icon_Base_Config *icon_cfg = CONFIG_CACHE_ICON->find_config(it->first);
		if(!icon_cfg) {
			continue;
		}

		if(icon_cfg->type != ICON_TYPE_DOWN) {
			continue;
		}

		if(icon_cfg->is_forever_close && it->second.is_ever_opened && it->second.status == 0) {
			continue;
		}

		if(it->second.status == 0) {
			//使用道具开启图标
			if(is_use_item(*icon_cfg) && !it->second.is_used_item) {
				continue;
			}

			//加入公会开启图标
			if(is_join_gang(*icon_cfg) && !player_self()->ganger_detail().gang_id) {
				continue;
			}

			//玩家等级开启图标
			if(is_level(*icon_cfg) && player_self()->level() < icon_cfg->level) {
				continue;
			}

			//任务状态开启条件列表
			if(is_task(*icon_cfg) && !is_all_task_pass(*icon_cfg)) {
				continue;
			}//end if

			if(11701 == it->first && player_self()->has_hero_id(60220001)) {
				it->second.status = 0;
				it->second.is_ever_opened = true;
				icon_detail_->detail_change();
				continue;
			}

			//推送开启图标消息
			MSG_80100401 icon_msg;
			icon_msg.id = it->first;
			icon_msg.status = 1;
			THIS_SEND_TO_CLIENT(icon_msg);
			//更改图标状态为开启
			it->second.status = 1;
			it->second.is_ever_opened = true;
			icon_detail_->detail_change();
			hook_icon_open(it->first);
		}
	}
}

void Icon::notify_open_icon_by_use_item(uint32_t item_id) {
	for(Icon_Info_Map::iterator it = icon_detail_->icon_map.begin(); it != icon_detail_->icon_map.end(); ++it) {
		const Icon_Base_Config *icon_cfg = CONFIG_CACHE_ICON->find_config(it->first);
		if(!icon_cfg) {
			continue;
		}

		if(icon_cfg->type != ICON_TYPE_DOWN) {
			continue;
		}

		if(icon_cfg->is_forever_close && it->second.is_ever_opened && it->second.status == 0) {
			continue;
		}

		if(it->second.status == 0) {
			//使用道具开启图标
			if(is_use_item(*icon_cfg) && item_id != icon_cfg->item_id) {
				continue;
			}

			if(is_use_item(*icon_cfg) && item_id == icon_cfg->item_id) {
				it->second.is_used_item = true;
				icon_detail_->detail_change();
			}

			//加入公会开启图标
			if(is_join_gang(*icon_cfg) && !player_self()->ganger_detail().gang_id) {
				continue;
			}

			//玩家等级开启图标
			if(is_level(*icon_cfg) && player_self()->level() < icon_cfg->level) {
				continue;
			}

			//任务状态开启条件列表
			if(is_task(*icon_cfg) && !is_all_task_pass(*icon_cfg)) {
				continue;
			}//end if

			if(11701 == it->first && player_self()->has_hero_id(60220001)) {
				it->second.status = 0;
				it->second.is_ever_opened = true;
				icon_detail_->detail_change();
				continue;
			}

			//推送开启图标消息
			MSG_80100401 icon_msg;
			icon_msg.id = it->first;
			icon_msg.status = 1;
			THIS_SEND_TO_CLIENT(icon_msg);
			//更改图标状态为开启
			it->second.status = 1;
			it->second.is_ever_opened = true;
			icon_detail_->detail_change();
			hook_icon_open(it->first);
		}
	}
}

Svr_Icon_Info* Icon::find_icon(uint16_t icon_id) {
	if(!icon_detail_) {
		return 0;
	}

	Icon_Info_Map::iterator it = icon_detail_->icon_map.find(icon_id);
	if(it == icon_detail_->icon_map.end()) {
		return 0;
	}
	return &(it->second);
}

bool Icon::is_icon_open(uint16_t icon_id) {
	Icon_Info_Map::iterator it = icon_detail_->icon_map.find(icon_id);
	if(it == icon_detail_->icon_map.end()) {
		MSG_USER("icon_id not exist, icon_id=%d\n", icon_id);
		return false;
	}

	return it->second.status == 1 ? true : false;
}

void Icon::hook_icon_open(uint16_t icon_id) {
	player_self()->helper_listen_icon_open(icon_id);
	player_self()->knight_trials_listen_icon_open(icon_id);
	player_self()->dragon_vale_listen_icon_open(icon_id);
}

void Icon::other_module_notify_icon_to_close_icon(uint16_t icon_id) {
	Icon_Info_Map::iterator it = icon_detail_->icon_map.find(icon_id);
	if(it == icon_detail_->icon_map.end()) {
		MSG_USER("icon_id not exist, icon_id=%d\n", icon_id);
		return;
	}

	if(0 == it->second.status) {
		return;
	}

	MSG_80100401 msg;
	msg.id = icon_id;
	msg.status = 0;
	THIS_SEND_TO_CLIENT(msg);

	it->second.is_ever_opened = true;
	it->second.status = 0;
	icon_detail_->detail_change();
}

void Icon::safe_guide_open_icon(uint16_t icon_id) {
	Icon_Info_Map::iterator it = icon_detail_->icon_map.find(icon_id);
	if(it == icon_detail_->icon_map.end()) {
		MSG_USER("icon_id not exist, icon_id=%d\n", icon_id);
		return;
	}

	if(1 == it->second.status) {
		return;
	}

	const Icon_Base_Config *icon_cfg = CONFIG_CACHE_ICON->find_config(icon_id);

	if(icon_cfg && icon_cfg->is_forever_close && it->second.is_ever_opened) {
		return;
	}

	if(11701 == icon_id && player_self()->has_hero_id(60220001)) {
		it->second.status = 0;
		it->second.is_ever_opened = true;
		icon_detail_->detail_change();
		return;
	}

	MSG_80100401 msg;
	msg.id = icon_id;
	msg.status = 1;
	THIS_SEND_TO_CLIENT(msg);

	it->second.is_ever_opened = true;
	it->second.status = 1;
	icon_detail_->detail_change();
	hook_icon_open(icon_id);
}
