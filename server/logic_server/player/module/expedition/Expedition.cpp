/*
 * Expedition.cpp
 *
 *  Created on: 2014-10-16
 *    	Author: xiaoliang
 */

#include "Expedition.h"
#include "Expedition_Struct.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Scene.h"
#include "Config_Cache_Expedition.h"
#include "Expedition_Manager.h"
#include "Logic_Player.h"
#include "gang/Gang_Struct.h"
#include "Config_Cache.h"
#include "Expedition_Def.h"
#include "Msg_Expedition_Struct.h"
#include "Msg_Inner_Enum.h"
#include "Config_Cache_NPC.h"
#include "team/Team_Func.h"
#include "Msg_Active_Struct.h"
#include "Scene_Func.h"

Expedition::Expedition(void) {
	reset();
}

Expedition::~Expedition() {
	// TODO Auto-generated destructor stub
}

void Expedition::reset(void) {
	expedition_detail_ = 0;
}

int Expedition::create_init(Expedition_Detail &detail) {

	return 0;
}

void Expedition::load_detail(Expedition_Detail &detail) {
	expedition_detail_ = &detail;
}

void Expedition::save_detail(Expedition_Detail &detail) {
	if (expedition_detail().is_change) {
		detail = expedition_detail();
		expedition_detail_->is_change = false;
	}
}

int Expedition::trigger_daily_zero(const Time_Value &now) {

	return 0;
}

int Expedition::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Expedition::sync_scene_module_info(Block_Buffer &buf) {
	expedition_detail_->award_nums = 0;
	Int64_Item_Info_Vec_Map::const_iterator it_iiiv = EXPEDITION_MANAGER->exepdition_manager_detail().award.find(player_self()->ganger_detail().gang_id);
	if (it_iiiv != EXPEDITION_MANAGER->exepdition_manager_detail().award.end()) {
		Item_Info item_info;
		for (Item_Info_Vec::const_iterator it = it_iiiv->second.begin();
			it != it_iiiv->second.end(); ++it) {
			expedition_detail_->award_nums += it->amount;
		}
	}
	expedition_detail_->serialize(buf);
	return 0;
}

int Expedition::move_scene_completed(void) {
	active_daily_award_info();
	return 0;
}

int Expedition::client_ready_now(void) {
	active_expedition_status();
	return 0;
}

void Expedition::module_init(void) {
	const Scene_Config *scene_config = CONFIG_CACHE_SCENE->scene_config_cache(player_self()->location().scene_id);
	if (scene_config) {
		if(Expedition_Type == scene_config->type) { //如果在远征场景中的;就按传入
			const Expedition_Common_Cfg *conf = CONFIG_CACHE_EXPEDITION->get_common_conf_by_key(Expedition_common_conf_type::LIMIT_LEVEL);
			if(!conf || conf->value == 0) {
				return;
			}
			if((uint32_t)player_self()->level() < conf->value) {
				return;
			}
			if(!player_self()->ganger_detail().gang_id) {
				return;
			}
			uint32_t line = EXPEDITION_MANAGER->get_line_by_gang_id(player_self()->ganger_detail().gang_id);
			if(!line) {
				return;
			}
			int64_t uuid = CONFIG_CACHE->server_flag();
			player_self()->set_create_scene_args(0, Time_Value::zero, uuid, 0, 0, line, true);
		}
	}
}

void Expedition::sign_in(void) {

}

void Expedition::sign_out(void) {

}

void Expedition::trusteeship_sign_out(void) {

}

int Expedition::sign_in_repeat(void) {
	sync_repeat_login_to_scene();
	return 0;
}

int Expedition::set_expedition_coord(Coord &coord) {
	if (player_self()->ganger_detail().gang_id) {
		expedition_detail_->coord = coord;
		expedition_detail_->detail_change();
	}
	return 0;
}

