/*
 * Module_Manager.h
 *
 *  Created on: Feb 25, 2014
 *      Author: ti
 */

#ifndef MODULE_MANAGER_H_
#define MODULE_MANAGER_H_

#include "Game_Header.h"

class Module_Manager {
public:
	Module_Manager();
	virtual ~Module_Manager();

	virtual void tick(Time_Value &now) = 0;

};

#endif /* MODULE_MANAGER_H_ */
