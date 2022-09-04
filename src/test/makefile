BASEINC=$(RUN_PATH)/inc
BASELIB=$(RUN_PATH)/lib
INC=$(RUN_PATH)/src/inc
LIB=$(RUN_PATH)/src/lib

CC = gcc

CFLAGS = -I$(BASEINC) -L$(BASELIB) -I$(INC) -L$(LIB) -I.
CLIBS = -L$(BASELIB) -L$(LIB) -lbase_lib

all:Test install clean
Test:Test.o
	$(CC) -o $@ Test.o $(CFLAGS) $(CLIBS)
install:
	mv Test $(RUN_PATH)/bin
.c.o:
	$(CC) $(CFLAGS) -c $(<:.c=.c)	
uninstall:clean
	rm -f $(RUN_PATH)/bin/Test
clean:
	rm -f *.o
