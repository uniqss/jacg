#pragma once

#include <stdint.h>

#include "json.hpp"
#include "json/json.h"

#include <string>

bool _TrimJsonValueString(Json::Value& Root, const char* pszNodeName, std::string& strOut);
bool _TrimJsonValueInt(Json::Value& Root, const char* pszNodeName, int& nOut, bool bFromString = false);
bool _TrimJsonValueUint(Json::Value& Root, const char* pszNodeName, unsigned int& nOut, bool bFromString = false);
bool _TrimJsonValueDouble(Json::Value& Root, const char* pszNodeName, double& nOut, bool bFromString = false);
bool _TrimJsonValueDouble(Json::Value& Root, const char* pszNodeName, float& nOut, bool bFromString = false);
bool _TrimJsonValueBool(Json::Value& Root, const char* pszNodeName, bool& nOut);
bool _TrimJsonValueInt64(Json::Value& Root, const char* pszNodeName, int64_t& nOut);
bool _TrimJsonValueUint64(Json::Value& Root, const char* pszNodeName, uint64_t& nOut);
