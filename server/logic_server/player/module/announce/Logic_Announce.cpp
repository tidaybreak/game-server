/*
 * Logic_Announce.cpp
 *
 *  Created on: May 12, 2014
 *      Author: caijianwei
 */

#include "Logic_Announce.h"
#include "Msg_Inner_Struct.h"
#include "announcement/Announcement_Struct.h"
#include "Msg_Struct.h"
#include "Logic_Player.h"
#include "Logic_Monitor.h"
#include "Logic_Player_Struct.h"
#include "Config_Cache_Role.h"
#include "gang/Gang_Struct.h"
#include "Config_Cache_Gang.h"

Logic_Announce::Logic_Announce() { }

Logic_Announce::~Logic_Announce() { }

void Logic_Announce::announce_send() {
	if ( district == 0 ) return ;
	MSG_20500000 msg;
	msg.district = district;
	msg.announce_id = announce_id;
	msg.gang_id = gang_id;
	msg.contents = ann_contents;
	msg.recievers = recievers;
	Block_Buffer buf__;
	buf__.make_message(msg.msg_id);
	msg.serialize(buf__);
	buf__.finish_message();
	this->send_to_local_chat(buf__);
}

void Logic_Announce::create_gang_announce(int64_t gang_id, const std::string& gang_name) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_CREATE_GANG);
	Logic_Player* self = player_self();
	Role_Info role_info;
	role_info.career = career();
	role_info.combat = self->fighter_detail().force;
	role_info.gender = self->base_detail().gender;
	role_info.level  = level();
	role_info.role_id = role_id();
	role_info.role_name = role_name();
	role_info.vip_level = self->vip();
	role_info.guild_name = gang_name;

	announce_push_role( role_info );
	announce_push_text( gang_name );
	announce_send();
}

void Logic_Announce::create_arena_rise_announce(const std::string& group_name){
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_ARENA_RISE);
	Logic_Player* self = player_self();
	Role_Info role_info;
	role_info.career = career();
	// TODO fill combat value
	role_info.combat = 0;
	role_info.gender = self->base_detail().gender;
	role_info.level  = level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = role_name();
	role_info.vip_level = self->vip();
	role_info.guild_name = self->base_detail().gang_name;

	announce_push_role( role_info );
	announce_push_text( group_name );
	announce_send();
}

void Logic_Announce::create_handsel_flowers_announce(const int32_t ann_id, const role_id_t role_id, const Chat_Item_Info& item) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ann_id);
	{
		Logic_Player* self = player_self();
		Role_Info role_info;
		role_info.career = career();
		role_info.gender = self->base_detail().gender;
		role_info.level  = level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = role_name();
		role_info.vip_level = self->vip();
		role_info.guild_name = self->base_detail().gang_name;
		announce_push_role( role_info );
	}
	{
		//announce_push_item(item);
	}
	{
		DB_View::ID_Player_View_Map::iterator iter = LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if (iter == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			MSG_USER("player(role_id=%ld) is not in db_view", role_id);
			return;
		}
		Role_Info role_info;
		role_info.career = iter->second->career;
		role_info.gender = iter->second->gender;
		role_info.level  = iter->second->level;
		role_info.role_id = iter->second->role_id;
		role_info.role_name = iter->second->role_name;
		role_info.vip_level = iter->second->vip();
		role_info.guild_name = iter->second->gang_name;
		announce_push_role( role_info );
	}
	announce_send();
}

void Logic_Announce::create_vip_turntable_announce(const Chat_Item_Info& item) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_VIP_TURNTABLE);
	{
		Logic_Player* self = player_self();
		Role_Info role_info;
		role_info.career = career();
		role_info.gender = self->base_detail().gender;
		role_info.level  = level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = role_name();
		role_info.vip_level = self->vip();
		role_info.guild_name = self->base_detail().gang_name;
		announce_push_role( role_info );
	}
	{
		announce_push_item(item);
	}
	announce_send();
}

