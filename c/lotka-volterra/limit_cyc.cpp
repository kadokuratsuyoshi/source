// Lotka-Volterra equations, Euler-method
// to obtain limit cycle
#include <iostream>

using namespace std;

int main(void){
	int i = 0;
	double a = 8.0;
	double b = 3.0;
	double c = 4.0;
	double d = 18.0;
	double x, y;
	double dx, dy;
	double dx0, dy0;
	double dx1, dy1;
	double dxdt0, dydt0;
	double dxdt1, dydt1;
	double t;
	double dt;

	x = 10.0;	// prey
	y = 7.0;	// predator
	dx0 = 0.0;
	dy0 = 0.0;
	dx1 = 0.0;
	dy1 = 0.0;
	dt = 0.001;
	for (i=0; i<5000; i++ ) {
		//cout << t << ' ' << x << ' ' << y << endl;
		//cout << x << ' ' << y << endl;
		cout << x << ' ' << y << ' ' << t << endl;
		dxdt0 = a * x - b * x * y;
		dydt0 = c * x * y - d * y;
		dx0 = dxdt0 * dt;
		dy0 = dydt0 * dt;
		dxdt1 = a * (x+dx0) - b * (x+dx0) * (y+dy1);
		dydt1 = c * (x+dx0) * (y+dy1) - d * (y+dy1);
		dx1 = dxdt1 * dt;
		dy1 = dydt1 * dt;
		dx = (dx0 + dx1)/2.0;
		dy = (dy0 + dy1)/2.0;
        //limit cycle
		//x = x + dx;
		//y = y + dy;
        
        //no-limit cycle
		x = x + dx0;
		y = y + dy0;
		t = t + dt;
	}
	return 0;
}
