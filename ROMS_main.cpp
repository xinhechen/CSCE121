//============================================================================
// Name        : ROMS_main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : C++ project Part II for your class (121, or 112) 
//============================================================================
//
#include "Simple_window.h"
#include "Graph.h"
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>

#include "ROMS_Menu.h"

using namespace std;
using namespace ROMS;
using namespace Graph_lib;

//types we need for a ROMS GUI interface-- a memu_bar and a place to display text and a means of attaching them to a Window.

namespace Graph_lib {
//------------------------------------------------------------------------------
	struct Window_text_display : Widget {
		Window_text_display(Point XY, int W, int H, string L, Callback cb): Widget(XY, W, H, L, cb)
		{}
		void set_buffer(string s);
		void attach(Window&);
	private:
		Fl_Text_Buffer data;
		Fl_Text_Display *ptd;

	};
	void Window_text_display::attach(Window& win)
	{
		ptd = new Fl_Text_Display(loc.x, loc.y, width, height, label.c_str());
		ptd->buffer(&data);
		pw  = (Fl_Widget*) ptd;
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
		own = &win;
	}
	void Window_text_display::set_buffer(string s) 
	{
		data.text(s.c_str());
	}

//------------------------------------------------------------------------------
	struct Menu_bar : Widget {
		Menu_bar(Point XY, int W, int H, Callback cb): Widget(XY, W, H, "", cb)
		{}
		void menu(Fl_Menu_Item *ma);
		void attach(Window&);
	private:
		Fl_Menu_Bar *pmb;
		Fl_Menu_Item *ma;

	};
	void Menu_bar::attach(Window& win)
	{
		pmb = new Fl_Menu_Bar(loc.x, loc.y, width, height);
		pmb->menu(ma);
		pw  = (Fl_Widget*) pmb;
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
		own = &win;
	}
	void Menu_bar::menu(Fl_Menu_Item *m) 
	{
		ma = m;
	}
}

//header declarations
void do_about_box(Window&);
void do_read(Window&, ROMS_Menu&, string, string, Msg_type);
void Main_Window_CB(Fl_Widget*, void*);
void Menu_Bar_CB (Fl_Widget*, void*);

void do_update_order_item(Window& w,ROMS_Menu&m);             //==========================================JKB C
void do_find_menu_item_sales(Window& w,ROMS_Menu& m);         //==========================================JKB C
void do_update_menu_item(Window& w,ROMS_Menu&m);
void do_find_table_sales(Window& w, ROMS_Menu&m);
//global data
bool main_window_click = false;
bool menu_bar_click = false;
bool main_click_enabled = true;//used to force serialization on clicks
int window_userdata;
bool button_click=false; //========================================================================JKB C


void do_graph(Window& w,ROMS_Menu&m);  //=========================================================Part D


