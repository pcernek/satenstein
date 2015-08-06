//
// Created by pcernek on 7/28/15.
//

#ifndef SATENSTEIN_TESTJNA_H
#define SATENSTEIN_TESTJNA_H

#include <gtest/gtest.h>

extern "C" {
  #include "jna_ubcsat.hpp"
};

class JNATest : public ::testing::Test {

protected:
  UBCSATState* _state;
  const char*_dummyCNF;
  const char*_notSoRandomKsatCNF;
  const char* _miniUnsatCNF;


  const char* _facCNF;
  JNATest();

  virtual ~JNATest();
};

#endif //SATENSTEIN_TESTJNA_H
