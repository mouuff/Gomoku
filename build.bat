mkdir build
cd build
cmake ..
cmake --build . --config Release
copy Release\*.exe ..
cd ..
pause
