CFLAGS = -O3 -std=c23
LIB = -lcrypto

nsauth : nsauth.o
	$(CC) -o $@ nsauth.o ${LIB}
	help2man --no-info ./nsauth > manpage
clean:
	rm -f *.o
