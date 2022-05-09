#pragma once

#include "stl.h"

#include "strtk.hpp"

template <class T>
inline bool GetArrayFromTable(std::vector<T>& vecData, const std::string& s, const char* del = "|") {
    strtk::multiple_char_delimiter_predicate predicate(del);

    typedef strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer_type;
    tokenizer_type tokenizer(s, predicate);

    std::copy(tokenizer.begin(), tokenizer.end(), strtk::range_to_type_back_inserter(vecData));

    return true;
}


template <class T>
inline bool GetArray2FromTable(std::vector<std::vector<T>>& vecData, const std::string& s) {
    std::vector<std::string> vs;
    if (!GetArrayFromTable(vs, s, "+")) {
        return false;
    }
    for (auto it : vs) {
        auto nSize = vecData.size();
        vecData.resize(nSize + 1);
        std::vector<T>& rData = vecData[nSize];
        if (!GetArrayFromTable(rData, it, "|")) {
            return false;
        }
    }

    return true;
}

