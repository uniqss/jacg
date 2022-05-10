#include "CfgTables.h"
#include "DTtest.h"

#include <iostream>

using std::cout;
using std::endl;


int main() {
    int ret = -1;

    std::string err;
    do {
        if (!LoadDTTables(err)) break;


        auto it = g_pCfg_test->find(10000101);
        if (it != nullptr) {
            cout << it->int1 << endl;
            cout << it->str2server << endl;
        }

        ret = 0;
    } while (false);


    if (ret != 0) std::cout << "err:" << err << endl;

    return 0;
}
