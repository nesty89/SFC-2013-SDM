NAME=sdm
COMPILER=g++

FILES=Printer.o File.o Neuron.o GetOpt.o sdm.o main.o 

ALL: $(FILES)
	$(COMPILER) -Wall -std=c++11 -o $(NAME) $(FILES)

clean:
	rm -f *.o $(NAME) *~ *.exe
