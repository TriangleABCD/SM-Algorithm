#include <iostream>
#include <string>
#include <assert.h>

#include "SM3.hpp"
#include "SM4.hpp"

void showBytes(const std::string& str) {
  for(auto& c : str) {
    std::cout << std::setbase(16) << std::setfill('0') << std::setw(2) << int((unsigned char)c);
  }
  std::cout << std::endl;
}

void test_SM3(const std::string& msg) {
  std::cout << "[*] message:\t" << msg << std::endl;
  std::cout << "[*] hash:\t";
  SM::SM3 sm3(msg);
  auto res = sm3.getHash();
  std::cout << res << std::endl;
}

void test_SM4(const std::string& msg, const std::string& key) {
  std::cout << "[*] message:\t\t";
  showBytes(msg);
  std::cout << "[*] key:\t\t";
  showBytes(key);
  std::cout << "[*] encrypt result:\t"; 
  SM::SM4 sm4(msg, key);
  auto res = sm4.getCode(0);
  showBytes(res);

  sm4.setMsg(res);
  std::cout << "[*] decrypt result:\t"; 
  res = sm4.getCode(1);
  showBytes(res);
}

int main(int argc, char** argv) {
  assert(argc >= 3);
  std::string test(argv[1]);
  std::string msg(argv[2]);

  if("sm3" == test) {
    test_SM3(msg);
  } else if("sm4" == test) {
    assert(argc == 4);
    std::string key(argv[3]);
    test_SM4(msg, key);
  } else {
    std::cerr << "invalid test argument !\n";
    exit(1);
  }
  return 0;
}
