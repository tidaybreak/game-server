/*
 * Material.h
 *
 *  Created on: Mar 13, 2014
 *      Author: linyisheng
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "NPC.h"

class Scene_Player;

class Material: public NPC {
	typedef boost::unordered_map<role_id_t, Time_Value> Gather_Player_Map;
	enum Material_Type{
		Material_Type_Single   = 0,			// 单人采集
		Material_Type_Mult     = 1,			// 多人采集
	};
public:
	Material(void);
	virtual ~Material(void);

	int birth(role_id_t npc_id, int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info);

	int init(int npc_type_id, Move_Scene* scene, NPC_Record *point, NPC_Addition_Info &npc_addition_info);
	int fini(void);
	int reset(void);

	virtual int tick(Time_Value &now);
	virtual int move_appear_info(Block_Buffer &buf);

	virtual std::string role_name(void);
	virtual void set_role_name(const std::string& name);

//	virtual int die_process(const Attack_Info &info, Fighter &killer);

	virtual bool is_material(void);

	virtual Material *material_self(void);

	inline Material_Detail &material_detail(void);

	virtual void material_drop(role_id_t attacker = 0);

	int change_self(int jid);

	// 通知逻辑服死亡
	int notice_logic_die(role_id_t attacker);

	int start_gather(Scene_Player* player);
	int stop_gather(role_id_t role_id);

	inline void add_gather_status(void);
	inline void del_gather_status(void);

	int drop_item(Scene_Player* player);
	int gather_goods_result(role_id_t role_id, int result);
	int8_t &camp(void);
	int8_t &camp_bak(void);
	int8_t &camp_last(void);
	int8_t is_gathering_camp(void);
public:
	int get_gather_players(std::vector<role_id_t>& players);
	int change_gather_cd(role_id_t role_id, int total_time, int remain_time);

	void send_appear_addition(void);
	void send_player_end_time(const Time_Value &end_time, const Time_Value &now);
	const Gather_Player_Map &gather_player_map(void);

	void add_addition(const role_id_t role_id = 0);
private:
	int gang_war_handle(Scene_Player* player);
private:
	Material_Detail material_detail_;
	Gather_Player_Map gather_player_map_;
	Gather_Player_Map success_player_map_;
	int gather_status_;
	int8_t camp_;
	int8_t camp_bak_;
	int8_t camp_last_;
//	Role_Id_Set gang_war_all_gather_player_;
};

////////////////////////////////////////////////////////////////////////////////

Material_Detail &Material::material_detail(void) {
	return material_detail_;
}

void Material::add_gather_status(void){
	gather_status_ += 1;
}

void Material::del_gather_status(void){
	gather_status_ -= 1;
}
#endif /* MATERIAL_H_ */
