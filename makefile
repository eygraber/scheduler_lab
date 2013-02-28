scheduler : main.o Process.o Scheduler.o FCFSAlgorithm.o RRAlgorithm.o SRJFAlgorithm
	g++ -o linker main.o Process.o Scheduler.o FCFSAlgorithm.o RRAlgorithm.o SRJFAlgorithm

main.o : main.cpp Process.h Scheduler.h
	g++ -c main.cpp
Process.o : Process.cpp Process.h
	g++ -c Process.cpp
Scheduler.o : Scheduler.cpp Scheduler.h Process.h
	g++ -c Scheduler.cpp
FCFSAlgorithm.o : FCFSAlgorithm.cpp FCFSAlgorithm.h Scheduler.h
	g++ -c FCFSAlgorithm.cpp
RRAlgorithm.o : RRAlgorithm.cpp RRAlgorithm.h Scheduler.h
	g++ -c RRAlgorithm.cpp
SRJFAlgorithm.o : SRJFAlgorithm.cpp SRJFAlgorithm.h Scheduler.h
	g++ -c SRJFAlgorithm.cpp
clean : 
	rm scheduler main.o Process.o Scheduler.o FCFSAlgorithm.o RRAlgorithm.o SRJFAlgorithm.o