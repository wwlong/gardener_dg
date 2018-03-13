SRC += ./simplest_web_server.c
SRC += ../mongoose/mongoose.c
INC += -I../http_client_test -I../mongoose
CFLAGS +=  -g -W -Wall -Werror -I../..  -I./ -Wno-unused-function
CFLAGS += ${INC} 

CC = gcc
	
simplest_web_server:${SRC}
	${CC}  ${CFLAGS} $^ -o $@

clean:
	rm *.exe *.bin -rf
