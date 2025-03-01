all:
	gcc -lm -c matrix.c
	gcc -c hw0201.c
	gcc matrix.o hw0201.o -o hw0201 -lm
	gcc hw0202.c -o hw0202
	gcc -c hw0204.c `pkg-config --cflags glib-2.0`
	gcc hw0204.o -o hw0204 `pkg-config --libs glib-2.0`
	gcc ContributionCalculate.c -o ContributionCalculate
	gcc hw0203.c -o hw0203
