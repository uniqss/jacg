#pragma once

#include "stl.h"

enum ELoopType {
    ELT_Invalid,
    ELT_Col,
    ELT_Row,
    ELT_Sheet,
};

struct UTemplateLine {
    std::string line;
    bool newLine = true;
};

struct UTemplateLoopCol {
    std::vector<UTemplateLine> lines;
};

struct UTemplateLoopTree {

};

struct UTemplateLineOrLoopTree {
    UTemplateLine line;
    UTemplateLoopTree loopTree;
};

struct UTemplate {
    std::string templateFileName;
    std::vector<UTemplateLineOrLoopTree> sections;
};
