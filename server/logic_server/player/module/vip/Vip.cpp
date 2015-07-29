/*
 * Vip.cpp
 *
 *  Created on: 2014-07-21
 *      Author: xiaoliang
 */

#include "Vip.h"
#include "Vip_Struct.h"
#include "Logic_Module_Head.h"
#include "Config_Cache_Vip.h"
#include "Logic_Player.h"
#include "Err_Code.h"
#include "Config_Cache_Mail.h"
#include "Logic_Public.h"
#include "Msg_Active_Struct.h"
#include "Msg_Vip_Struct.h"
#include "Vip_Def.h"
#include "pack/Pack_Def.h"
#include "Msg_Inner_Struct.h"
#include "gang/Gang_Manager.h"
#include "active/Active_Content_Manager.h"
#include "Config_Cache.h"
#include "fighter/Logic_Fighter_Struct.h"
#include "public/Public_Def.h"

Vip::Vip(void) {
	reset();
}

Vip::~Vip() {
	// TODO Auto-generated destructor stub
}

void Vip::reset(void) {
	vip_detail_ = 0;
}

int Vip::create_init(Vip_Detail &detail) {

	return 0;
}

void Vip::load_detail(Vip_Detail &detail) {
	vip_detail_ = &detail;
}

void Vip::save_detail(Vip_Detail &detail) {
	if (vip_detail().is_change) {
		detail = vip_detail();
		vip_detail_->is_change = false;
	}
}

int Vip::trigger_daily_zero(const Time_Value &now) {
	vip_detail_->has_receive_daily_reward = false;
	return 0;
}

int Vip::trigger_weekly_zero(const Time_Value &now) {

	return 0;
}

int Vip::test_trigger_daily_six(const Time_Value &now) { //todo delete
	return Vip::trigger_daily_six(now);
}

void Vip::test_set_last_settle_grow(uint32_t days) { //todo delete
	Time_Value now = Time_Value::gettimeofday();
	vip_detail_->last_settle_grow.sec(now.sec() - days*3600*24);
}

int Vip::trigger_daily_six(const Time_Value &now) {
	uint32_t vip_level = player_self()->vip();
	if(vip_level) { //没过期
		//增加每天成长值
		get_grow(vip_detail_->last_settle_grow, now);
		//增加消费成长值
		get_cost_grow(now);
		//尝试升级VIP
		try_vip_level_up();
	} else {  //如果过期了或未开通
		//补全未过期时候的成长值
		get_grow(vip_detail_->last_settle_grow, player_self()->base_detail().vip_over_time);//vip_detail_->last_settle_grow,在函数内可能会被修改
		//尝试升级VIP
		try_vip_level_up();
		//扣除过期后的每天丢失的成长值
			/*
			 * 使用最后一次结算时间是为了下次结算不重复扣除已经扣过的天数的成长值
			 * get_grow() lost_grow() 两个函数在成长值变化的时候;最后一次结算时间会被赋予第二个参数(结束时间)的值
			 * */
		lost_grow(vip_detail_->last_settle_grow, now);
	}
	return 0;
}

int Vip::sync_scene_module_info(Block_Buffer &buf) {

	return 0;
}

int Vip::move_scene_completed(void) {

	return 0;
}

int Vip::client_ready_now(void) {
	send_vip_info_client(VIP_ACTIVE_READY_NOW);
	if(player_self()->level() >= 15) {
		send_world_tree_icon_status(1);
	}
	return 0;
}

void Vip::send_world_tree_icon_status(int8_t open) {
	MSG_81000408 msg;
	msg.open = get_vip_world_tree_have_receive();//open
	msg.active = get_vip_world_tree_have_receive();
	msg.time = 0;
	msg.icon_id = 2;
	THIS_SEND_TO_CLIENT(msg);
}

void Vip::module_init(void) {
	const World_Tree_Conf_Map &world_tree_map = CONFIG_CACHE_VIP->world_tree_conf_map();
	for(World_Tree_Conf_Map::const_iterator iter = world_tree_map.begin(); iter != world_tree_map.end(); ++iter) {
		if(vip_detail_->world_tree_val.find(iter->first) == vip_detail_->world_tree_val.end()) {
			World_Tree_Flock wtf;
			wtf.reset();
			wtf.id = iter->first;
			vip_detail_->world_tree_val.insert(std::make_pair(iter->first, wtf));
		}
	}
	//删除配置不存在的
	UInt_Set todo_del;
	todo_del.clear();
	for(World_Tree_Flock_Map::const_iterator iter = vip_detail_->world_tree_val.begin(); iter != vip_detail_->world_tree_val.end(); ++iter) {
		if(world_tree_map.find(iter->first) == world_tree_map.end()) {
			todo_del.insert(iter->first);
		}
	}
	for(UInt_Set::iterator it = todo_del.begin(); it != todo_del.end(); ++it) {
		World_Tree_Flock_Map::iterator iter = vip_detail_->world_tree_val.find(*it);
		if(iter != vip_detail_->world_tree_val.end()) {
			vip_detail_->world_tree_val.erase(iter);
		}
	}
	vip_detail_->detail_change();
}

