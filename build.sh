#!/bin/bash

# - install depends tools
# yum -y install git
# yum -y install gcc gcc-c++ autoconf libtool automake make
#



cd projects




PROJS=(openxlsx jacg test_simdjson)




for P in "$PROJS"
do

    echo "build $P start ############################################################################"
    pushd $P
    call build.sh
    popd

    echo "build $P done ==========================================================================="
done

cd ..

# echo continue && read -n 1
