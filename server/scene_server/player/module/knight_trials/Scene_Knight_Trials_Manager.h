/*
 * Scene_Knight_Trials_Manager.h
 *
 *  Created on: 2014年9月16日
 *      Author: xiaoliang
 */

#ifndef KNIGHT_TRIALS_MANAGER_H_
#define KNIGHT_TRIALS_MANAGER_H_

#include <sys/types.h>
#include "Singleton.h"
#include "Msg_Inner_Struct.h"

class Scene_Player;

class Scene_Knight_Trials_Manager {
public:
	Scene_Knight_Trials_Manager() {
		reset();
	}
	void reset();

	// 匹配跨服玩家
	int scene_knight_trials_match_player(Block_Buffer& buf);

	//进入战斗
	int scene_knight_trials_enter_fighter(Scene_Player *player, Block_Buffer& buf);

private:

};

typedef Singleton<Scene_Knight_Trials_Manager> Scene_Knight_Trials_Manager_Single;
#define SCENE_KNIGHT_TRIALS_MANAGER   (Scene_Knight_Trials_Manager_Single::instance())

#endif /* KNIGHT_TRIALS_MANAGER_H_ */
