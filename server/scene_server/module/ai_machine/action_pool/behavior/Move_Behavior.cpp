/*
 * Move_Behavior.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */


#include "Move_Behavior.h"
#include "NPC/NPC_Manager.h"
#include "Move_Scene.h"
//#include "Map_Manager.h"
//#include "Map_Player.h"

const int MIN_MOVE_GRID_NUM = 3;			//开始移动的步数


/// be_移动到目标
Bev_Running_Status Be_Move_Mode_A::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();
	Monster* leader;
	if(monster->get_leader_id() != 0 &&
			(leader = NPC_MANAGER->find_monster(monster->get_leader_id())) != NULL)
	{
		if(monster->walk_path().has_walk_coord())
		{
			Coord coord;

			monster->walk_path().get_next_coord(coord);
			if (monster->move_scene()->grid_mover_count(coord, monster->mover_detail().layer_id) > 0) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
				}
			}
		}
		else
		{// move to leader
			int distance = calculate_pixels_distance(monster->mover_detail().grid_coord, leader->mover_detail().grid_coord);
			if(distance <= TEAMMATE_MIN_DISTANCE)
			{
				return k_BRS_SUCCESS;
			}
			else
			{
				//random pos
				Coord target_coord;
				Coord leader_coord = leader->mover_detail().grid_coord;
				monster->get_move_inner_coord(leader_coord, target_coord);
				monster->path_find(monster->mover_detail().grid_coord, target_coord, monster->walk_path().walk_coord, false);
				monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
				if (monster->walk_path().curr_coord_index > 0) {
					Coord_Vec coord_vec;
					int remain_move_num = monster->get_gauge_coord_num();
					int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
					if(move_num > monster->walk_path().curr_coord_index){
						move_num = monster->walk_path().curr_coord_index;
					}

					for(int i = 0; i < move_num; ++i){
						Coord coord;
						monster->walk_path().get_next_coord(coord);
						coord_vec.push_back(coord.pixel_coord());
					}
					if(move_num > 0){
						monster->move_inner_vec(coord_vec);
						monster->push_watcher_tick(0, 100000);
					}

//					Coord coord;
//					monster->walk_path().get_next_coord(coord);
//					if (monster->move_inner(coord.x, coord.y) != 0) {
//						monster->walk_path().curr_coord_index++;
//					}
//					//monster->push_watcher_tick(0, 1000);
//					monster->push_watcher_tick(1, 0);
				} else {
					MSG_DEBUG("patrol A scene:%d type id:%d x:%d y:%d => x:%d y:%d",monster->move_scene_id(), monster->npc_type_id(),monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, target_coord.x, target_coord.y);
					monster->push_watcher_tick(1);
				}
			}
		}
		return k_BRS_SUCCESS;
	}else
	{
		if (!monster->walk_path().has_walk_path()) {
			monster->walk_path().reset();
			monster->walk_path().walk_path = monster->npc_record()->config.walk_path;
			monster->walk_path().curr_path_index = monster->ai_running_info().path_index;

			if (monster->npc_status() != NPC::MOVE_TARGET) {
				monster->set_npc_status(NPC::MOVE_TARGET);
			}
		}

		if (monster->walk_path().has_walk_coord()) {
			Coord coord;
			monster->walk_path().get_next_coord(coord);

			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
				}
			}
		} else {
			Coord coord, available_coord;
			monster->walk_path().get_last_path(coord);
			monster->ai_running_info().path_index = monster->walk_path().curr_path_index;
			if (coord == monster->grid_coord()) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->walk_path().clean_walk_coord();
			monster->path_find(monster->grid_coord(), coord, monster->walk_path().walk_coord, false);

			if (monster->walk_path().walk_coord.empty()) {
				monster->push_watcher_tick(1);
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
			if (monster->walk_path().curr_coord_index > 0) {
				Coord_Vec coord_vec;
				int remain_move_num = monster->get_gauge_coord_num();
				int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
				if(move_num > monster->walk_path().curr_coord_index){
					move_num = monster->walk_path().curr_coord_index;
				}
				for(int i = 0; i < move_num; ++i){
					Coord coord;
					monster->walk_path().get_next_coord(coord);
					coord_vec.push_back(coord.pixel_coord());
				}
				if(move_num > 0){
					monster->move_inner_vec(coord_vec);
					monster->push_watcher_tick(0, 100000);
				}

//				Coord coord;
//				monster->walk_path().get_next_coord(coord);
//				if (monster->move_inner(coord.x, coord.y) != 0) {
//					monster->walk_path().curr_coord_index++;
//				}
//				monster->push_watcher_tick(0, 1000);
			}
		}

		return k_BRS_SUCCESS;
	}
}


