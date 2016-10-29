#include "ROMS_Menu.h"

//added includes
#include <fstream>
#include <algorithm>	//B.1.b
#include <sstream>
using namespace std;
using namespace ROMS;

ROMS_Menu::ROMS_Menu()
{
}

void ROMS_Menu::display_main_menu() const 
{
	//A.3
	cout << "\nMAIN MENU \n";
	cout << "1-Info 2-Read 3-Show 4-Find 5-Update 6-Exit\n";
}
void ROMS_Menu::display_info() const
{
	//A.3
	cout << "\nWard-team Project Part I: Restaurant Order Management System\n";
}

void ROMS_Menu::read_recipes_ingredients(string fname)
{
	//A.1, 2 & 3 merge
	ifstream ist;

	//A.1 read ingredients	//should error check
	int num_rec = get_file_info(ist, "Please enter recipe data file name: ", fname);
	ingredients.clear();	//start with an empty list
	read_file(ist, num_rec, ROMS_Menu::Ingred);
	cout << "read " << ingredients.size() << " ingredient items \n";
	//A.1 read and store the recipe data	//should error check
	ist >> num_rec;
	recipes.clear();	//start with an empty list
	read_file(ist, num_rec, ROMS_Menu::Rec);
	cout << "read " << recipes.size() << " recipes \n";
	ist.close();
}

void ROMS_Menu::Read_orders(string fname)
{
	//A.2 read and store the order and order item data 
	ifstream ist;
	int num_orders = get_file_info(ist, "Please enter orders data file name: ", fname);
	orders.clear();	//start with an empty list
	read_file(ist, num_orders, ROMS_Menu::Ord);
	cout << "read " << orders.size() << " orders \n";

	//A.2 read order item data	
	int num_items = 0;
	order_items.clear();	//start with an empty list
	ist >> num_items;
	read_file(ist, num_items, ROMS_Menu::Ord_item);
	cout << "read " << order_items.size() << " order items \n";
	ist.close();
}

void ROMS_Menu::read_catprds(string fname)
{
	ifstream ist;
	int num_items;
	//A.3 read the category and menu data
	int num_cats = get_file_info(ist, "Please enter file name for category/menu data: ", fname);
	categories.clear();	//start with an empty list
	read_file(ist, num_cats, ROMS_Menu::Cats);
	cout << "read " << categories.size() << " categories \n";

	//A.3 read menu items	//should error check
	ist >> num_items;	//get number of menu items
	menu_items.clear();	//start with an empty list
	read_file(ist, num_items, Menu_entry);
	cout << "read " << menu_items.size() << " menu items \n";
	ist.close();
	return;
}


void ROMS_Menu::update_menu(int m,int c,int r,string n,double p,string d)
{
	Menu_Item a( m,c,r,n,p,d);
	menu_items.push_back(a);
}

vector<double> ROMS_Menu::sales_per_months()
{
	vector<double> sales;
	for(int x=1;x<=12;x++)
	{
		int qty=0;
		int price=0;
		int total=0;
		for(int o=0;o<orders.size();o++)
		{
			if(orders[o].get_date().month()==x)
			{
				for(int z=0;z<order_items.size();z++)
				{
					if(orders[o].get_order_id()==order_items[z].get_order_id())
					{
						qty=order_items[z].get_qty();
						for(int m=0;m<menu_items.size();m++)
						{
							if(order_items[z].get_menu_item_id()==menu_items[m].get_menu_item_id())
							{
								price = menu_items[m].get_price();
								total = total+price*qty;
							}
						}
					}
				}
			}
			
		}
		sales.push_back(total);
	}
	return sales;
}
//================================================= JKB C ===========================================

void ROMS_Menu::put_info(string sid, int oid, int miid, int n)
{
	Order_Item A(sid, oid, miid, n);
	order_items.push_back(A);
}

//================================================= JKB C ===========================================