//main window menu bar
//Note: for expediancy sake, to define the menu bar below we used an FLTK type Fl_Menu_Item. 
//This makes our program  directly tied to FLTK instead of Dr. Stroustrup's interface Graph_lib.
//We don't like this, but we did it to save time, which often happens in the real world.
//A better solution is to write code to hide this in the implementation of the Graph_lib::Menu_bar, but that means adding
//changes to it and getting general agreement with Dr. Stroustrup. Subsequent testing and distribution would take time we don't have.
//An extra credit exercise is to see if you can eliminate direct references in your code to FLTK types like we have here
Fl_Menu_Item menu_bar[] = {
 {"Read", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Recipes", 0,  (Fl_Callback*)Main_Window_CB, Address (Read_recipes_ingredients), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Orders", 0,  (Fl_Callback*)Main_Window_CB, Address (Read_orders), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Menu Items", 0,  (Fl_Callback*)Main_Window_CB, Address (Read_category_menu_items), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Show", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Orders", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_order), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Recipes", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_recipe), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Menu Items", 0,  (Fl_Callback*)Main_Window_CB, Address (Show_menu), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Find", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Category Sales", 0,  (Fl_Callback*)Main_Window_CB, Address (Find_category_sales), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Table Sales", 0,  (Fl_Callback*)Main_Window_CB, Address (Find_table_sales), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Menu Item Sales", 0,  (Fl_Callback*)Main_Window_CB, Address (Find_menu_item_sales), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Update", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Add Order Item", 0,  (Fl_Callback*)Main_Window_CB, Address (Update_add_order_item), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Add Menu Item", 0,  (Fl_Callback*)Main_Window_CB, Address (Update_add_menu_item), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Add Recipe", 0,  (Fl_Callback*)Main_Window_CB, Address (Update_add_recipe), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},

//=====================================================Part D===============================================================

 {"Graph", 0, 0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Order Sales by Month", 0, (Fl_Callback*)Main_Window_CB, Address (Graph_sales), 0, FL_NORMAL_LABEL, 0, 14, 0}, 
 {0,0,0,0,0,0,0,0,0},

//=====================================================Part D===============================================================
 {"About", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Info", 0,  (Fl_Callback*)Main_Window_CB, Address (About_info), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Help", 0,  (Fl_Callback*)Main_Window_CB, Address (About_help), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Exit", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Save and exit", 0,  (Fl_Callback*)Main_Window_CB, Address (Exit_program), 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};


//call back functions
void recps_cb(Address addr, Address) // callback for recipes button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Tables_button));
}
void order_cb(Address addr, Address) // callback for orders button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Orders_button));
}
void cat_cb(Address addr, Address) // callback for category button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Categories_button));
}
void mi_cb(Address addr, Address) // callback for menu item button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Menu_items_button));
}
void oi_cb(Address addr, Address) // callback for order item button
{
	Main_Window_CB((Fl_Widget*) addr, Address (Recipes_button));
}
 void display_cb(Address addr, Address) // callback for display window
{
	Main_Window_CB((Fl_Widget*) addr, Address (Display_window));
}
 void menu_bar_cb(Address addr, Address)//callback for menu bar
 {
	 Menu_Bar_CB((Fl_Widget*) addr, Address (Menu_bar_cb));
 }
  
//================================================= JKB C ===========================================

 void button_cb(Address addr, Address)
 {
	button_click=true;
 }

//================================================= JKB C ===========================================

void Menu_Bar_CB (Fl_Widget*, void* ud) {
	cout << "Menu Bar Callback" << " Userdata=" << (int) ud << "\n";
	menu_bar_click = true;
}

void Main_Window_CB(Fl_Widget*, void* ud) {
	if(main_click_enabled)//don't signal the window click if main loop is active
	{
		cout << "Main Window Callback" << " Userdata=" << (int) ud << "\n";
		window_userdata = (int) ud;
		main_window_click = true;
		main_click_enabled = false; //main loop active on current click
	}
}
//wait functions for callbacks to be executed
void wait_for_main_window_click() {
	while (!main_window_click) 
		Fl::wait();
	main_window_click = false;
	Fl::redraw();
}

void wait_for_menu_bar_click() {
	while (!menu_bar_click)
		Fl::wait();
	menu_bar_click = false;
	Fl::redraw();
}

