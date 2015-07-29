
#ifndef CHAT_FRIEND_H_
#define CHAT_FRIEND_H_

#include "Chat_Player_Base.h"

struct Chat_Friend_Info;

class Chat_Friend : public Chat_Player_Base {
public:

	Chat_Friend(void);
	virtual ~Chat_Friend();
	void reset(void);

	int syn_black_list(int8_t type, role_id_t role_id);

	bool is_in_black_list(role_id_t role_id);

	int deserialize_black_list(Block_Buffer &buffer);

private:
	Role_Id_Set black_list_;
};


#endif /* FRIEND_H_ */

