#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

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
};