/// be_D移动模式
Bev_Running_Status Be_Move_Mode_D::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();

	Monster* leader;
	if(monster->get_leader_id() != 0 &&
			(leader = NPC_MANAGER->find_monster(monster->get_leader_id())) != NULL)
	{
		if(monster->walk_path().has_walk_coord())
		{
			Coord coord;

			monster->walk_path().get_next_coord(coord);
			if (monster->move_scene()->grid_mover_count(coord, monster->mover_detail().layer_id) > 0) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
				}
			}
		}
		else
		{// move to leader
			int distance = calculate_pixels_distance(monster->mover_detail().grid_coord, leader->mover_detail().grid_coord);
			if(distance <= TEAMMATE_MIN_DISTANCE)
			{
				return k_BRS_SUCCESS;
			}
			else
			{
				//random pos
				Coord target_coord;
				Coord leader_coord = leader->mover_detail().grid_coord;
				monster->get_move_inner_coord(leader_coord, target_coord);
				monster->path_find(monster->mover_detail().grid_coord, target_coord, monster->walk_path().walk_coord, false);
				monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
				if (monster->walk_path().curr_coord_index > 0) {
					Coord_Vec coord_vec;
					int remain_move_num = monster->get_gauge_coord_num();
					int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
					if(move_num > monster->walk_path().curr_coord_index){
						move_num = monster->walk_path().curr_coord_index;
					}
					for(int i = 0; i < move_num; ++i){
						Coord coord;
						monster->walk_path().get_next_coord(coord);
						coord_vec.push_back(coord.pixel_coord());
					}
					if(move_num > 0){
						monster->move_inner_vec(coord_vec);
						monster->push_watcher_tick(0, 100000);
					}

//					Coord coord;
//					monster->walk_path().get_next_coord(coord);
//					if (monster->move_inner(coord.x, coord.y) != 0) {
//						if(monster->walk_path().curr_coord_index > 0){
//							monster->walk_path().curr_coord_index++;
//						}
//					}
//					//monster->push_watcher_tick(0, 1000);
//					monster->push_watcher_tick(1, 0);
				} else {
					MSG_DEBUG("patrol A scene:%d type id:%d x:%d y:%d => x:%d y:%d",monster->move_scene_id(), monster->npc_type_id(),monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, target_coord.x, target_coord.y);
					monster->push_watcher_tick(1);
				}
			}
		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if (!monster->walk_path().has_walk_path())
			return k_BRS_Failure;

		if (monster->walk_path().has_walk_coord()) {
			if (!monster->move_scene()) {
				MSG_USER_TRACE("ERROR_9: scnen null");
				return k_BRS_SUCCESS;
			}

			Coord coord;
			monster->walk_path().get_next_coord(coord);
			if (monster->move_scene()->mpt_value_by_coord(coord.x, coord.y) < Move_Scene::MOVE_ABLE_FROM) {
				monster->normal_die_self();
				return k_BRS_SUCCESS;
			}

			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
				}
			}
		} else {
			Coord target_coord;
			if (monster->walk_path().get_next_path(target_coord) != 0) {
				monster->normal_die_self();
				return k_BRS_SUCCESS;
			}

			std::vector<Coord> grid_path;
			monster->path_find(monster->mover_detail().grid_coord, target_coord,
					monster->walk_path().walk_coord, false, false);
			monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
			if (monster->walk_path().curr_coord_index > 0) {
				Coord_Vec coord_vec;
				int remain_move_num = monster->get_gauge_coord_num();
				int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
				if(move_num > monster->walk_path().curr_coord_index){
					move_num = monster->walk_path().curr_coord_index;
				}
				for(int i = 0; i < move_num; ++i){
					Coord coord;
					monster->walk_path().get_next_coord(coord);
					coord_vec.push_back(coord.pixel_coord());
				}
				if(move_num > 0){
					monster->move_inner_vec(coord_vec);
					monster->push_watcher_tick(0, 100000);
				}

//				Coord coord;
//				monster->walk_path().get_next_coord(coord);
//				if (monster->move_inner(coord.x, coord.y) != 0) {
//					monster->walk_path().curr_coord_index++;
//				}
//				monster->push_watcher_tick(0, 1000);
			}
		}

		return k_BRS_SUCCESS;
	}
}


