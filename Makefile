.PHONY: all clean clean-all

CXX = g++
LIBS = `pkg-config gtkmm-3.0 --cflags --libs`

BIN_FOLDER = bin
SRC_FOLDER = src

all: $(BIN_FOLDER)/app

$(BIN_FOLDER)/app: $(BIN_FOLDER)/main.o $(BIN_FOLDER)/m_learning.o $(BIN_FOLDER)/mynumber.o
	$(CXX) $^ -o $@ $(LIBS)

$(BIN_FOLDER)/main.o: main.cpp
	$(CXX) -c $^ $(LIBS) -o $@

$(BIN_FOLDER)/m_learning.o: $(SRC_FOLDER)/m_learning.cpp $(SRC_FOLDER)/m_learning.h
	$(CXX) -c $< -o $@

$(BIN_FOLDER)/mynumber.o: $(SRC_FOLDER)/mynumber.cpp $(SRC_FOLDER)/mynumber.h
	$(CXX) -c $< -o $@ $(LIBS)

clean:
	rm $(BIN_FOLDER)/*.o

clean-all:
	rm $(BIN_FOLDER)/*.o $(BIN_FOLDER)/app
