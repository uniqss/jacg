
#include "uniqs_xlsx_generator_cpp.h"

#include "indent.h"

#include "strtk_helper.h"

#define PERROR cout << __FILE__ << __LINE__

void trans_lower(std::string& col) {
    std::transform(col.begin(), col.end(), col.begin(), ::tolower);
}


bool CGeneratorCPP::GenerateOneSheet(const std::string& sheet_name, const std::string& gen_name, const std::vector<std::vector<std::string>>& rData) {
    if (rData.size() < 4) {
        return false;
    }
    const std::vector<string>& dHead = rData[0];
    const std::vector<string>& dColName = rData[1];
    const std::vector<string>& dColType = rData[2];
    const std::vector<string>& dComment = rData[3];
    std::vector<string> comment = dComment;
    for (auto& it : comment) {
        for (auto& c : it)
            if (c == '\r' || c == '\n') c = '\t';
    }

    // DTorder_pet.h
    if (!GenDTHeader(gen_name, dHead, dColName, dColType, comment)) return false;

#if 1
    if (!GenDatas(gen_name, rData)) return false;
#endif

    return true;
}

bool CGeneratorCPP::GenCfgTables(const std::vector<std::string>& rNames) {
    Indent indent;
    string strOutFile = strOutPath + "CfgTables.cpp";
    ofstream ofs(strOutFile.c_str());
    GenSpecification(ofs);
    ofs << indent << "#include \"CfgTables.h\"" << endl;
    ofs << endl;
    for (auto it : rNames) {
        ofs << indent << "#include \"DT" << it << ".h\"" << endl;
    }
    ofs << endl;

    for (auto it : rNames) {
        ofs << indent << "std::unique_ptr<DTCfgTable<int, DT" << it << ">> g_pCfg_" << it << " = nullptr;" << endl;
    }
    ofs << endl;

    for (auto it : rNames) {
        ofs << indent << "bool LoadDTTables_" << it << "(const char* szFile, std::string& err)" << endl;
        ofs << indent << "{" << endl;
        ++indent;
        ofs << indent << "if (!g_pCfg_" << it << ")" << endl;
        ofs << indent << "{" << endl;
        ++indent;
        ofs << indent << "g_pCfg_" << it << " = std::make_unique<DTCfgTable<int, DT" << it << ">>();" << endl;
        --indent;
        ofs << indent << "}" << endl;
        ofs << endl;
        ofs << indent << "if (!g_pCfg_" << it << "->Load(szFile, err)) return false;" << endl;
        ofs << endl;
        ofs << indent << "return true;" << endl;
        --indent;
        ofs << indent << "}" << endl;
        ofs << endl;
    }

    ofs << "bool LoadDTTables(const std::string& prefix, std::string& err)" << endl;
    ofs << "{" << endl;
    ++indent;
    ofs << indent << "std::string file;" << endl << endl;
    for (auto it : rNames) {
        ofs << indent << "file = prefix + \"DT" << it << ".json\";" << endl;
        ofs << indent << "if (!LoadDTTables_" << it << "(file.c_str(), err)) return false;" << endl;
    }
    ofs << endl;
    ofs << indent << "return true;" << endl;
    --indent;
    ofs << "}" << endl;

    return true;
}

bool CGeneratorCPP::GenSpecification(ofstream& ofs) {
    ofs << "// author: uniqs" << endl;
    ofs << "// uniqs@163.com" << endl;
    ofs << "// ====================== This file is generated by uniqs, don't change it!!!!!! ====================== " << endl;
    ofs << endl;
    ofs << endl;
    ofs << endl;
    return true;
}

bool CGeneratorCPP::GenThisCol(const std::vector<std::string>& head, int nIdx, bool bServer) {
    if (head.size() <= nIdx) return true;
    if (head[nIdx].size() >= 1 && head[nIdx][0] == '#') return false;
    if (head[nIdx].empty()) return true;
    string strHead = head[nIdx];
    trans_lower(strHead);
    if (strHead.find("c") != string::npos && !bServer) return true;
    if (strHead.find("s") != string::npos && bServer) return true;

    return false;
}

