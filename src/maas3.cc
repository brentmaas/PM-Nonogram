/**
 * Auteur: Brent Maas (s1826247)
 * Bestand: maas3.cc
 * Functie: 
 * Getest op: Windows 10 + MSys2 (MinGW64) met g++ 8.2.0
 * Getest met: g++ -o Assignment3 maas3.cc -std=c++17 -Wall -Wextra
 * Laatst bewerkt: 18/10/2018
 */

#include <iostream>
#include <string>

//Voert het infoblok uit naar de standaarduitvoer
void infoBlokje(){
	std::cout << "/////////////////// INFOBLOK ///////////////////\n"
	"Naam: Brent Maas (s1826247)\n"
	"Jaar van aankomst: 2016\n"
	"Studierichting: Natuur- & Sterrenkunde + minor Data Science\n"
	"Studentnummer: s1826247\n"
	"Opgave: Nonogram\n"
	"\n"
	"\n"
	"Laatst bewerkt: 18/10/2018\n"
	"////////////////////////////////////////////////\n" << std::endl;
}

int main(){
	//Print het infoblok
	infoBlokje();
	
	return 0;
}