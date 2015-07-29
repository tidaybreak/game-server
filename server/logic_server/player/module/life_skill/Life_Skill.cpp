
#include "Logic_Module_Head.h"
#include "Life_Skill_Struct.h"
#include "Life_Skill.h"
//#include "Msg_Struct.h"
#include "Msg_Active_Struct.h"
#include "Logic_Player.h"
#include "Logic_Public.h"
#include "Configurator.h"
#include "DB_Operator.h"
#include "DB_Manager.h"
#include "Config_Cache.h"
#include <queue>
#include "Logic_Player_Struct.h"
#include "Err_Code.h"
#include "Config_Cache_Scene.h"
#include "Msg_Life_Skill_Struct.h"
#include "Config_Cache_LFSkill.h"
#include "Config_Cache_Item.h"
#include "Utility_Func.h"
#include "Msg_Inner_Enum.h"

Life_Skill::Life_Skill(void) {
	reset();
}

Life_Skill::~Life_Skill() {
}

void Life_Skill::reset(void) {

	life_skill_detail_ = 0;
}

int Life_Skill::create_init(Life_Skill_Detail &detail) {

	detail.energy = MAX_ENERGY;
	detail.inil = 1;
	detail.remain_time = ENERGY_ON_TIME;
	detail.is_change = true;
	return 0;
}

void Life_Skill::load_detail(Life_Skill_Detail &detail) {

	life_skill_detail_ = &detail;
}

void Life_Skill::save_detail(Life_Skill_Detail &detail) {
	if (life_skill_detail_->is_change) {
		detail = *life_skill_detail_;
		life_skill_detail_->is_change = false;
	}
}

void Life_Skill::module_init(void) {

	return;
}

void Life_Skill::sign_in(void) {

	if ( !life_skill_detail_->inil ) {
		life_skill_detail_->energy = MAX_ENERGY;
		life_skill_detail_->inil = 1;
		life_skill_detail_->remain_time = ENERGY_ON_TIME;
		life_skill_detail_->is_change = true;
		return;
	} else {
		if ( life_skill_detail_->energy < MAX_ENERGY )
			start_energy_timer(Logic_Enum::EVENT_LOGIC_ENERGY_TICK,life_skill_detail_->remain_time);
	}

}

void Life_Skill::sign_out(void) {

	Time_Value now = Time_Value::gettimeofday();
	if ( now > last_time ) {
		if ( now.sec() < last_time.sec() + ENERGY_ON_TIME ) {
			if ( life_skill_detail_->remain_time < ENERGY_ON_TIME && now.sec() - last_time.sec() < life_skill_detail_->remain_time) {
				life_skill_detail_->remain_time = life_skill_detail_->remain_time - (now.sec() - last_time.sec());
			}else
			    life_skill_detail_->remain_time = last_time.sec() + ENERGY_ON_TIME - now.sec();
		}else
			life_skill_detail_->remain_time = ENERGY_ON_TIME;
	}else
		life_skill_detail_->remain_time = ENERGY_ON_TIME;
}

int Life_Skill::move_scene_completed(void) {

	return 0;
}

int Life_Skill::client_ready_now(void) {

	return 0;
}

int Life_Skill::trigger_daily_six(const Time_Value& now, bool logic_check /*= false*/) {

	int tmp_energy = ENERGY_SIX_ADD;
	if ( logic_check ) {
		int spand_time = now.sec() - player_self()->base_detail().last_trigger_daily_six.sec();
		if(spand_time >= Time_Value::ONE_DAY_IN_SECS*2){
			tmp_energy = ENERGY_SIX_ADD*2;
		}
	}
	if ( life_skill_detail_->energy < MAX_ENERGY ) {

		add_energy(tmp_energy,logic_check);
	}

	return 0;
}

Int_Int * Life_Skill::skill_unit(uint8_t type) {

	Life_Skill_Map::iterator iter = life_skill_detail_->skill_map.find(type);
	if ( iter == life_skill_detail_->skill_map.end() )
		return NULL;
	return &iter->second;
}

void Life_Skill::add_energy(const uint16_t energy,bool logic_check) {

	if ( life_skill_detail_->energy >= MAX_ENERGY )
		return;
	uint8_t old_energy = life_skill_detail_->energy;
	life_skill_detail_->energy += energy;
	if (life_skill_detail_->energy >= MAX_ENERGY ) {
		life_skill_detail_->energy = MAX_ENERGY;
		life_skill_detail_->remain_time = ENERGY_ON_TIME;
	}
	if ( old_energy != life_skill_detail_->energy ) {
		life_skill_detail_->is_change = true;
		if ( !logic_check ) {
			// 在登录时检测到6点刷新，这个时候不需要发主动消息
			// 发送主动消息
			send_energy_change();
			syns_energy_to_sence();
		}
	}
}

