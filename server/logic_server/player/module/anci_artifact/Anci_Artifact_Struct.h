/*
 * Anci_Artifact_Struct.h
 *
 *  Created on: 2014年10月10日
 *      Author: Linqiyou
 */

#ifndef ANCI_ARTIFACT_STRUCT_H_
#define ANCI_ARTIFACT_STRUCT_H_

#include "Base_Detail.h"

struct Anci_Artifact_Cond_Info {
	Anci_Artifact_Cond_Info(void) {
		reset();
	}
	void reset(void) {
		id = 0;
		state_count = 0;
		data = 0;
	}
	int32_t id;
	int8_t state_count;
	int32_t data;
};
typedef boost::unordered_map<int32_t, Anci_Artifact_Cond_Info> AA_Info_Map;
typedef boost::unordered_map<int32_t, Anci_Artifact_Cond_Info*> AA_Info_Listen;

struct Anci_Artifact_Unban_Info {
	Anci_Artifact_Unban_Info(void) { reset(); }
	void reset(void) {
		id = 0;
		state = 0;
		box_state = 0;
	}
	int id;
	int8_t state;
	int8_t box_state;
};
typedef boost::unordered_map<int, Anci_Artifact_Unban_Info> AA_Unban_Info;

struct Anci_Artifact_Detail : public Base_Detail {
	Anci_Artifact_Detail(void);
	virtual ~Anci_Artifact_Detail();
	void reset(void);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

	int load(void);
	int save(void);
	void detail_change(void) {
		is_change = true;
	}

	role_id_t role_id;
//	int64_t perfectPoint; //
	AA_Info_Map aa_info_map;
	Int_Hash_Set complete_ids;
	AA_Unban_Info aa_unban_info;
};


#endif /* ANCI_ARTIFACT_STRUCT_H_ */
