#define NOMINMAX // Deactivates min e max Windows macros
#include <iostream>
#include <string>
#ifdef _WIN32
#include <Windows.h> // Used to change terminal's encoding
#endif
#include <cmath> // For round() function
#include <vector>
#include <limits> // Necessary for "numeric_limits"
#include <format>
#include <charconv> // Necessary for "from_chars"
#include <string_view>

using namespace std;

class art // Manages only the article data
{
   // Attributes
   int code;
   string name;
   int qt;
   int priceCent;

public:
   art(int code, string name, int qtStart, int price) : code(code), name(name), qt(qtStart), priceCent(price) {} // Constructor

   // Attributes access methods
   void set_code(int newCode)
   {
      code = newCode;
   }

   int get_code() const
   {
      return code;
   }

   void set_name(string newName)
   {
      name = newName;
   }

   string get_name() const
   {
      return name;
   }

   void set_qt(int qtBought)
   {
      qt -= qtBought;
   }

   void set_qtMod(int newQt)
   {
      qt = newQt;
   }

   int get_qt() const
   {
      return qt;
   }

   void set_price(int newPrice)
   {
      priceCent = newPrice;
   }

   int get_price() const
   {
      return priceCent;
   }

   void getAttrsExpo() const // Shows code, name and price of an article
   {
      cout << format("Code: {} - {} {:.2f}€\n", code, name, priceCent / 100.0); // Price is calculated in cents, but it's shown in € by dividing by 100.0 (to ensure a type:double result), with 2 floating digits
   }
};

// Functions declaration
void typeCheck(int &cents);     // Checks the denomination of the money
void qtCheck(int &qt, art art); // Checks the selected quantity
int inputInt_check();           // Checks the input where integer
double inputDouble_check();     // Checks the input where decimal
void manageTransaction(art &art);

class vendingMachine // Manages inventory and transactions
{
   // Attributes
   vector<art> arts;
   const int PIN = 50778133; // Inventory access code
   static int earnings;

public:
   void addArt(string name, int qtStart, int price) // Adds an item to the vector after creating it using the constructor of the class "art"
   {
      if (qtStart > 0)
      {
         arts.emplace_back((static_cast<int>(arts.size()) + 1), name, qtStart, price); // The casting avoids compiling warnings regarding strange type conversions
      }
   }

   art* getArt(int code) // Gets an item of the vector based on the code
   {
      return &arts.at(code - 1);
   }

   auto get_qtTot() const
   {
      auto qtTot = 0;
      for (const auto art : arts)
      {
         qtTot += art.get_qt();
      }
      return qtTot;
   }

   int getHowManyArts() const
   {
      return static_cast<int>(arts.size());
   }

   int getPIN() const
   {
      return PIN;
   }

   void expo() const // Shows the list of the articles
   {
      cout << "\nThis is a vending machine. Here's the list of purchasable articles with the respective codes:\n";
      for (const auto art : arts)
      {
         if (art.get_qt() > 0)
         {
            art.getAttrsExpo();
         }
      }
   }

   void inventory() const
   {
      for (const auto art : arts)
      {
         cout << format("{}: {} available.\n", art.get_name(), art.get_qt());
      }
      cout << format("\nTotal earnings: {:.2f}€.\n", earnings / 100.0) << endl;
   }

   void delArt(int code)
   {
      if (code > 0 && code <= arts.size())
      {
         arts.erase(arts.begin() + code - 1); // Removes the item at the specified position of the vector, based on the code

         // Resets the articles codes, starting from the position of the removed item
         for (auto i = code - 1; i < arts.size(); i++)
         {
            arts.at(i).set_code(i + 1);
         }
      }
      else
      {
         cout << "Invalid code.\n";
      }
   }

