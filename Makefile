all: lector

lector:
	gcc ReadersWriters.c -pthread
clean:
	rm -f *.out