int Expedition::expedition_data_channle(Block_Buffer &b) {
	int8_t type = 0;
	b.read_int8(type);
	switch(type) {
	case Data_Channel::ASK_KILL_BEAST: {
		int point_id = 0;
		b.read_int32(point_id);
		int num = 0;
		Int_Int_Map::iterator it = expedition_detail_->kill_beast.find(point_id);
		if (it != expedition_detail_->kill_beast.end()) {
			num = it->second;
		}

		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_PLAYER);
		buf.write_int8(type);
		buf.write_int32(point_id);
		buf.write_int32(num);
		buf.finish_message();
		this->send_to_scene(buf);
		break;
	}
//	case Data_Channel::ASK_COLLECT_CHEST: {
//		int point_id = 0;
//		b.read_int32(point_id);
//		bool is_complete = false;
//		is_complete = (bool)expedition_detail_->collect_chest.count(point_id);
//
////		Block_Buffer buf;
////		buf.make_message(INNER_EXPEDITION_PLAYER);
////		buf.write_int8(type);
////		buf.write_int32(point_id);
////		buf.write_bool(is_complete);
////		buf.finish_message();
////		this->send_to_scene(buf);
//		break;
//	}
//	case Data_Channel::ASK_COLLECT_BEAST: {
//		int point_id = 0;
//		b.read_int32(point_id);
//		bool is_complete = false;
//		is_complete = (bool)expedition_detail_->collect_beast.count(point_id);
//
////		Block_Buffer buf;
////		buf.make_message(INNER_EXPEDITION_PLAYER);
////		buf.write_int8(type);
////		buf.write_int32(point_id);
////		buf.write_bool(is_complete);
////		buf.finish_message();
////		this->send_to_scene(buf);
//		break;
//	}
	case Data_Channel::COULD_COLLECT_CHEST: {
		int point_id = 0;
		int8_t tp = 0;
		b.read_int32(point_id);
		b.read_int8(tp);
		handle_drops_by_point_id(point_id);
		int percent = CONFIG_CACHE_EXPEDITION->get_percent_for_chest_by_count((int)expedition_detail_->collect_chest.size());
		if (tp == 0) {
			MSG_50101300 res_msg;
			res_msg.point_id = point_id;
			const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
			if (empc) {
				Reward_Info ri;
				boost::unordered_map<int, Drop_Vec>::iterator it = this->expedition_detail_->drops_all_vec_.find(point_id);
				if (it != this->expedition_detail_->drops_all_vec_.end()) {
					for (Drop_Vec::const_iterator it_drops_all = it->second.begin();
							it_drops_all != it->second.end(); ++it_drops_all) {
						ri.reset();
						ri.reward_id = (*it_drops_all).item;
						ri.quantity = (*it_drops_all).amount * percent / 100;
						ri.bind = (*it_drops_all).bind;
						if (ri.quantity <= 0) {
							continue;
						}
						res_msg.list.push_back(ri);
					}
				}
			}
			THIS_SEND_TO_CLIENT(res_msg);
		} else {
			MSG_50101301 res_msg;
			res_msg.point_id = point_id;
			res_msg.result = 1;
			MSG_81000102 acv_msg;
			acv_msg.type = 0;

			boost::unordered_map<int, Drop_Vec>::iterator it = this->expedition_detail_->drops_all_vec_.find(point_id);
			if (it != this->expedition_detail_->drops_all_vec_.end()) {
				Item_Vec iv;
				for (Drop_Vec::const_iterator it_drops_all = it->second.begin();
						it_drops_all != it->second.end(); ++it_drops_all) {
					int amount =  (*it_drops_all).amount * percent / 100;
					if (amount <= 0) {
						continue;
					}
					if (it_drops_all->item < 1000000) {
						int result = 0;
						switch(it_drops_all->item) {
						case COPPER:
							result = player_self()->pack_add_money(Money_Add_Info(COPPER, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
							break;
						case DRAGON_SOUL: {
							result = player_self()->pack_add_money(Money_Add_Info(DRAGON_SOUL, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
							break;
						}
						case SOULS: {
							result = player_self()->pack_add_money(Money_Add_Info(SOULS, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
							break;
						}
						case FRIENDSHIP: {
							result = player_self()->pack_add_money(Money_Add_Info(FRIENDSHIP, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
							break;
						}
						case GOLD: {
							result = player_self()->pack_add_money(Money_Add_Info(GOLD, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
							break;
						}
						case BIND_GOLD: {
							result = player_self()->pack_add_money(Money_Add_Info(BIND_GOLD, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
							break;
						}
						case 503003: {
							result = player_self()->change_exploit_val(amount);
							break;
						}
						default:
							break;
						}

						if (0 == result) {
							acv_msg.property.push_back(Property((*it_drops_all).item, amount));
						}
					} else {
						Item_Detail item_detail((*it_drops_all).item, amount, (*it_drops_all).bind);
						iv.push_back(item_detail);
						Item_Basic_Info ibi;
						item_detail.set_item_basic_info(ibi);
						acv_msg.item_info.push_back(ibi);
					}

				}
				if (!iv.empty()) {
					player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, iv, Gain_Item_DM_Info(Pack::ITEM_GAIN_EXPEDITION_CHEST));
				}
			}

			expedition_detail_->collect_chest.insert(point_id);
			expedition_detail_->detail_change();
			THIS_SEND_TO_CLIENT(res_msg);
			sync_to_scene(point_id);
			if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
				THIS_SEND_TO_CLIENT(acv_msg);
			}
		}
		break;
	}
	case COULD_COLLECT_BEAST: {
		int point_id = 0;
		b.read_int32(point_id);
		expedition_detail_->collect_beast.insert(point_id);
		expedition_detail_->detail_change();
		break;
	}
	case COULD_COLLECT_CASTLE: {
		int8_t tp = 0;
		b.read_int8(tp);

		Time_Value now = Time_Value::gettimeofday();
		Date_Time dt(now);
		Date_Time dt_opentime(CONFIG_CACHE->server_maintainer_cache().game_open_time);
		if (now < CONFIG_CACHE->server_maintainer_cache().game_open_time
				|| (dt.year() == dt_opentime.year() && dt.month() == dt_opentime.month() && dt.day() == dt_opentime.day() && dt.hour() < 22)) {
			return -1;
		}

		Date_Time dt_aw(expedition_detail().award_time);
		Date_Time dt_last_10(now);
		dt_last_10.day(dt_last_10.day() -1);
		dt_last_10.hour(22);
		dt_last_10.minute(0);
		dt_last_10.second(0);
		dt_last_10.microsec(0);

		Date_Time dt_10(now);
		dt_10.hour(22);
		dt_10.minute(0);
		dt_10.second(0);
		dt_10.microsec(0);

		if (dt_aw.time_sec() < dt_last_10.time_sec()) {
			// not collect catch yesterday 10
	//		res_msg.state = 1;
		} else if (dt_aw.time_sec() < dt_10.time_sec()) {
			if (dt.time_sec() < dt_10.time_sec()) {
				// alredy and not ready
				return 0;
			} else {
				// not collect
			}
		} else {
			// already catch time
			return 0;
		}

		if (0 == tp) {
			MSG_50201301 res_msg;
			res_msg.state = 1;
			Int64_Item_Info_Vec_Map::const_iterator it_iiiv = EXPEDITION_MANAGER->exepdition_manager_detail().award.find(player_self()->ganger_detail().gang_id);
			if (it_iiiv != EXPEDITION_MANAGER->exepdition_manager_detail().award.end()) {
				Item_Info item_info;
				for (Item_Info_Vec::const_iterator it = it_iiiv->second.begin();
					it != it_iiiv->second.end(); ++it) {
					item_info.reset();
					item_info = *it;
					res_msg.award_info.push_back(item_info);
				}
			} else {
				Block_Buffer buf;
				buf.make_message(INNER_EXPEDITION_PLAYER);
				buf.write_int8(REQ_ACV_DAILY_AWARD);
				buf.finish_message();
				this->send_to_scene(buf);
				return 0;
			}
			THIS_SEND_TO_CLIENT(res_msg);
		} else {
			int nums[5] = {1,0};
			uint32_t line = 0;
			for (int i = 0; i < 5; ++i) {
				b.read_int32(nums[i]);
			}
			b.read_uint32(line);

			double decay = CONFIG_CACHE_EXPEDITION->get_settle_award_decay(line) / 100.0;
			MSG_81000102 acv_msg;
			acv_msg.type = 0;
			Item_Vec iv;
			int64_t gang_id = player_self()->ganger_detail().gang_id;
			Int64_Item_Info_Vec_Map::const_iterator it_iiiv = EXPEDITION_MANAGER->exepdition_manager_detail().award.find(gang_id);
			if (it_iiiv != EXPEDITION_MANAGER->exepdition_manager_detail().award.end()) {
				for (Item_Info_Vec::const_iterator it = it_iiiv->second.begin();
						it != it_iiiv->second.end(); ++it) {
					int amount = it->amount * decay;
					if (amount <= 0) {
						continue;
					}
					if (it->id < 1000000) {
						int result = 0;
						switch(it->id) {
						case COPPER:
							result = player_self()->pack_add_money(Money_Add_Info(COPPER, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_SETTLE_AWARD, it->id, 1)));
							break;
						case DRAGON_SOUL: {
							result = player_self()->pack_add_money(Money_Add_Info(DRAGON_SOUL, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_SETTLE_AWARD, it->id, 1)));
							break;
						}
						case SOULS: {
							result = player_self()->pack_add_money(Money_Add_Info(SOULS, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_SETTLE_AWARD, it->id, 1)));
							break;
						}
						case FRIENDSHIP: {
							result = player_self()->pack_add_money(Money_Add_Info(FRIENDSHIP, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_SETTLE_AWARD, it->id, 1)));
							break;
						}
						case GOLD: {
							result = player_self()->pack_add_money(Money_Add_Info(GOLD, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_SETTLE_AWARD, it->id, 1)));
							break;
						}
						case BIND_GOLD: {
							result = player_self()->pack_add_money(Money_Add_Info(BIND_GOLD, amount,
									Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_SETTLE_AWARD, it->id, 1)));
							break;
						}
						case 503003: {
							result = player_self()->change_exploit_val(amount);
							break;
						}
						default:
							break;
						}

						if (0 == result) {
							acv_msg.property.push_back(Property(it->id, amount));
						}
					} else {
						Item_Detail item_detail(it->id, amount, it->bind);
						iv.push_back(item_detail);
						Item_Basic_Info ibi;
						item_detail.set_item_basic_info(ibi);
						acv_msg.item_info.push_back(ibi);
					}

				}
			}
			if (!iv.empty()) {
				player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, iv, Gain_Item_DM_Info(Pack::ITEM_GAIN_EXPEDITION_SETTLE_AWARD));
			}

			expedition_detail_->award_time = Time_Value::gettimeofday();
			expedition_detail_->award_nums = 0;
			expedition_detail_->detail_change();

			if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
				THIS_SEND_TO_CLIENT(acv_msg);
			}

			MSG_50201300 res_msg;
			res_msg.result = 1;
			THIS_SEND_TO_CLIENT(res_msg);
			sync_to_scene();

			{
				MSG_81101302 acv_msg_2;
				acv_msg_2.type = 2;
				acv_msg_2.state = 0;
				if (dt.time_sec() < dt_10.time_sec()) {
					acv_msg_2.award_time = dt_10.time_sec() - dt.time_sec();
				} else {
					acv_msg_2.award_time = dt_10.time_sec() - dt.time_sec() + Time_Value::ONE_DAY_IN_SECS;
				}

				int nums_all = 0;
				const Item_Info_Vec *iiv = 0;
				for (int i = 0; i < 5; ++i) {
					iiv = CONFIG_CACHE_EXPEDITION->find_settle_award(i + 1);
					if (iiv) {
						for (Item_Info_Vec::const_iterator it = iiv->begin();
								it != iiv->end(); ++it) {
							acv_msg_2.time += it->amount * nums[i];
							nums_all += it->amount * nums[i];
						}
					}
				}
				const Expedition_Daily_Award_Big_Data *big_award = CONFIG_CACHE_EXPEDITION->get_big_settle_award_by_nums(nums_all);
				if (big_award) {
					for (Item_Info_Vec::const_iterator it = big_award->award.begin();
							it != big_award->award.end(); ++it) {
						acv_msg_2.time += it->amount * big_award->amount;
					}
				}

				acv_msg_2.time *= decay;
				THIS_SEND_TO_CLIENT(acv_msg_2);
			}
		}
		break;
	}
	case DEMAGE_AWARD: {
		int gold = 0;
		int souls = 0;
		b.read_int32(gold);
		b.read_int32(souls);
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		if (gold > 1) {
			acv_msg.property.push_back(Property(COPPER, gold));
			player_self()->pack_add_money(Money_Add_Info(COPPER, gold, Pack::MONEY_ADD_EXPEDITION_DEMAGE_AWARD));
		}
		if (souls > 1) {
			acv_msg.property.push_back(Property(SOULS, souls));
			player_self()->pack_add_money(Money_Add_Info(SOULS, souls, Pack::MONEY_ADD_EXPEDITION_DEMAGE_AWARD));
		}

		if (!acv_msg.property.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
		break;
	}
	case OCCUPY_CASTLE_AWARD_GET: {
		role_id_t role_id = 0;
		int16_t nums = 0, get_nums = 0;
		b.read_int64(role_id);
		b.read_int16(nums);
		b.read_int16(get_nums);

		int result = 0;
		const Item_Info_Vec *iiv = CONFIG_CACHE_EXPEDITION->find_occupy_castle_rank_award(get_nums);
		if (iiv) {
			Item_Vec item_list;
			Item_Detail item_detail;
			for (Item_Info_Vec::const_iterator it = iiv->begin(); it != iiv->end(); ++it) {
				item_detail.reset();
				item_detail.id_ = it->id;
				item_detail.amount_ = it->amount;
				item_detail.bind_ = it->bind;
				item_list.push_back(item_detail);
			}
			if (!item_list.empty()) {
				result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(Pack::ITEM_GAIN_EXPEDITION_OCCUPY_CASTLE_AWARD));
			}
		}

		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_PLAYER);
		buf.write_uint8(OCCUPY_CASTLE_AWARD_GET);
		buf.write_int32(result);

		buf.finish_message();
		LOGIC_MONITOR->send_to_scene(EXPEDITION_INNER_SCENE_ID, 0, buf);
		break;
	}
	default:
		break;
	}
	return 0;
}

