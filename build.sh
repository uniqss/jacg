#!/bin/bash

# - install depends tools
# yum -y install git
# yum -y install gcc gcc-c++ autoconf libtool automake make
#



cd projects


cd openxlsx
sh build.sh
cd ..

echo "build openxlsx done ==========================================================================="


cd jacg
sh build.sh
cd ..

echo "build jacg done ==========================================================================="




# echo continue && read -n 1
