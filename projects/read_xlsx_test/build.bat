


IF "%~1" == "" (
    set BAT_BUILD_TYPE=Release
) else (
    set BAT_BUILD_TYPE=%1
)



mkdir build
pushd build


cmake -A x64 -DCMAKE_BUILD_TYPE=%BAT_BUILD_TYPE% ..

cmake --build . --config %BAT_BUILD_TYPE% --parallel 8

@REM cmake --build . --config Debug --parallel 8

@REM cmake --install . --prefix ..\..\..\ --config %BAT_BUILD_TYPE%

popd

xcopy /S /Q /Y .\bin\%BAT_BUILD_TYPE%\*.exe ..\..\bin\%BAT_BUILD_TYPE%\
xcopy /S /Q /Y .\bin\%BAT_BUILD_TYPE%\*.dll ..\..\bin\%BAT_BUILD_TYPE%\
xcopy /S /Q /Y .\bin\%BAT_BUILD_TYPE%\*.pdb ..\..\bin\%BAT_BUILD_TYPE%\

xcopy /S /Q /Y .\%BAT_BUILD_TYPE%\*.exe ..\..\bin\%BAT_BUILD_TYPE%\
xcopy /S /Q /Y .\%BAT_BUILD_TYPE%\*.dll ..\..\bin\%BAT_BUILD_TYPE%\
xcopy /S /Q /Y .\%BAT_BUILD_TYPE%\*.pdb ..\..\bin\%BAT_BUILD_TYPE%\

xcopy /S /Q /Y .\build\%BAT_BUILD_TYPE%\*.exe ..\..\bin\%BAT_BUILD_TYPE%\
xcopy /S /Q /Y .\build\%BAT_BUILD_TYPE%\*.dll ..\..\bin\%BAT_BUILD_TYPE%\
xcopy /S /Q /Y .\build\%BAT_BUILD_TYPE%\*.pdb ..\..\bin\%BAT_BUILD_TYPE%\

cd ..

rem pause