int Life_Skill::try_sub_energy(const uint16_t energy) {
	if ( energy > 0) {
		if (life_skill_detail_->energy < energy )
			return ERROR_ENERGY_NOT_ENOUGHT;
	}
	return 0;
}

int Life_Skill::sub_energy(const uint16_t energy,const bool from_scene /*= false*/,const bool check) {

	if (!check ) {
		int result = try_sub_energy(energy);
		if ( 0 != result )
			return result;
	}
	uint8_t old_energy = life_skill_detail_->energy;

	life_skill_detail_->energy = life_skill_detail_->energy - energy;
	life_skill_detail_->is_change = true;
	send_energy_change();
    if ( !from_scene )
    	syns_energy_to_sence();
	if ( old_energy >= MAX_ENERGY ) {
		// 这种情况(精力本来就是最大值得情况)需要先删掉定时器   重新定时,
		//如果之前已经小于最大值，那之前肯定是有定时，所以不需要重新定时
		this->remove_event(Logic_Enum::EVENT_LOGIC_ENERGY_TICK);
		this->remove_event(Logic_Enum::EVENT_LOGIC_ENERGY_COMFIRE_TICK);
		{
			start_energy_timer(Logic_Enum::EVENT_LOGIC_ENERGY_TICK,ENERGY_ON_TIME);

		}
		{
			const Scene_Config * s_scene = CONFIG_CACHE_SCENE->scene_config_cache(
						player_self()->location().scene_id);
			if ( life_skill_detail_->energy < MAX_ENERGY && Campfire_Scene_Type == s_scene->type)
				start_energy_timer(Logic_Enum::EVENT_LOGIC_ENERGY_COMFIRE_TICK,ENERGY_IN_BONFIRE_TIME);
		}
	}
	return 0;
}

void Life_Skill::start_energy_timer(int type,const int time_inter) {

	if ( life_skill_detail_->energy < MAX_ENERGY ) {
		if ( type == Logic_Enum::EVENT_LOGIC_ENERGY_TICK ) {
			this->create_event(Logic_Enum::EVENT_LOGIC_ENERGY_TICK,&Logic_Player::energy_time_up,Time_Value(time_inter),1);
			last_time = Time_Value::gettimeofday();
		}else
			this->create_event(Logic_Enum::EVENT_LOGIC_ENERGY_COMFIRE_TICK,&Logic_Player::energy_fire_time_up,Time_Value(time_inter),1);
	}
}

void Life_Skill::energy_time_up(void) {

	life_skill_detail_->remain_time = ENERGY_ON_TIME;
	life_skill_detail_->is_change = true;
	if ( life_skill_detail_->energy < MAX_ENERGY )
		add_energy(ENERGY_ON_ADD);
	if ( life_skill_detail_->energy < MAX_ENERGY )
		start_energy_timer(Logic_Enum::EVENT_LOGIC_ENERGY_TICK,ENERGY_ON_TIME);
}

void Life_Skill::energy_fire_time_up(void) {
	// 篝火定时器到后需要判断是否玩家还处在篝火场景
	const Scene_Config * s_scene = CONFIG_CACHE_SCENE->scene_config_cache(
				player_self()->location().scene_id);
	if ( life_skill_detail_->energy < MAX_ENERGY && Campfire_Scene_Type == s_scene->type) {
		add_energy(ENERGY_BONFIRE_ADD);
		if ( life_skill_detail_->energy < MAX_ENERGY )
			start_energy_timer(Logic_Enum::EVENT_LOGIC_ENERGY_COMFIRE_TICK,ENERGY_IN_BONFIRE_TIME);
	}
}

void Life_Skill::handle_energy_fire(void) {

	if ( life_skill_detail_->energy < MAX_ENERGY ) {
		start_energy_timer(Logic_Enum::EVENT_LOGIC_ENERGY_COMFIRE_TICK,ENERGY_IN_BONFIRE_TIME);
	}
}

void Life_Skill::send_energy_change(void) {

	MSG_81000101 msg;
	msg.type = 0;
	msg.property.push_back(Property(PT_ENERGY,life_skill_detail_->energy));
	THIS_SEND_TO_CLIENT(msg);
}

