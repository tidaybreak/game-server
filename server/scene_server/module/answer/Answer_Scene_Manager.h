/*
 * Answer_Scene_Manager.h
 *
 *  Created on: 2014年8月13日
 *      Author: xiaoliang
 */

#ifndef ANSWER_SCENE_MANAGER_H_
#define ANSWER_SCENE_MANAGER_H_


#include "Scene_Manager.h"

class Move_Scene;
class Answer_Scene;

class Answer_Scene_Manager: public Scene_Manager {
public:
	typedef boost::unordered_map<Monitor_Unique_ID, Move_Scene *> Scene_Map;
	typedef Object_Pool_Ex<Answer_Scene, NULL_MUTEX> Scene_Pool;

	Answer_Scene_Manager();
	virtual ~Answer_Scene_Manager();

	virtual int init(const Monitor_Unique_ID &scene_uid);

	virtual int tick(Time_Value &now);

	virtual void show_scene_status(void);

	virtual Move_Scene *find_scene(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);

	virtual Move_Scene *find_scene_simple(const Monitor_Unique_ID &muid, const role_id_t role_id, const Create_Scene_Args &csa);


	int data_channle(Block_Buffer& buf);//逻辑服到场景服的数据接口通道

	void open_active(Block_Buffer& buf);
	void close_active(Block_Buffer& buf);

private:
	Scene_Map scene_map_;
	Scene_Pool scene_pool_;
	Monitor_Unique_ID mid;
};

typedef Singleton<Answer_Scene_Manager> Answer_Scene_Manager_Single;
#define ANSWER_SCENE_MANAGER    (Answer_Scene_Manager_Single::instance())



#endif /* ANSWER_SCENE_MANAGER_H_ */
