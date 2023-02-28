#include "uniqs_tmpl_generator.h"

#include "uniqs_xlsx_reader.h"

#include "dirhelper.h"
#include "pugixml.hpp"

static bool StrReplace(std::string& str, const std::string& from, const std::string& to) {
    auto start_pos = str.find(from);
    if (start_pos == std::string::npos) return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

static void StrReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

static bool StrContains(const std::string& str, const std::string& target) {
    return str.find(target) != std::string::npos;
}

static void TrimLeftRightSpaces(std::string& ret, const std::string& str) {
    if (str.empty()) {
        ret = str;
        return;
    }

    size_t begin = 0;
    size_t end = str.size() - 1;
    auto isSpace = [](const std::string& s, size_t idx) { return s[idx] == ' ' || s[idx] == '\t'; };
    while (begin < end) {
        if (isSpace(str, begin)) {
            ++begin;
            continue;
        } else if (isSpace(str, end)) {
            --end;
            continue;
        } else {
            break;
        }
    }
    ret = str.substr(begin, end - begin);
}

int CTemplatedGenerator::Generate(const std::vector<string>& vecFileFullNames, const std::vector<string>& vecFileNames,
                                  const std::string& strCfgPath, const std::string& strOutPath) {
    int ret = 0;
    std::vector<std::string> vecXlsxFileNames;

    Rules rules;
    UTemplates templates;
    ret = ReadCfg(rules, templates, strCfgPath);
    if (ret != 0) {
        return ret;
    }

    std::vector<XlsxDataWithName> allHeadData;

    ret = ReadAllHeadData(allHeadData, vecFileFullNames, vecFileNames, 6);
    if (ret != 0) {
        return ret;
    }

    for (const auto& tmpl : templates) {
        const auto& tmplName = tmpl.first;
        if (tmplName.find("{{%=SheetName%}}") != std::string::npos) {
            // foreach sheet
            for (const auto& xlsx : allHeadData) {
                const std::string& xlsxName = xlsx.first;
                for (const auto& xlsxData : xlsx.second) {
                    const std::string sheetName = xlsxData.first;
                    const auto& sheetData = xlsxData.second;
                    std::string genFileName = tmplName;
                    StrReplaceAll(genFileName, "{{%=SheetName%}}", sheetName);
                    ret = TemplatedGenOneSheet(genFileName, strOutPath, allHeadData, rules, tmpl, xlsxName, sheetName,
                                               sheetData);
                    if (ret != 0) return ret;
                }
            }

        } else if (tmplName.find("{{%=XlsxName%}}") != std::string::npos) {
            // foreach xlsx.
            cout << "currently not implemented." << endl;
            return -1;
        } else {
            // only one file
        }
    }

    return 0;
}

int CTemplatedGenerator::ReadAllHeadData(std::vector<XlsxDataWithName>& allHeadData,
                                         const std::vector<std::string>& vecFileFullNames,
                                         const std::vector<std::string>& vecFileNames, int maxRows) {
    for (int nIdx = 0; nIdx < vecFileNames.size(); ++nIdx) {
        auto pos = vecFileNames[nIdx].find(".xlsx");
        if (pos == std::string::npos) continue;
        if (vecFileNames[nIdx].find("$") != std::string::npos) continue;
        string gen_name = vecFileNames[nIdx].substr(0, pos);
        std::string xlsxName = gen_name;

        XlsxData vecxlsx;
        int nRet = read_xlsx(vecFileFullNames[nIdx].c_str(), vecxlsx, maxRows);
        if (nRet != 0) {
            cout << "read_xlsx error." << endl;
            continue;
        }

        allHeadData.emplace_back(std::make_pair(xlsxName, vecxlsx));
    }

    return 0;
}

int CTemplatedGenerator::ReadCfg(Rules& rules, UTemplates& templates, const std::string& strCfgPath) {
    // rules
    std::string pathRules = strCfgPath + "/rules/";
    std::vector<std::string> rulesFiles;
    readFileList(pathRules.c_str(), &rulesFiles);

    rules.clear();

    for (const auto& ruleFile : rulesFiles) {
        auto pos = ruleFile.rfind(".xml");
        if (pos == std::string::npos) continue;
        if (pos != ruleFile.size() - 4) continue;

        std::string rulePath = pathRules + ruleFile;

        Rule rule;
        rule.first = ruleFile.substr(0, ruleFile.size() - 4);

        pugi::xml_document doc;
        auto result = doc.load_file(rulePath.c_str());
        if (!result) {
            return -1;
        }
        auto root = doc.child("root");
        if (root.empty()) {
            return -1;
        }
        std::string fromCol = root.attribute("fromCol").as_string();
        if (fromCol.empty()) {
            return -1;
        }

        for (auto trans : root.children("trans")) {
            std::string from = trans.attribute("from").as_string();
            if (from.empty()) {
                continue;
                ;
            }
            std::string to = trans.attribute("to").as_string();
            rule.second[from] = to;
        }

        rules.emplace_back(rule);
    }

    // templates
    std::string pathTemplates = strCfgPath + "/templates/";
    std::vector<std::string> templateFiles;
    readFileList(pathTemplates.c_str(), &templateFiles);

    templates.clear();
    for (const auto& templateFile : templateFiles) {
        std::string tmplPath = pathTemplates + templateFile;

        std::ifstream ifs(tmplPath.c_str());
        UTemplate tmpl;
        tmpl.first = templateFile;
        std::string str;
        while (std::getline(ifs, str)) {
            tmpl.second.emplace_back(str);
        }
        templates.emplace_back(tmpl);
    }

    return 0;
}

enum EParseState {
    EPS_Invalid,
    EPS_Line,
    EPS_LoopSheet,
    EPS_LoopSheetRow,
    EPS_LoopSheetCol,
    EPS_LoopSheetRowCol,
};

int CTemplatedGenerator::TemplatedGenOneSheet(const std::string& genFileName, const std::string& strOutPath,
                                              const std::vector<XlsxDataWithName>& allHeadData, const Rules& rules,
                                              const UTemplate& tmpl, const std::string& xlsxName,
                                              const std::string& sheetName, const SheetData& sheetData) {
    std::ofstream ofs;
    std::filesystem::path p = strOutPath;
    p.append(genFileName);
    ofs.open(p);
    if (!ofs) {
        return -1;
    }
    EParseState parseState = EPS_LoopSheet;

    const std::string LoopCol = "{{%Loop.col%}}";
    const std::string LoopColEnd = "{{%LoopEnd.col%}}";
    const std::string LoopRow = "{{%Loop.row%}}";
    const std::string LoopRowEnd = "{{%LoopEnd.row%}}";

    std::string lineTrimed;
    for (auto line : tmpl.second) {
        TrimLeftRightSpaces(lineTrimed, line);
        if (StrContains(lineTrimed, LoopCol)) {
            if (parseState != EPS_LoopSheet && parseState != EPS_LoopSheetRow) {
                cout << "Loop.col can only be in Loop.sheet." << endl;
                return -1;
            }
            if (parseState == EPS_LoopSheet)
                parseState = EPS_LoopSheetCol;
            else
                parseState = EPS_LoopSheetRowCol;
            if (lineTrimed == LoopCol) continue;
        } else if (StrContains(lineTrimed, LoopColEnd)) {
            if (parseState != EPS_LoopCol) {
                cout << "LoopEnd not match." << endl;
                return -1;
            }
            if (lineTrimed == LoopColEnd) continue;
        } else if (StrContains(lineTrimed, LoopRow)) {
            if (parseState != EPS_LoopSheet) }

        if (!LoopCol) {
            // directly
            StrReplaceAll(line, "{{%=SheetName%}}", sheetName);
            StrReplaceAll(line, "{{%=XlsxName%}}", xlsxName);
            ofs << line << endl;
        } else {
            // LoopCol
            // for (int i = 1; i <= 9; ++i) {
            //     std::string key = "{{%=col.2%}}";
            //     StrReplaceAll(line, key, sheetData[])
            // }
        }
    }
    ofs.flush();
    ofs.close();
}
