/*
 * Activity_Manager.h
 *
 *  Created on: 2014-07-02
 *      Author: xiaoliang
 */

#ifndef ACTIVE_MANAGER_H_
#define ACTIVE_MANAGER_H_

#include "Singleton.h"
#include "Game_Typedef.h"
#include "Activity_Struct.h"

class Activity_Manager {

public:
	Activity_Manager();
	virtual ~Activity_Manager();
	void reset(void);

	int start(void);

	void tick(Time_Value &now);

private:
	//37VIP活动配置start

	//37VIP活动配置end
};


typedef Singleton<Activity_Manager> Activity_Manager_Single;
#define ACTIVE_MANAGER    (Activity_Manager_Single::instance())

#endif /* ACTIVE_H_ */
