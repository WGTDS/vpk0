####################
# C Compiler = GCC #
####################
CC=gcc

##################
# Compiler Flags #
##################
CFLAGS=-ansi -Wall -Wextra -pedantic -pedantic-errors

######################
# Optimization Level #
######################
OLEVEL=-O3

#######################
# Extra Optimizations #
#######################
OEXTRA=-fexpensive-optimizations -flto

####################################
# The following are subdirectories #
# of the current work directory.   #
####################################

###############################
# <Compiled Binary Directory> #
###############################
BDIR=bin

########################################
# <Header Include Directory>           #
# The "-I" flags specifies searching   #
# the immediate work directory for the #
# specified subdirectory.              #
########################################
IDIR=-Iinclude

####################################
# <Compiled Object Code Directory> #
####################################
ODIR=obj

_BINOBJ=utils.o lz.o encode.o decode.o vpk0.o
BINOBJ=$(patsubst %,$(ODIR)/%,$(_BINOBJ))

####################################
# Executed by simply typing "make" #
####################################
all: $(BDIR)/vpk0

$(ODIR)/%.o: src/%.c
	$(CC) $(CFLAGS) $(OLEVEL) $(IDIR) -c -o $@ $<

$(BDIR)/vpk0: $(BINOBJ)
	$(CC) $(CFLAGS) $(OEXTRA) $(OLEVEL) -s -o $(BDIR)/vpk0 $(ODIR)/*

.PHONY: clean

clean:
	rm -rf $(BDIR)/* $(ODIR)/* 
