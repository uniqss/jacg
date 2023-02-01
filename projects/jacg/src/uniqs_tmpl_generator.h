#pragma once

#include "stl.h"

using SheetData = std::vector<std::vector<std::string>>;
using SheetDataWithName = std::pair<std::string, SheetData>;
using XlsxData = std::vector<SheetDataWithName>;
using XlsxDataWithName = std::pair<std::string, XlsxData>;

using Rule = std::pair<std::string, std::unordered_map<std::string, std::string>>;
using Rules = std::vector<Rule>;

using UTemplate = std::pair<std::string, std::vector<std::string>>;
using UTemplates = std::vector<UTemplate>;

class CTemplatedGenerator {
   public:
    int Generate(const std::vector<string>& vecFileFullNames, const std::vector<string>& vecFileNames,
                 const std::string& strCfgPath, const std::string& strOutPath);

   private:
    int ReadAllHeadData(std::vector<XlsxDataWithName>& allHeadData, const std::vector<string>& vecFileFullNames,
                        const std::vector<string>& vecFileNames);
    int ReadCfg(Rules& rules, UTemplates& templates, const std::string& strCfgPath);

    int TemplatedGenOneFile(const std::string& genFileName, const std::string& strOutPath,
                            const std::vector<XlsxDataWithName>& allHeadData, const Rules& rules, const UTemplate& tmpl,
                            const std::string& xlsxName, const std::string& sheetName, const SheetData& sheetData);
};