int main()
{
	//create the main window and display it
	ROMS_Menu m;
    Window sw(600, 500, "Restaurant Order Management System");
    Button recps(Point(40, 50), 100, 30, "Recipes", recps_cb);
	sw.attach(recps);
	Button order(Point(40, 100), 100, 30, "Orders", order_cb);
	sw.attach(order);
	Button cat(Point(40, 150), 100, 30, "Categories", cat_cb);
	sw.attach(cat);
	Button mi(Point(40, 200), 100, 30, "Menu Items", mi_cb);
	sw.attach(mi);
	Button ci(Point(40, 250), 100, 30, "Order Items", oi_cb);
	sw.attach(ci);
    sw.callback((Fl_Callback*)Main_Window_CB, Address (Exit_program));
	Menu_bar mb(Point(0, 0), 600, 30, menu_bar_cb);
    mb.menu(menu_bar);
	sw.attach(mb);

	Window_text_display t(Point(150,50),400, 400, "Data Display", display_cb );
	t.set_buffer("No Data Loaded\n");
	sw.attach(t);
 	sw.resizable(&sw);
	

	//main loop to execute user clicks on the main window
	//do you understand how this loop works compared to the way the main loop worked in Part I of the project?
	try {
		
		while(true) {
			main_click_enabled = true;
			wait_for_main_window_click();
			cout << "Act on userdata = " << window_userdata << endl;
			switch(window_userdata) 
			{
				//need to complete all the menu cases
				case Read_recipes_ingredients:
					do_read(sw, m, "recipes.dat", "Open Recipes/Ingredients File", Read_recipes_ingredients);
					break;
				case Read_orders:
					do_read(sw, m, "orders.dat", "Open Order/Order Items File", Read_orders);
					break;
				case Read_category_menu_items:
					do_read(sw, m, "catmenu.dat", "Open Category/Menu Item File", Read_category_menu_items);
					break;
				case About_info:
					do_about_box(sw);
					break;
				case Tables_button: case Orders_button: case Categories_button: case Recipes_button: case Menu_items_button:
				case Show_order: case Show_recipe: case Show_menu:  //JKB B.2------------------------------------------------------------
					t.set_buffer(m.show_button((Msg_type) window_userdata).c_str());
					break;

//================================================= JKB C ===========================================
				case Find_menu_item_sales:
					do_find_menu_item_sales(sw, m);
					break;

//---------------------------------------------------------------------------------------------------
				
				case Update_add_order_item:
					do_update_order_item(sw, m);
					break;
//================================================= JKB C ===========================================
				case Find_table_sales:
					do_find_table_sales(sw,m);
					break;
				case Update_add_menu_item:
					do_update_menu_item(sw,m);
					break;

//=====================================================Part D===============================================================
				case Graph_sales:
					do_graph(sw, m);
					break;
//=====================================================Part D===============================================================

				case Exit_program:
					cout << "save files and exit\n";
					exit(0); //exit program
					break;
				default:
					cout << "case not implemented\n";
			}
		}
		return 0;
	}
	catch(exception& e) {
		cerr << "exception: " << e.what() << '\n';
		return 1;
	}
	catch (...) {
		cerr << "Some exception\n";
		return 2;
	}
}
//show about box info window
void do_about_box(Window& w) 
{
	Window ab(Point(w.x()+100, w.y()+100), 400, 250, "About Box");
	ab.color(Color::white);
	ab.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));
	Text msg(Point(15,50), "Ward Project Part II: Restaurant Order Management System");
	msg.set_color(Color::black);
	ab.attach(msg);
	Text namexc(Point(15,90),"Xinhe Chen");
	namexc.set_color(Color::black);
	ab.attach(namexc);
	Image xinhe(Point(100,90),"XinheChen.jpg");
	ab.attach(xinhe);
	Text namejb(Point(200,90),"Justin Brysch");
	namejb.set_color(Color::black);
	ab.attach(namejb);
	Image justin(Point(300,90),"JustinBrysch.jpg");
	ab.attach(justin);
	Text team(Point (15,70),"Team 49 \n Section 519");
	team.set_color(Color::black);
	ab.attach(team);
	wait_for_menu_bar_click();
	ab.detach(msg);//clean up window
	return;
}

//================================================= JKB C ===========================================

void wait_for_button_click()
{
	while (!button_click)
	Fl::wait();
	button_click=false;
	Fl::redraw;
}

//-----------------------------------------

string double_to_string(double d)
{
	stringstream ss;
	ss << d;
	return ss.str();	
}

//------------------------------------------

void do_find_menu_item_sales(Window& w,ROMS_Menu& m)
{
	Window win(Point(w.x()+150, w.y()+50), 400, 100, "Find Menu Item Sales");
	win.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));

	In_box men_id(Point(150,25),50,30,"Enter Menu Item ID: ");
	Button A(Point(250,60),75,30,"Find sales",button_cb);
	Out_box sales(Point(150,60),50,30,"Total Sales: ");
	
	win.attach(men_id);
	win.attach(A);
	win.attach(sales);
	
	wait_for_button_click();
	
	double z = m.sales_for_menu_id(men_id.get_int());
	sales.put(double_to_string(z));
	wait_for_menu_bar_click();
	return;
}
void do_find_table_sales(Window& w,ROMS_Menu& m)
{

	Window win(Point(w.x()+150, w.y()+50), 400, 100, "Find  Table Sales");
	win.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));

	In_box table_id(Point(150,25),50,30,"Enter Table ID: ");
	Button A(Point(250,60),75,30,"Find sales",button_cb);
	Out_box sales(Point(150,60),50,30,"Total Sales: ");
	
	win.attach(sales);
	win.attach(A);
	win.attach(sales);
	
	wait_for_button_click();
	
	double z = m.sales_for_table(table_id.get_int());
	sales.put(double_to_string(z));
	wait_for_menu_bar_click();
	return;




}

//--------------------------------------------

