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
