cd jsoncpp-1.9.5



IF "%~1" == "" (
    set BAT_BUILD_TYPE=Release
) else (
    set BAT_BUILD_TYPE=%1
)



rm -rf build
mkdir build
cd build


cmake -A x64 -DCMAKE_BUILD_TYPE=%BAT_BUILD_TYPE% ..

cmake --build . --config %BAT_BUILD_TYPE% --parallel 8

cmake --install . --prefix ..\..\..\ --config %BAT_BUILD_TYPE%

cd ..

umkdir ../../../bin/%BAT_BUILD_TYPE%/
cp -R ../../bin/*.dll ../../../bin/%BAT_BUILD_TYPE%/

cd ..

rem pause