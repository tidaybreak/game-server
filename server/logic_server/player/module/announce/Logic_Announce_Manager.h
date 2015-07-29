/*
 * Logic_Announce_Manager.h
 *
 *  Created on: May 12, 2014
 *      Author: caijianwei
 */

#ifndef Logic_Announce_Manager_H_
#define Logic_Announce_Manager_H_

#include "announcement/Annoucement.h"

class Logic_Announce_Manager: public Annoucement {
public:
	Logic_Announce_Manager();
	virtual ~Logic_Announce_Manager();

	virtual void announce_send();

	void create_ann_active_content_count(const int32_t id);
	void create_ann_success_protect_dragon_vale(const std::string server_name, const std::string &player_name,
			int64_t role_id);
	void create_ann_gang_rank_change(const int rank, const gang_id_t gang_id);
	void create_ann_gang_fund_lack(const int day, const gang_id_t gang_id);
	void create_ann_answer_rank_first(role_id_t role_id);
};

typedef Singleton<Logic_Announce_Manager> Logic_Announce_Manager_Single;
#define LOGIC_ANN_MANAGER    (Logic_Announce_Manager_Single::instance())

#endif /* Logic_Announce_Manager_H_ */
