#pragma once

#include "stl.h"

#include "strtk_helper.h"
#include "json_helper.h"

template <class T>
inline bool GetArrayFromTable(std::vector<T>& vecData, simdjson::ondemand::value& tValue, const char* column, const char* del = "|") {
    std::string s;
    if (!_TrimJsonValueString(tValue, column, s)) {
        return false;
    }

    return GetArrayFromTable(vecData, s, del);
}

template <class T>
inline bool GetArray2FromTable(std::vector<std::vector<T>>& vecData, simdjson::ondemand::value& tValue, const char* column) {
    std::string s;
    if (!_TrimJsonValueString(tValue, column, s)) {
        return false;
    }
    return GetArray2FromTable(vecData, s);
}

template <class T>
inline bool GetVal2FromTable(T& cfgVal, simdjson::ondemand::value& tValue, const char* column, const char* del = "|") {
    std::vector<decltype(cfgVal.val1)> tmpData;
    if (!GetArrayFromTable(tmpData, tValue, column, del)) return false;

    if (tmpData.size() != 2) return false;
    cfgVal.val1 = tmpData[0];
    cfgVal.val2 = tmpData[1];

    return true;
}

template <class T>
inline bool GetVal2ArrFromTable(std::vector<T>& vecData, simdjson::ondemand::value& tValue, const char* column) {
    std::vector<std::vector<decltype(T::val1)>> tmpData;
    if (!GetArray2FromTable(tmpData, tValue, column)) return false;

    vecData.resize(tmpData.size());
    for(size_t i = 0;i < tmpData.size();++i){
        const auto& cfgVal = tmpData[i];
        if (cfgVal.size() != 2) return false;
        vecData[i].val1 = cfgVal[0];
        vecData[i].val2 = cfgVal[1];
    }

    return true;
}

template <class T>
inline bool GetVal3FromTable(T& cfgVal, simdjson::ondemand::value& tValue, const char* column, const char* del = "|") {
    std::vector<decltype(cfgVal.val1)> tmpData;
    if (!GetArrayFromTable(tmpData, tValue, column, del)) return false;

    if (tmpData.size() != 3) return false;
    cfgVal.val1 = tmpData[0];
    cfgVal.val2 = tmpData[1];
    cfgVal.val3 = tmpData[2];

    return true;
}

template <class T>
inline bool GetVal3ArrFromTable(std::vector<T>& vecData, simdjson::ondemand::value& tValue, const char* column) {
    std::vector<std::vector<decltype(T::val1)>> tmpData;
    if (!GetArray2FromTable(tmpData, tValue, column)) return false;

    vecData.resize(tmpData.size());
    for(size_t i = 0;i < tmpData.size();++i){
        const auto& cfgVal = tmpData[i];
        if (cfgVal.size() != 3) return false;
        vecData[i].val1 = cfgVal[0];
        vecData[i].val2 = cfgVal[1];
        vecData[i].val3 = cfgVal[2];
    }

    return true;
}
