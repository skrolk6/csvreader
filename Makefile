CC = g++

all: csvreader

csvreader: main.o CSVReader.o
	$(CC) main.o CSVReader.o -o csvreader

main.o: main.cpp
	$(CC) -c main.cpp
CSVReader.o: CSVReader.cpp
	$(CC) -c CSVReader.cpp
clean:
	rm -rf *o csvreader
