/*
 * Scene_Ganger.h
 *
 *  Created on: 2014年7月31日
 *      Author: Linqiyou
 */

#ifndef SCENE_GANGER_H_
#define SCENE_GANGER_H_

#include "Scene_Move_Base.h"
#include "gang/Gang_Struct.h"
#include "Gang_War_Struct.h"

struct Ganger_Detail;
struct Ganger_Info;
struct Gang_DB_View;
struct Gang_War_Rank_Detail;

class Scene_Ganger : virtual public Scene_Move_Base {
	static const int GANG_WAR_SCENE_ID = 80213001;
public:
	Scene_Ganger();
	virtual ~Scene_Ganger();
	void reset();
	int sign_in(void);
	int sign_out(void);
	int time_up(const Time_Value& now);

	int client_scene_complete(void);

	Ganger_Detail &ganger_detail(void);

	Gang_War_Rank_Detail &gang_war_info(void);

	int gang_load_detail(Block_Buffer &buf);
	int ask_gang_war_rank_info(Block_Buffer &buf);

	int sync_gang_war_to_logic(void);

	int set_gang_war_info(const Gang_War_Rank_Detail &gang_war_info);

	void update_mover_gang_war_info(void);
	int get_gang_war_hp(void);

	int send_gang_war_player_info(void);

	void gang_war_die_revive(void);
	void gang_war_die_revive_winner(void);

	void gang_war_check_revive(const Time_Value& now);

	void reset_gang_war_info(void);

	void gang_war_gather_revive_ball(void);

	int gang_war_scene_notice(int type, std::string name1, std::string name2, int merit, int contribute, bool delay = false);

	int gang_war_scene_announce(int type, role_id_t role_id = 0, std::string name = "", std::string gang_name = "", int value1 = 0, int value2 = 0, int value3 = 0, bool delay = false);

	int req_gang_war_heal(void);

	int gang_war_set_point_id(const int point_id);
	int gang_war_point_id(void);
private:
	Ganger_Detail ganger_detail_;
	Gang_War_Rank_Detail gang_war_info_;
	std::vector<Block_Buffer> announce_buf_;
	std::vector<Block_Buffer> notice_buf_;
	int point_id_;
};

////////////////////////////////////////////////////////////////////////////


#endif /* SCENE_GANGER_H_ */
