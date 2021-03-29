CC=gcc
CFLAGS=-g -Wall -Wextra
LIBS=-lm -lpthread -L ./CommandParser -lcli
TARGET=graph.exe

OBJS=gluethread/glthread.o \
		graph.o \
		net.o \
		utils.o \
		topologies.o \
		nwcli.o \
		comm.o

graph.exe: testapp.o ${OBJS} CommandParser/libcli.a
	@${CC} ${CFLAGS} testapp.o ${OBJS} ${LIBS} -o ${TARGET} ${LIBS}

testapp.o: testapp.c
	@${CC} ${CFLAGS} -c testapp.c -o testapp.o

gluethread/glthread.o: gluethread/glthread.c
	@${CC} ${CFLAGS} -c -I gluethread gluethread/glthread.c -o gluethread/glthread.o

graph.o: graph.c
	@${CC} ${CFLAGS} -c -I . graph.c -o graph.o

net.o: net.c
	@${CC} ${CFLAGS} -c -I . net.c -o net.o

utils.o: utils.c
	@${CC} ${CFLAGS} -c -I . utils.c -o utils.o

topologies.o: topologies.c
	@${CC} ${CFLAGS} -c -I . topologies.c -o topologies.o

nwcli.o:
	@${CC} ${CFLAGS} -c -I . nwcli.c -o nwcli.o

comm.o:
	@${CC} ${CFLAGS} -c -I . comm.c -o comm.o

CommandParser/libcli.a:
	@(cd CommandParser; make)

all:
	@make
	@make CommandParser/libcli.a
	# @(cd CommandParser; make)

clean:
	@rm -f *.o
	@rm -f gluethread/glthread.o
	@rm -f *.exe

cleanall:
	@make clean
	@(cd CommandParser; make clean)