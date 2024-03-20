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

void test_SM4(const std::string& msg) {
  SM::SM4 sm4;
}

int main(int argc, char** argv) {
  assert(argc == 3);
  std::string test(argv[1]);
  std::string msg(argv[2]);

  if("sm3" == test) {
    test_SM3(msg);
  } else if("sm4" == test) {
    test_SM4(msg);
  } else {
    std::cerr << "invalid test argument !\n";
    exit(1);
  }
  return 0;
}
