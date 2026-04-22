#define NOMINMAX
#include <iostream>
#include <string>
#ifdef _WIN32
#include <Windows.h> // Necessaria per cambiare la codifica del terminale
#endif
#include <cmath>
#include <vector>
#include <limits> //Necessaria per "numeric_limits"
#include <format>
#include <charconv> // Necessaria per "from_chars"
#include <string_view>
using namespace std;

class articolo // Gestisce unicamente i dati del singolo prodotto
{
   int codice;
   string nome;
   int qt;
   int przCentesimi;

public:
   articolo(int codice, string nome, int qtIniziale, int prz) : codice(codice), nome(nome), qt(qtIniziale), przCentesimi(prz) {} // Costruttore

   // Metodi di accesso agli attributi
   void set_qt(int qtAcquistata)
   {
      qt -= qtAcquistata;
   }

   void set_qtMod(int nuovaQt)
   {
      qt = nuovaQt;
   }

   void set_codice(int nuovoCodice)
   {
      codice = nuovoCodice;
   }

   void set_prz(int nuovoPrz)
   {
      przCentesimi = nuovoPrz;
   }

   void set_nome(string nuovoNome)
   {
      nome = nuovoNome;
   }

   void getAttrsEsposizione() const
   {
      cout << format("Codice: {} - {} {:.2f}€\n", codice, nome, przCentesimi / 100.0);
   }

   string get_nome() const
   {
      return nome;
   }

   int get_qt() const
   {
      return qt;
   }

   int get_prz() const
   {
      return przCentesimi;
   }

   int get_codice() const
   {
      return codice;
   }
};

// Dichiarazione funzioni
void controlloTipo(int &centesimi);
void controlloQt(int &qt, articolo articolo);
int controllaInputInt();
double controllaInputDouble();
void gestisciTransazione(articolo &articolo);

class distributoreAutomatico // Gestisce l'inventario e le transazioni
{
   vector<articolo> articoli;
   const int PIN = 50778133; // Codice per accedere all'inventario
   static int incassi;

public:
   void aggiungiArticolo(string nome, int qtIniziale, int prz)
   {
      if (qtIniziale > 0)
      {
         articoli.emplace_back((static_cast<int>(articoli.size()) + 1), nome, qtIniziale, prz);
      }
   }

   articolo *getArticolo(int codice)
   {
      return &articoli.at(codice - 1);
   }

   void esposizione() const
   {
      cout << "\nQuesto è un distributore automatico. Ecco la lista degli articoli acquistabili con i relativi codici:\n";
      for (const auto articolo : articoli)
      {
         if (articolo.get_qt() > 0)
         {
            articolo.getAttrsEsposizione();
         }
      }
   }

   void inventario() const
   {
      for (const auto articolo : articoli)
      {
         cout << format("{}: {} disponibili.\n", articolo.get_nome(), articolo.get_qt());
      }
      cout << format("\nIncassi totali: {:.2f}€.\n", incassi / 100.0) << endl;
   }

   auto get_qtTot() const
   {
      auto qtTot = 0;
      for (const auto articolo : articoli)
      {
         qtTot += articolo.get_qt();
      }
      return qtTot;
   }

   int getNumArticoli() const
   {
      return static_cast<int>(articoli.size());
   }

   int getPIN() const
   {
      return PIN;
   }

   void rimuoviArticolo(int codice)
   {
      if (codice > 0 && codice <= articoli.size())
      {
         articoli.erase(articoli.begin() + codice - 1);
         for (auto i = codice - 1; i < articoli.size(); i++)
         {
            articoli.at(i).set_codice(i + 1);
         }
      }
      else
      {
         cout << "Codice non valido.\n";
      }
   }

