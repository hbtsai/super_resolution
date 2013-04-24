EXEC=sr
CC=gcc
CXX=g++
DBG_CFLAGS= -D_DEBUG -g
CFLAGS= $(DBG_CFLAGS)
CXXFLAGS=$(DBG_CFLAGS)
LIBS= -lopencv_core -lopencv_highgui -lopencv_imgproc
OBJS=main.o libimage.o

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(EXEC) $(LIBS) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -c -o $@ $<

clean:
	rm *.o $(EXEC)
