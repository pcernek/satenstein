//
// Created by pcernek on 6/5/15.
//

#ifndef SATENSTEIN_TESTDCCA_H
#define SATENSTEIN_TESTDCCA_H

#include <gtest/gtest.h>
#include <ubcsat.h>
extern "C" {
  #include <dcca.h>
};


class DccaTest : public ::testing::Test {

protected:
  DccaTest();

  const int _numVars = 6;
  UINT32 _numCSDvars;
  UINT32*_csdVarsList;

  void initializeArrays();

  template <typename T>
  void copyArray(T *src, T *dst, UINT32 size);

  UINT32 _numNVDvars;
  UINT32* _nvdVarsList;

  UINT32 _numSDvars;
  UINT32* _sdVarsList;
};


#endif //SATENSTEIN_TESTDCCA_H
