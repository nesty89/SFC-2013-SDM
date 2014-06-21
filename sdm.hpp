#ifndef SDM_HPP
#define SDM_HPP

#include <iostream>
#include <vector>
#include <fstream>

// multiplikativni konstanta pro pocet skrytych neuronu HN_MULT_CONST * pocet vstupnich vektoru prepocitat,
// aby to nedelal praseciny
#define HN_MULT_CONST 20

class GetOpt{
	private:
		// input info
		int argc;
		char ** argv;

		// flags and variables
		int error;

		char *learn;
		char *respond;
		char *input;
		char *output;
		char *modify;

		int radius;
		

		// methods
		int Init(void);
		void Radius(char * radius);

	public:

		GetOpt(int argc, char ** argv);
		~GetOpt();
		void Radius(int radius);
		int GetRadius(void);
		int GetECode(void);
		char * GetRespond(void);
		char * GetLearn(void);
		char * GetInput(void);
		char * GetModify(void);
		char * GetOutput(void);
		void CheckOpt(void);
};

class Printer{
	private:
		std::vector<std::string> emsg;
		int emax;
		
		void Init();

	public:
		Printer();
		~Printer();

		void PrintHelp(void);
		void PrintError(int eid);
};

class File{ // otevreni zapis/cteni dat souboru
	private:
		char * name;
		std::fstream stream;
		std::vector<std::string> filecontent; // obsah vstupniho souboru
		int row; // cislo aktualniho radku
		int type; // typ nacitani 1 - vstup, 2 - vystup

		int error;
		void Init(void);
	public:
		File(char * name, int type);
		~File();
		//File(File&);
		//void operator = (File&);
		void ReadFile(void);
		std::string ReadLine(void);
		void WriteLine(std::string line);
		int GetECode(void);
		void Close(void);

};

class SDM{
	private:
		int HammingDistance(std::vector<int> a, std::vector<int> b);
	public:
		SDM();
		~SDM();
		void Run(void);
		void Modify(int saveFlag);
		void Learn(void);
		//std::vector<int> Respond(std::vector<int> input);
		void Respond(void);
		void ProcessResponds(void);
};

class Neuron{ // zakladni kostra neuronu
	private:
		float prah;
		float potencial;
	public:
		Neuron(float prah);
		~Neuron();
		void Prah(float prah);
		float GetPrah(void);
		void Potencial(float potencial);
		float GetPotencial(void);
};

class Loader{ // nahrani dat ze souboru -- naucena sit
	private:
		char *fname;
		int error;
		File *file;

		void SSclear(std::stringstream &ss);
		void Init(void);

	public:
		Loader(char *fname);
		~Loader();
		void LoadRadius(void);
		void LoadMatrix(std::vector<std::vector<int> > &m, int r, int c);
		void LoadNeuron(std::vector<Neuron> &v, int vecSize);
		void LoadAll(void);
		void LoadPairs(void);
};



class Saver{ // ukladani dat do souboru -- zapamatovani si naucenych veci
	private:
		char* fname;
		int error;
		File *file;

		void SSclear(std::stringstream &ss);
		void Init(void);
	public:
		Saver(char* fname);
		~Saver();
		void SaveRadius(void);
		void SaveMatrix(std::vector< std::vector<int> > &m, std::string s);
		void SaveNeuron(std::vector< Neuron> &n, std::string s);
		void SaveAll(void);
		void SavePairs(void);
};

class Binary{
	public:
		void IntToBinary(int ch, std::vector<int> &out);
		void StringToVector(std::string s, std::vector<std::vector<int> > &m);
		char OutputDecode(std::vector<int> v);
};





// some shared variables
extern GetOpt *setup;
extern Printer *printer;

extern std::vector<Neuron> Hidden;
extern std::vector<Neuron> Output;

extern int bnum[];

extern std::vector< std::vector<int> > W_ri;
extern std::vector< std::vector<int> > V_jr;

extern std::vector<std::vector<int> > I;
extern std::vector<std::vector<int> > D;
#endif