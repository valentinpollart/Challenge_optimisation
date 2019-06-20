//
// Created by matte on 20/06/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cont     // Containers (stockés dans un tableau)
{
    int num;
};

struct OP       //Opérations (stockées dans un tableau)
{
    Cont C;     // Container (et donc numéro)
    int ini_x;  // Position initiale du container (0 0 si exterieur)
    int ini_y;
    int fin_x;  // Position finale du container (0 0 si exterieur)
    int fin_y;
};

int main(int num, char *argv[])
{
    int ok = 0;
    int nb_cont = 0;
    int L_baie = 0;
    int H_baie = 0;

    char glob = "_global.csv";
    char oper = "_operation.csv";
    char posi = "_position.csv";

    sprintf(glob, "%d%s", num, glob);
    sprintf(oper, "%d%s", num, oper);
    sprintf(posi, "%d%s", num, posi);

    int op = open(glob, O_RDONLY);
    if(op != -1)

    Cont** tab_cont[L_baie][H_baie];
}