   void modificaArticolo(int codice)
   {
      if (codice > 0 && codice <= articoli.size())
      {
         articolo *articolo = getArticolo(codice);
         cout << format("\nInserisci il nuovo nome dell'articolo, digita \"=\" per mantenere quello attuale.\nNome attuale: {}.\nNuovo nome: ", articolo->get_nome());
         string nuovoNome;
         getline(cin, nuovoNome);
         if (nuovoNome != "=")
         {
            articolo->set_nome(nuovoNome);
         }
         cout << format("\nInserisci la quantità, digita \"-1\" per mantenere quella attuale.\nQuantità attuale: {}.\nNuova quantità: ", articolo->get_qt());
         int nuova_qt = controllaInputInt();
         if (nuova_qt > -1)
         {
            articolo->set_qtMod(nuova_qt);
         }
         else
         {
            cout << "Quantità non valida.Verrà mantenuta quella attuale.\n";
         }
         cout << format("\nInserisci il nuovo prezzo dell'articolo (in centesimi), digita \"-1\" per mantenere quello attuale.\nPrezzo attuale: {}.\nNuovo prezzo: ", articolo->get_prz());
         int nuovo_prz = controllaInputInt();
         if (nuovo_prz >= 0)
         {
            articolo->set_prz(nuovo_prz);
         }
         else
         {
            cout << "Prezzo non valido. Verrà mantenuto quello attuale.\n";
         }
      }
      else
      {
         cout << "Codice non valido.\n";
      }
   }

   // Metodi static per gestire gli incassi del distributore
   static void set_incassi(int incasso)
   {
      incassi += incasso;
   }
};
int distributoreAutomatico::incassi = 0; // Inizializzazione variabile statica

int main()
{
#ifdef _WIN32
   // Forza il terminale a usare la codifica UTF-8
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
#endif

   distributoreAutomatico distributore;
   // Pannello degli articoli; attributi in ordine: Nome, Quantità, Prezzo (in centesimi).
   distributore.aggiungiArticolo("Acqua", 20, 50);
   distributore.aggiungiArticolo("Patatine", 20, 100);
   distributore.aggiungiArticolo("Sprite", 20, 100);
   distributore.aggiungiArticolo("Fanta", 20, 100);
   distributore.aggiungiArticolo("Biscotti", 20, 100);
   distributore.aggiungiArticolo("Barretta proteica", 20, 150);
   distributore.aggiungiArticolo("Caffè", 20, 120);
   distributore.aggiungiArticolo("Succo di frutta", 20, 100);

   while (distributore.get_qtTot() > 0)
   {
      distributore.esposizione();
      // Verifica quantità totale articoli (per testing del programma).
      // cout << format("Quantità totale articoli: {}.", distributore.get_qtTot()) << endl;

      cout << "\nInserire il codice del prodotto desiderato: ";
      int codice = controllaInputInt();
      if (codice == 999)
      {
         cout << "Inserisci il PIN: ";
         int PIN = controllaInputInt();
         cout << endl;
         if (PIN == distributore.getPIN())
         {
            distributore.inventario();
            cout << "Digita \"1\" per aggiungere un articolo, \"2\" per rimuovere un articolo, \"3\" per modificare un articolo, \"4\" per uscire dal menu Amministratore: ";
            int scelta = controllaInputInt();
            switch (scelta)
            {
            case 1:
            {
               cout << "Inserisci il nome dell'articolo da aggiungere: ";
               string nome;
               getline(cin, nome);
               cout << "Inserisci la quantità (>0): ";
               int qtIniziale = controllaInputInt();
               cout << "Inserisci il prezzo dell'articolo (in centesimi): ";
               int prz = controllaInputInt();
               distributore.aggiungiArticolo(nome, qtIniziale, prz);
               break;
            }
            case 2:
            {
               cout << "Inserisci il codice dell'articolo da rimuovere: ";
               int codiceRimozione = controllaInputInt();
               distributore.rimuoviArticolo(codiceRimozione);
               break;
            }
            case 3:
            {
               cout << "Inserisci il codice dell'articolo da modificare: ";
               int codiceModifica = controllaInputInt();
               distributore.modificaArticolo(codiceModifica);
               break;
            }
            case 4:
               break;
            default:
               cout << "Scelta non valida.\n";
            }
         }
         else
         {
            cout << "Codice PIN errato.\n"
                 << endl;
            continue;
         }
      }
      else if (codice > 0 && codice <= distributore.getNumArticoli())
      {
         articolo *articoloScelto = distributore.getArticolo(codice);
         gestisciTransazione(*articoloScelto);
      }
      else
      {
         cout << "Codice non valido.\n\n";
      }
   }
   cout << "TUTTO ESAURITO\n";

   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cout << "Premi INVIO per uscire...";
   cin.get();
   return 0;
}

