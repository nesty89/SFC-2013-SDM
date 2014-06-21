#include "sdm.hpp"
#include <cmath>
#include <cstdlib>

SDM::SDM(){

}

SDM::~SDM(){

}

int SDM::HammingDistance(std::vector<int> a, std::vector<int> b){
	int sum = 0;
	for(int i = 0; i < a.size(); i++){
		if(a[i] != b[i]) sum++;
	 // sum += std::abs(a[i] - b[i]);
	}
	return sum;
}

void SDM::Learn(void){
	if(setup->GetOutput() == NULL){ // mame soubor s binarne ulozenymi daty
		Loader *load = new Loader(setup->GetInput());
		load->LoadPairs(); 
	} else { // mame 2 soubory -- nutno nacist data
		this->Modify(0);
	}

	for(int i = 0; i < HN_MULT_CONST * I.size(); i++){
		Neuron n(0.0);
		Hidden.push_back(n);
	}

	for(int i = 0; i < 80; i++){
		Neuron n(0.0);
		Output.push_back(n);
	}
	
	for(int i = 0; i < Hidden.size(); i++){
		std::vector<int> v;
		for(int j = 0; j < I[0].size(); j++){
			v.push_back(0);
		}
		W_ri.push_back(v);
	}

	for(int i = 0; i < Output.size(); i++){
		std::vector<int> v;
		for(int j = 0; j < Hidden.size(); j++){
			v.push_back(0);
		}
		V_jr.push_back(v);
	}
	
	for(int r = 0; r < W_ri.size() ; r++){
	    for(int i = 0; i < W_ri[r].size(); i++){
			W_ri[r][i] = rand()%2;
		}
	}

  // vynuluje pocatecni vahy v_jr
	for(int j = 0; j < V_jr.size(); j++){
		for(int r = 0; r < V_jr[j].size(); r++){
		  V_jr[j][r] = 0;
		}
	}
	for(int p = 0; p < I.size(); p++){ // ukazatel na prvek trenovaci mnoziny (i,d)
		 int count = 0;
		for(int r = 0; r < W_ri.size(); r++){ // ukazatel na prvni neuron skryte vrstvy

			if(HammingDistance(I[p], W_ri[r]) <= setup->GetRadius()) { // upraveni vah neuronu vystupni vrstvy
			    count ++;
			    for(int j = 0; j < V_jr.size(); j++){
					V_jr[j][r] += ((2*D[p][j])-1);
			    }
			}
		}
		//std::cout<< count << std::endl;
	}

	// vypocet prahu neuronu vystupni vrstvy
	for (int j = 0; j < Output.size(); j++){
		float sum = 0.0;
		for (int r = 0; r < Hidden.size(); r++){
			sum += V_jr[j][r];
		}
		Output[j].Prah(sum/2);
	}
	// ulozime do souboru -l <FILE>
	Saver *save = new Saver(setup->GetLearn());
	save->SaveAll();
}

void SDM::Respond(void){
	int radius = setup->GetRadius();
	for(int i = 0; i < I.size(); i++){
		std::vector<int> o;
		for(int j = 0; j < Output.size(); j++){// vnitrni potencialy na 0
			Output[j].Potencial(0.0); 
		}
		for(int r = 0; r < V_jr.size(); r++){
			if(HammingDistance(I[i], W_ri[r]) <= radius){
				for (int j = 0; j < Output.size(); j++){
					Output[j].Potencial((Output[j].GetPotencial() + V_jr[j][r]));
				}
			}
		}

		for(int j = 0; j < Output.size(); j++){
			if(Output[j].GetPotencial() < Output[j].GetPrah()){
				o.push_back(0);
			} else {
				o.push_back(1);
			}
		}
		D.push_back(o);
	}
}

void SDM::ProcessResponds(void){
	// nacteni site
	Loader *load = new Loader(setup->GetRespond());
	load->LoadAll();

	// nacteni vstupnich mnozin
	File *fd = new File(setup->GetInput(), 1);
	std::string s = "";
	s = fd->ReadLine();
	Binary *b = new Binary();
	if(s == "INPUT"){
		/*while((s = fd->ReadLine()) != "FALSE"){
			b->StringToVector(s, I);
		}*/
		Loader *l = new Loader(setup->GetInput());
		l->LoadPairs();
	} else {
		this->Modify(0);
	}

	this->Respond();
	std::vector<std::string> out;
	for(int i = 0; i < D.size(); i++ ){
		std::string str = "";
		for(int j = 0; j < D[i].size(); j+=8){
			std::vector<int> v;
			for(int k = 0; k < 8; k ++){
				v.push_back(D[i][j+k]);
			}
			str += b->OutputDecode(v);
		}
		out.push_back(str);
	}
	for(int i = 0; i < out.size(); i++){
	//	std::cout << out[i].size() << " ";
		std::cout << out[i] << std::endl;
	}
}