void Vip::sign_in(void) {

}

void Vip::sign_out(void) {

}

void Vip::trusteeship_sign_out(void) {

}

void Vip::listen_vip_expired(const Time_Value &now) {
	if(player_self()->base_detail().vip_over_time <= now && player_self()->base_detail().vip_over_time != Time_Value::zero && !vip_detail_->expired_prompt) {
		send_expired_vip_mail();//发送过期邮件
		vip_detail_->expired_prompt = true;
		send_vip_info_client(VIP_ACTIVE_EXPIRED); //向客户端发送主动广播VIP最新信息
		vip_level_change(player_self()->vip(), player_self()->base_detail().vip);//等级改变
		vip_expired_listen(player_self()->base_detail().vip_over_time, now);
	}
}

void Vip::open_vip(uint32_t days, uint32_t init_level) {
	open_vip_hours(days * 24, init_level);
}

void Vip::open_vip_hours(uint32_t hours, uint32_t init_level) {
	Time_Value over_time = Time_Value::gettimeofday();
	uint32_t old_level = player_self()->vip(); //开通前的等级
	if(old_level) {	//当前等级为0;证明过期或未开通过
		over_time.sec(player_self()->base_detail().vip_over_time.sec() + hours * 3600);
	} else {
		over_time.sec(over_time.sec() + hours * 3600);
	}
	player_self()->set_vip_over_time(over_time);//设置新的过期时间;开通or续费
	/**************开通了**************/
	if(init_level > player_self()->vip()) {//开通或续费后的等级;要小于初始化的等级,补级
		player_self()->set_vip_level(init_level);
		if(init_level == 4) {
			player_self()->create_ann_init_open_vip4();//开通VIP4增加全服广播
		}
	}
	if(old_level != player_self()->vip()) {
		vip_level_change(player_self()->vip(), old_level); //等级改变接口
	}
	//如果使用体验卡就有体验时间
	if(hours && hours <= VIP_EXPERIENCE_CARD_HOURS) {//体验开通
		vip_detail_->exp_over_time.sec(Time_Value::gettimeofday().sec() + hours * 3600);
		LOGIC_PUBLIC->send_mail(VIP_SEND_EXPERIENCE_MAIL_ID, player_self()->role_id());
	}
	//判断初始开通和续费
	if(vip_detail_->last_settle_grow == Time_Value::zero) {
		if(hours && hours <= VIP_EXPERIENCE_CARD_HOURS) {
		} else {//初始开通(体验卡开通不领取每日成长值; 但也不会影响以后使用非体验卡开通VIP的初始开通邮件)
			vip_detail_->last_settle_grow = Time_Value::gettimeofday();
			send_open_vip_mail();
			//LOGIC_PUBLIC->send_mail(VIP_SEND_INIT_OPEN_MAIL_ID, player_self()->role_id());
		}
		send_vip_info_client(VIP_ACTIVE_OPEN);
	} else {
		send_contiune_vip_mail();
		send_vip_info_client(VIP_ACTIVE_CONTIUNE);
	}
	vip_detail_->expired_prompt = false;
	vip_detail_->detail_change();
	try_vip_level_up();
	//同步到聊天服
	sync_vip_info_to_chat();
}

void Vip::set_vip_exp_overtime(const Time_Value &time) {
	if(vip_detail_->exp_over_time.sec() > time.sec()) {
		vip_detail_->exp_over_time.sec(time.sec());
		vip_detail_->detail_change();
	}
}

int Vip::sign_in_repeat(void) {

	return 0;
}

uint32_t Vip::get_vip_days(void) {
	//求出VIP剩余天数
	Time_Value now = Time_Value::gettimeofday();
	int32_t sec = player_self()->base_detail().vip_over_time.sec() - now.sec();
	if(sec <= 0) {
		return 0;
	}
	//uint8_t mod = sec % (3600*24);
	uint32_t days = sec/(3600*24);// + ((mod == 0)?0:1);
	return days;
}

void Vip::try_vip_level_up(void) {
	uint32_t vip_level = player_self()->vip();
	if(vip_level) {
		const Grow_Get_Config *conf =  CONFIG_CACHE_VIP->get_grow_get_config(vip_level + 1);
		bool has_level_up = false;
		while(conf && vip_detail_->grow_val >= conf->need_val) {  //兼容一次获取很多成长值
			player_self()->set_vip_level(conf->vip_level);
			vip_detail_->grow_val -= conf->need_val;
			vip_detail_->detail_change();
			conf =  CONFIG_CACHE_VIP->get_grow_get_config(conf->vip_level + 1);
			has_level_up = true;
		}
		if(has_level_up) {
			send_vip_info_client(VIP_ACTIVE_LEVEL_UP);
			vip_level_change(player_self()->vip(), vip_level);
		}
	}
}

