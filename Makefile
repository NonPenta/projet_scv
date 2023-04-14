CFLAGS = -Wall -Wextra -g
EXEC = workfile main commit.o

all: $(EXEC)

main: gitlib.o main.o hashlib.o clist.o
	gcc -o $@ $(CFLAGS) $^
	
	
workfile: workfile.o hashlib.o
	gcc -o $@ $(CFLAGS) $^

clist.o: clist.c
	gcc $(CFLAGS) -c clist.c
	
gitlib.o: gitlib.c
	gcc $(CFLAGS) -c gitlib.c
	
hashlib.o: hashlib.c
	gcc $(CFLAGS) -c hashlib.c
	
main.o: main.c
	gcc $(CFLAGS) -c main.c
	
workfile.o: workfile.c
	gcc $(CFLAGS) -c workfile.c
	
commit.o: commit.c
	gcc $(CFLAGS) -c commit.c

clean:
	rm -f *.o *~ $(PROGRAMS)