int Expedition::req_expedition_open_chest(const MSG_10101300 &msg) {
	if (expedition_detail_->collect_chest.count(msg.point_id)) {
		MSG_50101300 res_msg;
		res_msg.point_id = msg.point_id;
		THIS_SEND_TO_CLIENT(res_msg);
	} else {
		const Expedition_Map_Point_Cfg * empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(msg.point_id);
		if (!empc || EBT_CHEST != empc->type) {
			return ERROR_CLIENT_OPERATE;
		}

		handle_drops_by_point_id(msg.point_id);
		int percent = CONFIG_CACHE_EXPEDITION->get_percent_for_chest_by_count((int)expedition_detail_->collect_chest.size());

		MSG_50101300 res_msg;
		res_msg.point_id = msg.point_id;

		Reward_Info ri;
		boost::unordered_map<int, Drop_Vec>::iterator it = this->expedition_detail_->drops_all_vec_.find(msg.point_id);
		if (it != this->expedition_detail_->drops_all_vec_.end()) {
			for (Drop_Vec::const_iterator it_drops_all = it->second.begin();
					it_drops_all != it->second.end(); ++it_drops_all) {
				ri.reset();
				ri.reward_id = (*it_drops_all).item;
				ri.quantity = (*it_drops_all).amount * percent / 100;
				ri.bind = (*it_drops_all).bind;
				if (ri.quantity <= 0) {
					continue;
				}
				res_msg.list.push_back(ri);
			}
		}

		THIS_SEND_TO_CLIENT(res_msg);
//		Block_Buffer buf;
//		buf.make_message(INNER_EXPEDITION_PLAYER);
//		buf.write_int8((int8_t)Data_Channel::ASK_COLLECT_CHEST);
//		buf.write_int32(msg.point_id);
//		buf.write_int8(0);// for ask list
//		buf.finish_message();
//		this->send_to_scene(buf);
	}
	return 0;
}