void Logic_Announce::create_ann_expedition_defend(const int64_t gang_id, const int point_id, const std::string &name, const std::string &name_2) {
//	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_EXPEDITION_DEFEND, gang_id);
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}
	{
		std::stringstream ss;
		ss << name_2;
		announce_push_text(ss.str());
	}
	announce_push_prop_id(point_id);
//	{
//		Role_Info role_info;
//		role_info.career = player_self()->career();
//		role_info.combat = 0;
//		role_info.gender = player_self()->base_detail().gender;
//		role_info.level  = player_self()->level();
//		role_info.role_id = player_self()->role_id();
//		role_info.role_name = player_self()->role_name();
//		role_info.vip_level = player_self()->vip();
//		role_info.guild_name = player_self()->base_detail().gang_name;
//		announce_push_role(role_info);
//	}
	announce_send();
}

void Logic_Announce::create_ann_rune_stone_smelt(const Chat_Item_Info& chat_item_info) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_RUNE_STONE_SMELT);
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;

	announce_push_role(role_info);
	announce_push_item(chat_item_info);
	announce_send();
}

void Logic_Announce::create_ann_active_content_vip_addnum(const uint32_t vip_level, const uint32_t num) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_VIP_CLUB_NUM);
	std::string level;
	std::string number;
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", vip_level);
		printf("%s", buf);
		level = buf;
	}
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", num);
		printf("%s", buf);
		number = buf;
	}

	announce_push_text(level);
	announce_push_text(number);
	announce_send();
}

void Logic_Announce::create_ann_active_content_guild_level_up(const uint32_t level) {
	make_announce(ANNOUNCE_DISTRICT_GANG, ANNOUNCE_ID_ACTIVE_GANG_LEVEL, player_self()->ganger_detail().gang_id);
	std::string guild_level;
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", level);
		printf("%s", buf);
		guild_level = buf;
	}
	announce_push_text(guild_level);
	announce_send();
}

void Logic_Announce::create_ann_equip_enchant(const int prop, const int val, const int star) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_EQUIP_ENCHANT);

	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	std::stringstream ss;
	ss << star;
	announce_push_text(ss.str());

	announce_push_prop_id(prop);

	ss.clear();
	ss.str("");
	ss << val;
	if (M_RACE_FIGHT_PROPS.count(prop)) {
		ss << "%";
	}
	announce_push_text(ss.str());

	announce_send();
}

void Logic_Announce::create_ann_gang_draw(const Chat_Item_Info& chat_item_info) {
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_DRAW, (long int)player_self()->gang_id());

	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;

	announce_push_role(role_info);
	announce_push_item(chat_item_info);
	announce_send();
}

void Logic_Announce::create_ann_rune_fill_energy(const int prop, const int level) {
	int ann_id = 0;
	switch (prop) {
	case PT_POWER:
		ann_id = ANN_ID_RUNE_FILL_ENERGY_1;
		break;
	case PT_ARMOR:
		ann_id = ANN_ID_RUNE_FILL_ENERGY_2;
		break;
	case PT_STAMINA:
		ann_id = ANN_ID_RUNE_FILL_ENERGY_3;
		break;
	default:
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ann_id);

	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	std::stringstream ss;
	ss << level;
	announce_push_text(ss.str());

	announce_send();
}

void Logic_Announce::create_ann_artifact_upgrade(const std::string& belonger_name, const std::string& artifact_name, const int step, const int level) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_ARTIFACT_UPGRADE);

	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	announce_push_text(belonger_name);
	announce_push_text(artifact_name);

	std::stringstream ss;
	ss << step;
	announce_push_text(ss.str());

	ss.clear();
	ss.str("");
	ss << level;
	announce_push_text(ss.str());

	announce_send();
}

void Logic_Announce::create_ann_init_open_vip4(void) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_INIT_OPEN_VIP4);

	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	announce_send();
}

void Logic_Announce::create_ann_open_covenant(const int32_t ann_id) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ann_id);
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	announce_send();
}

