#include "json_helper.h"


bool _TrimJsonValueString(simdjson::ondemand::value& root, const char* node_name, std::string& out) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<std::string_view> result = field.get_string();

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}

bool _TrimJsonValueInt(simdjson::ondemand::value& root, const char* node_name, int& out, bool from_string) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<int64_t> result;
    if (field.is_integer()) {
        result = field.get_int64();
    } else if (from_string) {
        result = field.get_int64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = (int)result.value();
    return true;
}

bool _TrimJsonValueUint(simdjson::ondemand::value& root, const char* node_name, unsigned int& out, bool from_string) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<int64_t> result;
    if (field.is_integer()) {
        result = field.get_int64();
    } else if (from_string) {
        result = field.get_int64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = (unsigned int)result.value();
    return true;
}

bool _TrimJsonValueInt64(simdjson::ondemand::value& root, const char* node_name, int64_t& out, bool from_string) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<int64_t> result;
    if (field.is_integer()) {
        result = field.get_int64();
    } else if (from_string) {
        result = field.get_int64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}

bool _TrimJsonValueUint64(simdjson::ondemand::value& root, const char* node_name, uint64_t& out, bool from_string) {
    auto field_result = root.find_field(node_name);
    if (field_result.error() != simdjson::SUCCESS) return false;
    simdjson::ondemand::value field = field_result.value();
    simdjson::simdjson_result<uint64_t> result;
    if (field.is_integer()) {
        result = field.get_uint64();
    } else if (from_string) {
        result = field.get_uint64_in_string();
    } else {
        return false;
    }

    if (result.error() != simdjson::SUCCESS) return false;

    out = result.value();
    return true;
}
