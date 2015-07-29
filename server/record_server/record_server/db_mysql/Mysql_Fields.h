// -*- C++ -*-
/*
 * Mysql_Fields.h
 *
 *  Created on: Nov 19, 2012
 *      Author: Ti
 */

#ifndef MYSQL_FIELDS_H_
#define MYSQL_FIELDS_H_

#include "Game_Typedef.h"

struct Data_Collector {
	std::string insert_head_; 		/// SQL INSERT语句头, 包含INSERT INTO Table VALUES, 不包含其后的()中的数据
	std::string sql_str_; 			/// 拼接出来的SQL语句
	size_t num_, max_num_; 			/// 当前收集器中的数据条数和最大条数
	Time_Value first_data_timestamp_, timeout_tv_;

	Data_Collector(void) {
		reset();
	}

	inline void set(std::string &insert_head, size_t max_num, const Time_Value &timeout_tv) {
		insert_head_ = insert_head;
		sql_str_ = insert_head_;
		max_num_ = max_num;
		timeout_tv_ = timeout_tv;
	}

	inline void reset_used(void) {
		sql_str_ = insert_head_;
		num_ = 0;
		first_data_timestamp_ = Time_Value::zero;
	}

	inline void reset(void) {
		insert_head_.clear();
		sql_str_.clear();
		num_ = max_num_ = 0;
		first_data_timestamp_ = timeout_tv_ = Time_Value::zero;
	}

	bool is_timeout(Time_Value &now) {
		return (num_ != 0 && (now - first_data_timestamp_ > timeout_tv_));
	}
};

template <typename FT>
struct My_Fields {
	FT data_;
	bool is_null_;

	My_Fields(void) : is_null_(false) { }

	void reset(void) {
		is_null_ = false;
	}
};

struct Table {
	Table(void) { }
	virtual ~Table(void) { }

	void append_insert_content(Data_Collector &collector) {
		if (collector.num_ == 0) {
			collector.first_data_timestamp_ = Time_Value::gettimeofday();
		} else {
			collector.sql_str_ += ",";
		}
		++collector.num_;

		append_insert_handler(collector);
	}

	virtual void append_insert_handler(Data_Collector &collector) = 0;
};

