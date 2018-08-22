SRC_DIR = src
OBJ_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -fopenmp

#Setup for release
all : CXXFLAGS += -O2
all : BrhanRenderer

#Setup for debug
debug : CXXFLAGS += -fopenmp -Wall -g -O0
debug : BrhanRenderer

#Links all object files
BrhanRenderer : $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o ./$(OBJ_DIR)/BrhanRenderer $(OBJ_FILES)

#Compiles all source files
#$< is the input file (.cpp file)
#$@ is the output file (.o file)
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY : clean
clean :
	rm build/*

#; $(info $$OBJ_FILES is [${OBJ_FILES}])
