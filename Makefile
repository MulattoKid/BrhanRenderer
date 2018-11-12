SRC_DIR = src
OBJ_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
MEM_POOL_CPP = $(SRC_DIR)/MemoryPool/MemoryPool.cpp
MEM_POOL_OBJ = $(OBJ_DIR)/MemoryPool.o

CXX = g++
CXXFLAGS = -std=c++11 -fopenmp
LDFLAGS = -fopenmp -lstdc++fs

#Setup for release
all : CXXFLAGS += -O2
all : BrhanRenderer

#Setup for debug
debug : CXXFLAGS += -Wall -g -O0
debug : BrhanRenderer

#Links all object files
BrhanRenderer : $(OBJ_FILES) $(MEM_POOL_OBJ)
	$(CXX) -o ./$(OBJ_DIR)/BrhanRenderer $(OBJ_FILES) $(MEM_POOL_OBJ) $(LDFLAGS)

.PHONY : clean
clean :
	rm build/*

#Compiles all source files
#$< is the input file (.cpp file)
#$@ is the output file (.o file)
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(MEM_POOL_OBJ) : $(MEM_POOL_CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@
