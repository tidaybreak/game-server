/*
 * Config_Cache_Item.cpp
 *
 *  Created on: Jul 9, 2012
 *      Author: linqiyou
 */

#include "Config_Cache_Item.h"
#include "Config_Cache_Fighter.h"
#include "Config_Cache_Role.h"
#include "Configurator.h"
#include "Json_Utility.h"
#include "pack/Pack_Struct.h"
#include "fighter/Fighter_Def.h"
#include "item_user/Item_User_Func.h"
#include "Record_Client.h"
#include "pack/Pack_Struct.h"

const Item_Config &Config_Cache_Item::item_config(void) const {
	return item_config_;
}

const Item_Detail_Config *Config_Cache_Item::find_item(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, item_config_);
}

const Item_Lottery_Config &Config_Cache_Item::item_lottery_config(void) const {
	return item_lottery_config_;
}

const Item_Lottery_Detail_Config *Config_Cache_Item::find_item_lottery(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, item_lottery_config_);
}

const Item_Drug_Cfg &Config_Cache_Item::item_drug_config(void) const {
	return item_drug_cfg_;
}

const Item_Drug_Detail *Config_Cache_Item::find_item_drug(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, item_drug_cfg_);
}

const Item_Card_Detail *Config_Cache_Item::find_item_card(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, item_card_cfg_);
}

const Item_Resource_Cfg &Config_Cache_Item::item_resource_config(void) const {
	return item_resource_cfg_;
}

const Item_Resource_Detail *Config_Cache_Item::find_item_resource(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, item_resource_cfg_);
}

const Item_Material_Cfg &Config_Cache_Item::item_material_config(void) const {
	return item_material_cfg_;
}

const Item_Material_Detail *Config_Cache_Item::find_item_material(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, item_material_cfg_);
}

const Item_Little_Fuction_Config *Config_Cache_Item::find_item_little_fun(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, item_little_fuc_cfg_);
}

const Quick_Buy *Config_Cache_Item::find_quick_buy(const int item_id) const {
	return get_map_second_pointer_by_key(item_id, quick_buy_cfg_);
}
const Item_Gain_Source_Detail *Config_Cache_Item::find_item_gain_source(const int type) const {
	return get_map_second_pointer_by_key(type, item_gain_source_cfg_);
}

void Config_Cache_Item::refresh_config_cache(void) {
	PERF_MON("Config_Cache_Item");

	refresh_item_config();
	refresh_quick_buy();
	refresh_item_gain_source();

	refresh_gemstone_base();
	refresh_gemstone_msic();
	refresh_gemstone_conversion();
	refresh_gemstone_ext_attr();
	refresh_gem_carve_prop();
}

void Config_Cache_Item::get_use_item_list(const Json::Value value,const int key) {

	if (value.isMember("gain_item_list") ) {
		const Json::Value item_value = value["gain_item_list"];
		if ( item_value.isArray() ) {
			Item_Resource_Detail re_config;
			for (uint8_t i = 0;i<item_value.size();++i ) {

			    if ( item_value[i].isArray() && item_value[i].size() == 2 ) {

					Int_Int gain_item;
					gain_item.val_1 = item_value[i][0u].asInt();
					gain_item.val_2 = item_value[i][1u].asInt();
					re_config.gain_list.push_back(gain_item);
				}
			}
			set_map_second_value_by_key(key, item_resource_cfg_, re_config);
		}
	}

}

