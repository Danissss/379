

default: compile

compile: 
	# gcc -o a2sdn a2sdn.c
	# g++ -o a2sdn a2sdn.cc
	g++ -o a2sdn a2sdn.cc

tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.html" ! -name "*.dat" ! -name "*.c" ! -name "*.cc" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 

test:
	gcc -o test test.c

fifo_remove:
	rm fifo_*

fifo:
	mkfifo fifo_0_1
	mkfifo fifo_1_1
	mkfifo fifo_2_1
	mkfifo fifo_3_1
	mkfifo fifo_4_1
	mkfifo fifo_5_1	
	mkfifo fifo_6_1
	mkfifo fifo_7_1