void do_update_order_item(Window& w,ROMS_Menu&m)
{
	Window win(Point(w.x()+150, w.y()+50), 400, 200, "Update Order Items");
	win.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));
	
	In_box sid(Point(150,20),50,30,"Enter Seat ID: ");
	In_box oid(Point(150,60),50,30,"Enter Order ID: ");
	In_box miid(Point(150,100),50,30,"Enter Menu Item ID: ");
	In_box qty(Point(150,140),50,30,"Enter Quantity: ");
	
	win.attach(sid);
	win.attach(oid);
	win.attach(miid);
	win.attach(qty);
	Button B(Point(210,20),50,150,"Update",button_cb);
	win.attach(B);
	wait_for_button_click();
	
	string seat = sid.get_string();
	int order = oid.get_int();
	int menu = miid.get_int();
	int quan = qty.get_int();
	m.put_info(seat, order, menu, quan);
	wait_for_menu_bar_click();
}

void do_update_menu_item(Window& w, ROMS_Menu&m)
{

	Window win(Point(w.x()+150, w.y()+50), 400, 200, "Update Menu Items");
	win.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));
	
	In_box mid(Point(150,20),50,30,"Enter Menu ID: ");
	In_box cid(Point(150,60),50,30,"Enter Category ID: ");
	In_box rid(Point(150,100),50,30,"Enter Recipe ID: ");
	In_box name(Point(150,140),50,30,"Enter Item name: ");
	In_box cost(Point(250,20),50,30,"Enter Price of Item: ");
	In_box descr(Point(250,60),50,30,"Enter Description: ");
	win.attach(mid);
	win.attach(cid);
	win.attach(rid);
	win.attach(name);
	win.attach(cost);
	win.attach(descr);
	Button B(Point(210,20),50,150,"Update",button_cb);
	win.attach(B);
	wait_for_button_click();
	
	int menu = mid.get_int();
	int category = cid.get_int();
	int recipe = rid.get_int();
	string n = name.get_string();
	string p = cost.get_string();
	std::istringstream stm;
	stm.str(p);
	double price;
	stm >>price;

	string description = descr.get_string();
	m.update_menu(menu,category,recipe,n,price,description);
	wait_for_menu_bar_click();
}

//================================================= JKB C ===========================================

//=====================================================Part D===============================================================

