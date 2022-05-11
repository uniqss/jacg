#pragma once

#include <string>
#include <vector>

struct CfgInt2 {
    CfgInt2() : val1(0), val2(0) {}
    int val1;
    int val2;
};

struct CfgString2 {
    std::string val1;
    std::string val2;
};

struct CfgInt3 {
    CfgInt3() : val1(0), val2(0), val3(0) {}
    int val1;
    int val2;
    int val3;
};

struct CfgString3 {
    std::string val1;
    std::string val2;
    std::string val3;
};
