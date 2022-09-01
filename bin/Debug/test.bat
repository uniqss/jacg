.\uconfig.exe %~dp0 ../test/xlsx ../../projects/test_simdjson/gen_cpp ../../projects/test_simdjson/bin/Debug/TableJson/ ../test/json_js

echo a|xcopy ..\..\projects\test_simdjson\bin\Debug\TableJson\*.* .\TableJson\ /S /Q

sleep 10s