void ROMS_Menu::show() const 
{	//Below is some old code from Part I. What changes, if any are needed for Part II?
	//B.1, a, b, c merge	//cout << "Show queries \n";
	cout << "Enter Show query number:" << endl << "1-Menu Item Recipe 2-All Menu Items by Chef 3-All Menu Items in a Category\n";
	int query_num;
	cin >> query_num;
	switch (query_num) 
	{
	case 1:
		{
			//B.1.a show a Menu Item Recipe
			string item_name;
			int i;
			cout <<"Enter Menu Item Name\n";
			cin >> item_name;
			for (i = 0; i < (int) menu_items.size(); ++i)
				if (menu_items[i].get_name() == item_name) break;
			if (i == (int) menu_items.size()) {
				cout << "Menu Name Not Found\n";
				break;
			}
			else {//found Menu Item, now find recipe and print it
				cout << menu_items[i].get_description() << endl;
				int rec_id = menu_items[i].get_recipe_id();
				int j;
				for (j = 0; j < (int) recipes.size(); ++j)
					if(rec_id == recipes[j].get_rec_id()) break;
				if (j == (int) recipes.size()) {
					cout << "Recipe ID not found -- data bug\n";
					break;
				}
				//print recipe j
				cout << recipes[j].display() << endl;
				//retrieve and print recipe j ingredients
				for (int k = 0; k < (int) ingredients.size(); ++k)
					if(rec_id == ingredients[k].get_rec_id()) cout << ingredients[k].display() << endl;
				cout << endl;
			}
		}
	case 2: 
		{
			//B.1.b list All Menu Items by Chef
			cout << menu_items.size() << " menu items sorted by chef\n";
			vector<Recipe> temp_recipes = recipes;
			//sort the recipes by chef, then print the menu items in the order of their recipe
			sort(temp_recipes.begin(), temp_recipes.end(), SortRecipesByChef());
			for (int i = 0; i< (int) temp_recipes.size(); ++i) {
				cout << "Chef: " << temp_recipes[i].get_chef() << endl;
				//loop thru sorted recipes and print requested coresponding Menu_Item info
				for (int j = 0; j< (int) menu_items.size(); ++j)
					if (menu_items[j].get_recipe_id() == temp_recipes[i].get_rec_id())
							cout << '\t' << menu_items[i].display() << endl;
			}
			cout << endl;
			break;
		}
	case 3:
		{
			//B.1.c All Menu Items in a Category
			cout << "Not implemented";
			cout << endl;
			break;
		}
	
	default:
		cout << "Invalid request\n";
	}
	
}

//================================================= JKB C ===========================================

double ROMS_Menu::sales_for_menu_id(int id)
{
		int menu_item_id = id;
		double total=0;
		for(int y=0; y < menu_items.size(); y++)
		{			
			if(menu_items[y].get_menu_item_id() == menu_item_id)
			{			
				for(int x=0; x < order_items.size(); x++)
				{
					if(order_items[x].get_menu_item_id() == menu_items[y].get_menu_item_id())
						total = total + (menu_items[y].get_price() * order_items[x].get_qty());				
				}
			}
		}
		return total;
}

//================================================= JKB C ===========================================

