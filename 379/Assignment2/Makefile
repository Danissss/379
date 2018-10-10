

default: compile

compile: 
	# gcc -o a2sdn a2sdn.c
	g++ -o a2sdn a2sdn.cc


tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.c" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 


test:
	gcc -o test test.c