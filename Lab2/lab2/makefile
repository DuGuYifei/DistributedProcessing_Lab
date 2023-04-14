static:
	gcc -c pqueue.c -o pqueue.o
	gcc -c main.c -o main.o
	ar r pqueue.a pqueue.o
	ar tv pqueue.a
	gcc main.o pqueue.a -o program

dynamic_lin:
	gcc -fPIC -c pqueue.c -o pqueue.o
	gcc -c main.c -o main.o
	gcc -shared -o libpqueue.so pqueue.o
	export LD_LIBRARY_PATH=.:"$LD_LIBRARY_PATH"
	gcc main.o -L. -lpqueue -o program

dynamic_win:
	gcc -fPIC -c pqueue.c -o pqueue.o
	gcc -c main.c -o main.o
	gcc -shared -o libpqueue.dll pqueue.o
	set PATH=%PATH%;.
	gcc main.o -L. -lpqueue -o program