void Vip::lost_grow(const Time_Value &start_time, const Time_Value &end_time) {
	if(start_time == Time_Value::zero) { //出现0时间的情况只会出现在未开通过VIP
		return;
	}
	const Grow_Get_Config *conf =  CONFIG_CACHE_VIP->get_grow_get_config(player_self()->base_detail().vip);
	if(!conf) {
		return;
	}
	if(end_time.sec() > start_time.sec()) {
		long int slip_sec = end_time.sec() - start_time.sec(); //时间间隔的秒数
		uint32_t slip_days = slip_sec/(3600*24); //间隔天数
		tm tm_end,tm_start; //求开始和结束的TM
		{
			time_t t_end = end_time.sec();
			localtime_r(&t_end, &tm_end);
			time_t t_start = start_time.sec();
			localtime_r(&t_start, &tm_start);
		}
		if(tm_end.tm_hour >= 6 && tm_start.tm_hour < 6) { //如果开始在六点前,结束在6点后; 加一天
			++slip_days;
		}
		int32_t lost_to_val = vip_detail_->grow_val - conf->lost_val * slip_days;
		if(lost_to_val >= 0) {
			vip_detail_->grow_val = lost_to_val;
		} else {
			vip_detail_->grow_val = 0;
		}
		vip_detail_->last_settle_grow = end_time;
		vip_detail_->detail_change();
		send_vip_info_client(VIP_ACTIVE_GROW_CHANGE);
	}
}

void Vip::get_grow(const Time_Value &start_time, const Time_Value &end_time) {
	if(start_time == Time_Value::zero) { //出现0时间的情况只会出现在未开通过VIP
		return;
	}
	MSG_DEBUG("get grow on six role_id:%ld, start:%ld, end:%ld", player_self()->role_id(), start_time.sec(), end_time.sec());
	const Grow_Get_Config *conf =  CONFIG_CACHE_VIP->get_grow_get_config(player_self()->base_detail().vip);
	if(!conf) {
		return;
	}
	if(end_time.sec() > start_time.sec()) {
		long int slip_sec = end_time.sec() - start_time.sec(); //时间间隔的秒数
		uint32_t slip_days = slip_sec/(3600*24); //间隔天数
		tm tm_end,tm_start; //求开始和结束的TM
		{
			time_t t_end = end_time.sec();
			localtime_r(&t_end, &tm_end);
			time_t t_start = start_time.sec();
			localtime_r(&t_start, &tm_start);
		}
		if(tm_end.tm_hour >= 6 && tm_start.tm_hour < 6) { //如果开始在六点前,结束在6点后; 加一天
			++slip_days;
		}
		vip_detail_->grow_val += conf->get_val * slip_days; //补全所缺天数成长值
		vip_detail_->last_settle_grow = end_time;
		vip_detail_->detail_change();
		MSG_DEBUG("get grow on six role_id:%ld, slip_days:%d, oldval:%d, newval:%d", player_self()->role_id(), slip_days, conf->get_val, vip_detail_->grow_val);
		send_vip_info_client(VIP_ACTIVE_GROW_CHANGE);
	}
}

void Vip::get_cost_grow(const Time_Value &now) {
	const Grow_Get_Config *conf =  CONFIG_CACHE_VIP->get_grow_get_config(player_self()->vip());
	if(!conf) {
		return;
	}
	uint32_t over_money = vip_detail_->cost_money % conf->cost_ratio;  //结算后余额
	uint32_t get_grow = vip_detail_->cost_money / conf->cost_ratio;	   //结算应该得到的成长值
	MSG_DEBUG("player: %ld, cost vip add grow, cost_money: %d, get_grow: %d, over_money: %d",player_self()->role_id(), vip_detail_->cost_money, get_grow, over_money);
	vip_detail_->cost_money = over_money;
	vip_detail_->grow_val += get_grow;
	vip_detail_->detail_change();
	if(get_grow) {
		send_vip_info_client(VIP_ACTIVE_GROW_CHANGE);
	}
	//尝试升级VIP
	try_vip_level_up();
}

void Vip::listen_vip_cost_money(const int32_t sub_num, const uint16_t type) {
	if(type != MONEY_SUB_MAIL_MONEY
			&& type != MONEY_SUB_TRADE_USE
			&& type != MONEY_SUB_MARKET_BUY
			&& type != MONEY_SUB_GM) {
		vip_detail_->cost_money += sub_num;
		MSG_DEBUG("player: %ld,  cost vip add money: %d ", player_self()->role_id(), sub_num);
		vip_detail_->detail_change();
		//增加消费成长值
		get_cost_grow(Time_Value::gettimeofday());
	}
}

