/**
 * Auteur: Brent Maas (s1826247)
 * Bestand: maas3.cc
 * Functie: Inlezen/schrijven van nonogram-beschrijvingen en -beelden, en het
 *          genereren van beschrijvingen aan de hand van een huidig beeld, en
 *          simpele bewerkingen van het huidige beeld.
 * Getest op: Windows 10 + MSys2 (MinGW64) met g++ 8.2.0
 * Getest met: g++ -o Assignment3 maas3.cc -std=c++17 -Wall -Wextra
 * Laatst bewerkt: 05/11/2018
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
	"Dit programma laat de gebruiker nonogrammen maken. De gebruiker kan\n"
	"deze nonogrammen zelf maken door zelf een beeld te maken en het\n"
	"programma de beschrijvingen te laten maken. Beelden en beschrijvingen\n"
	"kunnen worden gelezen of geschreven. Het beeld kan ook willekeurig\n"
	"worden gevuld aan de hand van een gegeven percentage.\n"
	"Laatst bewerkt: 05/11/2018\n"
	"////////////////////////////////////////////////\n" << std::endl;
}

//Nonogram klasse
class nonogram{
public:
	//Constructor
	nonogram(){
		//Initialiseer standaardwaarden
		m = 10;
		n = 10;
		stop = 0;
		cursorX = n / 2;
		cursorY = m / 2;
		menuDiepte = 0;
		randomP = 50;
		cursorOptie = 0;
	}
	
	//Controleer of een gegeven char een getal is
	bool isGetal(char c){
		return c >= '0' && c <= '9';
	}
	
	//Lees een getal dat uiterlijk maximum is
	int leesgetal(int maximum){
		int getal = 0; //Getalbuffer
		//Flag om de beeindiging van de whileloop aan te kondigen
		bool beeindig = false;
		char c = std::cin.get();
		while(c == '\n') c = std::cin.get(); //Negeer voorloop-enters
		//Lees tot enter
		while(c != '\n'){
			if(isGetal(c) && !beeindig){
				//Voeg het gelezen getal achter het huidige getal
				int nieuwGetal = getal * 10 + (c - '0');
				//Laat functie doorlezen tot eerstvolgende enter
				if(nieuwGetal > maximum) beeindig = true;
				//Anders doorgaan met nieuwe getal
				else getal = nieuwGetal;
			}
			c = std::cin.get();
		}
		return getal;
	}
	
	//Lees de volgende optie en geef deze terug als char
	char leesoptie(){
		char optie = std::cin.get();
		while(optie == '\n') optie = std::cin.get(); //Negeer newlines
		return optie;
	}
	
	//Geef een char (0-9, A-Z) voor de beschrijvingen (0-35)
	char beschrijvingChar(int beschrijvingGetal){
		if(beschrijvingGetal < 10) return '0' + beschrijvingGetal;
		else return 'A' + (beschrijvingGetal - 10);
	}
	
	//Controleer het huidige beeld aan de hand van de huidige beschrijving
	void controleer(){
		//Zet eerst alles als goed
		for(int i = 0;i < maxVeld;i++){
			goedH[i] = true;
			goedV[i] = true;
		}
		//Controleer kolommen
		for(int x = 0;x < maxVeld;x++){
			//i: index van huidige blok, teller: lengte van huidige blok
			int i = 0, teller = 0;
			//Ga de kolom af
			for(int y = 0;y < maxVeld;y++){
				//Als een vakje aanstaat, tel eentje bij huidige blok op
				if(veld[x][y]) teller++;
				//Anders als de lengte van het huidige blok meer dan nul is,
				//is een blok beeindigd
				else if(teller > 0){
					//Als dit niet klopt met de beschrijving, zet de kolom als
					//fout en stop
					if(teller != beschrijvingV[x][i]){
						goedV[x] = false;
						break;
					}
					//Anders ga door met het volgende blok
					i++;
					teller = 0;
				}
			}
			//Als wanneer de hele kolom gelezen is het laatste blok niet klopt
			//met de beschrijving, zet de kolom als fout. (teller = 0 mag,
			//want de beschrijving eindigt altijd met een nul)
			if(teller != beschrijvingV[x][i]) goedV[x] = false;
		}
		//Controleer rijen
		for(int y = 0;y < maxVeld;y++){
			//i: index van huidige blok, teller: lengte van huidige blok
			int i = 0, teller = 0;
			//Ga de rij af
			for(int x = 0;x < maxVeld;x++){
				//Als een vakje aanstaat, tel eentje bij huidige blok op
				if(veld[x][y]) teller++;
				//Anders als de lengte van het huidige blok meer dan nul is,
				//is een blok beeindigd
				else if(teller > 0){
					//Als dit niet klopt met de beschrijving, zet de rij als
					//fout en stop
					if(teller != beschrijvingH[y][i]){
						goedH[y] = false;
						break;
					}
					//Anders ga door met het volgende blok
					i++;
					teller = 0;
				}
			}
			//Als wanneer de hele rij gelezen is het laatste blok niet klopt
			//met de beschrijving, zet de rij als fout. (teller = 0 mag,
			//want de beschrijving eindigt altijd met een nul)
			if(teller != beschrijvingH[y][i]) goedH[y] = false;
		}
	}
	
	//Functie om het beeld met de beschrijvingen te tekenen
	void teken(bool metMenu){
		//Teken de bovenste streep van het kader
		for(int i = 0;i < 2 * n + 2;i++) std::cout << "-";
		std::cout << std::endl;
		//Teken de rijen
		for(int y = 0;y < m;y++){
			//Begin met het linkerstreepdeel van het kader
			std::cout << "|";
			//Ga de rij af
			for(int x = 0;x < n;x++){
				//Als de cursor op deze positie staat
				if(cursorX == x && cursorY == y)
					//Teken het speciale cursorteken
					std::cout << (veld[x][y] ? "*" : "_") << " ";
				//Anders teken het normale teken
				else std::cout << (veld[x][y] ? "X" : " ") << " ";
			}
			//Teken het rechterstreepdeel van het kader
			std::cout << "|";
			//Ga de beschrijving voor deze rij af
			//Zie regel 702 voor uitleg over limiet
			for(int i = 0;i < maxVeld / 2 + maxVeld % 2 + 1;i++){
				int b = beschrijvingH[y][i];
				//b == 0 geeft aan dat de beschrijving is beeindigd, dus stop
				if(b == 0) break;
				//Schrijf het karakter dat bij het getal van de beschrijving
				//hoort
				std::cout << " " << beschrijvingChar(b);
			}
			//Als deze rij goed is, teken een 'V' om dit aan te geven
			if(goedH[y]) std::cout << "  V";
			//Beeindig de rij
			std::cout << std::endl;
		}
		//Teken de onderste streep van het kader
		for(int i = 0;i < 2 * n + 2;i++) std::cout << "-";
		std::cout << std::endl;
		//Flag om aan te geven of afgelopen iteratie de een na laatste was
		bool eennalaatste = false;
		//Teken de beschrijvingen voor de kolommen
		//Zie regel 702 voor uitleg over limiet
		for(int y = 0;y < maxVeld / 2 + maxVeld % 2 + 1;y++){
			bool schrijf = false; //Of er wel of niet geschreven moet worden
			std::string buffer = ""; //Buffer voor de huidige regel
			//Ga de rij af
			for(int x = 0;x < n;x++){
				int b = beschrijvingV[x][y];
				//Schrijf alleen als er iets te schrijven valt
				schrijf = schrijf || b != 0;
				buffer += " ";
				//Teken de beschrijving als die er is
				if(b != 0) buffer += beschrijvingChar(b);
				//Anders kijk of hier een 'V' moet komen
				else if((y > 0 && beschrijvingV[x][y-1] != 0 && goedV[x]) ||
						(y == 0 && beschrijvingV[x][0] == 0 && goedV[x]))
					buffer += "V";
				//Anders teken lege ruimte
				else buffer += " ";
			}
			//Als er niet geschreven zou worden, maar de een na laatste regel
			//is nog niet geweest, schrijf evengoed en maak dit de een na
			//laatste regel
			if(!schrijf && !eennalaatste){
				eennalaatste = true;
				schrijf = true;
			}
			//Schrijf de regel als er geschreven moet worden
			if(schrijf) std::cout << buffer << std::endl;
			//Anders stop
			else break;
		}
		//Als er een menu moet komen
		if(metMenu){
			//Hoofdmenu
			if(menuDiepte == 0){
				std::cout << "S[T]op, [O]pties, [W]Omhoog, [S]Omlaag, [A]Links"
						", [D]Rechts, [F]Toggle, [R]andom" << std::endl;
				std::cout << "[L]eeg, Beschrijving: [C]Lees [V]Schrijf [B]"
						"update [N]ul, Beeld: [J]Lees [K]Schrijf" << std::endl;
			//Optiemenu
			}else if(menuDiepte == 1)
				std::cout << "[T]erug, [G]rootte wijzigen, [C]ursoropties, "
				"[R]andompercentage" << std::endl;
		}
	}
	
	//Functie om de cursor te bewegen
	void beweeg(int dx, int dy){
		//Beweeg de cursor
		cursorX += dx;
		cursorY += dy;
		//Corrigeer als de cursor buiten de grenzen valt
		if(cursorX < 0) cursorX = 0;
		if(cursorX >= n) cursorX = n - 1;
		if(cursorY < 0) cursorY = 0;
		if(cursorY >= m) cursorY = m - 1;
		
		//Cursoroptie om de doelpositie aan te zetten
		if(cursorOptie == 1) veld[cursorX][cursorY] = true;
		//Cursoroptie om de doelpositie uit te zetten
		else if(cursorOptie == 2) veld[cursorX][cursorY] = false;
		
		//Controleer het beeld als een van de wijzigende opties is gebruikt
		if(cursorOptie != 0) controleer();
	}
	
	//Functie om de huidige positie te togglen
	void toggleCursor(){
		veld[cursorX][cursorY] = !veld[cursorX][cursorY];
		//Beeld gewijzigd; controleer beeld
		controleer();
	}
	
	//Genereer een willekeurig percentage
	int randomPercentage(){
		static int getal = time(0);
		getal = (221 * getal + 1) % 1000;
		return getal % 100;
	}
	
	//Vul het beeld willekeurig
	void vulRandom(){
		for(int x = 0;x < n;x++) for(int y = 0;y < m;y++)
			veld[x][y] = randomPercentage() < randomP;
		//Beeld gewijzigd; controleer beeld
		controleer();
	}
	
	//Leeg het beeld
	void leeg(){
		for(int x = 0;x < n;x++) for(int y = 0;y < m;y++) veld[x][y] = 0;
		//Beeld gewijzigd; controleer beeld
		controleer();
	}
	
	//Functie om een bestandspad te lezen uit invoer
	std::string vraagBestand(){
		std::string out = ""; //Buffer
		char c = std::cin.get();
		while(c == '\n') c = std::cin.get(); //Negeer voorloop-enters
		//Zolang een enter niet gevonden is, blijf lezen en voeg toe aan buffer
		while(c != '\n'){
			out += c;
			c = std::cin.get();
		}
		return out;
	}
	
	//Zet alle beschrijvingen naar nul
	void beschrijvingNul(){
		for(int i = 0;i < maxVeld;i++)
			//Zie regel 702 voor uitleg over limiet
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				beschrijvingV[i][j] = 0;
				beschrijvingH[i][j] = 0;
			}
		//Beschrijving gewijzigd; controleer beeld
		controleer();
	}
	
	//Update de beschrijving naar het huidige beeld
	void beschrijvingUpdate(){
		//Zet eerst alle beschrijvingen naar nul
		beschrijvingNul();
		//Ga de hoogte/breedte van het beeld af
		for(int i = 0;i < maxVeld;i++){
			//i: index van huidige blok, teller: lengte van huidige blok
			//V; kolommen, H; rijen
			int iV = 0, tellerV = 0, iH = 0, tellerH = 0;
			//Ga de breedte/hoogte van het beeld af
			for(int j = 0;j < maxVeld;j++){
				//Kolommen
				//Als het vakje aan staat, tel eentje bij huidige blok op
				if(veld[i][j]) tellerV++;
				//Anders als de lengte van het huidige blok meer dan nul is,
				//is een blok beeindigd
				else if(tellerV > 0){
					//Zet dit in de beschrijving
					beschrijvingV[i][iV] = tellerV;
					//Ga door met het volgende blok
					iV++;
					tellerV = 0;
				}
				
				//Rijen
				//Als het vakje aan staat, tel eentje bij huidige blok op
				if(veld[j][i]) tellerH++;
				//Anders als de lengte van het huidige blok meer dan nul is,
				//is een blok beeindigd
				else if(tellerH > 0){
					//Zet dit in de beschrijving
					beschrijvingH[i][iH] = tellerH;
					//Ga door met het volgende blok
					iH++;
					tellerH = 0;
				}
			}
			//Als het laatste blok nog meer dan nul is
			if(tellerV > 0){
				//Zet dit blok in de beschrijving
				beschrijvingV[i][iV] = tellerV;
				//Ga een blok verder
				iV++;
			}
			//Als het laatste blok nog meer dan nul is
			if(tellerH > 0){
				//Zet dit blok in de beschrijving
				beschrijvingH[i][iH] = tellerH;
				//Ga een blok verder
				iH++;
			}
			//Zet de beeindigende nullen in de beschrijving
			beschrijvingV[i][iV] = 0;
			beschrijvingH[i][iH] = 0;
		}
		//Beschrijving gewijzigd; controleer beeld
		controleer();
	}
	
	//Lees de beschrijvingen uit een bestand
	void beschrijvingLees(){
		teken(false);
		//Vraag om een bestandspad
		std::cout << "Geef een invoerbestand om de"
				"beschrijvingen uit te lezen: ";
		std::string f = vraagBestand();
		//Creer een filestream
		std::fstream fs(f.c_str(), std::ios_base::in);
		//Als de stream niet open is, meld dit, sluit de stream en stop
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen."
					<< std::endl;
			fs.close();
			return;
		}
		
		//Zet de beschrijvingen op nul
		beschrijvingNul();
		
		//Lees de hoogte en breedte, en zet de nieuwe cursorpositie
		fs >> m;
		fs >> n;
		cursorX = n / 2;
		cursorY = m / 2;
		
		int g = 0; //Getalbuffer
		//Rijbeschrijvingen
		for(int i = 0;i < m;i++)
			//Lees tot de eerste nul of het einde van de beschrijvingarray
			//Zie regel 702 voor uitleg over limiet
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				//Lees een getal
				fs >> g;
				//Als dit getal nul is, is de beschrijving voor deze rij
				//beeindigd
				if(g == 0) break;
				//Anders schrijf het getal naar de beschrijvingarray
				beschrijvingH[i][j] = g;
			}
		
		//Kolombeschrijvingen
		for(int i = 0;i < n;i++)
			//Lees tot de eerste nul of het einde van de beschrijvingarray
			//Zie regel 702 voor uitleg over limiet
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				//Lees een getal
				fs >> g;
				//Als dit getal nul is, is de beschrijving voor deze kolom
				//beeindigd
				if(g == 0) break;
				//Anders schrijf het getal naar de beschrijvingarray
				beschrijvingV[i][j] = g;
			}
		
		//Sluit de stream
		fs.close();
		
		//Leeg het beeld
		leeg();
		//Beschrijving en beeld gewijzigd; controleer beeld
		controleer();
	}
	
	//Schrijf de beschrijvingen naar een bestand
	void beschrijvingSchrijf(){
		teken(false);
		//Vraag om een bestandspad
		std::cout << "Geef een invoerbestand om de beschrijvingen naar te "
				"schrijven: ";
		std::string f = vraagBestand();
		//Creer een fileestream
		std::fstream fs(f.c_str(), std::ios_base::out);
		//Als de stream niet open is, meld dit, sluit de stream en stop
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen."
					<< std::endl;
			fs.close();
			return;
		}
		//Schrijf hoogte en breedte
		fs << m << " " << n << std::endl;
		//Rijbeschrijvingen
		for(int i = 0;i < m;i++){
			//Schrijf tot de eerste nul of het einde van de beschrijvingarray
			//Zie regel 702 voor uitleg over limiet
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				//Verkrijg een getal
				int g = beschrijvingH[i][j];
				//Schrijf het getal
				fs << g;
				//Als dit getal niet nul was, schrijf een tussenruimte
				if(g != 0) fs << " ";
				//Anders stop
				else break;
			}
			//Schrijf de endline
			fs << std::endl;
		}
		//Kolombeschrijvingen
		for(int i = 0;i < n;i++){
			//Schrijf tot de eerste nul of het einde van de beschrijvingarray
			//Zie regel 702 voor uitleg over limiet
			for(int j = 0;j < maxVeld / 2 + maxVeld % 2 + 1;j++){
				//Verkrijg een getal
				int g = beschrijvingV[i][j];
				//Schrijf het getal
				fs << g;
				//Als dit getal niet nul was, schrijf een tussenruimte
				if(g != 0) fs << " ";
				//Anders stop
				else break;
			}
			//Schrijf de endline
			fs << std::endl;
		}
		
		//Sluit de stream
		fs.close();
	}
	
	//Lees het beeld uit een bestand
	void beeldLees(){
		teken(false);
		//Vraag om een bestandspad
		std::cout << "Geef een invoerbestand om het beeld uit te lezen: ";
		std::string f = vraagBestand();
		//Creer een filestream
		std::fstream fs(f.c_str(), std::ios_base::in);
		//Als de stream niet open is, meld dit, sluit de stream en stop
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen."
					<< std::endl;
			fs.close();
			return;
		}
		
		//Leeg het beeld
		leeg();
		
		//Lees hoogte en breedte, en zet nieuwe cursorpositie
		fs >> m;
		fs >> n;
		cursorX = n / 2;
		cursorY = m / 2;
		
		//Lees eerste karakter
		char c = fs.get();
		//Lees rijen
		for(int y = 0;y < m;y++){
			//Negeer enters
			while(c == '\n') c = fs.get();
			//Lees kolommen
			for(int x = 0;x < n;x++){
				//Als er een '1' is, zet het vakje aan
				if(c == '1') veld[x][y] = true;
				//Lees een nieuw karakter
				c = fs.get();
			}
		}
		//Sluit stream
		fs.close();
		//Pas beschrijving aan aan nieuwe beeld
		beschrijvingUpdate();
	}
	
	//Schrijf het beeld naar een bestand
	void beeldSchrijf(){
		teken(false);
		//Vraag om een bestandspad
		std::cout << "Geef een invoerbestand om het beeld naar te schrijven: ";
		std::string f = vraagBestand();
		//Creer een filestream
		std::fstream fs(f.c_str(), std::ios_base::out);
		//Als de stream niet open is, meld dit, sluit de stream en stop
		if(!fs.is_open()){
			std::cout << "Kon het bestand '" << f << "' niet openen."
					<< std::endl;
			fs.close();
			return;
		}
		
		//Schijf de hoogte en breedte
		fs << m << " " << n << std::endl;
		
		//Schrijf rijen
		for(int y = 0;y < m;y++){
			//Schrijf kolommen
			for(int x = 0;x < n;x++) fs << veld[x][y];
			//Schrijf de endline
			fs << std::endl;
		}
		
		//Sluit de stream
		fs.close();
	}
	
	//Wijzigen van de beeldgrootte
	void wijzigGrootte(){
		teken(false);
		//Zet beschrijvingen op nul en leeg beeld
		beschrijvingNul();
		leeg();
		
		//Negeer voorloop-enters
		char c = std::cin.get();
		while(c != '\n') c = std::cin.get();
		
		int nieuwN, nieuwM;
		//Hoogte vragen
		while(true){
			//Vraag een getal
			std::cout << "Geef een nieuwe hoogte(1-100) (huidig: " << n
					<< "): ";
			nieuwN = leesgetal(maxVeld);
			//Als het getal in de goede range valt, stop
			if(nieuwN > 0) break;
			//Anders meld dit en vraag opnieuw
			else std::cout << "Hoogte moet meer dan nul zijn. ";
		}
		//Zet de nieuwe hoogte
		n = nieuwN;
		
		//Breedte vragen
		while(true){
			//Vraag een getal
			std::cout << "Geef een nieuwe breedte(1-100) (huidig: " << m
					<< "): ";
			nieuwM = leesgetal(maxVeld);
			//Als het getal in de goede range valt, stop
			if(nieuwM > 0) break;
			//Anders meld dit en vraag opnieuw
			else std::cout << "Breedte moet meer dan nul zijn. ";
		}
		//Zet de nieuwe breedte
		m = nieuwM;
		
		//Beeld en beschrijving gewijzegd; controleer beeld
		controleer();
	}
	
	//Zetten van de cursoropties
	void cursorOpties(){
		teken(false);
		
		//Print de opties
		std::cout << "Cursoropties:" << std::endl;
		std::cout << "[0] Doe niets" << std::endl;
		std::cout << "[1] Zet doelpositie aan" << std::endl;
		std::cout << "[2] Zet doelpositie uit" << std::endl;
		//Optie vragen
		int nieuwC;
		while(true){
			//Vraag een getal
			std::cout << "Kies een optie(0-2) (huidig:" << cursorOptie <<
					"): ";
			nieuwC = leesgetal(2);
			//Als het getal in de goede range valt, stop
			if(nieuwC >= 0) break;
			//Anders meld dit en vraag opnieuw
			std::cout << "Ongeldige optie. ";
		}
		//Zet de nieuw cursoroptie
		cursorOptie = nieuwC;
	}
	
	//Wijzigen van random percentage
	void wijzigRandomP(){
		teken(false);
		//Percentage vragen
		int nieuwRP;
		while(true){
			//Vraag een getal
			std::cout << "Geef een nieuw random percentage(0-100) (huidig: " << randomP << "): ";
			nieuwRP = leesgetal(100);
			//Als het getal in de goede range valt, stop
			if(nieuwRP >= 0) break;
			//Anders meld dit en vraag opnieuw
			else std::cout << "Percentage moet meer dan of gelijk aan nul zijn. ";
		}
		//Zet de nieuwe random percentage
		randomP = nieuwRP;
	}
	
	//Functie voor het menu
	void menu(){
		teken(true);
		//Lees een optie
		char c = leesoptie();
		//Gebruik optie voor hoofdmenu
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
		//Gebruik optie voor optiemenu
		}else if(menuDiepte == 1){
			if(c == 't' || c == 'T') menuDiepte = 0;
			if(c == 'g' || c == 'G') wijzigGrootte();
			if(c == 'c' || c == 'C') cursorOpties();
			if(c == 'r' || c == 'R') wijzigRandomP();
		}
	}
	
	//Flag om aan te geven of het programma stopt
	int stop;
private:
	//Maximale veldgrootte
	const static int maxVeld = 100;
	//m, n: hoogte, breedte
	//cursorX, cursorY: cursorpositie
	//menuDiepte: 0 is hoofdmenu, 1 is optiemenu
	//randomP: random percentage
	//cursorOptie: 0 is doe niets, 1 is zet aan, 2 is zet uit
	int m, n, cursorX, cursorY, menuDiepte, randomP, cursorOptie;
	//Beeld, goede rijen, goede kolommen
	bool veld[maxVeld][maxVeld] = {false}, goedH[maxVeld] = {true}, goedV[maxVeld] = {true};
	//In het slechtste geval, alternerend true en false, zijn er maxVeld / 2
	//resp. maxVeld / 2 + 1 ints nodig om een beschrijving te genereren op een
	//even resp. oneven zijdig veld. Daarna nog + 1 voor een afsluitnul
	int beschrijvingV[maxVeld][maxVeld / 2 + maxVeld % 2 + 1] = {0}, beschrijvingH[maxVeld][maxVeld / 2 + maxVeld % 2 + 1] = {0};
};

int main(){
	//Print het infoblok
	infoBlokje();
	
	//Creer nonogram instantie
	nonogram ng = nonogram();
	
	//Voer het menu uit zolang er niet gestopt wordt
	while(!ng.stop) ng.menu();
	
	return 0;
}