struct Test_Table: public Table {
	My_Fields<int> role_id_;
	My_Fields<std::string> role_name_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Loginout_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<std::string> role_name_;
	My_Fields<std::string> account_;
	My_Fields<uint16_t> level_;
	My_Fields<std::string> client_ip_;
	My_Fields<uint32_t> login_time_;
	My_Fields<uint32_t> logout_time_;
	My_Fields<uint32_t> online_time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Item_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint16_t> type_;
	My_Fields<uint32_t> item_id_;
	My_Fields<uint32_t> update_num_;
	My_Fields<uint16_t> bind_;
	My_Fields<int64_t> ext_1_;
	My_Fields<int64_t> ext_2_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Money_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint16_t> type_;
	My_Fields<uint16_t> money_id_1_;
	My_Fields<uint32_t> oper_money_1_;
	My_Fields<uint32_t> remain_money_1_;
	My_Fields<uint16_t> money_id_2_;
	My_Fields<uint32_t> oper_money_2_;
	My_Fields<uint32_t> remain_money_2_;
	My_Fields<uint32_t> item_id_;
	My_Fields<uint32_t> item_num_;
	My_Fields<int64_t> ext_1_;
	My_Fields<int64_t> ext_2_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct FB_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint32_t> fb_id_;
	My_Fields<uint32_t> scene_id_;
	My_Fields<uint16_t> type_;
	My_Fields<uint32_t> value_;
	My_Fields<int64_t> ext_1_;
	My_Fields<int64_t> ext_2_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Task_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint32_t> task_id_;
	My_Fields<uint16_t> value_;
	My_Fields<int64_t> ext_1_;
	My_Fields<int64_t> ext_2_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Level_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint16_t> level_;
	My_Fields<uint32_t> scene_id_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Pet_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint32_t> pet_cid_;
	My_Fields<uint16_t> pet_stream_type_;
	My_Fields<uint32_t> level_;
	My_Fields<uint32_t> growth_;
	My_Fields<uint32_t> quality_;
	My_Fields<uint32_t> ext_1_;
	My_Fields<uint32_t> ext_2_;
	My_Fields<uint32_t> ext_3_;
	My_Fields<uint32_t> ext_4_;
	My_Fields<uint32_t> ext_5_;
	My_Fields<uint32_t> ext_6_;
	My_Fields<uint32_t> ext_7_;
	My_Fields<uint32_t> ext_8_;
	My_Fields<uint32_t> ext_9_;
	My_Fields<uint32_t> ext_10_;
	My_Fields<int64_t> ext_i64_1_;
	My_Fields<int64_t> ext_i64_2_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Drops_Stream: public Table {
	My_Fields<uint32_t> scene_id_;
	My_Fields<uint32_t> npc_type_;
	My_Fields<uint32_t> item_;
	My_Fields<uint32_t> money_type_;
	My_Fields<uint32_t> money_value_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Pet_Stream_Info {
	/*
	 * pet_cid：宠物类型ID
	 * pet_stream_type：流水类型
	 * level：宠物等级
	 * growth：成长
	 * quality：资质
	 * ext_1：32位扩展字段1 [相关id或值]
	 * ext_{n}：32位扩展字段{n} [相关id或值]
	 * ext_i64_1：64位扩展字段1 [相关id或值]
	 * ext_i64_{n}：64位扩展字段{n} [相关id或值]
	 * time：时间戳
	 */
	int64_t role_id;
	uint32_t pet_cid;
	uint16_t pet_stream_type;
	uint32_t level;
	uint32_t growth;
	uint32_t quality;
	uint32_t ext_1;
	uint32_t ext_2;
	uint32_t ext_3;
	uint32_t ext_4;
	uint32_t ext_5;
	uint32_t ext_6;
	uint32_t ext_7;
	uint32_t ext_8;
	uint32_t ext_9;
	uint32_t ext_10;
	int64_t ext_i64_1;
	int64_t ext_i64_2;
	uint32_t time;

	void reset(void) {
		role_id = 0;
		pet_cid = 0;
		pet_stream_type = 0;
		level = 0;
		growth = 0;
		quality = 0;
		ext_1 = 0;
		ext_2 = 0;
		ext_3 = 0;
		ext_4 = 0;
		ext_5 = 0;
		ext_6 = 0;
		ext_7 = 0;
		ext_8 = 0;
		ext_9 = 0;
		ext_10 = 0;
		ext_i64_1 = 0;
		ext_i64_2 = 0;
		time = 0;
	}
};

struct Mount_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint16_t> mount_stream_type_;
	My_Fields<uint32_t> facade_id_;
	My_Fields<uint32_t> level_;
	My_Fields<uint32_t> experience_;
	My_Fields<uint32_t> evolve_level_;
	My_Fields<int64_t> ext_i64_1_;
	My_Fields<int64_t> ext_i64_2_;
	My_Fields<uint32_t> ext_1_;
	My_Fields<uint32_t> ext_2_;
	My_Fields<uint32_t> ext_3_;
	My_Fields<uint32_t> ext_4_;
	My_Fields<uint32_t> ext_5_;
	My_Fields<uint32_t> ext_6_;
	My_Fields<uint32_t> ext_7_;
	My_Fields<uint32_t> ext_8_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Equip_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint16_t> equip_stream_type_;
	My_Fields<uint32_t> equip_id_;
	My_Fields<uint16_t> bind_;
	My_Fields<uint32_t> level_;
	My_Fields<uint32_t> score_;
	My_Fields<int64_t> ext_i64_1_;
	My_Fields<int64_t> ext_i64_2_;
	My_Fields<uint32_t> ext_1_;
	My_Fields<uint32_t> ext_2_;
	My_Fields<uint32_t> ext_3_;
	My_Fields<uint32_t> ext_4_;
	My_Fields<uint32_t> ext_5_;
	My_Fields<uint32_t> ext_6_;
	My_Fields<uint32_t> ext_7_;
	My_Fields<uint32_t> ext_8_;
	My_Fields<uint32_t> ext_9_;
	My_Fields<uint32_t> ext_10_;
	My_Fields<uint32_t> ext_11_;
	My_Fields<uint32_t> ext_12_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct FB_Member_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint16_t> career_;
	My_Fields<uint16_t> level_;
	My_Fields<uint32_t> fb_id_;
	My_Fields<uint32_t> scene_id_;
	My_Fields<int64_t> team_id_;
	My_Fields<int64_t> leader_id_;
	My_Fields<std::string> member_ids_;
	My_Fields<uint32_t> ext_1_;
	My_Fields<uint32_t> ext_2_;
	My_Fields<int64_t> ext_i64_;
	My_Fields<uint32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Comprehension_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<uint16_t> stream_type_;
	My_Fields<uint32_t> value_;
	My_Fields<uint32_t> time_;
	My_Fields<uint32_t> ext_1_;
	My_Fields<uint32_t> ext_2_;
	My_Fields<int64_t> ext_i64_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Faction_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<int64_t> faction_id_;
	My_Fields<uint16_t> stream_type_;
	My_Fields<uint32_t> time_;
	My_Fields<uint64_t> value_1_;
	My_Fields<uint64_t> value_2_;
	My_Fields<uint64_t> ext_1_;
	My_Fields<uint64_t> ext_2_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Activity_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<int32_t> activity_type;
	My_Fields<int32_t> join_time;
	My_Fields<int32_t> play_time;
	My_Fields<int32_t> till_end;
	My_Fields<double> degree;
	My_Fields<int32_t> ext_32_1;
	My_Fields<int32_t> ext_32_2;
	My_Fields<int64_t> ext_64_1;
	My_Fields<int64_t> ext_64_2;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Qingyi_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<int16_t> stream_type_;
	My_Fields<int32_t> update_qingyi_;
	My_Fields<int32_t> remain_qingyi_;
	My_Fields<int16_t> qingyi_lvl_;
	My_Fields<int64_t> friend_id_;
	My_Fields<int32_t> ext_1_;
	My_Fields<int64_t> ext_i64_;
	My_Fields<int32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Transfer_Scene_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<int16_t> stream_type_;
	My_Fields<int32_t> s_scene_;
	My_Fields<int32_t> s_coord_x_;
	My_Fields<int32_t> s_coord_y_;
	My_Fields<int32_t> d_scene_;
	My_Fields<int32_t> d_coord_x_;
	My_Fields<int32_t> d_coord_y_;
	My_Fields<int32_t> result_;
	My_Fields<int32_t> ext_i32_;
	My_Fields<int64_t> ext_i64_;
	My_Fields<int32_t> time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Honor_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<int16_t> stream_type_;
	My_Fields<int32_t> update_honor_;
	My_Fields<int32_t> remain_honor_;
	My_Fields<int32_t> honor_lvl_;
	My_Fields<int32_t> time_;
	My_Fields<int32_t> ext_i32_1_;
	My_Fields<int32_t> ext_i32_2_;
	My_Fields<int32_t> ext_i32_3_;
	My_Fields<int32_t> ext_i32_4_;
	My_Fields<int64_t> ext_i64_1_;
	My_Fields<int64_t> ext_i64_2_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Gem_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<int16_t> stream_type_;
	My_Fields<int32_t> gem_id_;
	My_Fields<int32_t> gem_step_;
	My_Fields<int32_t> gem_purity_;
	My_Fields<int32_t> time_;
	My_Fields<int32_t> ext_i32_1_;
	My_Fields<int32_t> ext_i32_2_;
	My_Fields<int32_t> ext_i32_3_;
	My_Fields<int32_t> ext_i32_4_;
	My_Fields<int32_t> ext_i32_5_;
	My_Fields<int32_t> ext_i32_6_;
	My_Fields<int32_t> ext_i32_7_;
	My_Fields<int32_t> ext_i32_8_;
	My_Fields<int32_t> ext_i32_9_;
	My_Fields<int32_t> ext_i32_10_;
	My_Fields<int32_t> ext_i32_11_;
	My_Fields<int32_t> ext_i32_12_;
	My_Fields<int64_t> ext_i64_1_;

	virtual void append_insert_handler(Data_Collector &collector);
};

// 获得道具类型
enum ITEM_GET_TYPE
{
	ITEM_PICKUP = 1, //------------------拾取包裹内物品；子类型：场景id
	ITEM_BUY = 2, //---------------------购买商店物品
	ITEM_BUY_BACK = 3, //----------------购回售出物品
	ITEM_OBTAIN = 4, //------------交易获得；子类型：对方角色id
	ITEM_SHOPPING = 5, //----------------商城购买
	ITEM_TASK_AWARD = 6, //--------------任务奖励

};
//	失去道具类型
enum ITEM_USE_TYPE
{
	ITEM_USE = 1, //------------------使用物品,子类型：如果是蛮兽的物品，为蛮兽的ID
	ITEM_DROP = 2, //-----------------丢弃物品
	ITEM_SELL = 3, //-----------------出售物品
	ITEM_TRADE_LOSE = 4, //-----------交易失去；子类型：对方角色id

};

//// 获得金钱类型
//enum MONEY_GET_TYPE
//{
//	MONEY_RECHARGE = 1, //------------------充值
//	MONEY_SELL_GOODS = 2, //----------------卖出物品; 子项目：物品id
//	MONEY_OBTAIN_COUPON = 3, //-------------获得礼券
//	MONEY_MAIL_MONEY = 4, //----------------邮件铜钱; 子项目：对方id
//	MONEY_TASK_AWARD = 5, //----------------任务奖励; 子项目：任务id
//	MONEY_TRADE_OBTAIN = 6, //--------------交易获得; 子项目：交易对方id
//	MONEY_TEST_BOX = 7, //------------------封测礼包赠送
//	MONEY_STALLAGE_SELL = 8,//--------------摆摊收入; 子项目：对方id
//	MONEY_PACKAGE_DROPPED = 9, //-----------物品掉落获得
//	MONEY_LEAGUE_ADD = 10, //---------------宗派收入
//	MONEY_AWARD = 11, //--------------------后台奖励金钱
//};
//// 失去金钱类型
//enum MONEY_USE_TYPE
//{
//	MONEY_MEND_ALL_EQUIP = 1, //---------修理所有装备
//	MONEY_EQUIP_REFINE = 2, //-----------装备精炼; 子项目：装备id
//	MONEY_GEMSTONE_COMBINE = 3, //-------宝石合成; 子项目：宝石id
//	MONEY_GEMSTONE_REMOVE = 4, //--------宝石拆除; 子项目：装备id
//	MONEY_EQUIP_INJECT = 5, //-----------装备注灵; 子项目：装备id
//	MONEY_TRANSFER_POST_SCENE = 6, //----传送指定场景; 子项目：场景id
//	MONEY_SKILL_LEARN = 7, //------------学习技能; 子项目：技能id
//	MONEY_BUY_GOODS = 8, //--------------商店购买物品; 子项目：物品id
//	MONEY_BUY_BACK_GOODS = 9, //---------购回售出物品; 子项目：物品id
//	MONEY_CONFIRM_SHOPING = 10, //--------商城购买物品; 子项目：物品id
//	MONEY_TRADE_PAY = 11, //--------------交易支付; 子项目：对方id
//	MONEY_MAIL = 12,//--------------------发送邮件; 子项目：对方id
//};

enum FB_STREAM_TYPE
{
    FB_STREAM_DROP = 2,                          //副本BOSS掉落
    FB_STREAM_WAVE = 3,                          //副本波数
    FB_STREAM_RATE = 4,                          //副本完成度
    FB_STREAM_FLOOR = 5,                         //副本通关层数
    FB_STREAM_FAME = 6,                          //副本声望
    FB_STREAM_IN = 7,                            //副本进入
    FB_STREAM_OUT = 8,                           //副本离开
    FB_STREAM_MONEY = 9,                         //副本获得金钱
    FB_STREAM_EXP = 10,                          //副本获得经验
    FB_STREAM_FETCH_MONEY = 11,                  //领取副本金钱
    FB_STREAM_FETCH_EXP = 12,                    //领取副本经验
    FB_STREAM_WIPE = 13,                         //副本扫荡流水
    FB_STREAM_FETCH_WUXING = 14,                 //副本领取悟性

