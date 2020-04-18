TARGET = bot
CC=gcc
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
LIBS = -ldl -lpthread
LOCAL_LIBS = -L ../libs
LOCAL_LIBS += -lnetwork -lthrd -llistes
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