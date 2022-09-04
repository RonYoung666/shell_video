BASEINC=$(RUN_PATH)/inc
BASELIB=$(RUN_PATH)/lib
INC=$(RUN_PATH)/src/inc
LIB=$(RUN_PATH)/src/lib

CC = gcc

CFLAGS = -I$(BASEINC) -L$(BASELIB) -I$(INC) -L$(LIB) -I.
CLIBS = -L$(BASELIB) -L$(LIB)

OBJS = 24BitImage.o

all:install lib clean
install:
	cp 24BitImage.h $(BASEINC)
lib:$(OBJS)
	ar -rv $(BASELIB)/libbase_lib.a $(OBJS)
uninstall:clean
	rm -f $(BASEINC)/24BitImage.h
.c.o:
	$(CC) $(CFLAGS) -c $(<:.c=.c)
clean:
	rm -f *.o
