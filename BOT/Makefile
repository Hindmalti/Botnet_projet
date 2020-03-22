TARGET=commudp
FLAGS=-Wall -Wextra -I ../libs
LIBS= -lnetwork -lthrd -lpthread -L ../libs

all: compile

debug: FLAGS += -DDEBUG -g
debug: compile

clean:
	rm -f *.o *.a
	rm -f ../bin/$(TARGET)

exec: compile
	./$(TARGET)

compile:
	gcc *.c -o $(TARGET) $(LIBS) $(FLAGS)
	mkdir -p ../bin
	cp $(TARGET) ../bin/