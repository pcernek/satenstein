//
// Created by pcernek on 6/5/15.
//

#include "DccaTest.h"

SINT32* aVarScore;
UINT32* aVarLastChange;
UINT32 iFlipCandidate;

TEST_F(DccaTest, PickCSDvar) {
  initConfChecking();
  numCSDvars = _numCSDvars;
  CSDvarsList = _csdVarsList;

  PickDCCA();

  EXPECT_EQ(6, iFlipCandidate);
}

TEST_F(DccaTest, PickNVDvar) {
  initConfChecking();
  numCSDvars = 0;
  CSDvarsList = _csdVarsList;

  numNVDvars = _numNVDvars;
  NVDvarsList = _nvdVarsList;

  PickDCCA();

  EXPECT_EQ(1, iFlipCandidate);
}

TEST_F(DccaTest, PickSDvar) {
  initConfChecking();
  numCSDvars = 0;
  CSDvarsList = _csdVarsList;

  numNVDvars = 0;
  NVDvarsList = _nvdVarsList;

  numSDvars = _numSDvars;
  SDvarsList = _sdVarsList;

  PickDCCA();

  EXPECT_EQ(4, iFlipCandidate);
}

TEST_F(DccaTest, PickBestVarInRandUNSATClause_noFalse) {
  iNumFalse = 0;
  PickBestVarInRandUNSATClause();
  EXPECT_EQ(0, iFlipCandidate);
}

TEST_F(DccaTest, PickBestVarInRandUNSATClause_oneFalse) {
  initSmoothing();
  initVarScores();
  initVarLastChange();
  iNumFalse = 1;
  iNumVars = 6;

  bPen = FALSE;

  pClauseLits = (LITTYPE **) AllocateRAM( 4 * sizeof(LITTYPE*) );
  pClauseLits[3] = (LITTYPE*) AllocateRAM( iNumVars * sizeof(LITTYPE) );
  pClauseLits[3][0] = GetNegLit(1);
  pClauseLits[3][1] = GetPosLit(2);
  pClauseLits[3][2] = GetPosLit(3);

  aClauseLen = (UINT32 *) AllocateRAM(iNumClauses * sizeof(UINT32));
  aClauseLen[3] = 3;

  PickBestVarInRandUNSATClause();
  EXPECT_EQ(1, iFlipCandidate);
}

TEST_F(DccaTest, SmoothSWTbelowThreshold) {
  initSmoothing();
  fDCCAp = 0.8;
  fDCCAq = 0.2;
  iAvgClauseWeightThreshold = 300;

  SmoothSWT();

  EXPECT_EQ(5, aClausePenaltyINT[0]);
  EXPECT_EQ(10, aClausePenaltyINT[1]);
  EXPECT_EQ(15, aClausePenaltyINT[2]);
  EXPECT_EQ(20, aClausePenaltyINT[3]);
}

TEST_F(DccaTest, SmoothSWTaboveThreshold) {
  initSmoothing();
  fDCCAp = 0.3;
  fDCCAq = 0.7;
  iAvgClauseWeightThreshold = 10;

  SmoothSWT();

  EXPECT_EQ(5, aClausePenaltyINT[0]);
  EXPECT_EQ(11, aClausePenaltyINT[1]);
  EXPECT_EQ(15, aClausePenaltyINT[2]);
  EXPECT_EQ(14, aClausePenaltyINT[3]);
}

TEST_F(DccaTest, SmoothSWTaboveThresholdInv) {
  initSmoothing();
  fDCCAp = 0.7;
  fDCCAq = 0.3;
  iAvgClauseWeightThreshold = 10;

  SmoothSWT();

  EXPECT_EQ(5, aClausePenaltyINT[0]);
  EXPECT_EQ(10, aClausePenaltyINT[1]);
  EXPECT_EQ(15, aClausePenaltyINT[2]);
  EXPECT_EQ(17, aClausePenaltyINT[3]);
}

TEST_F(DccaTest, updateConfCheckVars) {

}

void DccaTest::initConfChecking() {
  initVarScores();
  initVarLastChange();
  initCSDvars();
  initNVDvars();
  initSDvars();
}

void DccaTest::initSmoothing() {
  iNumClauses = 4;
  UINT32 tempClausePenalties[] = {5, 10, 15, 20};
  aClausePenaltyINT = (UINT32 *) AllocateRAM( (iNumClauses) * sizeof(UINT32));
  copyArray(tempClausePenalties, aClausePenaltyINT, iNumClauses);
  iTotalPenaltyINT = sum(aClausePenaltyINT, iNumClauses);

  UINT32 tempFalseList[] = {3,1};
  aFalseList = (UINT32 *) AllocateRAM( (iNumClauses) * sizeof(UINT32));
  copyArray(tempFalseList, aFalseList, iNumClauses);
  iNumFalse = 2;
}

void DccaTest::initSDvars() {
  _numSDvars = 5;
  UINT32 tempSDvarsList[] = {1,2,3,4,5};
  _sdVarsList = (UINT32 *) AllocateRAM( _numSDvars * sizeof(UINT32));
  copyArray(tempSDvarsList, _sdVarsList, _numSDvars);
}

void DccaTest::initNVDvars() {
  _numNVDvars = 2;
  UINT32 tempNVDvarsList[] = {1,3};
  _nvdVarsList = (UINT32 *) AllocateRAM( _numNVDvars * sizeof(UINT32));
  copyArray(tempNVDvarsList, _nvdVarsList, _numNVDvars);
}

void DccaTest::initCSDvars() {
  _numCSDvars = 4;
  UINT32 tempCSDvarsList[] = {1,3,4,6};
  _csdVarsList = (UINT32 *) AllocateRAM( _numCSDvars * sizeof(UINT32));
  copyArray(tempCSDvarsList, _csdVarsList, _numCSDvars);
}

void DccaTest::initVarLastChange() {
  iNumVars = 6;
  UINT32 tempVarLastChange[] = {0,  1,  2,  3,  4,  5,  1};
  aVarLastChange = (UINT32 *) AllocateRAM( (iNumVars + 1) * sizeof(UINT32));
  copyArray(tempVarLastChange, aVarLastChange, iNumVars + 1);
}

void DccaTest::initVarScores() {
  iNumVars = 6;
  SINT32 tempVarScores[] =     {0, -3,  0, -3, -8,  2, -8};
  aVarScore = (SINT32 *) AllocateRAM( (iNumVars + 1) * sizeof(SINT32));
  copyArray(tempVarScores, aVarScore, iNumVars + 1);
}

template <typename T>
void DccaTest::copyArray(T *src, T *dst, UINT32 size) {
  for (int i = 0; i < size; i++) {
    dst[i] = src[i];
  }
}

UINT32 DccaTest::sum(UINT32* array, int size) {
  UINT32 total = 0;
  for (int i=0; i < size; i++) {
    total += array[i];
  }
  return total;
}

DccaTest::DccaTest() {
  iNumHeap = 0;
  iFlipCandidate = 0;
}

DccaTest::~DccaTest() {
  FreeRAM();
}
