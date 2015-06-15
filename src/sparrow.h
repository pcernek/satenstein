//
// Created by pcernek on 6/12/15.
//

#ifndef SATENSTEIN_SPARROW_H
#define SATENSTEIN_SPARROW_H

#include "ubcsat.h"

// Sparrow params
extern FLOAT fSparrowC1;
extern UINT32 iSparrowC2;
extern FLOAT fSparrowC3;
extern FLOAT fInvSparrowC3;

extern FLOAT aSparrowPreCalc[11];

extern FLOAT *aSparrowWeights;

extern BOOL bUseSparrowDefaults;


void PickSparrowProbDist();

void CreateSparrowWeights();
void InitSparrow();

void ScaleSparrow();
void SmoothSparrow();
void FlipSparrow();

/******** standalone sparrow ********/
void PickSparrow();

void CreateSparrowPromVars();
void InitSparrowPromVars();
void SparrowSat2011Settings();

void CreateSparrowScore();
void InitSparrowScore();



#endif //SATENSTEIN_SPARROW_H