int8_t Vip::get_vip_world_tree_have_receive(void) {
	for(World_Tree_Flock_Map::iterator iter = vip_detail_->world_tree_val.begin(); iter != vip_detail_->world_tree_val.end(); ++iter) {
		World_Tree_Flock &wtf = (iter->second);
		const World_Tree_Config *conf =  CONFIG_CACHE_VIP->get_world_tree_config(wtf.id);
		if(conf) {
			uint32_t addition = 0;
			if(conf->addition.size() >= player_self()->vip()) {
				addition = conf->addition[player_self()->vip()];
			}
			if(addition) {
				int32_t copper_num = (wtf.copper * addition) / 100 - wtf.receive_copper;
				int32_t souls_num = (wtf.souls * addition) / 100 - wtf.receive_souls;
				int32_t dragon_soul_num = (wtf.dragon_soul * addition) / 100 - wtf.receive_dragon_soul;
				if(copper_num >= 20000 || souls_num >= 20000 || dragon_soul_num >= 20000) {//策划坑 两万后才显示特效
					return 1;
				}
			}
		}
	}
	return 0;
}

void Vip::listen_vip_reward(const uint32_t type, const uint32_t money_type, const uint32_t num) {
	World_Tree_Flock_Map::iterator iter = vip_detail_->world_tree_val.find(type);
	if(iter != vip_detail_->world_tree_val.end()) {
		switch(money_type) {
			case COPPER: {
				iter->second.copper += num;
				break;
			}
			case SOULS: {
				iter->second.souls += num;
				break;
			}
			case DRAGON_SOUL: {
				iter->second.dragon_soul += num;
				break;
			}
		}
	} else {
		if(!CONFIG_CACHE_VIP->get_world_tree_config(type)) {
			return;
		}
		World_Tree_Flock wtf;
		wtf.reset();
		switch(money_type) {
			case COPPER: {
				wtf.copper += num;
				break;
			}
			case SOULS: {
				wtf.souls += num;
				break;
			}
			case DRAGON_SOUL: {
				wtf.dragon_soul += num;
				break;
			}
		}
		vip_detail_->world_tree_val.insert(std::make_pair(type, wtf));
	}
	vip_detail_->detail_change();
	int8_t have_receive = get_vip_world_tree_have_receive();
	if(have_receive) {
		send_world_tree_icon_status(1);
	}
}

int Vip::req_vip_receive_world_tree(uint32_t id) {
	World_Tree_Flock_Map::iterator iter = vip_detail_->world_tree_val.find(id);
	const World_Tree_Config *conf =  CONFIG_CACHE_VIP->get_world_tree_config(id);
	if(iter == vip_detail_->world_tree_val.end() || !conf) {
		return ERROR_CLIENT_PARAM;
	}
	World_Tree_Flock &wtf = (iter->second);
	uint32_t addition = 0;
	if(conf->addition.size() > player_self()->vip()) {
		addition = conf->addition[player_self()->vip()];
	}
	if(addition == 0) {
		return 0;
	}
	Money_Add_List money_add_list;
	int32_t copper_num = (wtf.copper * addition) / 100 - wtf.receive_copper;
	int32_t souls_num = (wtf.souls * addition) / 100 - wtf.receive_souls;
	int32_t dragon_soul_num = (wtf.dragon_soul * addition) / 100 - wtf.receive_dragon_soul;
	{
		money_add_list.clear();
		//金币
		if(copper_num > 0) {
			Money_Add_Info copper(Money_Type(COPPER), copper_num, MONEY_ADD_VIP_WORLD_TREE);
			money_add_list.push_back(copper);
		}
		//英灵
		if(souls_num > 0) {
			Money_Add_Info souls(Money_Type(SOULS), souls_num, MONEY_ADD_VIP_WORLD_TREE);
			money_add_list.push_back(souls);
		}
		//龙魂
		if(dragon_soul_num > 0) {
			Money_Add_Info dragon_soul(Money_Type(DRAGON_SOUL), dragon_soul_num, MONEY_ADD_VIP_WORLD_TREE);
			money_add_list.push_back(dragon_soul);
		}
	}
	if(money_add_list.size() <= 0) {
		return ERROR_VIP_TREE_NOT_RECEIVE;
	}
	int return_status = player_self()->pack_add_money(money_add_list);
	if(return_status != 0) {
		return return_status;
	}
	wtf.receive_copper += copper_num;
	wtf.receive_souls += souls_num;
	wtf.receive_dragon_soul += dragon_soul_num;
	vip_detail_->detail_change();
	MSG_50106202 msg;
	msg.reset();
	msg.id = id;
	msg.tree_info = wtf;
	THIS_SEND_TO_CLIENT(msg);
	send_world_tree_icon_status(1);//需要统计是否还有可领
	{ // 背包获得客户端特效
		MSG_81000102 acv_msg;
		if (copper_num > 0) {
			acv_msg.property.push_back(Property(COPPER, copper_num));
		}
		if (souls_num > 0) {
			acv_msg.property.push_back(Property(SOULS, souls_num));
		}
		if (dragon_soul_num > 0) {
			acv_msg.property.push_back(Property(DRAGON_SOUL, dragon_soul_num));
		}
		if (!acv_msg.property.empty()) {
			THIS_SEND_TO_CLIENT(acv_msg);
		}
	}
	return 0;
}

