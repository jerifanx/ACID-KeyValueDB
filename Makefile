CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread
TARGET = run_kvs
OBJS = main.o kvs.o transaction.o tests.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cc kvs.h transaction.h tests.h
	$(CXX) $(CXXFLAGS) -c main.cc

kvs.o: kvs.cc kvs.h transaction.h
	$(CXX) $(CXXFLAGS) -c kvs.cc

transaction.o: transaction.cc transaction.h kvs.h
	$(CXX) $(CXXFLAGS) -c transaction.cc

tests.o: tests.cc tests.h kvs.h transaction.h
	$(CXX) $(CXXFLAGS) -c tests.cc

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
