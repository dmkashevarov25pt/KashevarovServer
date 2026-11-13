CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = vcalc
SRCS = main.cpp Server.cpp Communicator.cpp HashProcessor.cpp Vector.cpp Logger.cpp
OBJS = $(SRCS:.cpp=.o)
LIBS = -lboost_program_options -lcrypto
TEST_TARGET = tests/run_tests
TEST_CASES = $(wildcard tests/[1-5].*.cpp)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET)

$(TEST_TARGET): tests/test_runner.cpp $(TEST_CASES) $(filter-out main.cpp,$(SRCS)) $(TARGET)
	$(CXX) $(CXXFLAGS) -pthread -I. -Itests -o $@ tests/test_runner.cpp $(filter-out main.cpp,$(SRCS)) $(LIBS)

test: $(TARGET) $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean test
