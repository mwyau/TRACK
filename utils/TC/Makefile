SOURCES=convert_track.c \
        meantrd.c \
        orog_test.c \
        read_tracks.c \
        write_fields_netcdf.c \
        write_intflux_netcdf.c


OBJECTS=$(SOURCES:.c=.o)

#CFLAGS = -g -Xa -I../../include ${ADDINCS} -D${SYSTEM} -D${HAVNETC} $(AD_C_FLAGS) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64


CFLAGS = -g -ansi -Wall -I../../include ${ADDINCS} -D${SYSTEM} -D${HAVNETC} $(AD_C_FLAGS) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

LIBS = -lm ${ADLIBS}


.KEEP_STATE:

all: tcident combine_reg reg_diff write_fields diffke median reg_maxmin rad_dist diffvec

tcident: tc_identify.c dirangle_shear.c $(OBJECTS)
	$(CC) $(CFLAGS) -c tc_identify.c dirangle_shear.c dir_rot.c shear_rot.c bicubic_intrp.c
	$(CC) $(CFLAGS) -o ../bin/$@ tc_identify.o dirangle_shear.o dir_rot.o shear_rot.o bicubic_intrp.o $(OBJECTS) $(LIBS)

tcident_flux: tc_identify_flux.c $(OBJECTS)
	$(CC) $(CFLAGS) -c tc_identify_flux.c
	$(CC) $(CFLAGS) -o ../bin/$@ tc_identify_flux.o $(OBJECTS) $(LIBS)

combine_reg: combine_reg.c
	$(CC) $(CFLAGS) -o ../bin/$@ combine_reg.c

reg_diff: reg_diff.c
	$(CC) $(CFLAGS) -o ../bin/$@ reg_diff.c

write_fields: write_fields.c
	$(CC) $(CFLAGS) -o ../bin/$@ write_fields.c read_tracks.o

diffke: diffke.c
	$(CC) $(CFLAGS) -o ../bin/$@ diffke.c read_tracks.o $(LIBS)
	
median: median.c binmedian.c
	$(CC) $(CFLAGS) -o ../bin/$@ median.c binmedian.c write_fields_netcdf.o $(LIBS)

reg_maxmin: reg_maxmin.c realloc_n.c
	$(CC) $(CFLAGS) -o ../bin/$@ reg_maxmin.c realloc_n.c read_tracks.o meantrd.o $(LIBS)
	
rad_dist: radial_dist.c realloc_n.c
	$(CC) $(CFLAGS) -o ../bin/$@ radial_dist.c realloc_n.c read_tracks.o meantrd.o $(LIBS)

diffvec: diff-shear.c realloc_n.c dirangle_shear.c read_tracks.c convert_track.c meantrd.c
	${CC} ${CFLAGS} -c diff-shear.c realloc_n.c dirangle_shear.c read_tracks.c convert_track.c meantrd.c
	$(CC) $(CFLAGS) -o ../bin/$@ diff-shear.o realloc_n.o dirangle_shear.o read_tracks.o convert_track.o meantrd.o $(LIBS)
