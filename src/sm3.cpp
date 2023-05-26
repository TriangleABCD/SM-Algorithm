#include "sm3.h"

SM3::SM3() {
}


SM3::SM3(const std::string m) {
    this->msg_origin = m;
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

}


WORD SM3::GG(int j, WORD X, WORD Y, WORD Z) {

}


WORD SM3::P0(WORD X) {

}


WORD SM3::P1(WORD X) {

}


WORD SM3::rLeftShift(const WORD& w, int x) {

}


void SM3::digest() {

}

