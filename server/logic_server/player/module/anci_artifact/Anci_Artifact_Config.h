/*
 * Anci_Artifact_Config.h
 *
 *  Created on: 2014年10月10日
 *      Author: Linqiyou
 */

#ifndef ANCI_ARTIFACT_CONFIG_H_
#define ANCI_ARTIFACT_CONFIG_H_

struct AA_Cond {
	AA_Cond(void) { reset(); }
	void reset(void) {
		id = 0;
		name.clear();
		type = 0;
		num = 0;
		cond_ext = 0;
		perfectPoint = 0;
		unban_cond_id = 0;
		gem_id = 0;
	}
	int id;
	std::string name;
	int type;
	int num;
	int cond_ext;
	int perfectPoint;
	int unban_cond_id;
	int gem_id;
};
typedef boost::unordered_map<int, AA_Cond> AA_Cond_Cfg;

struct AA_Gem {
	AA_Gem(void) { reset(); }
	void reset(void) {
		id = 0;
		name.clear();
		cond.clear();
		perfectPoint = 0;
		unban_cond_id = 0;
	}
	int id;
	std::string name;
	Int_Hash_Set cond;
	int perfectPoint; // 完美度
	int unban_cond_id;
};

typedef boost::unordered_map<int, AA_Gem> AA_Gem_Cfg;

struct AA_Unban_Cond {
	AA_Unban_Cond(void) { reset(); }
	void reset(void) {
		id = 0;
		name.clear();
		type = 0;
		num = 0;
		property.clear();
		gem.clear();
		extraProperty.clear();
		award.clear();
	}
	int id;
	std::string name;
	int type;
	int num;
	Int_Double_Map property;
	Int_Hash_Set gem;
	Int_Double_Map extraProperty; // 完美度100%时属性加成
	Item_Info_Vec award;
};
typedef boost::unordered_map<int, AA_Unban_Cond> AA_Unban_Cond_Cfg;


#endif /* ANCI_ARTIFACT_CONFIG_H_ */
