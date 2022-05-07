#include "json_helper.h"


bool _TrimJsonValueString(Json::Value& Root, const char* pszNodeName, std::string& strOut) {
    if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isString()) {
        strOut = Root[pszNodeName].asString();
        return true;
    }

    return false;
}

bool _TrimJsonValueInt(Json::Value& Root, const char* pszNodeName, int& nOut, bool bFromString) {
    if (bFromString) {
        std::string strTmp;
        if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
        nOut = atoi(strTmp.c_str());
        return true;
    }

    if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isInt()) {
        nOut = Root[pszNodeName].asInt();
        return true;
    }

    return false;
}

bool _TrimJsonValueUint(Json::Value& Root, const char* pszNodeName, unsigned int& nOut, bool bFromString) {
    if (bFromString) {
        std::string strTmp;
        if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
        nOut = atoi(strTmp.c_str());
        return true;
    }

    if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isInt()) {
        nOut = Root[pszNodeName].asInt();
        return true;
    }

    return false;
}

bool _TrimJsonValueDouble(Json::Value& Root, const char* pszNodeName, double& nOut, bool bFromString) {
    if (bFromString) {
        std::string strTmp;
        if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
        nOut = atof(strTmp.c_str());
        return true;
    }

    if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isDouble()) {
        nOut = Root[pszNodeName].asDouble();
        return true;
    }

    return false;
}

bool _TrimJsonValueDouble(Json::Value& Root, const char* pszNodeName, float& nOut, bool bFromString) {
    if (bFromString) {
        std::string strTmp;
        if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
        nOut = (float)atof(strTmp.c_str());
        return true;
    }

    if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isDouble()) {
        nOut = Root[pszNodeName].asDouble();
        return true;
    }

    return false;
}

bool _TrimJsonValueBool(Json::Value& Root, const char* pszNodeName, bool& nOut) {
    if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isBool()) {
        nOut = Root[pszNodeName].asBool();
        return true;
    }

    return false;
}

bool _TrimJsonValueInt64(Json::Value& Root, const char* pszNodeName, int64_t& nOut) {
    std::string strTmp;
    if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
    nOut = atoll(strTmp.c_str());
    return true;
}

bool _TrimJsonValueUint64(Json::Value& Root, const char* pszNodeName, uint64_t& nOut) {
    std::string strTmp;
    if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
    nOut = atoll(strTmp.c_str());
    return true;
}