void Logic_Announce::create_ann_open_all_covenant(void) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_OPEN_ALL_COVENANT);
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	announce_send();
}

void Logic_Announce::create_ann_buy_promo_goods(const Chat_Item_Info& item) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_BUY_PROMO_GOODS);
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	announce_push_item(item);
	announce_send();
}

void Logic_Announce::create_ann_handsel_covenant(Logic_Player *player,const int32_t ann_id) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ann_id);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		Role_Info role_info;
		role_info.career = player->career();
		role_info.combat = 0;
		role_info.gender = player->base_detail().gender;
		role_info.level  = player->level();
		role_info.role_id = player->role_id();
		role_info.role_name = player->role_name();
		role_info.vip_level = player->vip();
		role_info.guild_name = player->base_detail().gang_name;
		announce_push_role(role_info);
		announce_push_role(role_info);
	}
	announce_send();
}

void Logic_Announce::create_ann_handsel_all_covenant(Logic_Player *player) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_ID_HANDSEL_ALL_COVENANT);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		Role_Info role_info;
		role_info.career = player->career();
		role_info.combat = 0;
		role_info.gender = player->base_detail().gender;
		role_info.level  = player->level();
		role_info.role_id = player->role_id();
		role_info.role_name = player->role_name();
		role_info.vip_level = player->vip();
		role_info.guild_name = player->base_detail().gang_name;
		announce_push_role(role_info);
	}
	announce_send();
}

void Logic_Announce::create_ann_dragon_evolution_success(int64_t role_id, const std::string &dragon_name) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANNOUNCE_DRAGON_EVOLUTION_SUCCESS);
	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);

	Role_Info role_info;

	if(player) {
		role_info.career = player->career();
		role_info.combat = 0;
		role_info.gender = player->base_detail().gender;
		role_info.level  = player->level();
		role_info.role_id = player->role_id();
		role_info.role_name = player->role_name();
		role_info.vip_level = player->vip();
		role_info.guild_name = player->base_detail().gang_name;
	}
	else {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			return;
		}

		if(!find_it->second) {
			return;
		}

		role_info.career = find_it->second->career;
		role_info.combat = 0;
		role_info.gender = find_it->second->gender;
		role_info.level = find_it->second->level;
		role_info.role_id = find_it->second->role_id;
		role_info.role_name = find_it->second->role_name;
		role_info.vip_level = find_it->second->vip();
		role_info.guild_name = find_it->second->gang_name;
	}

	announce_push_role(role_info);
	announce_push_text(dragon_name);
	announce_send();
}

void Logic_Announce::create_ann_title(Logic_Player *player, const std::string &name, const std::string &title) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_TITLE);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}
	{
		std::stringstream ss;
		ss << title;
		announce_push_text(ss.str());
	}
	announce_send();
}
void Logic_Announce::create_ann_title_sign_in(Logic_Player *player, const int title_id, const Role_Vec &role_vec) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, title_id);
	announce_push_recievers(role_vec);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	announce_send();
}

void Logic_Announce::create_ann_achievement(Logic_Player *player, const std::string &name, const std::string &title, const uint32_t achi_id) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_ACHIEVEMENT);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}
	{
		std::stringstream ss;
		ss << title;
		announce_push_text(ss.str());
	}
	{
		announce_push_prop_id(achi_id);
	}
	announce_send();
}

void Logic_Announce::create_ann_gang_war(void) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_GANG_WAR);
	announce_send();
}
void Logic_Announce::create_ann_equip_master(Logic_Player *player, const uint32_t index, const int lvl) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_MASTER);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	int index_l = index % 10;
	const std::string *st = CONFIG_CACHE_GANG->find_part_string(index_l);
	std::string name = "";
	if (st) {
		name = *st;
	}
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}
	{
		std::stringstream ss;
		ss << lvl;
		announce_push_text(ss.str());
	}
	announce_send();
}