double ROMS_Menu::sales_for_table(int id)
{
	int table_id=id;
	double sales_total=0;
	for (int i = 0; i < (int)orders.size(); ++i)
			{
				//select an order that includes this table
				if(table_id == orders[i].get_table_id())
				{
					//add this order sales to the table sales total
					int order_id = orders[i].get_order_id();
					for (int j = 0; j < (int)order_items.size(); ++j)
						//select an order item for the current order
						if(order_id == order_items[j].get_order_id()) 
						{
							int menu_item_id = order_items[j].get_menu_item_id();
							double item_price = 0.;
							for (int k = 0; k < (int) menu_items.size(); ++k) 
								if(menu_item_id == menu_items[k].get_menu_item_id()) 
								{
									item_price = menu_items[k].get_price();
									break;
								}
							//add the sale of this item to the table total
							sales_total += item_price * order_items[j].get_qty();
						}
				}
	}
	return sales_total;
}
void ROMS_Menu::find() const 
{	//Below is some old code from Part I. What changes, if any are needed for Part II?
	//cout << "Find queries \n";
	//B.2.a,b,c merge 
	cout << "Enter find query number:" << endl;
	cout << "1-Total Sales for a Table 2-Total Sales for a Server 3-Total Sales for a Menu Item\n";
	int find_num;
	cin >> find_num;
	switch (find_num) 
	{
	case 1:
		//cout << "Not implemented\n";
		//break;
		{
			//B.2.a calculate total sales for a given table
			//read table id
			//loop thru orders that have that table, pick order items for those orders, calc menu_item price * qty ordered and add to total
			//print total
			int table_id;
			cout << "Enter Table ID: ";
			cin >> table_id;
			//validate category id
			
			double sales_total = 0;
			//total up all sales for this table.
			for (int i = 0; i < (int)orders.size(); ++i)
			{
				//select an order that includes this table
				if(table_id == orders[i].get_table_id())
				{
					//add this order sales to the table sales total
					int order_id = orders[i].get_order_id();
					for (int j = 0; j < (int)order_items.size(); ++j)
						//select an order item for the current order
						if(order_id == order_items[j].get_order_id()) {
							int menu_item_id = order_items[j].get_menu_item_id();
							double item_price = 0.;
							for (int k = 0; k < (int) menu_items.size(); ++k) 
								if(menu_item_id == menu_items[k].get_menu_item_id()) 
								{
									item_price = menu_items[k].get_price();
									break;
								}
							//add the sale of this item to the table total
							sales_total += item_price * order_items[j].get_qty();
						}
				}
			}
			cout << " Table sales = "<< sales_total << endl;
			break;
		}
	case 2: 
		{
		//B.2.b Total Sales for a Server
			
			cout << "Not implemented" << endl;
			break;
		}
	case 3:
		{
			//B.2.c Total Sales for a Menu Item
			cout << "Not Implemented" << endl;
			break;
		}
	default:
		cout << "Invalid request\n";
	}
}
void ROMS_Menu::update()  
{	//This is some old code from Part I. What changes, if any are needed for Part II?
	//B.3.a, b, c merge //cout << "Update queries \n";
	cout << "Enter update query number: " << endl;
	cout << "1-Add New Order Item 2-Add New Menu Item 3-Add New Categories" << endl;
	int query_num;
	cin >> query_num;
	switch (query_num) 
	{
	case 1:
		{
			//B.1 Add new order item to an existing order
			int order_id, menu_item_id, qty;
			string seat_id;
			cout << "Enter Order_id    Menu_item_id    quantity   seat_id\n";
			cin >> order_id >> menu_item_id >> qty >> seat_id;
			if(!cin) throw InvalidData();
			//check that order_id exists
			int n = 0;
			for(n=0; n<(int)orders.size(); ++n)
				if(orders[n].get_order_id() == order_id) break;
			if(!(n < (int)orders.size())||!cin) throw InvalidData();
			//check that menu item id exists
			n = 0;
			for(n=0; n<(int)menu_items.size(); ++n)
				if(menu_items[n].get_menu_item_id() == menu_item_id) break;
			if(!(n < (int)menu_items.size())||!cin) throw InvalidData();
			//check that quantity is positive--maybe should check some reasonable upper bound, too?
			if(qty < 1) 
				throw InvalidData();
			//check that seat is valid
			if(seat_id < "A" || seat_id > "H")
				throw InvalidData();
			
			//add order item
			order_items.push_back(Order_Item(seat_id, order_id, menu_item_id, qty));

			cout << "Number of order items is now " << order_items.size() << endl;
			break;
		}
	case 2: 
		{
			//B.3.b Add new menu item using an existing recipe to an existing category
			cout << "Not Implemented" << endl;
			break;
		}
	case 3:
		{
			//B.3.c Add New Categories Soups, Salads and Sides
			cout << "Not Implemented " << endl;
			break;
		}
	default:
		cout << "Invalid request\n";
	}
}

