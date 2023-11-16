#
# Makefile for smoopy and sphery
#
      CC = gcc

  RANLIB = ranlib

  INCGCC = $(GCCINC)
 ADDINCS = 
AD_C_FLAGS =

  ARFLAGS =

#Include for older gcc compilation
#INCLUDES = -I../include -I../../include -I$(GCCINC) -I/usr/include -nostdinc $(ADDINCS)
#
INCLUDES = -I../include -I../../include -I$(INCGCC) -I/usr/include $(ADDINCS)

#INCLUDES = -I../include -I../../include $(ADDINCS)

 LIBRARY = ../libsmp.a

#
# CFLAGS for gcc, add -Wno-unknown-pragmas for egcs or gcc-2.95
CFLAGS = -ansi -g -D$(MDEBUG) -D$(SYSTEM) -D$(USAPPEND) -D$(G77) $(INCLUDES) -Wall -Wpointer-arith -Wshadow -Wnested-externs $(AD_C_FLAGS)

#
# CFLAGS for SUN cc (ANSI)
# code does not work with SunC due to using adjustable arrays which are 
# a GNU C extension. These must be replaced with dynamic routines to work
# with SunC.
#CFLAGS = -c -g -D$(MDEBUG) -D$(SYSTEM) -D$(USAPPEND) $(INCLUDES) -I/usr/include -Xa $(AD_C_FLAGS) #-xc99 #-xO4


  FFLAGS = -c $(N_FFLAGS)


C_SOURCES = bsplin_c_.c \
            error_interp.c \
            grismo_c_.c \
            grisph_c_.c \
            smoopy_c.c \
            smoopy_setup.c \
            sphery_c.c \
            sphery_setup.c \
            sqgrid_c_.c


F_SOURCES = bisp.f \
            bsplin.f \
            bsplin_all.f \
            cossin.f \
            cytri1.f \
            cytri2.f \
            disco.f \
            grismo.f \
            grisph.f \
            nknot.f \
            optsph.f \
            ration.f \
            rotate.f \
            smoopy.f \
            sphery.f \
            sqgrid.f \
            symsys.f

C_OBJECTS = $(C_SOURCES:.c=.o)
F_OBJECTS = $(F_SOURCES:.f=.o)

all: $(LIBRARY)

.KEEP_STATE:

C_OBJS: $(C_OBJECTS)

F_OBJS: $(F_OBJECTS)

$(LIBRARY): C_OBJS F_OBJS FORCE
	rm -f $@
	ar $(ARFLAGS) rc $@  $(C_OBJECTS) $(F_OBJECTS)
	$(RANLIB) $@
#	rm -f $(C_OBJECTS) $(F_OBJECTS)

depend:
	makedepend -- $(CFLAGS) -- $(C_SOURCES)
	../../sfmakedepend $(F_SOURCES)

FORCE:


# DO NOT DELETE

