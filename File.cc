#include "sdm.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
//#include <cstring>
#include <string>

/////////////////////////////////////////////////////////////////////////////
// File class
/////////////////////////////////////////////////////////////////////////////
File::File(char * name, int type) {// : stream(name)
	this->name = name;
	this->type = type;
	
	this->error = 0;
	this->row = 0;
	this->Init();
}

void File::Init(void){
	if(this->name == ""){
		std::cerr << "Nebyl zadan nazev souboru" << std::endl;
		exit(0);
	}

	if(this->type == 1){
		this->stream.open(this->name,std::fstream::in);
		this->ReadFile();
	} else if(this->type == 2){
		this->stream.open(this->name, std::fstream::out);
	} 
}

int File::GetECode(void){
	return this->error;
}

void File::ReadFile(void){	
	if(this->stream.good()){
		while(!this->stream.eof()){
			std::string s ="";
			std::getline(this->stream, s);
			this->filecontent.push_back(s);
		}
	}
	if(!this->stream.eof()) {
		std::cerr << "Chyba pri cteni souboru" << std::endl;
		exit(0);
	} 
	this->Close();
}

std::string File::ReadLine(void){
	if(this->filecontent.size() > this->row) {
		return this->filecontent[this->row++];
	} else return "FALSE";
}

void File::WriteLine(std::string line){
	
	if(!this->stream.good()) {
		std::cerr << "Chyba pri zapisu do souboru" << std::endl;
		exit(0);
		return;
	}
	this->stream << line << std::endl;
}

