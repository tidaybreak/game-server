/*
 * Logic_Outpost_Manager.h
 *
 *  Created on: 2015年4月8日
 *      Author: root
 */

#ifndef LOGIC_OUTPOST_MANAGER_H_
#define LOGIC_OUTPOST_MANAGER_H_

#include "Game_Typedef.h"
#include "Singleton.h"

class Logic_Player;

class Logic_Outpost_Manager{
public:
	Logic_Outpost_Manager(void);
	~Logic_Outpost_Manager(void);
public:
	void reset(void);
public:
	int inner_sync_info(Logic_Player* player, Block_Buffer& buf);
};


typedef Singleton<Logic_Outpost_Manager> Logic_Outpost_Manager_Single;
#define LOGIC_OUTPOST_MANAGER (Logic_Outpost_Manager_Single::instance())



#endif /* LOGIC_OUTPOST_MANAGER_H_ */
