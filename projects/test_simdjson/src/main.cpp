#include "CfgTables.h"
#include "DTtest.h"

#include <iostream>

using std::cout;
using std::endl;


int main() {
    cout << "main begin" << endl;
    int ret = -1;

    std::string err;
    std::string prefix = "./TableJson/";
    do {
        if (!LoadDTTables(prefix, err)) {
            cout << "LoadDTTables failed!" << endl;
            break;
        }

        int key = 10000101;
        auto it = g_pCfg_test->find(key);
        if (it != nullptr) {
            cout << it->int1 << endl;
            cout << it->str2server << endl;
        } else {
            cout << "key not found. key:" << key << endl;
        }

        ret = 0;
    } while (false);


    if (ret != 0) std::cout << "err:" << err << endl;

    cout << "main end" << endl;
    return 0;
}
