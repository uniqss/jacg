



set PROJS=jsoncpp openxlsx jacg




cd projects

rm -rf include
rm -rf lib
rm -rf bin

for %%P in (%PROJS%) do (
    echo "clean %%P start ############################################################################"
    pushd %%P
    call clean.bat %BAT_BUILD_TYPE%
    popd

    echo "clean %%P done ==========================================================================="
)

cd ..


