/*
* Generate by devtool
*/

#ifndef _MSG_TASK_H_
#define _MSG_TASK_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
获取任务列表
备注：
获得任务界面的任务列表            //可接,已接,日常. 
初次登录获取,或发生错时再次获取. 
*/
struct MSG_10110000  {
	MSG_10110000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
获取任务列表(返回)
备注：
获得任务界面的任务列表            //可接,已接,日常. 
初次登录获取,或发生错时再次获取. 
*/
struct MSG_50110000 : public Base_Msg  {
	std::vector<Task_Info> tasks;
	MSG_50110000(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __tasks_vec_size = tasks.size();
		w.write_uint16(__tasks_vec_size);
		for(uint16_t i = 0; i < __tasks_vec_size; ++i) {
				tasks[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __tasks_vec_size;
		if(r.read_uint16(__tasks_vec_size)  )
				return -1;
		tasks.reserve(__tasks_vec_size);
		for(uint16_t i = 0; i < __tasks_vec_size; ++i) {
				Task_Info v;
				if(v.deserialize(r))
						return -1;
				tasks.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50110000;
		tasks.clear();
	}
};

/*
获取NPC任务(历史原因，遗留，未用到，待删除)
返回 520201 
假如NPC有任务,服务器将返回任务列表
*/
struct MSG_10110001  {
	uint32_t npc_id;
	uint8_t source;
	MSG_10110001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(npc_id);
		w.write_uint8(source);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(npc_id)  ||  r.read_uint8(source)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		npc_id = 0;
		source = 0;
	}
};

/*
获取NPC任务(返回)
返回 520201 
假如NPC有任务,服务器将返回任务列表
*/
struct MSG_50110001 : public Base_Msg  {
	std::vector<Task_Status> tasks;
	uint8_t source;
	uint32_t npc_id;
	int8_t open_window;
	MSG_50110001(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __tasks_vec_size = tasks.size();
		w.write_uint16(__tasks_vec_size);
		for(uint16_t i = 0; i < __tasks_vec_size; ++i) {
				tasks[i].serialize(w);
		}

		w.write_uint8(source);
		w.write_uint32(npc_id);
		w.write_int8(open_window);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __tasks_vec_size;
		if(r.read_uint16(__tasks_vec_size)  )
				return -1;
		tasks.reserve(__tasks_vec_size);
		for(uint16_t i = 0; i < __tasks_vec_size; ++i) {
				Task_Status v;
				if(v.deserialize(r))
						return -1;
				tasks.push_back(v);
		}

		if( r.read_uint8(source)  ||  r.read_uint32(npc_id)  ||  r.read_int8(open_window)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110001;
		tasks.clear();
		source = 0;
		npc_id = 0;
		open_window = 0;
	}
};

/*
接受任务
成功返回 520203,否则返回错误号. 

如任务可接，800200  800201会主动通知更新任务信息,客户端根据任务状态变化改变任务可接/以接列表．同时800200  800201还会通知把任务加入到"可接列表". (一次接收至少有4条主动消息)
*/
struct MSG_10110002  {
	uint32_t task_id;
	MSG_10110002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id = 0;
	}
};

/*
接受任务(返回)
*/
struct MSG_50110002 : public Base_Msg  {
	uint32_t task_id;
	uint16_t type;
	uint8_t status;
	MSG_50110002(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint16(type);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint16(type)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110002;
		task_id = 0;
		type = 0;
		status = 0;
	}
};

/*
放弃任务
成功返回 520204,否则返回错误号.  

如任务可放弃,800200  800201会主动通知移除"以接列表"的任务信息.　同时800200  800201还会通知把任务加入到"可接列表". (一次放弃至少有4条主动消息)
*/
struct MSG_10110003  {
	uint32_t task_id;
	MSG_10110003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id = 0;
	}
};

/*
放弃任务(返回)
*/
struct MSG_50110003 : public Base_Msg  {
	uint32_t task_id;
	uint16_t type;
	uint8_t status;
	MSG_50110003(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint16(type);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint16(type)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110003;
		task_id = 0;
		type = 0;
		status = 0;
	}
};

/*
交付任务
成功返回 50110004,否则返回错误号.  

如任务可放弃,800200  800201会主动通知移除"以接列表"的任务信息.
*/
struct MSG_10110004  {
	uint32_t task_id;
	MSG_10110004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id = 0;
	}
};

/*
交付任务(返回)
*/
struct MSG_50110004 : public Base_Msg  {
	uint32_t task_id;
	uint16_t type;
	uint8_t status;
	MSG_50110004(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint16(type);
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint16(type)  ||  r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110004;
		task_id = 0;
		type = 0;
		status = 0;
	}
};

/*
领取任务奖励
自动交付任务用到．如领取成功返回 520206 ,　失败返回错误号 , 领取成功后客户端移除奖励状态．
*/
struct MSG_10110005  {
	uint32_t task_id;
	MSG_10110005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id = 0;
	}
};

