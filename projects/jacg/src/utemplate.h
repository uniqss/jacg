#pragma once

#include "stl.h"

union UTemplateBlock {
    std::string simpleLine;
    std::vector<std::string> loopLines;
};

class UTemplate {
   public:
    std::string templateFileName;
};