void do_graph(Window& w,ROMS_Menu&m)
{
	const int xmax = 700;
	const int ymax = 450;
	const int yspace = 25;
	const int yoffset = 75;
	const int xspace = 50;
	const int xoffset = 100;
	const int xlength = xmax - xspace - xoffset;
	const int ylength = ymax - yspace - yoffset;
	const int yorig = ylength + yspace;
	const int months = 11;
	const int sale = 7;
	const int xscale = xlength / months;

	Window win(Point(w.x()+100, w.y()+25), xmax, ymax, "Sales per Month");
	win.callback((Fl_Callback*)Menu_Bar_CB, Address (Close_about_box));
	Axis x(Axis::x,Point(xoffset,yorig),xlength,months,"");
	Axis y(Axis::y,Point(xoffset,yorig),ylength,sale,"Total Sales");
	vector<double> sales = m.sales_per_months();	
	Open_polyline spm;
	int j = xoffset;
	for (int i = 0; i<sales.size() ; i++)
	{
		spm.add(Point(j,yorig-(sales[i])));
		j = j + xscale;
	}
	spm.set_color(Color::red);
	Text xlab(Point(25, ymax-50), "Month:");
	Text xlab2(Point(25, ymax-25),"Sales:");

	Text m_lab1(Point(xoffset-10,ymax-50), "Jan");
	Text m_lab2(Point(xoffset-10+(xscale),ymax-50), "Feb");
	Text m_lab3(Point(xoffset-10+(2*xscale),ymax-50), "Mar");
	Text m_lab4(Point(xoffset-10+(3*xscale),ymax-50), "Apr");
	Text m_lab5(Point(xoffset-10+(4*xscale),ymax-50), "May");
	Text m_lab6(Point(xoffset-10+(5*xscale),ymax-50), "Jun");
	Text m_lab7(Point(xoffset-10+(6*xscale),ymax-50), "Jul");
	Text m_lab8(Point(xoffset-10+(7*xscale),ymax-50), "Aug");
	Text m_lab9(Point(xoffset-10+(8*xscale),ymax-50), "Sep");
	Text m_lab10(Point(xoffset-10+(9*xscale),ymax-50), "Oct");
	Text m_lab11(Point(xoffset-10+(10*xscale),ymax-50), "Nov");
	Text m_lab12(Point(xoffset-10+(11*xscale),ymax-50), "Dec");
	
	Text sale_lab1(Point(xoffset - 25, ymax - yoffset -45), "50");
	Text sale_lab2(Point(xoffset - 35, ymax - yoffset -95), "100");
	Text sale_lab3(Point(xoffset - 35, ymax - yoffset -145), "150");
	Text sale_lab4(Point(xoffset - 35, ymax - yoffset -195), "200");
	Text sale_lab5(Point(xoffset - 35, ymax - yoffset -245), "250");
	Text sale_lab6(Point(xoffset - 35, ymax - yoffset -295), "300");
	Text sale_lab7(Point(xoffset - 35, ymax - yoffset -345), "350");
	
	Text tot1(Point(xoffset-5,ymax-25), double_to_string(sales[0]));
	Text tot2(Point(xoffset-5+xscale,ymax-25), double_to_string(sales[1]));
	Text tot3(Point(xoffset-5+(2*xscale),ymax-25), double_to_string(sales[2]));
	Text tot4(Point(xoffset-10+(3*xscale),ymax-25), double_to_string(sales[3]));
	Text tot5(Point(xoffset-5+(4*xscale),ymax-25), double_to_string(sales[4]));
	Text tot6(Point(xoffset-10+(5*xscale),ymax-25), double_to_string(sales[5]));
	Text tot7(Point(xoffset-5+(6*xscale),ymax-25), double_to_string(sales[6]));
	Text tot8(Point(xoffset-5+(7*xscale),ymax-25), double_to_string(sales[7]));
	Text tot9(Point(xoffset-5+(8*xscale),ymax-25), double_to_string(sales[8]));
	Text tot10(Point(xoffset-8+(9*xscale),ymax-25), double_to_string(sales[9]));
	Text tot11(Point(xoffset-5+(10*xscale),ymax-25), double_to_string(sales[10]));
	Text tot12(Point(xoffset-5+(11*xscale),ymax-25), double_to_string(sales[11]));

	win.attach(spm);
	win.attach(x);
	win.attach(y);
	win.attach(xlab);
	win.attach(xlab2);
	win.attach(tot1);
	win.attach(tot2);
	win.attach(tot3);
	win.attach(tot4);
	win.attach(tot5);
	win.attach(tot6);
	win.attach(tot7);
	win.attach(tot8);
	win.attach(tot9);
	win.attach(tot10);
	win.attach(tot11);
	win.attach(tot12);
	win.attach(sale_lab1);
	win.attach(sale_lab2);
	win.attach(sale_lab3);
	win.attach(sale_lab4);
	win.attach(sale_lab5);
	win.attach(sale_lab6);
	win.attach(sale_lab7);
	win.attach(m_lab1);
	win.attach(m_lab2);
	win.attach(m_lab3);
	win.attach(m_lab4);
	win.attach(m_lab5);
	win.attach(m_lab6);
	win.attach(m_lab7);
	win.attach(m_lab8);
	win.attach(m_lab9);
	win.attach(m_lab10);
	win.attach(m_lab11);
	win.attach(m_lab12);

	wait_for_menu_bar_click();
}

//=====================================================Part D===============================================================



void do_read(Window& w, ROMS_Menu& m, string dfn, string msg, Msg_type type)
{
	//Here again we linked our code directly to the FLTK library instead of creating an interface in our Graph_lib interface to FLTK
	//We did it for lack of time, which reflects the real word. Unfortunately if we wanted to move our code to a different graphics
	//library, we have to change our code and not just our Graphic_lib implementation. we are using the FLTK file chooser directly.
	Fl_File_Chooser fc(dfn.c_str(), "*.dat", Fl_File_Chooser::CREATE, msg.c_str());
	fc.show();
	while (fc.shown())
		Fl::wait();	//wait for OK or Cancel button
	if(0!=fc.value())//make sure there is a string to use as a filename
	{
		string fname = fc.value();
		switch(type) {
			case Read_recipes_ingredients:
				m.read_recipes_ingredients(fname);
				break;
			case Read_orders:
				m.Read_orders(fname);
				break;
			case Read_category_menu_items:
				m.read_catprds(fname);
				break;
			default:
				throw InvalidType();
		}
	}
	return;
}
