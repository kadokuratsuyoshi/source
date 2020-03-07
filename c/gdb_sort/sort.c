/*
 	filename:	sort.c
	contents:	sort example for gdb
	make:		gcc -g -O0 sort.c -o sort.exe
	gdb inst.
	>gdb sort.exe
	>list 0 (list program center 0)
	>break main
	>run
	>step
	>print a[i]
	>list 20 (list program center 20)
	>break swap
	>info breakpoints
	>print *a
	>print *b
	>c
	>print *a
	>print *b
	>c
	>delete 2
	>c
*/
#include <stdio.h>

int a[] = {3, 1, 2, 8, 5, 7, 6};

void printa(void){
	int i;
	int n = sizeof a / sizeof a[0];

	printf("a :");
	for (i=0; i<n; i++) {
		printf("%d, ", a[i]);
	}
	printf("\n");
}

void swap(int *a, int *b){
	int w;

	w = *a;
	*a = *b;
	*b = w;
}

int main(void) {
	int i, j;
	int n = sizeof a / sizeof a[0];
	
	printa();
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (a[i] < a[j]) {
				swap(&a[i], &a[j]);
			}
		}
	}
	printa();
	return 0;
}
