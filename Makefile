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

####################################
# Executed by simply typing "make" #
####################################
all: $(BDIR)/vpk0

$(BDIR)/vpk0: $(BINOBJ)
	$(CC) $(CFLAGS) $(OEXTRA) $(OLEVEL) -s -o $(BDIR)/vpk0 src/vpk0.c

.PHONY: clean

clean:
	rm -rf $(BDIR)/*
