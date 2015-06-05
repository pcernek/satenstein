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

typedef UINT32 LITTYPE;

#define GetTrueLit(V) (((V) << 1) + 1 - aVarValue[V])
#define GetFalseLit(V) (((V) << 1) + aVarValue[V])

#define GetPosLit(V) (((V) << 1))
#define GetNegLit(V) (((V) << 1) + 1)

#define GetNegatedLit(L) ((L) ^ 1)

#define GetLitSign(L) ((L) & 1)

#define GetVarFromLit(L) ((L) >> 1)
#define GetVar(c,p) (pClauseLits[c][p] >> 1)

#define IsLitTrue(L) (aVarValue[(L) >> 1] ^ (L & 1))

#define SetLitFromFile(V) ((LITTYPE) (((V) < 0) ? (((-(V)) << 1) + 1) : ((V) << 1)))

