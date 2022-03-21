CC = g++
CFLAGS = -Wall
DEBUG = -DDEBUG -g
VERSION = -std=c++11
OPT = -o3

all: DB

DB: SPARQL_engine.cpp query_parser.cpp Turtle_handler.cpp RDF_index.cpp HashTable.cpp  query_planner.cpp interface.cpp 
	$(CC) $(CFLAGS) $(VERSION) $(OPT) SPARQL_engine.cpp query_parser.cpp Turtle_handler.cpp RDF_index.cpp HashTable.cpp  query_planner.cpp interface.cpp -o DB.exe
	./DB.exe

clean:
	rm  DB.exe a.out
