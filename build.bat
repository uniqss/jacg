@echo off




set PROJS=openxlsx jacg test_simdjson

if "1" == "1" (
    set BAT_BUILD_TYPE=Debug
) else (
    set BAT_BUILD_TYPE=Release
)




cd projects

for %%P in (%PROJS%) do (
    echo "build %%P start ############################################################################"
    pushd %%P
    call build.bat %BAT_BUILD_TYPE%
    popd

    echo "build %%P done ==========================================================================="
)

cd ..




@REM pause
