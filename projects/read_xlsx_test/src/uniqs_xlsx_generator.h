#pragma once

#include "stl.h"

extern std::string strOutPath;
extern std::string strInputPath;
extern std::string strJsonOutputPathCpp;
extern std::string strJsonOutputPathJs;

class CGenerator {
   public:
    virtual bool GenerateOneSheet(const std::string& sheet_name, const std::string& gen_name, const std::vector<std::vector<std::string> >& rData) = 0;
    virtual bool GenCfgTables(const std::vector<std::string>& rNames) = 0;
    virtual bool GenSpecification(ofstream& ofs) = 0;
    virtual bool GenThisCol(const std::vector<std::string>& head, int nIdx, bool bServer = true) = 0;
};
