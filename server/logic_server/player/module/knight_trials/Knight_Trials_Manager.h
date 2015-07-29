/*
 * Knight_Trials_Manager.h
 *
 *  Created on: May 12, 2014
 *      Author: xiaoliang
 */

#ifndef KNIGHT_TRIALS_MANAGER_H_
#define KNIGHT_TRIALS_MANAGER_H_

#include "Game_Typedef.h"

typedef std::vector<uint8_t> UInt8_Vec;

typedef std::map<int, Role_Id_Set, std::less<int> > Force_Role_Set_Map;


struct Knight_Trials_Player;
struct Logic_Player_Detail;

struct Force_Role_Struct {
	Force_Role_Struct(void) { reset(); }
	void reset(void) {
		force = 0;
		role_vec.clear();
	}
	int force;
	Role_Vec role_vec;
};

typedef std::vector<Force_Role_Struct> Force_Role_Vec;

class Knight_Trials_Manager {
public:
	Knight_Trials_Manager(void);
	~Knight_Trials_Manager(void);

	void reset(void);
	void tick(const Time_Value &now);


	int logic_knight_trials_match_player(Block_Buffer& buf);

	int match_player_role_id_by_rankvec(const UInt8_Vec &rank, Role_Id_Set &role_id_set, int ttl_force, role_id_t self_role_id);

//	void match_player_callback(role_id_t role_id, int64_t from_uuid, role_id_t from_role_id);
	int match_player_local(role_id_t role_id, Knight_Trials_Player &player, role_id_t from_role_id, int64_t match_day, bool clear);

	void process_load_player_data(Logic_Player_Detail* logic_player_detail);
	void process_load_fighter_player_data(Knight_Trials_Player *kplayer);

	void insert_force_role_id_match(int force, role_id_t role_id);
	void make_force_role_vec_form_map(void);
	void login_out_update_map(int old_force, int new_force, role_id_t role_id);

private:

	Force_Role_Set_Map force_role_vec_map;
	Force_Role_Vec force_role_vec;
	Time_Value lmake_time;
};

typedef Singleton<Knight_Trials_Manager> Knight_Trials_Manager_Single;
#define KNIGHT_TRIALS_MANAGER    (Knight_Trials_Manager_Single::instance())

#endif /* KNIGHT_TRIALS_MANAGER_H_ */
