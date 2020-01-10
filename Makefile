CC=gcc
CFLAGS=-g -Wall -Wextra
LIBS=-lm
TARGET=graph.exe

OBJS=gluethread/glthread.o \
		graph.o \
		net.o \
		utils.o \
		topologies.o

graph.exe: testapp.o ${OBJS}
	@${CC} ${CFLAGS} testapp.o ${OBJS} ${LIBS} -o ${TARGET}

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

clean:
	@rm -f *.o
	@rm -f gluethread/glthread.o
	@rm -f *.exe