

default: compile

compile: 
	# gcc -o a2sdn a2sdn.c
	g++ -o a2sdn a2sdn.cc

tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.c" ! -name "*.cc" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 

test:
	gcc -o test test.c

fifo_remove:
	rm fifo-*

fifo:
	touch fifo_0_1
	touch fifo_1_1
	touch fifo_2_1
	touch fifo_3_1
	touch fifo_4_1
	touch fifo_5_1	
	touch fifo_6_1
	touch fifo_7_1