void Config_Cache_Item::refresh_item_config(void) {
	const Json::Value& item_json = CONFIG_INSTANCE->config_json()["item"]["item"];
	Int_Vec id_vec;
	id_vec.clear();

	for (Json::Value::iterator it = item_json.begin();
			it != item_json.end(); ++it) {
		int key = (*it)["id"].asInt();
		if (key < Item_Detail::ITEM_START || key > Item_Detail::ITEM_END) {
			id_vec.push_back(key);
		} else {
			Item_Detail_Config item_detail;
			item_detail.type = (*it)["type"].asInt();
			item_detail.name = (*it)["name"].asString();
			item_detail.item_id = key;
			item_detail.needLevel = (*it)["needLevel"].asInt();
			item_detail.occupation = (*it)["occupation"].asInt();
			item_detail.sellPrice = (*it)["sellPrice"].asInt();
			item_detail.marketPrice = (*it)["marketPrice"].asInt();
			item_detail.sys_sell = (*it)["sys_sell"].asInt();
			item_detail.sys_sell_type = (*it)["sys_sell_type"].asInt();
			item_detail.bind = static_cast<Item_Detail::Bind_Type>((*it)["bind"].asInt());
			item_detail.notAbandon = (*it)["notAbandon"].asInt();
			item_detail.rare = (*it)["rare"].asInt();
			item_detail.overlay = (*it)["overlay"].asInt();
			if (0 == item_detail.overlay) {
				item_detail.overlay = 1;
			}
			item_detail.time = Time_Value((*it)["time"].asInt(), 0);
			if (item_detail.overlay > 1 && item_detail.time > Time_Value::zero) {
				LOG_ABORT("item_id: %d is error(time and overlay is conflict) in item.json.", key);
			}
			item_detail.clickHandle = (*it)["clickHandle"].asInt();
			item_detail.moduleId = (*it)["moduleId"].asInt();
			if (!(*it)["scene"].isNull() &&
					(*it)["scene"].isArray()) {
				Json_Utility::assign_int_vec((*it)["scene"], item_detail.scene);
			}
			if (!(*it)["timeLimit"].isNull() &&
					(*it)["timeLimit"].isArray()) {
				Json_Utility::assign_int_vec((*it)["timeLimit"], item_detail.timeLimit);
			}
			item_detail.cdTime = (*it)["cdTime"].asInt();
//			item_detail.cool_type = (*it)["cool_type"].asInt();
			item_detail.color = (*it)["color"].asInt();
			item_detail.fashion_upgrade_type = (*it)["fashion_advance"].asInt();
			item_detail.next_spar_id = (*it)["next_id"].asUInt();

			/// equip
			if (! (*it)["equip"].empty()) {
				refresh_item_equip(item_detail, (*it)["equip"]);
			}
			// item_special
			if (! (*it)["item_special"].empty()) {
				refresh_item_special(item_detail, (*it)["item_special"]);
			}
			// rune_stone
			if (! (*it)["rune_stone"].empty()) {
				refresh_item_rune_stone(item_detail, (*it)["rune_stone"]);
			}

			// stone_nature
			if (!(*it)["stone_nature"].empty()) {
				refresh_item_gemstone(item_detail, (*it)["stone_nature"]);
			}

			set_map_second_value_by_key(key, item_config_, item_detail);

			Item_Effect_Type type = get_item_type(key);
			switch(type) {
			case ITEM_EFFECT_T_LOTTERY: {
				if ((*it)["usetype"].empty()) {
//					LOG_USER("lottery:%d empty", key);
				} else {
					Item_Lottery_Detail_Config item_lottery;
					item_lottery.item_id = key;
                    bool from_lottery = false;
					Item_Lottery_Item_Info_Config item_lottery_item_info;
					uint32_t array_size = (*it)["usetype"].size();
					for (uint32_t i = 0; i < array_size; ++i) {
						const Json::Value &temp_json = (*it)["usetype"][i];
						if ( !temp_json.isMember("from_table"))
							continue;
						std::string form_table = temp_json["from_table"].asString();
						if (form_table == "resource") {
							get_use_item_list(temp_json,key);
							continue;
						}
						if ( form_table != "lottery" )
							continue;
						else
							from_lottery = true;
						item_lottery_item_info.reset();
//						const Json::Value &temp_json = (*it)["usetype"][i];
						if (temp_json["level"] != Json::Value::null && temp_json["level"].size() == 2) {
							item_lottery_item_info.level_low = temp_json["level"][0u].asInt();
							item_lottery_item_info.level_high = temp_json["level"][1u].asInt();
							if (item_lottery_item_info.level_low > item_lottery_item_info.level_high) {
								LOG_ABORT("item_id: %d is error(level) in lottery.json.", key);
							}
						}

						item_lottery_item_info.count = temp_json["count"].asInt();
						if (0 == item_lottery_item_info.count) {
							item_lottery_item_info.count = 1;//未配置，默认为1
						}

						item_lottery_item_info.drate_rate = temp_json["drate_rate"].asInt();
						if (0 == item_lottery_item_info.drate_rate) {
							LOG_ABORT("error lottery %d : drate_rate = 0", key);
						}

						if (!temp_json["item_list"].isNull() &&
								temp_json["item_list"].isArray()) {
							Json_Utility::assign_item_vec(temp_json["item_list"], item_lottery_item_info.item_list);
						}
						if (!temp_json["item_list_ck"].isNull() &&
								temp_json["item_list_ck"].isArray()) {
							Json_Utility::assign_item_vec(temp_json["item_list_ck"], item_lottery_item_info.item_list_ck);
						} else {
							item_lottery_item_info.item_list_ck = item_lottery_item_info.item_list;
						}
						if (!temp_json["item_list_yss"].isNull() &&
								temp_json["item_list_yss"].isArray()) {
							Json_Utility::assign_item_vec(temp_json["item_list_yss"], item_lottery_item_info.item_list_yss);
						} else {
							item_lottery_item_info.item_list_yss = item_lottery_item_info.item_list;
						}

						if (!temp_json["weight"].isNull() &&
								temp_json["weight"].isArray()) {
							Json_Utility::assign_int_vec(temp_json["weight"], item_lottery_item_info.weight);
						}
						item_lottery_item_info.type = temp_json["type"].asInt();
						Json_Utility::assign_item_vec(temp_json["cost"], item_lottery_item_info.cost);

						if (item_lottery_item_info.weight.size() > 0
								&& item_lottery_item_info.item_list.size() != item_lottery_item_info.weight.size()) {
							LOG_ABORT("error item_lsit:%d,weight:%d", item_lottery_item_info.item_list.size(), item_lottery_item_info.weight.size());
						}

						if (!temp_json["airing_id"].isNull() &&
								temp_json["airing_id"].isArray() &&
								temp_json["airing_id"].size() >0) {
							for ( uint8_t i = 0;i < temp_json["airing_id"].size();++i) {
								item_lottery_item_info.item_set.insert(temp_json["airing_id"][i].asInt());
							}
						}

						item_lottery.item_list.push_back(item_lottery_item_info);
					}
					if ( from_lottery)
					    set_map_second_value_by_key(key, item_lottery_config_, item_lottery);
				}
			}
			break;
			case ITEM_EFFECT_T_DRUG: {
				if ((*it)["usetype"].empty()) {
					//LOG_USER("drug:%d empty", key);
				} else {
					Item_Drug_Detail item_drug;
					uint32_t array_size = (*it)["usetype"].size();
					for (uint32_t i = 0; i < array_size; ++i) {
						const Json::Value &temp_json = (*it)["usetype"][i];
						if ( !temp_json.isMember("from_table"))
							continue;
						std::string form_table = temp_json["from_table"].asString();
						if (form_table == "resource") {
							get_use_item_list(temp_json,key);
							continue;
						}
//						const Json::Value &temp_json = (*it)["usetype"][i];
						if (temp_json["status"] != Json::Value::null) {
							item_drug.val = temp_json["status"].asDouble();
						}
						if (temp_json["usetype"] != Json::Value::null) {
							item_drug.usetype = static_cast<int8_t>(temp_json["usetype"].asInt());
						}
						set_map_second_value_by_key(key, item_drug_cfg_, item_drug);
					}
//					set_map_second_value_by_key(key, item_drug_cfg_, item_drug);
				}
				break;
			}
			case ITEM_EFFECT_T_MATERIAL: {
				if (!(*it)["usetype"].empty()) {
					Item_Material_Detail item;
//					uint32_t array_size = (*it)["usetype"].size();
//					for (uint32_t i = 0; i < array_size; ++i) {
//						const Json::Value &temp_json = (*it)["usetype"][i];
//						if (temp_json["from_table"] != Json::Value::null) {
//							item.from_table = temp_json["from_table"].asString();
//							if (item.from_table == "resource") {
//								Item_Resource_Detail item_drug;
				    uint32_t array_size = (*it)["usetype"].size();
				    for (uint32_t i = 0; i < array_size; ++i) {
					    const Json::Value &temp_json = (*it)["usetype"][i];
					    if ( !temp_json.isMember("from_table"))
					    	continue;
					    std::string form_table = temp_json["from_table"].asString();
					    if (form_table == "resource") {
						    get_use_item_list(temp_json,key);
						    continue;
					    }
					    item.from_table = form_table;
					    set_map_second_value_by_key(key, item_material_cfg_, item);
//									const Json::Value &temp_json = (*it)["usetype"][i];
//									if (temp_json["status"] != Json::Value::null) {
//										item_drug.val = temp_json["status"].asDouble();
//									} else {
//										LOG_USER("resource: %d, val empty", key);
//										continue;
//									}
//									if (temp_json["prop"] != Json::Value::null) {
//										item_drug.prop = temp_json["prop"].asInt();
//									} else {
//										LOG_USER("resource: %d, prop empty", key);
//										continue;
//									}
				     }
//								set_map_second_value_by_key(key, item_resource_cfg_, item_drug);
//							} else {
//								set_map_second_value_by_key(key, item_material_cfg_, item);
//							}
				}
				break;
			}
			case ITEM_EFFECT_T_CARD: {
				if ((*it)["usetype"].empty()) {
					//LOG_USER("card:%d empty", key);
				} else {
					Item_Card_Detail item;
					uint32_t array_size = (*it)["usetype"].size();
					for (uint32_t i = 0; i < array_size; ++i) {
						const Json::Value &temp_json = (*it)["usetype"][i];
						if ( !temp_json.isMember("from_table"))
							continue;
						std::string form_table = temp_json["from_table"].asString();
						if (form_table == "resource") {
							get_use_item_list(temp_json,key);
							continue;
						}
						if (temp_json["deadline"] != Json::Value::null) {
							item.deadline = temp_json["deadline"].asInt();
						}
						if (temp_json["type"] != Json::Value::null) {
							item.covenant_type = temp_json["type"].asInt();
						}
						if (temp_json["vip_lvl"] != Json::Value::null) {
							item.vip_lvl = temp_json["vip_lvl"].asInt();
						}
						set_map_second_value_by_key(key, item_card_cfg_, item);
					}
//					set_map_second_value_by_key(key, item_card_cfg_, item);
				}
				break;
			}
			case ITEM_EFFECT_T_TASK:
			case ITEM_EFFECT_T_FLOWER:
			case ITEM_EFFECT_T_EXCHANGE:
			case ITEM_EFFECT_T_ACHIEVEMENT:
			case ITEM_EFFECT_T_MOUNT:
			case ITEM_EFFECT_T_GEM_COMPOSE:
			case ITEM_EFFECT_T_RESOURCE:
			case ITEM_EFFECT_T_EQUIP: {
				if ((*it)["usetype"].empty()) {
					//LOG_USER("resource:%d empty", key);
				} else {
					Item_Resource_Detail item_drug;
					uint32_t array_size = (*it)["usetype"].size();
					for (uint32_t i = 0; i < array_size; ++i) {
						const Json::Value &temp_json = (*it)["usetype"][i];
						if ( !temp_json.isMember("from_table") )
							continue;
						std::string form_table = temp_json["from_table"].asString();
						if (form_table == "resource") {
							get_use_item_list(temp_json,key);
							continue;
						}
//						if (temp_json["status"] != Json::Value::null) {
//							item_drug.val = temp_json["status"].asDouble();
//						} else {
//							LOG_USER("resource: %d, val empty", key);
//							continue;
//						}
//						if (temp_json["prop"] != Json::Value::null) {
//							item_drug.prop = temp_json["prop"].asInt();
//						} else {
//							LOG_USER("resource: %d, prop empty", key);
//							continue;
//						}
					}
//					set_map_second_value_by_key(key, item_resource_cfg_, item_drug);
				}
				break;
			}
			case ITEM_EFFECT_T_CONSUME_TOOL: {
				if (!(*it)["usetype"].empty()) {
					Item_Material_Detail item;
					uint32_t array_size = (*it)["usetype"].size();
					for (uint32_t i = 0; i < array_size; ++i) {
						const Json::Value &temp_json = (*it)["usetype"][i];
						if (temp_json["from_table"] != Json::Value::null) {
							if ( temp_json["from_table"] == "resource") {
								get_use_item_list(temp_json,key);
								continue;
							}
							item.from_table = temp_json["from_table"].asString();
							set_map_second_value_by_key(key, item_material_cfg_, item);
							break;
						} else {
							continue;
						}
					}
				}
				break;
			}
			case ITEM_EFFECT_T_LITTLE_FUN: {
				if (!(*it)["usetype"].empty()) {
					Item_Little_Fuction_Config item;
					uint32_t array_size = (*it)["usetype"].size();
					for (uint32_t i = 0; i < array_size; ++i) {
						const Json::Value &temp_json = (*it)["usetype"][i];
						if (temp_json["from_table"] != Json::Value::null) {
					        if ( temp_json["from_table"] == "resource") {
								get_use_item_list(temp_json,key);
								continue;
							}
//							item.from_table = temp_json["from_table"].asString();
                            if ( temp_json.isMember("type"))
                            	item.type = temp_json["type"].asUInt();
                            if ( temp_json.isMember("content"))
                            	item.arg = temp_json["content"].asInt();
                            if ( temp_json.isMember("content1"))
                                item.arg1 = temp_json["content1"].asInt();
							set_map_second_value_by_key(key, item_little_fuc_cfg_, item);
							break;
						} else {
							continue;
						}
					}
				}
				break;
			}
			default:
				break;
			}
		}
	}

}

