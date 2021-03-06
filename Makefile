CC=g++
CFLAGS=-c -Wall -std=c++11 -g
LDFLAGS=-lX11 -lpthread -lao -lsndfile -ldl -lm
SOURCES=main.cpp Sound.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=TestTinyArkanoid
BUILDPATH=Binaries.Linux/
SRCPATH=Source/
TARGET=$(BUILDPATH)$(EXECUTABLE)

all: build-dir $(TARGET)

$(TARGET): $(addprefix $(BUILDPATH), $(OBJECTS))
	$(CC) $(addprefix $(BUILDPATH), $(OBJECTS)) -o $@ $(LDFLAGS)

$(addprefix $(BUILDPATH), %.o): $(addprefix $(SRCPATH), %.cpp)
	$(CC) $(CFLAGS) $< -o $@

run: all
	cd $(BUILDPATH) && ./$(EXECUTABLE)

build-dir: 
	mkdir -p $(BUILDPATH)

clean:
	rm $(BUILDPATH)/*.o $(TARGET)