///   be_B移动模式
Bev_Running_Status Be_Move_Mode_B::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();
	Monster* leader;
	if(monster->get_leader_id() != 0 &&
			(leader = NPC_MANAGER->find_monster(monster->get_leader_id())) != NULL)
	{
		if(monster->walk_path().has_walk_coord())
		{
			Coord coord;

			monster->walk_path().get_next_coord(coord);
			if (monster->move_scene()->grid_mover_count(coord, monster->mover_detail().layer_id) > 0) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
				}
			}
		}
//		else
//		{// move to leader
//			int distance = calculate_pixels_distance(monster->mover_detail().grid_coord, leader->mover_detail().grid_coord);
//			if(distance <= TEAMMATE_MIN_DISTANCE)
//			{
//				return k_BRS_SUCCESS;
//			}
//			else
//			{
//				//random pos
//				Coord target_coord;
//				Coord leader_coord = leader->mover_detail().grid_coord;
//				monster->get_move_inner_coord(leader_coord, target_coord);
//				monster->path_find(monster->mover_detail().grid_coord, target_coord, monster->walk_path().walk_coord, false);
//				monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
//				if (monster->walk_path().curr_coord_index > 0) {
//					Coord coord;
//					monster->walk_path().get_next_coord(coord);
//					if (monster->move_inner(coord.x, coord.y) != 0) {
//						monster->walk_path().curr_coord_index++;
//					}
//					//monster->push_watcher_tick(0, 1000);
//					monster->push_watcher_tick(1, 0);
//				} else {
//					MSG_DEBUG("patrol A scene:%d type id:%d x:%d y:%d => x:%d y:%d",monster->scene_id(), monster->npc_type_id(),monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, target_coord.x, target_coord.y);
//				}
//			}
//		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if (!monster->walk_path().has_walk_path()) {
			monster->walk_path().reset();
			monster->walk_path().walk_path = monster->npc_record()->config.walk_path;

			if (monster->npc_status() != NPC::MOVE_TARGET) {
				monster->set_npc_status(NPC::MOVE_TARGET);
			}
		}

		if (monster->walk_path().has_walk_coord()) {
			Coord coord;
			monster->walk_path().get_next_coord(coord);

			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
				}
			}
		} else {
			Coord coord, available_coord;
			monster->walk_path().get_loop_path(coord);
			if (coord == monster->grid_coord()) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->walk_path().clean_walk_coord();
			monster->path_find(monster->grid_coord(), coord, monster->walk_path().walk_coord, false);

			if (monster->walk_path().walk_coord.empty()) {
				MSG_DEBUG("AI: monster fight path find error npc_type:%d, scene:%d  now=> x:%d  y:%d target=> x:%d  y:%d",
						monster->npc_type_id(), monster->move_scene_id(), monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, coord.x, coord.y);
				monster->push_watcher_tick(1);
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
			// ask teammate to move
			monster->teammate_move(coord);
//			if (monster->walk_path().curr_coord_index > 0) {
//				Coord coord;
//				monster->walk_path().get_next_coord(coord);
//				if (monster->move_inner(coord.x, coord.y) != 0) {
//					monster->walk_path().curr_coord_index++;
//				}
//				monster->push_watcher_tick(0, 1000);
//			}
			monster->push_watcher_tick(0,1000);
		}

		return k_BRS_SUCCESS;
	}

}


/// be_A巡逻模式
Bev_Running_Status Be_Patrol_Mode_A::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();
	Monster* leader;
	if(monster->get_leader_id() != 0 &&
			(leader = NPC_MANAGER->find_monster(monster->get_leader_id())) != NULL)
	{
		if(monster->walk_path().has_walk_coord())
		{
			Coord coord;

			monster->walk_path().get_next_coord(coord);
			if (monster->move_scene()->grid_mover_count(coord, monster->mover_detail().layer_id) > 0) {
				monster->walk_path().reset();
				return k_BRS_SUCCESS;
			}

			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
				}
			}
		}
