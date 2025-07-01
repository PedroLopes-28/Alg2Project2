
#ifndef CONVERSAO_H
#define CONVERSAO_H


#include "234.h"
#include "Rb.h"

//Menus
void menuRB(rb* arv);
void menu234(arv234* arv);

//Conversão de 234 para RB
rb* converte234paraRB(arv234* arvOrigem);
noRB* converteDireto(no234* no, rb* arv, noRB* pai);

#endif //CONVERSAO_H
