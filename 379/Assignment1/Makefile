


default: compile

compile: 
	gcc -o a1jobs a1jobs.c
	#gcc -o a1mon a1mon.c

tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.c" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 


test:
	gcc -o test test.c