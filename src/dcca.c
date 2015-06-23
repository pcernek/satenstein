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

UINT32 iAvgClauseWeightThreshold;

FLOAT fDCCAp;
FLOAT fDCCAq;

void AddDCCA() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("dcca","",0,
                            "Deouble Configuration Checking with Aspiration",
                            "Luo, Cai, Wu, Su [AAAI 2014]",
                            "PickDCCA",
                            "DefaultProcedures,ConfChecking,Flip+TrackChanges+FCLPen,VarLastChange,ClausePenaltyINT,PenClauseList,VarsShareClauses,VarPenScore",
                            "default","default");

  AddParmUInt(&pCurAlg->parmList,"-avgweightthreshold",
             "average clause weight threshold [default %s]",
             "on the diversification step, if the average clause weight exceeds~this threshold, smoothing is performed",
             "",&iAvgClauseWeightThreshold,300);
  AddParmFloat(&pCurAlg->parmList,"-DCCAp","DCCA p param [default %s]","weight of current clause score in SWT smoothing [default %s]","",&fDCCAp,0.3);
  AddParmFloat(&pCurAlg->parmList,"-DCCAq","DCCA q param [default %s]","weight of average clause score in SWT smoothing [default %s]","",&fDCCAp,0.0);

  CreateTrigger("InitDCCA",PostRead,InitDCCA,"","");

  CreateTrigger("PickDCCA",ChooseCandidate,PickDCCA,"","");

  // TODO: Consider moving this to ubcsat-triggers.c
  CreateTrigger("CreateCSDvars",CreateData,CreateCSDvars,"","");
  CreateTrigger("InitCSDvars",InitStateInfo,InitCSDvars,"","");
  CreateTrigger("UpdateCSDvars",UpdateStateInfo,UpdateCSDvars,"","");
  CreateContainerTrigger("CSDvars","CreateCSDvars,InitCSDvars,UpdateCSDvars");

  CreateTrigger("CreateNVDvars",CreateData,CreateNVDvars,"","");
  CreateTrigger("InitNVDvars",InitStateInfo,InitNVDvars,"","");
  CreateTrigger("UpdateNVDvars",UpdateStateInfo,UpdateNVDvars,"","");
  CreateContainerTrigger("NVDvars","CreateNVDvars,InitNVDvars,UpdateNVDvars");

  CreateTrigger("CreateSDvars",CreateData,CreateSDvars,"","");
  CreateTrigger("InitSDvars",InitStateInfo,InitSDvars,"","");
  CreateTrigger("UpdateSDvars",UpdateStateInfo,UpdateSDvars,"","");
  CreateContainerTrigger("SDvars","CreateSDvars,InitSDvars,UpdateSDvars");

  CreateContainerTrigger("ConfChecking","CSDvars,NVDvars,SDvars");
}

// TODO: Complete this function
void InitDCCA() {
  InitSDvars();
}

void PickDCCA() {

  PickCSDvar();

  if (iFlipCandidate == 0) {
    PickNVDvar();
  }

  if (iFlipCandidate == 0) {
    PickSDvar();
  }

  if (iFlipCandidate == 0) {
    PickDCCADiversify();
  }

}

void PickDCCADiversify() {
  UpdateClauseWeightsSWT();
  PickBestVarInRandUNSATClause();
}

/**
 * Picks an UNSAT clause at random, then picks the variable with the
 *  best score from that clause. If two variables are tied for the
 *  best score, pick the variable that was flipped the least recently.
 */
