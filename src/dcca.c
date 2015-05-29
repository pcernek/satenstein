//
// Created by pcernek on 5/28/15.
//

#include "ubcsat.h"

void PickDCCAStructured();

void AddDCCA() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("dcca","structured",FALSE,
                            "DCCA: Double Configuration Checking in Stochastic Local Search for Satisfiability",
                            "Luo, Cai, Wu, Su [AAAI 2014]",
                            "PickDCCAStructured",
                            "TODO",
                            "TODO","TODO");

  CreateTrigger("PickDCCAStructured",ChooseCandidate,PickDCCAStructured,"","");

}

void PickDCCAStructured() {
  printf("Hello from DCCA!\n");
}