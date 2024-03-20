#pragma once
#ifndef SM3_H
#define SM3_H

#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdint.h>

namespace SM {
using word = std::bitset<32>;
using bit512 = std::bitset<512>;
using u32 = uint32_t;

struct SM3 {
  std::vector<word> IV {
    word((u32)0x7380166f),
    word((u32)0x4914b2b9),
    word((u32)0x172442d7),
    word((u32)0xda8a0600),
    word((u32)0xa96f30bc),
    word((u32)0x163138aa),
    word((u32)0xe38dee4d),
    word((u32)0xb0fb0e4e),
  };

  std::string msg;

  std::vector<bit512> msgs;

  int msg_len;

  std::vector<word> hash;

  SM3() {}

  SM3(const std::string& m) {
    this->msg = "";
    for (auto& c : m) {
      this->msg += std::bitset<8>(c).to_string();
    }
    this->padding();
  }

  word Tj (int j) {
    if (0 <= j && j <= 15) {
      return word((u32)0x79cc4519);
    } else if (16 <= j && j <= 63) {
      return word((u32)0x7a879d8a);
    }
    return word(0);
  }

  void padding() {
    auto len = this->msg.length();
    this->msg += '1';

    int k = 447 - len;
    while (k <= 0) {
      k += 512;
    }
    
    this->msg += std::string(k, '0');
    this->msg += std::bitset<64>((u32)len).to_string();

    auto nlen = this->msg.length();
    this->msg_len = nlen >> 9;
    this->msgs.reserve(this->msg_len);

    for (int i = 0; i < this->msg_len; ++i) {
      this->msgs.push_back(bit512(this->msg.substr(i*512, 512)));
    }
  }

  word FF(int j, word X, word Y, word Z) {
    if (0 <= j && j <= 15) {
      return X ^ Y ^ Z;
    } else if (16 <= j && j <= 63) {
      return (X & Y) | (X & Z) |(Y & Z);
    }
    return word(0);
  }

  word GG(int j, word X, word Y, word Z) {
    if (0 <= j && j <= 15) {
      return X ^ Y ^ Z;
    } else if (16 <= j && j <= 63) {
      return (X & Y) | ((~X) & Z);
    }
    return word(0);
  }

  word P0(word X) {
    return X ^ this->rLeftShift(X, 9) ^ this->rLeftShift(X, 17);
  }

  word P1(word X) {
    return X ^ this->rLeftShift(X, 15) ^ this->rLeftShift(X, 23);
  }

  word rLeftShift(word w, int x) {
    x %= 32;
    word r = w;
    r = r << x;
    w = w >> (32 - x);
    r = r | w;
    return r;
  }

  std::vector<word> getW(const bit512& B) {
    std::vector<word> W;
    std::string sB = B.to_string();
    
    for (int i = 0; i < 16; ++i) {
      W.push_back(word(sB.substr(i * 32, 32)));
    }

    for (int i = 16; i < 68; ++i) {
      W.push_back(
        this->P1(W[i-16] ^ W[i-9] ^ this->rLeftShift(W[i-3], 15))
        ^ this->rLeftShift(W[i-13], 7)
        ^ W[i-6]
      );
    }

    for (int i = 0; i < 64; i++) {
      W.push_back(W[i] ^ W[i+4]);
    }

    return W;
  }
  
  void CF(std::vector<word>& V, bit512 BB, std::vector<word>& NV) {
    auto W = this->getW(BB);

    word A = V[0];
    word B = V[1];
    word C = V[2];
    word D = V[3];
    word E = V[4];
    word F = V[5];
    word G = V[6];
    word H = V[7];

    word SS1, SS2, TT1, TT2;

    for (int j = 0; j < 64; ++j) {
      SS1 = this->rLeftShift(
        word(this->rLeftShift(A, 12).to_ulong() + E.to_ulong() + this->rLeftShift(this->Tj(j), j).to_ulong()),
        7
      );

      SS2 = SS1 ^ this->rLeftShift(A, 12);

      TT1 = word(
        this->FF(j, A, B, C).to_ulong()
        + D.to_ulong() + SS2.to_ulong() + W[j+68].to_ulong()
      );

      TT2 = word(
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

      if (j == 33) {
        A = word((u32)0x9ccd089c);
        E = word((u32)0xda8c835f);
      }
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

  void digest() {
    auto V0 = this->IV;
    auto V1 = V0;

    for (int i = 0; i < this->msg_len; ++i) {
      this->CF(V0, this->msgs[i], V1);
      V0 = V1;
    }

    this->hash = V1;
  }

  std::string getHash() {
    this->digest();
    std::string str = "";
    for (auto& w : this->hash) {
      std::stringstream ss;
      ss << std::setbase(16) << std::setfill('0') << std::setw(8) << w.to_ulong();
      str += ss.str();
    }
    return str;
  }
};
}
#endif
