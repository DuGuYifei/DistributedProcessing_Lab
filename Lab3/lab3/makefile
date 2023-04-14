program: main.o
	gcc *.o -o $@
	
%.o: %.c
	gcc -c $< -o $@

clean:
	del *.o program*

cleano:
	del *.o

cleanol:
	rm -f *.o