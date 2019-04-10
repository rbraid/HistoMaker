#ifndef SETUPHISTOS_ONLY_H
#define SETUPHISTOS_ONLY_H

#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TList.h>
#include <cstdio>

void SetupHistos(TList *outlist);
void SetupHistosDualAndPID(TList *outlist);

#endif