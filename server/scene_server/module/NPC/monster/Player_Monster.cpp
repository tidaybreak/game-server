/*
 * Player_Monster.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */


#include "Player_Monster.h"
#include "NPC/NPC_Manager.h"
#include "Config_Cache_NPC.h"
#include "Msg_Active_Enum.h"
#include "Msg_Active_Struct.h"
#include "Scene_Public.h"
#include "Pool_Manager.h"
#include "Config_Cache_Fighter.h"


Player_Monster::Player_Monster(void):career_(0), gender_(0), hero_role_id_(0) {
	reset();
}

Player_Monster::~Player_Monster(void) {

}


int Player_Monster::reset(void) {
	Monster::reset();
	hero_role_id_ = 0;
	return 0;
}


int Player_Monster::battle_birth(role_id_t npc_id, int npc_type_id, NPC_Record *record, NPC_Addition_Info &npc_addition_info,
		Battle_Scene* battle_scene, Block_Buffer& buf) {

	if (battle_scene == NULL) {
		MSG_USER("player_monster birth parm error");
		return -1;
	}

	npc_detail_.npc_id = npc_id;
	int ret = this->battle_init(npc_type_id, record, npc_addition_info, battle_scene, buf);
	if (ret != 0) {
		reset();
		return -1;
	}
	set_battle_position(npc_addition_info.birth_coord.x, npc_addition_info.birth_coord.y, battle_scene);
	return 0;
}


int Player_Monster::battle_init(int npc_type_id, NPC_Record *record, NPC_Addition_Info
		&npc_addition_info, Battle_Scene* battle_scene, Block_Buffer& buf) {
	const NPC_Config_Cache *config_cache = CONFIG_CACHE_NPC->find_npc_config_cache(npc_type_id);
	if (config_cache == 0) {
		MSG_USER("find npc config:%d error", npc_type_id);
		return -1;
	}
	monster_detail_.config_cache = config_cache;

	/// npc base init
	npc_record_ = record;
	npc_detail_.npc_status = NORMAL_STATUS;
	npc_detail_.birth_time = Time_Value::gettimeofday();
	npc_detail_.npc_type_id = npc_type_id;
	if(record){
		npc_detail_.state_type = record->config.state_type;
	}
	npc_detail_.birth_coord = npc_addition_info.birth_coord;


	/// monster base init

	monster_detail_.name = npc_addition_info.name.empty() ? config_cache->name : npc_addition_info.name;
	monster_detail_.jid = npc_addition_info.jid ? npc_addition_info.jid : config_cache->jid;
	career_ = npc_addition_info.ex_val1;
	gender_ = npc_addition_info.ex_val2;

	/// monster mover init
	npc_detail_.mover_detail_.mover_type = MOVER_T_PLAYER_MONSTER;
	Mover::set_detail_prt(npc_detail_.mover_detail_);

	/// monster fighter init
	Fighter::set_detail_prt(npc_detail_.fighter_detail_);
	this->load_detail(buf);
	this->restore_hp(this->blood_max(), false);
	npc_detail_.fighter_detail_.role_id = npc_id();
	this->set_clear_label(true);

//	//skill
//	int skill_count = config_cache->skill_vec.size();
//
//	Talent_Page page;
//	page.reset();
//	page.page_id = 1;
//
//	if(skill_count > 0)
//	{
//		npc_detail_.fighter_detail_.default_skill = config_cache->skill_vec[0];
//		Skill_DB_Info skill_info;
//		for(int i=0; i<skill_count; ++i)
//		{
//			skill_info.skill_id = config_cache->skill_vec[i];
//			skill_info.skill_level = 1;
//			//npc_detail_.fighter_detail_.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//			page.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//		}
//	}
//	//test for debug lys
//	for(int i=0; i<11; ++i){
//		Skill_DB_Info skill_info;
//		skill_info.skill_id = 31000101+i*100;
//		skill_info.skill_level = 1;
//		//npc_detail_.fighter_detail_.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//		page.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//	}
//	for(int i=0; i<6; ++i){
//		Skill_DB_Info skill_info;
//		skill_info.skill_id = 31001401+i*100;
//		skill_info.skill_level = 1;
//		//npc_detail_.fighter_detail_.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//		page.skill_db_info_map.insert(std::make_pair(skill_info.skill_id,skill_info));
//	}
//	npc_detail_.fighter_detail_.talent_page_map[page.page_id] = page;
//	npc_detail_.fighter_detail_.cur_used_talent_page = page.page_id;

	return 0;
}

int Player_Monster::tick(Time_Value &now) {
	Monster::tick(now);
	return 0;
}
void Player_Monster::fight_tick(const Time_Value &now) {
	Fighter::fight_tick(now);
	Fighter::time_up(now);
}

bool Player_Monster::is_monster(void){
	return false;
}

Monster *Player_Monster::monster_self(void){
	return 0;
}

bool Player_Monster::is_player_monster(void) {
	return true;
}

Player_Monster *Player_Monster::player_monster_self(void){
	return this;
}

