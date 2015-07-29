/*
 * Title_Config.h
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#ifndef TITLE_CONFIG_H_
#define TITLE_CONFIG_H_

struct Title_Cfg_Detail_Condition {
	Title_Cfg_Detail_Condition(void) {
		reset();
	}
	void reset(void) {
		cond = 0;
		from_table.clear();
		cond_list.clear();
	}
	int32_t cond;
	std::string from_table;
	Int_Set cond_list;
};

struct Title_Cfg_Detail {
	Title_Cfg_Detail(void) {
		reset();
	}
	void reset(void) {
		id = 0;
		quality = 0;
		prop.clear();
		validity = 0;
		condition.reset();
		name.clear();
		des.clear();
		cond_type = 0;
		cond = 0;
		cond_ext = 0;
		ann_sign_in = 0;
		announ = true;
	}
	int32_t id;
	int8_t quality;//从低到高
	Int_Double_Map prop;
	int32_t validity; // 有效期
	Title_Cfg_Detail_Condition condition;
	std::string name;
	std::string des;
	int16_t cond_type;
	int cond;
	int cond_ext;
	int ann_sign_in; // 上线公告号
	bool announ;
};
typedef boost::unordered_map<int32_t, Title_Cfg_Detail> Title_Cfg;


#endif /* TITLE_CONFIG_H_ */
