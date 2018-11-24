

default: compile

compile: 
	# g++ -o a2sdn  a2sdn.cc
	g++ -o a2tasks  a2tasks.cc

tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.html" ! -name "*.dat" ! -name "*.c" ! -name "*.cc" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 
