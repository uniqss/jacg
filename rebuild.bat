@rem @echo off




set PROJS=openxlsx jacg read_xlsx_test test_simdjson

if "1" == "1" (
    set BAT_BUILD_TYPE=Debug
) else (
    set BAT_BUILD_TYPE=Release
)




cd projects

for %%P in (%PROJS%) do (
    echo "rebuild %%P start ############################################################################"
    pushd %%P
    call rebuild.bat %BAT_BUILD_TYPE%
    popd

    echo "rebuild %%P done ==========================================================================="
)

cd ..




@REM pause