/*
领取任务奖励(返回)
自动交付任务用到．如领取成功返回 520206 ,　失败返回错误号 , 领取成功后客户端移除奖励状态．
*/
struct MSG_50110005 : public Base_Msg  {
	MSG_50110005(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50110005;
	}
};

/*
批量接受任务
自动交付任务用到．如领取成功返回 520206 ,　失败返回错误号 , 领取成功后客户端移除奖励状态．
*/
struct MSG_10110006  {
	std::vector<uint32_t> task_id_list;
	int32_t type;
	MSG_10110006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __task_id_list_vec_size = task_id_list.size();
		w.write_uint16(__task_id_list_vec_size);
		for(uint16_t i = 0; i < __task_id_list_vec_size; ++i) {
				w.write_uint32(task_id_list[i]);
		}

		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		uint16_t __task_id_list_vec_size;
		if(r.read_uint16(__task_id_list_vec_size)  )
				return -1;
		task_id_list.reserve(__task_id_list_vec_size);
		for(uint16_t i = 0; i < __task_id_list_vec_size; ++i) {
				uint32_t v;
				if(r.read_uint32(v) )
						return -1;
				task_id_list.push_back(v);
		}

		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id_list.clear();
		type = 0;
	}
};

/*
批量接受任务(返回)
自动交付任务用到．如领取成功返回 520206 ,　失败返回错误号 , 领取成功后客户端移除奖励状态．
*/
struct MSG_50110006 : public Base_Msg  {
	int32_t type;
	MSG_50110006(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110006;
		type = 0;
	}
};

/*
快速完成
客户端请求快速完成任务的消息，服务端成功不返回。 如果失败，采用通用的错误消息，返回相应的错误号。
不管成功还是失败，都会主动发送任务更新或删除的消息。

type:  
       默认情况下，  type=0；
      如果策划配置物品、货币都可以扣，在这种情况下，如果物品不足，不处理快速完成，服务器主动发送81110005 消息。 客户端收到此消息，弹窗告知物品不足，是否愿意消耗指定货币及数量。 玩家点确定， 客户端此时再次发快速完成消息， type =1.  其他所有情况下， type=0 。
 
*/
struct MSG_10110007  {
	uint32_t task_id;
	uint8_t type;
	uint8_t is_bind_diamond_first;
	MSG_10110007(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
		w.write_uint8(type);
		w.write_uint8(is_bind_diamond_first);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  ||  r.read_uint8(type)  ||  r.read_uint8(is_bind_diamond_first)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id = 0;
		type = 0;
		is_bind_diamond_first = 0;
	}
};

/*
剧情消息
*/
struct MSG_10110008  {
	std::string plot_id;
	int8_t action;
	int16_t num;
	int8_t type;
	MSG_10110008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(plot_id);
		w.write_int8(action);
		w.write_int16(num);
		w.write_int8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(plot_id)  ||  r.read_int8(action)  ||  r.read_int16(num)  ||  r.read_int8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		plot_id.clear();
		action = 0;
		num = 0;
		type = 0;
	}
};

/*
剧情消息(返回)
*/
struct MSG_50110008 : public Base_Msg  {
	int8_t exist;
	MSG_50110008(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int8(exist);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int8(exist)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110008;
		exist = 0;
	}
};

