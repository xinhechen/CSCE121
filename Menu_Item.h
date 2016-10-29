#ifndef MENU_ITEM_H_
#define MENU_ITEM_H_

#include <string>

// other types
#include "Description.h"				//uncomment once you have added the type definition
using namespace std;

struct Menu_Item
{
public:
	// constructors
	Menu_Item (int id, int cat, int rec, string n, double amt, Description s)	//uncomment once you have added the type definition
		: menu_item_id(id), cat_id(cat), recipe_id(rec), menu_item_name(n), price(amt), descr(s) {}

   // utility functions
	string display() const {	//B.1.b
		ostringstream oss;                                                    //VVV ----------------JKB B.2----------------------- VVV
		oss << setw(15) << left << menu_item_name << "  -   " <<setw(4) <<  menu_item_id << "  -  " << setw(4) << price << endl << "\tDescription: " << descr.display() << "\n";
		return oss.str();
	}	
	string get_name() const {return menu_item_name;} //B.1.a
	int get_recipe_id() const {return recipe_id;} //B.1.a
	string get_description() const {return descr.display();}	//B.1.a
	int get_menu_item_id() const {return menu_item_id;}	//B.2.a
	double get_price() const {return price;}	//B.2.a
	int get_cat_id() const { return cat_id;}

private:
   //constants
     
   // private data
	int menu_item_id;
	int cat_id;
	int recipe_id;
	string menu_item_name;
	double price;
	Description descr;			//uncomment once you have added the type definition
   
};


#endif //MENU_ITEM_H_
