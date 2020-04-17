TARGET=cc
FLAGS=-Wall -Wextra -I ../libs
LIBS=-lshm -lnetwork -lthrd -llistes -lpthread -L ../libs

all: compile

debug: FLAGS += -g -DDEBUG
debug: compile

clean:
	rm -f *.o *.out
	rm -f ../bin/$(TARGET).out

exec: compile
	./$(TARGET)

compile:
	gcc *.c -o $(TARGET) $(LIBS) $(FLAGS)
	mkdir -p ../bin
	cp $(TARGET) ../bin/