#include "sdm.hpp"
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <sstream>

GetOpt::GetOpt(int argc, char ** argv){
	this->argc = argc;
	this->argv = argv;

	this->error = 0;
	
	this->learn = NULL;
	this->respond = NULL;
	this->input = NULL;
	this->output = NULL;
	this->modify = NULL;
	this->radius = -1;
	this->Init();
}

int GetOpt::Init(void){
	int id;
	while((id = getopt(argc, argv, "hR:l:r:m:i:o:")) != -1){
		switch(id){
			case 'h': // napoveda
				printer->PrintHelp();
				exit(0);
				break;
			case 'R': // polomer
				Radius(optarg);
				break;
			case 'l':
				this->learn= optarg; //  vystup - nauceni site
				break;
			case 'r':// naucena sit
				this->respond = optarg;
				break;
			case 'i':// vstupni soubor (obrazky)
				this->input = optarg;
				break;
			case 'm': // prevedeni do bin formy
				this->modify = optarg;
				break;
			case 'o': // soubor s prirazenymi dvojicemy (nazvy)
				this->output = optarg;
				break;
			default:
				std::cerr << "neplatny parametr"  << std::endl << std::endl;
				printer->PrintHelp();
				break;
		}
	}

	this->CheckOpt();
	return 0;
}

void GetOpt::Radius(char * radius){
	std::stringstream ss;
	std::string s = "";
	ss << radius;

	if(!(ss >> this->radius)) {
		std::cerr << "Chybne zadany parametr -R" << std::endl;
		exit(1);
	}
	
	ss >> s; // nema to prilepek?
	
	if(s != ""){
		std::cerr << "Chybne zadany parametr -R" << std::endl;
		exit(1);
	}
}

void GetOpt::Radius(int radius){
	this->radius = radius;
}

int GetOpt::GetRadius(void){
	return this->radius;
}

int GetOpt::GetECode(void){
	return this->error;
}

void GetOpt::CheckOpt(void){
	if(this->argc == 1){
		printer->PrintHelp();
		exit(0);
	} else if((this->modify != NULL && this->learn != NULL) ||
	          (this->modify != NULL && this->respond != NULL) ||
	          (this->learn != NULL && this->respond != NULL) ){
		std::cerr << "Chybne zadane parametry" << std::endl; 
		printer->PrintHelp();
		exit(0);
	} else if(this->modify != NULL && (this->input == NULL || this->radius >= 0)){
		std::cerr << "Chybne zadane parametry" << std::endl; 
		printer->PrintHelp();
		exit(0);
	} else if(this->learn != NULL && this->input == NULL){
		std::cerr << "Chybne zadane parametry" << std::endl; 
		printer->PrintHelp();
		exit(0);
	} else if(this->respond != NULL && (this->input == NULL || this->radius >= 0)){
		std::cerr << "Chybne zadane parametry" << std::endl; 
		printer->PrintHelp();
		exit(0);
	}  else if(this->learn != NULL && this->radius <= 0) {
		std::cerr << "Chybne zadane parametry" << std::endl; 
		printer->PrintHelp();
		exit(0);
	}
}

char * GetOpt::GetRespond(void){
	return this->respond;
}

char * GetOpt::GetLearn(void){
	return this->learn;
}

char * GetOpt::GetInput(void){
	return this->input;
}

char * GetOpt::GetOutput(void){
	return this->output;
}

char * GetOpt::GetModify(void){
	return this->modify;
}