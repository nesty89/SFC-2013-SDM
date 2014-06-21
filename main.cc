#include <iostream>
//#include <fstream>
// some libraris
#include <cstdlib>
//#include <getopt.h>

// some cpp headers
#include "sdm.hpp"
#include <fstream>
#include <cstring>

GetOpt *setup;
Printer *printer = new Printer();

std::vector<Neuron> Hidden;
std::vector<Neuron> Output;

std::vector< std::vector<int> > W_ri;
std::vector< std::vector<int> > V_jr;

std::vector<std::vector<int> > I;
std::vector<std::vector<int> > D;

int bnum[] = {128, 64, 32, 16, 8, 4, 2, 1};

int main(int argc, char **argv) {
  setup = new GetOpt(argc, argv);
  SDM *sdm = new SDM();
  sdm->Run();
}

//http://paulbourke.net/dataformats/bmp/