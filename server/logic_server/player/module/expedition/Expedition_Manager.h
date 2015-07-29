/*
 * Expedition_Manager.h
 *
 *  Created on: 2014-05-22
 *      Author: xiaoliang
 */

#ifndef EXPEDITION_MANAGER_H_
#define EXPEDITION_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "expedition_scene/Expedition_Scene_Struct.h"
#include "expedition_scene/Expedition_Scene_Def.h"
#include "Expedition_Struct.h"
struct Base_Detail;

class Expedition_Manager {
public:
	typedef boost::unordered_map<int64_t, Gang_Data_Info> Int64_Gang_Data_Info_Map;

	Expedition_Manager();
	virtual ~Expedition_Manager();
	void reset(void);
	void load(void);
	void save(void);

	int start(void);

	int data_channle(Block_Buffer& buf);//场景服到逻辑服的数据接口通道
	void send_all_gang_info_to_scene(void);//发送所有公会信息到跨服
	void send_gang_info_to_scene(const int64_t gang_id);
	void send_del_gang_info_to_scene(const int64_t gang_id);
	void listen_gang_leader_change(const Base_Detail &detail);

	uint32_t get_line_by_gang_id(int64_t gang_id);
	const Coord *get_coord_by_gang_id(int64_t gang_id);

	inline const Expedition_Manager_Detail &exepdition_manager_detail(void);
private:
	Expedition_Manager_Detail expedition_manager_detail_;
};

const Expedition_Manager_Detail &Expedition_Manager::exepdition_manager_detail(void) {
	return expedition_manager_detail_;
}

typedef Singleton<Expedition_Manager> Expedition_Manager_Single;
#define EXPEDITION_MANAGER    (Expedition_Manager_Single::instance())

#endif /* EXPEDITION_H_ */
