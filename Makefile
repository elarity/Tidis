objects = main.o event/ev_factory.o protocol/protocol.o datastructure/hashtable.o
tidis : $(objects)
	gcc -o tidis $(objects)
main.o : main.c
	gcc -c main.c
ev_factory.o : ev_factory.c 
	gcc -c event/ev_factory.c
prootocol.o : prootocol.c
	gcc -c prootocol/prootocol.c
hashtable.o : hashtable.c
	gcc -c datastructure/hashtable.c
.PHONY : clean
clean :
	-rm $(objects)
#main.o : main.c defs.h
#    cc -c main.c
