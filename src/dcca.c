//
// Created by pcernek on 5/28/15.
//

#include "dcca.h"

void PickDCCA() {

  pickCSDvar();

  if (iFlipCandidate == 0) {
    pickNVDvar();
  }

  if (iFlipCandidate == 0) {
    pickSDvar();
  }

}

void pickCSDvar() {
  pickBestOldestVar(csdVarsList, iNumCSDvars);
}

void pickNVDvar() {
  pickBestOldestVar(nvdVarsList, iNumNVDvars);
}

void pickSDvar() {
  pickBestOldestVar(sdVarsList, iNumSDvars);
}

void pickBestOldestVar(UINT32* varList, UINT32 listSize) {
  UINT32 iVar;
  SINT32 maxScore = 0;
  UINT32 bestVar = 0;
  UINT32 i;

  // Find the variable with the best score, breaking ties
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