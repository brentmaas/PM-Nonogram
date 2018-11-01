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
#include <ctime>

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
		menuDiepte = 0;
		randomP = 50;
		getal = time(0);
	}
	
	char beschrijvingChar(int beschrijvingGetal){
		if(beschrijvingGetal < 10) return '0' + beschrijvingGetal;
		else return 'A' + (beschrijvingGetal - 10);
	}
	
	void teken(){
		for(int i = 0;i < 2 * n + 2;i++) std::cout << "-";
		std::cout << std::endl;
		for(int y = 0;y < m;y++){
			std::cout << "|";
			for(int x = 0;x < n;x++){
				if(cursorX == x && cursorY == y) std::cout << (veld[x][y] ? "*" : "_") << " ";
				else std::cout << (veld[x][y] ? "X" : " ") << " ";
			}
			std::cout << "|";
			for(int i = 0;i < maxVeld / 2 + maxVeld % 2 + 1;i++){
				int b = beschrijvingH[y][i];
				if(b == 0) break;
				std::cout << " " << beschrijvingChar(b);
			}
			std::cout << std::endl;
		}
		for(int i = 0;i < 2 * n + 2;i++) std::cout << "-";
		for(int y = 0;y < maxVeld / 2 + maxVeld % 2 + 1;y++){
			bool schrijf = false;
			std::string buffer = "";
			for(int x = 0;x < n;x++){
				int b = beschrijvingV[x][y];
				schrijf = schrijf || b != 0;
				buffer += " ";
				if(b != 0) buffer += beschrijvingChar(b);
				else buffer += " ";
			}
			if(schrijf){
				std::cout << buffer << std::endl;
			}else{
				break;
			}
		}
		std::cout << std::endl;
		if(menuDiepte == 0){
			std::cout << "S[T]op,[O]pties,[W]Omhoog,[S]Omlaag,[A]Links,[D]Rechts,[F]Toggle,[R]andom" << std::endl;
			std::cout << "[L]eeg,Beschrijving:[C]Lees[V]Schrijf[B]update[N]ul,Beeld:[J]Lees[K]Schrijf" << std::endl;
		}else if(menuDiepte == 1){
			std::cout << "[T]erug,[G]rootte wijzigen,[R]andompercentage" << std::endl;
		}
	}
	
	void beweeg(int dx, int dy){
		cursorX += dx;
		cursorY += dy;
		if(cursorX < 0) cursorX = 0;
		if(cursorX >= n) cursorX = n - 1;
		if(cursorY < 0) cursorY = 0;
		if(cursorY >= m) cursorY = m - 1;
	}
	
	void toggleCursor(){
		veld[cursorX][cursorY] = !veld[cursorX][cursorY];
	}
	
	int randomPercentage(){
		//static int getal = time(0);
		getal = (221 * getal + 1) % 1000;
		return getal % 100;
	}
	
	void vulRandom(){
		for(int x = 0;x < n;x++) for(int y = 0;y < m;y++){
			veld[x][y] = randomPercentage() < randomP;
		}
	}
	
	void leeg(){
		for(int x = 0;x < n;x++) for(int y = 0;y < m;y++){
			veld[x][y] = 0;
		}
	}
	
	void beschrijvingLees(){
		
	}
	
	void beschrijvingSchrijf(){
		
	}
	
	void beschrijvingUpdate(){
		for(int i = 0;i < maxVeld;i++){
			int iV = 0, tellerV = 0, iH = 0, tellerH = 0;
			for(int y = 0;y < maxVeld;y++){
				if(veld[i][y]){
					tellerV++;
				}else if(tellerV > 0){
					beschrijvingV[i][iV] = tellerV;
					iV++;
					tellerV = 0;
				}
				if(veld[y][i]){
					tellerH++;
				}else if(tellerH > 0){
					beschrijvingH[i][iH] = tellerH;
					iH++;
					tellerH = 0;
				}
			}
			if(tellerV > 0){
				beschrijvingV[i][iV] = tellerV;
				iV++;
			}
			if(tellerH > 0){
				beschrijvingH[i][iH] = tellerH;
				iH++;
			}
			beschrijvingV[i][iV] = 0;
			beschrijvingH[i][iH] = 0;
		}
	}
	
	void beschrijvingNul(){
		for(int i = 0;i < maxVeld;i++) for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
			beschrijvingV[i][j] = 0;
			beschrijvingH[i][j] = 0;
		}
	}
	
	void beeldLees(){
		
	}
	
	void beeldSchrijf(){
		
	}
	
	void menu(){
		teken();
		char c = std::cin.get();
		while(c == '\n') c = std::cin.get(); //Negeer newlines
		if(menuDiepte == 0){
			if(c == 't' || c == 'T') stop = 1;
			if(c == 'o' || c == 'O') menuDiepte = 1;
			if(c == 'w' || c == 'W') beweeg(0, -1);
			if(c == 's' || c == 'S') beweeg(0, 1);
			if(c == 'a' || c == 'A') beweeg(-1, 0);
			if(c == 'd' || c == 'D') beweeg(1, 0);
			if(c == 'f' || c == 'F') toggleCursor();
			if(c == 'r' || c == 'R') vulRandom();
			if(c == 'l' || c == 'L') leeg();
			if(c == 'c' || c == 'C') beschrijvingLees();
			if(c == 'v' || c == 'V') beschrijvingSchrijf();
			if(c == 'b' || c == 'B') beschrijvingUpdate();
			if(c == 'n' || c == 'N') beschrijvingNul();
			if(c == 'j' || c == 'J') beeldLees();
			if(c == 'k' || c == 'K') beeldSchrijf();
		}else if(menuDiepte == 1){
			
		}
	}
	
	int stop;
private:
	const static int maxVeld = 100;
	int m, n, cursorX, cursorY, menuDiepte, randomP, getal;
	bool veld[maxVeld][maxVeld] = {false};
	//In het slechtste geval, alternerend true en false, zijn er maxVeld / 2
	//resp. maxVeld / 2 + 1 ints nodig om een beschrijving te genereren op een
	//even resp. oneven zijdig veld. Daarna nog + 1 voor een afsluitnul
	int beschrijvingV[maxVeld][maxVeld / 2 + maxVeld % 2 + 1] = {0}, beschrijvingH[maxVeld][maxVeld / 2 + maxVeld % 2 + 1] = {0};
};

int main(){
	//Print het infoblok
	infoBlokje();
	
	nonogram ng = nonogram();
	
	while(!ng.stop) ng.menu();
	
	return 0;
}