#include <iostream>

using namespace std;

int main(void) {
    int i;
    double x, y;

    x = -5.0;
    for ( i=0; i<200; i++ ) {
        y = x * (x - 2.0) * (x - 2.0);
         cout << scientific << x << ' ' << y << endl;
        x += 0.1;
    }
    return 0;
}
