//
// Created by pcernek on 5/28/15.
//

#include "dcca.h"

BOOL*  CSchangedList;

UINT32 *CSDvarsList;
UINT32 numCSDvars;
UINT32 *CSDvarsListPos;

BOOL*  NVchangedList;

UINT32 *NVDvarsList;
UINT32 *NVDvarsListPos;
UINT32 numNVDvars;

UINT32 *SDvarsList;
UINT32 numSDvars;
UINT32 *SDvarsListPos;

void PickDCCA() {

  PickCSDvar();

  if (iFlipCandidate == 0) {
    PickNVDvar();
  }

  if (iFlipCandidate == 0) {
    PickSDvar();
  }

}

void CreateConfCheckingVars() {
  CSDvarsList      = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  CSDvarsListPos   = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  CSchangedList    = AllocateRAM((iNumVars+1) * sizeof(BOOL));

  NVDvarsList      = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  NVDvarsListPos   = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  NVchangedList    = AllocateRAM((iNumVars+1) * sizeof(BOOL));

  SDvarsList       = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  SDvarsListPos    = AllocateRAM((iNumVars+1) * sizeof(UINT32));
}

void InitConfCheckingVars() {
  UINT32 j;

  numCSDvars = 0;
  numNVDvars = 0;
  numSDvars = 0;

  for (j = 1; j <= iNumVars; j++) {
    CSDvarsListPos[j] = 0;
    CSchangedList[j] = TRUE;

    NVDvarsListPos[j] = 0;
    NVchangedList[j] = TRUE;

    SDvarsListPos[j] = 0;
  }
}

// If CSchanged and NVchanged are up-to-date, then it is trivial to check
//  that the score is correct for each variable.
void UpdateVarConfigurations() {
  UpdateCSDvars();
  UpdateNVDvars();
  // TODO: This should probably be moved out of conf-checking
  UpdateSDvars();
}

void UpdateSDvars() {

}


void UpdateNVchanged(UINT32 flippedVar) {
  NVchangedList[flippedVar] = FALSE;

  UINT32 neighborVarIndex;
  for (neighborVarIndex = 0; neighborVarIndex < aNumVarsShareClause[flippedVar]; neighborVarIndex++) {
    UINT32 neighborVar = pVarsShareClause[flippedVar][neighborVarIndex];
    NVchangedList[neighborVar] = TRUE;
  }
}

/**
 * If using clause weights, this function MUST be called after
 *  clause weights have been updated
 */
void UpdateNVDvars() {
  UpdateConfigurationDecreasing(NVchangedList, NVDvarsList, NVDvarsListPos, &numNVDvars);
}

/**
 * If using clause weights, this function MUST be called after
 *  clause weights have been updated
 */
void UpdateCSDvars() {
  UpdateConfigurationDecreasing(CSchangedList, CSDvarsList, CSDvarsListPos, &numCSDvars);
}

void UpdateConfigurationDecreasing(BOOL* confChangedList, UINT32* confDecreasingSet,
                                      UINT32* confDecreasingSetPos, UINT32* pointerToSetSize)
{
  UINT32 i;
  UINT32 numChanges = *pointerToSetSize;

  // cycle through all the variables whose scores have changed this step
  for(i = 0; i < numChanges; i++) {
    UINT32 var = aChangeList[i];

    // add vars that have become "configuration changed decreasing"
    if (confChangedList[var] && !confDecreasingSetPos[var] && isDecreasing(var)) {
      confDecreasingSet[ *pointerToSetSize ] = var;
      (*pointerToSetSize)++;
    }
      // remove vars that are no longer "configuration changed decreasing"
    else if (confDecreasingSetPos[var] && !isDecreasing(var)) {
      RemoveVarFromSet(var, confDecreasingSet, confDecreasingSetPos, pointerToSetSize);
    }
  }
}

/**
 * PRE: Flipping variable iFlipCandidate has caused the state of
 *  the given clause to change.
 *
 * POST: All variables that appear in the clause as literals are
 *  added to the list of variables whose Clause State-based configuration
 *  has changed since their last flip, if they are not already present
 *  in that list.
 * POST: Global variable iFlipCandidate is removed from the list of
 *  Clause States Configuration Changed vars.
 *
 * INVARIANT: CSchangedList contains numCSchanged variables
 * INVARIANT: numCSchanged points to the current last "filled" slot in
 *  CSchangedList. This applies even when numCSchanged == 0, since the
 *  zeroth index of CSchangedList is reserved as a "null" space.
 * INVARIANT: Each entry in CSchangedListPos is either the 1-based index
 *  of the position at which the variable with the corresponding index
 *  occurs in CSchangedList, or 0, which is reserved to indicate that
 *  variable does NOT occur in CSchangedList.
 */
void UpdateCSchanged(UINT32 toggledClause, UINT32 exceptVar) {
  UINT32 litIndex;

  // Set the clause states-based configuration of each variable in
  //  this clause is to TRUE, with the exception of the given variable.
  for (litIndex = 0; litIndex < aClauseLen[toggledClause]; litIndex++) {
    UINT32 curVar = GetVar(toggledClause, litIndex);
    if (curVar != exceptVar){
      CSchangedList[curVar] = TRUE;
    }
  }

}

void RemoveVarFromSet(UINT32 varToRemove, UINT32 *varSet, UINT32 *varSetPositions, UINT32 *pointerToSetSize)
{
  UINT32 varPos = varSetPositions[ varToRemove ];
  UINT32 setSize = *pointerToSetSize;

  if (varPos) { // varPos == 0 means the variable is not in the set
    // Move the last variable in the list to the position
    //  previously held by the variable being removed
    varSet[varPos] = varSet[setSize];
    varSet[setSize] = 0;
    (*pointerToSetSize)--;
    varSetPositions[varToRemove] = 0;
  }
}

void PickCSDvar() {
  PickBestOldestVar(CSDvarsList, numCSDvars);
}

void PickNVDvar() {
  PickBestOldestVar(NVDvarsList, numNVDvars);
}

void PickSDvar() {
  // TODO: make this not rely on SDvarsList, which may become deprecated
  PickBestOldestVar(SDvarsList, numSDvars);
}

void PickBestOldestVar(UINT32 *varList, UINT32 listSize) {
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

