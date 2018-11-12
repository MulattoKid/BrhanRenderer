@echo off

mkdir build
pushd build

set SRC_FILES="../src/*.cpp"
set MEM_POOL_SRC="../src/MemoryPool/MemoryPool.cpp"
set OUT="/Fe:BrhanRenderer.exe"
set COMPILER_THREADS="/cgthreads4"
set OPTIMIZATION="/O2"
set OPENMP="/openmp"
set EXCEPTION="/EHsc"

cl %SRC_FILES% %MEM_POOL_SRC% %OUT% %COMPILER_THREADS% %OPTIMIZATION% %OPENMP% %EXCEPTION%

popd
