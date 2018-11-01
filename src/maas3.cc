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
		cursorX = n / 2;
		cursorY = m / 2;
		menuDiepte = 0;
		randomP = 50;
		cursorOptie = 0;
		controleer();
	}
	
	bool isGetal(char c){
		return c >= '0' && c <= '9';
	}
	
	int leesgetal(int maximum){
		int getal = 0;
		bool beeindig = false;
		char c = std::cin.get();
		while(c == '\n') c = std::cin.get(); //Negeer voorloop-enters
		//Lees tot enter
		while(c != '\n'){
			if(isGetal(c) && !beeindig){
				int nieuwGetal = getal * 10 + (c - '0');
				if(nieuwGetal > maximum) beeindig = true; //Laat functie doorlezen tot eerstvolgende enter
				else getal = nieuwGetal;
			}
			c = std::cin.get();
		}
		return getal;
	}
	
	char leesoptie(){
		char optie = std::cin.get();
		while(optie == '\n') optie = std::cin.get(); //Negeer newlines
		return optie;
	}
	
	char beschrijvingChar(int beschrijvingGetal){
		if(beschrijvingGetal < 10) return '0' + beschrijvingGetal;
		else return 'A' + (beschrijvingGetal - 10);
	}
	
	void controleer(){
		for(int i = 0;i < maxVeld;i++){
			goedH[i] = true;
			goedV[i] = true;
		}
		for(int x = 0;x < maxVeld;x++){
			int i = 0, teller = 0;
			for(int y = 0;y < maxVeld;y++){
				if(veld[x][y]){
					teller++;
				}else if(teller > 0){
					if(teller != beschrijvingV[x][i]){
						goedV[x] = false;
						break;
					}
					i++;
					teller = 0;
				}
			}
			if(teller != beschrijvingV[x][i]) goedV[x] = false;
		}
		for(int y = 0;y < maxVeld;y++){
			int i = 0, teller = 0;
			for(int x = 0;x < maxVeld;x++){
				if(veld[x][y]){
					teller++;
				}else if(teller > 0){
					if(teller != beschrijvingH[y][i]){
						goedH[y] = false;
						break;
					}
					i++;
					teller = 0;
				}
			}
			if(teller != beschrijvingH[y][i]) goedH[y] = false;
		}
	}
	
	void teken(bool metMenu){
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
			if(goedH[y]) std::cout << "  V";
			std::cout << std::endl;
		}
		for(int i = 0;i < 2 * n + 2;i++) std::cout << "-";
		std::cout << std::endl;
		bool eennalaatste = false;
		for(int y = 0;y < maxVeld / 2 + maxVeld % 2 + 1;y++){
			bool schrijf = false;
			std::string buffer = "";
			for(int x = 0;x < n;x++){
				int b = beschrijvingV[x][y];
				schrijf = schrijf || b != 0;
				buffer += " ";
				if(b != 0) buffer += beschrijvingChar(b);
				else if((y > 0 && beschrijvingV[x][y-1] != 0 && goedV[x]) || (y == 0 && beschrijvingV[x][0] == 0 && goedV[x])) buffer += "V";
				else buffer += " ";
			}
			if(!schrijf && !eennalaatste){
				eennalaatste = true;
				schrijf = true;
			}
			if(schrijf) std::cout << buffer << std::endl;
			else break;
		}
		if(metMenu){
			if(menuDiepte == 0){
				std::cout << "S[T]op, [O]pties, [W]Omhoog, [S]Omlaag, [A]Links, [D]Rechts, [F]Toggle, [R]andom" << std::endl;
				std::cout << "[L]eeg, Beschrijving: [C]Lees [V]Schrijf [B]update [N]ul, Beeld: [J]Lees [K]Schrijf" << std::endl;
			}else if(menuDiepte == 1) std::cout << "[T]erug, [G]rootte wijzigen, [C]ursoropties, [R]andompercentage" << std::endl;
		}
	}
	
	void beweeg(int dx, int dy){
		cursorX += dx;
		cursorY += dy;
		if(cursorX < 0) cursorX = 0;
		if(cursorX >= n) cursorX = n - 1;
		if(cursorY < 0) cursorY = 0;
		if(cursorY >= m) cursorY = m - 1;
		
		if(cursorOptie == 1) veld[cursorX][cursorY] = true;
		if(cursorOptie == 2) veld[cursorX][cursorY] = false;
		
		if(cursorOptie != 0) controleer();
	}
	
	void toggleCursor(){
		veld[cursorX][cursorY] = !veld[cursorX][cursorY];
		controleer();
	}
	
	int randomPercentage(){
		static int getal = time(0);
		getal = (221 * getal + 1) % 1000;
		return getal % 100;
	}
	
	void vulRandom(){
		for(int x = 0;x < n;x++) for(int y = 0;y < m;y++) veld[x][y] = randomPercentage() < randomP;
		controleer();
	}
	
	void leeg(){
		for(int x = 0;x < n;x++) for(int y = 0;y < m;y++) veld[x][y] = 0;
		controleer();
	}
	
	std::string vraagBestand(){
		std::string out = "";
		char c = std::cin.get();
		while(c == '\n') c = std::cin.get(); //Negeer voorloop-enters
		while(c != '\n'){
			out += c;
			c = std::cin.get();
		}
		return out;
	}
	
	void beschrijvingNul(){
		for(int i = 0;i < maxVeld;i++) for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
			beschrijvingV[i][j] = 0;
			beschrijvingH[i][j] = 0;
		}
	}
	
	void beschrijvingUpdate(){
		beschrijvingNul();
		for(int i = 0;i < maxVeld;i++){
			int iV = 0, tellerV = 0, iH = 0, tellerH = 0;
			for(int y = 0;y < maxVeld;y++){
				if(veld[i][y]) tellerV++;
				else if(tellerV > 0){
					beschrijvingV[i][iV] = tellerV;
					iV++;
					tellerV = 0;
				}
				if(veld[y][i]) tellerH++;
				else if(tellerH > 0){
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
		controleer();
	}
	
	void beschrijvingLees(){
		beschrijvingNul();
		teken(false);
		std::cout << "Geef een invoerbestand om de beschrijvingen uit te lezen: ";
		std::string f = vraagBestand();
		std::fstream fs(f.c_str(), std::ios_base::in);
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen." << std::endl;
			fs.close();
			return;
		}
		
		fs >> m;
		fs >> n;
		cursorX = n / 2;
		cursorY = m / 2;
		
		int g = 0;
		for(int i = 0;i < m;i++){
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				fs >> g;
				if(g == 0) break;
				beschrijvingH[i][j] = g;
			}
		}
		for(int i = 0;i < n;i++){
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				fs >> g;
				if(g == 0) break;
				beschrijvingV[i][j] = g;
			}
		}
		controleer();
	}
	
	void beschrijvingSchrijf(){
		teken(false);
		std::cout << "Geef een invoerbestand om de beschrijvingen naar te schrijven: ";
		std::string f = vraagBestand();
		std::fstream fs(f.c_str(), std::ios_base::out);
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen." << std::endl;
			fs.close();
			return;
		}
		fs << m << " " << n << std::endl;
		for(int i = 0;i < m;i++){
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				int g = beschrijvingH[i][j];
				fs << g;
				if(g != 0) fs << " ";
				else break;
			}
			fs << std::endl;
		}
		for(int i = 0;i < n;i++){
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				int g = beschrijvingV[i][j];
				fs << g;
				if(g != 0) fs << " ";
				else break;
			}
			fs << std::endl;
		}
	}
	
	void beeldLees(){
		leeg();
		teken(false);
		std::cout << "Geef een invoerbestand om het beeld uit te lezen: ";
		std::string f = vraagBestand();
		std::fstream fs(f.c_str(), std::ios_base::in);
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen." << std::endl;
			fs.close();
			return;
		}
		fs >> m;
		fs >> n;
		cursorX = n / 2;
		cursorY = m / 2;
		
		char c = fs.get();
		for(int y = 0;y < m;y++){
			while(c == '\n') c = fs.get();
			for(int x = 0;x < n;x++){
				if(c == '1') veld[x][y] = true;
				c = fs.get();
			}
		}
		beschrijvingUpdate();
	}
	
	void beeldSchrijf(){
		teken(false);
		std::cout << "Geef een invoerbestand om het beeld naar te schrijven: ";
		std::string f = vraagBestand();
		std::fstream fs(f.c_str(), std::ios_base::out);
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen." << std::endl;
			fs.close();
			return;
		}
		fs << m << " " << n << std::endl;
		for(int y = 0;y < m;y++){
			for(int x = 0;x < n;x++){
				fs << veld[x][y];
			}
			fs << std::endl;
		}
	}
	
	void wijzigGrootte(){
		beschrijvingNul();
		leeg();
		
		char c = std::cin.get();
		while(c != '\n') c = std::cin.get();
		
		int nieuwN, nieuwM;
		bool nGeldig = false, mGeldig = false;
		while(!nGeldig){
			std::cout << "Geef een nieuwe hoogte(1-100): ";
			nieuwN = leesgetal(100);
			if(nieuwN > 0) nGeldig = true;
			else std::cout << "Ongeldige hoogte. ";
		}
		n = nieuwN;
		while(!mGeldig){
			std::cout << "Geef een nieuwe breedte(1-100): ";
			nieuwM = leesgetal(100);
			if(nieuwM > 0) mGeldig = true;
			else std::cout << "Ongeldige breedte. ";
		}
		m = nieuwM;
		
		controleer();
	}
	
	void cursorOpties(){
		
	}
	
	void wijzigRandomP(){
		
	}
	
	void menu(){
		teken(true);
		char c = leesoptie();
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
			if(c == 't' || c == 'T') menuDiepte = 0;
			if(c == 'g' || c == 'G') wijzigGrootte();
			if(c == 'c' || c == 'C') cursorOpties();
			if(c == 'r' || c == 'R') wijzigRandomP();
		}
	}
	
	int stop;
private:
	const static int maxVeld = 100;
	//cursorOptie: 0 is doe niets, 1 is zet aan, 2 is zet uit
	int m, n, cursorX, cursorY, menuDiepte, randomP, cursorOptie;
	bool veld[maxVeld][maxVeld] = {false}, goedH[maxVeld] = {false}, goedV[maxVeld] = {false};
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