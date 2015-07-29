/*
* Generate by devtool
*/

#ifndef _MSG_TEST_H_
#define _MSG_TEST_H_

#include "Msg_Struct.h"


/* SERVER MSG */

/*
逻辑服测试消息
*/
struct MSG_10199999  {
	std::string command;
	MSG_10199999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		command.clear();
	}
};

/*
场景服测试消息
*/
struct MSG_10299999  {
	std::string command;
	MSG_10299999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		command.clear();
	}
};

/*
聊天服测试消息
*/
struct MSG_10399999  {
	std::string command;
	MSG_10399999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		command.clear();
	}
};

/*
网关服测试消息
*/
struct MSG_10499999  {
	std::string command;
	MSG_10499999(void) { reset(); };

	void serialize(Block_Buffer & w) const {
		w.write_string(command);
	}

	int deserialize(Block_Buffer & r) {
		if( r.read_string(command)  )
				return -1;
 		return 0;
	}

	void reset(void) {
		command.clear();
	}
};



#endif