#ifndef SALESYSTEM_HPP
#define SALESYSTEM_HPP

#include "train.hpp"
#include "hash.hpp"
#include "bitManager.hpp"
#include "user_system.hpp"

const std::string operators[] = {
    "exit",
    "clean",
    "login", 
    "add_train",
    "buy_ticket",
    "sale_train",
    "delete_train",
    "register", "refund_ticket",
    "modify_profile", "modify_privilege", "modify_train",
    "query_ticket", "query_transfer", "query_order", "query_train", "query_profile"
};

class saleSystem {
    UserSystem user_system;
    train Train;

public:
    void processing() {
        char oper[20];
        
        while (true) {
            scanf("%s", oper);
            switch (oper[0]) {
            case 'e' :  //exit
                puts("BYE");
                return;
            case 'c': //clean
                printf("1\n");
                user_system.clear();
                Train.clear();
                hashT.clear();
                hashC.clear();
                break;
            case 'l': //login
                user_system.login();
                break;
            case 'a': //add_train
                printf("%d\n", Train.add());
                break;
            case 'b': //buy_ticket
                printf("%d\n", Train.buy());
                break;
            case 's': //sale_train
                printf("%d\n", Train.sale());
                break;
            case 'd': //delete
                printf("%d\n", Train.del());
                break;
            case 'r':
                if (oper[2] == 'g') user_system.append(); //register
                else printf("%d\n", Train.refund()); //refund_ticket
                break;
            case 'm':
                if (oper[9] == 'o') user_system.modify_profile(); //modify_profile
                else if (oper[9] == 'i') user_system.modify_privilege(); //modify_privilege
                else printf("%d\n", Train.mod()); //modify_train
                break;
            case 'q':
                if (oper[9] == 'k') Train.qryDirect(); //query_ticket
                else if (oper[9] == 'n') Train.qryTrans(); //query_transfer
                else if (oper[9] == 'e') Train.qryTicket();//query_order
                else if (oper[9] == 'i') Train.qry();
                else user_system.query_profile();
                break;
            default:
                break;
            }
        }
    }
};

#endif
