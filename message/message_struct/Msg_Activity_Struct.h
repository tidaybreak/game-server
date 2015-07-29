/*
* Generate by devtool
*/

#ifndef _MSG_ACTIVITY_H_
#define _MSG_ACTIVITY_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
请求活动信息
*/
struct MSG_10106400  {
	MSG_10106400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息(返回)
*/
struct MSG_50106400 : public Base_Msg  {
	uint32_t limit_buy_end_time;
	std::vector<Goods_Info> limit_buy_info;
	uint32_t vip_club_end_time;
	std::vector<Uint_Uint> vip_club_progress;
	std::vector<Uint_Uint> vip_club_reward;
	uint32_t level_ace_end_time;
	std::vector<Uint_Uint> leve_ace_reward;
	uint32_t suit_collect_end_time;
	std::vector<uint32_t> suit_collect_id;
	std::vector<Uint_Uint> suit_collect_reward;
	uint32_t guild_level_end_time;
	std::vector<Uint_Uint> guild_level_reward;
	uint32_t level_rank_end_time;
	uint32_t level_rank_count_time;
	uint32_t level_rank;
	std::vector<Role_Info> level_rank_role_info;
	uint32_t mount_rank_end_time;
	uint32_t mount_rank_count_time;
	uint32_t mount_rank;
	std::vector<Role_Info> mount_rank_role_info;
	std::vector<Uint_Uint> mount_rank_reward;
	uint32_t wing_rank_end_time;
	uint32_t wing_rank_count_time;
	uint32_t wing_rank;
	std::vector<Role_Info> wing_rank_role_info;
	std::vector<Uint_Uint> wing_rank_reward;
	uint32_t arena_rank_end_time;
	uint32_t arena_rank_count_time;
	uint32_t arena_rank;
	std::vector<Role_Info> arena_rank_role_info;
	uint32_t force_rank_end_time;
	uint32_t force_rank_count_time;
	uint32_t force_rank;
	std::vector<Role_Info> force_rank_role_info;
	uint32_t gem_levelup_end_time;
	std::vector<Uint_Uint> gem_levelup_reward;
	std::vector<Uint_Uint> gem_levelup_progress;
	uint32_t wing_levelup_end_time;
	std::vector<Uint_Uint> wing_levelup_reward;
	uint32_t mount_levelup_end_time;
	std::vector<Uint_Uint> mount_levelup_reward;
	std::vector<Uint_Uint> mount_levelup_progress;
	uint32_t sg_rank_end_time;
	uint32_t sg_rank_count_time;
	uint32_t sg_rank;
	std::vector<Role_Info> sg_rank_role_info;
	std::vector<Uint_Uint> sg_rank_reward;
	uint32_t arena_dan_end_time;
	std::vector<Uint_Uint> arena_dan_reward;
	std::vector<Uint_Uint> arena_dan_progress;
	uint32_t world_boss_end_time;
	uint32_t world_boss_all_hurt;
	uint32_t world_boss_today_hurt;
	uint32_t world_boss_rank;
	std::vector<Uint_Uint> world_boss_reward;
	uint32_t suit_collect_two_end_time;
	std::vector<uint32_t> suit_collect_two_id;
	std::vector<Uint_Uint> suit_collect_two_reward;
	uint32_t heroes_door_end_time;
	std::vector<Uint_Uint> heroes_door_reward;
	uint32_t team_arena_end_time;
	uint32_t team_arena_win_num;
	std::vector<Uint_Uint> team_arena_reward;
	uint32_t warer_end_time;
	uint32_t warer_win_num;
	std::vector<Uint_Uint> warer_reward;
	uint32_t reward_task_end_time;
	uint32_t reward_task_done_num;
	std::vector<Uint_Uint> reward_task_reward;
	uint32_t sg_fb_end_time;
	std::vector<Uint_Uint> sg_fb_reward;
	uint32_t reward_task_fs_end_time;
	uint32_t sg_welfare_end_time;
	uint32_t fm_x_ci_end_time;
	uint32_t fm_x_ci_num;
	std::vector<Uint_Uint> fm_x_ci_reward;
	uint32_t x_x_quality_heroer_end_time;
	std::vector<Uint_Uint> x_x_quality_heroer_reward;
	uint32_t mount_lvup_x_end_time;
	uint32_t mount_lvup_x_num;
	std::vector<Uint_Uint> mount_lvup_x_reward;
	uint32_t fusion_x_elite_card_end_time;
	uint32_t fusion_x_elite_card_num;
	std::vector<Uint_Uint> fusion_x_elite_card_reward;
	uint32_t alchemy_gift_end_time;
	uint32_t alchemy_gift_num;
	std::vector<Uint_Uint> alchemy_gift_reward;
	uint32_t online_reward_gift_end_time;
	uint32_t activity_gift_end_time;
	uint32_t activity_gift_num;
	std::vector<Uint_Uint> activity_gift_reward;
	uint32_t ring_task_end_time;
	uint32_t ring_task_num;
	std::vector<Uint_Uint> ring_task_reward;
	uint32_t dragon_prey_end_time;
	uint32_t dragon_prey_num;
	std::vector<Uint_Uint> dragon_prey_reward;
	MSG_50106400(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(limit_buy_end_time);
		uint16_t __limit_buy_info_vec_size = limit_buy_info.size();
		w.write_uint16(__limit_buy_info_vec_size);
		for(uint16_t i = 0; i < __limit_buy_info_vec_size; ++i) {
				limit_buy_info[i].serialize(w);
		}

		w.write_uint32(vip_club_end_time);
		uint16_t __vip_club_progress_vec_size = vip_club_progress.size();
		w.write_uint16(__vip_club_progress_vec_size);
		for(uint16_t i = 0; i < __vip_club_progress_vec_size; ++i) {
				vip_club_progress[i].serialize(w);
		}

		uint16_t __vip_club_reward_vec_size = vip_club_reward.size();
		w.write_uint16(__vip_club_reward_vec_size);
		for(uint16_t i = 0; i < __vip_club_reward_vec_size; ++i) {
				vip_club_reward[i].serialize(w);
		}

		w.write_uint32(level_ace_end_time);
		uint16_t __leve_ace_reward_vec_size = leve_ace_reward.size();
		w.write_uint16(__leve_ace_reward_vec_size);
		for(uint16_t i = 0; i < __leve_ace_reward_vec_size; ++i) {
				leve_ace_reward[i].serialize(w);
		}

		w.write_uint32(suit_collect_end_time);
		uint16_t __suit_collect_id_vec_size = suit_collect_id.size();
		w.write_uint16(__suit_collect_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_id_vec_size; ++i) {
				w.write_uint32(suit_collect_id[i]);
		}

		uint16_t __suit_collect_reward_vec_size = suit_collect_reward.size();
		w.write_uint16(__suit_collect_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_reward_vec_size; ++i) {
				suit_collect_reward[i].serialize(w);
		}

		w.write_uint32(guild_level_end_time);
		uint16_t __guild_level_reward_vec_size = guild_level_reward.size();
		w.write_uint16(__guild_level_reward_vec_size);
		for(uint16_t i = 0; i < __guild_level_reward_vec_size; ++i) {
				guild_level_reward[i].serialize(w);
		}

		w.write_uint32(level_rank_end_time);
		w.write_uint32(level_rank_count_time);
		w.write_uint32(level_rank);
		uint16_t __level_rank_role_info_vec_size = level_rank_role_info.size();
		w.write_uint16(__level_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __level_rank_role_info_vec_size; ++i) {
				level_rank_role_info[i].serialize(w);
		}

		w.write_uint32(mount_rank_end_time);
		w.write_uint32(mount_rank_count_time);
		w.write_uint32(mount_rank);
		uint16_t __mount_rank_role_info_vec_size = mount_rank_role_info.size();
		w.write_uint16(__mount_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __mount_rank_role_info_vec_size; ++i) {
				mount_rank_role_info[i].serialize(w);
		}

		uint16_t __mount_rank_reward_vec_size = mount_rank_reward.size();
		w.write_uint16(__mount_rank_reward_vec_size);
		for(uint16_t i = 0; i < __mount_rank_reward_vec_size; ++i) {
				mount_rank_reward[i].serialize(w);
		}

		w.write_uint32(wing_rank_end_time);
		w.write_uint32(wing_rank_count_time);
		w.write_uint32(wing_rank);
		uint16_t __wing_rank_role_info_vec_size = wing_rank_role_info.size();
		w.write_uint16(__wing_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __wing_rank_role_info_vec_size; ++i) {
				wing_rank_role_info[i].serialize(w);
		}

		uint16_t __wing_rank_reward_vec_size = wing_rank_reward.size();
		w.write_uint16(__wing_rank_reward_vec_size);
		for(uint16_t i = 0; i < __wing_rank_reward_vec_size; ++i) {
				wing_rank_reward[i].serialize(w);
		}

		w.write_uint32(arena_rank_end_time);
		w.write_uint32(arena_rank_count_time);
		w.write_uint32(arena_rank);
		uint16_t __arena_rank_role_info_vec_size = arena_rank_role_info.size();
		w.write_uint16(__arena_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __arena_rank_role_info_vec_size; ++i) {
				arena_rank_role_info[i].serialize(w);
		}

		w.write_uint32(force_rank_end_time);
		w.write_uint32(force_rank_count_time);
		w.write_uint32(force_rank);
		uint16_t __force_rank_role_info_vec_size = force_rank_role_info.size();
		w.write_uint16(__force_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __force_rank_role_info_vec_size; ++i) {
				force_rank_role_info[i].serialize(w);
		}

		w.write_uint32(gem_levelup_end_time);
		uint16_t __gem_levelup_reward_vec_size = gem_levelup_reward.size();
		w.write_uint16(__gem_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_reward_vec_size; ++i) {
				gem_levelup_reward[i].serialize(w);
		}

		uint16_t __gem_levelup_progress_vec_size = gem_levelup_progress.size();
		w.write_uint16(__gem_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_progress_vec_size; ++i) {
				gem_levelup_progress[i].serialize(w);
		}

		w.write_uint32(wing_levelup_end_time);
		uint16_t __wing_levelup_reward_vec_size = wing_levelup_reward.size();
		w.write_uint16(__wing_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __wing_levelup_reward_vec_size; ++i) {
				wing_levelup_reward[i].serialize(w);
		}

		w.write_uint32(mount_levelup_end_time);
		uint16_t __mount_levelup_reward_vec_size = mount_levelup_reward.size();
		w.write_uint16(__mount_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_reward_vec_size; ++i) {
				mount_levelup_reward[i].serialize(w);
		}

		uint16_t __mount_levelup_progress_vec_size = mount_levelup_progress.size();
		w.write_uint16(__mount_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_progress_vec_size; ++i) {
				mount_levelup_progress[i].serialize(w);
		}

		w.write_uint32(sg_rank_end_time);
		w.write_uint32(sg_rank_count_time);
		w.write_uint32(sg_rank);
		uint16_t __sg_rank_role_info_vec_size = sg_rank_role_info.size();
		w.write_uint16(__sg_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __sg_rank_role_info_vec_size; ++i) {
				sg_rank_role_info[i].serialize(w);
		}

		uint16_t __sg_rank_reward_vec_size = sg_rank_reward.size();
		w.write_uint16(__sg_rank_reward_vec_size);
		for(uint16_t i = 0; i < __sg_rank_reward_vec_size; ++i) {
				sg_rank_reward[i].serialize(w);
		}

		w.write_uint32(arena_dan_end_time);
		uint16_t __arena_dan_reward_vec_size = arena_dan_reward.size();
		w.write_uint16(__arena_dan_reward_vec_size);
		for(uint16_t i = 0; i < __arena_dan_reward_vec_size; ++i) {
				arena_dan_reward[i].serialize(w);
		}

		uint16_t __arena_dan_progress_vec_size = arena_dan_progress.size();
		w.write_uint16(__arena_dan_progress_vec_size);
		for(uint16_t i = 0; i < __arena_dan_progress_vec_size; ++i) {
				arena_dan_progress[i].serialize(w);
		}

		w.write_uint32(world_boss_end_time);
		w.write_uint32(world_boss_all_hurt);
		w.write_uint32(world_boss_today_hurt);
		w.write_uint32(world_boss_rank);
		uint16_t __world_boss_reward_vec_size = world_boss_reward.size();
		w.write_uint16(__world_boss_reward_vec_size);
		for(uint16_t i = 0; i < __world_boss_reward_vec_size; ++i) {
				world_boss_reward[i].serialize(w);
		}

		w.write_uint32(suit_collect_two_end_time);
		uint16_t __suit_collect_two_id_vec_size = suit_collect_two_id.size();
		w.write_uint16(__suit_collect_two_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_id_vec_size; ++i) {
				w.write_uint32(suit_collect_two_id[i]);
		}

		uint16_t __suit_collect_two_reward_vec_size = suit_collect_two_reward.size();
		w.write_uint16(__suit_collect_two_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_reward_vec_size; ++i) {
				suit_collect_two_reward[i].serialize(w);
		}

		w.write_uint32(heroes_door_end_time);
		uint16_t __heroes_door_reward_vec_size = heroes_door_reward.size();
		w.write_uint16(__heroes_door_reward_vec_size);
		for(uint16_t i = 0; i < __heroes_door_reward_vec_size; ++i) {
				heroes_door_reward[i].serialize(w);
		}

		w.write_uint32(team_arena_end_time);
		w.write_uint32(team_arena_win_num);
		uint16_t __team_arena_reward_vec_size = team_arena_reward.size();
		w.write_uint16(__team_arena_reward_vec_size);
		for(uint16_t i = 0; i < __team_arena_reward_vec_size; ++i) {
				team_arena_reward[i].serialize(w);
		}

		w.write_uint32(warer_end_time);
		w.write_uint32(warer_win_num);
		uint16_t __warer_reward_vec_size = warer_reward.size();
		w.write_uint16(__warer_reward_vec_size);
		for(uint16_t i = 0; i < __warer_reward_vec_size; ++i) {
				warer_reward[i].serialize(w);
		}

		w.write_uint32(reward_task_end_time);
		w.write_uint32(reward_task_done_num);
		uint16_t __reward_task_reward_vec_size = reward_task_reward.size();
		w.write_uint16(__reward_task_reward_vec_size);
		for(uint16_t i = 0; i < __reward_task_reward_vec_size; ++i) {
				reward_task_reward[i].serialize(w);
		}

		w.write_uint32(sg_fb_end_time);
		uint16_t __sg_fb_reward_vec_size = sg_fb_reward.size();
		w.write_uint16(__sg_fb_reward_vec_size);
		for(uint16_t i = 0; i < __sg_fb_reward_vec_size; ++i) {
				sg_fb_reward[i].serialize(w);
		}

		w.write_uint32(reward_task_fs_end_time);
		w.write_uint32(sg_welfare_end_time);
		w.write_uint32(fm_x_ci_end_time);
		w.write_uint32(fm_x_ci_num);
		uint16_t __fm_x_ci_reward_vec_size = fm_x_ci_reward.size();
		w.write_uint16(__fm_x_ci_reward_vec_size);
		for(uint16_t i = 0; i < __fm_x_ci_reward_vec_size; ++i) {
				fm_x_ci_reward[i].serialize(w);
		}

		w.write_uint32(x_x_quality_heroer_end_time);
		uint16_t __x_x_quality_heroer_reward_vec_size = x_x_quality_heroer_reward.size();
		w.write_uint16(__x_x_quality_heroer_reward_vec_size);
		for(uint16_t i = 0; i < __x_x_quality_heroer_reward_vec_size; ++i) {
				x_x_quality_heroer_reward[i].serialize(w);
		}

		w.write_uint32(mount_lvup_x_end_time);
		w.write_uint32(mount_lvup_x_num);
		uint16_t __mount_lvup_x_reward_vec_size = mount_lvup_x_reward.size();
		w.write_uint16(__mount_lvup_x_reward_vec_size);
		for(uint16_t i = 0; i < __mount_lvup_x_reward_vec_size; ++i) {
				mount_lvup_x_reward[i].serialize(w);
		}

		w.write_uint32(fusion_x_elite_card_end_time);
		w.write_uint32(fusion_x_elite_card_num);
		uint16_t __fusion_x_elite_card_reward_vec_size = fusion_x_elite_card_reward.size();
		w.write_uint16(__fusion_x_elite_card_reward_vec_size);
		for(uint16_t i = 0; i < __fusion_x_elite_card_reward_vec_size; ++i) {
				fusion_x_elite_card_reward[i].serialize(w);
		}

		w.write_uint32(alchemy_gift_end_time);
		w.write_uint32(alchemy_gift_num);
		uint16_t __alchemy_gift_reward_vec_size = alchemy_gift_reward.size();
		w.write_uint16(__alchemy_gift_reward_vec_size);
		for(uint16_t i = 0; i < __alchemy_gift_reward_vec_size; ++i) {
				alchemy_gift_reward[i].serialize(w);
		}

		w.write_uint32(online_reward_gift_end_time);
		w.write_uint32(activity_gift_end_time);
		w.write_uint32(activity_gift_num);
		uint16_t __activity_gift_reward_vec_size = activity_gift_reward.size();
		w.write_uint16(__activity_gift_reward_vec_size);
		for(uint16_t i = 0; i < __activity_gift_reward_vec_size; ++i) {
				activity_gift_reward[i].serialize(w);
		}

		w.write_uint32(ring_task_end_time);
		w.write_uint32(ring_task_num);
		uint16_t __ring_task_reward_vec_size = ring_task_reward.size();
		w.write_uint16(__ring_task_reward_vec_size);
		for(uint16_t i = 0; i < __ring_task_reward_vec_size; ++i) {
				ring_task_reward[i].serialize(w);
		}

		w.write_uint32(dragon_prey_end_time);
		w.write_uint32(dragon_prey_num);
		uint16_t __dragon_prey_reward_vec_size = dragon_prey_reward.size();
		w.write_uint16(__dragon_prey_reward_vec_size);
		for(uint16_t i = 0; i < __dragon_prey_reward_vec_size; ++i) {
				dragon_prey_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(limit_buy_end_time)  )
				return -1;
 		uint16_t __limit_buy_info_vec_size;
		if(r.read_uint16(__limit_buy_info_vec_size)  )
				return -1;
		limit_buy_info.reserve(__limit_buy_info_vec_size);
		for(uint16_t i = 0; i < __limit_buy_info_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				limit_buy_info.push_back(v);
		}

		if( r.read_uint32(vip_club_end_time)  )
				return -1;
 		uint16_t __vip_club_progress_vec_size;
		if(r.read_uint16(__vip_club_progress_vec_size)  )
				return -1;
		vip_club_progress.reserve(__vip_club_progress_vec_size);
		for(uint16_t i = 0; i < __vip_club_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				vip_club_progress.push_back(v);
		}

		uint16_t __vip_club_reward_vec_size;
		if(r.read_uint16(__vip_club_reward_vec_size)  )
				return -1;
		vip_club_reward.reserve(__vip_club_reward_vec_size);
		for(uint16_t i = 0; i < __vip_club_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				vip_club_reward.push_back(v);
		}

		if( r.read_uint32(level_ace_end_time)  )
				return -1;
 		uint16_t __leve_ace_reward_vec_size;
		if(r.read_uint16(__leve_ace_reward_vec_size)  )
				return -1;
		leve_ace_reward.reserve(__leve_ace_reward_vec_size);
		for(uint16_t i = 0; i < __leve_ace_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				leve_ace_reward.push_back(v);
		}

		if( r.read_uint32(suit_collect_end_time)  )
				return -1;
 		uint16_t __suit_collect_id_vec_size;
		if(r.read_uint16(__suit_collect_id_vec_size)  )
				return -1;
		suit_collect_id.reserve(__suit_collect_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				suit_collect_id.push_back(v);
		}

		uint16_t __suit_collect_reward_vec_size;
		if(r.read_uint16(__suit_collect_reward_vec_size)  )
				return -1;
		suit_collect_reward.reserve(__suit_collect_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				suit_collect_reward.push_back(v);
		}

		if( r.read_uint32(guild_level_end_time)  )
				return -1;
 		uint16_t __guild_level_reward_vec_size;
		if(r.read_uint16(__guild_level_reward_vec_size)  )
				return -1;
		guild_level_reward.reserve(__guild_level_reward_vec_size);
		for(uint16_t i = 0; i < __guild_level_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				guild_level_reward.push_back(v);
		}

		if( r.read_uint32(level_rank_end_time)  ||  r.read_uint32(level_rank_count_time)  ||  r.read_uint32(level_rank)  )
				return -1;
 		uint16_t __level_rank_role_info_vec_size;
		if(r.read_uint16(__level_rank_role_info_vec_size)  )
				return -1;
		level_rank_role_info.reserve(__level_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __level_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				level_rank_role_info.push_back(v);
		}

		if( r.read_uint32(mount_rank_end_time)  ||  r.read_uint32(mount_rank_count_time)  ||  r.read_uint32(mount_rank)  )
				return -1;
 		uint16_t __mount_rank_role_info_vec_size;
		if(r.read_uint16(__mount_rank_role_info_vec_size)  )
				return -1;
		mount_rank_role_info.reserve(__mount_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __mount_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				mount_rank_role_info.push_back(v);
		}

		uint16_t __mount_rank_reward_vec_size;
		if(r.read_uint16(__mount_rank_reward_vec_size)  )
				return -1;
		mount_rank_reward.reserve(__mount_rank_reward_vec_size);
		for(uint16_t i = 0; i < __mount_rank_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_rank_reward.push_back(v);
		}

		if( r.read_uint32(wing_rank_end_time)  ||  r.read_uint32(wing_rank_count_time)  ||  r.read_uint32(wing_rank)  )
				return -1;
 		uint16_t __wing_rank_role_info_vec_size;
		if(r.read_uint16(__wing_rank_role_info_vec_size)  )
				return -1;
		wing_rank_role_info.reserve(__wing_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __wing_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				wing_rank_role_info.push_back(v);
		}

		uint16_t __wing_rank_reward_vec_size;
		if(r.read_uint16(__wing_rank_reward_vec_size)  )
				return -1;
		wing_rank_reward.reserve(__wing_rank_reward_vec_size);
		for(uint16_t i = 0; i < __wing_rank_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				wing_rank_reward.push_back(v);
		}

		if( r.read_uint32(arena_rank_end_time)  ||  r.read_uint32(arena_rank_count_time)  ||  r.read_uint32(arena_rank)  )
				return -1;
 		uint16_t __arena_rank_role_info_vec_size;
		if(r.read_uint16(__arena_rank_role_info_vec_size)  )
				return -1;
		arena_rank_role_info.reserve(__arena_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __arena_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				arena_rank_role_info.push_back(v);
		}

		if( r.read_uint32(force_rank_end_time)  ||  r.read_uint32(force_rank_count_time)  ||  r.read_uint32(force_rank)  )
				return -1;
 		uint16_t __force_rank_role_info_vec_size;
		if(r.read_uint16(__force_rank_role_info_vec_size)  )
				return -1;
		force_rank_role_info.reserve(__force_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __force_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				force_rank_role_info.push_back(v);
		}

		if( r.read_uint32(gem_levelup_end_time)  )
				return -1;
 		uint16_t __gem_levelup_reward_vec_size;
		if(r.read_uint16(__gem_levelup_reward_vec_size)  )
				return -1;
		gem_levelup_reward.reserve(__gem_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				gem_levelup_reward.push_back(v);
		}

		uint16_t __gem_levelup_progress_vec_size;
		if(r.read_uint16(__gem_levelup_progress_vec_size)  )
				return -1;
		gem_levelup_progress.reserve(__gem_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				gem_levelup_progress.push_back(v);
		}

		if( r.read_uint32(wing_levelup_end_time)  )
				return -1;
 		uint16_t __wing_levelup_reward_vec_size;
		if(r.read_uint16(__wing_levelup_reward_vec_size)  )
				return -1;
		wing_levelup_reward.reserve(__wing_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __wing_levelup_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				wing_levelup_reward.push_back(v);
		}

		if( r.read_uint32(mount_levelup_end_time)  )
				return -1;
 		uint16_t __mount_levelup_reward_vec_size;
		if(r.read_uint16(__mount_levelup_reward_vec_size)  )
				return -1;
		mount_levelup_reward.reserve(__mount_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_levelup_reward.push_back(v);
		}

		uint16_t __mount_levelup_progress_vec_size;
		if(r.read_uint16(__mount_levelup_progress_vec_size)  )
				return -1;
		mount_levelup_progress.reserve(__mount_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_levelup_progress.push_back(v);
		}

		if( r.read_uint32(sg_rank_end_time)  ||  r.read_uint32(sg_rank_count_time)  ||  r.read_uint32(sg_rank)  )
				return -1;
 		uint16_t __sg_rank_role_info_vec_size;
		if(r.read_uint16(__sg_rank_role_info_vec_size)  )
				return -1;
		sg_rank_role_info.reserve(__sg_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __sg_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				sg_rank_role_info.push_back(v);
		}

		uint16_t __sg_rank_reward_vec_size;
		if(r.read_uint16(__sg_rank_reward_vec_size)  )
				return -1;
		sg_rank_reward.reserve(__sg_rank_reward_vec_size);
		for(uint16_t i = 0; i < __sg_rank_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				sg_rank_reward.push_back(v);
		}

		if( r.read_uint32(arena_dan_end_time)  )
				return -1;
 		uint16_t __arena_dan_reward_vec_size;
		if(r.read_uint16(__arena_dan_reward_vec_size)  )
				return -1;
		arena_dan_reward.reserve(__arena_dan_reward_vec_size);
		for(uint16_t i = 0; i < __arena_dan_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				arena_dan_reward.push_back(v);
		}

		uint16_t __arena_dan_progress_vec_size;
		if(r.read_uint16(__arena_dan_progress_vec_size)  )
				return -1;
		arena_dan_progress.reserve(__arena_dan_progress_vec_size);
		for(uint16_t i = 0; i < __arena_dan_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				arena_dan_progress.push_back(v);
		}

		if( r.read_uint32(world_boss_end_time)  ||  r.read_uint32(world_boss_all_hurt)  ||  r.read_uint32(world_boss_today_hurt)  ||  r.read_uint32(world_boss_rank)  )
				return -1;
 		uint16_t __world_boss_reward_vec_size;
		if(r.read_uint16(__world_boss_reward_vec_size)  )
				return -1;
		world_boss_reward.reserve(__world_boss_reward_vec_size);
		for(uint16_t i = 0; i < __world_boss_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				world_boss_reward.push_back(v);
		}

		if( r.read_uint32(suit_collect_two_end_time)  )
				return -1;
 		uint16_t __suit_collect_two_id_vec_size;
		if(r.read_uint16(__suit_collect_two_id_vec_size)  )
				return -1;
		suit_collect_two_id.reserve(__suit_collect_two_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				suit_collect_two_id.push_back(v);
		}

		uint16_t __suit_collect_two_reward_vec_size;
		if(r.read_uint16(__suit_collect_two_reward_vec_size)  )
				return -1;
		suit_collect_two_reward.reserve(__suit_collect_two_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				suit_collect_two_reward.push_back(v);
		}

		if( r.read_uint32(heroes_door_end_time)  )
				return -1;
 		uint16_t __heroes_door_reward_vec_size;
		if(r.read_uint16(__heroes_door_reward_vec_size)  )
				return -1;
		heroes_door_reward.reserve(__heroes_door_reward_vec_size);
		for(uint16_t i = 0; i < __heroes_door_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				heroes_door_reward.push_back(v);
		}

		if( r.read_uint32(team_arena_end_time)  ||  r.read_uint32(team_arena_win_num)  )
				return -1;
 		uint16_t __team_arena_reward_vec_size;
		if(r.read_uint16(__team_arena_reward_vec_size)  )
				return -1;
		team_arena_reward.reserve(__team_arena_reward_vec_size);
		for(uint16_t i = 0; i < __team_arena_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				team_arena_reward.push_back(v);
		}

		if( r.read_uint32(warer_end_time)  ||  r.read_uint32(warer_win_num)  )
				return -1;
 		uint16_t __warer_reward_vec_size;
		if(r.read_uint16(__warer_reward_vec_size)  )
				return -1;
		warer_reward.reserve(__warer_reward_vec_size);
		for(uint16_t i = 0; i < __warer_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				warer_reward.push_back(v);
		}

		if( r.read_uint32(reward_task_end_time)  ||  r.read_uint32(reward_task_done_num)  )
				return -1;
 		uint16_t __reward_task_reward_vec_size;
		if(r.read_uint16(__reward_task_reward_vec_size)  )
				return -1;
		reward_task_reward.reserve(__reward_task_reward_vec_size);
		for(uint16_t i = 0; i < __reward_task_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				reward_task_reward.push_back(v);
		}

		if( r.read_uint32(sg_fb_end_time)  )
				return -1;
 		uint16_t __sg_fb_reward_vec_size;
		if(r.read_uint16(__sg_fb_reward_vec_size)  )
				return -1;
		sg_fb_reward.reserve(__sg_fb_reward_vec_size);
		for(uint16_t i = 0; i < __sg_fb_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				sg_fb_reward.push_back(v);
		}

		if( r.read_uint32(reward_task_fs_end_time)  ||  r.read_uint32(sg_welfare_end_time)  ||  r.read_uint32(fm_x_ci_end_time)  ||  r.read_uint32(fm_x_ci_num)  )
				return -1;
 		uint16_t __fm_x_ci_reward_vec_size;
		if(r.read_uint16(__fm_x_ci_reward_vec_size)  )
				return -1;
		fm_x_ci_reward.reserve(__fm_x_ci_reward_vec_size);
		for(uint16_t i = 0; i < __fm_x_ci_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				fm_x_ci_reward.push_back(v);
		}

		if( r.read_uint32(x_x_quality_heroer_end_time)  )
				return -1;
 		uint16_t __x_x_quality_heroer_reward_vec_size;
		if(r.read_uint16(__x_x_quality_heroer_reward_vec_size)  )
				return -1;
		x_x_quality_heroer_reward.reserve(__x_x_quality_heroer_reward_vec_size);
		for(uint16_t i = 0; i < __x_x_quality_heroer_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				x_x_quality_heroer_reward.push_back(v);
		}

		if( r.read_uint32(mount_lvup_x_end_time)  ||  r.read_uint32(mount_lvup_x_num)  )
				return -1;
 		uint16_t __mount_lvup_x_reward_vec_size;
		if(r.read_uint16(__mount_lvup_x_reward_vec_size)  )
				return -1;
		mount_lvup_x_reward.reserve(__mount_lvup_x_reward_vec_size);
		for(uint16_t i = 0; i < __mount_lvup_x_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_lvup_x_reward.push_back(v);
		}

		if( r.read_uint32(fusion_x_elite_card_end_time)  ||  r.read_uint32(fusion_x_elite_card_num)  )
				return -1;
 		uint16_t __fusion_x_elite_card_reward_vec_size;
		if(r.read_uint16(__fusion_x_elite_card_reward_vec_size)  )
				return -1;
		fusion_x_elite_card_reward.reserve(__fusion_x_elite_card_reward_vec_size);
		for(uint16_t i = 0; i < __fusion_x_elite_card_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				fusion_x_elite_card_reward.push_back(v);
		}

		if( r.read_uint32(alchemy_gift_end_time)  ||  r.read_uint32(alchemy_gift_num)  )
				return -1;
 		uint16_t __alchemy_gift_reward_vec_size;
		if(r.read_uint16(__alchemy_gift_reward_vec_size)  )
				return -1;
		alchemy_gift_reward.reserve(__alchemy_gift_reward_vec_size);
		for(uint16_t i = 0; i < __alchemy_gift_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				alchemy_gift_reward.push_back(v);
		}

		if( r.read_uint32(online_reward_gift_end_time)  ||  r.read_uint32(activity_gift_end_time)  ||  r.read_uint32(activity_gift_num)  )
				return -1;
 		uint16_t __activity_gift_reward_vec_size;
		if(r.read_uint16(__activity_gift_reward_vec_size)  )
				return -1;
		activity_gift_reward.reserve(__activity_gift_reward_vec_size);
		for(uint16_t i = 0; i < __activity_gift_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				activity_gift_reward.push_back(v);
		}

		if( r.read_uint32(ring_task_end_time)  ||  r.read_uint32(ring_task_num)  )
				return -1;
 		uint16_t __ring_task_reward_vec_size;
		if(r.read_uint16(__ring_task_reward_vec_size)  )
				return -1;
		ring_task_reward.reserve(__ring_task_reward_vec_size);
		for(uint16_t i = 0; i < __ring_task_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				ring_task_reward.push_back(v);
		}

		if( r.read_uint32(dragon_prey_end_time)  ||  r.read_uint32(dragon_prey_num)  )
				return -1;
 		uint16_t __dragon_prey_reward_vec_size;
		if(r.read_uint16(__dragon_prey_reward_vec_size)  )
				return -1;
		dragon_prey_reward.reserve(__dragon_prey_reward_vec_size);
		for(uint16_t i = 0; i < __dragon_prey_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				dragon_prey_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106400;
		limit_buy_end_time = 0;
		limit_buy_info.clear();
		vip_club_end_time = 0;
		vip_club_progress.clear();
		vip_club_reward.clear();
		level_ace_end_time = 0;
		leve_ace_reward.clear();
		suit_collect_end_time = 0;
		suit_collect_id.clear();
		suit_collect_reward.clear();
		guild_level_end_time = 0;
		guild_level_reward.clear();
		level_rank_end_time = 0;
		level_rank_count_time = 0;
		level_rank = 0;
		level_rank_role_info.clear();
		mount_rank_end_time = 0;
		mount_rank_count_time = 0;
		mount_rank = 0;
		mount_rank_role_info.clear();
		mount_rank_reward.clear();
		wing_rank_end_time = 0;
		wing_rank_count_time = 0;
		wing_rank = 0;
		wing_rank_role_info.clear();
		wing_rank_reward.clear();
		arena_rank_end_time = 0;
		arena_rank_count_time = 0;
		arena_rank = 0;
		arena_rank_role_info.clear();
		force_rank_end_time = 0;
		force_rank_count_time = 0;
		force_rank = 0;
		force_rank_role_info.clear();
		gem_levelup_end_time = 0;
		gem_levelup_reward.clear();
		gem_levelup_progress.clear();
		wing_levelup_end_time = 0;
		wing_levelup_reward.clear();
		mount_levelup_end_time = 0;
		mount_levelup_reward.clear();
		mount_levelup_progress.clear();
		sg_rank_end_time = 0;
		sg_rank_count_time = 0;
		sg_rank = 0;
		sg_rank_role_info.clear();
		sg_rank_reward.clear();
		arena_dan_end_time = 0;
		arena_dan_reward.clear();
		arena_dan_progress.clear();
		world_boss_end_time = 0;
		world_boss_all_hurt = 0;
		world_boss_today_hurt = 0;
		world_boss_rank = 0;
		world_boss_reward.clear();
		suit_collect_two_end_time = 0;
		suit_collect_two_id.clear();
		suit_collect_two_reward.clear();
		heroes_door_end_time = 0;
		heroes_door_reward.clear();
		team_arena_end_time = 0;
		team_arena_win_num = 0;
		team_arena_reward.clear();
		warer_end_time = 0;
		warer_win_num = 0;
		warer_reward.clear();
		reward_task_end_time = 0;
		reward_task_done_num = 0;
		reward_task_reward.clear();
		sg_fb_end_time = 0;
		sg_fb_reward.clear();
		reward_task_fs_end_time = 0;
		sg_welfare_end_time = 0;
		fm_x_ci_end_time = 0;
		fm_x_ci_num = 0;
		fm_x_ci_reward.clear();
		x_x_quality_heroer_end_time = 0;
		x_x_quality_heroer_reward.clear();
		mount_lvup_x_end_time = 0;
		mount_lvup_x_num = 0;
		mount_lvup_x_reward.clear();
		fusion_x_elite_card_end_time = 0;
		fusion_x_elite_card_num = 0;
		fusion_x_elite_card_reward.clear();
		alchemy_gift_end_time = 0;
		alchemy_gift_num = 0;
		alchemy_gift_reward.clear();
		online_reward_gift_end_time = 0;
		activity_gift_end_time = 0;
		activity_gift_num = 0;
		activity_gift_reward.clear();
		ring_task_end_time = 0;
		ring_task_num = 0;
		ring_task_reward.clear();
		dragon_prey_end_time = 0;
		dragon_prey_num = 0;
		dragon_prey_reward.clear();
	}
};

/*
请求领取VIP俱乐部奖励
*/
struct MSG_10106401  {
	uint32_t id;
	uint32_t reward_id;
	MSG_10106401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(reward_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(reward_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		reward_id = 0;
	}
};

/*
请求领取VIP俱乐部奖励(返回)
*/
struct MSG_50106401 : public Base_Msg  {
	uint32_t id;
	uint32_t reward_id;
	MSG_50106401(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(reward_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(reward_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106401;
		id = 0;
		reward_id = 0;
	}
};

/*
请求领取冲级高手奖励
*/
struct MSG_10106402  {
	uint32_t level;
	MSG_10106402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
	}
};

/*
请求领取冲级高手奖励(返回)
*/
struct MSG_50106402 : public Base_Msg  {
	uint32_t level;
	MSG_50106402(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106402;
		level = 0;
	}
};

/*
 请求领取套装收集奖励
*/
struct MSG_10106403  {
	uint32_t level;
	MSG_10106403(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
	}
};

/*
 请求领取套装收集奖励(返回)
*/
struct MSG_50106403 : public Base_Msg  {
	uint32_t level;
	MSG_50106403(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106403;
		level = 0;
	}
};

/*
请求领取公会升级奖励
*/
struct MSG_10106404  {
	uint32_t id;
	MSG_10106404(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取公会升级奖励(返回)
*/
struct MSG_50106404 : public Base_Msg  {
	uint32_t id;
	MSG_50106404(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106404;
		id = 0;
	}
};

/*
请求活动信息可领取TIP数
*/
struct MSG_10106405  {
	MSG_10106405(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息可领取TIP数(返回)
*/
struct MSG_50106405 : public Base_Msg  {
	uint32_t num;
	MSG_50106405(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(num);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(num)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106405;
		num = 0;
	}
};

/*
请求活动信息-限时购买
*/
struct MSG_10106420  {
	MSG_10106420(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-vip俱乐部
*/
struct MSG_10106421  {
	MSG_10106421(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-冲级高手
*/
struct MSG_10106422  {
	MSG_10106422(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-套装收集
*/
struct MSG_10106423  {
	MSG_10106423(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-公会升级 
*/
struct MSG_10106424  {
	MSG_10106424(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-等级排名
*/
struct MSG_10106425  {
	MSG_10106425(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-竞技场排名
*/
struct MSG_10106426  {
	MSG_10106426(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-战斗力排名
*/
struct MSG_10106427  {
	MSG_10106427(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-限时购买(返回)
*/
struct MSG_50106420 : public Base_Msg  {
	uint32_t limit_buy_end_time;
	std::vector<Goods_Info> limit_buy_info;
	MSG_50106420(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(limit_buy_end_time);
		uint16_t __limit_buy_info_vec_size = limit_buy_info.size();
		w.write_uint16(__limit_buy_info_vec_size);
		for(uint16_t i = 0; i < __limit_buy_info_vec_size; ++i) {
				limit_buy_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(limit_buy_end_time)  )
				return -1;
 		uint16_t __limit_buy_info_vec_size;
		if(r.read_uint16(__limit_buy_info_vec_size)  )
				return -1;
		limit_buy_info.reserve(__limit_buy_info_vec_size);
		for(uint16_t i = 0; i < __limit_buy_info_vec_size; ++i) {
				Goods_Info v;
				if(v.deserialize(r))
						return -1;
				limit_buy_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106420;
		limit_buy_end_time = 0;
		limit_buy_info.clear();
	}
};

/*
请求活动信息-vip俱乐部(返回)
*/
struct MSG_50106421 : public Base_Msg  {
	uint32_t vip_club_end_time;
	std::vector<Uint_Uint> vip_club_progress;
	std::vector<Uint_Uint> vip_club_reward;
	MSG_50106421(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(vip_club_end_time);
		uint16_t __vip_club_progress_vec_size = vip_club_progress.size();
		w.write_uint16(__vip_club_progress_vec_size);
		for(uint16_t i = 0; i < __vip_club_progress_vec_size; ++i) {
				vip_club_progress[i].serialize(w);
		}

		uint16_t __vip_club_reward_vec_size = vip_club_reward.size();
		w.write_uint16(__vip_club_reward_vec_size);
		for(uint16_t i = 0; i < __vip_club_reward_vec_size; ++i) {
				vip_club_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(vip_club_end_time)  )
				return -1;
 		uint16_t __vip_club_progress_vec_size;
		if(r.read_uint16(__vip_club_progress_vec_size)  )
				return -1;
		vip_club_progress.reserve(__vip_club_progress_vec_size);
		for(uint16_t i = 0; i < __vip_club_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				vip_club_progress.push_back(v);
		}

		uint16_t __vip_club_reward_vec_size;
		if(r.read_uint16(__vip_club_reward_vec_size)  )
				return -1;
		vip_club_reward.reserve(__vip_club_reward_vec_size);
		for(uint16_t i = 0; i < __vip_club_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				vip_club_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106421;
		vip_club_end_time = 0;
		vip_club_progress.clear();
		vip_club_reward.clear();
	}
};

/*
请求活动信息-冲级高手(返回)
*/
struct MSG_50106422 : public Base_Msg  {
	uint32_t level_ace_end_time;
	std::vector<Uint_Uint> leve_ace_reward;
	MSG_50106422(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level_ace_end_time);
		uint16_t __leve_ace_reward_vec_size = leve_ace_reward.size();
		w.write_uint16(__leve_ace_reward_vec_size);
		for(uint16_t i = 0; i < __leve_ace_reward_vec_size; ++i) {
				leve_ace_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level_ace_end_time)  )
				return -1;
 		uint16_t __leve_ace_reward_vec_size;
		if(r.read_uint16(__leve_ace_reward_vec_size)  )
				return -1;
		leve_ace_reward.reserve(__leve_ace_reward_vec_size);
		for(uint16_t i = 0; i < __leve_ace_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				leve_ace_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106422;
		level_ace_end_time = 0;
		leve_ace_reward.clear();
	}
};

/*
请求活动信息-套装收集(返回)
*/
struct MSG_50106423 : public Base_Msg  {
	uint32_t suit_collect_end_time;
	std::vector<uint32_t> suit_collect_id;
	std::vector<Uint_Uint> suit_collect_reward;
	MSG_50106423(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(suit_collect_end_time);
		uint16_t __suit_collect_id_vec_size = suit_collect_id.size();
		w.write_uint16(__suit_collect_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_id_vec_size; ++i) {
				w.write_uint32(suit_collect_id[i]);
		}

		uint16_t __suit_collect_reward_vec_size = suit_collect_reward.size();
		w.write_uint16(__suit_collect_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_reward_vec_size; ++i) {
				suit_collect_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(suit_collect_end_time)  )
				return -1;
 		uint16_t __suit_collect_id_vec_size;
		if(r.read_uint16(__suit_collect_id_vec_size)  )
				return -1;
		suit_collect_id.reserve(__suit_collect_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				suit_collect_id.push_back(v);
		}

		uint16_t __suit_collect_reward_vec_size;
		if(r.read_uint16(__suit_collect_reward_vec_size)  )
				return -1;
		suit_collect_reward.reserve(__suit_collect_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				suit_collect_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106423;
		suit_collect_end_time = 0;
		suit_collect_id.clear();
		suit_collect_reward.clear();
	}
};

/*
请求活动信息-公会升级(返回)
*/
struct MSG_50106424 : public Base_Msg  {
	uint32_t guild_level_end_time;
	std::vector<Uint_Uint> guild_level_reward;
	MSG_50106424(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(guild_level_end_time);
		uint16_t __guild_level_reward_vec_size = guild_level_reward.size();
		w.write_uint16(__guild_level_reward_vec_size);
		for(uint16_t i = 0; i < __guild_level_reward_vec_size; ++i) {
				guild_level_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(guild_level_end_time)  )
				return -1;
 		uint16_t __guild_level_reward_vec_size;
		if(r.read_uint16(__guild_level_reward_vec_size)  )
				return -1;
		guild_level_reward.reserve(__guild_level_reward_vec_size);
		for(uint16_t i = 0; i < __guild_level_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				guild_level_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106424;
		guild_level_end_time = 0;
		guild_level_reward.clear();
	}
};

/*
请求活动信息-等级排名(返回)
*/
struct MSG_50106425 : public Base_Msg  {
	uint32_t level_rank_end_time;
	uint32_t level_rank_count_time;
	uint32_t level_rank;
	std::vector<Role_Info> level_rank_role_info;
	MSG_50106425(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level_rank_end_time);
		w.write_uint32(level_rank_count_time);
		w.write_uint32(level_rank);
		uint16_t __level_rank_role_info_vec_size = level_rank_role_info.size();
		w.write_uint16(__level_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __level_rank_role_info_vec_size; ++i) {
				level_rank_role_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level_rank_end_time)  ||  r.read_uint32(level_rank_count_time)  ||  r.read_uint32(level_rank)  )
				return -1;
 		uint16_t __level_rank_role_info_vec_size;
		if(r.read_uint16(__level_rank_role_info_vec_size)  )
				return -1;
		level_rank_role_info.reserve(__level_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __level_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				level_rank_role_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106425;
		level_rank_end_time = 0;
		level_rank_count_time = 0;
		level_rank = 0;
		level_rank_role_info.clear();
	}
};

/*
请求活动信息-竞技场排名(返回)
*/
struct MSG_50106426 : public Base_Msg  {
	uint32_t arena_rank_end_time;
	uint32_t arena_rank_count_time;
	uint32_t arena_rank;
	std::vector<Role_Info> arena_rank_role_info;
	MSG_50106426(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(arena_rank_end_time);
		w.write_uint32(arena_rank_count_time);
		w.write_uint32(arena_rank);
		uint16_t __arena_rank_role_info_vec_size = arena_rank_role_info.size();
		w.write_uint16(__arena_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __arena_rank_role_info_vec_size; ++i) {
				arena_rank_role_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(arena_rank_end_time)  ||  r.read_uint32(arena_rank_count_time)  ||  r.read_uint32(arena_rank)  )
				return -1;
 		uint16_t __arena_rank_role_info_vec_size;
		if(r.read_uint16(__arena_rank_role_info_vec_size)  )
				return -1;
		arena_rank_role_info.reserve(__arena_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __arena_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				arena_rank_role_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106426;
		arena_rank_end_time = 0;
		arena_rank_count_time = 0;
		arena_rank = 0;
		arena_rank_role_info.clear();
	}
};

/*
请求活动信息-战斗力排名(返回)
*/
struct MSG_50106427 : public Base_Msg  {
	uint32_t force_rank_end_time;
	uint32_t force_rank_count_time;
	uint32_t force_rank;
	std::vector<Role_Info> force_rank_role_info;
	MSG_50106427(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(force_rank_end_time);
		w.write_uint32(force_rank_count_time);
		w.write_uint32(force_rank);
		uint16_t __force_rank_role_info_vec_size = force_rank_role_info.size();
		w.write_uint16(__force_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __force_rank_role_info_vec_size; ++i) {
				force_rank_role_info[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(force_rank_end_time)  ||  r.read_uint32(force_rank_count_time)  ||  r.read_uint32(force_rank)  )
				return -1;
 		uint16_t __force_rank_role_info_vec_size;
		if(r.read_uint16(__force_rank_role_info_vec_size)  )
				return -1;
		force_rank_role_info.reserve(__force_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __force_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				force_rank_role_info.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106427;
		force_rank_end_time = 0;
		force_rank_count_time = 0;
		force_rank = 0;
		force_rank_role_info.clear();
	}
};

/*
请求活动信息-宝石升级
*/
struct MSG_10106428  {
	MSG_10106428(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-宝石升级(返回)
*/
struct MSG_50106428 : public Base_Msg  {
	uint32_t gem_levelup_end_time;
	std::vector<Uint_Uint> gem_levelup_reward;
	std::vector<Uint_Uint> gem_levelup_progress;
	MSG_50106428(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(gem_levelup_end_time);
		uint16_t __gem_levelup_reward_vec_size = gem_levelup_reward.size();
		w.write_uint16(__gem_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_reward_vec_size; ++i) {
				gem_levelup_reward[i].serialize(w);
		}

		uint16_t __gem_levelup_progress_vec_size = gem_levelup_progress.size();
		w.write_uint16(__gem_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_progress_vec_size; ++i) {
				gem_levelup_progress[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(gem_levelup_end_time)  )
				return -1;
 		uint16_t __gem_levelup_reward_vec_size;
		if(r.read_uint16(__gem_levelup_reward_vec_size)  )
				return -1;
		gem_levelup_reward.reserve(__gem_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				gem_levelup_reward.push_back(v);
		}

		uint16_t __gem_levelup_progress_vec_size;
		if(r.read_uint16(__gem_levelup_progress_vec_size)  )
				return -1;
		gem_levelup_progress.reserve(__gem_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __gem_levelup_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				gem_levelup_progress.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106428;
		gem_levelup_end_time = 0;
		gem_levelup_reward.clear();
		gem_levelup_progress.clear();
	}
};

/*
请求领取宝石升级奖励
*/
struct MSG_10106406  {
	uint32_t id;
	MSG_10106406(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取宝石升级奖励(返回)
*/
struct MSG_50106406 : public Base_Msg  {
	uint32_t id;
	MSG_50106406(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106406;
		id = 0;
	}
};

/*
请求领取翅膀升级奖励
*/
struct MSG_10106407  {
	uint32_t level;
	MSG_10106407(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		level = 0;
	}
};

/*
请求领取翅膀升级奖励(返回)
*/
struct MSG_50106407 : public Base_Msg  {
	uint32_t level;
	MSG_50106407(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(level);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(level)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106407;
		level = 0;
	}
};

/*
请求活动信息-翅膀升级
*/
struct MSG_10106429  {
	MSG_10106429(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-翅膀升级(返回)
*/
struct MSG_50106429 : public Base_Msg  {
	uint32_t wing_levelup_end_time;
	std::vector<Uint_Uint> wing_levelup_reward;
	MSG_50106429(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(wing_levelup_end_time);
		uint16_t __wing_levelup_reward_vec_size = wing_levelup_reward.size();
		w.write_uint16(__wing_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __wing_levelup_reward_vec_size; ++i) {
				wing_levelup_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(wing_levelup_end_time)  )
				return -1;
 		uint16_t __wing_levelup_reward_vec_size;
		if(r.read_uint16(__wing_levelup_reward_vec_size)  )
				return -1;
		wing_levelup_reward.reserve(__wing_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __wing_levelup_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				wing_levelup_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106429;
		wing_levelup_end_time = 0;
		wing_levelup_reward.clear();
	}
};

/*
请求活动信息-坐骑升级
*/
struct MSG_10106430  {
	MSG_10106430(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-坐骑升级(返回)
*/
struct MSG_50106430 : public Base_Msg  {
	uint32_t mount_levelup_end_time;
	std::vector<Uint_Uint> mount_levelup_reward;
	std::vector<Uint_Uint> mount_levelup_progress;
	MSG_50106430(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(mount_levelup_end_time);
		uint16_t __mount_levelup_reward_vec_size = mount_levelup_reward.size();
		w.write_uint16(__mount_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_reward_vec_size; ++i) {
				mount_levelup_reward[i].serialize(w);
		}

		uint16_t __mount_levelup_progress_vec_size = mount_levelup_progress.size();
		w.write_uint16(__mount_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_progress_vec_size; ++i) {
				mount_levelup_progress[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(mount_levelup_end_time)  )
				return -1;
 		uint16_t __mount_levelup_reward_vec_size;
		if(r.read_uint16(__mount_levelup_reward_vec_size)  )
				return -1;
		mount_levelup_reward.reserve(__mount_levelup_reward_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_levelup_reward.push_back(v);
		}

		uint16_t __mount_levelup_progress_vec_size;
		if(r.read_uint16(__mount_levelup_progress_vec_size)  )
				return -1;
		mount_levelup_progress.reserve(__mount_levelup_progress_vec_size);
		for(uint16_t i = 0; i < __mount_levelup_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_levelup_progress.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106430;
		mount_levelup_end_time = 0;
		mount_levelup_reward.clear();
		mount_levelup_progress.clear();
	}
};

/*
请求领取坐骑升级奖励
*/
struct MSG_10106408  {
	uint32_t id;
	MSG_10106408(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取坐骑升级奖励(返回)
*/
struct MSG_50106408 : public Base_Msg  {
	uint32_t id;
	MSG_50106408(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106408;
		id = 0;
	}
};

/*
请求活动信息-上古副本
*/
struct MSG_10106431  {
	MSG_10106431(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-上古副本(返回)
*/
struct MSG_50106431 : public Base_Msg  {
	uint32_t sg_rank_end_time;
	uint32_t sg_rank_count_time;
	uint32_t sg_rank;
	std::vector<Role_Info> sg_rank_role_info;
	std::vector<Uint_Uint> sg_rank_reward;
	MSG_50106431(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(sg_rank_end_time);
		w.write_uint32(sg_rank_count_time);
		w.write_uint32(sg_rank);
		uint16_t __sg_rank_role_info_vec_size = sg_rank_role_info.size();
		w.write_uint16(__sg_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __sg_rank_role_info_vec_size; ++i) {
				sg_rank_role_info[i].serialize(w);
		}

		uint16_t __sg_rank_reward_vec_size = sg_rank_reward.size();
		w.write_uint16(__sg_rank_reward_vec_size);
		for(uint16_t i = 0; i < __sg_rank_reward_vec_size; ++i) {
				sg_rank_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(sg_rank_end_time)  ||  r.read_uint32(sg_rank_count_time)  ||  r.read_uint32(sg_rank)  )
				return -1;
 		uint16_t __sg_rank_role_info_vec_size;
		if(r.read_uint16(__sg_rank_role_info_vec_size)  )
				return -1;
		sg_rank_role_info.reserve(__sg_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __sg_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				sg_rank_role_info.push_back(v);
		}

		uint16_t __sg_rank_reward_vec_size;
		if(r.read_uint16(__sg_rank_reward_vec_size)  )
				return -1;
		sg_rank_reward.reserve(__sg_rank_reward_vec_size);
		for(uint16_t i = 0; i < __sg_rank_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				sg_rank_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106431;
		sg_rank_end_time = 0;
		sg_rank_count_time = 0;
		sg_rank = 0;
		sg_rank_role_info.clear();
		sg_rank_reward.clear();
	}
};

/*
请求领取上古排行通关奖励
*/
struct MSG_10106409  {
	uint32_t id;
	MSG_10106409(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取上古排行通关奖励(返回)
*/
struct MSG_50106409 : public Base_Msg  {
	uint32_t id;
	MSG_50106409(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106409;
		id = 0;
	}
};

/*
请求活动信息-竞技场段位
*/
struct MSG_10106432  {
	MSG_10106432(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-竞技场段位(返回)
*/
struct MSG_50106432 : public Base_Msg  {
	uint32_t arena_dan_end_time;
	std::vector<Uint_Uint> arena_dan_reward;
	std::vector<Uint_Uint> arena_dan_progress;
	MSG_50106432(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(arena_dan_end_time);
		uint16_t __arena_dan_reward_vec_size = arena_dan_reward.size();
		w.write_uint16(__arena_dan_reward_vec_size);
		for(uint16_t i = 0; i < __arena_dan_reward_vec_size; ++i) {
				arena_dan_reward[i].serialize(w);
		}

		uint16_t __arena_dan_progress_vec_size = arena_dan_progress.size();
		w.write_uint16(__arena_dan_progress_vec_size);
		for(uint16_t i = 0; i < __arena_dan_progress_vec_size; ++i) {
				arena_dan_progress[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(arena_dan_end_time)  )
				return -1;
 		uint16_t __arena_dan_reward_vec_size;
		if(r.read_uint16(__arena_dan_reward_vec_size)  )
				return -1;
		arena_dan_reward.reserve(__arena_dan_reward_vec_size);
		for(uint16_t i = 0; i < __arena_dan_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				arena_dan_reward.push_back(v);
		}

		uint16_t __arena_dan_progress_vec_size;
		if(r.read_uint16(__arena_dan_progress_vec_size)  )
				return -1;
		arena_dan_progress.reserve(__arena_dan_progress_vec_size);
		for(uint16_t i = 0; i < __arena_dan_progress_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				arena_dan_progress.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106432;
		arena_dan_end_time = 0;
		arena_dan_reward.clear();
		arena_dan_progress.clear();
	}
};

/*
请求领取竞技场段位奖励
*/
struct MSG_10106410  {
	uint32_t id;
	MSG_10106410(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取竞技场段位奖励(返回)
*/
struct MSG_50106410 : public Base_Msg  {
	uint32_t id;
	MSG_50106410(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106410;
		id = 0;
	}
};

/*
请求活动信息-世界boss
*/
struct MSG_10106433  {
	MSG_10106433(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-套装收集二段
*/
struct MSG_10106434  {
	MSG_10106434(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-英雄之门
*/
struct MSG_10106435  {
	MSG_10106435(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-组队竞技
*/
struct MSG_10106436  {
	MSG_10106436(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-战场
*/
struct MSG_10106437  {
	MSG_10106437(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-悬赏
*/
struct MSG_10106438  {
	MSG_10106438(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-上古副本
*/
struct MSG_10106439  {
	MSG_10106439(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-世界boss(返回)
*/
struct MSG_50106433 : public Base_Msg  {
	uint32_t world_boss_end_time;
	uint32_t world_boss_all_hurt;
	uint32_t world_boss_today_hurt;
	uint32_t world_boss_rank;
	std::vector<Uint_Uint> world_boss_reward;
	MSG_50106433(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(world_boss_end_time);
		w.write_uint32(world_boss_all_hurt);
		w.write_uint32(world_boss_today_hurt);
		w.write_uint32(world_boss_rank);
		uint16_t __world_boss_reward_vec_size = world_boss_reward.size();
		w.write_uint16(__world_boss_reward_vec_size);
		for(uint16_t i = 0; i < __world_boss_reward_vec_size; ++i) {
				world_boss_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(world_boss_end_time)  ||  r.read_uint32(world_boss_all_hurt)  ||  r.read_uint32(world_boss_today_hurt)  ||  r.read_uint32(world_boss_rank)  )
				return -1;
 		uint16_t __world_boss_reward_vec_size;
		if(r.read_uint16(__world_boss_reward_vec_size)  )
				return -1;
		world_boss_reward.reserve(__world_boss_reward_vec_size);
		for(uint16_t i = 0; i < __world_boss_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				world_boss_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106433;
		world_boss_end_time = 0;
		world_boss_all_hurt = 0;
		world_boss_today_hurt = 0;
		world_boss_rank = 0;
		world_boss_reward.clear();
	}
};

/*
请求活动信息-套装收集二段(返回)
*/
struct MSG_50106434 : public Base_Msg  {
	uint32_t suit_collect_two_end_time;
	std::vector<uint32_t> suit_collect_two_id;
	std::vector<Uint_Uint> suit_collect_two_reward;
	MSG_50106434(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(suit_collect_two_end_time);
		uint16_t __suit_collect_two_id_vec_size = suit_collect_two_id.size();
		w.write_uint16(__suit_collect_two_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_id_vec_size; ++i) {
				w.write_uint32(suit_collect_two_id[i]);
		}

		uint16_t __suit_collect_two_reward_vec_size = suit_collect_two_reward.size();
		w.write_uint16(__suit_collect_two_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_reward_vec_size; ++i) {
				suit_collect_two_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(suit_collect_two_end_time)  )
				return -1;
 		uint16_t __suit_collect_two_id_vec_size;
		if(r.read_uint16(__suit_collect_two_id_vec_size)  )
				return -1;
		suit_collect_two_id.reserve(__suit_collect_two_id_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_id_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				suit_collect_two_id.push_back(v);
		}

		uint16_t __suit_collect_two_reward_vec_size;
		if(r.read_uint16(__suit_collect_two_reward_vec_size)  )
				return -1;
		suit_collect_two_reward.reserve(__suit_collect_two_reward_vec_size);
		for(uint16_t i = 0; i < __suit_collect_two_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				suit_collect_two_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106434;
		suit_collect_two_end_time = 0;
		suit_collect_two_id.clear();
		suit_collect_two_reward.clear();
	}
};

/*
请求活动信息-英雄之门(返回)
*/
struct MSG_50106435 : public Base_Msg  {
	uint32_t heroes_door_end_time;
	std::vector<Uint_Uint> heroes_door_reward;
	MSG_50106435(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(heroes_door_end_time);
		uint16_t __heroes_door_reward_vec_size = heroes_door_reward.size();
		w.write_uint16(__heroes_door_reward_vec_size);
		for(uint16_t i = 0; i < __heroes_door_reward_vec_size; ++i) {
				heroes_door_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(heroes_door_end_time)  )
				return -1;
 		uint16_t __heroes_door_reward_vec_size;
		if(r.read_uint16(__heroes_door_reward_vec_size)  )
				return -1;
		heroes_door_reward.reserve(__heroes_door_reward_vec_size);
		for(uint16_t i = 0; i < __heroes_door_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				heroes_door_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106435;
		heroes_door_end_time = 0;
		heroes_door_reward.clear();
	}
};

/*
请求活动信息-组队竞技(返回)
*/
struct MSG_50106436 : public Base_Msg  {
	uint32_t team_arena_end_time;
	uint32_t team_arena_win_num;
	std::vector<Uint_Uint> team_arena_reward;
	MSG_50106436(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(team_arena_end_time);
		w.write_uint32(team_arena_win_num);
		uint16_t __team_arena_reward_vec_size = team_arena_reward.size();
		w.write_uint16(__team_arena_reward_vec_size);
		for(uint16_t i = 0; i < __team_arena_reward_vec_size; ++i) {
				team_arena_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(team_arena_end_time)  ||  r.read_uint32(team_arena_win_num)  )
				return -1;
 		uint16_t __team_arena_reward_vec_size;
		if(r.read_uint16(__team_arena_reward_vec_size)  )
				return -1;
		team_arena_reward.reserve(__team_arena_reward_vec_size);
		for(uint16_t i = 0; i < __team_arena_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				team_arena_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106436;
		team_arena_end_time = 0;
		team_arena_win_num = 0;
		team_arena_reward.clear();
	}
};

/*
请求活动信息-战场(返回)
*/
struct MSG_50106437 : public Base_Msg  {
	uint32_t warer_end_time;
	uint32_t warer_win_num;
	std::vector<Uint_Uint> warer_reward;
	MSG_50106437(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(warer_end_time);
		w.write_uint32(warer_win_num);
		uint16_t __warer_reward_vec_size = warer_reward.size();
		w.write_uint16(__warer_reward_vec_size);
		for(uint16_t i = 0; i < __warer_reward_vec_size; ++i) {
				warer_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(warer_end_time)  ||  r.read_uint32(warer_win_num)  )
				return -1;
 		uint16_t __warer_reward_vec_size;
		if(r.read_uint16(__warer_reward_vec_size)  )
				return -1;
		warer_reward.reserve(__warer_reward_vec_size);
		for(uint16_t i = 0; i < __warer_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				warer_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106437;
		warer_end_time = 0;
		warer_win_num = 0;
		warer_reward.clear();
	}
};

/*
请求活动信息-悬赏(返回)
*/
struct MSG_50106438 : public Base_Msg  {
	uint32_t reward_task_end_time;
	uint32_t reward_task_done_num;
	std::vector<Uint_Uint> reward_task_reward;
	MSG_50106438(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(reward_task_end_time);
		w.write_uint32(reward_task_done_num);
		uint16_t __reward_task_reward_vec_size = reward_task_reward.size();
		w.write_uint16(__reward_task_reward_vec_size);
		for(uint16_t i = 0; i < __reward_task_reward_vec_size; ++i) {
				reward_task_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(reward_task_end_time)  ||  r.read_uint32(reward_task_done_num)  )
				return -1;
 		uint16_t __reward_task_reward_vec_size;
		if(r.read_uint16(__reward_task_reward_vec_size)  )
				return -1;
		reward_task_reward.reserve(__reward_task_reward_vec_size);
		for(uint16_t i = 0; i < __reward_task_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				reward_task_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106438;
		reward_task_end_time = 0;
		reward_task_done_num = 0;
		reward_task_reward.clear();
	}
};

/*
请求活动信息-上古副本(返回)
*/
struct MSG_50106439 : public Base_Msg  {
	uint32_t sg_fb_end_time;
	std::vector<Uint_Uint> sg_fb_reward;
	MSG_50106439(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(sg_fb_end_time);
		uint16_t __sg_fb_reward_vec_size = sg_fb_reward.size();
		w.write_uint16(__sg_fb_reward_vec_size);
		for(uint16_t i = 0; i < __sg_fb_reward_vec_size; ++i) {
				sg_fb_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(sg_fb_end_time)  )
				return -1;
 		uint16_t __sg_fb_reward_vec_size;
		if(r.read_uint16(__sg_fb_reward_vec_size)  )
				return -1;
		sg_fb_reward.reserve(__sg_fb_reward_vec_size);
		for(uint16_t i = 0; i < __sg_fb_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				sg_fb_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106439;
		sg_fb_end_time = 0;
		sg_fb_reward.clear();
	}
};

/*
请求领取活动奖励(公用1)
*/
struct MSG_10106411  {
	uint32_t id;
	uint32_t act_type;
	MSG_10106411(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(act_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(act_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
		act_type = 0;
	}
};

/*
请求领取活动奖励(公用1)(返回)
*/
struct MSG_50106411 : public Base_Msg  {
	uint32_t id;
	uint32_t act_type;
	MSG_50106411(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
		w.write_uint32(act_type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  ||  r.read_uint32(act_type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106411;
		id = 0;
		act_type = 0;
	}
};

/*
请求活动信息-累计附魔X次
*/
struct MSG_10106440  {
	MSG_10106440(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-累计附魔X次(返回)
*/
struct MSG_50106440 : public Base_Msg  {
	uint32_t fm_x_ci_end_time;
	uint32_t fm_x_ci_num;
	std::vector<Uint_Uint> fm_x_ci_reward;
	MSG_50106440(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(fm_x_ci_end_time);
		w.write_uint32(fm_x_ci_num);
		uint16_t __fm_x_ci_reward_vec_size = fm_x_ci_reward.size();
		w.write_uint16(__fm_x_ci_reward_vec_size);
		for(uint16_t i = 0; i < __fm_x_ci_reward_vec_size; ++i) {
				fm_x_ci_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(fm_x_ci_end_time)  ||  r.read_uint32(fm_x_ci_num)  )
				return -1;
 		uint16_t __fm_x_ci_reward_vec_size;
		if(r.read_uint16(__fm_x_ci_reward_vec_size)  )
				return -1;
		fm_x_ci_reward.reserve(__fm_x_ci_reward_vec_size);
		for(uint16_t i = 0; i < __fm_x_ci_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				fm_x_ci_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106440;
		fm_x_ci_end_time = 0;
		fm_x_ci_num = 0;
		fm_x_ci_reward.clear();
	}
};

/*
请求活动信息-累计X个X品质英雄
*/
struct MSG_10106441  {
	MSG_10106441(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-累计X个X品质英雄(返回)
*/
struct MSG_50106441 : public Base_Msg  {
	uint32_t x_x_quality_heroer_end_time;
	std::vector<Uint_Uint> x_x_quality_heroer_reward;
	MSG_50106441(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(x_x_quality_heroer_end_time);
		uint16_t __x_x_quality_heroer_reward_vec_size = x_x_quality_heroer_reward.size();
		w.write_uint16(__x_x_quality_heroer_reward_vec_size);
		for(uint16_t i = 0; i < __x_x_quality_heroer_reward_vec_size; ++i) {
				x_x_quality_heroer_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(x_x_quality_heroer_end_time)  )
				return -1;
 		uint16_t __x_x_quality_heroer_reward_vec_size;
		if(r.read_uint16(__x_x_quality_heroer_reward_vec_size)  )
				return -1;
		x_x_quality_heroer_reward.reserve(__x_x_quality_heroer_reward_vec_size);
		for(uint16_t i = 0; i < __x_x_quality_heroer_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				x_x_quality_heroer_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106441;
		x_x_quality_heroer_end_time = 0;
		x_x_quality_heroer_reward.clear();
	}
};

/*
请求活动信息-坐骑进阶X次
*/
struct MSG_10106442  {
	MSG_10106442(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-坐骑进阶X次(返回)
*/
struct MSG_50106442 : public Base_Msg  {
	uint32_t mount_lvup_x_end_time;
	uint32_t mount_lvup_x_num;
	std::vector<Uint_Uint> mount_lvup_x_reward;
	MSG_50106442(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(mount_lvup_x_end_time);
		w.write_uint32(mount_lvup_x_num);
		uint16_t __mount_lvup_x_reward_vec_size = mount_lvup_x_reward.size();
		w.write_uint16(__mount_lvup_x_reward_vec_size);
		for(uint16_t i = 0; i < __mount_lvup_x_reward_vec_size; ++i) {
				mount_lvup_x_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(mount_lvup_x_end_time)  ||  r.read_uint32(mount_lvup_x_num)  )
				return -1;
 		uint16_t __mount_lvup_x_reward_vec_size;
		if(r.read_uint16(__mount_lvup_x_reward_vec_size)  )
				return -1;
		mount_lvup_x_reward.reserve(__mount_lvup_x_reward_vec_size);
		for(uint16_t i = 0; i < __mount_lvup_x_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_lvup_x_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106442;
		mount_lvup_x_end_time = 0;
		mount_lvup_x_num = 0;
		mount_lvup_x_reward.clear();
	}
};

/*
请求活动信息-合成X张精英卡牌
*/
struct MSG_10106443  {
	MSG_10106443(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-合成X张精英卡牌(返回)
*/
struct MSG_50106443 : public Base_Msg  {
	uint32_t fusion_x_elite_card_end_time;
	uint32_t fusion_x_elite_card_num;
	std::vector<Uint_Uint> fusion_x_elite_card_reward;
	MSG_50106443(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(fusion_x_elite_card_end_time);
		w.write_uint32(fusion_x_elite_card_num);
		uint16_t __fusion_x_elite_card_reward_vec_size = fusion_x_elite_card_reward.size();
		w.write_uint16(__fusion_x_elite_card_reward_vec_size);
		for(uint16_t i = 0; i < __fusion_x_elite_card_reward_vec_size; ++i) {
				fusion_x_elite_card_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(fusion_x_elite_card_end_time)  ||  r.read_uint32(fusion_x_elite_card_num)  )
				return -1;
 		uint16_t __fusion_x_elite_card_reward_vec_size;
		if(r.read_uint16(__fusion_x_elite_card_reward_vec_size)  )
				return -1;
		fusion_x_elite_card_reward.reserve(__fusion_x_elite_card_reward_vec_size);
		for(uint16_t i = 0; i < __fusion_x_elite_card_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				fusion_x_elite_card_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106443;
		fusion_x_elite_card_end_time = 0;
		fusion_x_elite_card_num = 0;
		fusion_x_elite_card_reward.clear();
	}
};

/*
请求活动信息-炼金有礼
*/
struct MSG_10106444  {
	MSG_10106444(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-炼金有礼(返回)
*/
struct MSG_50106444 : public Base_Msg  {
	uint32_t alchemy_gift_end_time;
	uint32_t alchemy_gift_num;
	std::vector<Uint_Uint> alchemy_gift_reward;
	MSG_50106444(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(alchemy_gift_end_time);
		w.write_uint32(alchemy_gift_num);
		uint16_t __alchemy_gift_reward_vec_size = alchemy_gift_reward.size();
		w.write_uint16(__alchemy_gift_reward_vec_size);
		for(uint16_t i = 0; i < __alchemy_gift_reward_vec_size; ++i) {
				alchemy_gift_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(alchemy_gift_end_time)  ||  r.read_uint32(alchemy_gift_num)  )
				return -1;
 		uint16_t __alchemy_gift_reward_vec_size;
		if(r.read_uint16(__alchemy_gift_reward_vec_size)  )
				return -1;
		alchemy_gift_reward.reserve(__alchemy_gift_reward_vec_size);
		for(uint16_t i = 0; i < __alchemy_gift_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				alchemy_gift_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106444;
		alchemy_gift_end_time = 0;
		alchemy_gift_num = 0;
		alchemy_gift_reward.clear();
	}
};

/*
请求活动信息-活跃度好礼
*/
struct MSG_10106445  {
	MSG_10106445(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-环任务活动
*/
struct MSG_10106446  {
	MSG_10106446(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-龙谷掠夺活动
*/
struct MSG_10106447  {
	MSG_10106447(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-龙谷掠夺活动(返回)
*/
struct MSG_50106447 : public Base_Msg  {
	uint32_t dragon_prey_end_time;
	uint32_t dragon_prey_num;
	std::vector<Uint_Uint> dragon_prey_reward;
	MSG_50106447(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(dragon_prey_end_time);
		w.write_uint32(dragon_prey_num);
		uint16_t __dragon_prey_reward_vec_size = dragon_prey_reward.size();
		w.write_uint16(__dragon_prey_reward_vec_size);
		for(uint16_t i = 0; i < __dragon_prey_reward_vec_size; ++i) {
				dragon_prey_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(dragon_prey_end_time)  ||  r.read_uint32(dragon_prey_num)  )
				return -1;
 		uint16_t __dragon_prey_reward_vec_size;
		if(r.read_uint16(__dragon_prey_reward_vec_size)  )
				return -1;
		dragon_prey_reward.reserve(__dragon_prey_reward_vec_size);
		for(uint16_t i = 0; i < __dragon_prey_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				dragon_prey_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106447;
		dragon_prey_end_time = 0;
		dragon_prey_num = 0;
		dragon_prey_reward.clear();
	}
};

/*
请求活动信息-环任务活动(返回)
*/
struct MSG_50106446 : public Base_Msg  {
	uint32_t ring_task_end_time;
	uint32_t ring_task_num;
	std::vector<Uint_Uint> ring_task_reward;
	MSG_50106446(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(ring_task_end_time);
		w.write_uint32(ring_task_num);
		uint16_t __ring_task_reward_vec_size = ring_task_reward.size();
		w.write_uint16(__ring_task_reward_vec_size);
		for(uint16_t i = 0; i < __ring_task_reward_vec_size; ++i) {
				ring_task_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(ring_task_end_time)  ||  r.read_uint32(ring_task_num)  )
				return -1;
 		uint16_t __ring_task_reward_vec_size;
		if(r.read_uint16(__ring_task_reward_vec_size)  )
				return -1;
		ring_task_reward.reserve(__ring_task_reward_vec_size);
		for(uint16_t i = 0; i < __ring_task_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				ring_task_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106446;
		ring_task_end_time = 0;
		ring_task_num = 0;
		ring_task_reward.clear();
	}
};

/*
请求活动信息-活跃度好礼(返回)
*/
struct MSG_50106445 : public Base_Msg  {
	uint32_t activity_gift_end_time;
	uint32_t activity_gift_num;
	std::vector<Uint_Uint> activity_gift_reward;
	MSG_50106445(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(activity_gift_end_time);
		w.write_uint32(activity_gift_num);
		uint16_t __activity_gift_reward_vec_size = activity_gift_reward.size();
		w.write_uint16(__activity_gift_reward_vec_size);
		for(uint16_t i = 0; i < __activity_gift_reward_vec_size; ++i) {
				activity_gift_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(activity_gift_end_time)  ||  r.read_uint32(activity_gift_num)  )
				return -1;
 		uint16_t __activity_gift_reward_vec_size;
		if(r.read_uint16(__activity_gift_reward_vec_size)  )
				return -1;
		activity_gift_reward.reserve(__activity_gift_reward_vec_size);
		for(uint16_t i = 0; i < __activity_gift_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				activity_gift_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106445;
		activity_gift_end_time = 0;
		activity_gift_num = 0;
		activity_gift_reward.clear();
	}
};

/*
请求活动信息-坐骑排名
*/
struct MSG_10106448  {
	MSG_10106448(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-翅膀排名
*/
struct MSG_10106449  {
	MSG_10106449(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求活动信息-翅膀排名(返回)
*/
struct MSG_50106449 : public Base_Msg  {
	uint32_t wing_rank_end_time;
	uint32_t wing_rank_count_time;
	uint32_t wing_rank;
	std::vector<Role_Info> wing_rank_role_info;
	std::vector<Uint_Uint> wing_rank_reward;
	MSG_50106449(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(wing_rank_end_time);
		w.write_uint32(wing_rank_count_time);
		w.write_uint32(wing_rank);
		uint16_t __wing_rank_role_info_vec_size = wing_rank_role_info.size();
		w.write_uint16(__wing_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __wing_rank_role_info_vec_size; ++i) {
				wing_rank_role_info[i].serialize(w);
		}

		uint16_t __wing_rank_reward_vec_size = wing_rank_reward.size();
		w.write_uint16(__wing_rank_reward_vec_size);
		for(uint16_t i = 0; i < __wing_rank_reward_vec_size; ++i) {
				wing_rank_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(wing_rank_end_time)  ||  r.read_uint32(wing_rank_count_time)  ||  r.read_uint32(wing_rank)  )
				return -1;
 		uint16_t __wing_rank_role_info_vec_size;
		if(r.read_uint16(__wing_rank_role_info_vec_size)  )
				return -1;
		wing_rank_role_info.reserve(__wing_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __wing_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				wing_rank_role_info.push_back(v);
		}

		uint16_t __wing_rank_reward_vec_size;
		if(r.read_uint16(__wing_rank_reward_vec_size)  )
				return -1;
		wing_rank_reward.reserve(__wing_rank_reward_vec_size);
		for(uint16_t i = 0; i < __wing_rank_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				wing_rank_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106449;
		wing_rank_end_time = 0;
		wing_rank_count_time = 0;
		wing_rank = 0;
		wing_rank_role_info.clear();
		wing_rank_reward.clear();
	}
};

/*
请求活动信息-坐骑排名(返回)
*/
struct MSG_50106448 : public Base_Msg  {
	uint32_t mount_rank_end_time;
	uint32_t mount_rank_count_time;
	uint32_t mount_rank;
	std::vector<Role_Info> mount_rank_role_info;
	std::vector<Uint_Uint> mount_rank_reward;
	MSG_50106448(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(mount_rank_end_time);
		w.write_uint32(mount_rank_count_time);
		w.write_uint32(mount_rank);
		uint16_t __mount_rank_role_info_vec_size = mount_rank_role_info.size();
		w.write_uint16(__mount_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __mount_rank_role_info_vec_size; ++i) {
				mount_rank_role_info[i].serialize(w);
		}

		uint16_t __mount_rank_reward_vec_size = mount_rank_reward.size();
		w.write_uint16(__mount_rank_reward_vec_size);
		for(uint16_t i = 0; i < __mount_rank_reward_vec_size; ++i) {
				mount_rank_reward[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(mount_rank_end_time)  ||  r.read_uint32(mount_rank_count_time)  ||  r.read_uint32(mount_rank)  )
				return -1;
 		uint16_t __mount_rank_role_info_vec_size;
		if(r.read_uint16(__mount_rank_role_info_vec_size)  )
				return -1;
		mount_rank_role_info.reserve(__mount_rank_role_info_vec_size);
		for(uint16_t i = 0; i < __mount_rank_role_info_vec_size; ++i) {
				Role_Info v;
				if(v.deserialize(r))
						return -1;
				mount_rank_role_info.push_back(v);
		}

		uint16_t __mount_rank_reward_vec_size;
		if(r.read_uint16(__mount_rank_reward_vec_size)  )
				return -1;
		mount_rank_reward.reserve(__mount_rank_reward_vec_size);
		for(uint16_t i = 0; i < __mount_rank_reward_vec_size; ++i) {
				Uint_Uint v;
				if(v.deserialize(r))
						return -1;
				mount_rank_reward.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50106448;
		mount_rank_end_time = 0;
		mount_rank_count_time = 0;
		mount_rank = 0;
		mount_rank_role_info.clear();
		mount_rank_reward.clear();
	}
};

/*
请求领取坐骑排行通关奖励
*/
struct MSG_10106412  {
	uint32_t id;
	MSG_10106412(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取翅膀排行通关奖励
*/
struct MSG_10106413  {
	uint32_t id;
	MSG_10106413(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		id = 0;
	}
};

/*
请求领取翅膀排行通关奖励(返回)
*/
struct MSG_50106413 : public Base_Msg  {
	uint32_t id;
	MSG_50106413(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106413;
		id = 0;
	}
};

/*
请求领取坐骑排行通关奖励(返回)
*/
struct MSG_50106412 : public Base_Msg  {
	uint32_t id;
	MSG_50106412(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50106412;
		id = 0;
	}
};



#endif