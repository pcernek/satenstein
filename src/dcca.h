//
// Created by pcernek on 6/1/15.
//

#ifndef SATENSTEIN_DCCA_H
#define SATENSTEIN_DCCA_H

#include "ubcsat.h"

#include <float.h>

/***** lists of "configuration changed decreasing" vars ******/
extern BOOL*   aCSchanged;
extern UINT32* aCSDvars;
extern UINT32  iNumCSDvars;
extern UINT32* aCSDvarsPos;
extern BOOL*   aIsCSDvar;

extern BOOL*   aNVchanged;
extern UINT32* aNVDvars;
extern UINT32* aNVDvarsPos;
extern UINT32  iNumNVDvars;
extern BOOL*   aIsNVDvar;

extern UINT32* aSDvars;
extern UINT32  iNumSDvars;
extern UINT32* aSDvarsPos;
extern BOOL*   aIsSDvar;

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
void UpdateCSchanged(UINT32 toggledClause);
void UpdateNVchanged(UINT32 flippedVar);
void UpdateConfigurationDecreasing(BOOL *aConfChanged, UINT32 *aConfDecVars, UINT32 *aConfDecVarsPos,
                                   UINT32 *pNumConfDecVars, BOOL *isConfDecreasing);

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
