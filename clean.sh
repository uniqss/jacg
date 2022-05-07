#!/bin/bash

# - install depends tools
# yum -y install git
# yum -y install gcc gcc-c++ autoconf libtool automake make
#




PROJS=(jsoncpp openxlsx jacg)




cd projects

rm -rf include
rm -rf lib
rm -rf bin

for P in "$PROJS"
do

    echo "clean $P start ############################################################################"
    pushd $P
    call clean.sh
    popd

    echo "clean $P done ==========================================================================="
done

cd ..






pushd spdlog
rm -rf bin
rm -rf build
rm -rf lib
sh clean_all.sh
popd

echo "clean spdlog done ==========================================================================="


pushd uconfiglua
rm -rf bin
rm -rf build
rm -rf lib
sh clean_all.sh
popd

echo "clean uconfiglua done ==========================================================================="

