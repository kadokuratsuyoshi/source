/*
	program:	kurt.c
	contents:	test kurtosis for the standard distribution and, the uniform distribution
	make:		gcc kurt.c -o kurt -lm
	usage:		
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX (100)
#define NY (100)
#define SIZE (NX*NY)
#define DIST_SIZE (20)

double gaussrand(double m, double sgm)
{
	double u_0, u_1, v_0, v_1;

	u_0 = rand()/(1.0+RAND_MAX);
	u_1 = rand()/(1.0+RAND_MAX);
	v_0 = m + sgm * sqrt(-2*log(1-u_0))*cos(2*M_PI*u_1);
	v_1 = m + sgm * sqrt(-2*log(1-u_0))*sin(2*M_PI*u_1);
	return v_0;
}

int main(void)
{
	int i, j, cnt;
	double data[SIZE];
	double sum, ave, var, std, skew;
	double flat, flat_var, flat_std, flat_skew, dn, flat_a, flat_b;
	double x, y1, y2, min_y, max_y, dy, z;
	int dist[DIST_SIZE];
	double k;

	srand((unsigned)time(NULL));

	for (i=0; i<DIST_SIZE; i++ ) {
		dist[i]=0.0;
	}
	k=0;
	cnt=0;
	sum=0;
	min_y=100.0;
	max_y=0.0;
	for (i=0; i<NX; i++) {
		for (j=0; j<NY; j++) {
			//k=rand()%100+1; // uniform random number
			k=gaussrand(0.0, 1.0); // gaussian random number
			if (max_y < k) {
				max_y = k;
			}
			if (k < min_y) {
				min_y = k;
			}
			//printf("%6.2f ", k);;
			data[cnt] = k;	
			sum = sum + k;
			cnt++;
		}
		//printf("\n");
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
	printf("\n  SUM:%5.0f, \
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

	printf("max_y : %6.2f\n", max_y);
	dy = (max_y - min_y)/DIST_SIZE;
	y1 = min_y;
	y2 = y1+dy;
	z = 0.0;
	for (i=0; i<DIST_SIZE; i++) {
		for (j=0; j<SIZE; j++) {
			x = data[j];
			if ((y1<x) && (x<=y2)) {
				dist[i]++;
			}
		}
		y1 = y1 + dy;
		y2 = y2 + dy;
	}
	for (i=0; i<DIST_SIZE; i++) {
		j = dist[i]/20;
		printf("%*s %d\n", j, "", dist[i]);
	}
	return 0;
}
// eof
