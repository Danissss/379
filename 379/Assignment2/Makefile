

default: compile

compile: 
	# gcc -o a2sdn a2sdn.c
	g++ -o a2sdn a2sdn.cc

tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.c" ! -name "*.cc" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 
	rm fifo
test:
	gcc -o test test.c

fifo:
	touch fifo-0-1
	touch fifo-1-0	
	touch fifo-2-0	
	touch fifo-3-0	
	touch fifo-4-0	
	touch fifo-5-0	
	touch fifo-6-0	
	touch fifo-7-0	
