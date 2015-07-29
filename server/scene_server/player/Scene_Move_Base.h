/*
 * Scene_Player_Base.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef SCENE_PLAYER_BASE_H_
#define SCENE_PLAYER_BASE_H_

#include "Player_Base.h"

class Scene_Player;
class Move_Scene;
class Fighter;
class NPC;
class Monster;
class Player_Monster;
class Material;
class Machine;
class Drops;
class Hero;

class Scene_Move_Base {
public:
	Scene_Move_Base(void);
	virtual ~Scene_Move_Base(void);

	void reset(void);

	virtual Scene_Player *player_self(void){return 0;}

	virtual NPC *npc_self(void){return 0;}

	virtual Monster *monster_self(void){return 0;}

	virtual Player_Monster *player_monster_self(void){return 0;}

	virtual Material *material_self(void){return 0;}

	virtual Machine *machine_self(void){return 0;}

	virtual Drops *drops_self(void){return 0;}

	virtual Hero *hero_self(void){return 0;}

	// player send
	virtual int send_to_gate(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_logic(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_client(Block_Buffer &buf, bool use_swap = false);
	virtual int send_to_local_chat(Block_Buffer &buf, bool use_swap = false);

	virtual Fighter *fighter(void) = 0;

	virtual Move_Scene *move_scene(void) = 0;

	virtual role_id_t role_id(void) = 0;

	virtual int level(void) = 0;

	virtual std::string role_name(void);

	virtual int career(void);

	virtual int vip(void);

	void make_message(Block_Buffer &buf, int msg_id);

	virtual int send_err_to_client(int error_id, int source);

	virtual void call_broadcast_around(Block_Buffer &buf) = 0;
};


#endif /* SCENE_PLAYER_BASE_H_ */