void Logic_Announce::create_ann_team_sports(void) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_TEAM_SPORTS);
	announce_send();
}

void Logic_Announce::create_ann_recharge_rebate(const int32_t ann_id) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ann_id);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	announce_send();
}

void Logic_Announce::create_ann_add_auction_goods(const int32_t ann_id, const Chat_Item_Info& item, const uint32_t price) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ann_id);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		announce_push_item(item);
	}
	{
		std::stringstream ss;
		ss << price;
		announce_push_text(ss.str());
	}
	announce_send();
}

void Logic_Announce::announce_push_role_view(Player_DB_View* player_db_view) {
	if (! player_db_view) {
		return;
	}
	Role_Info role_info;
	role_info.career = player_db_view->career;
	role_info.combat = player_db_view->force;
	role_info.gender = player_db_view->gender;
	role_info.level  = player_db_view->level;
	role_info.role_id = player_db_view->role_id;
	role_info.role_name = player_db_view->role_name;
	role_info.vip_level = player_db_view->vip();
	role_info.guild_name = player_db_view->gang_name;
	announce_push_role(role_info);
}

void Logic_Announce::create_ann_gang_join_in(const role_id_t role_id) {
	DB_View::ID_Player_View_Map::iterator player_view =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (player_view == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_JOIN_IN, player_view->second->gang_id);
	announce_push_role_view(player_view->second);
	announce_send();
}

void Logic_Announce::create_ann_gang_headship_appoint(const role_id_t role_id_1, const role_id_t role_id_2, const int headship) {
	DB_View::ID_Player_View_Map::iterator player_view_1 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_1);
	if (player_view_1 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	DB_View::ID_Player_View_Map::iterator player_view_2 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_2);
	if (player_view_2 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}

	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_HEADSHIP_APPOINT, player_view_1->second->gang_id);
	announce_push_role_view(player_view_1->second);
	announce_push_role_view(player_view_2->second);
	announce_push_text(CONFIG_CACHE_GANG->power(headship).name);
	announce_send();
}

void Logic_Announce::create_ann_gang_quit(const role_id_t role_id, const gang_id_t gang_id) {
	DB_View::ID_Player_View_Map::iterator player_view =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (player_view == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_QUIT, gang_id);
	announce_push_role_view(player_view->second);
	announce_send();
}

void Logic_Announce::create_ann_gang_kick(const role_id_t role_id_1, const role_id_t role_id_2) {
	DB_View::ID_Player_View_Map::iterator player_view_1 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_1);
	if (player_view_1 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	DB_View::ID_Player_View_Map::iterator player_view_2 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_2);
	if (player_view_2 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_KICK, player_view_1->second->gang_id);
	announce_push_role_view(player_view_1->second);
	announce_push_role_view(player_view_2->second);
	announce_send();
}

void Logic_Announce::create_ann_gang_upgrade(const role_id_t role_id, const int cost_fund, const int gang_level) {
	DB_View::ID_Player_View_Map::iterator player_view =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (player_view == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_UPGRADE, player_view->second->gang_id);

	announce_push_role_view(player_view->second);

	std::stringstream ss;
	ss << cost_fund;
	announce_push_text(ss.str());

	ss.clear();
	ss.str("");
	ss << gang_level;
	announce_push_text(ss.str());

	announce_send();
}

void Logic_Announce::create_ann_gang_resign_to(const role_id_t role_id_1, const role_id_t role_id_2) {
	DB_View::ID_Player_View_Map::iterator player_view_1 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_1);
	if (player_view_1 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	DB_View::ID_Player_View_Map::iterator player_view_2 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_2);
	if (player_view_2 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_RESIGN_TO, player_view_2->second->gang_id);
	announce_push_role_view(player_view_1->second);
	announce_push_role_view(player_view_2->second);
	announce_send();
}

