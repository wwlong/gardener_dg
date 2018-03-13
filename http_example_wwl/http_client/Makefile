SRC += ./http_client.c 
SRC += ./mongoose/mongoose.c
INC += -I./ -I./mongoose
CFLAGS +=  -g -W -Wall -I../..  -I./ -Wno-unused-function
CFLAGS += ${INC} 

CC = gcc
	
http_client:${SRC}
	${CC}  ${CFLAGS} $^ -o $@

clean:
	rm http_client -rf
