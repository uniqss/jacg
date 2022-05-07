


IF "%~1" == "" (
    set BAT_BUILD_TYPE=Release
) else (
    set BAT_BUILD_TYPE=%1
)



rm -rf build
mkdir build
cd build


cmake -A x64 -DCMAKE_BUILD_TYPE=%BAT_BUILD_TYPE% ..

cmake --build . --config %BAT_BUILD_TYPE%

@REM cmake --install . --prefix ..\..\..\ --config %BAT_BUILD_TYPE%

cd ..

mkdir ..\..\bin\%BAT_BUILD_TYPE%\
cp -R .\bin\%BAT_BUILD_TYPE%\*.exe ..\..\bin\%BAT_BUILD_TYPE%\
cp -R .\bin\%BAT_BUILD_TYPE%\*.dll ..\..\bin\%BAT_BUILD_TYPE%\
cp -R .\bin\%BAT_BUILD_TYPE%\*.pdb ..\..\bin\%BAT_BUILD_TYPE%\

cd ..

rem pause
