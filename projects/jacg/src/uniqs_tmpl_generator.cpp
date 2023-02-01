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
    ret = ReadAllHeadData(allHeadData, vecFileFullNames, vecFileNames);
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
                    TemplatedGenOneFile(genFileName, strOutPath, allHeadData, rules, tmpl, xlsxName, sheetName,
                                        sheetData);
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
                                         const std::vector<std::string>& vecFileNames) {
    for (int nIdx = 0; nIdx < vecFileNames.size(); ++nIdx) {
        auto pos = vecFileNames[nIdx].find(".xlsx");
        if (pos == std::string::npos) continue;
        if (vecFileNames[nIdx].find("$") != std::string::npos) continue;
        string gen_name = vecFileNames[nIdx].substr(0, pos);
        std::string xlsxName = gen_name;

        XlsxData vecxlsx;
        int nRet = read_xlsx(vecFileFullNames[nIdx].c_str(), vecxlsx, 6);
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



int CTemplatedGenerator::TemplatedGenOneFile(const std::string& genFileName, const std::string& strOutPath,
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
    bool LoopCol = false;
    for (auto line : tmpl.second) {
        if (StrContains(line, "{{%Loop.col%}}")) {
            if (LoopCol) {
                cout << "LoopCol cannot nest." << endl;
                return -1;
            }
            LoopCol = true;
            continue;
        } else if (StrContains(line, "{{%LoopEnd.col%}}")) {
            if (!LoopCol) {
                cout << "LoopEnd not match." << endl;
                return -1;
            }
            LoopCol = false;
            continue;
        }

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
