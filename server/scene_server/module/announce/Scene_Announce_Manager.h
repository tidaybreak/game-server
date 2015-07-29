/*
 * Scene_Announce_Manager.h
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */

#ifndef SCENE_ANNOUNCE_MANAGER_H_
#define SCENE_ANNOUNCE_MANAGER_H_

#include "announcement/Annoucement.h"

class Scene_Player;

class Scene_Announce_Manager: public Annoucement {
public:

	Scene_Announce_Manager();
	virtual ~Scene_Announce_Manager();

	void announce_send_to_scene(Scene_Player *player, int32_t annid, std::vector<Chat_Content> &content);
	void announce_send_to_all(uint8_t type, int32_t annid, std::vector<Chat_Content> &content);
	void announce_send_to_gang(int64_t gang_id, int32_t annid, std::vector<Chat_Content> &content);
	void announce_send_to_roles(const std::vector<int64_t>& recievers, int32_t annid, std::vector<Chat_Content> &content);

	void create_ann_battle_in_bonfire_scene(Scene_Player *player, Scene_Player *loser);

private:

	void announce_send(void);
};

typedef Singleton<Scene_Announce_Manager> Scene_Announce_Manager_Single;
#define SCENE_ANNOUNCE_MANAGER    (Scene_Announce_Manager_Single::instance())



#endif /* SCENE_ANNOUNCE_MANAGER_H_ */
