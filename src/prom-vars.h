//
// Created by pcernek on 6/10/15.
//

#ifndef SATENSTEIN_PROM_LISTS_H
#define SATENSTEIN_PROM_LISTS_H

#include <stdbool.h>
#include "ubcsat-globals.h"
#include "ubcsat.h"
#include "ubcsat-types.h"
#include "ubcsat-triggers.h"

/***** Trigger DecPromVars *****/
void CreateDecPromVars();
void InitDecPromVars();
void UpdateDecPromVars();

void CreateDecPromPenVars();
void InitDecPromPenVars();
void UpdateDecPromPenVars();
BOOL bPen;

UINT32 *aDecPromVarsList;
UINT32 *aDecPromVarsListPos;
BOOL *aIsDecPromVar;
UINT32 iNumDecPromVars;


bool isDecreasing(UINT32 var);
void PickGNoveltyPlusProm();


#endif //SATENSTEIN_PROM_LISTS_H
