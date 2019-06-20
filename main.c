//
// Created by matte on 20/06/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



typefdef struct     // Containers (stockés dans un tableau)
{
    int num;
}Cont;

typedef struct OPout      //Opérations (stockées dans une liste chainee)
{
    Cont C;     // Container (et donc numéro)
    int ini_x;  // Position initiale du container (0 0 si exterieur)
    int ini_y;
    int fin_x;  // Position finale du container (0 0 si exterieur)
    int fin_y;
    OPout * suiv;
}OPout;

typedef struct OPin       //Opérations (stockées dans une liste chainee)
{
    Cont C;     // Container (et donc numéro)
    int ini_x;  // Position initiale du container (0 0 si exterieur)
    int ini_y;
    int fin_x;  // Position finale du container (0 0 si exterieur)
    int fin_y;
    OPin * suiv;
}OPin;



int main(int num, char *argv[])
{
    int ok = 0;
    int nb_cont = 0;
    int l_baie = 0;
    int h_baie = 0;

    char * glob = "_global.csv";
    char * oper = "_operation.csv";
    char * posi = "_position.csv";

    sprintf(glob, "%d%s", num, glob);
    sprintf(oper, "%d%s", num, oper);
    sprintf(posi, "%d%s", num, posi);

    int gl = open(glob, O_RDONLY);
    if(gl != -1)
    {
        ok++; // ok pour x_global.csv

        char str[20];
        read(gl, str, sizeof(gl));

        /* Parsing pour les lignes */
        const char s[2] = "\n";
        char * token;
        token = strtok(str, s);
        token = strtok(NULL, s); //selection 2e ligne

        /* Parsing pour les ' , ' */
        int i = 0;
        const char s2[3] = " , ";
        token = strtok(str, s2);
        while(token != NULL && i<3)
        {
            if(i==0)
                nb_cont = (int) token;
            if(i==1)
                l_baie = (int) token;
            if(i==2)
                h_baie = (int) token;

            token = strtok(NULL, s2);
            i++;
        }
    }


    /* Creation du Tableau des containers */
    Cont*** tab_cont[l_baie][h_baie];


    int op = open(oper, O_RDONLY);
    if(op != -1)
    {
        ok++; // ok pour x_operation.csv

        char str[20];
        read(gl, str, sizeof(gl));

        /* Parsing pour les lignes */
        const char s[2] = "\n";
        char * token;
        token = strtok(str, s);
        token = strtok(NULL, s); //selection 2e ligne
        while(token != NULL)
        {
            int tempNum = 0;
            char tempChar = "0";
            /* Parsing pour les ' , ' */
            fscanf(gl, "CT%d , %c", tempNum, tempChar);

            if(strcmp())
                OPout * opX = (OPout*) malloc(sizeof(OPout));

            token = strtok(NULL, s);
        }

    }


}