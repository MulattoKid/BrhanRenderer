@echo off

mkdir build_debug
pushd build_debug

set SRC_FILES="../src/*.cpp"
set MEM_POOL_SRC="../src/MemoryPool.cpp"
set OUT="/Fe:BrhanRendererDebug.exe"
set DEBUG_PARAM="/Zi"
set OPTIMIZATION="/Od"
set EXCEPTION="/EHsc"

cl %SRC_FILES% %MEM_POOL_SRC% %OUT% %DEBUG_PARAM% %OPTIMIZATION% %EXCEPTION%

popd
