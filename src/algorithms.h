/*

      ##  ##  #####    #####   $$$$$   $$$$   $$$$$$    
      ##  ##  ##  ##  ##      $$      $$  $$    $$      
      ##  ##  #####   ##       $$$$   $$$$$$    $$      
      ##  ##  ##  ##  ##          $$  $$  $$    $$      
       ####   #####    #####  $$$$$   $$  $$    $$      
  ======================================================
  SLS SAT Solver from The University of British Columbia
  ======================================================
  ...Developed by Dave Tompkins (davet [@] cs.ubc.ca)...
  ------------------------------------------------------
  .......consult legal.txt for legal information........
  ......consult revisions.txt for revision history......
  ------------------------------------------------------
  ... project website: http://www.satlib.org/ubcsat ....
  ------------------------------------------------------
  .....e-mail ubcsat-help [@] cs.ubc.ca for support.....
  ------------------------------------------------------

*/

void AddAlgorithms();

/* gsat.c */

void AddGSat();

/* gwsat.c */

void AddGWSat();
extern PROBABILITY iWp;

/* gsat-tabu.c */

void AddGSatTabu();
extern UINT32 iTabuTenure;

/* hsat.c */

void AddHSat();
void PickHSat();
void PickHSatW();

/* hwsat.c */

void AddHWSat();

/* walksat.c */

void AddWalkSat();
UINT32 PickClauseWCS();

/* walksat-tabu.c */

void AddWalkSatTabu();
void PickWalkSatTabu();
extern UINT32 iWalkSATTabuClause;

/* novelty.c */

void AddNovelty();
void AddNoveltyPlus();
void AddNoveltyPlusPlus();
void PickNoveltyPlusPlus();
void PickNoveltyPlusPlusW();

extern PROBABILITY iNovNoise;

/* rnovelty.c */

void AddRNovelty();
void AddRNoveltyPlus();

/* adaptnovelty.c */

void AddAdaptNoveltyPlus();
extern UINT32 iLastAdaptStep;
extern UINT32 iLastAdaptNumFalse;
extern FLOAT fLastAdaptSumFalseW;
extern const UINT32 iInvPhi;
extern const UINT32 iInvTheta;

/* saps.c */

void AddSAPS();

/* paws.c */

void AddPAWS();
extern PROBABILITY iPAWSFlatMove;

/* ddfw.c */

void AddDDFW();

/* g2wsat.c */

void AddG2WSat();

/* rots.c */

void AddRoTS();
void PickRoTS();

extern UINT32 iTabuTenureInterval;
extern UINT32 iTabuTenureLow;
extern UINT32 iTabuTenureHigh;

/* irots.c */

void AddIRoTS();

/* samd.c */

void AddSAMD();

/* random.c */

void AddRandom();

/* vw.c */

void AddVW();