int Expedition::req_expedition_chest_collect(const MSG_10101301 &msg) {
	if (expedition_detail_->collect_chest.count(msg.point_id)) {
		MSG_50101301 res_msg;
		res_msg.point_id = msg.point_id;
		THIS_SEND_TO_CLIENT(res_msg);
	} else {
		const Expedition_Map_Point_Cfg * empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(msg.point_id);
		if (!empc || EBT_CHEST != empc->type) {
			return ERROR_CLIENT_OPERATE;
		}

		MSG_50101301 res_msg;
		res_msg.point_id = msg.point_id;
		res_msg.result = 1;
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		int percent = CONFIG_CACHE_EXPEDITION->get_percent_for_chest_by_count((int)expedition_detail_->collect_chest.size());

		boost::unordered_map<int, Drop_Vec>::iterator it = this->expedition_detail_->drops_all_vec_.find(msg.point_id);
		if (it != this->expedition_detail_->drops_all_vec_.end()) {
			Item_Vec iv;
			for (Drop_Vec::const_iterator it_drops_all = it->second.begin();
					it_drops_all != it->second.end(); ++it_drops_all) {
				int amount =  (*it_drops_all).amount * percent / 100;
				if (amount <= 0) {
					continue;
				}
				if (it_drops_all->item < 1000000) {
					int result = 0;
					switch(it_drops_all->item) {
					case COPPER:
						result = player_self()->pack_add_money(Money_Add_Info(COPPER, amount,
								Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
						break;
					case DRAGON_SOUL: {
						result = player_self()->pack_add_money(Money_Add_Info(DRAGON_SOUL, amount,
								Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
						break;
					}
					case SOULS: {
						result = player_self()->pack_add_money(Money_Add_Info(SOULS, amount,
								Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
						break;
					}
					case FRIENDSHIP: {
						result = player_self()->pack_add_money(Money_Add_Info(FRIENDSHIP, amount,
								Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
						break;
					}
					case GOLD: {
						result = player_self()->pack_add_money(Money_Add_Info(GOLD, amount,
								Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
						break;
					}
					case BIND_GOLD: {
						result = player_self()->pack_add_money(Money_Add_Info(BIND_GOLD, amount,
								Money_DM_Info(Pack::MONEY_ADD_EXPEDITION_CHEST, (*it_drops_all).item, 1)));
						break;
					}
					case 503003: {
						result = player_self()->change_exploit_val(amount);
						break;
					}
					default:
						break;
					}

					if (0 == result) {
						acv_msg.property.push_back(Property((*it_drops_all).item, amount));
					}
				} else {
					Item_Detail item_detail((*it_drops_all).item, amount, (*it_drops_all).bind);
					iv.push_back(item_detail);
					Item_Basic_Info ibi;
					item_detail.set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
				}

			}
			if (!iv.empty()) {
				player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, iv, Gain_Item_DM_Info(Pack::ITEM_GAIN_EXPEDITION_CHEST));
			}
		}

		expedition_detail_->collect_chest.insert(msg.point_id);
		expedition_detail_->detail_change();
		THIS_SEND_TO_CLIENT(res_msg);
		sync_to_scene(msg.point_id);
		if (!acv_msg.property.empty() || !acv_msg.item_info.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
//		Block_Buffer buf;
//		buf.make_message(INNER_EXPEDITION_PLAYER);
//		buf.write_int8((int8_t)Data_Channel::ASK_COLLECT_CHEST);
//		buf.write_int32(msg.point_id);
//		buf.write_int8(1);// for collect list
//		buf.finish_message();
//		this->send_to_scene(buf);
	}
	return 0;
}


void Expedition::active_expedition_status(void) {
	MSG_81101302 acv_msg;
	acv_msg.type = 4;

	if (Time_Value::zero != EXPEDITION_MANAGER->exepdition_manager_detail().first_listen_time
			|| !EXPEDITION_MANAGER->exepdition_manager_detail().gang_data_info_map.empty()) {
		acv_msg.state = 2;
	}

	THIS_SEND_TO_CLIENT(acv_msg);
}


int Expedition::sync_to_scene(const int point_id) {
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_PLAYER);
	buf.write_int8(SYNC_PLAYER_DETAIL);

	expedition_detail_->award_nums = 0;
	Int64_Item_Info_Vec_Map::const_iterator it_iiiv = EXPEDITION_MANAGER->exepdition_manager_detail().award.find(player_self()->ganger_detail().gang_id);
	if (it_iiiv != EXPEDITION_MANAGER->exepdition_manager_detail().award.end()) {
		Item_Info item_info;
		for (Item_Info_Vec::const_iterator it = it_iiiv->second.begin();
			it != it_iiiv->second.end(); ++it) {
			expedition_detail_->award_nums += it->amount;
		}
	}

	expedition_detail_->serialize(buf);
	buf.write_int32(point_id);
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

int Expedition::sync_to_scene_chest_point(const int point_id) {
	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_PLAYER);
	buf.write_int8(SYNC_CHEST_BY_POINT);
	buf.write_int32(point_id);
	buf.write_bool(expedition_detail_->collect_chest.count(point_id));
	buf.finish_message();
	this->send_to_scene(buf);
	return 0;
}

int Expedition::expedition_listen_quit_gang(void) {
	expedition_detail_->coord.reset();
	expedition_detail_->detail_change();
	return 0;
}

int Expedition::handle_drops_by_point_id(const int point_id) {
	if (this->expedition_detail_->drops_all_vec_.count(point_id)
			|| expedition_detail_->collect_chest.count(point_id)) {
		return 0;
	}
	const Expedition_Map_Point_Cfg *empc = CONFIG_CACHE_EXPEDITION->get_map_point_conf_by_id(point_id);
	if (empc) {
		for (Int_Vec::const_iterator it_drops = empc->drops.begin();
				it_drops != empc->drops.end(); ++it_drops) {
			const Drop_Config* drop_config = CONFIG_CACHE_NPC->find_drop_config_cache(*it_drops);
			if (drop_config == NULL) continue;
			Drop_Vec drops_all;

			team_card_process_drop_mode(0, drops_all, drop_config, 1.0, 1.0);
			boost::unordered_map<int, Drop_Vec>::iterator it = this->expedition_detail_->drops_all_vec_.find(point_id);
			if (it != this->expedition_detail_->drops_all_vec_.end()) {
				for (Drop_Vec::const_iterator it_drops_all = drops_all.begin();
						it_drops_all != drops_all.end(); ++it_drops_all) {
					it->second.push_back(*it_drops_all);
				}
			} else {
				this->expedition_detail_->drops_all_vec_.insert(std::make_pair(point_id, drops_all));
			}
		}
	}
	return 0;
}

int Expedition::active_daily_award_info(void) {
	if (Expedition_Type != get_scene_type(player_self()->location().scene_id)) {
		return -1;
	}
	Time_Value now = Time_Value::gettimeofday();
	Date_Time dt(now);
	Date_Time dt_opentime(CONFIG_CACHE->server_maintainer_cache().game_open_time);
	if (now < CONFIG_CACHE->server_maintainer_cache().game_open_time
			|| (dt.year() == dt_opentime.year() && dt.month() == dt_opentime.month() && dt.day() == dt_opentime.day() && dt.hour() < 22)) {
		return 0;
	}

	Date_Time dt_aw(expedition_detail().award_time);
	Date_Time dt_last_10(now);
	dt_last_10.day(dt_last_10.day() -1);
	dt_last_10.hour(22);
	dt_last_10.minute(0);
	dt_last_10.second(0);
	dt_last_10.microsec(0);

	Date_Time dt_10(now);
	dt_10.hour(22);
	dt_10.minute(0);
	dt_10.second(0);
	dt_10.microsec(0);

	if (dt_aw.time_sec() < dt_last_10.time_sec()) {
		// not collect catch yesterday 10
//		res_msg.state = 1;
	} else if (dt_aw.time_sec() < dt_10.time_sec()) {
		if (dt.time_sec() < dt_10.time_sec()) {
			// alredy and not ready
			return 0;
		} else {
			// not collect
		}
	} else {
		// already catch time
		return 0;
	}

	MSG_81101302 res_msg;
	res_msg.state = 1;
	res_msg.type = 2;
	Int64_Item_Info_Vec_Map::const_iterator it_iiiv = EXPEDITION_MANAGER->exepdition_manager_detail().award.find(player_self()->ganger_detail().gang_id);
	if (it_iiiv != EXPEDITION_MANAGER->exepdition_manager_detail().award.end()) {
		Item_Info item_info;
		for (Item_Info_Vec::const_iterator it = it_iiiv->second.begin();
			it != it_iiiv->second.end(); ++it) {
			res_msg.time += it->amount;
		}
	} else {
		Block_Buffer buf;
		buf.make_message(INNER_EXPEDITION_PLAYER);
		buf.write_int8(REQ_ACV_DAILY_AWARD);
		buf.finish_message();
		this->send_to_scene(buf);
		return 0;
	}
	THIS_SEND_TO_CLIENT(res_msg);

	return 0;
}

void Expedition::sync_repeat_login_to_scene(void) {

	Block_Buffer buf;
	buf.make_message(INNER_EXPEDITION_PLAYER);
	buf.write_int8(SYNC_REPEAT_TO_SCENE);
	buf.finish_message();
	this->send_to_scene(buf);
}

int Expedition::reset_award_time(void) {
	expedition_detail_->award_time = Time_Value::zero;
	return 0;
}
