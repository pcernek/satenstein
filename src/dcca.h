//
// Created by pcernek on 6/1/15.
//

#ifndef SATENSTEIN_DCCA_H
#define SATENSTEIN_DCCA_H

#include "ubcsat.h"

#include <float.h>

/***** lists of "configuration changed decreasing" vars ******/
extern BOOL*  CSchangedList;
extern UINT32 *CSDvarsList;
extern UINT32 numCSDvars;
extern UINT32 *CSDvarsListPos;

extern BOOL*  NVchangedList;
extern UINT32 *NVDvarsList;
extern UINT32 *NVDvarsListPos;
extern UINT32 numNVDvars;

extern UINT32 *SDvarsList;
extern UINT32 numSDvars;
extern UINT32 *SDvarsListPos;

extern UINT32 iAvgClauseWeightThreshold;
extern FLOAT fDCCAp;
extern FLOAT fDCCAq;

void PickDCCA();

void CreateCSDvars();
void CreateNVDvars();
void CreateSDvars();

void InitCSDvars();
void InitNVDvars();
void InitSDvars();

void UpdateCSDvars();
void UpdateNVDvars();
void UpdateSDvars();

// TODO: Reconsider the need for the following functions
void UpdateDCCA();
void CreateConfCheckingVars();
void UpdateVarConfigurations();
void UpdateCSchanged(UINT32 toggledClause, UINT32 exceptVar);
void UpdateNVchanged(UINT32 flippedVar);
void UpdateConfigurationDecreasing(BOOL* confChangedList, UINT32* confDecreasingSet,
                                   UINT32* confDecreasingSetPos, UINT32* pointerToSetSize);

void RemoveVarFromSet(UINT32 varToRemove, UINT32 *varSet, UINT32 *varSetPositions, UINT32 *pointerToSetSize);

void UpdateClauseWeightsSWT();

void IncrementUNSATClauseWeights();

void SmoothSWT();

void PickDCCADiversify();

void PickBestVarInRandUNSATClause();

void InitDCCA();

void PickCSDvar();
void PickNVDvar();
void PickSDvar();

void PickBestOldestVar(UINT32 *varList, UINT32 listSize);

#endif //SATENSTEIN_DCCA_H
