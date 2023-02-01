#include "uniqs_xlsx_reader.h"
#include "uniqs_xlsx_generator.h"
#include "uniqs_xlsx_generator_cpp.h"

#include "uniqs_tmpl_generator.h"

#include "stl.h"
#include "dirhelper.h"

std::string strOutPath;
std::string strCfgPath;
std::string strInputXlsxPath;

void help(const string& pe_name) {
    cout << "uniqs cfg generator" << endl;
    cout << "by uniqs. contact uniqs@163.com for details." << endl;
    cout << "usage:" << endl;
    cout << pe_name << " cfg_path input_xlsx_path [output_path]" << endl;
    cout << "cfg_path: must contain folder templates and rules" << endl;
    cout << "input_xlsx_path: the xlsx input folder fullpath" << endl;
    cout << "output_path: where the generated file to put. If not set, use current path." << endl;
}

void trimpathwin(std::string& str, bool append_splash_end = true) {
    // for (int nIdx = 0; nIdx < str.size(); ++nIdx) {
    //     if (str[nIdx] == '\\') {
    //         str[nIdx] = '/';
    //     }
    // }
    if (append_splash_end && str[str.size() - 1] != '/' && str[str.size() - 1] != '\\') {
        str += "/";
    }
}

string trimpathfromarg(std::string& strCurrPath, std::string& str) {
    trimpathwin(str);
    trimpathwin(strCurrPath);

    // absolute path
    if (isPathAbsWin(str) || (str.size() > 0 && str[0] == '/')) return str;

    if (str[0] == '.' && str[1] == '.')
        return strCurrPath + str;
    else if (str[0] == '.' && str[1] == '/')
        return strCurrPath + str.substr(2);
    else if (str[0] == '.' && str[1] != '.')
        return strCurrPath + str.substr(1);
    else {
        cout << __FUNCTION__ << __FILE__ << __LINE__ << "error. strCurrPath:" << strCurrPath << " str:" << str << endl;
        return "";
    }
}

int main(int argc, const char** argv) {
    string pe_name;
    if (argc > 0) pe_name = argv[0];
    int nPreArg = 1;
    if (argc < nPreArg + 2) {
        help(pe_name);
        return -1;
    }
    string strCurrPath = ".";

    strCfgPath = argv[nPreArg + 0];
    strCfgPath = trimpathfromarg(strCurrPath, strCfgPath);

    strInputXlsxPath = argv[nPreArg + 1];
    strInputXlsxPath = trimpathfromarg(strCurrPath, strInputXlsxPath);

    if (argc > nPreArg + 2)
        strOutPath = argv[nPreArg + 2];
    else
        strOutPath = "./";
    strOutPath = trimpathfromarg(strCurrPath, strOutPath);

    cout << "strInputXlsxPath:[" << strInputXlsxPath << "]" << endl;
    cout << "strCfgPath:[" << strCfgPath << "]" << endl;
    cout << "strOutPath:[" << strOutPath << "]" << endl;
    int nResult = 0;
    nResult = makeDir(strOutPath);

    std::vector<string> vecFileFullNames, vecFileNames;
    readFileList(strInputXlsxPath.c_str(), &vecFileNames, &vecFileFullNames);

    std::vector<std::string> vecXlsxFileNames;
    CTemplatedGenerator generator;

    return generator.Generate(vecFileFullNames, vecFileNames, strCfgPath, strOutPath);


    CGeneratorCPP oGenerator;
    for (int nIdx = 0; nIdx < vecFileNames.size(); ++nIdx) {
        auto pos = vecFileNames[nIdx].find(".xlsx");
        if (pos == std::string::npos) continue;
        if (vecFileNames[nIdx].find("$") != std::string::npos) continue;
        string gen_name = vecFileNames[nIdx].substr(0, pos);

        std::vector<std::pair<std::string, std::vector<std::vector<std::string>>>> vecxlsx;
        int nRet = read_xlsx(vecFileFullNames[nIdx].c_str(), vecxlsx);
        if (nRet != 0) {
            cout << "read_xlsx error." << endl;
            continue;
        }
        for (auto it_sheet : vecxlsx) {
            std::string sheetName = it_sheet.first;
            std::string xlsxName = gen_name;
            const std::vector<std::vector<std::string>>& datas = it_sheet.second;
            vecXlsxFileNames.push_back(gen_name);
        }
    }

    for (int nIdx = 0; nIdx < vecFileNames.size(); ++nIdx) {
        auto pos = vecFileNames[nIdx].find(".xlsx");
        if (pos == std::string::npos) continue;
        if (vecFileNames[nIdx].find("$") != std::string::npos) continue;
        string gen_name = vecFileNames[nIdx].substr(0, pos);

        cout << "generating " << vecFileNames[nIdx] << endl;

        std::vector<std::pair<std::string, std::vector<std::vector<std::string>>>> vecxlsx;
        int nRet = read_xlsx(vecFileFullNames[nIdx].c_str(), vecxlsx);
        if (nRet != 0) {
            cout << "read_xlsx error." << endl;
            continue;
        }

        for (auto it_sheet : vecxlsx) {
#if 0
			cout << "sheet_name:" << it_sheet.first << endl;
			for(auto it_row : it_sheet.second)
			{
				for(auto it_col : it_row)
				{
					cout << it_col << "\t";
				}
				cout << endl;
			}
#endif

            oGenerator.GenerateOneSheet(it_sheet.first, gen_name, it_sheet.second);
            vecXlsxFileNames.push_back(gen_name);

            // currently only first sheet generated
            break;
        }
    }

    oGenerator.GenCfgTables(vecXlsxFileNames);

    cout << "generate done." << endl;

    return 0;
}
