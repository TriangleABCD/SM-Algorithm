#include <iostream>
#include <bitset>
#include <string>
#include <functional>

using WORD = std::bitset<32>;

using BIT512 = std::bitset<512>;

struct SM3 {
    std::vector<WORD> IV{
        WORD((unsigned long)0x7380166f),
        WORD((unsigned long)0x4914b2b9),
        WORD((unsigned long)0x172442d7),
        WORD((unsigned long)0xda8a0600),
        WORD((unsigned long)0xa96f30bc),
        WORD((unsigned long)0x163138aa),
        WORD((unsigned long)0xe38dee4d),
        WORD((unsigned long)0xb0fb0e4e),
    };

    std::function<WORD(int)> Tj = [](int j) -> WORD {
        if(0 <= j && j <= 15) {
            return WORD((unsigned long)0x79cc4519);
        } else if(16 <= j && j <= 63) {
            return WORD((unsigned long)0x7a879d8a);
        } else {
            return WORD(0);
        }
    };

    std::string msg_origin;

    std::string msg_padding;

    std::vector<BIT512> msg;

    int n;
    
    std::vector<WORD> hash;

    SM3();

    SM3(const std::string m);

    void padding();

    WORD FF(int j, WORD X, WORD Y, WORD Z);

    WORD GG(int j, WORD X, WORD Y, WORD Z);

    WORD P0(WORD X);

    WORD P1(WORD X);

    WORD rLeftShift(WORD w, int x);

    std::vector<WORD> getW(BIT512& B);

    void CF(std::vector<WORD>& V, BIT512 B, std::vector<WORD>& NV);

    void digest();

    std::string getHash();

};

