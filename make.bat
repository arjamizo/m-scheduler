g++ main.cpp -c -O3 -o main.o
g++ timer.cpp -o timer.o -c
g++ main.o timer.o -o prog.exe