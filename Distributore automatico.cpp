#define NOMINMAX
#include <iostream>
#include <string>
#include <Windows.h> //Necessaria per cambiare la codifica del terminale
#include <cmath>
#include <vector>
#include <limits> //Necessaria per numeric_limits
using namespace std;

class Articolo {
   int codice;
   string nome;
   int quantita;
   int prezzoCentesimi;
   static vector<Articolo*> oggetti;
   static int qttTot;
   
   public:
   //Costruttore
   Articolo(string n, int qtIniziale, int prz):nome(n), quantita(qtIniziale), prezzoCentesimi(prz) {
      if (qtIniziale > 0) {
         oggetti.push_back(this);
         codice=oggetti.size();
         qttTot+=qtIniziale;
      }
   }

   //Metodi di accesso agli attributi
   auto setqt(int qt) {
      quantita-=qt;
   }

   auto getAttrEspos() {
      cout << "Codice: " << codice << " - " << nome << " " << prezzoCentesimi/100.0 << "€" << endl;
   }

   string getNome() {
      return nome;
   }

   int getqt() {
      return quantita;
   }

   int getPrezzo() {
      return prezzoCentesimi;
   }

   int getCodice() {
      return codice;
   }

   //Metodi statici
   static auto getNumeroOggetti() {
      return oggetti.size();
   }

   static Articolo* getOggetto(int indice) {
      return oggetti.at(indice);
   }

   static auto esposizione() {
      cout << "Questo è un distributore automatico. Ecco la lista degli articoli acquistabili con i relativi codici:" << endl;
      for (auto oggetto : oggetti) {
         if (oggetto->getqt() > 0) {
            oggetto->getAttrEspos();
         }
      }
   }

   static auto inventario() {
      for (auto oggetto : oggetti) {
         cout << oggetto->getNome() << ": " << oggetto->getqt() << " pezzi disponibili." << endl;
      }
      cout << "\n";
   }

   static int getQttTot(){
      return qttTot;
   }
};
//Inizializzazione variabili statiche
vector<Articolo*> Articolo::oggetti;
int Articolo::qttTot=0;

//Dichiarazione funzioni
void controlloTaglio(int &centesimi);
void controlloQuantita(int &qt, Articolo oggetto);
int controllaInt();
double controllaDouble();
void gestisciTransazione(Articolo &oggetto, int &qttTotVarAppoggio);

int main() {
   // Forza il terminale a usare la codifica UTF-8
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);

   //Pannello degli oggetti; proprietà in ordine: Nome, Quantità, Prezzo (in centesimi).
   Articolo acqua("Acqua", 1, 50);
   Articolo patatine("Patatine", 0, 100);
   Articolo sprite("Sprite", 0, 100);
   Articolo fanta("Fanta", 0, 100);
   Articolo biscotti("Biscotti", 0, 100);
   Articolo barretta_proteica("Barretta proteica", 0, 150);
   Articolo caffe("Caffè", 1, 120);
   Articolo succo_di_frutta("Succo di frutta", 0, 100);
   
   //Variabili principali
   auto qttTotVarAppoggio=Articolo::getQttTot();
   int codice;
   
   while (qttTotVarAppoggio > 0) {
      Articolo::esposizione();
      //Verifica quantità totale articoli (per testing del programma)
      //cout << "Quantità totale articoli: " << qttTotAppoggio << "." << endl;

      cout << "\nInserire il codice del prodotto desiderato: ";
      codice = controllaInt();
      if (codice==999) {
         Articolo::inventario();
      } else if (codice>0 && codice<=Articolo::getNumeroOggetti()) {
         gestisciTransazione(*Articolo::getOggetto(codice-1), qttTotVarAppoggio);
      } else {
         cout << "Codice non valido.\n" << endl;
      }
   }
   cout << "TUTTO ESAURITO" << endl;

   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cin.get();
   return 0;
}

//Definizione funzioni
void controlloTaglio(int &centesimi) {
   auto denaro=0.0;
   while (centesimi!=10 && centesimi!=20 && centesimi!=50 && centesimi!=100 && centesimi!=200 && centesimi!=500 && centesimi!=1000 && centesimi!=2000) {
      cout << "Taglio non riconosciuto. Riprovare..." << endl;
      denaro = controllaDouble();
      centesimi = static_cast<int>(round(denaro * 100));
   }
}

void controlloQuantita(int &qt, Articolo oggetto) {
   while (qt<1 || qt>oggetto.getqt()) {
      cout << "Quantità non valida. Riprovare...\n" << endl;
      cout << "Scegliere la quantità desiderata: ";
      qt = controllaInt();
   }
}

int controllaInt() {
   float valore;
   while (!(cin >> valore) || valore!=static_cast<int>(valore)) {
      cout << "Errore. Valore non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return valore;
}

double controllaDouble() {
   double valore;
   while (!(cin >> valore)) {
      cout << "Errore. Valore non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return valore;
}

void gestisciTransazione(Articolo &oggetto, int &qttTotVarAppoggio) {
   if (oggetto.getqt() > 0) {
      int qt, centesimi=0, pagamento=0;
      auto denaro=0.0;
      cout << "\nCodice " << oggetto.getCodice() << ":" << endl;
      cout << oggetto.getNome() << " " << oggetto.getPrezzo()/100.0 << "€ - Quantità disponibile: " << oggetto.getqt() << endl;
      cout << "\nScegliere la quantità desiderata: ";
      qt = controllaInt();
      controlloQuantita(qt, oggetto);
      int Totale=qt*oggetto.getPrezzo();
      cout << "\nQuantità selezionata: " << qt << ". TOTALE: " << Totale/100.0 << "€." << endl;
      cout << "*I tagli superiori a 20€ non verranno accettati.*\nInserire il denaro..." << endl;
      denaro = controllaDouble();
      centesimi = static_cast<int>(round(denaro * 100));
      controlloTaglio(centesimi);
      pagamento+=centesimi;
      while (pagamento < Totale) {
         cout << "Pagato: " << pagamento/100.0 << "€, rimanente: " << (Totale-pagamento)/100.0 << "€." << endl;
         cout << "Inserire altro denaro..." << endl;
         denaro = controllaDouble();
         centesimi = static_cast<int>(round(denaro * 100));
         controlloTaglio(centesimi);
         pagamento+=centesimi;
      }
      cout << "------------------------------" << endl;
      if (pagamento > Totale) {
         cout << "Ecco i suoi prodotti. Il resto è " << (pagamento-Totale)/100.0 << "€." << endl;
         cout << "Buona giornata :)\n" << endl;
      }
      else {
         cout << "Ecco i suoi prodotti. Buona giornata :)\n" << endl;
      }
      oggetto.setqt(qt);
      qttTotVarAppoggio-=qt;
   } else {
      cout << "\nSiamo spiacenti, il prodotto è esaurito.\n" << endl;
   }
}