#pragma once

// author: uniqs
// uniqs@163.com
// ====================== This file is generated by uniqs, don't change it!!!!!! ====================== 



#include "BaseTypes.h"

class DTtest
{
	public:
	DTtest()
		: int1(0)
	{}
	bool LoadDT(simdjson::ondemand::value& tValue, int& key, std::string& err)
	{
		if (!_TrimJsonValueInt(tValue, "int1", int1)) { err = " DTtest.int1 error."; return false; }
		key = int1;
		if (!_TrimJsonValueString(tValue, "str2server", str2server)) { err = " DTtest.str2server error."; return false; }
		if (!GetArrayFromTable(intarray, tValue, "intarray")) { err = " DTtest.intarray error."; return false; }
		if (!GetArrayFromTable(stringarray, tValue, "stringarray")) { err = " DTtest.stringarray error."; return false; }
		if (!GetVal2FromTable(reward_sg, tValue, "reward_sg")) { err = " DTtest.reward_sg error."; return false; }
		if (!GetVal2FromTable(name_icon, tValue, "name_icon")) { err = " DTtest.name_icon error."; return false; }
		if (!GetVal2ArrFromTable(reward_mt, tValue, "reward_mt")) { err = " DTtest.reward_mt error."; return false; }
		if (!GetVal2ArrFromTable(icon_namr_arr, tValue, "icon_namr_arr")) { err = " DTtest.icon_namr_arr error."; return false; }
		if (!GetVal3FromTable(idtypenum, tValue, "idtypenum")) { err = " DTtest.idtypenum error."; return false; }
		if (!GetVal3FromTable(sss, tValue, "sss")) { err = " DTtest.sss error."; return false; }
		if (!GetVal3ArrFromTable(reward_mt_2, tValue, "reward_mt_2")) { err = " DTtest.reward_mt_2 error."; return false; }
		if (!GetVal3ArrFromTable(str3arr, tValue, "str3arr")) { err = " DTtest.str3arr error."; return false; }
		if (!GetArray2FromTable(intarray2, tValue, "intarray2")) { err = " DTtest.intarray2 error."; return false; }
		if (!GetArray2FromTable(stringarray2, tValue, "stringarray2")) { err = " DTtest.stringarray2 error."; return false; }
		return true;
	}

public:
	int int1;// 这是int1
	std::string str2server;// 我是你二叔
	std::vector<int> intarray;// 我是你三大爷
	std::vector<std::string> stringarray;// 我是你四舅
	CfgInt2 reward_sg;// 单个奖励
	CfgString2 name_icon;// 名称与icon
	std::vector<CfgInt2> reward_mt;// 多个单类型奖励
	std::vector<CfgString2> icon_namr_arr;// icon名字
	CfgInt3 idtypenum;// 复杂奖励单个
	CfgString3 sss;// 字符字符字符
	std::vector<CfgInt3> reward_mt_2;// 复杂奖励多个
	std::vector<CfgString3> str3arr;// str3数组
	std::vector<std::vector<int>> intarray2;// 我是你外公
	std::vector<std::vector<std::string>> stringarray2;// 我是孙悟空
};

extern std::unique_ptr<DTCfgTable<int, DTtest>> g_pCfg_test;// DTtest.json

