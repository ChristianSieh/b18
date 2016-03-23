# Author: Christian Sieh

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = g++
# LIBS = -lpthread
FLAGS = -g -std=c++11 -Wall

# the build target executable:
TARGET = b18

all: $(TARGET) $(TARGET2) $(TARGET3)

# specific targets
b18:		b18.cpp
		$(CC) $(FLAGS) -o $@ $? $(LIBS) -lm

debugging:	b18.cpp
		$(CC) $(FLAGS) -DDEBUG -o b18 $? $(LIBS) -lm

# utility targets
clean:
	$(RM) $(TARGET) $(TARGET2) $(TARGET3) -f *.o *~
