#ifndef ORDER_ITEM_H_
#define ORDER_ITEM_H_

#include <string>
#include <sstream>
using namespace std;


struct Order_Item {
public:
	// constructors
	Order_Item () {}
	Order_Item(string sid, int oid, int miid, int n):
	seat_id(sid), order_id(oid), menu_item_id(miid), prod_qty(n) {}
   // utility functions
	string display() const {	
		ostringstream oss;
		oss << setw(4) << seat_id << setw(12) << order_id << setw(13) << menu_item_id << setw(11) << prod_qty ;  //JKB B.2
		return oss.str();
	}
	int get_order_id() const {return order_id;}	//B.2.a
	int get_qty() const {return prod_qty;}	//B.2.a
	int get_menu_item_id() const {return menu_item_id;}	//B.2.a
private:
   //constants  

   // private data
	string seat_id;
	int order_id;
	int menu_item_id;
	int prod_qty;
};


#endif //ORDER_ITEM_H_