void Config_Cache_Item::refresh_item_equip(Item_Detail_Config &item_detail, Json::Value &item_equip) {
	if (item_equip.empty()) {
		return;
	}
	item_detail.equip_cfg.career = item_equip["career"].asInt();
	item_detail.equip_cfg.part = item_equip["part"].asInt();
	item_detail.equip_cfg.equip_lvl = item_equip["equip_lvl"].asInt();
	item_detail.equip_cfg.max_unban_lvl = item_equip["max_unban_lvl"].asInt();
	item_detail.equip_cfg.suit_id = item_equip["suit_id"].asInt();
	item_detail.equip_cfg.hero_id = item_equip["heroID"].asInt();
	item_detail.equip_cfg.init_card_holes = item_equip["default_num"].asInt();
	item_detail.equip_cfg.card_holes = item_equip["check_num"].asInt();

	item_detail.equip_cfg.gemstone_holes.val_1 = item_equip["init"].asInt();
	item_detail.equip_cfg.gemstone_holes.val_2 = item_equip["max"].asInt();

	if (item_detail.equip_cfg.init_card_holes > Equip_Addon_Detail::MAX_CARD_HOLE ||
			item_detail.equip_cfg.card_holes > Equip_Addon_Detail::MAX_CARD_HOLE) {
		LOG_ABORT ("equip cfg error, card hole num, equip_id=%d, init=%d, max=%d",
				item_detail.item_id, item_detail.equip_cfg.init_card_holes, item_detail.equip_cfg.card_holes);
	}

	Int_Int_Map avatar;
	for (Json::Value::iterator it = item_equip["avatars"].begin(); it != item_equip["avatars"].end(); ++it) {
		item_detail.equip_cfg.avatar_map[(*it)[0u].asInt()][(*it)[1].asInt()] = (*it)[2].asInt();
	}

	Json_Utility::assign_int_vec(item_equip["skill"], item_detail.equip_cfg.skills);

	Prop_Value_Vec prop_value_vec;
	Int_Double prop_value;
	for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
		prop_value.reset();
		prop_value.id = *it_prop;
		std::ostringstream prop_stream;
		prop_stream << prop_value.id;
		if (Json::Value::null != item_equip[prop_stream.str()]) {
			prop_value.value = item_equip[prop_stream.str()].asDouble();
			item_detail.equip_cfg.prop_val_vec.push_back(prop_value);
		}
	}
}

