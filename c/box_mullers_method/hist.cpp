/*
    filename:   hist.cpp
    contents:   output histgram
    author:     T.Kadokura
    date:       2019.1.25
    make:       >g++ -o hist.exe hist.cpp
    usage:      >./hist.exe input_data
*/
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
	int fcnt, n, i;
	double x, ex, y, ey, dist[32];
	double total, ave;
	char fname[32];
	ifstream fin;
	ofstream fout;

    if ( argc != 2 ) {
        cerr << "usage: ./ave.exe input_data" << endl;
        return 1;
    }
    sprintf(fname, argv[1] );
	fin.open(fname);
	if ( fin.fail() ) {
		cerr << "can not open " << fname << endl;
		return 1;
	}

	for (i=0; i<32; i++ ) {
		dist[i] = 0.0;
	}
    total = 0.0;
	while(true) {
		fin >> n >> x >> y;
        //printf("%d %f %f\n", n, x, y);
        //exit(1);
        ex = (y * y)*32.0;
        if ( ex < 1.0 ) {
            dist[0]++;
        }
        else if ( ex < 2.0 ) {
            dist[1]++;
        }
        else if ( ex < 3.0 ) {
            dist[2]++;
        }
        else if ( ex < 4.0 ) {
            dist[3]++;
        }
        else if ( ex < 5.0 ) {
            dist[4]++;
        }
        else if ( ex < 6.0 ) {
            dist[5]++;
        }
        else if ( ex < 7.0 ) {
            dist[6]++;
        }
        else if ( ex < 8.0 ) {
            dist[7]++;
        }
        else if ( ex < 9.0 ) {
            dist[8]++;
        }
        else if ( ex < 10.0 ) {
            dist[9]++;
        }
        else if ( ex < 11.0 ) {
            dist[10]++;
        }
        else if ( ex < 12.0 ) {
            dist[11]++;
        }
        else if ( ex < 13.0 ) {
            dist[12]++;
        }
        else if ( ex < 14.0 ) {
            dist[13]++;
        }
        else if ( ex < 15.0 ) {
            dist[14]++;
        }
        else if ( ex < 16.0 ) {
            dist[15]++;
        }
        else if ( ex < 17.0 ) {
            dist[16]++;
        }
        else if ( ex < 18.0 ) {
            dist[17]++;
        }
        else if ( ex < 19.0 ) {
            dist[18]++;
        }
        else if ( ex < 20.0 ) {
            dist[19]++;
        }
        else if ( ex < 21.0 ) {
            dist[20]++;
        }
        else if ( ex < 22.0 ) {
            dist[21]++;
        }
        else if ( ex < 23.0 ) {
            dist[22]++;
        }
        else if ( ex < 24.0 ) {
            dist[23]++;
        }
        else if ( ex < 25.0 ) {
            dist[24]++;
        }
        else if ( ex < 26.0 ) {
            dist[25]++;
        }
        else if ( ex < 27.0 ) {
            dist[26]++;
        }
        else if ( ex < 28.0 ) {
            dist[27]++;
        }
        else if ( ex < 29.0 ) {
            dist[28]++;
        }
        else if ( ex < 30.0 ) {
            dist[29]++;
        }
        else if ( ex < 31.0 ) {
            dist[30]++;
        }
        else if ( ex <= 32.0 ) {
            dist[31]++;
        }
		total = total + ex;
		if ( fin.fail() ) {
			break;
		}
	}
	fin.close();
	for ( i=0; i<32; i++ ) {
		printf("%d %f\n", i, dist[i]/dist[0] );
	}
//    printf("%d \n", total );
	return 0;
}