bool CGeneratorCPP::GenDTHeader(const std::string& gen_name, const std::vector<string>& head, const std::vector<string>& col_name,
                                const std::vector<string>& col_type, const std::vector<string>& comment) {
    Indent indent;
    string strOutFile = strOutPath + "DT" + gen_name + ".h";
    ofstream ofs(strOutFile.c_str());
    ofs << indent << "#pragma once" << endl;
    ofs << endl;
    GenSpecification(ofs);
    ofs << indent << "#include \"BaseTypes.h\"" << endl;
    ofs << endl;
    string cname = "DT";
    cname += gen_name;
    ofs << indent << "class " << cname << endl;
    ofs << indent << "{" << endl;
    ++indent;
    ofs << indent << "public:" << endl;
    ofs << indent << cname << "()" << endl;
    ++indent;
    bool bFirst = true;

    static unordered_set<string> setInit0 = {"int"};

    for (int nIdx = 0; nIdx < col_name.size() && nIdx < col_type.size(); ++nIdx) {
        string ctype = col_type[nIdx];
        trans_lower(ctype);
        if (setInit0.find(ctype) != setInit0.end() && GenThisCol(head, nIdx)) {
            if (bFirst) {
                bFirst = false;
                ofs << indent << ": " << col_name[nIdx] << "(0)" << endl;
            } else
                ofs << indent << ", " << col_name[nIdx] << "(0)" << endl;
        }
    }
    --indent;
    ofs << indent << "{}" << endl;
    ofs << indent << "bool LoadDT(simdjson::ondemand::value& tValue, int& key, std::string& err)" << endl;
    ofs << indent << "{" << endl;
    ++indent;

    static unordered_set<string> setPK = {"int"};

    bFirst = true;
    for (int nIdx = 0; nIdx < col_name.size() && nIdx < col_name.size() && nIdx < col_type.size(); ++nIdx) {
        if (!GenThisCol(head, nIdx)) continue;
        if (col_type[nIdx].empty() || col_name[nIdx].empty()) {
            continue;
        }

        string ctype = col_type[nIdx];
        trans_lower(ctype);

        if (bFirst) {
            bFirst = false;

            if (setPK.find(ctype) != setPK.end()) {
                // ofs << indent << "key = " << col_name[nIdx] << " = tValue[\"" << col_name[nIdx] << "\"].asInt();" << endl;
                ofs << indent << "if (!_TrimJsonValueInt(tValue, \"" << col_name[nIdx] << "\", " << col_name[nIdx] << ")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
                ofs << indent << "key = " << col_name[nIdx] << ";" << endl;
            } else {
                PERROR << " ==========ERROR========== key must be numeric type(";
                std::for_each(setPK.begin(), setPK.end(), [](const std::string& val) { cout << val << " "; });
                cout << "):" << col_type[nIdx] << " col_name:" << col_name[nIdx] << endl;
                return false;
            }
        } else {
            if (ctype == "int") {
                ofs << indent << "if (!_TrimJsonValueInt(tValue, \"" << col_name[nIdx] << "\", " << col_name[nIdx] << ")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else if (ctype == "string") {
                ofs << indent << "if (!_TrimJsonValueString(tValue, \"" << col_name[nIdx] << "\", " << col_name[nIdx] << ")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else if (ctype == "intarr" || ctype == "stringarr") {
                ofs << indent << "if (!GetArrayFromTable(" << col_name[nIdx] << ", tValue, \"" << col_name[nIdx] << "\")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else if (ctype == "intarr2" || ctype == "stringarr2") {
                ofs << indent << "if (!GetArray2FromTable(" << col_name[nIdx] << ", tValue, \"" << col_name[nIdx] << "\")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else if (ctype == "int2" || ctype == "string2") {
                ofs << indent << "if (!GetVal2FromTable(" << col_name[nIdx] << ", tValue, \"" << col_name[nIdx] << "\")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else if (ctype == "int2arr" || ctype == "string2arr") {
                ofs << indent << "if (!GetVal2ArrFromTable(" << col_name[nIdx] << ", tValue, \"" << col_name[nIdx] << "\")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else if (ctype == "int3" || ctype == "string3") {
                ofs << indent << "if (!GetVal3FromTable(" << col_name[nIdx] << ", tValue, \"" << col_name[nIdx] << "\")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else if (ctype == "int3arr" || ctype == "string3arr") {
                ofs << indent << "if (!GetVal3ArrFromTable(" << col_name[nIdx] << ", tValue, \"" << col_name[nIdx] << "\")) { err = \" " << cname << "."
                    << col_name[nIdx] << " error.\"; return false; }" << endl;
            } else {
                PERROR << " ==========ERROR========== type not supported:" << ctype << endl;
                return false;
            }
        }
    }
    ofs << indent << "return true;" << endl;
    --indent;
    ofs << indent << "}" << endl;
    ofs << endl;

    --indent;
    ofs << "public:" << endl;
    ++indent;
    for (int nIdx = 0; nIdx < col_name.size() && nIdx < col_name.size() && nIdx < col_type.size(); ++nIdx) {
        if (!GenThisCol(head, nIdx)) continue;
        if (col_type[nIdx].empty() || col_name[nIdx].empty()) {
            continue;
        }

        string ctype = col_type[nIdx];
        trans_lower(ctype);

        if (ctype == "int") {
            ofs << indent << "int " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "string") {
            ofs << indent << "std::string " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "intarr") {
            ofs << indent << "std::vector<int> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "stringarr") {
            ofs << indent << "std::vector<std::string> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "int2") {
            ofs << indent << "CfgInt2 " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "string2") {
            ofs << indent << "CfgString2 " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "int2arr") {
            ofs << indent << "std::vector<CfgInt2> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "string2arr") {
            ofs << indent << "std::vector<CfgString2> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "int3") {
            ofs << indent << "CfgInt3 " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "string3") {
            ofs << indent << "CfgString3 " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "int3arr") {
            ofs << indent << "std::vector<CfgInt3> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "string3arr") {
            ofs << indent << "std::vector<CfgString3> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "intarr2") {
            ofs << indent << "std::vector<std::vector<int>> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else if (ctype == "stringarr2") {
            ofs << indent << "std::vector<std::vector<std::string>> " << col_name[nIdx] << ";// " << comment[nIdx] << endl;
        } else {
            PERROR << " ==========ERROR========== type not supported:" << col_type[nIdx] << endl;
            return false;
        }
    }
    --indent;
    ofs << indent << "};" << endl;
    ofs << endl;


    ofs << indent << "extern std::unique_ptr<DTCfgTable<int, " << cname << ">> g_pCfg_" << gen_name << ";// DT" << gen_name << ".json" << endl;

    ofs << endl;

    ofs.flush();
    ofs.close();

    return true;
}