void Config_Cache_Item::refresh_item_special(Item_Detail_Config &item_detail, Json::Value &item_special) {
	if (item_special.empty()) {
		return;
	}
	item_detail.special_cfg.lucky_stone_rate = item_special["lucky_stone_rate"].asInt();
	item_detail.special_cfg.rune_essence_energy = item_special["rune_essence_energy"].asInt();
	item_detail.special_cfg.type = item_special["type"].asInt();
	switch(item_detail.special_cfg.type) {
	case 1: {
		item_detail.special_cfg.addon.mount_skill = item_special["content"].asInt();
		break;
	}
	case 2: {
		if (item_special["content"].isArray() && item_special["content"].size() == 3) {
			item_detail.special_cfg.addon.iu_hero.id = item_special["content"][0u].asInt();
			item_detail.special_cfg.addon.iu_hero.wake_lvl = item_special["content"][1u].asInt();
			item_detail.special_cfg.addon.iu_hero.lvl = item_special["content"][2u].asInt();
		} else {
			MSG_TRACE_ABORT("ERROR: item_special type(%d), id(%d) config not correct!", item_detail.special_cfg.type, item_detail.item_id);
		}
		break;
	}
	default:
		break;
	}

	if (Json::Value::null != item_special["status_argv"] && item_special["status_argv"].isArray()) {
		item_detail.special_cfg.insert_hero = item_special["insert_hero"].asBool();
		item_detail.special_cfg.trigger_buff.status_id = item_special["status_argv"][0u].asInt();
		item_detail.special_cfg.trigger_buff.status_from = TYPE_SOURCE_BUFF;
		//读取skill_val1
		Skill_Effect_Val_Inner inner;
		if (!item_special["status_argv"][1].isNull() &&
				item_special["status_argv"][1].isArray()) {
			Json_Utility::assign_int_vec(item_special["status_argv"][1], item_detail.special_cfg.trigger_buff.disappear_gap);
		}
		if (!item_special["status_argv"][2].isNull() &&
				item_special["status_argv"][2].isArray()
				&& !item_special["status_argv"][2].empty()) {
			if (item_special["status_argv"][2][0u].isArray()) {
				uint16_t len = item_special["status_argv"][2].size();
				for (uint16_t i = 0; i < len; ++i) {
					const Json::Value &array_json = item_special["status_argv"][2][i];
					inner.reset();
					if (!array_json.isNull() && array_json.size() > 0) {
						int id = array_json[0u].asInt();
						double value = array_json[1u].asDouble();
						CONFIG_CACHE_FIGHTER->get_attr_change_data(id, inner, value);
						if (0 == inner.val1) { //伤害链\经验加成
							inner.val1 = id;
							inner.val4 = value;
						}
						if (3 == item_special["status_argv"][2].size()) {
							double value2 = item_special["status_argv"][2][2].asDouble();
							inner.val7 = value2;//目前使用作治疗的下限值
						}
					} else {
						inner.val1 = -1;
					}

					Effect_Argv_Prop prop;
					prop.i_effect_val1 = inner.val1;
					prop.d_effect_val1 = inner.val4;
					prop.d_effect_val2 = inner.val5;
					prop.d_effect_val3 = inner.val6;
					prop.d_effect_val4 = inner.val7;
					item_detail.special_cfg.trigger_buff.data.push_back(prop);
				}
			} else {
				inner.reset();
				int id = item_special["status_argv"][2][0u].asInt();
				double value = item_special["status_argv"][2][1u].asDouble();
				CONFIG_CACHE_FIGHTER->get_attr_change_data(id, inner, value);
				if (0 == inner.val1) { //伤害链\经验加成
					inner.val1 = id;
					inner.val4 = value;
				}
				if (3 == item_special["status_argv"][2].size()) {
					double value2 = item_special["status_argv"][2][2].asDouble();
					inner.val7 = value2;//目前使用作治疗的下限值
				}

				Effect_Argv_Prop prop;
				prop.i_effect_val1 = inner.val1;
				prop.d_effect_val1 = inner.val4;
				prop.d_effect_val2 = inner.val5;
				prop.d_effect_val3 = inner.val6;
				prop.d_effect_val4 = inner.val7;
				item_detail.special_cfg.trigger_buff.data.push_back(prop);
			}
		}
	}
}