    OTHER_SUB_TYPE_END
};

enum PET_STREAM_TYPE {
	PET_STREAM_GET = 1,				//宠物获得流水
	PET_STREAM_ABANDON = 2,			//宠物丢弃流水
	PET_STREAM_COMBINE = 3,			//宠物融合流水 ext_1:副宠类型id
	PET_STREAM_GROWTH = 4,			//宠物成长流水 ext_1: 是否使用保护符
	PET_STREAM_SKILL_LERAN = 5,		//宠物技能学习流水 ext_1: 宠物学习的技能
};

enum MOUNT_STREAM_TYPE {
	MOUNT_STREAM_GET = 1,			//坐骑获得流水
	MOUNT_STREAM_FEED = 2,			//坐骑喂养流水
	MOUNT_STREAM_TRAIN = 3,			//坐骑培养流水 ext_1:培养类型
	MOUNT_STREAM_EVOLVE = 4,		//坐骑进阶流水
	MOUNT_STREAM_TRAIN_SAVE = 5,	//坐骑培养保存
	MOUNT_STREAM_EQUIP_ENHANCE = 6,	//坐骑装备强化
};

enum Equip_Stream_Type {
    EQUIP_STREAM_ON = 1,                         //穿上装备
    EQUIP_STREAM_OFF = 2,                        //脱下装备
    EQUIP_STREAM_INCREASE = 3,                   //装备强化
    EQUIP_STREAM_ADDITION_SAVE = 4,              //装备保存洗炼属性
    EQUIP_STREAM_LEVEL_UP = 5,                   //装备进阶
    EQUIP_STREAM_GEMSTONE_FILL = 6,              //宝石镶嵌
    EQUIP_STREAM_GEMSTONE_ERASE = 7,             //宝石摘除
    EQUIP_STREAM_INHERIT = 8,                    //装备传承
    EQUIP_STREAM_WUHUN_DAN_CHANGE = 9,           //武魂段位改变
    EQUIP_STREAM_SEAR_UPGRADE = 10,              //烙印升级
    EQUIP_STREAM_SEAR_REFINE = 11,               //烙印精炼
    EQUIP_STREAM_QUENCH = 12,                    //装备淬炼
    EQUIP_STREAM_SMELT = 13,                     //装备熔炼
    EQUIP_STREAM_FAHUN_STEP_UP = 14,             //装备法魂进阶

