objects = main.o HanFeiziObservable.o LiSiObserver.o ZhouSiObserver.o

CC = gcc
XX = g++

CXXFLAGS += -std=c++0x 

all:$(objects)
	$(XX) -o observer  $(objects) 


.PHONY:clean
clean:
	rm observer $(objects)


	 
