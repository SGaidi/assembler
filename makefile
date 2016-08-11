assembler: main.o convert.o first.o second.o label.o data.o instruction.o
	gcc -g -Wall -ansi -pedantic main.o convert.o first.o second.o label.o data.o instruction.o -o assembler
main.o: main.c assembler.h main.h
	gcc -g -Wall -ansi -pedantic main.c assembler.h main.h -c
convert.o: convert.c assembler.h
	gcc -g -Wall -ansi -pedantic convert.c assembler.h -c
first.o: first.c assembler.h instruction.h
	gcc -g -Wall -ansi -pedantic first.c assembler.h instruction.h -c
second.o: second.c assembler.h
	gcc -g -Wall -ansi -pedantic second.c assembler.h -c
label.o: label.c assembler.h label.h
	gcc -g -Wall -ansi -pedantic label.c assembler.h label.h -c
data.o: data.c assembler.h data.h
	gcc -g -Wall -ansi -pedantic data.c assembler.h data.h -c
instruction.o: instruction.c assembler.h instruction.h
	gcc -g -Wall -ansi -pedantic instruction.c assembler.h instruction.h -c
clean:
	rm assembler main.o convert.o first.o second.o label.o data.o instruction.o