void Config_Cache_Item::refresh_item_rune_stone(Item_Detail_Config &item_detail_cfg, Json::Value &rune_stone) {
	if (rune_stone.empty()) {
		return;
	}

	item_detail_cfg.rune_stone.rune_type = rune_stone["rune_type"].asInt();
	item_detail_cfg.rune_stone.need_points = rune_stone["need_points"].asInt();
	item_detail_cfg.rune_stone.combine_id = rune_stone["combine_id"].asInt();
	Json_Utility::assign_item_vec(rune_stone["need_id"], item_detail_cfg.rune_stone.need_id);

	Int_Double prop_value;
	for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
		prop_value.reset();
		prop_value.id = *it_prop;
		std::ostringstream prop_stream;
		prop_stream << prop_value.id;
		const Json::Value& jv_prop_stream = rune_stone[prop_stream.str()];
		if (Json::Value::null != jv_prop_stream) {
			prop_value.value = jv_prop_stream.asDouble();
			item_detail_cfg.rune_stone.prop_val_map.insert(std::make_pair(prop_value.id, prop_value.value));
		}
	}
}

void Config_Cache_Item::refresh_quick_buy(void) {
	const Json::Value& item_json = CONFIG_INSTANCE->config_json()["item"]["quick_buy"];
	quick_buy_cfg_.clear();
	Quick_Buy quick_buy;

	for (Json::Value::iterator it = item_json.begin();
			it != item_json.end(); ++it) {
		int key = (*it)["id"].asInt();
		const Item_Detail_Config *idc = find_item(key);
		if (NULL == idc) {
			continue;
		}
		quick_buy.reset();
		quick_buy.id = key;
		quick_buy.price = (*it)["price"].asInt();
		quick_buy.type = (*it)["type"].asInt();
		set_map_second_value_by_key(key, quick_buy_cfg_, quick_buy);
	}
}