void Logic_Announce::create_ann_gang_seize_power(const role_id_t role_id_1, const role_id_t role_id_2) {
	DB_View::ID_Player_View_Map::iterator player_view_1 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_1);
	if (player_view_1 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	DB_View::ID_Player_View_Map::iterator player_view_2 =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id_2);
	if (player_view_2 == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_SEIZE_POWER, player_view_1->second->gang_id);
	announce_push_role_view(player_view_1->second);
	announce_push_role_view(player_view_2->second);
	announce_send();
}

void Logic_Announce::create_ann_gang_announce_set(const role_id_t role_id) {
	DB_View::ID_Player_View_Map::iterator player_view =
			LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
	if (player_view == LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
		return;
	}
	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_GANG_ANNOUNCE_SET, player_view->second->gang_id);
	announce_push_role_view(player_view->second);
	announce_send();
}

void Logic_Announce::create_gang_boss_announce(const gang_id_t gang_id, const int ann_id, const role_id_t role_id,
		std::string str1/* = ""*/, std::string str2/* = ""*/){
	make_announce(ANNOUNCE_DISTRICT_GANG, ann_id, gang_id);
	if(role_id != 0){
		DB_View::ID_Player_View_Map::iterator it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if (it != LOGIC_MONITOR->db_view()->id_player_view_map.end()) {
			announce_push_role_view(it->second);
		}
	}
	if(str1 != ""){
		announce_push_text(str1);
	}
	if(str2 != ""){
		announce_push_text(str2);
	}
	announce_send();
}

void Logic_Announce::create_ann_wing_active() {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_WING_ACTIVE);

	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	announce_send();
}

void Logic_Announce::create_ann_wing_upgrade(const int32_t lv) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_WING_UPGRADE);

	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	std::stringstream ss;
	ss << lv;
	announce_push_text(ss.str());

	announce_send();
}

void Logic_Announce::create_ann_dragon_vale_specail_online() {
	if(!player_self()->gang_id()) {
		return;
	}

	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_DRAGON_VALE_SPECIAL_ONLINE, player_self()->gang_id());
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	announce_send();
}

void Logic_Announce::create_ann_challenge(const role_id_t role_id, std::string role_name) {
	if(!player_self()->gang_id()) {
		return;
	}

	make_announce(ANNOUNCE_DISTRICT_GANG, ANN_ID_CHALLENGE, player_self()->gang_id());
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);

	Logic_Player *player = LOGIC_MONITOR->find_logic_player_by_role_id(role_id);
	if(player) {
		Role_Info target_role;
		target_role.career = player->career();
		target_role.combat = 0;
		target_role.gender = player->base_detail().gender;
		target_role.level = player->level();
		target_role.role_id = role_id;
		target_role.role_name = player->role_name();
		target_role.vip_level = player->vip();
		target_role.guild_name = player->base_detail().gang_name;
		announce_push_role(target_role);
	}
	else {
		DB_View::ID_Player_View_Map::iterator find_it =
				LOGIC_MONITOR->db_view()->id_player_view_map.find(role_id);
		if(find_it != LOGIC_MONITOR->db_view()->id_player_view_map.end() && find_it->second) {
			Role_Info target_role;
			target_role.career = find_it->second->career;
			target_role.combat = 0;
			target_role.gender = find_it->second->gender;
			target_role.level = find_it->second->level;
			target_role.role_id = find_it->second->role_id;
			target_role.role_name = find_it->second->role_name;
			target_role.vip_level = find_it->second->vip_level;
			target_role.guild_name = find_it->second->gang_name;
			announce_push_role(target_role);
		}
		else {
			Role_Info target_role;
			target_role.role_id = role_id;
			target_role.role_name = role_name;
			announce_push_role(target_role);
		}
	}

	announce_send();
}

void Logic_Announce::thorlottery_item(const Chat_Item_Info& chat_item_info) {

	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_THORLOTTERY_ITEM);
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);
	announce_push_item(chat_item_info);

	announce_send();
}

