//
// Created by pcernek on 6/10/15.
//

#include "prom-vars.h"
#include "satenstein-types.h"


bool isDecreasing(UINT32 var) {
  return aVarScore[var] < 0;
}

void CreateDecPromVars() {

  aDecPromVarsList = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aIsDecPromVar = AllocateRAM((iNumVars+1) * sizeof(BOOL));
  aDecPromVarsListPos = AllocateRAM((iNumVars+1) * sizeof(UINT32));

}

void InitDecPromVars() {

  UINT32 j;

  iNumDecPromVars = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aVarScore[j] < 0) {
      aDecPromVarsListPos[j] = iNumDecPromVars;
      aDecPromVarsList[iNumDecPromVars++] = j;
      aIsDecPromVar[j]= TRUE;
    }
    else{
      aIsDecPromVar[j]= FALSE;
    }
  }
}

void UpdateDecPromVars() {

  UINT32 j, k;
  UINT32 iVar;

  switch (iUpdateSchemePromList) {

    case UPDATE_G2WSAT_1:

      for (j = 0; j < iNumChanges; j++) {
        iVar = aChangeList[j];
        if ((aVarScore[iVar] < 0) && (aChangeOldScore[iVar] >= 0)) {
          aDecPromVarsListPos[iVar] = iNumDecPromVars;
          aDecPromVarsList[iNumDecPromVars++] = iVar;
          aIsDecPromVar[iVar] = TRUE;

        }
      }

      j = 0;
      k = 0;

      if (aIsDecPromVar[iFlipCandidate]) {
        /* A variable just flipped at the last step cannot be
           a promising decreasing variable.
           Thats why the variable is swapped with the last
           variable of the promising decreasing variable list.
           The position of the swapped variable is also
           updated.
           Because of this change the flip performance of Satenstein Version 1.2
           will not be identical for Satenstein for a particular random seed.
       */

        j = aDecPromVarsListPos[iFlipCandidate];
        iNumDecPromVars--;
        aDecPromVarsList[j] = aDecPromVarsList[iNumDecPromVars];
        aDecPromVarsListPos[aDecPromVarsList[j]] = j;
        aIsDecPromVar[iFlipCandidate] = FALSE;
      }

      break;

    case UPDATE_G2WSAT_2:

      for (j = 0; j < iNumChanges; j++) {
        iVar = aChangeList[j];
        if ((aVarScore[iVar] < 0) && (aChangeOldScore[iVar] >= 0)) {
          aDecPromVarsListPos[iVar] = iNumDecPromVars;
          aDecPromVarsList[iNumDecPromVars++] = iVar;
          aIsDecPromVar[iVar] = TRUE;

        }
      }

      j = 0;
      k = 0;

      while (j < iNumDecPromVars) {
        iVar = aDecPromVarsList[k];
        if ((aVarScore[iVar] >= 0) || (iVar == iFlipCandidate)) {
          iNumDecPromVars--;
          aIsDecPromVar[j] = FALSE;
        } else {
          aDecPromVarsList[j] = aDecPromVarsList[k];
          aDecPromVarsListPos[iVar] = j;
          j++;
        }
        k++;
      }

      break;

    case UPDATE_DCCA:
      UpdateVarConfigurations();
      break;

    default:
      // do nothing
      break;
  }
}

void CreateDecPromPenVars() {
  CreateDecPromVars();
}

void InitDecPromPenVars() {

  register UINT32 j;

  iNumDecPromVars = 0;

  numCSDvars = 0;
  numNVDvars = 0;
  numSDvars = 0;

  for (j=1;j<=iNumVars;j++) {

    CSchangedList[j]= FALSE;
    NVchangedList[j] = FALSE;

    if (aVarPenScore[j] < 0) {
      aDecPromVarsListPos[j] = iNumDecPromVars;
      aDecPromVarsList[iNumDecPromVars] = j;
      iNumDecPromVars++;
      aIsDecPromVar[j] = TRUE;
    }
    else
    {
      aIsDecPromVar[j]=FALSE;
    }
  }
}

void UpdateDecPromPenVars() {
  UINT32 j, k;
  UINT32 iVar;
  switch (iUpdateSchemePromList) {
    case 1:
      for (j = 0; j < iNumChanges; j++) {
        iVar = aChangeList[j];
        if ((aVarPenScore[iVar] < 0) && (aChangeOldScore[iVar] >= 0)) {
          aDecPromVarsListPos[iVar] = iNumDecPromVars;
          aDecPromVarsList[iNumDecPromVars++] = iVar;
          aIsDecPromVar[iVar] = TRUE;

        }
      }
      j = 0;
      k = 0;

      if (aIsDecPromVar[iFlipCandidate]) {
        /* A variable just flipped at the last step cannot be
           a promising decreasing variable.
           Thats why the variable is swapped with the last
           variable of the promising decreasing variable list.
           The position of the swapped variable is also
           updated.
           Because of this change the flip performance of Satenstein Version 1.2
           will not be identical for Satenstein for a particular random seed.
       */

        j = aDecPromVarsListPos[iFlipCandidate];
        iNumDecPromVars--;
        aDecPromVarsList[j] = aDecPromVarsList[iNumDecPromVars];
        aDecPromVarsListPos[aDecPromVarsList[j]] = j;
        aIsDecPromVar[iFlipCandidate] = FALSE;
      }


      break;

    case 2:
      for (j = 0; j < iNumChanges; j++) {
        iVar = aChangeList[j];
        if ((aVarPenScore[iVar] < 0) && (aChangeOldScore[iVar] >= 0)) {
          aDecPromVarsListPos[iVar] = iNumDecPromVars;
          aDecPromVarsList[iNumDecPromVars++] = iVar;
          aIsDecPromVar[iVar] = TRUE;

        }
      }
      j = 0;
      k = 0;
      while (j < iNumDecPromVars) {
        iVar = aDecPromVarsList[k];
        if ((aVarPenScore[iVar] >= 0) || (iVar == iFlipCandidate)) {
          iNumDecPromVars--;
          aIsDecPromVar[j] = FALSE;
        } else {
          aDecPromVarsList[j] = aDecPromVarsList[k];
          aDecPromVarsListPos[iVar] = j;
          j++;
        }
        k++;
      }

      break;


  }

}

void PickGNoveltyPlusProm() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;

  iBestScore = 0;
  j=0;
  k=0;
  while (j < iNumDecPromVars) {
    iVar = aDecPromVarsList[k];
    if (aVarScore[iVar] >= 0) {
      iNumDecPromVars--;
      aIsDecPromVar[iVar] = 0;
    } else {
      if (aVarScore[iVar] < iBestScore) {
        iFlipCandidate = iVar;
        iBestScore = aVarScore[iVar];
      } else {
        if (aVarScore[iVar] == iBestScore) {
          if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
            iFlipCandidate = iVar;
          }
        }
      }
      aDecPromVarsList[j++]= aDecPromVarsList[k];
    }
    k++;
  }
}