   void modArt(int code)
   {
      if (code > 0 && code <= arts.size())
      {
         art *art = getArt(code);
         cout << format("\nWrite the new name of the article, type \"=\" to keep the current one.\nCurrent name: {}.\nNew name: ", art->get_name());
         string newName;
         getline(cin, newName);
         if (newName != "=")
         {
            art->set_name(newName);
         }
         cout << format("\nPut in the quantity, type \"-1\" to keep the current one.\nCurrent quantity: {}.\nNew quantity: ", art->get_qt());
         int new_qt = inputInt_check();
         if (new_qt > -1) // It is possible to set the new quantity to 0. This allows to keep the existing article without showing it
         {
            art->set_qtMod(new_qt);
         }
         else
         {
            cout << "Invalid quantity. The current one will be kept.\n";
         }
         cout << format("\nType in the new price for the article (in cents), type \"-1\" to keep the current one.\nCurrent price: {}.\nNew price: ", art->get_price());
         int new_price = inputInt_check();
         if (new_price >= 0)
         {
            art->set_price(new_price);
         }
         else
         {
            cout << "Invalid price. The current one will be kept.\n";
         }
      }
      else
      {
         cout << "Invalid code.\n";
      }
   }

   // Static method for managing earnings
   static void set_earnings(int earning)
   {
      earnings += earning;
   }
};
int vendingMachine::earnings = 0; // Static variable initialization

int main()
{
#ifdef _WIN32
   // Forces terminal to use UTF-8 encoding
   SetConsoleOutputCP(CP_UTF8);
   SetConsoleCP(CP_UTF8);
#endif
   vendingMachine dispenser; // Creates an object of the class "vendingMachine" to access the class methods
   // Articles panel; attributes in order: Name, Quantity, Price (in cents).
   dispenser.addArt("Water", 20, 50);
   dispenser.addArt("Chips", 20, 100);
   dispenser.addArt("Sprite", 20, 100);
   dispenser.addArt("Fanta", 20, 100);
   dispenser.addArt("Biscuits", 20, 100);
   dispenser.addArt("Protein bar", 20, 150);
   dispenser.addArt("Coffee", 20, 120);
   dispenser.addArt("Apple juice", 20, 100);

   while (dispenser.get_qtTot() > 0) // Every time the function is called, it dynamically calculates the qtTot
   {
      dispenser.expo();
      // Verify articles total quantity (for program testing).
      // cout << format("Articles total quantity: {}.", dispenser.get_qtTot()) << endl;

      cout << "\nEnter the desired product code: ";
      int code = inputInt_check();
      if (code == 999) // Secret code
      {
         cout << "Enter the PIN: ";
         int PIN = inputInt_check();
         cout << endl;
         if (PIN == dispenser.getPIN())
         {
            // The Admin console allows to manage articles during execution time
            dispenser.inventory(); // Shows inventory
            cout << "Enter \"1\" to add a new article, \"2\" to remove an existing article, \"3\" to modify an existing article, \"4\" to exit Admin console: ";
            int selection = inputInt_check();
            switch (selection)
            {
            case 1:
            {
               cout << "Enter the name of the new article: ";
               string name;
               getline(cin, name);
               cout << "Put in the quantity (>0): "; // If the quantity is not >0, the item is not created
               int qtStart = inputInt_check();
               cout << "Type in the article price (in cents): ";
               int price = inputInt_check();
               dispenser.addArt(name, qtStart, price); // Creates the new article
               break;
            }
            case 2:
            {
               cout << "Enter the code of the article you want to remove: ";
               int removalCode = inputInt_check();
               dispenser.delArt(removalCode); // Removes the specified article
               break;
            }
            case 3:
            {
               cout << "Enter the code of the article you want to modify: ";
               int modCode = inputInt_check();
               dispenser.modArt(modCode); // Modifies the specified article
               break;
            }
            case 4:
               break; // Exits the Admin console
            default:
               cout << "Invalid input.\n";
            }
         }
         else
         {
            cout << "The PIN is not correct!\n"
                 << endl;
            continue; // Skips to the next iteration
         }
      }
      else if (code > 0 && code <= dispenser.getHowManyArts())
      {
         art *chosenArt = dispenser.getArt(code);
         manageTransaction(*chosenArt);
      }
      else
      {
         cout << "Invalid code.\n\n";
      }
   }
   cout << "SOLD OUT\n";

   // Stalls the execution until the user presses ENTER
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   cout << "Press ENTER to exit the program...";
   cin.get();
   return 0;
}

