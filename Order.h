/*
	The directives starting with # below ensures that this file is read by the compiler only once
	even if it is #included several times. It is call an "include guard"
*/
#ifndef ORDER_H_
#define ORDER_H_

//other types
#include "Chrono.h"					
#include <string>
#include <sstream>
#include "std_lib_facilities.h"
using namespace std;
using namespace Chrono;

struct Order {
public:
	//data

	// constructors
	Order () {}
	Order(int oid, int sid, int tid, Date od, Time ot):
	order_id(oid), server_id(sid), table_id(tid), order_date(od), order_time(ot) {}
   // utility functions
	string display() const {	
		ostringstream oss;
		oss << setw(6) << order_id << setw(11) << server_id << setw(12) << table_id << "      " << order_date << "\t   " << order_time; //JKB B.2
		return oss.str();
	}
	int get_table_id() const {return table_id;}	//B.2.a
	int get_order_id() const {return order_id;}	//B.2.a
	Date get_date() const {return order_date;}
private:
   //constants

   // private data
	int order_id;
	int server_id;
	int table_id;
	Date order_date;
	Time order_time;
};


#endif //ORDER_H_
