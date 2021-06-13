CC=gcc
nn:	nn.c
	$(CC) -O3 -o nn nn.c -lm
forest: forest.c
	$(CC) -o forest forest.c -lm
treetest: treetest.c
	$(CC) -o treetest treetest.c -lm
pointertest: pointertest.c
	$(CC) -o pointertest pointertest.c -lm
clean:
	rm *.o nn forest treetest pointertest
