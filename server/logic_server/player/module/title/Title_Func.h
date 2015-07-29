/*
 * Title_Func.h
 *
 *  Created on: 2014年7月24日
 *      Author: Linqiyou
 */

#ifndef TITLE_FUNC_H_
#define TITLE_FUNC_H_

#include "Title_Struct.h"

inline int insert_title_and_sort(Title_Detail_Info_Detail *info, Title_Detail_Info_List &list) {
	if (!info || 0 == info->last_time) {
		return 0;// 永久buff
	}
	Title_Detail_Info_List::iterator iter = list.begin();
	for (; iter != list.end(); ++iter) {
		if (info->last_end_time < (**iter).last_end_time) {
			list.insert(iter, info);
			break;
		}
	}

	if (iter == list.end()) {
		list.push_back(info);
	}

	return 0;
}


#endif /* TITLE_FUNC_H_ */
