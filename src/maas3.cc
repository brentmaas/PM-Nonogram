/**
 * Auteur: Brent Maas (s1826247)
 * Bestand: maas3.cc
 * Functie: 
 * Getest op: Windows 10 + MSys2 (MinGW64) met g++ 8.2.0
 * Getest met: g++ -o Assignment3 maas3.cc -std=c++17 -Wall -Wextra
 * Laatst bewerkt: 18/10/2018
 */

#include <iostream>
#include <fstream>
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

class nonogram{
public:
	nonogram(){
		m = 10;
		n = 10;
		stop = 0;
		cursorX = m / 2;
		cursorY = n / 2;
	}
	
	void teken(){
		for(int i = 0;i < 2 * n + 2;i++) std::cout << "-";
		std::cout << std::endl;
		for(int i = 0;i < m;i++){
			std::cout << "|";
			for(int j = 0;j < n;j++){
				if(cursorX == i && cursorY == j) std::cout << (veld[i][j] ? "*" : "_") << " ";
				else std::cout << (veld[i][j] ? "X" : " ") << " ";
			}
			std::cout << "|" << std::endl;
		}
		for(int i = 0;i < 2 * n + 2;i++) std::cout << "-";
		std::cout << std::endl;
	}
	
	void menu(){
		teken();
		char c = std::cin.get();
		while(c == '\n') c = std::cin.get(); //Negeer newlines
		if(c == 's' || c == 'S') stop = 1;
	}
	
	int stop;
private:
	const static int maxVeld = 100;
	int m, n, cursorX, cursorY;
	bool veld[maxVeld][maxVeld] = {};
};

int main(){
	//Print het infoblok
	infoBlokje();
	
	nonogram ng = nonogram();
	
	while(!ng.stop) ng.menu();
	
	return 0;
}