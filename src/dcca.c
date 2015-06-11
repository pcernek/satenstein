//
// Created by pcernek on 5/28/15.
//

#include "dcca.h"
#include "conf-checking.h"

void PickDCCA() {

  PickCSDvar();

  if (iFlipCandidate == 0) {
    PickNVDvar();
  }

  if (iFlipCandidate == 0) {
    PickSDvar();
  }

}

// TODO: Expand this file to include all DCCA functionality with appropriate calls to conf-checking.c functions