    EQUIP_STREAM_END
};

enum Comprehension_Stream_Type {
    COMPREHENSION_GET_TASK = 1,                  //正常任务获得
    COMPREHENSION_GET_TASK_USE_ITEM = 2,         //消耗道具完成任务获得
    COMPREHENSION_GET_UNLEARN_SKILL = 3,         //技能重修获得
    COMPREHENSION_GET_MARS_TEST_ENTER = 4,       //战神考验进入挑战获得
    COMPREHENSION_GET_MARS_TEST_AUTO = 5,        //战神考验自动挑战获得
    COMPREHENSION_GET_EXCHANGE = 6,              //兑换获得
    COMPREHENSION_GET_ITEM = 7,					 //使用道具获得
    COMPREHENSION_GET_ZHIZUN = 8,				//活动-演武至尊

    COMPREHENSION_USE_SKILL = 10001,           	 //技能消耗
    COMPREHENSION_GM = 20000,					 //gm获得
};

enum Qingyi_Stream_Type {
    QINGYI_KILL_BOSS = 2,                        //组队杀BOSS获得
    QINGYI_FB = 3,                               //组队打副本获得
    QINGYI_GET_FLOWER = 4,                       //收获鲜花获得
    QINGYI_SEND_FLOWER = 5,                      //赠送鲜花获得
    QINGYI_KILL_LITTLE_MONSTER = 6,              //组队杀小怪获得
};

enum Transfer_Scene_Stream_Type {
    TRANSFER_SCENE_STREAM_NOMAL = 1,             //一般场景传送类型
};

enum Faction_Stream_Type {
    FACTION_JOIN = 1,                            //加入帮派
    FACTION_EXIT = 2,                            //退出帮派
    FACTION_STORAGE_PUT = 3,                     //存入帮派仓库
    FACTION_STORAGE_GET = 4,                     //帮派仓库取出
    FACTION_GET_CONTRIBUTION = 5,				 //获得帮派贡献
    FACTION_USE_CONTRIBUTION = 6,				 //使用帮派贡献
    FACTION_GET_GOLD = 7,						 //获得荒金
    FACTION_USE_GOLD = 8, 						 //使用荒金
};

enum Honor_Stream_Type {
    HONOR_STREAM_USE_ITEM = 1,                   //使用物品获得
    HONOR_STREAM_MAIL = 2,                       //邮件获得
    HONOR_STREAM_ZHIZUN_EMPTY = 3,               //演武至尊轮空获得
    HONOR_STREAM_ZHIZUN_LEIZHU_WIN = 4,          //演武至尊擂主战胜奖励
    HONOR_STREAM_ZHIZUN_TIAOZHAN_LOSS = 5,       //演武至尊挑战者战败奖励
    HONOR_STREAM_ZHIZUN_LEIZHU_LOSS = 6,         //演武至尊擂主战败奖励
    HONOR_STREAM_ZHIZUN_TIAOZHAN_WIN = 7,        //演武至尊挑战者战胜奖励
};

enum Gem_Stream_Type {
    GEM_STREAM_REFINE = 1,                       //宝石提纯
    GEM_STREAM_COMBINE = 2,                      //宝石合成
};


#endif /* MYSQL_FIELDS_H_ */
