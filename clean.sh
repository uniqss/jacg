#!/bin/bash



PROJS=(openxlsx jacg test_simdjson)




cd projects

rm -rf include
rm -rf lib
rm -rf bin

for P in "$PROJS"
do

    echo "clean $P start ############################################################################"
    cd $P
    call clean.sh
	cd ..

    echo "clean $P done ==========================================================================="
done

cd ..






cd spdlog
rm -rf bin
rm -rf build
rm -rf lib
sh clean_all.sh
cd ..

echo "clean spdlog done ==========================================================================="


cd uconfiglua
rm -rf bin
rm -rf build
rm -rf lib
sh clean_all.sh
cd ..

echo "clean uconfiglua done ==========================================================================="

