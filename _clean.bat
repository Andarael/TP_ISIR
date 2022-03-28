@ECHO OFF
ECHO [TP_ISIR] CLEANING SOLUTION
rmdir /s /q "CMakeFiles"
rmdir /s /q ".vs"
rmdir /s /q "x64"
rmdir /s /q "Debug"
rmdir /s /q "Release"
rmdir /s /q "build"
rmdir /s /q "TP_ISIR.dir"
del /q "CMakeCache.txt"
del /q "*.cmake"
del /q "*.vcxproj"
del /q "*.sln"
del /q "*.filters"
del /q "*.user"
ECHO [TP_ISIR] DONE
PAUSE