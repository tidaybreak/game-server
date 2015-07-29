/*
 * Gang_Manor_Scene.h
 *
 *  Created on: Jun 26, 2014
 *      Author: chenyulin
 */

#ifndef GANG_MANOR_SCENE_H_
#define GANG_MANOR_SCENE_H_

#include "Move_Scene.h"

class Gang_Manor_Scene: public Move_Scene {
public:
	Gang_Manor_Scene();
	virtual ~Gang_Manor_Scene();

	void reset(void);
};


#endif /* GANG_MANOR_SCENE_H_ */
