@echo off

mkdir build
pushd build

set SRC_FILES="../src/*.cpp"
set OUT="/Fe:BrhanRenderer.exe"

cl %SRC_FILES% %OUT%

popd