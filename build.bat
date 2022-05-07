@echo off

cd projects




pushd openxlsx
call build.bat
popd

echo "build openxlsx done ==========================================================================="


pushd jacg
call build.bat
popd

echo "build jacg done ==========================================================================="


cd ..

rem pause