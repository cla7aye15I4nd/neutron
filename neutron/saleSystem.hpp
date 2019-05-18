#ifndef SALESYSTEM_HPP
#define SALESYSTEM_HPP

#include "user.hpp"
#include "train.hpp"
#include "hash.hpp"
#include "bitManager.hpp"

class saleSystem {
	static const int orderN = 17;
	const std::string string_orders[orderN] = { 
		"register", "login", "query_profile", "modify_profile", "modify_privilege",
		"query_ticket", "query_transfer", "buy_ticket", "query_order", "refund_ticket",
		"add_train", "sale_train", "query_train", "delete_train", "modify_train",
		"clean", "exit"
	};
	str<20> orders[orderN];
	user User;
	train Train;
public:
	saleSystem() {
		for (int i = 0; i < orderN; i++)
			orders[i] = string_orders[i];
	}
	~saleSystem() {}
	void processing() {
		str<20> order;
		while (1) {
			scanf("%s", order.ch);
			int p = 0;
			while (p < orderN && orders[p] != order) p++;
			switch (p)
			{
				case 0:printf("%d\n", User.reg()); break;
				case 1:printf("%d\n", User.log()); break;
				case 2:User.qry(); break;
				case 3:printf("%d\n", User.mod()); break;
				case 4:printf("%d\n", User.mop()); break;
				case 5:Train.qryDirect(); break;
				case 6:Train.qryTrans(); break;
				case 7:printf("%d\n", Train.buy()); break;
				case 8:Train.qryTicket(); break;
				case 9:printf("%d\n", Train.refund()); break;
				case 10:printf("%d\n", Train.add()); break;
				case 11:printf("%d\n", Train.sale()); break;
				case 12:Train.qry(); break;
				case 13:printf("%d\n", Train.del()); break;
				case 14:printf("%d\n", Train.mod()); break;
				case 15:
					printf("1\n");
					User.clear();
					Train.clear();
					break;
				case 16:
					printf("BYE\n");
					exit(0);
			default:
				break;
			}
		}
	}
};

#endif // !SALESYSTEM_HPP
