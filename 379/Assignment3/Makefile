

default: compile

compile: 
	# gcc -o a2sdn a2sdn.c
	# g++ -o a2sdn a2sdn.cc
	g++ -o a2sdn  a2sdn.cc
	# gcc -o client client.c
	# gcc -o server server.c
	# gcc -o poll_client poll_function_client.c
	# gcc -o poll_server poll_funcion.c

tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.html" ! -name "*.dat" ! -name "*.c" ! -name "*.cc" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 

test:
	gcc -o test test.c

fifo_remove:
	rm fifo_*

fifo:
	mkfifo fifo_1_0
	mkfifo fifo_1_1
	mkfifo fifo_1_2
	mkfifo fifo_1_3
	mkfifo fifo_1_4
	mkfifo fifo_1_5	
	mkfifo fifo_1_6
	mkfifo fifo_1_7
