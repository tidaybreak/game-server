/*
 * Scene_Expeditioner.h
 *
 *  Created on: Jan 29, 2015
 *      Author: linqiyou
 */

#ifndef SCENE_EXPEDITIONER_H_
#define SCENE_EXPEDITIONER_H_

#include "Scene_Move_Base.h"
#include "expedition/Expedition_Struct.h"
#include "expedition_scene/Expedition_Scene_Struct.h"


class Scene_Expeditioner : virtual public Scene_Move_Base {
public:
	Scene_Expeditioner();
	virtual ~Scene_Expeditioner();

	int load_detail(Block_Buffer &buf);

	void reset(void);
	int time_up(const Time_Value& now);

	// inner --
	int expedition_data_channel(Block_Buffer &buf);
	int update_mover_expedition_info(void);
	int get_hp(void);
	// -- inner
	int set_expedition_scene_info(const Expedition_Scene_Info &expedition_scene_info);
	int reset_expedition_scene_info(void);
	int send_expedition_scene_info(void);
	void expedition_die_revive(void);
	void expedition_die_revive_winner(void);
	void expedition_check_revive(const Time_Value& now);
	void check_defending(void);

	void active_fight_status(void);
	void active_fight_status_for_all(void);
	bool expedition_is_fight_status(void);

	Expedition_Detail &expedition_detail(void);

	Expedition_Scene_Info &expedition_scene_info(void);
private:
	Expedition_Detail expedition_detail_;

	Expedition_Scene_Info expedition_scene_info_;
};

#endif /* SCENE_EXPEDITIONER_H_ */