void Life_Skill::handle_energy_from_sence(const uint16_t energy) {

	// 主要是为了处理精力增加定时器
	if ( life_skill_detail_->energy == energy )
		return ;
	if ( energy > life_skill_detail_->energy )
		add_energy(energy - life_skill_detail_->energy);
	else {
		sub_energy(life_skill_detail_->energy - energy,true);
	}
}

void Life_Skill::syns_energy_to_sence() {

	Block_Buffer inner_buf;
	inner_buf.make_message(SYSN_ENERGY_TO_LOGIC);
	inner_buf.write_uint16(life_skill_detail_->energy);
	inner_buf.finish_message();
	this->send_to_scene(inner_buf);
}

void Life_Skill::syns_skill_to_scene() {
	Block_Buffer inner_buf;
	inner_buf.make_message(SYNS_LIFE_SKILL_TO_SCENE);
	Int_Int *unit = skill_unit(Life_Skill_Collect);
	if ( NULL == unit ) {
		return;
	}else {
		inner_buf.write_int32(unit->val_1);
	}
	inner_buf.finish_message();
	this->send_to_scene(inner_buf);
}

int Life_Skill::sync_energy_info(Block_Buffer &buf) {
	buf.write_uint16(life_skill_detail_->energy);
	Int_Int *unit = skill_unit(Life_Skill_Collect);
	if ( NULL == unit ) {
		// 这个时候还没学基础技能
		buf.write_int32(0);
	}else {
		buf.write_int32(unit->val_1);
	}
	return 0;
}

int Life_Skill::collect_skill_up(Int_Int * unit,const LFSkill_Base * base) {

	if ( unit && base ) {
		int result = 0;

		Money_DM_Info dm_info(Pack::MONEY_SUB_LIFE_SKILL_UP); //

		Money_Sub_Info info(Pack::COPPER_ONLY,base->collect_unit.cast_gold_once,dm_info);
		result = player_self()->pack_try_sub_money(info);
		if (0 != result )
			return result;
		player_self()->pack_sub_money(info,Pack::MONEY_OPT_REAL);
		unit->val_2 += base->collect_unit.skilled_once;
		if (unit->val_2 >= base->collect_unit.skilled_max ) {
			unit->val_1 = base->next_id;
			const LFSkill_Base *tmp_base = CONFIG_CACHE_LFSKILL->lfskill_base_config(unit->val_1);
			if ( tmp_base && tmp_base->next_id == 0)
				unit->val_2 = 0;
			else
			    unit->val_2 = unit->val_2 - base->collect_unit.skilled_max;
		}
		life_skill_detail_->is_change = true;
	}
	return 0;
}

int Life_Skill::card_skill_up(Int_Int * unit,const LFSkill_Base * base) {
	if ( unit && base ) {
		int result = 0;

		Money_DM_Info dm_info(Pack::MONEY_SUB_LIFE_SKILL_UP);

	    Money_Sub_Info info(Pack::COPPER_ONLY,base->card_unit.cast_gold_once,dm_info);
	    result = player_self()->pack_try_sub_money(info);
	    if (0 != result )
	    	return result;
	    player_self()->pack_sub_money(info,Pack::MONEY_OPT_REAL);
	    unit->val_2 += base->card_unit.skilled_once;
	    if (unit->val_2 >= base->card_unit.skilled_max ) {

	    	unit->val_1 = base->next_id;
	    	const LFSkill_Base *tmp_base = CONFIG_CACHE_LFSKILL->lfskill_base_config(unit->val_1);
	    	if ( tmp_base && tmp_base->next_id == 0)
	    		unit->val_2 = 0;
	    	else
	    		unit->val_2 = unit->val_2 - base->card_unit.skilled_max;
	    }
	    life_skill_detail_->is_change = true;
	}
	return 0;
}

int Life_Skill::smelt_skill_up(Int_Int * unit,const LFSkill_Base * base) {

	if ( unit && base ) {
		int result = 0;
		Money_DM_Info dm_info(Pack::MONEY_SUB_LIFE_SKILL_UP);

		Money_Sub_Info info(Pack::COPPER_ONLY,base->smelt_unit.cast_gold_once,dm_info);
	    result = player_self()->pack_try_sub_money(info);
		if (0 != result )
		    return result;
		player_self()->pack_sub_money(info,Pack::MONEY_OPT_REAL);
		unit->val_2 += base->smelt_unit.skilled_once;
		if (unit->val_2 >= base->smelt_unit.skilled_max ) {

		    unit->val_1 = base->next_id;
		    const LFSkill_Base *tmp_base = CONFIG_CACHE_LFSKILL->lfskill_base_config(unit->val_1);
		    if ( tmp_base && tmp_base->next_id == 0)
		    	unit->val_2 = 0;
		    else
		    	unit->val_2 = unit->val_2 - base->smelt_unit.skilled_max;
		}
		life_skill_detail_->is_change = true;
	}
	return 0;
}

