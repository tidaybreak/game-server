/*
 * Logic_Announce.h
 *
 *  Created on: May 12, 2014
 *      Author: caijianwei
 */

#ifndef LOGIC_ANNOUNCE_H_
#define LOGIC_ANNOUNCE_H_

#include "announcement/Annoucement.h"
#include "Logic_Player_Base.h"

struct Player_DB_View;

class Logic_Announce: virtual public Logic_Player_Base, public Annoucement {
public:
	Logic_Announce();
	virtual ~Logic_Announce();

	virtual void announce_send();
	void create_gang_announce(int64_t gang_id, const std::string& gang_name);
	void create_arena_rise_announce(const std::string& group_name);
	void create_handsel_flowers_announce(const int32_t ann_id, const role_id_t role_id, const Chat_Item_Info& item);
	void create_ann_rune_stone_smelt(const Chat_Item_Info& chat_item_info);
	void create_ann_active_content_vip_addnum(const uint32_t vip_level, const uint32_t num);
	void create_ann_active_content_guild_level_up(const uint32_t level);
	void create_ann_equip_enchant(const int prop, const int val, const int star);
	void create_ann_gang_draw(const Chat_Item_Info& chat_item_info);
	void create_ann_rune_fill_energy(const int prop, const int level);
	void create_ann_artifact_upgrade(const std::string& belonger_name, const std::string& artifact_name, const int step, const int level);
	void create_ann_init_open_vip4(void);
	void create_ann_open_covenant(const int32_t ann_id);
	void create_ann_open_all_covenant(void);
	void create_ann_handsel_covenant(Logic_Player *player,const int32_t ann_id);
	void create_ann_handsel_all_covenant(Logic_Player *player);
	void create_ann_buy_promo_goods(const Chat_Item_Info& item);
	void create_ann_dragon_evolution_success(int64_t role_id, const std::string &dragon_name);
	void create_ann_title(Logic_Player *player, const std::string &name, const std::string &title);
	void create_ann_title_sign_in(Logic_Player *player, const int title_id, const Role_Vec &role_vec = Role_Vec());
	void create_ann_achievement(Logic_Player *player, const std::string &name, const std::string &title, const uint32_t achi_id);
	void create_ann_gang_war(void);
	void create_ann_equip_master(Logic_Player *player, const uint32_t index, const int lvl);
	void create_ann_team_sports(void);
	void create_ann_recharge_rebate(const int32_t ann_id);
	void create_ann_add_auction_goods(const int32_t ann_id, const Chat_Item_Info& item, const uint32_t price);

	void create_ann_gang_join_in(const role_id_t role_id);
	void create_ann_gang_headship_appoint(const role_id_t role_id_1, const role_id_t role_id_2, const int headship);
	void create_ann_gang_quit(const role_id_t role_id, const gang_id_t gang_id);
	void create_ann_gang_kick(const role_id_t role_id_1, const role_id_t role_id_2);
	void create_ann_gang_upgrade(const role_id_t role_id, const int cost_fund, const int gang_level);
	void create_ann_gang_resign_to(const role_id_t role_id_1, const role_id_t role_id_2);
	void create_ann_gang_seize_power(const role_id_t role_id_1, const role_id_t role_id_2);
	void create_ann_gang_announce_set(const role_id_t role_id);
	void create_gang_boss_announce(const gang_id_t gang_id, const int ann_id, const role_id_t role_id,
			std::string str1 = "", std::string str2 = "");

	void create_ann_wing_active(void);
	void create_ann_wing_upgrade(const int32_t lv);

	void thorlottery_item(const Chat_Item_Info& chat_item_info);
	void thorlottery_exchange(const Chat_Item_Info& chat_item_info);

	void create_ann_dragon_vale_specail_online();
	void create_ann_challenge(const role_id_t role_id, std::string role_name);

	void create_ann_get_good_card(const Chat_Item_Info& chat_item_info);

	void create_vip_turntable_announce(const Chat_Item_Info& item);
	void create_ann_expedition_defend(const int64_t gang_id, const int point_id, const std::string &name, const std::string &name_2);
	void create_ann_title_nobility(Logic_Player *player, const std::string &name);
	void create_ann_gem_four(Logic_Player *player, const std::string &name);
	void create_ann_gem_five(Logic_Player *player, const std::string &name);
	void create_ann_gold_syn(Logic_Player *player, const std::string &name);
	void create_ann_gem_carve_ten(Logic_Player *player);
	void create_ann_gem_carve_twenty(Logic_Player *player);
	void create_ann_gem_carve_thirty(Logic_Player *player);
	void create_ann_open_lottery(Logic_Player *player,const Chat_Item_Info &item,const Chat_Item_Info &get_item);

	void create_ann_expedition_start(const int num = 1);
	void create_ann_expedition_timing_monster_refresh(const std::string name);
	void create_ann_expedition_timing_monster_killed(const std::string name, const Role_Info &role_info);
	void create_ann_expedition_box_open(const Role_Info &role_name, const std::string box_name, const std::string item_name);

	void create_ann_card_make(Logic_Player *player,const Chat_Item_Info &item);
private:
	void announce_push_role_view(Player_DB_View* player_db_view);

};

#endif /* LOGIC_ANNOUNCE_H_ */
