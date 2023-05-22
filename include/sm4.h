#ifndef SM4_H
#define SM4_H

#include <iostream>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

using WORD = bitset<32>;

using BYTE = bitset<8>;

struct SM4 {
    std::vector<WORD> MK{std::vector<WORD>(4)};
    std::vector<WORD> rk{std::vector<WORD>(32)};

    std::vector<WORD> FK{std::vector<WORD>(4)};
    std::vector<WORD> CK{std::vector<WORD>(32)};

    std::vector<WORD> X{std::vector<WORD>(36)};
    std::vector<WORD> Y{std::vector<WORD>(36)};

   
    SM4();
    SM4(std::string msg, std::string key);

    void setMsg(std::string msg);
    void setKey(std::string key);

    void rLeftShift(WORD& w, int x);    

    WORD L(WORD w);

    WORD LL(WORD w);

    BYTE Sbox(BYTE a);

    WORD Tao(WORD w);

    WORD T(WORD w);

    WORD TT(WORD w);

    WORD F(WORD x0, WORD x1, WORD x2, WORD x3, WORD _rk);

    void genRK();

    void encrypt();

    void decrypt();

};

#endif
