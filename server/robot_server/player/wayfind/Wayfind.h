/*
 * Wayfind.h
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */

#ifndef WAYFIND_H_
#define WAYFIND_H_

#include "Wayfind_Struct.h"

class Wayfind {
public:
	Wayfind();
	virtual ~Wayfind();

	void reset(void);

	void path_find(void);
	Wayfind_Info &wayfind_info(void);

protected:
	Wayfind_Info wayfind_info_;

};



#endif /* ROBOT_PLAYER_H_ */
