@echo off

mkdir build_debug
pushd build_debug

set SRC_FILES="../src/*.cpp"
set OUT="/Fe:BrhanRendererDebug.exe"
set DEBUG_PARAM="/Zi"
set OPTIMIZATION="/Od"
set EXCEPTION="/EHsc"

cl %SRC_FILES% %OUT% %DEBUG_PARAM% %OPTIMIZATION% %EXCEPTION%

popd