//		else
//		{// move to leader
//			int distance = calculate_pixels_distance(monster->mover_detail().grid_coord, leader->mover_detail().grid_coord);
//			if(distance <= TEAMMATE_MIN_DISTANCE)
//			{
//				return k_BRS_SUCCESS;
//			}
//			else
//			{
//				//random pos
//				Coord target_coord;
//				Coord leader_coord = leader->mover_detail().grid_coord;
//				monster->get_move_inner_coord(leader_coord, target_coord);
//				monster->path_find(monster->mover_detail().grid_coord, target_coord, monster->walk_path().walk_coord, false);
//				monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
//				if (monster->walk_path().curr_coord_index > 0) {
//					Coord coord;
//					monster->walk_path().get_next_coord(coord);
//					if (monster->move_inner(coord.x, coord.y) != 0) {
//						monster->walk_path().curr_coord_index++;
//					}
//					monster->push_watcher_tick(0, 1000);
//					//monster->push_watcher_tick(1, 0);
//				}
//				else
//				{
//					MSG_DEBUG("patrol A scene:%d type id:%d x:%d y:%d => x:%d y:%d",monster->scene_id(), monster->npc_type_id(),monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, target_coord.x, target_coord.y);
//				}
//			}
//		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if (monster->walk_path().has_walk_path()) {
			if (monster->walk_path().has_walk_coord()) {
				Coord coord;
				monster->walk_path().get_next_coord(coord);
				if (monster->move_scene()->grid_mover_count(coord, monster->mover_detail().layer_id) > 0) {
					monster->walk_path().reset();
					return k_BRS_SUCCESS;
				}
				monster->move_watcher_tick(coord);
				if (monster->move_inner(coord.x, coord.y) != 0) {
					if(monster->walk_path().curr_coord_index > 0){
						monster->walk_path().curr_coord_index++;
					}
				}
			} else {
				// 超过3个格子没有走完的，返回
				if(monster->get_gauge_coord_num() >= MIN_MOVE_GRID_NUM){
					return k_BRS_SUCCESS;
				}
				Coord target_coord;
				if (monster->walk_path().get_next_path(target_coord) != 0 ||
						(target_coord != monster->birth_coord().grid_coord() && monster->move_scene()->grid_mover_count(target_coord) > 0)) {
					monster->walk_path().reset();
					return k_BRS_SUCCESS;
				}

				monster->path_find(monster->mover_detail().grid_coord, target_coord, monster->walk_path().walk_coord, false);

				monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
				if (monster->walk_path().curr_coord_index > 0) {
					int remain_move_num = monster->get_gauge_coord_num();
					int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
					Coord_Vec coord_vec;
					if(move_num > monster->walk_path().curr_coord_index){
						move_num = monster->walk_path().curr_coord_index;
					}
					for(int i = 0; i < move_num; ++i){
						Coord coord;
						monster->walk_path().get_next_coord(coord);
						coord_vec.push_back(coord.pixel_coord());
					}
					if(move_num > 0){
						monster->move_inner_vec(coord_vec);
						monster->push_watcher_tick(0, 100000);
					}

//					Coord coord;
//					monster->walk_path().get_next_coord(coord);
//					monster->push_watcher_tick(0, 5000);
//					if (monster->move_inner(coord.x, coord.y) != 0) {
//						monster->walk_path().curr_coord_index++;
//					}
					monster->teammate_move(target_coord);
				} else {
					MSG_DEBUG("patrol A scene:%d type id:%d x:%d y:%d => x:%d y:%d",monster->move_scene_id(), monster->npc_type_id(),monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, target_coord.x, target_coord.y);
					monster->push_watcher_tick(3,0);
				}
			}

			return k_BRS_SUCCESS;
		}

		// 设置巡逻状态
		if (monster->npc_status() != NPC::PATROL_STATUS) {
			monster->set_npc_status(NPC::PATROL_STATUS);
		}

		// 巡逻距离为0表示不巡逻
		if (npc->npc_record()->config.move_range == 0) {
			return k_BRS_Failure;
		}

		// 巡逻tick
		if (!monster->ai_running_info().tick_patrol_.cycle_interval()) {
			return k_BRS_SUCCESS;
		}
		// 等移动完成后，再走
		if(monster->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		// 返回或者随机移动
		int distance = calculate_pixels_distance(monster->grid_coord(), monster->birth_coord().grid_coord());
		if (distance > npc->npc_record()->config.move_range) {
			monster->walk_path().walk_path.push_back(monster->birth_coord().grid_coord());
		} else {
			monster->ai_running_info().tick_patrol_.set_tick_now();
			if(monster->play_chat() == 0){
				int rand_sec = 2 + random()%2;
				monster->ai_running_info().tick_patrol_.set_interval(rand_sec, 0);
				return k_BRS_SUCCESS;
			}
			monster->teammate_play_chat();
			Coord target_coord;
			/// 随机一个点
//			static bool test_i = false;
//			int rand_test = random()%2;
//			if(test_i){
//				if( rand_test == 0){
//					target_coord.x = monster->grid_coord().x + 5;
//					target_coord.y = monster->grid_coord().y;
//				}else{
//					target_coord.x = monster->grid_coord().x;
//					target_coord.y = monster->grid_coord().y + 5;
//				}
//
//			}else{
//				if( rand_test == 0){
//					target_coord.x = monster->grid_coord().x - 5;
//					target_coord.y = monster->grid_coord().y;
//				}else{
//					target_coord.x = monster->grid_coord().x;
//					target_coord.y = monster->grid_coord().y - 5;
//				}
//			}
//			test_i = !test_i;

			int r = 3 + random()%3;
			double rndn = random()%101;
			double n = rndn/100*3.1415;
			target_coord.x = monster->grid_coord().x+r*sin(n);
			target_coord.y = monster->grid_coord().y+r*cos(n);
			uint16_t pix = grid_x_to_pixel(target_coord.x);
			uint16_t piy = grid_y_to_pixel(target_coord.y);
			if (!monster->validate_move_value(pix, piy ) )
			{
				monster->ai_running_info().tick_patrol_.set_interval(1 + random()%2, 0);
				return k_BRS_SUCCESS;
			}

			/// 是否没人地方
			if (monster->move_scene()->calculate_coord_has_mover(monster->mover_detail().layer_id, target_coord, 0, 2)) {
				int rand_sec = 1 + random()%2;
				monster->ai_running_info().tick_patrol_.set_interval(rand_sec, 0);
				return k_BRS_SUCCESS;
			}

			monster->walk_path().walk_path.push_back(target_coord);
		}

		int rand_sec = 3 + random()%2;
		monster->ai_running_info().tick_patrol_.set_interval(rand_sec, 0);

		return k_BRS_SUCCESS;
	}

}