void Config_Cache_Item::refresh_item_gain_source(void) {
	const Json::Value& item_json = CONFIG_INSTANCE->config_json()["item"]["item_gain_source"];
	item_gain_source_cfg_.clear();
	Item_Gain_Source_Detail quick_buy;

	for (Json::Value::iterator it = item_json.begin();
			it != item_json.end(); ++it) {
		quick_buy.reset();
		quick_buy.type = (*it)["type"].asInt();
		quick_buy.name = (*it)["name"].asString();
		set_map_second_value_by_key(quick_buy.type, item_gain_source_cfg_, quick_buy);
	}
}

void Config_Cache_Item::refresh_item_gemstone(Item_Detail_Config &item_detail,
		Json::Value &gemstone) {

	if (gemstone.empty()) return;
	item_detail.gemstone_cfg.type = gemstone["stone_type"].asInt();
	item_detail.gemstone_cfg.lv = gemstone["stone_lv"].asInt();
	if (gemstone["stone_nature"].empty() || !gemstone["stone_nature"].isArray()) {
		LOG_ABORT("equip gemstone attribute error, id=%d", item_detail.item_id);
		return;
	}
	item_detail.gemstone_cfg.attr.id = gemstone["stone_nature"][0u].asInt();
	item_detail.gemstone_cfg.attr.value= gemstone["stone_nature"][1].asDouble();
}

