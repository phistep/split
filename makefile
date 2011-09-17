split: main.o
	gcc main.o -o split

main.o: main.c
	gcc -c main.c

install:
	@cp ./split /usr/bin && echo "\"split\" successfully installed in \"/usr/bin\""

clean:
	@rm -rf split main.o && echo "\"split\" and \"main.o\" successfully deleted"
