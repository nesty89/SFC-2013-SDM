#include "sdm.hpp"
#include <iostream>

Printer::Printer(){
	this->emax = 0;
}
void Printer::Init(){
	
}

void Printer::PrintHelp(void){
	std::cout << "Projekt: Prakticka uloha resena siti SDM" << std::endl;
	std::cout << "Rozpoznavani obrazku formatu *.bmp" << std::endl;
	std::cout << "Autor: Igor Pavlu, xpavlu06@stud.fit.vutbr.cz" << std::endl;
	std::cout << "Parametry:" << std::endl;
	std::cout << "-h ... napoveda" << std::endl;
	std::cout << std::endl;
	std::cout << "-l <soubor> -R <num> -i <soubor> [-o <soubor>] ... uceni site	-l soubor predstavuje soubor ulozeni naucene site, -i <soubor> vstupni soubor s binarni reprezentaci paru bmp soubor a odpovidajici popisek (10 znaku), kombinace -i soubor a -o soubor predstavuje soubor s odkazy na jednotlive obrazky (-i) a soubor se popisky (-o), -R predstavuje polomer uceni" << std::endl;
	std::cout << std::endl;
	std::cout << "-m <soubor> -i <soubor> [-o <soubor>] ... -m <soubor> urcuje, kam se ulozi binarni reprezentace bmp obrazku urcenych -i <soubor> nebo paru s popisky v pripade specifikace -o <soubor>" << std::endl;
	std::cout << std::endl;
	std::cout << "-r -i <soubor> ... odezva site na vlozeny vstup v souboru -i <soubor> ve forme binarni nebo jako odkazy na sobory " << std::endl;
}
void Printer::PrintError(int eid){
	if(eid != 0 && eid < emax){
		std::cerr << emsg[eid] << std::endl;
	}
}