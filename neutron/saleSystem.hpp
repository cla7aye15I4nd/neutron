#ifndef SALESYSTEM_HPP
#define SALESYSTEM_HPP

#include "user.hpp"
//#include "train.hpp"
//#include "ticket.hpp"

class saleSystem {
	const int orderN = 17;
	const std::string orders[17] = { 
		"register", "login", "query_profile", "modify_profile", "modify_privilege",
		"query_ticket", "query_transfer", "buy_ticket", "query_order", "refund_ticket",
		"add_train", "sale_train", "query_train", "delete_train", "modify_train",
		"clean", "exit"
	};
	user User;
	//train Train;
	//ticket Ticket;
public:
	saleSystem() {}
	~saleSystem() {}
	void processing() {
		std::string str;
		while (1) {
			getline(std::cin, str);
			ss.clear();
			ss << str;
			std::string first;
			ss >> first;
			int p = 0;
			while (p < orderN && orders[p] != first) p++;
			switch (p)
			{
				case 0:printf("%d\n", User.reg()); break;
				case 1:printf("%d\n", User.log()); break;
				case 2:printf("%s\n", User.qry().c_str()); break;
				case 3:printf("%d\n", User.mod()); break;
				case 4:printf("%d\n", User.mop()); break;
			default:
				break;
			}
		}
	}
};

#endif // !SALESYSTEM_HPP
