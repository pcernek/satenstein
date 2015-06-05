//
// Created by pcernek on 6/1/15.
//

#ifndef SATENSTEIN_DCCA_H
#define SATENSTEIN_DCCA_H

#include "ubcsat.h"

void PickDCCA();

void pickNVDvar();
void pickCSDvar();
void pickSDvar();

void pickBestOldestVar(UINT32* varList, UINT32 listSize);

void UpdateDCCA();

#endif //SATENSTEIN_DCCA_H
