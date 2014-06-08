CC= gcc
LIBS= -L $(LIB) -lglut -lGL -lGLU -lm -ldelaunay
OPT= -O3 `sdl-config --cflags` -Wall -Wextra
SRC= src
OBJ= obj
INC= include
LIB= lib
BIN= bin
EXEC= Delaunay

all : directories $(EXEC)

directories:
	mkdir -p $(OBJ) 
	mkdir -p $(LIB)
	mkdir -p $(BIN)

#== Executable ==#

$(EXEC): $(LIB)/libdelaunay.a $(OBJ)/Main.o
	$(CC) -o $(BIN)/$@ $^ $(OPT) $(LIBS)

#== OBJET ==#

$(OBJ)/DestCons.o: $(SRC)/DestCons.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Flipalgo.o: $(SRC)/Flipalgo.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Graphique.o: $(SRC)/Graphique.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Liste.o: $(SRC)/Liste.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/TerrainImpl.o: $(SRC)/TerrainImpl.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Terran.o: $(SRC)/Terran.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Triangle.o: $(SRC)/Triangle.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Delaunay.o: $(SRC)/Delaunay.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Vecteur.o: $(SRC)/Vecteur.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

$(OBJ)/Main.o: $(SRC)/Main.c
	$(CC) -c $< -o $@ $(OPT) -I$(INC)

#== Bibliotheque ==#

$(LIB)/libdelaunay.a: $(OBJ)/DestCons.o $(OBJ)/Flipalgo.o $(OBJ)/Graphique.o $(OBJ)/Liste.o $(OBJ)/TerrainImpl.o $(OBJ)/Terran.o $(OBJ)/Triangle.o $(OBJ)/Delaunay.o $(OBJ)/Vecteur.o
	ar -rs $@ $^

.PHONY: all clean cleanall proper run

#== Nettoyage ==#

clean:
	rm -f $(OBJ)/* 
	rm -f $(BIN)/*
	rm -f $(LIB)/*

cleanall: clean
	rm -r $(OBJ) $(BIN) $(LIB)

proper: 
	rm -f $(INC)/*~
	rm -f $(SRC)/*~
	rm -f *~

#== Lancement ==#

run:
	./$(BIN)/$(EXEC)