int Life_Skill::up_item_id(const Id_Amount_Vec &vec,uint32_t &item_id) {
	uint8_t size = vec.size();
	if (size ==0 || size >3 )
		return ERROR_CLIENT_PARAM;
	if (size == 1) {
		item_id = vec[0].id;
	} else if ( size == 2) {
		if (Utility_Func::lottery_success(0.67)) {
			if ( vec[0].amount == 2 )
				item_id = vec[0].id;
			else
				item_id = vec[1].id;
		} else {
			if ( vec[0].amount == 1 )
				item_id = vec[0].id;
			else
				item_id = vec[1].id;
		}
	} else {
		int index = Utility_Func::get_rand_min_max(1,3);
		if ( index >0 && index <= size )
			item_id = vec[index -1].id;
	}
	return 0;
}

int Life_Skill::refine_success(const int color,bool &success) {

	Int_Int * unit = skill_unit(Life_Skill_Collect);
	if ( unit ) {
		if ( unit->val_1%1000/100 + 1 > color )
			success = true;
		else {
			const LFSkill_Base *base_unit = CONFIG_CACHE_LFSKILL->lfskill_base_config(unit->val_1);
			if ( NULL == base_unit )
				return ERROR_CONFIG_NOT_EXIST;
			double pro = base_unit->collect_unit.base_success + base_unit->collect_unit.refine_sucess;
			if ( pro >= 1.00000 )
				success = true;
			else {
				if (Utility_Func::lottery_success(pro) )
					success = true;
				else
					success = false;
			}
		}
	}
	return 0;
}

int Life_Skill::card_trigger_change(Int_Int * unit,const uint8_t type,bool &success) {

	double pro = 0.0;
	if ( unit ) {
		const LFSkill_Base *self_cof = CONFIG_CACHE_LFSKILL->lfskill_base_config(unit->val_1);
		if ( NULL == self_cof )
			return ERROR_CONFIG_NOT_EXIST;
		if ( unit->val_1%1000/100 > type ) {
			const LFSkill_Base *base_cof = CONFIG_CACHE_LFSKILL->lfskill_base_config(Life_Skill_Make*1000 +type*100 +1);
			if ( NULL == base_cof )
				return ERROR_CONFIG_NOT_EXIST;
			const Life_Skill_Comm &com_config = CONFIG_CACHE_LFSKILL->common_config();
			if ( com_config.card_probability.size() >= uint8_t(unit->val_1%1000/100 -type) )
				pro = base_cof->card_unit.base_success + com_config.card_probability[unit->val_1%1000/100 -type -1];
			else
				return ERROR_CONFIG_NOT_EXIST;
		}else {
			pro = self_cof->card_unit.base_success + self_cof->card_unit.refine_sucess;
		}
		if ( pro >= 1.00000)
			success = true;
		else {
			if (Utility_Func::lottery_success(pro) )
				success = true;
			else
			    success = false;
		}
	}
	return 0;
}

int Life_Skill::get_card_make_mer(const LFSkill_Base * base,Id_Amount_Vec &vec) {

	if ( base) {
		vec.clear();
		for (std::vector<Int_Int>::const_iterator iter = base->card_unit.meterial_vec.begin();iter !=base->card_unit.meterial_vec.end();++iter ) {
			vec.push_back(Id_Amount(iter->val_1,iter->val_2));
		}
	}
	return 0;
}

int Life_Skill::get_card_id(const bool success,const LFSkill_Base * base,uint32_t &item_id) {

	if ( base ) {
		std::vector<int32_t> weight;
		if ( success ) {
			if ( base->card_unit.change_vec.size() == 0)
				return ERROR_CONFIG_NOT_EXIST;
			for ( std::vector<Int_Int>::const_iterator iter = base->card_unit.change_vec.begin();iter != base->card_unit.change_vec.end();++iter )
				weight.push_back(iter->val_2);
			if (weight.size() > 0) {
				int index = Utility_Func::get_rand_index(weight);
				if ( index != -1 && index < int(weight.size()) )
					item_id = base->card_unit.change_vec[index].val_1;
			}
		} else {
			for ( std::vector<Int_Int>::const_iterator iter = base->card_unit.card_vec.begin();iter != base->card_unit.card_vec.end();++iter )
				weight.push_back(iter->val_2);
			if (weight.size() > 0) {
				int index = Utility_Func::get_rand_index(weight);
				if ( index != -1 && index < int(weight.size()) )
					item_id = base->card_unit.card_vec[index].val_1;
			}
		}
	}
	return 0;
}

