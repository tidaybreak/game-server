/*
 * Fashion.h
 *
 *  Created on: 2014年9月3日
 *      Author: root
 */

#ifndef FASHION_H_
#define FASHION_H_

#include "Logic_Player_Base.h"

struct Fashion_Detail;
struct Fashion_Item;

class Fashion : virtual public Logic_Player_Base {
public:
	enum Upgrade_Stuff_Type {
		TYPE_UPGRADE		= 1,
		TYPE_ADVANCED		= 2,
	};

	Fashion(void) { reset(); }
	virtual ~Fashion(void);
	void reset(void);

	static int create_init(Fashion_Detail &detail);

	void load_detail(Fashion_Detail &detail);
	void save_detail(Fashion_Detail &detail);

	void sign_int(void);
	void sign_out(void);

	void module_init(void);

	int handle_get_fashion_info(void);

	int handle_use_fashion_item(UInt_Vec item_idx_vec, int to_wear);

	int handle_use_one_fashion_item(uint32_t index, int to_wear, Int_Int_Map &suit_map);

	void handle_get_new_suit(Int_Int_Map &suit_map);

	int handle_wear_fashion(Int_Vec fashion_id_vec);

	int handle_fashion_part_upgrade(int part, int auto_buy = 0);

	int handle_change_fashion_show_state(int state);

	void refresh_fashion_prop_total(std::map<int, double> &prop_val_map);

	void refresh_fashion_prop(std::map<int, double> &prop_val_map, Int_Int_Map &suit_id_map);

	void refresh_fashion_suit_prop(Int_Int_Map &suit_id_map, std::map<int, double> &prop_val_map);

	void refresh_fashion_avatar(Int_Int_Map &avatar_map);

	inline const Fashion_Detail& fashion_detail(void) const;

protected:
	void fashion_soul_upgrade(void);

private:
	Fashion_Detail *fashion_detail_;
};

inline const Fashion_Detail& Fashion::fashion_detail(void) const {
	return *fashion_detail_;
}

#endif /* FASHION_H_ */