template <class T>
bool _GenArr1(ofstream& ofs, Indent& indent, const string& ctype, bool& cdefault, const vector<string>& rLine, int ncol, int nIdx, const string& gen_name,
              const int& row_start, int fixed_len1 = -1) {
    ofs << "[";
    if (!cdefault) {
        vector<T> vdata;
        if (!GetArrayFromTable(vdata, rLine[ncol])) {
            PERROR << " ==========ERROR========== IntArr pasrse data error. gen_name:" << gen_name << " line:" << nIdx + row_start + 1 << " data:" << rLine[ncol]
                   << endl;
            return false;
        }
        if (fixed_len1 > 0 && vdata.size() != fixed_len1) {
            PERROR << " ==========ERROR========== IntArr pasrse data error, fixed_len1 check error. gen_name:" << gen_name << " line:" << nIdx + row_start + 1
                   << " data:" << rLine[ncol] << " fixed_len1:" << fixed_len1 << " len:" << vdata.size() << endl;
            return false;
        }
        for (int ntmp = 0; ntmp < vdata.size(); ++ntmp) {
            if (typeid(T) == typeid(string)) {
                ofs << "\"" << vdata[ntmp] << "\"";
            } else {
                ofs << vdata[ntmp];
            }
            if (ntmp < vdata.size() - 1) {
                ofs << ", ";
            }
        }
    }
    ofs << "]";
    return true;
}

