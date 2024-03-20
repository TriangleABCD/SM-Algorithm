#pragma once
#ifndef SM4_H
#define SM4_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <stdint.h>

namespace SM {

using word = std::bitset<32>;
using byte = std::bitset<8>;
using u8 = uint8_t;
using u32 = uint32_t;

struct SM4 {
  std::vector<word> MK {std::vector<word>(4)};
  std::vector<word> rk {std::vector<word>(32)};

  std::vector<word> FK {std::vector<word>(4)};
  std::vector<word> CK {std::vector<word>(32)};

  std::vector<word> X {std::vector<word>(36)};
  std::vector<word> Y {std::vector<word>(36)};

  SM4() {
    this->init();
  }

  SM4(std::string msg, std::string key) {
    this->init();
    this->setMsg(msg);
    this->setKey(key);
  }

  void init() {
    this->FK[0] = std::move(word((u32)0xa3b1bac6));
    this->FK[1] = std::move(word((u32)0x56aa3350));
    this->FK[2] = std::move(word((u32)0x677d9197));
    this->FK[3] = std::move(word((u32)0xb27022dc));

    this->CK[0] = std::move(word((u32)0x00070e15));
    this->CK[1] = std::move(word((u32)0x1c232a31));
    this->CK[2] = std::move(word((u32)0x383f464d));
    this->CK[3] = std::move(word((u32)0x545b6269));
    this->CK[4] = std::move(word((u32)0x70777e85));
    this->CK[5] = std::move(word((u32)0x8c939aa1));
    this->CK[6] = std::move(word((u32)0xa8afb6bd));
    this->CK[7] = std::move(word((u32)0xc4cbd2d9));
    this->CK[8] = std::move(word((u32)0xe0e7eef5));
    this->CK[9] = std::move(word((u32)0xfc030a11));
    this->CK[10] = std::move(word((u32)0x181f262d));
    this->CK[11] = std::move(word((u32)0x343b4249));
    this->CK[12] = std::move(word((u32)0x50575e65));
    this->CK[13] = std::move(word((u32)0x6c737a81));
    this->CK[14] = std::move(word((u32)0x888f969d));
    this->CK[15] = std::move(word((u32)0xa4abb2b9));
    this->CK[16] = std::move(word((u32)0xc0c7ced5));
    this->CK[17] = std::move(word((u32)0xdce3eaf1));
    this->CK[18] = std::move(word((u32)0xf8ff060d));
    this->CK[19] = std::move(word((u32)0x141b2229));
    this->CK[20] = std::move(word((u32)0x30373e45));
    this->CK[21] = std::move(word((u32)0x4c535a61));
    this->CK[22] = std::move(word((u32)0x686f767d));
    this->CK[23] = std::move(word((u32)0x848b9299));
    this->CK[24] = std::move(word((u32)0xa0a7aeb5));
    this->CK[25] = std::move(word((u32)0xbcc3cad1));
    this->CK[26] = std::move(word((u32)0xd8dfe6ed));
    this->CK[27] = std::move(word((u32)0xf4fb0209));
    this->CK[28] = std::move(word((u32)0x10171e25));
    this->CK[29] = std::move(word((u32)0x2c333a41));
    this->CK[30] = std::move(word((u32)0x484f565d));
    this->CK[31] = std::move(word((u32)0x646b7279));
  }
    
  void setMsg(const std::string& msg) {
    for (int i = 3; i >= 0; --i) {
      this->X[i] = word(msg.substr(i*32, 32));
    }
  }

  void setKey(const std::string& key) {
    for (int i = 3; i >= 0; --i) {
      this->MK[i] = word(key.substr(i*32, 32));
    }
  }

  void rLeftShift(word& w, int x) {
    x %= 32;
    auto t = w;
    t >>= (32 - x);
    w <<= x;
    w |= t;
  }    

  word L(word w) {
    auto r = w;
    this->rLeftShift(w, 2);
    r ^= w;
    this->rLeftShift(w, 8);
    r ^= w;
    this->rLeftShift(w, 8);
    r ^= w;
    this->rLeftShift(w, 6);
    r ^= w;
    return r;
  }

  word LL(word w) {
    auto r = w;
    this->rLeftShift(w, 13);
    r ^= w;
    this->rLeftShift(w, 10);
    r ^= w;
    return r;
  }

  byte Sbox(byte a) {
    int x = a.to_ulong() % 16;
    a >>= 4;
    int y = a.to_ulong() % 16;

    static std::vector<std::vector<u8>> box {
      {0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05},
      {0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99},
      {0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62},
      {0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6},
      {0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8},
      {0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35},
      {0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87},
      {0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e},
      {0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1},
      {0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3},
      {0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f},
      {0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51},
      {0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8},
      {0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0},
      {0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84},
      {0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48}
    };

    return byte(box[x][y]);
  }

  word Tao(const word& w) {
    std::vector<byte> bytes(4, byte(0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            bytes[i][j] = w[i*8 + j];
        }
    }
    for (int i = 0; i < 4; ++i) {
        bytes[i] = this->Sbox(bytes[i]);
    }
    word res;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            res[i*8 + j] = bytes[i][j];
        }
    }
    return res;
  }

  word T(const word& w) {
    return L(Tao(w));
  }

  word TT(const word& w) {
    return LL(Tao(w));
  }

  word F(const word& x0, const word& x1, const word& x2, const word& x3, const word& _rk) {
    word tmp;
    tmp ^= x1;
    tmp ^= x2;
    tmp ^= x3;
    tmp ^= _rk;
    tmp = this->T(tmp);
    tmp ^= x0;
    return tmp;
  }

  void genRK() {
    std::vector<word> kk{std::vector<word>(36)};
    for (int i = 0; i < 4; ++i) {
        kk[i] = MK[i];
        kk[i] ^= FK[i];
    }
    for (int i = 0; i < 32; ++i) {
        kk[i+4] = kk[i];
        kk[i+4] ^= TT(kk[i+1] ^ kk[i+2] ^ kk[i+3] ^ CK[i]);
        rk[i] = kk[i+4];
    }
  }

  void encrypt() {
    for (int i = 0; i < 32; ++i) {
        X[i+4] = this->F(
            X[i], X[i+1], X[i+2], X[i+3], rk[i]
        );    
    }
    for (int i = 0; i < 4; ++i) {
        Y[i] = X[35-i];
    }
  }

  void decrypt() {
    for (int i = 0; i < 32; ++i) {
        X[i+4] = this->F(
            X[i], X[i+1], X[i+2], X[i+3], rk[31-i]
        );    
    }
    for (int i = 0; i < 4; ++i) {
        Y[i] = X[35-i];
    }
  }

  std::string getCode() {
    this->encrypt();
    std::string str = "";
    for(int i = 0; i < 4; ++i) {
      std::stringstream ss;
      ss << std::setbase(16) << std::setfill('0') << std::setw(8) << Y[i].to_ulong();
      str += ss.str();
    }
    return str;
  }
};
};

#endif