int Vip::req_vip_world_tree_info(void) {
	MSG_50106201 msg;
	for(World_Tree_Flock_Map::iterator iter = vip_detail_->world_tree_val.begin(); iter != vip_detail_->world_tree_val.end(); ++iter) {
		msg.info.push_back(iter->second);
	}
	const Server_Maintainer_Cache server_maintainer = CONFIG_CACHE->server_maintainer_cache();
	msg.all_day = get_time_subdays(server_maintainer.game_open_time, Time_Value::gettimeofday());
	msg.re_day = msg.all_day;
	if(player_self()->base_detail().vip_over_time.sec()){
		msg.re_day = get_time_subdays(player_self()->base_detail().vip_over_time, Time_Value::gettimeofday());
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Vip::make_vip_turntable_data(Turntable_Config &data, std::vector<uint32_t> &id_list) {//更新物品组内容
	data.reset();
	data.max_open = 8;
	data.list_size = 18;
	//生成3个中奖次在1的;
	//生成3个中奖次在2的;
	//生成2个中奖次在3的;
	//生成2个中奖次在4的;
	//生成2个中奖次在5的;
	//生成2个中奖次在6的;
	//生成2个中奖次在7的;
	//生成2个中奖次在8的;
	for(int j = 1; j <= 8; ++j) {//第几次中奖的物品
		const Turntable_Item_Config_Vec *conf = CONFIG_CACHE_VIP->turntable_item_conf_vec_by_type(j);
		if(!conf){
			return ERROR_CONFIG_NOT_EXIST;
		}
		int n = 3;
		if(j > 2) {//第一第二次有3个; 剩余的2个
			n = 2;
		}
		for(int i = 0;  i < n; ++i) {//个数
			UInt_Vec tem_item_list;//临时抽取的物品id列表
			tem_item_list.clear();
			Int_Vec weight;
			weight.clear();
			for(Turntable_Item_Config_Vec::const_iterator it = conf->begin(); it != conf->end(); ++it) {
				if(data.item_list.find(it->id) == data.item_list.end()) {//物品组 不允许重复id
					tem_item_list.push_back(it->id);
					weight.push_back(it->rate);
				}
			}
			if(tem_item_list.empty() || weight.empty()) {//有可能出现在配置不合理的情况下 如配置的物品不住够概率的抽取
				return ERROR_CONFIG_NOT_EXIST;
			}
			int index = Utility_Func::lottery_one(weight);
			data.item_list.insert(tem_item_list[index]);
			id_list.push_back(tem_item_list[index]);
		}
	}
	return 0;
}

int Vip::req_vip_turntable_open() {//请求打开VIP转盘获取物品组
//	Int_Vec weight;
//	weight.clear();
//	const Turntable_Config_Vec &trun_conf = CONFIG_CACHE_VIP->turntable_conf_vec();
//	for(Turntable_Config_Vec::const_iterator iter = trun_conf.begin(); iter != trun_conf.end(); ++iter) {
//		weight.push_back(iter->rate);
//	}
//	int index = Utility_Func::lottery_one(weight);
//	vip_detail_->turntable_data = trun_conf[index];
	MSG_50106203 msg;
	msg.reset();
	int result = make_vip_turntable_data(vip_detail_->turntable_data, msg.item_id);
	if(result) {
		return result;
	}
	vip_detail_->turntable_reward.clear();
	vip_detail_->turntable_cost = 0;

	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Vip::req_vip_turntable_lottery(uint8_t type) {//请求VIP转盘抽奖
	if(!player_self()->vip()) {
		return ERROR_IS_NO_VIP;
	}
	switch(type) {
		case VIP_TURNTABLE_LOTTERY_ONE: {
			int32_t id;
			int re_status = vip_turntable_lottery(id, VIP_TURNTABLE_LOTTERY_ONE);
			if(re_status) {
				return re_status;
			}
			if(id) {
				MSG_50106204 msg;
				msg.reset();
				msg.id.push_back(id);
				THIS_SEND_TO_CLIENT(msg);
			}
			break;
		}
		case VIP_TURNTABLE_LOTTERY_ALL: {
			MSG_50106204 msg;
			msg.reset();
			int re_status;
			do{
				int32_t id;
				re_status = vip_turntable_lottery(id, VIP_TURNTABLE_LOTTERY_ALL);
				if(!re_status && id) {
					msg.id.push_back(id);
				}
			}while(re_status == 0);
			if(msg.id.size()) {
				THIS_SEND_TO_CLIENT(msg);
			}
			if(re_status) {
				return re_status;
			}
			break;
		}
		default: {
			return ERROR_VIP_TURNTABLE_TYPE_ERROR;
		}
	}
	return 0;
}

int Vip::req_open_vip_daily_receive(void){
	MSG_50106207 msg;
	if(player_self()->vip()) {
		msg.rec = vip_detail_->has_receive_daily_reward?1:0;
	} else {
		msg.rec = 2;//不是VIP
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Vip::req_vip_daily_receive(void){
	if(vip_detail_->has_receive_daily_reward) {
		return ERROR_RECEIVED_VIP_DAILY;
	}
	const Vip_Daily_Item_Config_Vec *conf = CONFIG_CACHE_VIP->get_vip_daily_config(player_self()->vip());
	if(conf) {
		Item_Vec item_list; item_list.clear();
		Money_Add_List money_add_list; money_add_list.clear();
		uint32_t exp = 0;
		MSG_81000102 acv_msg;
		acv_msg.type = 0;
		for(Vip_Daily_Item_Config_Vec::const_iterator it = conf->begin(); it != conf->end(); ++it) {
			if(it->item_id == PT_EXP_CURRENT) {
				exp += it->num;
			} else {
				if(it->item_id > 999999) {
					Item_Detail item = Item_Detail(it->item_id, it->num, it->bind);
					item_list.push_back(item);
					Item_Basic_Info ibi;
					item.set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
				} else {
					money_add_list.push_back(Money_Add_Info(Money_Type(it->item_id), it->num,Money_DM_Info(MONEY_ADD_VIP_DAILY)));
					acv_msg.property.push_back(Property(it->item_id, it->num));
				}
			}
		}
		if(!item_list.empty()) {
			int result = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item_list, Gain_Item_DM_Info(ITEM_GAIN_VIP_DAILY));
			if(result) {
				return result;
			}
		}
		if(!money_add_list.empty()) {
			player_self()->pack_add_money(money_add_list);
		}
		if(exp) {
			const Exp_All exp_all = player_self()->modify_experience_inter(exp, false, false, true);
			if (0 == exp_all.result) {
				acv_msg.property.push_back(Property(PT_EXP_CURRENT, exp_all.all));
				acv_msg.vip_exp = exp_all.vip_exp;
				acv_msg.world_level = player_self()->get_world_level_add();
			}
		}
		vip_detail_->has_receive_daily_reward = true;
		vip_detail_->detail_change();
		MSG_50106206 msg;
		THIS_SEND_TO_CLIENT(msg);
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}

int Vip::req_vip_turntable_receive(void){
	if(vip_detail_->turntable_cost) {//减少VIP币
		int return_status = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(VIP_CURRENCY_ITEM_ID, vip_detail_->turntable_cost), Lose_Item_DM_Info(ITEM_LOSE_VIP_TURNTABLE));
		if(return_status != 0) {
			return return_status;
		}
	}
	MSG_81000102 acv_msg;
	acv_msg.type = 0;

	{//增加获得奖品
		std::vector<Item_Basic_Info> item_vector;
		item_vector.clear();
		Item_Basic_Info ibi;
		int try_all_status = player_self()->pack_try_insert_item(PACK_T_PACKAGE_INDEX, vip_detail_->turntable_reward);
		if(try_all_status) {//不能一次放下所有
			for(Item_Vec::iterator iter = vip_detail_->turntable_reward.begin(); iter != vip_detail_->turntable_reward.end(); ++iter) {
				int return_status = player_self()->pack_try_insert_item(PACK_T_PACKAGE_INDEX, *iter);
				if(return_status != 0) {
					ibi.reset();
					iter->set_item_basic_info(ibi);
					item_vector.push_back(ibi);
				} else {
					player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, *iter, Gain_Item_DM_Info(ITEM_GAIN_VIP_TURNTABLE));

					Item_Basic_Info ibi;
					iter->set_item_basic_info(ibi);
					acv_msg.item_info.push_back(ibi);
				}
			}
			if(item_vector.size()) {
				LOGIC_PUBLIC->send_mail(VIP_SEND_TURNTABLE_RECEIVE_MAIL_ID, role_id(), item_vector);
			}
		} else {//可以一次放下所有
			player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, vip_detail_->turntable_reward, Gain_Item_DM_Info(ITEM_GAIN_VIP_TURNTABLE));
			for(Item_Vec::iterator iter = vip_detail_->turntable_reward.begin(); iter != vip_detail_->turntable_reward.end(); ++iter) {
				Item_Basic_Info ibi;
				iter->set_item_basic_info(ibi);
				acv_msg.item_info.push_back(ibi);
			}
		}
	}//增加获得奖品 end
//	player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, vip_detail_->turntable_reward, Gain_Item_DM_Info(ITEM_GAIN_VIP_TURNTABLE));
	vip_detail_->turntable_reward.clear();
	vip_detail_->turntable_cost = 0;
	if (!acv_msg.item_info.empty()) {
		THIS_SEND_TO_CLIENT(acv_msg);
	}
	return 0;
}

int Vip::req_open_vip(uint32_t id) {
	const Vip_Open_Config *conf =  CONFIG_CACHE_VIP->get_vip_open_config(id);
	if(!conf) {
		return ERROR_CLIENT_PARAM;
	}
	uint16_t dm_info;
	if(vip_detail_->last_settle_grow == Time_Value::zero) {
		dm_info = MONEY_SUB_OPEN_VIP;  //开通VIP
	} else {
		dm_info = MONEY_SUB_CONTINUE_VIP; //续费VIP
	}
	//是否有足够的金钱
	int return_status = player_self()->pack_try_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), conf->price, dm_info));
	if(return_status != 0) {
		return return_status;
	}
	//减掉相应的钱
	if(player_self()->pack_sub_money(Money_Sub_Info(Money_Sub_Type(GOLD_ONLY), conf->price, dm_info)) == 0) {
		open_vip(conf->days, conf->init_level);
	}
	//回应客户端
	MSG_50106200 msg;
	msg.reset();
	msg.id = id;
	msg.vip_over_time = player_self()->base_detail().vip_over_time.sec();
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

