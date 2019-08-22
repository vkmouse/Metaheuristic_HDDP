HEADER = __header__.h
TARGET = main

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CC = g++
OPTION = -O3

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(OPTION) -lm 

%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) $(OPTION)  -c $<

clean:
	del -rf *.o
	del $(TARGET).exe



