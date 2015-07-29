/*
 * Logic_Outpost_Manager.cpp
 *
 *  Created on: 2015年4月8日
 *      Author: root
 */


#include "Logic_Outpost_Manager.h"
#include "Msg_Inner_Enum.h"
#include "Msg_Scene_Outpost_Enum.h"
#include "Msg_Active_Struct.h"
#include "Logic_Player.h"



Logic_Outpost_Manager::Logic_Outpost_Manager(void){
	reset();
}

Logic_Outpost_Manager::~Logic_Outpost_Manager(void){

}

void Logic_Outpost_Manager::reset(void){

}

int Logic_Outpost_Manager::inner_sync_info(Logic_Player* player, Block_Buffer& buf){
	int8_t sync_type = 0;
	buf.read_int8(sync_type);
	switch(sync_type){
		case 10:{//购买流浪商人物品
			if(!player){
				return 0;
			}
			int scene_id = 0;
			int64_t monster_role_id = 0;
			int item_index = 0;
			int item_id = 0;
			int item_amount = 0;
			int8_t cost_type = 0;
			int cost_num  = 0;

			buf.read_int32(scene_id);
			buf.read_int64(monster_role_id);
			buf.read_int32(item_index);
			buf.read_int32(item_id);
			buf.read_int32(item_amount);
			buf.read_int8(cost_type);
			buf.read_int32(cost_num);

			int buy_result = 0;
			Money_Sub_Info m_s_info((Pack::Money_Sub_Type)cost_type, cost_num, MONEY_SUB_VAGRANT_STORE);
			if(cost_num > 0){
				buy_result = player->pack_try_sub_money(m_s_info);
			}
			if(buy_result != 0){
				player->send_err_to_client(buy_result, REQ_VAGRANT_BUY_ITEM);
				{
					Block_Buffer buf;
					buf.make_message(INNER_SYNC_SCENE_OUTPOST);
					buf.write_int8(sync_type);
					buf.write_int32(scene_id);
					buf.write_int64(monster_role_id);
					buf.write_int32(item_index);
					buf.write_int8(0);
					buf.finish_message();
					player->send_to_scene(buf);
				}
				return 0;
			}
			// 背包检测
			Item_Detail item = Item_Detail(item_id, item_amount, (Item_Detail::Bind_Type)0);
			buy_result = player->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX, item);
			if( buy_result != 0){
				player->send_err_to_client(buy_result, REQ_VAGRANT_BUY_ITEM);
				{
					Block_Buffer buf;
					buf.make_message(INNER_SYNC_SCENE_OUTPOST);
					buf.write_int8(sync_type);
					buf.write_int32(scene_id);
					buf.write_int64(monster_role_id);
					buf.write_int32(item_index);
					buf.write_int8(0);
					buf.finish_message();
					player->send_to_scene(buf);
				}
				return 0;
			}
			// 添加到背包
			player->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX, item,Gain_Item_DM_Info(Pack::ITEM_GAIN_VAGRANT_STORE));
			// 扣钱
			if(cost_num > 0){
				player->pack_sub_money(m_s_info);
			}
			{
				MSG_81000102 acv_msg;
				acv_msg.type = 0;
				Item_Basic_Info ibi;
				item.set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
				OBJ_SEND_TO_CLIENT(acv_msg, (*player));
			}
			Block_Buffer buf;
			buf.make_message(INNER_SYNC_SCENE_OUTPOST);
			buf.write_int8(sync_type);
			buf.write_int32(scene_id);
			buf.write_int64(monster_role_id);
			buf.write_int32(item_index);
			buf.write_int8(1);
			buf.finish_message();
			player->send_to_scene(buf);
			return 0;
			break;
		}
		case 20:{// 宝藏海湾神秘祝福
			int8_t bless_index;
			int cost_type = 0;
			int cost_num = 0;
			buf.read_int8(bless_index);
			buf.read_int32(cost_type);
			buf.read_int32(cost_num);
			int buy_result = 0;
			Money_Sub_Info m_s_info((Pack::Money_Sub_Type)cost_type, cost_num, MONEY_SUB_TBAY_BLESS);
			if(cost_num > 0){
				buy_result = player->pack_try_sub_money(m_s_info);
			}
			if(buy_result != 0){
				player->send_err_to_client(buy_result, REQ_TBAY_BLESS);
				{
					Block_Buffer buf;
					buf.make_message(INNER_SYNC_SCENE_OUTPOST);
					buf.write_int8(sync_type);
					buf.write_int8(bless_index);
					buf.write_int8(0);
					buf.finish_message();
					player->send_to_scene(buf);
				}
				return 0;
			}
			// 扣钱
			if(cost_num > 0){
				player->pack_sub_money(m_s_info);
			}
			Block_Buffer buf;
			buf.make_message(INNER_SYNC_SCENE_OUTPOST);
			buf.write_int8(sync_type);
			buf.write_int8(bless_index);
			buf.write_int8(1);
			buf.finish_message();
			player->send_to_scene(buf);
			return 0;
			break;
		}
		default:{
			break;
		}
	}
	return 0;
}