/*
NPC对话任务，完成对话，请求完成任务
*/
struct MSG_10110009  {
	uint32_t npc_id;
	MSG_10110009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(npc_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(npc_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		npc_id = 0;
	}
};

/*
NPC对话任务，完成对话，请求完成任务(返回)
*/
struct MSG_50110009 : public Base_Msg  {
	MSG_50110009(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50110009;
	}
};

/*
完成副本清除剧情数据
*/
struct MSG_10110010  {
	std::vector<std::string> plot_ids;
	MSG_10110010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __plot_ids_vec_size = plot_ids.size();
		w.write_uint16(__plot_ids_vec_size);
		for(uint16_t i = 0; i < __plot_ids_vec_size; ++i) {
				w.write_string(plot_ids[i]);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __plot_ids_vec_size;
		if(r.read_uint16(__plot_ids_vec_size)  )
				return -1;
		plot_ids.reserve(__plot_ids_vec_size);
		for(uint16_t i = 0; i < __plot_ids_vec_size; ++i) {
				std::string v;
				if(r.read_string(v) )
						return -1;
				plot_ids.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		plot_ids.clear();
	}
};

/*
完成副本清除剧情数据(返回)
*/
struct MSG_50110010 : public Base_Msg  {
	MSG_50110010(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50110010;
	}
};

/*
任务随身怪处理
*/
struct MSG_10110011  {
	int32_t npc_type;
	int8_t set;
	int64_t npc_id;
	std::string funcName;
	int32_t direction;
	std::string sceneId;
	std::string open;
	int32_t x;
	int32_t y;
	std::string role_name;
	MSG_10110011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(npc_type);
		w.write_int8(set);
		w.write_int64(npc_id);
		w.write_string(funcName);
		w.write_int32(direction);
		w.write_string(sceneId);
		w.write_string(open);
		w.write_int32(x);
		w.write_int32(y);
		w.write_string(role_name);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(npc_type)  ||  r.read_int8(set)  ||  r.read_int64(npc_id)  ||  r.read_string(funcName)  ||  r.read_int32(direction)  ||  r.read_string(sceneId)  ||  r.read_string(open)  ||  r.read_int32(x)  ||  r.read_int32(y)  ||  r.read_string(role_name)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		npc_type = 0;
		set = 0;
		npc_id = 0;
		funcName.clear();
		direction = 0;
		sceneId.clear();
		open.clear();
		x = 0;
		y = 0;
		role_name.clear();
	}
};

/*
任务随身怪处理(返回)
*/
struct MSG_50110011 : public Base_Msg  {
	int32_t npc_type;
	int8_t set;
	int64_t npc_id;
	int64_t move_npc_id;
	std::string funcName;
	int32_t direction;
	std::string sceneId;
	std::string open;
	MSG_50110011(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int32(npc_type);
		w.write_int8(set);
		w.write_int64(npc_id);
		w.write_int64(move_npc_id);
		w.write_string(funcName);
		w.write_int32(direction);
		w.write_string(sceneId);
		w.write_string(open);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int32(npc_type)  ||  r.read_int8(set)  ||  r.read_int64(npc_id)  ||  r.read_int64(move_npc_id)  ||  r.read_string(funcName)  ||  r.read_int32(direction)  ||  r.read_string(sceneId)  ||  r.read_string(open)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110011;
		npc_type = 0;
		set = 0;
		npc_id = 0;
		move_npc_id = 0;
		funcName.clear();
		direction = 0;
		sceneId.clear();
		open.clear();
	}
};

/*
请求悬赏任务列表
备注：打开悬赏榜时，悬赏榜任务列表显示上一次刷新出来的列表。
         免费刷新、立即刷新，会把悬赏榜刷新为新的任务列表。
         is_refresh:  是否请求刷新。0:否， 1：是。
                            (1)打开悬赏榜时，应该显示上一次刷新出来的列表，所以， is_refresh=0 。
                            (2)点击  免费刷新、立即刷新， is_refresh = 1
*/
struct MSG_10110012  {
	uint8_t is_refresh;
	MSG_10110012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_refresh);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_refresh)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		is_refresh = 0;
	}
};

