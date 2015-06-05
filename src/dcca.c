//
// Created by pcernek on 5/28/15.
//

#include "ubcsat.h"
#include "dcca.h"

void pickBestOldestVar(UINT32* varList, UINT32 listSize);

void PickDCCA() {
  printf("Picking a variable via DCCA\n");

  pickBestOldestVar(csdVarsList, iNumCSDvars);

  if (iFlipCandidate == 0) {
    pickBestOldestVar(nvdVarsList, iNumNVDvars);
  }

  if (iFlipCandidate == 0) {
    pickBestOldestVar(sdVarsList, iNumSDvars);
  }

}

void pickBestOldestVar(UINT32* varList, UINT32 listSize) {
  UINT32 iVar;
  SINT32 maxScore = 0;
  UINT32 bestVar = 0;
  UINT32 i;

  // Find the CSD variable with the best score, breaking ties
  //   in favor of the least recently-flipped variable
  for (i = 0; i < listSize; i ++) {
    iVar = varList[i];
    if (aVarScore[iVar] < maxScore ||
        (aVarScore[iVar] == maxScore && aVarLastChange[iVar] < aVarLastChange[bestVar]) )
    {
      maxScore = aVarScore[iVar];
      bestVar = iVar;
    }
  }

  iFlipCandidate = bestVar;
}

//void pickCSDvar() {
//  UINT32 iVar;
//  SINT32 maxScore = 0;
//  UINT32 bestCSDvar = 0;
//  UINT32 i;
//
//  // Find the CSD variable with the best score, breaking ties
//  //   in favor of the least recently-flipped variable
//  for (i = 0; i < iNumCSDvars; i ++) {
//    iVar = csdVarsList[i];
//    if (aVarScore[iVar] < maxScore ||
//        (aVarScore[iVar] == maxScore && aVarLastChange[iVar] < aVarLastChange[bestCSDvar]) )
//    {
//      maxScore = aVarScore[iVar];
//      bestCSDvar = iVar;
//    }
//  }
//
//  iFlipCandidate = bestCSDvar;
//}
//
//void pickNVDvar() {
//  UINT32 iVar;
//  SINT32 maxScore = 0;
//  UINT32 bestNVDvar = 0;
//  UINT32 i;
//
//  // Find the NVD variable with the best score, breaking ties
//  //   in favor of the least recently-flipped variable
//  for (i = 0; i < iNumNVDvars; i ++) {
//    iVar = nvdVarsList[i];
//    if (aVarScore[iVar] < maxScore ||
//        (aVarScore[iVar] == maxScore && aVarLastChange[iVar] < aVarLastChange[bestNVDvar]) )
//    {
//      maxScore = aVarScore[iVar];
//      bestNVDvar = iVar;
//    }
//  }
//
//  iFlipCandidate = bestNVDvar;
//}
//
//void pickSDvar() {
//  UINT32 iVar;
//  SINT32 maxScore = 0;
//  UINT32 bestSDvar = 0;
//  UINT32 i;
//
//  // Find the SD variable with the best score, breaking ties
//  //   in favor of the least recently-flipped variable
//  for (i = 0; i < iNumNVDvars; i ++) {
//    iVar = sdVarsList[i];
//    if (aVarScore[iVar] < maxScore ||
//        (aVarScore[iVar] == maxScore && aVarLastChange[iVar] < aVarLastChange[bestSDvar]) )
//    {
//      maxScore = aVarScore[iVar];
//      bestSDvar = iVar;
//    }
//  }
//
//  iFlipCandidate = bestSDvar;
//}