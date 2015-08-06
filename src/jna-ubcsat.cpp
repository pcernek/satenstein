//
// Created by pcernek on 7/27/15.
//

#include <sstream>
#include "jna-ubcsat.hpp"
#include "ubcsat-globals.h"

/**
 * This is a convenience function intended as a nominal attempt to deal with the horror of
 *  all the global state that can spill over from one configuration of UBCSAT to the next, in
 *  the event that two different algorithms of UBCSAT are run one after the other (e.g. first
 *  run -alg satenstein, then run -alg sparrow [aka "standalone" sparrow]).
 *
 * The intent is to reset statically-allocated global variables to 0 every time a configuration
 *  is destroyed. However, for this to work, we would need to guarantee that every such variable
 *  that occurs throughout the code is reset in this function. Since this kind of an approach is
 *  inherently unmaintainable, this function is meant as nothing more than a "best effort", and
 *  cannot guarantee that state will be completely cleaned up across different algorithms within
 *  the same instantiation of this library.
 *
 * Maintainers are invited to add such variables to this function as they are encountered.
 *
 * @author Paul Cernek
 */
void resetAllStaticallyAllocatedGlobalVars();

ubcsat_state::ubcsat_state() {
  resultState = 0;
  assignment = nullptr; // the first entry in this array contains its length
  errorState = 0;
  errorMessage = (char*) malloc(1024 * sizeof(char));
}

/* TODO: We can't currently do all this one-time initialization stuff
 *  because UBCSAT's memory model is not made to handle multiple different
 *  instances in one run, so the easiest way to de-allocate everything
 *  is to completely kill the library. In the future, if we're brave
 *  we might find a more efficient way of doing this.
 */
void initLibrary() {
//  InitSeed();
//  SetupUBCSAT();
//  AddAlgorithms();
//  AddParameters();
//  AddReports();
//  AddDataTriggers();
//  AddReportTriggers();
//  AddLocal();
}

char** split(const char *command, int* size) {
  char** ret = (char**) malloc(sizeof(char*) * MAXTOTALPARMS);
  const char* programName = "ubcsat";
  ret[0] = (char*) programName;
  char* t;
  int i;
  for (i = 1, t = strtok((char*)command, " "); t != NULL; ++i) {
    ret[i] = t;
    t = strtok(NULL, " ");
  }
  *size = i;
  return ret;
}

void* initConfig(const char* params) {
  InitSeed();
  SetupUBCSAT();
  AddAlgorithms();
  AddParameters();
  AddReports();
  AddDataTriggers();
  AddReportTriggers();
  AddLocal();

  int* numParams = (int*) malloc(sizeof(int));
  char** args = split(params, numParams);

  UBCSATState * ubcsat = new UBCSATState;

  ParseAllParameters(*numParams, args);
  ActivateAlgorithmTriggers();
  RandomSeed(ubcsat::iSeed);

  RunProcedures(PostParameters);
  ActivateTriggers((char *) "CheckTimeout");

  return ubcsat;
}

int initProblem(void* ubcsatState, const char* problem) {
  UBCSATState* state = (UBCSATState *) ubcsatState;

  UINT32 j = 0;
  UINT32 k;

  SINT32 signedVarNum;

  LITTYPE *pData;
  LITTYPE *pNextLit;
  LITTYPE *pLastLit;

  iNumClauses = 0;

  std::string cnf(problem);
  std::istringstream ss(cnf);
  std::string line;

  // Parse CNF header
  while (iNumClauses == 0) {
    std::getline(ss, line);

    if (line[0] =='p') {
      sscanf(line.c_str(),"p cnf %" SCAN32 " %" SCAN32 "",&iNumVars,&iNumClauses);
      if(iNumClauses == 0 || iNumVars == 0) {
        state->errorMessage = "Error: invalid instance: contains 0 vars and/or 0 clauses";
        return 0;
      }
    }
    else if (line[0] =='c') {
      //pass
    }
//    else {
//      std::cout << "Warning: Ignoring line in input: " << line << std::endl;
//    }
  }

  iVARSTATELen = (iNumVars >> 3) + 1;
  if ((iNumVars & 0x07)==0) {
    iVARSTATELen--;
  }

  aClauseLen = (UINT32 *) AllocateRAM(iNumClauses * sizeof(UINT32));
  pClauseLits = (LITTYPE **) AllocateRAM(iNumClauses * sizeof(LITTYPE *));

  pLastLit = pNextLit = pData = 0;

  iNumLits = 0;
  iMaxClauseLen = 0;

  // Parse body of CNF
  while(std::getline(ss, line)) {

    if(line[0] == 'c') {
      continue;
    }

    std::istringstream lineStream(line);

    pClauseLits[j] = pNextLit;
    aClauseLen[j] = 0;

    while (lineStream >> signedVarNum && signedVarNum != 0) {
      if (pNextLit >= pLastLit) {
        pData = (LITTYPE *) AllocateRAM(LITSPERCHUNK * sizeof(LITTYPE));
        pNextLit = pData;
        pLastLit = pData + LITSPERCHUNK;
        for (k=0;k<aClauseLen[j];k++) {
          *pNextLit = pClauseLits[j][k];
          pNextLit++;
        }
        pClauseLits[j] = pData;
      }

      *pNextLit = SetLitFromFile(signedVarNum);

      if (GetVarFromLit(*pNextLit) > iNumVars) {
        std::ostringstream msg;
        msg << "Error: Invalid Literal " << signedVarNum << " in clause " << j;
        state->errorMessage = msg.str().c_str();
        return 0;
      }

      pNextLit++;
      aClauseLen[j]++;
      iNumLits++;

    }

    if (aClauseLen[j] > iMaxClauseLen) {
      iMaxClauseLen = aClauseLen[j];
    }

    if (aClauseLen[j] == 0) {
      std::ostringstream msg;
      msg << "Error: Reading .cnf, clause " << j << " is empty" << std::endl;
      msg << "bad line: " << line << std::endl;
      state->errorMessage = msg.str().c_str();
      return 0;
    }

    j++;
  }

  AdjustLastRAM((pNextLit - pData) * sizeof(LITTYPE));

  RunProcedures(PostRead);

  RunProcedures(CreateData);
  RunProcedures(CreateStateInfo);

  RunProcedures(PreStart);

  RunProcedures(PreRun);

  return TRUE;
}

