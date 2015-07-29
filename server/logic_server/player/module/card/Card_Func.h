/*
 * Card_Func.h
 *
 *  Created on: 2014年11月26日
 *      Author: root
 */

#ifndef CARD_FUNC_H_
#define CARD_FUNC_H_

struct Card_Config;
struct Int_Int;
struct Card_Info;
struct Card_Addon_Detail;

int handle_rand_card_part(const Card_Config &cfg, Int_Int &result);

int handle_get_potential(const Card_Config &cfg);

int handle_get_sub_attr(const Card_Config &cfg);

double handle_get_attr_value(int id, int attr_id, int star, int lv, int poten);

int handle_get_ttl_exp(int cur_lv, int qlt, int type);

bool is_card_max_type(int type);

int get_next_id_dif_by_type(int type);

#endif /* CARD_FUNC_H_ */