/*
请求悬赏任务列表(返回)
*/
struct MSG_50110012 : public Base_Msg  {
	uint8_t is_open_countdown;
	uint32_t auto_refresh_time;
	uint32_t free_refresh_count;
	uint32_t normal_accept_count;
	std::vector<Task_Info> reaward_task_list;
	MSG_50110012(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(is_open_countdown);
		w.write_uint32(auto_refresh_time);
		w.write_uint32(free_refresh_count);
		w.write_uint32(normal_accept_count);
		uint16_t __reaward_task_list_vec_size = reaward_task_list.size();
		w.write_uint16(__reaward_task_list_vec_size);
		for(uint16_t i = 0; i < __reaward_task_list_vec_size; ++i) {
				reaward_task_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(is_open_countdown)  ||  r.read_uint32(auto_refresh_time)  ||  r.read_uint32(free_refresh_count)  ||  r.read_uint32(normal_accept_count)  )
				return -1;
 		uint16_t __reaward_task_list_vec_size;
		if(r.read_uint16(__reaward_task_list_vec_size)  )
				return -1;
		reaward_task_list.reserve(__reaward_task_list_vec_size);
		for(uint16_t i = 0; i < __reaward_task_list_vec_size; ++i) {
				Task_Info v;
				if(v.deserialize(r))
						return -1;
				reaward_task_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50110012;
		is_open_countdown = 0;
		auto_refresh_time = 0;
		free_refresh_count = 0;
		normal_accept_count = 0;
		reaward_task_list.clear();
	}
};

/*
请求打开悬赏榜
备注：如果可以打开悬赏榜，服务端返回 50110013， 并且，status =1 (0：不能打开悬赏榜)。 如果不能打开悬赏榜，不发送50110013。
发送80000001 消息给客户端。
*/
struct MSG_10110013  {
	MSG_10110013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求打开悬赏榜（返回）
*/
struct MSG_50110013 : public Base_Msg  {
	uint8_t status;
	MSG_50110013(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(status);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(status)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		msg_id = 50110013;
		status = 0;
	}
};

/*
任务监听打开私聊窗口
*/
struct MSG_10110014  {
	int64_t role_id;
	MSG_10110014(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_int64(role_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_int64(role_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		role_id = 0;
	}
};

/*
请求提交道具
*/
struct MSG_10110015  {
	uint32_t task_id;
	MSG_10110015(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint32(task_id);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint32(task_id)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		task_id = 0;
	}
};

/*
请求恢复任务链
*/
struct MSG_10110016  {
	uint8_t type;
	MSG_10110016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};

/*
请求恢复任务链(返回)
*/
struct MSG_50110016 : public Base_Msg  {
	MSG_50110016(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
		msg_id = 50110016;
	}
};

/*
设置特效已经播放
*/
struct MSG_10110017  {
	uint32_t id;
	MSG_10110017(void) { reset(); };

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
请求显示日常任务列表
*/
struct MSG_10110018  {
	MSG_10110018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
	}

	int deserialize(Block_Buffer & r) {
		return 0;
	}

	void reset(void) {
	}
};

/*
请求显示日常任务列表(返回)
*/
struct MSG_50110018 : public Base_Msg  {
	std::vector<Task_Info> day_task_list;
	MSG_50110018(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		uint16_t __day_task_list_vec_size = day_task_list.size();
		w.write_uint16(__day_task_list_vec_size);
		for(uint16_t i = 0; i < __day_task_list_vec_size; ++i) {
				day_task_list[i].serialize(w);
		}

	}

	int deserialize(Block_Buffer & r) {
		uint16_t __day_task_list_vec_size;
		if(r.read_uint16(__day_task_list_vec_size)  )
				return -1;
		day_task_list.reserve(__day_task_list_vec_size);
		for(uint16_t i = 0; i < __day_task_list_vec_size; ++i) {
				Task_Info v;
				if(v.deserialize(r))
						return -1;
				day_task_list.push_back(v);
		}

		return 0;
	}

	void reset(void) {
		msg_id = 50110018;
		day_task_list.clear();
	}
};

/*
玩家已读体验过期消息
*/
struct MSG_10110019  {
	uint8_t type;
	MSG_10110019(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_uint8(type);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_uint8(type)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		type = 0;
	}
};



#endif