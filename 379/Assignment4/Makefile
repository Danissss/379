

default: compile

compile: 
	# g++ -o a2sdn  a2sdn.cc
	g++ -o a4tasks  a4tasks.cc

eclass:
	gcc -o raceA raceA.c
	gcc -o raceB raceB.c
	gcc -o raceC raceC.c
	gcc -o raceD raceD.c
	gcc -o raceE raceE.c
clean_eclass:
	rm raceA
	rm raceB
	rm raceC
	rm raceD
	rm raceE

tar:
	tar -cvf submit.tar *

clean:
	find . -type f ! -name "*.html" ! -name "*.dat" ! -name "*.c" ! -name "*.cc" ! -name "*.o" ! -name "Makefile" -exec rm {} \; 