int Life_Skill::smelt_add(const Int_Int * unit,const int tab_id,const int num,int &add) {

	if ( unit ) {
		double pro = 0.0;
		const Smelt_Tab *tab_config = CONFIG_CACHE_LFSKILL->smelt_tab_config(tab_id);
		if ( NULL == tab_config )
			return ERROR_CLIENT_PARAM;
		const Life_Skill_Comm &com_config = CONFIG_CACHE_LFSKILL->common_config();
		if ( unit->val_1 %1000/100 > tab_config->skill_id%1000/100 ) {
			// 垮阶
			if ( unit->val_1 %1000/100 - tab_config->skill_id%1000/100 > int(com_config.smelt_probability.size()))
				return ERROR_CONFIG_NOT_EXIST;
			else
				pro = tab_config->base_success + com_config.smelt_probability[unit->val_1 %1000/100 - tab_config->skill_id%1000/100 -1];

		}else if (unit->val_1 %100 > tab_config->skill_id %100 ) {
			// 垮级
			if ( unit->val_1 %100 - tab_config->skill_id%100 > int(com_config.smelt_lv_probability.size()))
				return ERROR_CONFIG_NOT_EXIST;
			else
				pro = tab_config->base_success + com_config.smelt_probability[unit->val_1 %100 - tab_config->skill_id%100 -1];
		}else {
			pro = tab_config->base_success;
		}
		for ( uint8_t i = 0;i < num;++i ) {
			if ( pro >= 1.00000)
				add++;
			else {
				if ( Utility_Func::lottery_success(pro) )
					add++;
			}
		}
	}
	return 0;
}

int Life_Skill::get_smelt_mer(const Smelt_Tab *tab_cof,Id_Amount_Vec &vec,const int num) {

	if ( tab_cof) {
		vec.clear();
		for (std::vector<Int_Int>::const_iterator iter = tab_cof->meterial_vec.begin();iter !=tab_cof->meterial_vec.end();++iter ) {
			vec.push_back(Id_Amount(iter->val_1,iter->val_2*num));
		}
	}
	return 0;
}