// Functions definition
int inputInt_check()
{
   double input = 0;
   while (!(cin >> input) || input != static_cast<int>(input))
   {
      cout << "Error. Unrecognized input. Try again: ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return static_cast<int>(input); 
}

double inputDouble_check()
{
   string line = "";
   double input = 0.0;
   while (true)
   {
      if (!getline(cin, line) || line.empty())
      {
         continue;
      }
      // "from_chars" converts the string.
      // This function points exactly where the reading stopped.
      auto [ptr, ec] = from_chars(line.data(), line.data() + line.size(), input);

      // ec == errc{}: there were no conversion errors (e.g. overflow or no number).
      // ptr == line.data() + line.size(): the reading reached the END of the string.
      if (ec == errc{} && ptr == line.data() + line.size())
      {
         return input; // Perfectly clean input!
      }
      cout << "Error. Unrecognized input. Try again: ";
   }
}

void typeCheck(int &cents)
{
   while (cents != 10 && cents != 20 && cents != 50 && cents != 100 && cents != 200 && cents != 500 && cents != 1000 && cents != 2000) // List of allowed denominations
   {
      cout << "Money not recognized. Try again..." << endl;
      auto money = inputDouble_check();
      cents = static_cast<int>(round(money * 100)); // Every time money is inserted, it's transformed into cents to allow easier calculations. This is the standard when working with money and eliminates aproximation errors
   }
}

void qtCheck(int &qt, art art)
{
   while (qt < 1 || qt > art.get_qt())
   {
      cout << "Invalid quantity. Try again...\n"
           << endl;
      cout << "Choose the desired quantity: ";
      qt = inputInt_check();
   }
}

void manageTransaction(art &art)
{
   if (art.get_qt() < 1)
   {
      cout << "\nSorry, the product is out of stock.\n\n";
      return; // Terminates the function if the article is no more available
   }
   cout << format("\nCode {}:\n{} {:.2f}€ - Available quantity: {}\nSelect the desired quantity (type \"0\" to cancel): ", art.get_code(), art.get_name(), art.get_price() / 100.0, art.get_qt());
   int qt = inputInt_check();
   if (qt == 0)
   {
      return; // Typing "0" cancels the transaction
   }
   qtCheck(qt, art);
   int tot = qt * art.get_price();
   cout << format("\nSelected quantity: {}. TOTAL: {:.2f}€.\n*Denominations higher than 20€ will not be accepted.*\nInsert money... (type \"0\" to cancel)\n", qt, tot / 100.0);
   auto payed = 0; // Initializes the variable to start the payment process
   while (payed < tot)
   {
      auto money = inputDouble_check();
      if (money == 0)
      {
         return; // Typing "0" cancels the transaction
      }
      auto cents = static_cast<int>(round(money * 100));
      typeCheck(cents);
      payed += cents;
      if (payed < tot)
      {
         cout << format("Payed: {:.2f}€, remaining: {:.2f}€.\nInsert more money...\n", payed / 100.0, (tot - payed) / 100.0);
      }
   }
   cout << "------------------------------" << endl;
   if (payed > tot)
   {
      cout << format("Here are your products. The change is {:.2f}€.\n", (payed - tot) / 100.0);
   }
   else
   {
      cout << "Here are your products. ";
   }
   cout << "Have a nice day :)\n"
        << endl;
   vendingMachine::set_earnings(tot); // Counts the total as earnings
   art.set_qt(qt);                    // Reduces the article quantity
}
