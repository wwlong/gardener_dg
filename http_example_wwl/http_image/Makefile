SRC += ./http_client.c 
SRC += ../../http_mongoose/mongoose/mongoose.c
INC += -I./ -I../../http_mongoose/mongoose
CFLAGS +=  -g -W -Wall -Werror -I../..  -I./ -Wno-unused-function
CFLAGS += ${INC} 

CC = gcc
	
http_client:${SRC}
	${CC}  ${CFLAGS} $^ -o $@
.PHONY:clean
clean:
	rm http_client -rf