void Vip::send_open_vip_mail(uint32_t days) {
	int mail_id = VIP_SEND_OPEN_MAIL_ID;
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
	if(mailc) {
		uint32_t vip_level = player_self()->vip();
		uint32_t vip_days = days;
		if(days == 0) {
			 vip_days = get_vip_days();
		}
		std::string contents;
		const int size_buf = mailc->mail_content.size() + \
				sizeof(vip_level) + 10 + \
				sizeof(vip_level) + 10 + \
				sizeof(vip_days) + 10;
		char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mailc->mail_content.c_str(), vip_level, vip_level, vip_days);
		contents.assign(content_buf);
		LOGIC_PUBLIC->send_mail(mail_id, player_self()->role_id(), "", contents);
	}
}

void Vip::send_contiune_vip_mail(void) {
	int mail_id = VIP_SEND_CONTIUNE_MAIL_ID;
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
	if(mailc) {
		uint32_t vip_level = player_self()->vip();
		uint32_t vip_days = get_vip_days();
		std::string contents;
		const int size_buf = mailc->mail_content.size() + \
				sizeof(vip_level) + 10 + \
				sizeof(vip_level) + 10 + \
				sizeof(vip_days) + 10;
		char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mailc->mail_content.c_str(), vip_level, vip_level, vip_days);
		contents.assign(content_buf);
		LOGIC_PUBLIC->send_mail(mail_id, player_self()->role_id(), "", contents);
	}
}