int initAssignment(void* ubcsatState, const long* assignment, int sizeOfAssignment) {

  UBCSATState* state = (UBCSATState *) ubcsatState;
  if (sizeOfAssignment > iNumVars) {
    std::ostringstream msg;
    msg << "Error: tried to give an assignment for " << sizeOfAssignment <<
        " variables, which is greater than the total number of variables (" << iNumVars << ")";
    state->errorMessage = msg.str().c_str();
    return FALSE;
  }

  for (UINT32 j=1;j<=iNumVars;j++) {
    aVarInit[j] = 2;
  }

  long varNumAndAssignment;
  long varNum;

  for(int i=0; i < sizeOfAssignment; i++) {
    varNumAndAssignment = assignment[i];

    int multiplier;
    if (varNumAndAssignment > 0) {
      multiplier = 1;
    }
    else if (varNumAndAssignment < 0) {
      multiplier = -1;
    }
    else {
      std::ostringstream msg;
      msg << "Error: tried to give an assignment for variable 0, but 0 is a disallowed variable id.";
      state->errorMessage = msg.str().c_str();
      return FALSE;
    }

    varNum = varNumAndAssignment * multiplier;

    if (varNum > iNumVars) {
      std::ostringstream msg;
      msg << "Error: tried to give an assignment for variable number " << varNumAndAssignment <<
      ", whose magnitude exceeds the total number of variables (" << iNumVars << ")";
      state->errorMessage = msg.str().c_str();
      return FALSE;
    }

    aVarInit[varNum] = (UINT32) (multiplier == 1);
  }

  return TRUE;
}

int solveProblem(void* ubcsatState, double timeoutTime) {

  fTimeOut = timeoutTime;

  UBCSATState* state = (UBCSATState*) ubcsatState;
  iStep = 0;
  bSolutionFound = FALSE;
  bTerminateRun = FALSE;
  bRestart = TRUE;

  StartRunClock();

  while ((iStep < iCutoff) && (! bSolutionFound) && !bTerminateRun && !state->terminateRun) {

    iStep++;
    iFlipCandidate = 0;

    RunProcedures(PreStep);
    RunProcedures(CheckRestart);

    if (bRestart) {
      RunProcedures(InitData);
      RunProcedures(InitStateInfo);
      RunProcedures(PostInit);
      bRestart = FALSE;
    }
    else {
      RunProcedures(ChooseCandidate);
      RunProcedures(PreFlip);
      RunProcedures(FlipCandidate);
      RunProcedures(UpdateStateInfo);
      RunProcedures(PostFlip);
    }

    RunProcedures(PostStep);

    RunProcedures(StepCalculations);

    RunProcedures(CheckTerminate);
  }

  StopRunClock();

  if(bSolutionFound) {
    state->resultState = 1;
    state->assignment = (int *) AllocateRAM( (iNumVars + 1) * sizeof(int) );
    state->assignment[0] = iNumVars;
    int multiplier;
    for (int i = 1; i <= iNumVars; i++) {
      multiplier = 1;
      if (aVarValue[i] == FALSE) {
        multiplier = -1;
      }
      state->assignment[i] = multiplier * i;
    }
  }
  else if (state->resultState == 0) { // resultState has not been changed since initialization
    state->resultState = 2;
  }

  return TRUE;
}

void destroyProblem(void* ubcsatState) {
  UBCSATState * ubcsat = (UBCSATState *) ubcsatState;
  ResetTriggers();
  CleanExit();
  resetAllStaticallyAllocatedGlobalVars();
  delete ubcsat;
}

void resetAllStaticallyAllocatedGlobalVars() {
  iUpdateSchemePromList = 0;
}

void interrupt(void* ubcsatState) {
  UBCSATState * ubcsat = (UBCSATState *) ubcsatState;
  ubcsat->resultState = 3;
  ubcsat->terminateRun = TRUE;
}

int getProblemState(void* ubcsatState) {
  UBCSATState * ubcsat = (UBCSATState *) ubcsatState;
  return ubcsat->errorState;
}

int* getResultAssignment(void* ubcsatState) {
  UBCSATState * ubcsat = (UBCSATState *) ubcsatState;
  return ubcsat->assignment;
}

int getResultState(void* ubcsatState) {
  UBCSATState* state = (UBCSATState *) ubcsatState;
  return state->resultState;
}

const char* getErrorMessage(void* ubcsatState) {
  UBCSATState* state = (UBCSATState *) ubcsatState;
  return state->errorMessage;
}

int getNumVars() {
  return iNumVars;
}

int getNumClauses() {
  return iNumClauses;
}

int getVarAssignment(int varNumber) {
  return aVarValue[varNumber];
}