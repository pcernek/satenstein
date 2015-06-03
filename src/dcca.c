//
// Created by pcernek on 5/28/15.
//

#include "ubcsat.h"
#include "dcca.h"

void PickDCCAStructured();

void AddDCCA() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("dcca","structured",FALSE,
                            "DCCA: Double Configuration Checking in Stochastic Local Search for Satisfiability",
                            "Luo, Cai, Wu, Su [AAAI 2014]",
                            "PickDCCAStructured",
                            "TODO:DataTriggers",
                            "TODO:DefaultOutput","TODO:DefaultStats");

  CreateTrigger("PickDCCAStructured",ChooseCandidate,PickDCCAStructured,"","");
}

void PickDCCAStructured() {
  printf("Hello from DCCA!\n");
}

void DCCAProm() {

  UINT32 iLastChange;
  UINT32 csdVarIndex, i, k;
  UINT32 csdVar;
  SINT32 varScore = 0;

  iBestScore = iNumFalse;  //CWBIG;    //BIG;
  iLastChange = iStep;
  i = -1;
  k = 0;
  for (csdVarIndex = 0; csdVarIndex < iNumCSDvars; csdVarIndex++) {
    csdVar = csdVarsList[csdVarIndex];

    varScore = bPen ? aVarPenScore[csdVar] : aVarScore[csdVar];

    if (varScore < 0) {
      if (varScore < iBestScore) {
        iBestScore = varScore;
        iLastChange = aVarLastChange[csdVar];
        iFlipCandidate = csdVar;
      } else if (varScore == iBestScore) {
        if (aVarLastChange[csdVar] < iLastChange) {
          iLastChange = aVarLastChange[csdVar];
          iFlipCandidate = csdVar;
        }
      }
    } else {
      i = csdVarIndex;
      aIsDecPromVar[csdVar] = FALSE;
      break;
    }
  }
  csdVarIndex++;
  if (i != -1) {
    for (csdVarIndex = i + 1; csdVarIndex < iNumDecPromVars; csdVarIndex++) {
      csdVar = aDecPromVarsList[csdVarIndex];
      if (!bPen)
        varScore = aVarScore[csdVar];
      else
        varScore = aVarPenScore[csdVar];
      if (varScore < 0) {
        aDecPromVarsListPos[csdVar] = i;
        aDecPromVarsList[i++] = csdVar;
        if (varScore < iBestScore) {
          iBestScore = varScore;
          iLastChange = aVarLastChange[csdVar];
          iFlipCandidate = csdVar;
        } else if (varScore == iBestScore) {
          if (aVarLastChange[csdVar] < iLastChange) {
            iLastChange = aVarLastChange[csdVar];
            iFlipCandidate = csdVar;
          }
        }
      } else {
        aIsDecPromVar[csdVar] = FALSE;
      }
    }
    iNumDecPromVars = i;
  }
}