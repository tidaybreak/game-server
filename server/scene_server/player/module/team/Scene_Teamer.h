/*
 * Scene_Teamer.h
 *
 *  Created on: 2014年6月6日
 *      Author: Linqiyou
 */

#ifndef SCENE_TEAMER_H_
#define SCENE_TEAMER_H_

#include "Scene_Move_Base.h"
#include "team/Team_Struct.h"

struct Teamer_Detail;

class Scene_Teamer : virtual public Scene_Move_Base {
public:
	Scene_Teamer();
	virtual ~Scene_Teamer();

	int syn_teamer_info(Block_Buffer &buf);

	int64_t get_team_id();
	int8_t &follow(void);

	void reset(void);

	Teamer_Detail &teamer_detail(void);

	bool team_get_ready_status(void);

	int team_fb_create_player_monster(void);

	bool is_use_profile(void);

	void add_cumulate_info(const Int_Int_Map &award, const int kill_nums);

	Int_Int_Map &cumulate_award(void);
	int &kill_nums(void);
	Time_Value &start_time(void);

	void notice_team_rank(void);

private:
	Teamer_Detail teamer_detail_;
	int8_t follow_;
	int profile_set_;
	Int_Int_Map cumulate_award_;
	int kill_nums_;
	Time_Value start_time_;
};

#endif /* SCENE_TEAMER_H_ */