//open a file and return the expected number of records
int ROMS_Menu::get_file_info(ifstream& ist, string ptompt, string fname)
{	
	//A.1, 2 & 3 merge
	//string fname;
	//cout << prompt;
	//cin >> fname;
	ist.clear();
	ist.open(fname.c_str());
	if (!ist) throw InvalidFile();
	int num;
	ist >> num;
	if (!ist) throw InvalidData();
	return num;
}
void ROMS_Menu::read_file(ifstream& file, int num_rec, Rec_type ftype)
{
	//A.1, 2 & 3 merge
	//Category_ID Cat_name 
	//example category data: 1010	Appetizers	
	int cat_id;
	string cat_name;

	//Menu_item_ID Category_ID RecipeID Menu_item_name Menu_item_price Description #
	//example menu_item data:	2173	1010	3000	Pastel	2	fried flour dumpling filled with vegetables and meat #
	int menu_item_id, /*cat_id,*/ rec_id;
	string item_name;
	double item_price;
	string item_dscrp;

	//Order_ID Server_ID Table_ID Order_date Order_time
	//example order data: 7000	6220	1000	2011	4	4	1	11
	int order_id, server_id, table_id;
	int order_year, order_month, order_day;
	int order_hour, order_minute;

	//Seat_ID Order_ID Menu_item_ID Quantity_ordered
	//example order_item data:	A	7000	2834	1
	string seat_id; 
	/*int order_id, menu_item_id;*/ 
	int ord_qty;

	//Ingredient_ID Recipe_ID Amount_required Units Name 
	//example ingredient data: 6000	3619	5	medium	onionssliced
	int ingred_id; /*rec_id*/ 
	float amt_req;
	string units, name;

	//Recipe_ID Chef_name Instructions # 
		//example recipe data:	3015	Astrid	1. Liquidise the prawn meat till smooth. Mix in the tapioca flour, salt and pepper. Mix well into a stiff dough.
//2. Divide dough into three equal portions. Roll up each portion then place the rolls on a greased plate. Steam for 40 to 45 minutes over high flame.
//3.Leave the rolls to cool, then wrap with a clean tea towel. Chill well in the refrigerator. Use a very sharp knife to slice thinly. Thoroughly dry the cut-out pieces in the sun.
//4. Deep-fry the crackers   #	
	//int rec_id;
	string chef_name;
	string instr;

	//read given file
	int i = 0; 
	while(file && i < num_rec)
	{
		switch(ftype) {	//Rec_type{ Cats=1, Menu_entry, Rec, Ingred, Ord, Ord_item };
			case ROMS_Menu::Rec:
				file >> rec_id >> chef_name;
				instr = get_long_string(file);
				recipes.push_back(Recipe(rec_id, chef_name, Instructions(instr)));
				break;
			case ROMS_Menu::Ingred:
				file >> ingred_id >> rec_id >> amt_req >> units >> name;
				ingredients.push_back(Ingredient(ingred_id, rec_id, amt_req, units, name));
				break;
			case ROMS_Menu::Ord:
				file >> order_id >> server_id >> table_id >> order_year >> order_month >> order_day >> order_hour >> order_minute;
				orders.push_back(Order(order_id, server_id, table_id, Date(order_year, (Date::Month)order_month, order_day), Time(order_hour,order_minute)));
				break;
			case ROMS_Menu::Ord_item:
				file >> seat_id >> order_id >> menu_item_id >> ord_qty;
				order_items.push_back(Order_Item(seat_id, order_id, menu_item_id, ord_qty));
				break;
			case ROMS_Menu::Cats:
				file >> cat_id >> cat_name;
				categories.push_back(Category(cat_id, cat_name));
				break;
			case ROMS_Menu::Menu_entry:
				file >> menu_item_id >> cat_id >> rec_id >> item_name >> item_price;
				item_dscrp = get_long_string(file);
				menu_items.push_back(Menu_Item(menu_item_id, cat_id, rec_id, item_name, item_price, Description(item_dscrp)));
				break;
			default:
				throw InvalidData();
		}
		++i;
	}
	if(i != num_rec || !file) throw InvalidData();
}
string ROMS_Menu::get_long_string(ifstream& file) 
{
	string result, s;
	file >> result;
	if(result == "#") return "";
	char chr;
	file >> noskipws >> chr;
	while (file && !(chr == '#')) {
		stringstream ss;
		ss << result << chr;
		result = ss.str();
		file >> noskipws >> chr;
	}
	file >> skipws;
	return result;
}


//show functions added to support GUI interface

string ROMS_Menu::show_button(Msg_type type)
{
	switch(type) {
	case Tables_button:  case Show_recipe: //JKB B.2-------------------
		return show_func(recipes, "Recipes", 1);
	case Orders_button: case Show_order: //JKB B.2---------------------
		return show_func(orders, "Orders", 2);
	case Categories_button:
		return show_func(categories, "Category", 3);
	case Menu_items_button: case Show_menu:  //JKB B.2-----------------
		return show_func(menu_items, "Menu Items", 4);
	case Recipes_button:
		return show_func(order_items, "Order Items", 5);


	case Find_menu_item_sales:
		return show_func(menu_items, "Menu Item Sales", 6);


	default:
		throw InvalidType();
}

}
