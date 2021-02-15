/*
	program:	rand.c
	contents:	rand(), srand() test and check std-distribution
	make:		gcc rand.c -o rand -lm
	usage:		rand
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX (10)
#define NY (10)
#define SIZE (NX*NY)

int main(void)
{
	int i, j, k, cnt;
	double data[SIZE];
	double sum, ave, var, std, skew;
	double flat, flat_var, flat_std, flat_skew, dn, flat_a, flat_b;

	//srand((unsigned)time(NULL));
	k=0;
	cnt=0;
	sum=0;
	for (i=0; i<NX; i++) {
		for (j=0; j<NY; j++) {
			k=rand()%100+1;
			printf("%3d ", k );

			sum = sum + k;
			data[cnt] = k;	
			cnt++;
		}
		printf("\n");
	}
	ave = sum / (double)cnt;

	var=0.0;
	cnt=0;
	for (i=0; i<NX; i++) {
		for (j=0; j<NY; j++) {
			var +=  (data[cnt]-ave)*(data[cnt]-ave);
			cnt++;
		}
	}
	flat_var = var;
	flat_var = flat_var / (double)(cnt-1.0);
	flat_std = sqrt(flat_var);
	var = var / (double)(cnt);
	std = sqrt(var);

	skew=0.0;
	flat_skew=0.0;
	flat=0.0;
	cnt=0;
	for (i=0; i<NX; i++) {
		for (j=0; j<NY; j++) {
			skew += ((data[cnt]-ave)/std)
				*   ((data[cnt]-ave)/std)
				*   ((data[cnt]-ave)/std);
			flat_skew += ((data[cnt]-ave)/flat_std)
				*   ((data[cnt]-ave)/flat_std)
				*   ((data[cnt]-ave)/flat_std);
			flat += ((data[cnt]-ave)/flat_std)
				*   ((data[cnt]-ave)/flat_std)
				*   ((data[cnt]-ave)/flat_std)
				*   ((data[cnt]-ave)/flat_std);
			cnt++;
		}
	}
	dn=(double)cnt;
	flat_skew = (dn/((dn-1.0)*(dn-2.0)))*flat_skew;
	flat_a =(dn*(dn+1))/((dn-1.0)*(dn-2.0)*(dn-3.0));
	flat_b =(3.0*(dn-1.0)*(dn-1.0))/((dn-2.0)*(dn-3.0));
	flat = flat_a * flat - flat_b;
	printf("  SUM:%5.0f, \ 
 AVERAGE:%6.2f,\n \
 VAR.P  :%7.4f,\n \
 STDEV.P:%.9f\n \
 SKEW.P :%.9f,\n",
		sum, ave, var, std, skew);
	printf("\n \
 VAR.S  :%7.4f,\n \
 STDEV.S:%.9f\n \
 SKEW   :%.9f,\n \
 KURT   :%.9f\n",
		flat_var, flat_std, flat_skew, flat);
	return 0;
}