//msg handle
int Life_Skill::open_life_skill_panel(void) {

	if ( player_self()->level() < CONFIG_CACHE_LFSKILL->common_config().level)
	    return ERROR_ROLE_LV_LIM;
	MSG_50116000 msg;
	for (uint8_t i = 1;i<=3;++i) {
		Int_Int life_skill;
		Life_Skill_Map::iterator iter = life_skill_detail_->skill_map.find(i);
		if (iter == life_skill_detail_->skill_map.end() ) {
			life_skill.val_1 = 0;
			life_skill.val_2 = 0;
		} else {
			life_skill.val_1 = iter->second.val_1;
			life_skill.val_2 = iter->second.val_2;
		}
		msg.life_skill_vec.push_back(life_skill);
	}
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Life_Skill::learn_life_skill(const uint8_t type) {
    if ( player_self()->level() < CONFIG_CACHE_LFSKILL->common_config().level)
    	return ERROR_ROLE_LV_LIM;
	if ( type == 0 || type > Life_Skill_SMelt )
		return ERROR_CLIENT_PARAM;
	Life_Skill_Map::iterator tmp_iter = life_skill_detail_->skill_map.find(type);
	if ( tmp_iter != life_skill_detail_->skill_map.end() )
		return ERROR_CLIENT_OPERATE;
	if ( Life_Skill_Make == type || Life_Skill_SMelt == type ) {
		Life_Skill_Map::iterator iter = life_skill_detail_->skill_map.find(Life_Skill_Collect);
		if ( iter == life_skill_detail_->skill_map.end() )
			return ERROR_NEED_LEARN_BASE_SKILL;
	}

	// add money cost
	const LFSkill_Base *base_unit = CONFIG_CACHE_LFSKILL->lfskill_base_config(type * 1000);
	if (NULL == base_unit )
		return ERROR_CONFIG_NOT_EXIST;
	Money_DM_Info dm_info(Pack::MONEY_SUB_LEARN_LIFE_SKILL); //
	Money_Sub_Info info(Pack::COPPER_ONLY,base_unit->skill_cost,dm_info);
	int result = player_self()->pack_sub_money(info,Pack::MONEY_OPT_REAL);
	if ( 0 != result )
		return result;
	Int_Int skill_unit;
	skill_unit.val_1 = type * 1000 +1;
	skill_unit.val_2 = 0;
	life_skill_detail_->skill_map.insert(std::make_pair(type,skill_unit));
	life_skill_detail_->is_change = true;
	MSG_50116001 msg;
	msg.life_skill = skill_unit;
	THIS_SEND_TO_CLIENT(msg);
	// 同步场景
	if ( Life_Skill_Collect == type ) {
		syns_skill_to_scene();
	}

	return 0;
}

int Life_Skill::life_skill_upgrade(const int skill_id) {

	uint8_t type = skill_id/1000;
	Life_Skill_Map::iterator iter = life_skill_detail_->skill_map.find(type);
	if ( iter == life_skill_detail_->skill_map.end() || iter->second.val_1 != skill_id )
		return ERROR_CLIENT_PARAM;
	const LFSkill_Base *base_unit = CONFIG_CACHE_LFSKILL->lfskill_base_config(skill_id);
	if ( NULL == base_unit )
		return ERROR_CONFIG_NOT_EXIST;
	if ( base_unit->next_id == 0 )
		return 0;
	const LFSkill_Base *next_base_unit = CONFIG_CACHE_LFSKILL->lfskill_base_config(base_unit->next_id);
	if ( NULL == next_base_unit || base_unit->skill_id/1000 != base_unit->next_id/1000 || base_unit->skill_id >= base_unit->next_id)
		return ERROR_CONFIG_NOT_EXIST;

	Int_Int *s_unit = skill_unit(type);
	if ( NULL == s_unit )
		return ERROR_CLIENT_OPERATE;
    int old_id = s_unit->val_1;
    int result = 0;
    switch(type) {
    case Life_Skill_Collect:
    	result = collect_skill_up(s_unit,base_unit);
    	if ( 0 != result)
    		return result;
    	break;
    case Life_Skill_Make:
    	result = card_skill_up(s_unit,base_unit);
    	if ( 0 != result)
    	    return result;
    	break;
    case Life_Skill_SMelt:
    	result = smelt_skill_up(s_unit,base_unit);
    	if ( 0 != result)
    	    return result;
    	break;
    default:
    	break;
    }
    MSG_50116002 msg;
    msg.life_skill = *s_unit;
    if ( s_unit->val_1 > old_id ) {
    	// 同步场景
    	if ( Life_Skill_Collect == type ) {
    		syns_skill_to_scene();
    	}
    	if ( s_unit->val_1%1000/100 > old_id%1000/100 )
    		msg.type = 3;
    	else
    		msg.type = 2;
    }else {
    	msg.type = 1;
    }
    THIS_SEND_TO_CLIENT(msg);
    return 0;
}

int Life_Skill::spar_refine(std::vector<uint32_t> &id_vec) {
	if ( id_vec.size() != 3 )
		return ERROR_CLIENT_PARAM;
	boost::unordered_map<uint32_t,Id_Amount> id_map;
	for ( uint8_t i = 0;i<id_vec.size();++i) {
		boost::unordered_map<uint32_t,Id_Amount>::iterator iter = id_map.find(id_vec[i]);
		if ( iter == id_map.end() ) {
			id_map.insert(std::make_pair(id_vec[i],Id_Amount(id_vec[i],1)));
		}else
			iter->second.amount++;
	}
	Id_Amount_Vec id_amount_vec;

	int color = -1;

    for (boost::unordered_map<uint32_t,Id_Amount>::iterator iter1 = id_map.begin();iter1 != id_map.end();++iter1) {
    	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(iter1->first);
    	if ( item_config ) {
    		if ( -1 == color )
    			color = item_config->color;
    		else {
    			if (color != item_config->color)
    				return ERROR_REFINE_NEED_SAME_QUALITY;
    		}
    	}
    	id_amount_vec.push_back(iter1->second);
    }
    Int_Int * unit = skill_unit(Life_Skill_Collect);
    if ( NULL == unit )
    	return ERROR_CLIENT_OPERATE;
    if ( (unit->val_1 %1000/100) + 1 < color )
    	return ERROR_LIFE_SKILL_LEVEL_NOT_ENOUGHT;
    bool success = false;
    int result = refine_success(color,success);
    if ( 0 != result )
    	return result;
    const LFSkill_Base *base_unit = CONFIG_CACHE_LFSKILL->lfskill_base_config(Life_Skill_Collect*1000 +(color-1)*100 +1);
    if ( NULL == base_unit )
    	return ERROR_CONFIG_NOT_EXIST;

    Money_DM_Info dm_info(Pack::MONEY_SUB_SPAR_REFINE); //
    Money_Sub_Info info(Pack::COPPER_ONLY,base_unit->collect_unit.refine_cost,dm_info);
    result = player_self()->pack_try_sub_money(info);
    if ( !result ) {
    	result = player_self()->pack_try_erase_item(Pack::PACK_T_PACKAGE_INDEX,id_amount_vec);
    }
    if (0 != result )
    	return result;

    uint32_t tmp_id = 0;
    up_item_id(id_amount_vec,tmp_id);
    const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(tmp_id);
    if ( NULL == item_config )
        return ERROR_CONFIG_NOT_EXIST;

    if(item_config->next_spar_id == 0)
    	return 0;

    const Item_Detail_Config *next_item_config = CONFIG_CACHE_ITEM->find_item(item_config->next_spar_id);
    if ( NULL == next_item_config)
    	return ERROR_CONFIG_NOT_EXIST;
    Item_Detail item(item_config->next_spar_id,1,next_item_config->bind);
    result = player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX,item);
    if ( ERROR_PACK_FULL != result && 0 != result )
    	return result;

    {
    	// 消耗
    	player_self()->pack_sub_money(info,Pack::MONEY_OPT_REAL);
    	Lose_Item_DM_Info dm_info(Pack::ITEM_LOSE_SPAR_REFINE);//
    	player_self()->pack_erase_item(Pack::PACK_T_PACKAGE_INDEX,id_amount_vec,dm_info);
    }

    MSG_50116003 msg;
    {
    	if ( !success ) {
    	    msg.item_id = 0;
    	    THIS_SEND_TO_CLIENT(msg);
    	    return ERROR_REFINE_FAILURE;
    	}
    	// 精练获得

    	Gain_Item_DM_Info item_info(Pack::ITEM_GAIN_SPAR_REFINE);
    	result = player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,item,item_info);
    	Item_Vec item_vec;
    	Money_Add_List add_money;
    	item_vec.push_back(item);
    	MSG_81000102 res_msg;
    	player_self()->client_get_item_effect(item_vec,add_money,res_msg);
    }

    msg.item_id = next_item_config->item_id;
    THIS_SEND_TO_CLIENT(msg);
    return 0;
}

