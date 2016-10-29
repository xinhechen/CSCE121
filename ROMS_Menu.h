
#ifndef ROMS_MENU_H_
#define ROMS_MENU_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
//other types
#include "Category.h"
#include "Menu_Item.h"
#include "Recipe.h"
#include "Ingredient.h"
#include "Order.h"
#include "Order_Item.h"
#include "std_lib_facilities.h"

using namespace std;

namespace ROMS{   
	
	enum Msg_type{
	Exit_program=1, About_info, About_help, Close_about_box,
	Read_recipes_ingredients, Read_orders, Read_category_menu_items,
	Show_order, Show_recipe, Show_menu,
	Find_category_sales, Find_table_sales, Find_menu_item_sales,
	Update_add_order_item, Update_add_menu_item, Update_add_recipe,
	Tables_button, Orders_button, Categories_button, Recipes_button, Menu_items_button,
	Display_window, Menu_bar_cb,Graph_sales
	};
   
	struct InvalidType : std::runtime_error {
	  InvalidType(): runtime_error("Invalid type data") {}
	};

class ROMS_Menu {
public:
   struct InvalidData : std::runtime_error {
      InvalidData(): runtime_error("Invalid input data format") {}
   };

   struct InvalidFile : std::runtime_error {
      InvalidFile(): runtime_error("Invalid input file name") {}
   };

   enum Main_menu_options{ Info=1, Read, Show, Find, Update, Exit }; 

   enum Rec_type{ Cats=1, Menu_entry, Rec, Ingred, Ord, Ord_item };

   enum File_type{	Tbls=Tables_button, 
					CatMI=Categories_button, 
					Prds=Menu_items_button, 
					Carts=Orders_button, 
					Items=Recipes_button,
					
					Prds2=Show_menu,
					Carts2=Show_order,
					Items2=Show_recipe}; 
		
	ROMS_Menu();
   
   void display_main_menu() const;
   void display_info() const;//display assignment info & your name plus "Project Part I"
   void read();		 
   void update();
   void show() const;
   void find() const;
   void read_recipes_ingredients(string);
   void Read_orders(string);
   void read_catprds(string);
   double sales_for_menu_id(int id);  //========================================================================JKB C
   double sales_for_table(int id);
   void put_info(string a, int b, int c, int d);
   void update_menu(int m,int c,int r,string n,double p,string d);
	vector<double> sales_per_months();
   //show functions added to support GUI interface
   string show_button(Msg_type);

	template <typename T>
	string show_func(T t, string s, int n)
	{
		string r;
		if(0==t.size())
			r = "No "+s+" Data Loaded\n";
		else
		{

//----------------------------JKB B.2---------------------------------------
			switch (n)   {
				case 3:
					r = "   ID    -   Name\n\n";
					break;
				case 2:
					r = "Order ID - Server ID - Table ID -     Date      -   Time  \n\n";
					break;
				case 4:
					r = "Item Name        - Item ID - Price\n\n";
					break;
				case 5:
					r = "Seat ID - Order ID - Menu Item ID - Quantity\n\n";
					break;
				default:
					break;
			}
//----------------------------JKB B.2----------------------------------------			
			for(int i=0; i<t.size(); ++i)
			{
				r+=t[i].display();
				r+="\n";
			}
		}
		return r;
	}
private:
   //private data
	vector<Category> categories;
	vector<Menu_Item> menu_items;
	vector<Recipe> recipes;
	vector<Ingredient> ingredients;
	vector<Order> orders;
	vector<Order_Item> order_items;


	int get_file_info(ifstream& ist, string prompt, string fname);	//A.1,2,3 merge added a function for common code
	void read_file(ifstream& file, int num_rec, Rec_type ftype);
	string get_long_string(ifstream& ist);
};
} //end namespace ROMS

#endif //ROMS_MENU_H_
