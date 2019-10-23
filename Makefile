objects = main.o event/ev_factory.o
tidis : $(objects)
	gcc -o tidis $(objects)
main.o : main.c
	gcc -c main.c
ev_factory.o : ev_factory.c 
	gcc -c event/ev_factory.c
#select.o : select.c
	#gcc -c event/select.c
.PHONY : clean
clean :
	-rm $(objects)
#main.o : main.c defs.h
#    cc -c main.c
