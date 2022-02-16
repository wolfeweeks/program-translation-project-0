P0: main.o tree.o node.h
	gcc main.o tree.o -o P0

main.o: main.c node.h
	gcc -c main.c

tree.o: tree.c tree.h node.h
	gcc -c tree.c
	
clean:
	rm -rf P0.exe *.o *.preorder *.inorder *.postorder