int Player_Monster::battle_appear_info(Block_Buffer &buf){
	// size_t rd_idx = buf.get_read_idx();
	size_t wr_begin_idx = buf.get_write_idx();
	size_t wr_end_idx = 0;
	uint32_t len = 0;

	// fill head
	buf.make_message(ACTIVE_FIGHTER_APPEAR);

	MSG_81000008 msg;
	msg.fighter_info.role_id = role_id();
	msg.fighter_info.role_name = role_name();
	msg.fighter_info.career = career_;
	msg.fighter_info.gender = gender_;
	msg.fighter_info.position.val_1 = mover_detail().battle_position.pos;
	msg.fighter_info.position.val_2 = mover_detail().battle_position.idx;
	msg.fighter_info.hp_curr = blood_current();
	msg.fighter_info.hp_max = blood_max();
	msg.fighter_info.avatar = avatar_vec;
	boost::unordered_set<Property_Type> property_set;
	property_set.insert(PT_MORALE_CAPS);
	property_set.insert(PT_MORALE_CURRENT);

	for (boost::unordered_set<Property_Type>::iterator it = property_set.begin(); it != property_set.end(); ++it) {
		msg.fighter_info.init_props.push_back(Prop_Change(*it));
	}

	for (std::vector<Prop_Change>::iterator pc_it = msg.fighter_info.init_props.begin();
			pc_it != msg.fighter_info.init_props.end(); ++pc_it) {
		(*pc_it).current_val = fighter_detail().fetch_fight_property(Property_Type((*pc_it).prop_type));
	}

	msg.serialize(buf);

	// refill len in head
	wr_end_idx = buf.get_write_idx();
	len = wr_end_idx - wr_begin_idx - sizeof(uint32_t);
	buf.set_write_idx(wr_begin_idx);
	buf.write_uint32(len);
	buf.set_write_idx(wr_end_idx);

	return 0;
}

void Player_Monster::player_pos_key_fit_hero(Position_Key &player_pos_key, Position_Key &hero_pos_key) {
	// 英雄前排 hero_formation == 5
	if (hero_pos_key.idx == 5) {
		switch (player_pos_key.idx) {
		case 4:
			player_pos_key.idx = 7;
			hero_pos_key.idx = 4;
			break;
		case 5:
			player_pos_key.idx = 8;
			break;
		case 6:
			player_pos_key.idx = 9;
			hero_pos_key.idx = 6;
			break;
		case 7:
			hero_pos_key.idx = 4;
			break;
		//case 8:
		case 9:
			hero_pos_key.idx = 6;
			break;
		default:
			break;
		}

	// 英雄后排 hero_formation == 8
	} else {
		switch (player_pos_key.idx) {
		case 4:
			hero_pos_key.idx = 7;
			break;
		//case 5:
		case 6:
			hero_pos_key.idx = 9;
			break;
		case 7:
			player_pos_key.idx = 4;
			hero_pos_key.idx = 7;
			break;
		case 8:
			player_pos_key.idx = 5;
			hero_pos_key.idx = 8;
			break;
		case 9:
			player_pos_key.idx = 6;
			hero_pos_key.idx = 9;
			break;
		default:
			break;
		}
	}
}

int Player_Monster::hero_battle_birth(Block_Buffer& buf, Battle_Scene* scene) {
	int8_t has_fight_hero = 0;
	buf.read_int8(has_fight_hero);
	if (has_fight_hero) {
		Hero_Base_Detail base_detail;
		base_detail.reset();
		base_detail.deserialize(buf);

		Hero *hero = NPC_MANAGER->create_hero_in_battle_scene(scene, Coord(0,0));
		if (hero) {
			base_detail.hero_role_id = hero->role_id();
			//SCENE_PUBLIC->bind_master_id_hero(detail.master_role_id, *hero);

			hero->refresh_hero_base_detail(base_detail);

			Fighter_Detail fighter_detail;
			fighter_detail.reset();
			fighter_detail.deserialize(buf);
			hero->refresh_hero_fighter_detail(fighter_detail);

			Position_Key player_pos_key = mover_detail().battle_position;
			Position_Key hero_pos_key(player_pos_key.pos, hero->hero_const_base_detail().hero_formation);

			player_pos_key_fit_hero(player_pos_key, hero_pos_key);
			this->set_battle_position(player_pos_key.pos, player_pos_key.idx, scene);
			hero->set_pos_key(hero_pos_key.pos, hero_pos_key.idx);
			// birth
			const Coord &player_coord = mover_detail().grid_coord;
			hero->set_battle_ai();
			if (0 != hero->battle_birth(hero->hero_const_base_detail().hero_role_id, scene,	player_coord)) {
				NPC_MANAGER->destroy_npc(hero);
				return -1;
			}
			hero_role_id_ = hero->role_id();
			hero->hero_base_detail().master_role_id = role_id();
			NPC_MANAGER->battle_scene_npc_map()[hero->role_id()] = hero;
		}else{
			return -1;
		}
	}

	return 0;
}

void Player_Monster::floating_skill_trigger(const Skill_Config& cfg) {
	if (Skill_Type::TYPE_FLOAT_SKILL != cfg.skill_type) return;

	Hero *hero = find_hero();
	if (0 == hero) return;
	Talent_Map::const_iterator find_it = hero->fighter_detail().talent_map.find(hero->fighter_detail().cur_used_talent_);
	if (find_it == hero->fighter_detail().talent_map.end()) return;
	for (Skill_DB_Info_Map::const_iterator it = find_it->second.skill_db_info_map.begin();
			it != find_it->second.skill_db_info_map.end(); ++it) {
		const Skill_Config *cfg = CONFIG_CACHE_FIGHTER->skill_config_cache(it->second.skill_id);
		if (0 == cfg) continue;
		if (Skill_Type::TYPE_ADD_SKILL != cfg->skill_type) continue;
		hero->use_skill(cfg->skill_id);
		return;
	}
}

Hero* Player_Monster::find_hero(void){
	Hero* hero = NPC_MANAGER->find_hero(hero_role_id_);
	return hero;
}
int Player_Monster::battle_enter_appaer(Battle_Scene* scene){
	if (!enter_battle_scene(scene))
		battle_appear();
	return 0;
}
