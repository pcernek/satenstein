//
// Created by pcernek on 6/10/15.
//

#ifndef SATENSTEIN_CONF_CHECKING_H
#define SATENSTEIN_CONF_CHECKING_H

#include <ubcsat-triggers.h>
#include <stdint.h>
#include <ubcsat-mem.h>
#include <stdint-gcc.h>
#include <ubcsat-types.h>
#include <bfd.h>
#include <curses.h>
#include <stdio.h>
#include "ubcsat-mem.h"
#include "ubcsat-triggers.h"
#include "ubcsat-types.h"

#include "prom-vars.h"

extern BOOL performClauseConfChecking;
extern BOOL performNeighborConfChecking;

/***** lists of "configuration changed decreasing" vars ******/
extern BOOL*  CSchangedList;

extern UINT32 *CSDvarsList;
extern UINT32 numCSDvars;
extern UINT32 *CSDvarsListPos;

extern BOOL*  NVchangedList;

extern UINT32 *NVDvarsList;
extern UINT32 *NVDvarsListPos;
extern UINT32 numNVDvars;

// TODO: This should probably be moved to one of the promising list heuristics
extern UINT32 *SDvarsList;
extern UINT32 numSDvars;
extern UINT32 *SDvarsListPos;



void CreateConfCheckingVars();
void InitConfCheckingVars();

void UpdateVarConfigurations();

void UpdateSDvars();

void UpdateCSchanged(UINT32 toggledClause, UINT32 exceptVar);
void UpdateNVchanged(UINT32 flippedVar);

void UpdateNVDvars();

void UpdateCSDvars();


void UpdateConfigurationDecreasing(BOOL* confChangedList, UINT32* confDecreasingSet,
                                   UINT32* confDecreasingSetPos, UINT32* pointerToSetSize);

void RemoveVarFromSet(UINT32 varToRemove, UINT32 *varSet, UINT32 *varSetPositions, UINT32 *pointerToSetSize);

void PickNVDvar();
void PickCSDvar();
void PickSDvar();

void PickBestOldestVar(UINT32 *varList, UINT32 listSize);



#endif //SATENSTEIN_CONF_CHECKING_H
