#ifndef TERM_HPP
#define TERM_HPP

#include "train.hpp"
#include "hash_train.hpp"
#include "bitmgr.hpp"
#include "user_system.hpp"
#include <string>
#include <fstream>

std::string procStatus() {
    std::ifstream t("/proc/self/status");
    return std::string(std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>());
}

hashCity hashC;
hashTrain hashT;
bitManager bitMgr;
ticketnum_manager ticketMgr;

int bridgeN = 0;
ticketData bridge[100];
char bin[20];
trainData temp;

static const int orderN = 17;
    
const std::string operators[] = { 
    "register", "login", "query_profile", "modify_profile", "modify_privilege",
    "query_ticket", "query_transfer", "buy_ticket", "query_order", "refund_ticket",
    "add_train", "sale_train", "query_train", "delete_train", "modify_train",
    "clean", "exit"
};

class term {
    str<20> orders[orderN];
    UserSystem user_system;
    train train_system;
    bool load_train_system;
    
public:
    term() {
        for (int i = 0; i < orderN; i++)
            orders[i] = operators[i];
        load_train_system = false;
    }
    
    void run_train(int op) {
        if (!load_train_system) {
            load_train_system = true;
            hashC.init();
            hashT.init();
            bitMgr.init();
            train_system.init();
            ticketMgr.init();
        }
        switch (op) {
        case 5: train_system.qryDirect(); break;
        case 6: train_system.qryTrans(); break;
        case 7: printf("%d\n", train_system.buy()); break;
        case 8: train_system.qryTicket(); break;
        case 9: printf("%d\n", train_system.refund()); break;
        case 10: printf("%d\n", train_system.add()); break;
        case 11: printf("%d\n", train_system.sale()); break;
        case 12: train_system.qry(); break;
        case 13: printf("%d\n", train_system.del()); break;
        case 14: printf("%d\n", train_system.mod()); break;
        }
    }
    
    void run() {
        int op;
        str<20> command;
        
        while (true) {
            memset(command.ch, 0, sizeof(command.ch));
            scanf("%s", command.ch);

            for (op = 0; op < 17 && orders[op] != command; ++op);
            
            switch (op) {
            case 0: user_system.append(); break;
            case 1: user_system.login(); break;
            case 2: user_system.query_profile(); break;
            case 3: user_system.modify_profile(); break;
            case 4: user_system.modify_privilege(); break;
            case 15:
                puts("1");
                user_system.clear();
                //TODO
                if (!load_train_system) {
                    hashC.init();
                    hashT.init();
                    bitMgr.init();
                    train_system.init();
                    ticketMgr.init();
                    load_train_system = true;
                }
                train_system.clear();
                ticketMgr.clear();
                hashT.clear();
                hashC.clear();

                break;
            case 16:
                puts("BYE");
                if (load_train_system) {
                    hashC.write_back();
                    hashT.write_back();
                    bitMgr.write_back();
                    train_system.write_back();
                    ticketMgr.write_back();
                }
                exit(0);
            default:
                run_train(op);
            }
        }
    }
};

#endif