bsplin_c_.o: ../../include/Stdio.h /usr/include/stdio.h
bsplin_c_.o: /usr/include/features.h /usr/include/stdc-predef.h
bsplin_c_.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
bsplin_c_.o: /usr/include/gnu/stubs.h
bsplin_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
bsplin_c_.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
bsplin_c_.o: /usr/include/libio.h /usr/include/_G_config.h
bsplin_c_.o: /usr/include/wchar.h
bsplin_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
bsplin_c_.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
error_interp.o: ../../include/Stdio.h /usr/include/stdio.h
error_interp.o: /usr/include/features.h /usr/include/stdc-predef.h
error_interp.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
error_interp.o: /usr/include/gnu/stubs.h
error_interp.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
error_interp.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
error_interp.o: /usr/include/libio.h /usr/include/_G_config.h
error_interp.o: /usr/include/wchar.h
error_interp.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
error_interp.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
error_interp.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
error_interp.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
error_interp.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
error_interp.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
error_interp.o: /usr/include/time.h /usr/include/sys/select.h
error_interp.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
error_interp.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
error_interp.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
error_interp.o: /usr/include/bits/stdlib-float.h
grismo_c_.o: ../../include/Stdio.h /usr/include/stdio.h
grismo_c_.o: /usr/include/features.h /usr/include/stdc-predef.h
grismo_c_.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
grismo_c_.o: /usr/include/gnu/stubs.h
grismo_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
grismo_c_.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
grismo_c_.o: /usr/include/libio.h /usr/include/_G_config.h
grismo_c_.o: /usr/include/wchar.h
grismo_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
grismo_c_.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
grismo_c_.o: ../include/reg_dat.h
grisph_c_.o: ../../include/Stdio.h /usr/include/stdio.h
grisph_c_.o: /usr/include/features.h /usr/include/stdc-predef.h
grisph_c_.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
grisph_c_.o: /usr/include/gnu/stubs.h
grisph_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
grisph_c_.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
grisph_c_.o: /usr/include/libio.h /usr/include/_G_config.h
grisph_c_.o: /usr/include/wchar.h
grisph_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
grisph_c_.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
grisph_c_.o: ../include/sphery_dat.h
smoopy_c.o: ../../include/Stdio.h /usr/include/stdio.h
smoopy_c.o: /usr/include/features.h /usr/include/stdc-predef.h
smoopy_c.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
smoopy_c.o: /usr/include/gnu/stubs.h
smoopy_c.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
smoopy_c.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
smoopy_c.o: /usr/include/libio.h /usr/include/_G_config.h
smoopy_c.o: /usr/include/wchar.h
smoopy_c.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
smoopy_c.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
smoopy_c.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
smoopy_c.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
smoopy_c.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
smoopy_c.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
smoopy_c.o: /usr/include/time.h /usr/include/sys/select.h
smoopy_c.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
smoopy_c.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
smoopy_c.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
smoopy_c.o: /usr/include/bits/stdlib-float.h ../include/reg_dat.h
smoopy_c.o: ../../include/bisp.h ../../include/grid.h ../../include/complex.h
smoopy_c.o: ../../include/mem_er.h
smoopy_setup.o: ../../include/Stdio.h /usr/include/stdio.h
smoopy_setup.o: /usr/include/features.h /usr/include/stdc-predef.h
smoopy_setup.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
smoopy_setup.o: /usr/include/gnu/stubs.h
smoopy_setup.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
smoopy_setup.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
smoopy_setup.o: /usr/include/libio.h /usr/include/_G_config.h
smoopy_setup.o: /usr/include/wchar.h
smoopy_setup.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
smoopy_setup.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
smoopy_setup.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
smoopy_setup.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
smoopy_setup.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
smoopy_setup.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
smoopy_setup.o: /usr/include/time.h /usr/include/sys/select.h
smoopy_setup.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
smoopy_setup.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
smoopy_setup.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
smoopy_setup.o: /usr/include/bits/stdlib-float.h ../include/reg_dat.h
smoopy_setup.o: ../../include/mem_er.h ../../include/grid.h
smoopy_setup.o: ../../include/complex.h
sphery_c.o: ../../include/Stdio.h /usr/include/stdio.h
sphery_c.o: /usr/include/features.h /usr/include/stdc-predef.h
sphery_c.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
sphery_c.o: /usr/include/gnu/stubs.h
sphery_c.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
sphery_c.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
sphery_c.o: /usr/include/libio.h /usr/include/_G_config.h
sphery_c.o: /usr/include/wchar.h
sphery_c.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
sphery_c.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sphery_c.o: ../include/sphery_dat.h ../../include/bisp.h ../../include/grid.h
sphery_c.o: ../../include/complex.h
sphery_setup.o: ../../include/Stdio.h /usr/include/stdio.h
sphery_setup.o: /usr/include/features.h /usr/include/stdc-predef.h
sphery_setup.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
sphery_setup.o: /usr/include/gnu/stubs.h
sphery_setup.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
sphery_setup.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
sphery_setup.o: /usr/include/libio.h /usr/include/_G_config.h
sphery_setup.o: /usr/include/wchar.h
sphery_setup.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
sphery_setup.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sphery_setup.o: ../../include/Math.h /usr/include/math.h
sphery_setup.o: /usr/include/bits/huge_val.h /usr/include/bits/huge_valf.h
sphery_setup.o: /usr/include/bits/huge_vall.h /usr/include/bits/inf.h
sphery_setup.o: /usr/include/bits/nan.h /usr/include/bits/mathdef.h
sphery_setup.o: /usr/include/bits/mathcalls.h /usr/include/stdlib.h
sphery_setup.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
sphery_setup.o: /usr/include/endian.h /usr/include/bits/endian.h
sphery_setup.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
sphery_setup.o: /usr/include/sys/types.h /usr/include/time.h
sphery_setup.o: /usr/include/sys/select.h /usr/include/bits/select.h
sphery_setup.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
sphery_setup.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
sphery_setup.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
sphery_setup.o: ../include/sphery_dat.h ../../include/mem_er.h
sphery_setup.o: ../../include/m_values.h ../../include/grid.h
sphery_setup.o: ../../include/complex.h
sqgrid_c_.o: ../../include/Stdio.h /usr/include/stdio.h
sqgrid_c_.o: /usr/include/features.h /usr/include/stdc-predef.h
sqgrid_c_.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
sqgrid_c_.o: /usr/include/gnu/stubs.h
sqgrid_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h
sqgrid_c_.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
sqgrid_c_.o: /usr/include/libio.h /usr/include/_G_config.h
sqgrid_c_.o: /usr/include/wchar.h
sqgrid_c_.o: /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h
sqgrid_c_.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sqgrid_c_.o: ../include/sphery_dat.h