Bev_Running_Status Be_Chase_Mode::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();

	Mover* player = monster->move_scene()->find_mover_with_layer(monster->mover_detail().layer_id,
			monster->most_hatred_role().role_id);
	if(player == NULL || (player && player->is_in_battle())){
		if(monster->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		//back
		monster->set_npc_status(NPC::BACK_STATUS);
		monster->most_hatred_role().role_id = 0;
		monster->walk_path().clean_walk_coord();
		monster->walk_path().reset();
		monster->walk_path().walk_path = monster->npc_record()->config.walk_path;
		monster->path_find(monster->grid_coord(), monster->npc_detail().chase_coord, monster->walk_path().walk_coord, false);
		monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
		if (monster->walk_path().curr_coord_index > 0)
		{
			int remain_move_num = monster->get_gauge_coord_num();
			int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
			Coord_Vec coord_vec;
			if(move_num > monster->walk_path().curr_coord_index){
				move_num = monster->walk_path().curr_coord_index;
			}
			for(int i = 0; i < move_num; ++i){
				Coord coord;
				monster->walk_path().get_next_coord(coord);
				coord_vec.push_back(coord.pixel_coord());
			}
			if(move_num > 0){
				monster->move_inner_vec(coord_vec);
				monster->push_watcher_tick(0, 100000);
			}

//			Coord coord;
//			monster->walk_path().get_next_coord(coord);
//			monster->push_watcher_tick(0, 5000);
//			//monster->move_watcher_tick(coord);//test
//			if (monster->move_inner(coord.x, coord.y) != 0)
//			{
//				monster->walk_path().curr_coord_index++;
//			}
		}else{
			monster->push_watcher_tick(1);
		}
		return k_BRS_SUCCESS;
	}
	//
	Coord target_coord = player->mover_detail().grid_coord;
	int distance = calculate_pixels_distance(monster->grid_coord(), monster->npc_detail().chase_coord);
	if(distance > monster->npc_record()->config.chase_range){
		if(monster->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		//back
		monster->set_npc_status(NPC::BACK_STATUS);
		monster->most_hatred_role().role_id = 0;
		monster->walk_path().clean_walk_coord();
		monster->walk_path().reset();
		monster->walk_path().walk_path = monster->npc_record()->config.walk_path;
		monster->path_find(monster->mover_detail().grid_coord, monster->npc_detail().chase_coord, monster->walk_path().walk_coord, false);
		monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
		if (monster->walk_path().curr_coord_index > 0)
		{
			Coord_Vec coord_vec;
			int remain_move_num = monster->get_gauge_coord_num();
			int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
			if(move_num > monster->walk_path().curr_coord_index){
				move_num = monster->walk_path().curr_coord_index;
			}
			for(int i = 0; i < move_num; ++i){
				Coord coord;
				monster->walk_path().get_next_coord(coord);
				coord_vec.push_back(coord.pixel_coord());
			}
			if(move_num){
				monster->move_watcher_tick(coord_vec[0]);
				monster->move_inner_vec(coord_vec);
			}

//			Coord coord;
//			monster->walk_path().get_next_coord(coord);
//			monster->push_watcher_tick(0, 5000);
//			//monster->move_watcher_tick(coord);//test
//			if (monster->move_inner(coord.x, coord.y) != 0)
//			{
//				monster->walk_path().curr_coord_index++;
//			}
		}else{
			monster->push_watcher_tick(1);
		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if(monster->walk_path().has_walk_coord()){
			Coord coord;
			monster->walk_path().get_next_coord(coord);
			monster->move_watcher_tick(coord);
			if (monster->move_inner(coord.x, coord.y) != 0) {
				if(monster->walk_path().curr_coord_index > 0){
					monster->walk_path().curr_coord_index++;
					monster->walk_path().move_gride_count--;
				}
			}
			if(monster->walk_path().move_gride_count >= 5){
				monster->walk_path().clean_walk_coord();
			}
			return k_BRS_SUCCESS;
		}
		if(monster->get_gauge_coord_num() > 2){
			return k_BRS_SUCCESS;
		}else{
			monster->path_find(monster->mover_detail().grid_coord, target_coord, monster->walk_path().walk_coord, false);
			monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
			if (monster->walk_path().curr_coord_index > 0)
			{
				Coord_Vec coord_vec;
				int remain_move_num = monster->get_gauge_coord_num();
				int move_num = MIN_MOVE_GRID_NUM - remain_move_num;
				if(move_num > monster->walk_path().curr_coord_index){
					move_num = monster->walk_path().curr_coord_index;
				}
				for(int i = 0; i < move_num; ++i){
					Coord coord;
					monster->walk_path().get_next_coord(coord);
					coord_vec.push_back(coord.pixel_coord());
				}
				if(move_num > 0){
					monster->move_watcher_tick(coord_vec[0]);
					monster->move_inner_vec(coord_vec);
					//monster->push_watcher_tick(0, 100000);
				}

//				Coord coord;
//				monster->walk_path().get_next_coord(coord);
//				monster->push_watcher_tick(0, 5000);
//				//monster->move_watcher_tick(coord);test
//				if (monster->move_inner(coord.x, coord.y) != 0)
//				{
//					monster->walk_path().curr_coord_index++;
//				}

				//monster->push_watcher_tick(0, 1000);
				//monster->push_watcher_tick(1, 0);
				//monster->teammate_move(target_coord);
			}
			return k_BRS_SUCCESS;
		}
	}
	return k_BRS_SUCCESS;
}

Bev_Running_Status Be_Back_Mode::on_update(NPC *npc) {
	Monster *monster = npc->monster_self();
	if(monster->walk_path().has_walk_coord())
	{
		Coord coord;
		monster->walk_path().get_next_coord(coord);
		monster->move_watcher_tick(coord);
		if (monster->move_inner(coord.x, coord.y) != 0) {
			if(monster->walk_path().curr_coord_index > 0){
				monster->walk_path().curr_coord_index++;
			}
		}
		return k_BRS_SUCCESS;
	}
	else
	{
		if(monster->get_gauge_coord_num() > 0){
			return k_BRS_SUCCESS;
		}
		monster->set_npc_status(NPC::NORMAL_STATUS);
		return k_BRS_SUCCESS;
	}
	return k_BRS_Failure;
}

Bev_Running_Status Be_Pet_Mode::on_update(NPC *npc) {
//	Monster *monster = npc->monster_self();
//
//	if (monster->monster_status() == Monster::FOLLOW_STATUS) {
//		if (!monster->hatred_map().empty())
//			monster->del_all_hatred();
//	}
//
//	Fighter *player = monster->find_fighter_in_same_scene(monster->master_id());
//	if (player == 0 || player->dead_status()) {
//		monster->normal_die_self();
//		MSG_DEBUG("npc:%d manster%ld not find", npc->npc_type_id(), monster->master_id());
//		return k_BRS_SUCCESS;
//	}
//
//	if (monster->hatred_map().empty()) {
//		if (monster->monster_status() == Monster::FIGHT_STATUS || monster->monster_status() == Monster::CHASE_STATUS) {
//			monster->set_monster_status(Monster::NORMAL_STATUS);
//		}
//	}
//
//	int distance = calculate_pixels_distance(monster->mover_detail().grid_coord, player->grid_coord());
//	int max_distance = monster->monster_status() == Monster::FIGHT_STATUS || monster->monster_status() == Monster::CHASE_STATUS ? 400 : 120;
//
//	// 太远直接拉回
//	if (distance > max_distance * 3) {
//		monster->move_disappear();
//		monster->mover_detail().grid_coord = player->grid_coord();
//		monster->appear();
//		return k_BRS_SUCCESS;
//	}
//
//	if (distance < max_distance) {
//		if (monster->monster_status() == Monster::FOLLOW_STATUS)
//			monster->set_monster_status(Monster::NORMAL_STATUS);
//		return k_BRS_Failure;
//	}
//
//	// 移动
//	if (monster->monster_status() == Monster::FOLLOW_STATUS) {
//		if (monster->walk_path().has_walk_coord()) {
//			Coord coord;
//			int distance = calculate_pixels_distance(player->grid_coord(), monster->walk_path().walk_coord.front());
//			if (distance >= 40 * (random()%3 + 1)) {
//				monster->walk_path().reset();
//				monster->push_watcher_tick(0, 1000);
//				return k_BRS_SUCCESS;
//			}
//
//			monster->walk_path().get_next_coord(coord);
//
//			monster->move_watcher_tick(coord);
//			if (monster->move_inner(coord.x, coord.y) != 0) {
//				monster->walk_path().curr_coord_index++;
//			}
//		} else {
//			int master_speed = player->speed_sum();
//			if (monster->speed_value() != master_speed)
//				monster->speed_additional(0, master_speed);
//
//			monster->path_find(monster->grid_coord(), player->grid_coord(), monster->walk_path().walk_coord, false);
//			monster->walk_path().curr_coord_index = monster->walk_path().walk_coord.size();
//			if (monster->walk_path().curr_coord_index > 0) {
//				Coord coord;
//				monster->walk_path().get_next_coord(coord);
//				if (monster->move_inner(coord.x, coord.y) != 0) {
//					monster->walk_path().curr_coord_index++;
//				}
//				monster->push_watcher_tick(0, 100);
//			} else {
//				MSG_DEBUG("patrol pet scene:%d type id:%d x:%d y:%d => x:%d y:%d",monster->scene_id(), monster->npc_type_id(),monster->mover_detail().grid_coord.x, monster->mover_detail().grid_coord.y, player->grid_coord().x, player->gird_coord().y);
//				monster->normal_die_self();
//			}
//		}
//
//		return k_BRS_SUCCESS;
//	}
//
//	// 跟随状态
//	if (distance > max_distance) {
//		monster->del_all_hatred();
//		monster->set_monster_status(Monster::FOLLOW_STATUS);
//		monster->push_watcher_tick(0, 1000);
//		return k_BRS_SUCCESS;
//	}
//
//	if (!monster->ai_running_info().tick_patrol_.cycle_interval())
//		return k_BRS_Failure;
//
//	// 人物仇恨传递
//	if (player->most_hatred_role().role_id) {
//		Fighter *fighter = monster->find_fighter_in_same_scene(player->most_hatred_role().role_id);
//		if (fighter != 0)
//			monster->add_to_hatred(player->most_hatred_role().role_id);
//		else
//			player->del_from_hatred(player->most_hatred_role().role_id);
//	}

	return k_BRS_Failure;
}
