#BHEADER***********************************************************************
# (c) 1997   The Regents of the University of California
#
# See the file COPYRIGHT_and_DISCLAIMER for a complete copyright
# notice, contact person, and disclaimer.
#
# $Revision$
#EHEADER***********************************************************************

.SUFFIXES:
.SUFFIXES: .c .f .o

HEADERS =\
 general.h\
 headers.h\
 protos.h\
 smg.h\
 HYPRE_ls.h

FILES =\
 cyclic_reduction.c\
 general.c\
 smg2_setup_rap.c\
 smg3_setup_rap.c\
 smg.c\
 smg_intadd.c\
 smg_relax.c\
 smg_residual.c\
 smg_restrict.c\
 smg_setup.c\
 smg_setup_interp.c\
 smg_setup_rap.c\
 smg_setup_restrict.c\
 smg_solve.c\
 HYPRE_struct_smg.c\
 HYPRE_pcg.c\
 HYPRE_struct_pcg.c 

OBJS = ${FILES:.c=.o}

CC = cicc
F77 = ci77

# CFLAGS = -O
CFLAGS = -O -DHYPRE_TIMING
# CFLAGS = -g -DHYPRE_TIMING

FFLAGS = -O

LFLAGS =\
 -L/usr/local/lib\
 -L.\
 -L../struct_matrix_vector\
 -L../utilities\
 -lHYPRE_ls\
 -lHYPRE_mv\
 -lHYPRE_timing\
 -lHYPRE_memory\
 -lmpi\
 -lm


##################################################################
# Main rules
##################################################################

driver_internal: driver_internal.o libHYPRE_ls.a
	@echo  "Linking" $@ "... "
	${CC} -o driver_internal driver_internal.o ${LFLAGS}

driver_internal_cgsmg: driver_internal_cgsmg.o libHYPRE_ls.a
	@echo  "Linking" $@ "... "
	${CC} -o driver_internal_cgsmg driver_internal_cgsmg.o ${LFLAGS}

libHYPRE_ls.a: ${OBJS}
	@echo  "Building $@ ... "
	ar -ru $@ ${OBJS}
	ranlib $@

${OBJS}: ${HEADERS}

##################################################################
# Generic rules
##################################################################

.c.o:
	@echo "Making (c) " $@
	@${CC} -o $@ -c ${CFLAGS} $<

.f.${AMG_ARCH}.o:
	@echo "Making (f) " $@
	@${F77} -o $@ -c ${FFLAGS} $<

##################################################################
# Miscellaneous rules
##################################################################

veryclean: clean
	@rm -f libHYPRE_ls.a
	@rm -f driver driver_internal

clean:
	@rm -f *.o

