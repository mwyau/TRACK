SOURCES=read_tracks.c \
        convert_track.c

OBJECTS=$(SOURCES:.c=.o)

#CFLAGS = -g -Xa -I../../include ${ADDINCS} -D${SYSTEM} -D${HAVNETC} $(AD_C_FLAGS) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64


CFLAGS = -g -ansi -Wall -I../../include ${ADDINCS} -D${SYSTEM} -D${HAVNETC} $(AD_C_FLAGS) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

LIBS = -lm ${ADLIBS}


.KEEP_STATE:

all: helicity 

helicity: helicity.c $(OBJECTS)
	$(CC) $(CFLAGS) -c helicity.c
	$(CC) $(CFLAGS) -o ../bin/$@ helicity.o $(OBJECTS) $(LIBS)

