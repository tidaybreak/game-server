/*
 * Scene_Timer_Request.h
 *
 *  Created on: Jan 22, 2014
 *      Author: ti
 *      定时器消息
 */

#ifndef SCENE_TIMER_REQUEST_H_
#define SCENE_TIMER_REQUEST_H_

#include "Game_Typedef.h"

class Scene_Monitor;
class Scene_Player;

class Scene_Timer_Request {
public:
	Scene_Timer_Request();
	virtual ~Scene_Timer_Request();

	Scene_Monitor *monitor(void);

	void scene_tick(Time_Value &now);

	void scene_tick_50ms(Time_Value &now);
	void scene_tick_100ms(Time_Value &now);
	void scene_tick_300ms(Time_Value &now);
	void scene_tick_500ms(Time_Value &now);
	void scene_tick_800ms(Time_Value &now);
	void scene_tick_1000ms(Time_Value &now);

private:
	Scene_Monitor *monitor_;

	Time_Value tick_100ms_;
	Time_Value tick_300ms_;
	Time_Value tick_500ms_;
	Time_Value tick_800ms_;
	Time_Value tick_1000ms_;
};

typedef Singleton<Scene_Timer_Request> Scene_Timer_Request_Single;
#define SCENE_TIMER_REQUEST    (Scene_Timer_Request_Single::instance())

inline Scene_Monitor *Scene_Timer_Request::monitor(void) {
	return monitor_;
}

#endif /* SCENE_TIMER_REQUEST_H_ */
