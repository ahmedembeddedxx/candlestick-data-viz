
main: main.o candlestick.o
	g++ main.o candlestick.o -o main
	.\main

main.o: main.cpp candlestick.h
	g++ -c main.cpp -o main.o

candlestick.o: candlestick.cpp candlestick.h
	g++ -c candlestick.cpp -o candlestick.o


clean:
	rm -f main.o candlestick.o main