template <class T>
bool _GenArr2(ofstream& ofs, Indent& indent, const string& ctype, bool& cdefault, const vector<string>& rLine, int ncol, int nIdx, const string& gen_name,
              const int& row_start, int fixed_len1 = -1, int fixed_len2 = -1) {
    if (cdefault) {
        ofs << "[]";
    } else {
        ofs << "[" << endl;
        ++indent;
        vector<vector<T>> vdata;
        if (!GetArray2FromTable(vdata, rLine[ncol])) {
            PERROR << " ==========ERROR========== pasrse " << ctype << " data error. gen_name:" << gen_name << " line:" << nIdx + row_start + 1
                   << " data:" << rLine[ncol] << endl;
            return false;
        }

        for (int i = 0; i < vdata.size(); ++i) {
            const auto& rarr1 = vdata[i];
            if (rarr1.empty()) continue;
            if (fixed_len1 > 0 && rarr1.size() != fixed_len1) {
                PERROR << " ==========ERROR========== pasrse " << ctype << " data error, fixed_len1 check error. gen_name:" << gen_name
                       << " line:" << nIdx + row_start + 1 << " data:" << rLine[ncol] << ". " << rarr1.size() << " != " << fixed_len1 << endl;
                return false;
            }
            ofs << indent << "[";
            for (int j = 0; j < rarr1.size(); ++j) {
                const auto& rarr1j = rarr1[j];
                if (typeid(T) == typeid(string)) {
                    ofs << "\"" << rarr1j << "\"";
                } else {
                    ofs << rarr1j;
                }
                if (j < rarr1.size() - 1) {
                    ofs << ", ";
                }
            }
            ofs << "]";
            if (i < vdata.size() - 1) {
                ofs << "," << endl;
            } else {
                ofs << endl;
            }
        }
        --indent;
        ofs << indent << "]";
    }
    return true;
}

