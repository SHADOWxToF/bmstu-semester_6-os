all: p1 p2 p3

p1: p1_one p1_two

p2: p2_one p2_two

p3: p3_one p3_two

p1_one: p1/one.c
	gcc p1/one.c -o p1_one.out

p1_two: p1/two.c
	gcc -pthread p1/two.c -o p1_two.out

p2_one: p2/one.c
	gcc p2/one.c -o p2_one.out

p2_two: p2/two.c
	gcc -pthread p2/two.c -o p2_two.out

p3_one: p3/one.c
	gcc p3/one.c -o p3_one.out

p3_two: p3/two.c
	gcc -pthread p3/two.c -o p3_two.out
