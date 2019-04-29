all: myShell

myShell: myShell.o LineParser.o
	gcc -g -m32 -Wall -o myShell myShell.o LineParser.o 

myShell.o : myShell.c
	gcc -g -Wall -m32  -c -o myShell.o  myShell.c

LineParser.o : LineParser.c
	gcc -g -Wall -m32  -c -o LineParser.o  LineParser.c

.PHONY: clean

clean:
	rm -f *.o myShell