void Config_Cache_Item::refresh_gemstone_base(void) {

	const Json::Value& cfg = CONFIG_INSTANCE->config_json()["equip"]["stone_base"];

	gemstone_hole_cfg_.clear();
	Int_Int value;
	for (Json::Value::const_iterator it = cfg.begin(); it != cfg.end(); ++it) {
		value.reset();
		int color = (*it)["equip_color"].asInt();
		value.val_1 = (*it)["initial_hole"].asInt();
		value.val_2 = (*it)["limit_hole"].asInt();
		if (value.val_1 > Equip_Addon_Detail::MAX_GEMSTONE_HOLE ||
				value.val_2 > Equip_Addon_Detail::MAX_GEMSTONE_HOLE) {
			LOG_ABORT("hole number error, color=%d, initial hole=%d, max hole=%d",
					color, value.val_1, value.val_2);
		}
		gemstone_hole_cfg_[color] = value;
	}
}

void Config_Cache_Item::refresh_gemstone_msic(void) {

	const Json::Value& cfg = CONFIG_INSTANCE->config_json()["equip"]["stone_other"];
	gemstone_misc_cfg_.clear();
	for (Json::Value::const_iterator it = cfg.begin(); it != cfg.end(); ++it) {
		int type = (*it)["type"].asInt();
		if (1 != type && 2 != type) continue;
		gemstone_misc_cfg_[type] = (*it)["content"].asInt();
	}
}

void Config_Cache_Item::refresh_gemstone_conversion(void) {

	const Json::Value& cfg = CONFIG_INSTANCE->config_json()["equip"]["stone_change"];
	gemstone_conversion_cfg_.clear();
	for (Json::Value::const_iterator it = cfg.begin(); it != cfg.end(); ++it) {
		gemstone_conversion_cfg_[(*it)["stone_lv"].asInt()] = (*it)["consume"].asInt();
	}
}