void Vip::send_expired_vip_mail(void) {
	int mail_id = VIP_SEND_EXPIRED_MAIL_ID;
	const Mail_Base_Config *mailc = CONFIG_CACHE_MAIL->find_config(mail_id);
	const Grow_Get_Config *conf =  CONFIG_CACHE_VIP->get_grow_get_config(player_self()->base_detail().vip);
	if(mailc && conf) {
		uint32_t pri_num = conf->pri_num;
		uint32_t lost_val = conf->lost_val;
		std::string contents;
		const int size_buf = mailc->mail_content.size() + \
				sizeof(pri_num) + 10 + \
				sizeof(lost_val) + 10;
		char content_buf[size_buf];
		memset(content_buf, 0, size_buf);
		snprintf(content_buf, size_buf, mailc->mail_content.c_str(), pri_num, lost_val);
		contents.assign(content_buf);
		LOGIC_PUBLIC->send_mail(mail_id, player_self()->role_id(), "", contents);
	}
}

void Vip::send_vip_info_client(uint8_t type) {
	{
		MSG_82200230 msg;
		msg.reset();
		msg.vip_level = player_self()->vip();
		msg.flock_vip_level = player_self()->base_detail().vip;
		msg.vip_overtime = player_self()->base_detail().vip_over_time.sec();
		msg.grow_val = vip_detail_->grow_val;
		msg.exp_overtime = vip_detail_->exp_over_time.sec();
		msg.active_type = type;
		if(player_self()->vip()) {
			msg.daily_rec = vip_detail_->has_receive_daily_reward?1:0;
		} else {
			msg.daily_rec = 2;//不是VIP
		}
		THIS_SEND_TO_CLIENT(msg);
	}

	{//改变通知场景服
		if(type != VIP_ACTIVE_GROW_CHANGE) {
			MSG_20200020 msg;
			msg.reset();
			msg.vip = player_self()->vip();
			msg.vip_over_time = player_self()->base_detail().vip_over_time.sec();
			THIS_SEND_TO_SCENE(msg);
		}
	}
}

