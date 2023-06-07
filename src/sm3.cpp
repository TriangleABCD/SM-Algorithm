#include "sm3.h"

SM3::SM3() {
}


SM3::SM3(const std::string m) {
    std::string bit_str = "";
    for(auto& c : m) {
        bit_str += std::bitset<8>(c).to_string();
    }
    this->msg_origin = bit_str;
    this->padding();
}


void SM3::padding() {
    this->msg_padding = this->msg_origin;

    int l = int(this->msg_padding.size());

    this->msg_padding += "1";

    int k = 447-l;
    while(k <= 0) {
        k += 512;
    }

    this->msg_padding += std::string(k, '0');

    this->msg_padding += std::bitset<64>((unsigned long)l).to_string();
    
    int ll = int(this->msg_padding.size());

    this->n = ll >> 9;

    this->msg.reserve(this->n);

    for(int i = 0; i < this->n; ++i) {
        this->msg.push_back(BIT512(this->msg_padding.substr(i*512, 512)));
    }
}


WORD SM3::FF(int j, WORD X, WORD Y, WORD Z) {
    if(0 <= j && j <= 15) {
        return X ^ Y ^ Z;
    } else if(16 <= j && j <= 63) {
        return (X & Y) | (X & Z) | (Y & Z);
    }
    return WORD(0);
}


WORD SM3::GG(int j, WORD X, WORD Y, WORD Z) {
    if(0 <= j && j <= 15) {
        return X ^ Y ^ Z;
    } else if(16 <= j && j <= 63) {
        return (X & Y) | ((~X) & Z);
    }
    return WORD(0);
}


WORD SM3::P0(WORD X) {
    return X ^ rLeftShift(X, 9) ^ rLeftShift(X, 17);
}


WORD SM3::P1(WORD X) {
    return X ^ rLeftShift(X, 15) ^ rLeftShift(X, 23);
}


WORD SM3::rLeftShift(WORD w, int x) {
    x %= 32;
    WORD res = w;
    res = res << x;
    w = w >> (32 - x);
    res = res | w;
    return res;
}


std::vector<WORD> SM3::getW(BIT512& B) {
    std::vector<WORD> W(132);
    std::string sB = B.to_string();

    for(int i = 0; i < 16; ++i) {
        W[i] = std::move(WORD(sB.substr(i * 32, 32)));
    }

    for(int i = 16; i < 68; ++i) {
        W[i] = std::move(
            (this->P1((W[i-16]) ^ (W[i-9]) ^ (this->rLeftShift(W[i-3], 15)))) 
            ^ (this->rLeftShift(W[i-13], 7)) 
            ^ (W[i-6])
        );
    }

    for(int i = 0; i < 64; ++i) {
        W[i+68] = std::move(W[i] ^ W[i+4]);
    }
    return W;
}


void SM3::CF(std::vector<WORD>& V, BIT512 BB, std::vector<WORD>& NV) {
    auto W = this->getW(BB);

    WORD A = V[0];
    WORD B = V[1];
    WORD C = V[2];
    WORD D = V[3];
    WORD E = V[4];
    WORD F = V[5];
    WORD G = V[6];
    WORD H = V[7];

    WORD SS1, SS2, TT1, TT2;

    for(int j = 0; j < 64; ++j) {
        SS1 = this->rLeftShift(
            WORD(this->rLeftShift(A, 12).to_ulong() + E.to_ulong() + this->rLeftShift(this->Tj(j), j).to_ulong()),
            7
        );

        SS2 = SS1 ^ this->rLeftShift(A, 12);

        TT1 = WORD(
            this->FF(j, A, B, C).to_ulong()
            + D.to_ulong() + SS2.to_ulong() + W[j+68].to_ulong()
        );

        TT2 = WORD(
            this->GG(j, E, F, G).to_ulong()
            + H.to_ulong() + SS1.to_ulong() + W[j].to_ulong()
        );

        D = C;
        C = this->rLeftShift(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = this->rLeftShift(F, 19);
        F = E;
        E = this->P0(TT2);

        std::cout << A.to_string() << "\n";
        std::cout << B.to_string() << "\n";
        std::cout << C.to_string() << "\n";
        std::cout << D.to_string() << "\n";
        std::cout << E.to_string() << "\n";
        std::cout << F.to_string() << "\n";
        std::cout << G.to_string() << "\n";
        std::cout << H.to_string() << "\n";
    }

    NV[0] = V[0] ^ A;
    NV[1] = V[1] ^ B;
    NV[2] = V[2] ^ C;
    NV[3] = V[3] ^ D;
    NV[4] = V[4] ^ E;
    NV[5] = V[5] ^ F;
    NV[6] = V[6] ^ G;
    NV[7] = V[7] ^ H;
}


void SM3::digest() {
    auto V0 = this->IV;
    auto V1 = V0;

    for(int i = 0; i < this->n; ++i) {
        this->CF(V0, this->msg[i], V1);
        V0 = V1;
    }

    this->hash = V1;
}


std::string SM3::getHash() {
    this->digest();
    std::string res = "";
    for(auto& w: this->hash) {
        res += w.to_string();
    }
    return res;
}
