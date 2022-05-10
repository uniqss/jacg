#include "json_helper.h"


bool _TrimJsonValueString(simdjson::ondemand::value& root, const char* node_name, std::string& out) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    auto result = field.get_string();

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}

bool _TrimJsonValueInt64(simdjson::ondemand::value& root, const char* node_name, int64_t& out, bool try_from_string) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    auto result = field.get_int64();
    if (result.error() != simdjson::SUCCESS && try_from_string) result = field.get_int64_in_string();

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}

bool _TrimJsonValueInt(simdjson::ondemand::value& root, const char* node_name, int& out, bool try_from_string) {
    int64_t val64 = 0;
    if (!_TrimJsonValueInt64(root, node_name, val64, try_from_string)) return false;
    
    out = (int)val64;
    return true;
}

bool _TrimJsonValueUint(simdjson::ondemand::value& root, const char* node_name, unsigned int& out, bool try_from_string) {
    int64_t val64 = 0;
    if (!_TrimJsonValueInt64(root, node_name, val64, try_from_string)) return false;
    
    out = (unsigned int)val64;
    return true;
}

bool _TrimJsonValueUint64(simdjson::ondemand::value& root, const char* node_name, uint64_t& out, bool try_from_string) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    auto result = field.get_uint64();
    if (result.error() != simdjson::SUCCESS && try_from_string) result = field.get_uint64_in_string();

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}