int Vip::vip_turntable_lottery(int32_t &id, uint8_t type) {
	id = 0;
	//判断次数
	uint8_t use_num = vip_detail_->turntable_data.list_size - vip_detail_->turntable_data.item_list.size();
	if(use_num >= vip_detail_->turntable_data.max_open) {
		if(type == VIP_TURNTABLE_LOTTERY_ALL) {
			return -1;
		}
		return ERROR_VIP_TURNTABLE_NUM;
	}

	const Turntable_Cost_Config *tc_conf = CONFIG_CACHE_VIP->get_turntable_cost_config(use_num + 1);
	{	//验证VIP币
		if(!tc_conf) {
			return ERROR_INNER_SERVER;
		}
		int return_status = player_self()->pack_try_erase_item(PACK_T_PACKAGE_INDEX,Id_Amount(VIP_CURRENCY_ITEM_ID, vip_detail_->turntable_cost + tc_conf->cost));
		if(return_status != 0) {
			return return_status;
		}
	}
	unsigned int index = 0;
	int32_t tmp_id = 0;
	{//按物品权重求概率
		UInt_Vec list;
		Int_Vec weight;
		weight.clear();
		for(UInt_Mset::iterator iter = vip_detail_->turntable_data.item_list.begin(); iter != vip_detail_->turntable_data.item_list.end(); ++iter) {
			const Turntable_Item_Config *conf = CONFIG_CACHE_VIP->get_turntable_item_config(*iter);
			if(!conf){
				return ERROR_INNER_SERVER;
			}
			if(conf->type.find(use_num + 1) != conf->type.end()) {
				weight.push_back(conf->rate);
				list.push_back(conf->id);
			}
		}
		index = Utility_Func::lottery_one(weight);
		if(list.size() <= index) {
			return ERROR_INNER_SERVER;
		}
		tmp_id = list[index];
	}
	//验证背包放物品
	const Turntable_Item_Config *conf = CONFIG_CACHE_VIP->get_turntable_item_config(tmp_id);
	if(!conf) {
		return ERROR_INNER_SERVER;
	}
	Item_Detail item(conf->item_id, conf->num, Item_Detail::BIND);
	vip_detail_->turntable_reward.push_back(item);//记录转盘得到的物品
	vip_detail_->turntable_cost += tc_conf->cost; //记录转盘共消耗VIP币数量
	if(conf->ann) {
		//发送广播
		Chat_Item_Info item_;
		item_.reset();
		item_.item_base_info.id = conf->item_id;
		item_.item_base_info.amount = conf->num;
		player_self()->create_vip_turntable_announce(item_);
	}
//	{
//		int return_status = player_self()->pack_try_insert_item(PACK_T_PACKAGE_INDEX,item);
//		if(return_status != 0) {
//			return return_status;
//		}
//	}
	//都通过验证
	{	//删除一个
		vip_detail_->turntable_data.item_list.erase(vip_detail_->turntable_data.item_list.find(tmp_id));
	}
//	{//减少VIP币
//		int return_status = player_self()->pack_erase_item(PACK_T_PACKAGE_INDEX, Id_Amount(VIP_CURRENCY_ITEM_ID, 1), Lose_Item_DM_Info(ITEM_LOSE_VIP_TURNTABLE));
//		if(return_status != 0) {
//			return return_status;
//		}
//	}
//	{//增加获得奖品
//		int return_status = player_self()->pack_insert_item(PACK_T_PACKAGE_INDEX, item, Gain_Item_DM_Info(ITEM_GAIN_VIP_TURNTABLE));
//		if(return_status != 0) {
//			return return_status;
//		}
//	}
	id = tmp_id;
	return 0;
}

void Vip::vip_level_change(uint32_t new_vip_level, uint32_t old_vip_level) {
	MSG_DEBUG("player:%ld , vip_level_change new_level:%d, old_level: %d", player_self()->role_id(), new_vip_level, old_vip_level);
	player_self()->dragon_vale_listen_vip_level_chance(new_vip_level);
	player_self()->arena_listen_vip_level_change(new_vip_level, old_vip_level);
	player_self()->mount_listen_vip_level_change(new_vip_level, old_vip_level);
	ACTIVE_CONTENT_MANAGER->listen_all_player_vip_level_change(new_vip_level, old_vip_level, player_self());
	if (player_self()->gang_id()) {
		GANG_MANAGER->notify_ganger_row_info(GANG_ROW_INFO_UPDATE, role_id(), player_self()->gang_id());
	}
	player_self()->active_content_listen_vip_level_change(new_vip_level);
	player_self()->task_listen_vip_level_change(new_vip_level);

	//同步到聊天服
	sync_vip_info_to_chat();
}


void Vip::sync_vip_info_to_chat(void) {
	MSG_20200024 inner_msg;
	inner_msg.vip_level = player_self()->base_detail().vip;
	inner_msg.over_time = player_self()->base_detail().vip_over_time.sec();
	Block_Buffer buf;
	buf.make_message(inner_msg.msg_id);
	inner_msg.serialize(buf);
	buf.finish_message();
	this->send_to_local_chat(buf);
}

void Vip::vip_listen_level_up(const int level) {
	if(level == 15) {
		send_world_tree_icon_status(1);
	}
}

int Vip::get_vip_addition(uint32_t type) {
	const UInt_Vec *conf = CONFIG_CACHE_VIP->get_vip_addition_config(type);
	if(conf && conf->size() > player_self()->vip()) {
		return (*conf)[player_self()->vip()];
	}
	return 0;
}

void Vip::vip_expired_listen(const Time_Value &over_time, const Time_Value &now) {
	//vip过期监听接口
	player_self()->player_try_time_out(TRY_VIP_TIMEOUT, over_time);
}