// TODO: Consider moving to ubcsat-triggers.c
void PickBestVarInRandUNSATClause() {
  UINT32 i;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  UINT32 iBestVarAge;
  LITTYPE *pLit;

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  }
  else {
    iFlipCandidate = 0;
    return;
  }

  iBestScore = bPen ? iTotalPenaltyINT : iNumClauses;
  iBestVarAge = iStep;

  /* Find var with best score. */
  pLit = pClauseLits[iClause];
  for (i = 0; i < iClauseLen; i++) {
    iVar = GetVarFromLit(*pLit);
    iScore = GetScore(iVar);

    if (iScore < iBestScore ||
          (iScore == iBestScore && aVarLastChange[iVar] < iBestVarAge) )
    {
      iFlipCandidate = iVar;
      iBestScore = iScore;
      iBestVarAge = aVarLastChange[iVar];
    }
    pLit++;
  }
}

void UpdateClauseWeightsSWT() {
  IncrementUNSATClauseWeights();
  SmoothSWT();
}

/**
 * The SWT smoothing scheme (Cai & Su, "Local Search for Boolean
 *  satisfiability with configuration checking and subscore", 2013).
 *
 * PRE: As with any other smoothing scheme (as far as I know),
 *  assumes bPen == TRUE (since smoothing of clause weights
 *  is meaningless if clause weights are not being used)
 * POST: If the average clause weight has exceeded a threshold,
 *  the clause weights of all currently UNSAT clauses are smoothed.
 */
void SmoothSWT() {
  UINT32  iAvgClausePenalty = iTotalPenaltyINT / iNumClauses;

  if (iAvgClausePenalty  < iAvgClauseWeightThreshold)
    return;

  UINT32 i;
  UINT32 clause;

  for (i = 0; i < iNumFalse; i++) {
    clause = aFalseList[i];
    aClausePenaltyINT[clause] = (UINT32) ( (fDCCAp + FLT_EPSILON) * aClausePenaltyINT[clause]) +
                                (UINT32) ( (fDCCAq + FLT_EPSILON) * iAvgClausePenalty);
  }
}

void IncrementUNSATClauseWeights() {
  // TODO: Implement and move to ubcsat-triggers.c (or link to an already-existing function there)
}

void CreateConfCheckingVars() {
  CreateCSDvars();
  CreateNVDvars();
  CreateSDvars();
}

void CreateSDvars() {
  SDvarsList       = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  SDvarsListPos    = AllocateRAM((iNumVars+1) * sizeof(UINT32));
}

void CreateNVDvars() {
  NVDvarsList      = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  NVDvarsListPos   = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  NVchangedList    = AllocateRAM((iNumVars+1) * sizeof(BOOL));
}

void CreateCSDvars() {
  CSDvarsList      = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  CSDvarsListPos   = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  CSchangedList    = AllocateRAM((iNumVars+1) * sizeof(BOOL));
}

void InitSDvars() {
  UINT32 j;

  numSDvars = 0;

  for (j = 1; j <= iNumVars; j++) {
    SDvarsListPos[j] = 0;
  }
}

void InitCSDvars() {
  UINT32 j;

  numCSDvars = 0;

  for (j = 1; j <= iNumVars; j++) {
    CSDvarsListPos[j] = 0;
    CSchangedList[j] = TRUE;
  }
}

void InitNVDvars() {
  UINT32 j;

  numNVDvars = 0;

  for (j = 1; j <= iNumVars; j++) {
    NVDvarsListPos[j] = 0;
    NVchangedList[j] = TRUE;
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

/**
 * Find the variable with the best score, breaking ties
 *  in favor of the least recently-flipped variable.
 */
void PickBestOldestVar(UINT32 *varList, UINT32 listSize) {
  UINT32 iVar;
  SINT32 iBestScore = bPen ? iTotalPenaltyINT : iNumClauses;
  UINT32 iBestVarAge = iStep;
  UINT32 i;

  for (i = 0; i < listSize; i ++) {
    iVar = varList[i];
    if (aVarScore[iVar] < iBestScore ||
        (aVarScore[iVar] == iBestScore && aVarLastChange[iVar] < iBestVarAge) )
    {
      iFlipCandidate = iVar;
      iBestScore = aVarScore[iVar];
      iBestVarAge = aVarLastChange[iVar];
    }
  }
}
