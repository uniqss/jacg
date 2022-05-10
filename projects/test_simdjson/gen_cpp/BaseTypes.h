#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "simdjson.h"
#include "json_helper.h"
#include "CfgComplexType.h"

template <class K, class V>
class DTCfgTable {
    std::vector<V> values;
    std::unordered_map<K, V*> mvalues;

   public:
    const V* find(K key) {
        auto it = mvalues.find(key);
        if (it != mvalues.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool Load(const char* szFile, std::string& err) {
        values.clear();
        mvalues.clear();

        simdjson::ondemand::parser parser;
        auto result_json = simdjson::padded_string::load(szFile);
        if (result_json.error() != simdjson::SUCCESS) return -1;
        simdjson::ondemand::document doc = parser.iterate(result_json);
        auto result_obj = doc.get_object();
        if (result_obj.error() != simdjson::SUCCESS) return -1;
        auto result_count = result_obj.count_fields();
        if (result_count.error() != simdjson::SUCCESS) return -1;
        size_t count = result_count.value();
        values.resize(count);
        size_t idx = 0;

        for (auto it : result_obj) {
            simdjson::ondemand::value itvalue = it.value();

            K key;
            V& value = values[idx++];
            if (!value.LoadDT(itvalue, key, err)) return false;

            mvalues[key] = &value;
        }

        return true;
    }

    const std::vector<V>& datas() { return values; }
};
