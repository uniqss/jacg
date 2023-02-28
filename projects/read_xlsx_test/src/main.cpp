#include "uniqs_xlsx_reader.h"
#include "uniqs_xlsx_generator.h"
#include "uniqs_xlsx_generator_cpp.h"

#include "stl.h"
#include "dirhelper.h"

std::string strOutPath;
std::string strInputPath;
std::string strJsonOutputPathCpp;
std::string strJsonOutputPathJs;

void help(const string& pe_name) {
	cout << "uniqs cfg generator" << endl;
	cout << "by uniqs. contact uniqs@163.com for details." << endl;
	cout << "usage:" << endl;
	cout << pe_name
		<< " currpath(linux`pwd` windows %~dp0) input_path [cpp_output_path [json_output_path_cpp "
		"[json_output_path_js]]]"
		<< endl;
	cout << "input_path: the xlsx input folder fullpath" << endl;
	cout << "json_output_path_cpp: where the generated json data files to put(for cpp), default is ./json_cpp/" << endl;
	cout << "json_output_path_js: where the generated json data files to put(for js), default is ./json_js/" << endl;
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
	string strCurrPath = argv[1];
	strInputPath = argv[nPreArg + 1];
	strInputPath = trimpathfromarg(strCurrPath, strInputPath);

	if (argc > nPreArg + 2)
		strOutPath = argv[nPreArg + 2];
	else
		strOutPath = "./";
	strOutPath = trimpathfromarg(strCurrPath, strOutPath);

	if (argc > nPreArg + 3)
		strJsonOutputPathCpp = argv[nPreArg + 3];
	else
		strJsonOutputPathCpp = "./json_cpp/";
	strJsonOutputPathCpp = trimpathfromarg(strCurrPath, strJsonOutputPathCpp);

	if (argc > nPreArg + 4)
		strJsonOutputPathJs = argv[nPreArg + 4];
	else
		strJsonOutputPathJs = "./json_js/";
	strJsonOutputPathJs = trimpathfromarg(strCurrPath, strJsonOutputPathJs);

	cout << "strInputPath:[" << strInputPath << "]" << endl;
	cout << "strOutPath:[" << strOutPath << "]" << endl;
	cout << "strJsonOutputPathCpp:[" << strJsonOutputPathCpp << "]" << endl;
	cout << "strJsonOutputPathJs:[" << strJsonOutputPathJs << "]" << endl;
	int nResult = 0;
	nResult = makeDir(strOutPath);
	nResult = makeDir(strJsonOutputPathCpp);
	nResult = makeDir(strJsonOutputPathJs);

	std::vector<string> vecFileFullNames, vecFileNames;
	readFileList(strInputPath.c_str(), &vecFileNames, &vecFileFullNames);

	std::vector<std::string> vecXlsxFileNames;
	CGeneratorCPP oGenerator;

	auto isXLsx = [](const std::string& fileName) {
		// static const std::unordered_set<std::string> validEnd = {".xlsx", ".xlsm"};
		static const std::unordered_set<std::string> validEnd = { ".xlsx" };
		for (auto it : validEnd) {
			auto pos = fileName.find(it);
			if (pos != std::string::npos) return pos;
		}
		return std::string::npos;
	};

	std::unordered_map<std::string, int> uniqueTypes;

	for (int nIdx = 0; nIdx < vecFileNames.size(); ++nIdx) {
		if (vecFileNames[nIdx].find("$") != std::string::npos) continue;
		auto pos = isXLsx(vecFileNames[nIdx]);
		if (pos == std::string::npos) continue;
		string gen_name = vecFileNames[nIdx].substr(0, pos);

		cout << "generating " << vecFileNames[nIdx] << endl;

		std::vector<std::pair<std::string, std::vector<std::vector<std::string> > > > vecxlsx;
		int nRet = read_xlsx(vecFileFullNames[nIdx].c_str(), vecxlsx, 3);
		if (nRet != 0) {
			cout << "read_xlsx error." << endl;
			continue;
		}

		for (auto it_sheet : vecxlsx) {
#if 1
			cout << "sheet_name:" << it_sheet.first << endl;
			const auto& curr_sheet = it_sheet.second;
			if (curr_sheet.size() > 1) {
				for (const auto& curType : curr_sheet[1]) {
					uniqueTypes[curType]++;
				}
			}
#endif
#if 0
			oGenerator.GenerateOneSheet(it_sheet.first, gen_name, it_sheet.second);
			vecXlsxFileNames.push_back(gen_name);
#endif

			// currently only first sheet generated
			// break;
		}
	}

	// oGenerator.GenCfgTables(vecXlsxFileNames);

	std::ofstream ofs("./types.txt");
	std::vector<std::pair<std::string, int>> sortedDatas;
	for (auto it : uniqueTypes) {
		sortedDatas.push_back({ it.first, it.second });
	}
	std::sort(sortedDatas.begin(), sortedDatas.end(), [](auto& lhs, auto& rhs) {return lhs.second < rhs.second; });
	for (auto it : sortedDatas) {
		ofs << it.first << "\t" << it.second << endl;
	}
	ofs.flush();
	ofs.close();

	cout << "generate done." << endl;

	return 0;
}
