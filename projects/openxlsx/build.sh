#!/bin/bash

# - install depends tools
# yum -y install git
# yum -y install gcc gcc-c++ autoconf libtool automake make
#

#cd OpenXLSX-0.3.1
cd OpenXLSX-0.3.2
#cd OpenXLSX-20220507
#cd OpenXLSX-20220830

BAT_BUILD_TYPE=Release

rm -rf build
mkdir -p build
cd build


# shared
#cmake -DCMAKE_BUILD_TYPE=$BAT_BUILD_TYPE -DOPENXLSX_BUILD_BENCHMARKS=OFF -DOPENXLSX_LIBRARY_TYPE=SHARED ..
# static
cmake -DCMAKE_BUILD_TYPE=$BAT_BUILD_TYPE -DOPENXLSX_BUILD_BENCHMARKS=OFF ..

cmake --build . --config $BAT_BUILD_TYPE --parallel 8

cmake --install . --prefix ../../../ --config $BAT_BUILD_TYPE
#sudo cmake --install . --config $BAT_BUILD_TYPE


cd ..

mkdir -p ../../../bin/$BAT_BUILD_TYPE/
#cp -R ./build/output/$BAT_BUILD_TYPE/*.dll ../../../bin/$BAT_BUILD_TYPE/
#mkdir -p ../../lib
#cp -a ../../lib64/libOpenXLSX.* ../../lib/

cd ..

# echo continue && read -n 1
