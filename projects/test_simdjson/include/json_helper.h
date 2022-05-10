#pragma once

#include <string>

#include "simdjson.h"


bool _TrimJsonValueString(simdjson::ondemand::value& root, const char* node_name, std::string& out);

bool _TrimJsonValueInt(simdjson::ondemand::value& root, const char* node_name, int& out, bool try_from_string = true);

bool _TrimJsonValueUint(simdjson::ondemand::value& root, const char* node_name, unsigned int& out, bool try_from_string = true);

bool _TrimJsonValueInt64(simdjson::ondemand::value& root, const char* node_name, int64_t& out, bool try_from_string = true);

bool _TrimJsonValueUint64(simdjson::ondemand::value& root, const char* node_name, uint64_t& out, bool try_from_string = true);
