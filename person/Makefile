objects = Person.o RealPerson.o

CC = gcc
XX = g++

CXXFLAGS += -Wall -O3 -std=c++0x -save-temps
LIB = libperson.so
MY_LIB_NAME = -lperson
MY_ROOT = .
MY_LIB_PATH = -L $(MY_ROOT)

all:lib main.cpp
	$(XX) -o person main.cpp ${CXXFLAGS} ${MY_LIB_PATH} ${MY_LIB_NAME}

lib:${LIB}

Person.o:Person.h
	${XX}  ${CXXFLAGS} -c Person.cpp -fpic
${LIB}:${objects}
	${XX} -shared -o $@ ${objects}
	sudo cp ${LIB} /usr/lib

# main.o:Person.h Date.h Address.h
# 	${XX} -c main.cpp

RealPerson.o:RealPerson.h
	${XX} -c RealPerson.cpp -fpic

.PHONY:clean
clean:
	rm person $(objects) ${LIB}