void Logic_Announce::thorlottery_exchange(const Chat_Item_Info& chat_item_info) {

	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_THORLOTTERY_EXCHANGER);
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);
	announce_push_item(chat_item_info);


	announce_send();
}

void Logic_Announce::create_ann_get_good_card(const Chat_Item_Info& chat_item_info) {

	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_THORLOTTERY_ITEM);
	Role_Info role_info;
	role_info.career = player_self()->career();
	role_info.combat = 0;
	role_info.gender = player_self()->base_detail().gender;
	role_info.level  = player_self()->level();
	role_info.role_id = player_self()->role_id();
	role_info.role_name = player_self()->role_name();
	role_info.vip_level = player_self()->vip();
	role_info.guild_name = player_self()->base_detail().gang_name;
	announce_push_role(role_info);
	announce_push_item(chat_item_info);

	announce_send();
}

void Logic_Announce::create_ann_title_nobility(Logic_Player *player, const std::string &name) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_TITLE_NOBILITY);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}

	announce_send();
}

void Logic_Announce::create_ann_gem_four(Logic_Player *player, const std::string &name) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_GEM_FOUR);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}

	announce_send();
}

void Logic_Announce::create_ann_gem_five(Logic_Player *player, const std::string &name) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_GEM_FIVE);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}

	announce_send();
}

void Logic_Announce::create_ann_gold_syn(Logic_Player *player, const std::string &name) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_GOLD_SYN);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}

	announce_send();
}

void Logic_Announce::create_ann_gem_carve_ten(Logic_Player *player) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_GEM_CARVE_TEN);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}

	announce_send();
}

void Logic_Announce::create_ann_gem_carve_twenty(Logic_Player *player) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_GEM_CARVE_TWENTY);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}

	announce_send();
}

void Logic_Announce::create_ann_gem_carve_thirty(Logic_Player *player) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_GEM_CARVE_THIRTY);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}

	announce_send();
}

void Logic_Announce::create_ann_open_lottery(Logic_Player *player,const Chat_Item_Info &item,const Chat_Item_Info &get_item) {
	if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_OPEN_LOTTERY);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	announce_push_item(item);
	announce_push_item(get_item);

	announce_send();
}

void Logic_Announce::create_ann_expedition_start(const int num) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_EXPEDITION_START);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}

	for (int i = 0; i < num; ++i) {
		announce_send();
	}
}

void Logic_Announce::create_ann_expedition_timing_monster_refresh(const std::string name) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_EXPEDITION_TIMING_MONSTER_REFRESH);
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}

	announce_send();
}

void Logic_Announce::create_ann_expedition_timing_monster_killed(const std::string name, const Role_Info &role_info) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_EXPEDITION_TIMING_MONSTER_KILLED);
	{
		std::stringstream ss;
		ss << name;
		announce_push_text(ss.str());
	}
	{
		announce_push_role(role_info);
	}

	announce_send();
}

void Logic_Announce::create_ann_expedition_box_open(const Role_Info &role_info, const std::string box_name, const std::string item_name) {
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_EXPEDITION_BOX_OPEN);
	{
		announce_push_role(role_info);
	}
	{
		std::stringstream ss;
		ss << box_name;
		announce_push_text(ss.str());
	}
	{
		std::stringstream ss;
		ss << item_name;
		announce_push_text(ss.str());
	}
	announce_send();
}

void Logic_Announce::create_ann_card_make(Logic_Player *player,const Chat_Item_Info &item) {

    if(!player) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_CARD_MAKE);
	{
		Role_Info role_info;
		role_info.career = player_self()->career();
		role_info.combat = 0;
		role_info.gender = player_self()->base_detail().gender;
		role_info.level  = player_self()->level();
		role_info.role_id = player_self()->role_id();
		role_info.role_name = player_self()->role_name();
		role_info.vip_level = player_self()->vip();
		role_info.guild_name = player_self()->base_detail().gang_name;
		announce_push_role(role_info);
	}
	announce_push_item(item);
	announce_send();
}

