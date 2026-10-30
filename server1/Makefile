CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = vcalc
SRCS = main.cpp Server.cpp Communicator.cpp HashProcessor.cpp Vector.cpp Logger.cpp
OBJS = $(SRCS:.cpp=.o)
LIBS = -lboost_program_options -lcrypto

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean