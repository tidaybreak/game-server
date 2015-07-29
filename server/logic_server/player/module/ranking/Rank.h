/*
 * Ranking.h
 *
 *  Created on: Mar 7, 2014
 *      Author: zhouyajun
 */

#ifndef RANKING_H_
#define RANKING_H_

#include "Logic_Player_Base.h"
#include "Ranking_Def.h"

class Rank : virtual public Logic_Player_Base {
public:
	Rank(void);
	virtual ~Rank();
	void reset(void);
	int client_ready_now(void);

	int req_ranking_info(int32_t type);
	int req_ranking_role_info(int32_t type, uint32_t rank_);
	int req_ranking_heroer_info(int32_t type, uint32_t rank_);
	int req_ranking_item_info(int32_t type, uint32_t rank_, role_id_t role_id, uint32_t career);

private:

};


#endif /* RANKING_H_ */