bool CGeneratorCPP::GenDatas(const std::string& gen_name, const std::vector<std::vector<std::string>>& rData) {
    const int row_start = 4;
    const int key_count = 1;

    const auto& col_names = rData[1];
    auto col_types = rData[2];
    auto col_status = rData[0];
    for (auto& it : col_types) trans_lower(it);

    int col_count = std::min(rData[1].size(), rData[2].size());

    // gen server json, used for cpp
    {
        Indent indent;
        string json_cpp = strJsonOutputPathCpp + "DT" + gen_name + ".json";
        ofstream ofs(json_cpp.c_str());
        ofs << indent << "{" << endl;
        ++indent;
        for (int nIdx = row_start; nIdx < rData.size(); ++nIdx) {
            const auto& rLine = rData[nIdx];
            bool bLastLine = nIdx == rData.size() - 1;

            if (rLine.size() < key_count) {
                PERROR << " ==========ERROR========== data must have key. gen_name:" << gen_name << " line:" << nIdx + row_start + 1 << endl;
                return false;
            }

            ofs << indent << "\"" << rLine[0] << "\": {" << endl;
            ++indent;
            for (int ncol = 0; ncol < col_names.size(); ++ncol) {
                if (!GenThisCol(col_status, ncol, true)) continue;

                ofs << indent << "\"" << col_names[ncol] << "\": ";
                if (col_types[ncol] != "int") ofs << "\"";

                if (ncol < rLine.size())
                    ofs << rLine[ncol];
                else {
                    if (col_types[ncol] == "int") ofs << 0;
                }

                if (col_types[ncol] != "int") ofs << "\"";

                if (ncol < col_names.size() - 1)
                    ofs << "," << endl;
                else
                    ofs << endl;
            }
            --indent;
            if (bLastLine)
                ofs << indent << "}" << endl;
            else
                ofs << indent << "}," << endl;
        }
        --indent;
        ofs << indent << "}";
        ofs.flush();
        ofs.close();
    }

    // gen client json, used for js
    {
        Indent indent;
        string json_js = strJsonOutputPathJs + "DT" + gen_name + ".json";
        ofstream ofs(json_js.c_str());
        ofs << indent << "{" << endl;
        ++indent;
        for (int nIdx = row_start; nIdx < rData.size(); ++nIdx) {
            const auto& rLine = rData[nIdx];
            bool bLastLine = nIdx == rData.size() - 1;

            if (rLine.size() < key_count) {
                PERROR << " ==========ERROR========== data must have key. gen_name:" << gen_name << " line:" << nIdx + row_start + 1 << endl;
                return false;
            }

            ofs << indent << "\"" << rLine[0] << "\": {" << endl;
            ++indent;
            for (int ncol = 0; ncol < col_names.size(); ++ncol) {
                if (!GenThisCol(col_status, ncol, false)) continue;

                const auto& ctype = col_types[ncol];
                const auto& cname = col_names[ncol];
                bool cdefault = (ncol >= rLine.size()) || rLine[ncol].empty();

                // 字段名
                ofs << indent << "\"" << cname << "\": ";

                if (ctype == "int") {
                    if (cdefault) {
                        ofs << "0";
                    } else {
                        ofs << rLine[ncol];
                    }
                } else if (ctype == "string") {
                    ofs << "\"";
                    if (!cdefault) {
                        ofs << rLine[ncol];
                    }
                    ofs << "\"";
                } else if (ctype == "intarr") {
                    if (!_GenArr1<int>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start)) return false;
                } else if (ctype == "stringarr") {
                    if (!_GenArr1<string>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start)) return false;
                } else if (ctype == "intarr2") {
                    if (!_GenArr2<int>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start)) return false;
                } else if (ctype == "stringarr2") {
                    if (!_GenArr2<string>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start)) return false;
                } else if (ctype == "int2") {
                    if (!_GenArr1<int>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 2)) return false;
                } else if (ctype == "string2") {
                    if (!_GenArr1<string>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 2)) return false;
                } else if (ctype == "int2arr") {
                    if (!_GenArr2<int>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 2)) return false;
                } else if (ctype == "string2arr") {
                    if (!_GenArr2<string>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 2)) return false;
                } else if (ctype == "int3") {
                    if (!_GenArr1<int>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 3)) return false;
                } else if (ctype == "string3") {
                    if (!_GenArr1<string>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 3)) return false;
                } else if (ctype == "int3arr") {
                    if (!_GenArr2<int>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 3)) return false;
                } else if (ctype == "string3arr") {
                    if (!_GenArr2<string>(ofs, indent, ctype, cdefault, rLine, ncol, nIdx, gen_name, row_start, 3)) return false;
                } else {
                    PERROR << " ==========ERROR========== type not supported:" << ctype << endl;
                    return false;
                }

                // 处理最后的逗号，最后一个col不要加逗号
                if (ncol < col_names.size() - 1)
                    ofs << "," << endl;
                else
                    ofs << endl;
            }
            --indent;
            if (bLastLine)
                ofs << indent << "}" << endl;
            else
                ofs << indent << "}," << endl;
        }
        --indent;
        ofs << indent << "}";
        ofs.flush();
        ofs.close();
    }

    return true;
}