// Definizione funzioni
void controlloTipo(int &centesimi)
{
   while (centesimi != 10 && centesimi != 20 && centesimi != 50 && centesimi != 100 && centesimi != 200 && centesimi != 500 && centesimi != 1000 && centesimi != 2000)
   {
      cout << "Denaro non riconosciuto. Riprovare..." << endl;
      auto denaro = controllaInputDouble();
      centesimi = static_cast<int>(round(denaro * 100));
   }
}

void controlloQt(int &qt, articolo articolo)
{
   while (qt < 1 || qt > articolo.get_qt())
   {
      cout << "Quantità non valida. Riprovare...\n"
           << endl;
      cout << "Scegliere la quantità desiderata: ";
      qt = controllaInputInt();
   }
}

int controllaInputInt()
{
   double input = 0;
   while (!(cin >> input) || input != static_cast<int>(input))
   {
      cout << "Errore. Valore non riconosciuto. Riprovare: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return static_cast<int>(input);
}

double controllaInputDouble()
{
   string riga = "";
   double input = 0.0;
   while (true)
   {
      if (!getline(cin, riga) || riga.empty())
      {
         continue;
      }
      // "from_chars" converte la stringa.
      // Questa funzione dice esattamente dove si è fermata la lettura.
      auto [ptr, ec] = from_chars(riga.data(), riga.data() + riga.size(), input);

      // ec == errc{}: non ci sono stati errori di conversione (es. overflow o nessuna cifra).
      // ptr == riga.data() + riga.size(): la lettura è arrivata alla FINE della stringa.
      if (ec == errc{} && ptr == riga.data() + riga.size())
      {
         return input; // Input perfettamente pulito!
      }
      cout << "Errore. Valore non riconosciuto. Riprovare: ";
   }
}

void gestisciTransazione(articolo &articolo)
{
   if (articolo.get_qt() < 1)
   {
      cout << "\nSiamo spiacenti, il prodotto è esaurito.\n\n";
      return;
   }
   cout << format("\nCodice {}:\n{} {:.2f}€ - Quantità disponibile: {}\nScegliere la quantità desiderata (digita \"0\" per annullare): ", articolo.get_codice(), articolo.get_nome(), articolo.get_prz() / 100.0, articolo.get_qt());
   int qt = controllaInputInt();
   if (qt == 0)
   {
      return;
   }
   controlloQt(qt, articolo);
   int totale = qt * articolo.get_prz();
   cout << format("\nQuantità selezionata: {}. TOTALE: {:.2f}€.\n*I tagli superiori a 20€ non verranno accettati.*\nInserire il denaro... (digita \"0\" per annullare)\n", qt, totale / 100.0);
   auto pagato = 0;
   while (pagato < totale)
   {
      auto denaro = controllaInputDouble();
      if (denaro == 0)
      {
         return;
      }
      auto centesimi = static_cast<int>(round(denaro * 100));
      controlloTipo(centesimi);
      pagato += centesimi;
      if (pagato < totale)
      {
         cout << format("Pagato: {:.2f}€, rimanente: {:.2f}€.\nInserire altro denaro...\n", pagato / 100.0, (totale - pagato) / 100.0);
      }
   }
   cout << "------------------------------" << endl;
   if (pagato > totale)
   {
      cout << format("Ecco i suoi prodotti. Il resto è {:.2f}€.\n", (pagato - totale) / 100.0);
   }
   else
   {
      cout << "Ecco i suoi prodotti. ";
   }
   cout << "Buona giornata :)\n"
        << endl;
   distributoreAutomatico::set_incassi(totale);
   articolo.set_qt(qt);
}
