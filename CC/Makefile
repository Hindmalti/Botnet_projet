TARGET=cc
SRCS=$(wildcard *.c)
OBJS = $(SRCS:.c=.o)
LIBS = -lpthread -ldl
LOCAL_LIBS = -L ../libs
LOCAL_LIBS += -lshm -lnetwork -lthrd -llistes
CFLAGS = -Wall -Wextra -pedantic -I ../libs
DFLAGS = -g -DDEBUG

all: compile

debug: FLAGS += $(DFLAGS)
debug: compile

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f ../bin/$(TARGET)

exec: compile
	./$(TARGET)

%.o: %.c %.h
	 $(CC) -c $(CFLAGS) -o $@ $<

compile: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LOCAL_LIBS) $(LIBS)
	mkdir -p ../bin
	cp $(TARGET) ../bin/
	gcc *.c -o $(TARGET) $(LIBS) $(FLAGS)
	mkdir -p ../bin
	cp $(TARGET) ../bin/