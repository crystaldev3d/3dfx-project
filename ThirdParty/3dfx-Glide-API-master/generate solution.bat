cd glide3\src\fxgasm_tool

mkdir bin_win32
cd bin_win32
cmake --DBUILD_32BIT=ON -G "Visual Studio 17 2022" -A Win32
cmake --build .

pause