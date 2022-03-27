CC = g++
CFLAGS = -Wall
DEBUG = -DDEBUG -g
VERSION = -std=c++11
OPT = -O3
SRC = RDF_DB/

all: DB

DB: $(SRC)SPARQL_engine.cpp $(SRC)query_parser.cpp $(SRC)Turtle_handler.cpp $(SRC)RDF_index.cpp $(SRC)query_planner.cpp $(SRC)interface.cpp 
	$(CC) $(CFLAGS) $(VERSION) $(OPT) $(SRC)SPARQL_engine.cpp $(SRC)query_parser.cpp $(SRC)Turtle_handler.cpp $(SRC)RDF_index.cpp  $(SRC)query_planner.cpp $(SRC)interface.cpp main.cpp -o RDF_DB.out
	./RDF_DB.out
	
TEST: $(SRC)SPARQL_engine.cpp $(SRC)query_parser.cpp $(SRC)Turtle_handler.cpp $(SRC)RDF_index.cpp $(SRC)query_planner.cpp experiment.cpp $(SRC)interface.cpp
	$(CC) $(CFLAGS) $(VERSION) $(OPT) $(SRC)SPARQL_engine.cpp $(SRC)query_parser.cpp $(SRC)Turtle_handler.cpp $(SRC)RDF_index.cpp  $(SRC)query_planner.cpp experiment.cpp $(SRC)interface.cpp -o TEST.out
	./TEST.out

clean:
	rm  RDF_DB.out a.out TEST.out
