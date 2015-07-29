/*
 * Config_Cache_Item.h
 *
 *  Created on: Oct 15, 2012
 *      Author: "ti"
 *      频繁使用的配置
 */

#ifndef CONFIG_CACHE_ITEM_H_
#define CONFIG_CACHE_ITEM_H_

#include "Cache_Base.h"
#include "item_user/Item_User_Config.h"
#include "equiper/Equip_Config_Struct.h"

class Config_Cache_Item : public Cache_Base {
public:
	const Item_Config &item_config(void) const;
	const Item_Detail_Config *find_item(const int item_id) const;
	const Item_Lottery_Config &item_lottery_config(void) const;
	const Item_Lottery_Detail_Config *find_item_lottery(const int item_id) const;
	const Item_Drug_Cfg &item_drug_config(void) const;
	const Item_Drug_Detail *find_item_drug(const int item_id) const;
	const Item_Card_Detail *find_item_card(const int item_id) const;
	const Item_Resource_Cfg &item_resource_config(void) const;
	const Item_Resource_Detail *find_item_resource(const int item_id) const;
	const Item_Material_Cfg &item_material_config(void) const;
	const Item_Material_Detail *find_item_material(const int item_id) const;
	const Quick_Buy *find_quick_buy(const int item_id) const;
	const Item_Gain_Source_Detail *find_item_gain_source(const int type) const;
	const Item_Little_Fuction_Config *find_item_little_fun(const int item_id) const;

	const int get_gemstone_conversion_cost(int lv);
	const Int_Int *get_gemstone_hole_info(int color);
	const int get_gemstone_umembed_cost(void);
	const int get_gemstone_open_hole_item_id(void);
	const Gemstone_Ext_Attr_Config& get_gemstone_ext_attr(void);
	void get_use_item_list(const Json::Value value,const int key);
	const Gem_Carve_Prop *get_gemstone_carve_propo(int lv) const;
	int get_gemstone_carve_propo_max(void) const;


	void refresh_item_config(void);

	void refresh_config_cache(void);

private:
	void refresh_item_equip(Item_Detail_Config &item_detail, Json::Value &item_equip);
	void refresh_item_special(Item_Detail_Config &item_detail, Json::Value &item_special);
	void refresh_item_rune_stone(Item_Detail_Config &item_detail_cfg, Json::Value &rune_stone);
	void refresh_quick_buy(void);
	void refresh_item_gain_source(void);
	void refresh_item_gemstone(Item_Detail_Config &item_detail, Json::Value &gemstone);

	void refresh_gemstone_base(void);
	void refresh_gemstone_msic(void);
	void refresh_gemstone_conversion(void);
	void refresh_gemstone_ext_attr(void);
	void refresh_gem_carve_prop(void);

private:
	Item_Config item_config_;
	Item_Lottery_Config item_lottery_config_;
	Item_Drug_Cfg item_drug_cfg_;
	Item_Material_Cfg item_material_cfg_;
	Item_Card_Cfg item_card_cfg_;
	Item_Resource_Cfg item_resource_cfg_;
	Quick_Buy_Config quick_buy_cfg_;
	Item_Gain_Source_Cfg item_gain_source_cfg_;
	Item_Little_Fuction_Map item_little_fuc_cfg_;


	Int_Int_Map gemstone_conversion_cfg_;
	Int_Int_Map gemstone_misc_cfg_;
	Gemstone_Hole_Config gemstone_hole_cfg_;
	Gemstone_Ext_Attr_Config gemstone_ext_attr_cfg_;
	Gem_Carve_Prop_Map gem_carve_prop_map_;  //宝石雕刻
	uint32_t carve_max_; //宝石雕刻最大值
};
typedef Singleton<Config_Cache_Item> Config_Cache_Item_Single;
#define CONFIG_CACHE_ITEM    (Config_Cache_Item_Single::instance())

////////////////////////////////////////////////////////////////////////////////

#endif /* CONFIG_CACHE_ITEM_H_ */