void File::Close(void){
	if(this->stream.is_open()){
		this->stream.close();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Loader class
/////////////////////////////////////////////////////////////////////////////
Loader::Loader(char *fname){
	this->fname = fname;
	this->Init();
}

void Loader::SSclear(std::stringstream &ss){
	ss.str("");
	ss.seekg(0);
	ss.clear();
}

void Loader::Init(){
	this->file = new File(this->fname, 1);
	// ecode?
}

void Loader::LoadRadius(void){
	std::stringstream ss;
	std::string s = "";
	s = this->file->ReadLine();
	if(s == "FALSE") {
		std::cerr << "chyba nacteni polomeru"<<std::endl;
	}
	ss << s;
	s = "";
	int num;
	if(!(ss >> num)) {
		std::cerr << "chyba nacteni polomeru"<<std::endl;
	}
	setup->Radius(num);
	ss >> s; // nema to prilepek?
	
	if(s != ""){
		std::cerr << "chyba nacteni polomeru - neco navic"<<std::endl;
	}
}

void Loader::LoadMatrix(std::vector<std::vector<int> > &m, int r, int c){
	std::stringstream ss;
	int num;
	std::string s = "";
	for(int i = 0; i < r; i++){
		std::vector<int> v;
		SSclear(ss);
		s = this->file->ReadLine();
		if(s == "FALSE") {
			std::cerr << "Chyba v matici k nacteni" << std::endl; 
		}
		ss << s;
		for(int j = 0; j < c; j++){
			if(!(ss >> num)) { 
				std::cerr << "Chyba v matici k nacteni" << std::endl; 
			}
			v.push_back(num);
		}
		m.push_back(v);
	}
}

void Loader::LoadNeuron(std::vector<Neuron> &v, int vecSize){
	float fnum = 0.0;
	std::stringstream ss;
	std::string s;
	if(vecSize > 0){
		s = this->file->ReadLine();
		if(s == "FALSE") {
			std::cerr << "1chyba ve formatu dokumentu" << std::endl;
		}
		ss << s;
	}
	for(int i = 0; i < vecSize; i++){
		ss >> fnum;
		Neuron n(fnum);
		v.push_back(n);
		fnum = 0.0;
	}
}

void Loader::LoadAll(void){
	std::string s = "";
	std::stringstream ss;
	while((s = this->file->ReadLine()) != "FALSE"){
		SSclear(ss);
		if((s == "W_ri") || (s == "V_jr")){

			std::string name = s;
			s = "";
			s = this->file->ReadLine();
			if(s == "FALSE") {
				std::cerr << "chyba ve formatu dokumentu" << std::endl;
			}
			ss << s;
			int c = 0, r = 0;
			if(!(ss >> r)){
				std::cerr << "chyba ve formatu dokumentu" << std::endl;
			}

			if(!(ss >> c)){
				std::cerr << "chyba ve formatu dokumentu" << std::endl;
				exit(0);
			}
			if(name == "V_jr"){
				LoadMatrix(V_jr, r, c);
			} else if(name == "W_ri"){	
				LoadMatrix(W_ri, r, c);
			}
		} else if(s == "R"){
			LoadRadius();
		} else if((s == "Hidden") || (s == "Output")){ // TODO NEURONY
			std::string name = s;
			s = "";
			s = this->file->ReadLine();
			
			if(s == "FALSE") {
				std::cerr << "chyba ve formatu dokumentu" << std::endl;
				exit(0);
			}
			ss << s;
			int size = 0;

			if(!(ss >> size)){

				std::cerr << "chyba ve formatu dokumentu" << std::endl;
				
				exit(0);
			}
			if(name == "Hidden"){
				LoadNeuron(Hidden, size);
			}
			else if(name == "Output"){
				LoadNeuron(Output, size);
			}
		} else if(s == ""){
			
		} else {
			std::cout << s << std::endl;
			std::cerr << "chyba ve formatu dokumentu" << std::endl;
			exit(0);
		}
	}
}


void Loader::LoadPairs(void){ // nacita binarne zakodovane I pripadne D -- vstup/vystup
	std::string s = "";
	int x = -1;
	int counter = 0;
	bool flag = false;
	Binary *b = new Binary();
	while((s = this->file->ReadLine()) != "FALSE"){
		if(s == "") {
			continue;
		}
		if( s == "INPUT"){
			x = 0;
			continue;
		} else if (s == "OUTPUT"){
			x = 1;
			flag = true;
			continue;
		}
		if(x == 0) {
		//	b->StringToVector(s, I);
			std::vector<int> tmp; 
			for(int i = 0; i < s.size(); i++){
				if(s[i] == '0'){
					tmp.push_back(0);
				} else if(s[i] == '1'){
					tmp.push_back(1);
				}
			}
			I.push_back(tmp);
		} else if( x == 1){
			//b->StringToVector(s, D);
			
			std::vector<int> tmp; 
			for(int i = 0; i < s.size(); i++){
				if(s[i] == '0'){
					tmp.push_back(0);
				} else if(s[i] == '1'){
					tmp.push_back(1);
				}
			}
			D.push_back(tmp);
		} else {
			std::cerr<< "problem ve formatu souboru" << std::endl;
			exit(0);
		}
	}
	if(setup->GetOutput() != NULL && (D.size() != I.size())){
		std::cerr << "pocet vstupnich dvojic neodpovida" << std::endl;
		exit(0);
	}

	int len = I[0].size();
	for(int i = 0; i < I.size(); i++){
	    if(I[i].size() !=  len) {
	   		std::cerr << "nestejna delka radku vstupnich vektoru" << std::endl;
			exit(0);
		}
	}

	if(flag){
		len = D[0].size();

		for(int i = 0; i < D.size(); i++){
		 	if(D[i].size() !=  len) {
		  		std::cerr << "nestejna delka radku vystupnich vektoru" << std::endl;
				exit(0);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Saver class
/////////////////////////////////////////////////////////////////////////////
Saver::Saver(char* fname){
	this->fname = fname;
	this->Init();
}

void Saver::Init(){
	this->file = new File(this->fname, 2);
	// ecode?

}

void Saver::SSclear(std::stringstream &ss){
	ss.str("");
	ss.seekg(0);
	ss.clear();
}

void Saver::SaveRadius(void){
	std::stringstream ss;
	ss << "R";
	this->file->WriteLine(ss.str());
	SSclear(ss);
	ss << setup->GetRadius();
	this->file->WriteLine(ss.str());
}

void Saver::SaveMatrix(std::vector< std::vector<int> > &m, std::string s){
	std::stringstream ss;
	ss << s;
	this->file->WriteLine(ss.str());
	SSclear(ss);
	int r = 0, c = 0;
	r = m.size();
	if(r > 0) {
		c = m[0].size();
	}
	ss << r << " " << c;
	this->file->WriteLine(ss.str());
	SSclear(ss);
	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			ss << m[i][j] << " ";
		}
		this->file->WriteLine(ss.str());
		SSclear(ss);
	}

}

void Saver::SaveNeuron(std::vector< Neuron> &n, std::string s){
	// neuron num
	std::stringstream ss;
	ss << s;
	this->file->WriteLine(ss.str());
	SSclear(ss);
	int size = n.size();
	ss << size;
	this->file->WriteLine(ss.str());
	if(size == 0) return;
	SSclear(ss);
	for(int i = 0; i < size; i++){
		ss << n[i].GetPrah() << " ";
	}
	this->file->WriteLine(ss.str());
	SSclear(ss);
}

void Saver::SaveAll(void){
	// radius
	SaveRadius();
	// Hidden neurons
	SaveNeuron(Hidden,  "Hidden");
	// Output neurons
	SaveNeuron(Output,  "Output");
	// W_ri
	SaveMatrix(W_ri, "W_ri");
	// V_jr
	SaveMatrix(V_jr, "V_jr");
	
	this->file->Close();

}

void Saver::SavePairs(void){
	// zapis
    std::string out = "INPUT";
    this->file->WriteLine(out);
    
    out = "";
    
    for(int i = 0; i < I.size(); i++){
	    for(int j = 0; j < I[i].size(); j++){
	      if(I[i][j] == 0) {
	        out += "0";
	      } else{
	        out += "1";
	      }
	    }
	    this->file->WriteLine(out);
	    out = "";
	}
	if(setup->GetOutput() != NULL){
		out = "OUTPUT";
	    this->file->WriteLine(out);
	    
	    out = "";
	    
	    for(int i = 0; i < D.size(); i++){
		    for(int j = 0; j < D[i].size(); j++){
		      if(D[i][j] == 0) {
		        out += "0";
		      } else {
		        out += "1";
		      }
		    }
		    this->file->WriteLine(out);
		    out = "";
		}
	}
	this->file->Close();
}
