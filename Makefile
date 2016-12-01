object =  IRe.o ReMain.o 

all:$(object)
	g++ -o remain $(object)

IRe.o:IRe.h Re.h
ReMain.o:IRe.h
	
