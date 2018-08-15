@echo off

mkdir build_debug
pushd build_debug

set SRC_FILES="../src/*.cpp"
set OUT="/Fe:BrhanRendererDebug.exe"
set DEBUG_PARAM="/Zi"

cl %SRC_FILES% %OUT% %DEBUG_PARAM%

popd