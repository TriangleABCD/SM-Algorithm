#include <iostream>
#include <string>
#include <assert.h>

#include "SM3.hpp"
#include "SM4.hpp"

void test_SM3(const std::string& msg) {
  std::cout << "message: " << msg << std::endl;
  std::cout << "hash: ";
  SM::SM3 sm3(msg);
  auto res = sm3.getHash();
  int sp = 0;
  for (auto & c : res) {
    std::cout << c;
    ++sp;
    if (sp % 8 == 0) {
      std::cout << " ";
    }
  }
  std::cout << std::endl;
}

void test_SM4(const std::string& msg, const std::string& key) {
  std::cout << "message: " << msg << std::endl;
  std::cout << "key: " << key << std::endl;
  std::cout << "result:\n"; 
  SM::SM4 sm4(msg, key);
  auto res = sm4.getCode();
  int sp = 0;
  for (auto & c : res) {
    std::cout << c;
    ++sp;
    if (sp % 8 == 0) {
      std::cout << " ";
    }
  }
  std::cout << std::endl;
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
