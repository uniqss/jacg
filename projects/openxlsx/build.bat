@REM cd OpenXLSX-0.3.2
cd OpenXLSX-20220507



IF "%~1" == "" (
    set BAT_BUILD_TYPE=Release
) else (
    set BAT_BUILD_TYPE=%1
)



mkdir build
pushd build


cmake -A x64 -DCMAKE_BUILD_TYPE=%BAT_BUILD_TYPE% .. -DOPENXLSX_CREATE_DOCS=OFF -DOPENXLSX_BUILD_SAMPLES=OFF -DOPENXLSX_BUILD_TESTS=OFF -DOPENXLSX_BUILD_BENCHMARKS=OFF
@REM cmake -A x64 -DCMAKE_BUILD_TYPE=Debug .. -DOPENXLSX_CREATE_DOCS=OFF -DOPENXLSX_BUILD_SAMPLES=OFF -DOPENXLSX_BUILD_TESTS=OFF -DOPENXLSX_BUILD_BENCHMARKS=OFF

cmake --build . --config %BAT_BUILD_TYPE% --parallel 8

cmake --install . --prefix ..\..\..\ --config %BAT_BUILD_TYPE%

popd

xcopy /S /Q /Y ..\..\bin\*.dll ..\..\..\bin\%BAT_BUILD_TYPE%\

cd ..
