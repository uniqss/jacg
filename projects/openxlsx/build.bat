cd OpenXLSX-0.3.1

set BAT_BUILD_TYPE=RelWithDebInfo


rm -rf build
mkdir build
cd build


cmake -A x64 -DCMAKE_BUILD_TYPE=%BAT_BUILD_TYPE% ..

cmake --build . --config %BAT_BUILD_TYPE%

cmake --install . --prefix ..\..\..\ --config %BAT_BUILD_TYPE%

cd ..

mkdir ..\..\..\bin\%BAT_BUILD_TYPE%\
cp -R ./build/output/%BAT_BUILD_TYPE%/*.dll ..\..\..\bin\%BAT_BUILD_TYPE%\

cd ..

rem pause