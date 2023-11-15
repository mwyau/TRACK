SOURCES=read_tracks.c \
        meantrd.c \
        realloc_n.c 

OBJECTS=$(SOURCES:.c=.o)

#CFLAGS = -g -Xa -I../../include ${ADDINCS} -D${SYSTEM} -D${HAVNETC} $(AD_C_FLAGS) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64


CFLAGS = -g -ansi -Wall -I../../include -D${SYSTEM} $(AD_C_FLAGS) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

LIBS = -lm 


.KEEP_STATE:

all: rshear

rshear: $(OBJECTS) rshear.c 
	${CC} ${CFLAGS} -c rshear.c 
	$(CC) $(CFLAGS) -o ../bin/$@ rshear.o $(OBJECTS) $(LIBS)

