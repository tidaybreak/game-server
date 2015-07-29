
#include "Chat_Friend.h"
#include "Chat_Player.h"

Chat_Friend::Chat_Friend(void) {
	reset();
}

Chat_Friend::~Chat_Friend() {
}

void Chat_Friend::reset(void) {
	black_list_.clear();
}

int Chat_Friend::syn_black_list(int8_t type, role_id_t role_id) {
	if(type) {
		black_list_.insert(role_id);
	} else {
		black_list_.erase(role_id);
	}
	return 0;
}

bool Chat_Friend::is_in_black_list(role_id_t role_id) {
	return black_list_.find(role_id) != black_list_.end();
}

int Chat_Friend::deserialize_black_list(Block_Buffer &buffer) {
	uint16_t size;
	if(buffer.read_uint16(size)  )
			return -1;

	for(uint16_t i = 0; i < size; ++i) {
			int64_t v;
			if(buffer.read_int64(v) )
					return -1;
			black_list_.insert(v);
	}
	return 0;
}
