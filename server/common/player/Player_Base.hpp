/*
 * Player_Base.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: ti
 */


template <typename PLAYER>
Player_Base<PLAYER>::Player_Base() {
	// TODO Auto-generated constructor stub
}

template <typename PLAYER>
Player_Base<PLAYER>::~Player_Base() {
	// TODO Auto-generated destructor stub
}

template <typename PLAYER>
void Player_Base<PLAYER>::reset(void) {
	event_set_.clear();
	event_pool_.clear();
	Event_Queue tmp;
	std::swap(event_queue_, tmp);
}

template <typename PLAYER>
int Player_Base<PLAYER>::create_event(int type, event_fun fun, Time_Value interval, int loop_times) {
	Player_Event *event = event_pool_.pop();
	if (!event) {
		return 0;
	}

	event->type = type;
	event->fun = fun;
	event->interval = interval;
	event->loop_times = loop_times;
	event->next_opt = Time_Value::gettimeofday() + Time_Value(interval);

	if (event_set_.count(type)) {
		event_pool_.push(event);
		LOG_USER_TRACE("event has in set, type:%d event size:%d", type, event_set_.size());
		return -1;
	}

	event_set_.insert(type);
	event_queue_.push(event);

	return 0;
}

template <typename PLAYER>
int Player_Base<PLAYER>::remove_event(int type) {
	Player_Event event;
	event.type = type;

	Event_Set::iterator event_it = event_set_.find(type);
	if (event_it == event_set_.end()) // 防止定时器不在时间队列时的最差效率情况
		return 0;

	/// 删除定时事件
	Player_Event *e = 0;
	std::vector<Player_Event *> tvec;
	while (! event_queue_.empty()) {
		e = event_queue_.top();
		if (e->type == type) {
			event_queue_.pop();
			event_pool_.push(e);
			break;
		} else {
			event_queue_.pop();
			tvec.push_back(e);
		}
	}

	for (typename std::vector<Player_Event *>::iterator it = tvec.begin(); it != tvec.end(); ++it) {
		event_queue_.push(*it);
	}

	event_set_.erase(type);

	return 0;
}

template <typename PLAYER>
int Player_Base<PLAYER>::process_event(void) {
	Player_Event *evh = 0;

	while (1) {
		if (event_queue_.empty())
			break;

		if ((evh = this->event_queue_.top()) == 0) {
			LOG_USER("evh == 0");
			break;
		}

		Time_Value now(Time_Value::gettimeofday());
		if (evh->next_opt <= now) {
			this->event_queue_.pop();
			event_set_.erase(evh->type);
			(player_self()->*(evh->fun))();

			//(this->*(evh->fun))();
			if (evh->loop_times != INT_MAX)
				evh->loop_times--;
			if (evh->loop_times > 0) {
				now += evh->interval;
				evh->next_opt = now;
				this->event_queue_.push(evh);
				event_set_.insert(evh->type);
			} else {
				event_pool_.push(evh);
			}
		} else {
			break;
		}
	}

	return 0;
}

template <typename PLAYER>
void Player_Base<PLAYER>::make_message(Block_Buffer &buf, int msg_id) {
	buf.write_uint32(0); /// length
	buf.write_uint32(msg_id);
	buf.write_int64(role_id());
}

template <typename PLAYER>
int Player_Base<PLAYER>::send_err_to_client(int error_id, int source) {

	return 0;
}

template <typename PLAYER>
int Player_Base<PLAYER>::send_tip_to_client(int error_id, int arg1) {

	return 0;
}

template <typename PLAYER>
int Player_Base<PLAYER>::send_tip_to_client(int error_id, std::string &name) {

	return 0;
}
