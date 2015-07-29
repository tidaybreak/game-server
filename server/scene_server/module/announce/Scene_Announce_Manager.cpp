/*
 * Scene_Announce_Manager.cpp
 *
 *  Created on: 2014年8月13日
 *      Author: root
 */


#include "Scene_Announce_Manager.h"
#include "Config_Cache.h"
#include "Scene_Monitor.h"
#include "Msg_Inner_Struct.h"
#include "Scene_Player.h"
#include "announcement/Announcement_Struct.h"
#include "chater/Chater_Def.h"
#include "Msg_Active_Struct.h"
#include "mover/Mover_Struct.h"
#include "scene_move/Move_Scene.h"
#include "Base_Detail.h"

Scene_Announce_Manager::Scene_Announce_Manager() {
	// TODO Auto-generated constructor stub

}

Scene_Announce_Manager::~Scene_Announce_Manager() {
	// TODO Auto-generated destructor stub
}

void Scene_Announce_Manager::announce_send_to_scene(Scene_Player *player, int32_t annid, std::vector<Chat_Content> &content) {
	if(!player || !player->move_scene()) { return; }
	MSG_83000000 msg;
	msg.chat_type = chat_scene;
	msg.system_type = annid;
	msg.content = content;
	Block_Buffer buf;
	buf.make_message(msg.msg_id);
	msg.serialize(buf);
	buf.finish_message();
	player->move_scene()->broadcast_chat_all(player->mover_detail().layer_id, buf);
}

void Scene_Announce_Manager::announce_send_to_all(uint8_t type, int32_t annid, std::vector<Chat_Content> &content) {
	make_announce(type, annid);
	ann_contents = content;
	announce_send();
}

void Scene_Announce_Manager::announce_send_to_gang(int64_t gang_id, int32_t annid, std::vector<Chat_Content> &content) {
	make_announce(ANNOUNCE_DISTRICT_GANG, annid, gang_id);
	ann_contents = content;
	announce_send();
}

void Scene_Announce_Manager::announce_send_to_roles(const std::vector<int64_t>& recievers, int32_t annid, std::vector<Chat_Content> &content) {
	make_announce(ANNOUNCE_DISTRICT_GANG, annid);
	announce_push_recievers(recievers);
	ann_contents = content;
	announce_send();
}




void Scene_Announce_Manager::create_ann_battle_in_bonfire_scene(Scene_Player *player, Scene_Player *loser) {
	if(!player || !player->move_scene()) { return; }
	if(!loser || !loser->move_scene()) { return; }
	make_announce(ANNOUNCE_DISTRICT_SYSTEM, ANN_ID_BATTLE_IN_BONFIRE_SCENE);

	Role_Info winner_info;
	winner_info.career = player->career();
	winner_info.gender = player->base_detail().gender;
	winner_info.level = player->level();
	winner_info.role_id = player->role_id();
	winner_info.role_name = player->role_name();
	winner_info.vip_level = player->vip();
	winner_info.guild_name = player->base_detail().gang_name;


	Role_Info loser_info;
	loser_info.career = loser->career();
	loser_info.gender = loser->base_detail().gender;
	loser_info.level = loser->level();
	loser_info.role_id = loser->role_id();
	loser_info.role_name = loser->role_name();
	loser_info.vip_level = loser->vip();
	loser_info.guild_name = loser->base_detail().gang_name;

	announce_push_role(winner_info);
	announce_push_role(loser_info);

	announce_send_to_scene(player, ANN_ID_BATTLE_IN_BONFIRE_SCENE, ann_contents);
}





void Scene_Announce_Manager::announce_send(void) {
	int64_t server_flag = CONFIG_CACHE->server_flag();
	if(server_flag) {
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

		SCENE_MONITOR->send_to_logic(server_flag, buf__);

	}
}
