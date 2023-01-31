#pragma once

#include "stl.h"

#include "uniqs_xlsx_generator.h"

class CGeneratorCPP : public CGenerator {
   public:
    virtual bool GenerateOneSheet(const std::string &sheet_name, const std::string &gen_name, const std::vector<std::vector<std::string>> &rData);
    virtual bool GenCfgTables(const std::vector<std::string> &rNames);
    virtual bool GenSpecification(ofstream &ofs);
    virtual bool GenThisCol(const std::vector<std::string> &head, int nIdx, bool bServer = true);

   private:
    bool GenDTHeader(const std::string &gen_name, const std::vector<string> &head, const std::vector<string> &col_name, const std::vector<string> &col_type,
                     const std::vector<string> &comment);

    bool GenDatas(const std::string &gen_name, const std::vector<std::vector<std::string>> &rData);
};