void Config_Cache_Item::refresh_gemstone_ext_attr(void) {

	const Json::Value& cfg = CONFIG_INSTANCE->config_json()["equip"]["stone_award"];
	gemstone_ext_attr_cfg_.clear();
	Gemstone_Ext_Attr ext_attr;
	for (Json::Value::const_iterator it = cfg.begin(); it != cfg.end(); ++it) {
		ext_attr.reset();
		ext_attr.color = (*it)["stone_award"][0u].asInt();
		ext_attr.num = (*it)["stone_award"][1].asInt();

		for (Int_Set::const_iterator it_prop = FIGHT_PROP_SET.begin(); it_prop != FIGHT_PROP_SET.end(); ++it_prop) {
			int id = *it_prop;
			std::ostringstream prop_stream;
			prop_stream << id;
			if (Json::Value::null == (*it)[prop_stream.str()]) continue;
			double value = (*it)[prop_stream.str()].asDouble();
			ext_attr.attr_map[id] = value;
		}

		gemstone_ext_attr_cfg_.push_back(ext_attr);
	}
}

void Config_Cache_Item::refresh_gem_carve_prop(void) {
	gem_carve_prop_map_.clear();
	Gem_Carve_Prop gem_carve_cfg;
	gem_carve_cfg.carve_lvl = 0;
	gem_carve_cfg.gem_stats = 1.0;
	set_map_second_value_by_key(gem_carve_cfg.carve_lvl, gem_carve_prop_map_, gem_carve_cfg);

	const Json::Value &json = CONFIG_INSTANCE->config_json()["equip"]["stone_sculpture"];
	for (Json::Value::iterator it_json = json.begin(); it_json != json.end(); ++it_json) {
		gem_carve_cfg.reset();

		gem_carve_cfg.carve_lvl = (*it_json)["level"].asInt();
		ABORT_INT_ZERO(gem_carve_cfg.carve_lvl);
		gem_carve_cfg.gem_stats = (*it_json)["Stats"].asInt()/100.0 + 1;
		gem_carve_cfg.carve_max = (*it_json)["max"].asInt();
		gem_carve_cfg.broadcast_id = (*it_json)["broadcast_id"].asInt();
		if (!(*it_json)["item_base"].empty() && (*it_json)["item_base"].isArray()) {
			gem_carve_cfg.item_base.val_1 = (*it_json)["item_base"][0u].asInt();
			gem_carve_cfg.item_base.val_2 = (*it_json)["item_base"][1].asInt();
			}

		set_map_second_value_by_key(gem_carve_cfg.carve_lvl, gem_carve_prop_map_, gem_carve_cfg);
	}
	int max = 0;
	for (Gem_Carve_Prop_Map::iterator iter = gem_carve_prop_map_.begin(); iter != gem_carve_prop_map_.end(); ++iter) {
		if(iter->first > max) {
			max = iter->first;
		}
	}
	carve_max_ = max;
}

const int Config_Cache_Item::get_gemstone_conversion_cost(int lv)  {
	const int *cost = get_map_second_pointer_by_key(lv, gemstone_conversion_cfg_);
	if (0 == cost)  return 0;
	return *cost;
}

const Int_Int *Config_Cache_Item::get_gemstone_hole_info(int color) {
	return get_map_second_pointer_by_key(color, gemstone_hole_cfg_);
}

const int Config_Cache_Item::get_gemstone_umembed_cost(void) {
	const int *cost = get_map_second_pointer_by_key(1, gemstone_misc_cfg_);
	if (0 == cost)  return 0;
	return *cost;
}

const int Config_Cache_Item::get_gemstone_open_hole_item_id(void) {
	const int *cost = get_map_second_pointer_by_key(2, gemstone_misc_cfg_);
	if (0 == cost)  return 0;
	return *cost;
}

const Gemstone_Ext_Attr_Config& Config_Cache_Item::get_gemstone_ext_attr(void) {
	return gemstone_ext_attr_cfg_;
}

const Gem_Carve_Prop *Config_Cache_Item::get_gemstone_carve_propo(int lv) const {
	return get_map_second_pointer_by_key(lv, gem_carve_prop_map_);
}

int Config_Cache_Item::get_gemstone_carve_propo_max(void) const {
	return carve_max_;
}