void SDM::Modify(int saveFlag = 1){
	bool out = false;
	char *output;
	if((output = setup->GetOutput()) != NULL){
		out = true;
	}

	File *fd = new File(setup->GetInput(), 1);
	std::vector<std::string> files;
	std::string s = "";
	Binary *b = new Binary();
	// nacitani a kodovani souboru s obrazky
	while((s = fd->ReadLine()) != "FALSE"){
		std::string s2 = s;
		files.push_back(s2);
	}
	fd->Close();
	std::fstream fs;
	int a = 0;
	std::vector<int> tmp;
	int sss = 0;
	for (int i = 0; i < files.size(); i++){
		std::vector<int> tst;
		I.push_back(tst);
		fs.open (files[i].c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
		std::string s2 = "";
		while(getline(fs,s)){
		  s2 += s;
		}
		fs.close();
		int offset = s2[10] + s2[11] + s2[12] + s2[13];
		s2.erase(s2.begin(), s2.begin() + offset);
		for(int j = 0; j < s2.size(); j++){
			a = s2[j];			
			//if(a < 0){
				//a += 256;
			//}
			b->IntToBinary(a,tmp);
			for(int k = 0; k < tmp.size(); k++){
				I[i].push_back(tmp[k]);
			}
			tmp.clear();
		}
	}
	files.clear();
	// pokud je zadany i soubor s vystupnimy jmeny
	if(out){ 
		fd = new File(output, 1);
		std::vector<std::string> names;
		s = "";
		while((s = fd->ReadLine()) != "FALSE"){
			std::string s2 = s;
			names.push_back(s2);
		}
		fd->Close();
		for(int i = 0; i < names.size(); i++){
			int counter = 0;
			std::vector<int> tst;
			D.push_back(tst);
			if(names[i].size() > 10){
				std::cerr << "Maximalni delka jmena je 10 znaku" << std::endl;
				exit(0);
			}
			for(int j = 0; j < 80; j++){ // 10*8
				D[i].push_back(0);
			}

			for(int k = 0; k < names[i].size(); k++){
				a = names[i][k];
				//if(a < 0){
					//a += 256;
				//}
				b->IntToBinary(a,tmp);
				
			// vkladam do D odpredu -- counter bin. hodnoty jednotlivich znaku/cisel
				for(int j = 0; j < tmp.size(); j++){
					D[i][counter++] = tmp[j];
				}
				tmp.clear();
			}
		}
		names.clear();
	

		if(I.size() != D.size()) {
			std::cerr << "Nestejna delka radku vstupnich vektoru" << std::endl;
			exit(0);
		}
	}
	int len = I[0].size();
	for(int i = 0; i < I.size(); i++){
		if(I[i].size() !=  len){
			std::cerr << "Nestejna delka radku vystupnich vektoru" << std::endl;
			exit(0);
		}
	}
	// zapis do souboru s modifikaci
	if(saveFlag == 1){
		Saver *save = new Saver(setup->GetModify());
		save->SavePairs();
	}
}

void SDM::Run(void){
	if(setup->GetModify() != NULL){
		this->Modify(1);
	} else if(setup->GetLearn() != NULL){
		this->Learn();
	} else if(setup->GetRespond() != NULL){
		this->ProcessResponds();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// class binary
/////////////////////////////////////////////////////////////////////////////////

void Binary::IntToBinary(int ch, std::vector<int> &out){
  out.clear();
  for(int i = 0; i < 8; i++){
    out.push_back(0);
  }
  int counter = 7;
  while(ch > 0){
    if(ch % 2 == 1){
      out[counter] = 1;
    }
    ch /= 2;
    counter--;
  }
}

void Binary::StringToVector(std::string s, std::vector<std::vector<int> > &m){
  std::vector<int> line;
  for(int i = 0; i < s.size(); i++){
    std::vector<int> v;
    IntToBinary(s[i],v);
    for(int j = 0; j < v.size(); j++){
      line.push_back(v[j]);
    }
  }
  m.push_back(line);
}

char Binary::OutputDecode(std::vector<int> v){ // 1 bajt
  int sum = 0;
  if(v.size() > 8){
    std::cerr << "Retezec je prilis dlouhy" << std::endl;
    exit(0);
  }
  for(int i = 0; i < v.size(); i++){
    sum += bnum[i] * v[i];
  }
  return (char) sum;
}