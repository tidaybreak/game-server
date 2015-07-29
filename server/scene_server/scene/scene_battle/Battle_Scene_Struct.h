/*
 * Battle_Scene_Struct.h
 *
 *  Created on: Jan 25, 2015
 *      Author: linqiyou
 */

#ifndef BATTLE_SCENE_STRUCT_H_
#define BATTLE_SCENE_STRUCT_H_

enum Other_Type {
	OT_UNINIT		= 0,
	OT_EXPEDITION	= 1,
	OT_TEAM_ARENA	= 2,
};
struct Other_Info {
	Other_Info(void) {
		reset();
	}
	void reset(void) {
		type = OT_UNINIT;
		int_v1 = 0;
		int_v2 = 0;
		iv_1.clear();
		iv_2.clear();
	}
	Other_Type type;
	int int_v1;
	int int_v2;
	Int_Vec iv_1;
	Int_Vec iv_2;
};


#endif /* BATTLE_SCENE_STRUCT_H_ */