int Life_Skill::card_make(const uint8_t type) {

	Id_Amount_Vec card_mer_cost;
	Int_Int * unit = skill_unit(Life_Skill_Make);
	if ( NULL == unit )
		return ERROR_CLIENT_OPERATE;
	if (type > unit->val_1%1000/100 )
		return ERROR_LIFE_SKILL_LEVEL_NOT_ENOUGHT;
	const LFSkill_Base *base_unit = CONFIG_CACHE_LFSKILL->lfskill_base_config(Life_Skill_Make*1000 +type*100 +1);
	if ( NULL == base_unit )
		return ERROR_CONFIG_NOT_EXIST;

	Money_DM_Info dm_info(Pack::MONEY_SUB_CARD_MAKE); //
	Money_Sub_Info info(Pack::COPPER_ONLY,base_unit->card_unit.make_cost,dm_info);
	int result = player_self()->pack_try_sub_money(info);
	if ( !result ) {
		get_card_make_mer(base_unit,card_mer_cost);
		if ( !card_mer_cost.empty())
			result = player_self()->pack_try_erase_item(Pack::PACK_T_PACKAGE_INDEX,card_mer_cost);
		if ( !result )
			result = try_sub_energy(base_unit->card_unit.energy_cost);
	}
	if ( 0 != result )
		return result;
	bool success = false;
	uint32_t item_id = 0;
	result = card_trigger_change(unit,type,success);
	if ( !result ) {
		result = get_card_id(success,base_unit,item_id);
	}
	if ( 0 != result )
		return result;

	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(item_id);
	if ( NULL == item_config)
		return ERROR_CONFIG_NOT_EXIST;
	Item_Detail item(item_id,1,item_config->bind);
	result = player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX,item);
	if ( ERROR_PACK_FULL != result && 0 != result )
		return result;

	{
		// 消耗
		sub_energy(base_unit->card_unit.energy_cost,false,true);

		player_self()->pack_sub_money(info,Pack::MONEY_OPT_REAL);

		Lose_Item_DM_Info dm_info(Pack::ITEM_LOSE_CARD_MAKE);//
		player_self()->pack_erase_item(Pack::PACK_T_PACKAGE_INDEX,card_mer_cost,dm_info);
	}

	MSG_50116004 msg;
	{
		// 卡牌制作获得
		if ( 0 == item_id  ) {
			msg.type = 2;
			THIS_SEND_TO_CLIENT(msg);
			return ERROR_CARD_MAKE_FAILURE;
		}
		Gain_Item_DM_Info item_info(Pack::ITEM_GAIN_CARD_MAKE);
		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,item,item_info);
		Item_Vec item_vec;
		Money_Add_List add_money;
		item_vec.push_back(item);
		MSG_81000102 res_msg;
		player_self()->client_get_item_effect(item_vec,add_money,res_msg);
		// 广播
		if ( success ) {
			Chat_Item_Info item_info;
			item_info.reset();
			item.set_item_basic_info(item_info.item_base_info);
			item_info.item_base_info.amount = 0;
			player_self()->create_ann_card_make(player_self(),item_info);
		}
	}

	if ( success )
		msg.type = 1;
	else
		msg.type = 0;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}

