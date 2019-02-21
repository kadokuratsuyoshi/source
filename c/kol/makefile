kol.exe : kol.cpp fft.o
fft.o : fft.cpp
	g++ -c fft.cpp -lm
	g++ -o kol.exe -lm kol.cpp fft.o
