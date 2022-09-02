#!/bin/bash



cd projects




PROJS="openxlsx jacg test_simdjson"




for P in $PROJS
do

    echo "build $P start ############################################################################"
    cd $P
    sh build.sh
	cd ..

    echo "build $P done ==========================================================================="
done

cd ..

# echo continue && read -n 1