int Life_Skill::spar_smelt(const int tab_id,const int num) {

	Int_Int * unit = skill_unit(Life_Skill_SMelt);
	if ( NULL == unit || num < 0 )
		return ERROR_CLIENT_OPERATE;
	const Smelt_Tab *tab_config = CONFIG_CACHE_LFSKILL->smelt_tab_config(tab_id);
	if ( NULL == tab_config || tab_config->skill_id /1000 != Life_Skill_SMelt )
		return ERROR_CONFIG_NOT_EXIST;
	if ( unit->val_1 < tab_config->skill_id )
		return ERROR_LIFE_SKILL_LEVEL_NOT_ENOUGHT;

	Id_Amount_Vec smelt_mer_cost;
	int add_num = 0;
	Money_DM_Info dm_info(Pack::MONEY_SUB_SPAR_SMELT); //
	Money_Sub_Info info(Pack::COPPER_ONLY,tab_config->smelt_cost,dm_info);
	int result = player_self()->pack_try_sub_money(info);
	if ( !result ) {
		get_smelt_mer(tab_config,smelt_mer_cost,num);
		if ( !smelt_mer_cost.empty() )
		    result = player_self()->pack_try_erase_item(Pack::PACK_T_PACKAGE_INDEX,smelt_mer_cost);
		if ( !result )
			result = try_sub_energy(tab_config->energy_cost);
	}

	if ( !result ) {
		result = smelt_add(unit,tab_id,num,add_num);
	}

	if ( 0 != result )
		return result;

	const Item_Detail_Config *item_config = CONFIG_CACHE_ITEM->find_item(tab_config->item_id);
	if ( NULL == item_config)
		return ERROR_CONFIG_NOT_EXIST;
	Item_Detail item(tab_config->item_id,num + add_num,item_config->bind);
	result = player_self()->pack_try_insert_item(Pack::PACK_T_PACKAGE_INDEX,item);
	if ( ERROR_PACK_FULL != result && 0 != result )
		return result;

	{
		// 消耗
		sub_energy(tab_config->energy_cost,false,true);
		player_self()->pack_sub_money(info,Pack::MONEY_OPT_REAL);
		Lose_Item_DM_Info dm_info(Pack::ITEM_LOSE_SPAR_SMELT);//
		player_self()->pack_erase_item(Pack::PACK_T_PACKAGE_INDEX,smelt_mer_cost,dm_info);
	}

	{
        // 晶石熔炼获得

		Gain_Item_DM_Info item_info(Pack::ITEM_GAIN_SPAR_SMELT);
		player_self()->pack_insert_item(Pack::PACK_T_PACKAGE_INDEX,item,item_info);
		Item_Vec item_vec;
		Money_Add_List add_money;
		item_vec.push_back(item);
		MSG_81000102 res_msg;
		player_self()->client_get_item_effect(item_vec,add_money,res_msg);
	}
	MSG_50116005 msg;
	if ( add_num > 0 )
		msg.type = 1;
	else
		msg.type = 0;
	THIS_SEND_TO_CLIENT(msg);
	return 0;
}
