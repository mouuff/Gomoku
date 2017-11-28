mkdir build
cd build
cmake ..
cmake --build . --config Release
copy Release\*.exe ..
pause
