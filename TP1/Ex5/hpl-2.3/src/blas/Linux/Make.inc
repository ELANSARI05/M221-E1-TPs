SHELL        = /bin/sh
CD           = cd
CP           = cp
LN_S         = ln -fs
MKDIR        = mkdir -p
RM           = /bin/rm -f
TOUCH        = touch

ARCH         = Linux

TOPdir       = $(HOME)/hpl-2.3
INCdir       = $(TOPdir)/include
BINdir       = $(TOPdir)/bin/$(ARCH)
LIBdir       = $(TOPdir)/lib/$(ARCH)

HPLlib       = $(LIBdir)/libhpl.a

MPdir        =
MPinc        =
MPlib        =

LAdir        =
LAinc        =
LAlib        = -lopenblas -lpthread -lm

F2CDEFS      = -DAdd__ -DF77_INTEGER=int -DStringSunStyle

HPL_INCLUDES = -I$(INCdir) -I$(INCdir)/$(ARCH) $(LAinc) $(MPinc)
HPL_LIBS     = $(HPLlib) $(LAlib) $(MPlib)

HPL_OPTS     = -DHPL_DETAILED_TIMING -DHPL_PROGRESS_REPORT
HPL_DEFS     = $(F2CDEFS) $(HPL_OPTS) $(HPL_INCLUDES)

CC       = mpicc
CCNOOPT  = $(HPL_DEFS)
OMP_DEFS = -fopenmp
CCFLAGS  = $(HPL_DEFS) -O3 -march=native -funroll-loops -fomit-frame-pointer -fPIC -Wall

LINKER       = $(CC)
LINKFLAGS    = $(CCFLAGS) $(OMP_DEFS)

ARCHIVER     = ar
ARFLAGS      = r
RANLIB       = ranlib
