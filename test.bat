pushd bin\Debug

call test.bat

popd

pushd projects\test_simdjson\bin\Debug\

test_generated.exe

popd
