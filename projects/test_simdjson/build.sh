#!/bin/bash


rm -rf build
mkdir -p build
cd build

BAT_BUILD_TYPE=Release

cmake -DCMAKE_BUILD_TYPE=$BAT_BUILD_TYPE ..
cmake --build . --config $BAT_BUILD_TYPE --parallel 8

cd ..


mkdir -p ../../bin/$BAT_BUILD_TYPE/
cp -R ./bin/$BAT_BUILD_TYPE/* ../../bin/$BAT_BUILD_TYPE/

