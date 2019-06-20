//
// Created by matte on 20/06/2019.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct      // Containers (stockés dans un tableau)
{
    int num;

}Cont;

typedef struct        //Opérations (stockées dans un tableau)
{
    Cont *C;     // Container (et donc numéro)
    int ini_x;  // Position initiale du container (0 0 si exterieur)
    int ini_y;
    int fin_x;  // Position finale du container (0 0 si exterieur)
    int fin_y;
}OP;

typedef struct
{
    Cont*** bay_array;
    int* height_list;
    int* sum_list;

}Bay;

typedef struct _OPout//Opérations vers l'exterieur (stockées dans une liste chainee)
{
    Cont C;         // Container (et donc numéro)
    int ini_x;      // Position initiale du container (0 0 si exterieur)
    int ini_y;
    struct _OPout * suiv;
}OPout;

typedef struct _OPin //Opérations internes (stockées dans une liste chainee)
{
    Cont C;         // Container (et donc numéro)
    int ini_x;      // Position initiale du container (0 0 si exterieur)
    int ini_y;
    int fin_x;      // Position finale du container
    int fin_y;
    struct _OPin * suiv;
}OPin;

int nb_cont;
int l_baie;
int h_baie;


void addContainer(Bay* bay,Cont *container,int column);
void removeContainer(Bay* bay,int column);
void moveContainer(Bay* bay,int initial_column,int final_column);
int findBestColumn(Bay* bay, Cont* container);





int main(int num, char *argv[])
{
    int ok = 0;


    char *glob = "_global.csv";
    char *oper = "_operation.csv";
    char *posi = "_position.csv";

    sprintf(glob, "%d%s", num, glob);
    sprintf(oper, "%d%s", num, oper);
    sprintf(posi, "%d%s", num, posi);

    int gl = open(glob, O_RDONLY);
    if(gl != -1)
    {
        ok++;       // ok pour x_global.csv

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
    Cont* tab_cont[l_baie][h_baie];


    int po = open(posi, O_RDONLY);
    if(po != -1)
    {
        ok++;        // ok pour x_position.csv

        char str[20];
        read(gl, str, sizeof(gl));

        /* Parsing pour les lignes */
        const char s[2] = "\n";
        char *token;
        token = strtok(str, s);
        token = strtok(NULL, s); //selection 2e ligne
        while (token != NULL)
        {
            int tempNum = 0;
            int tempNumX = 0;
            int tempNumY = 0;
            /* Parsing pour les ' , ' */
            sprintf(token, "CT%d , %d , %d", tempNum, tempNumX, tempNumY);

            tab_cont[tempNumX][tempNumY]->num = tempNum;
        }
    }

    int op = open(oper, O_RDONLY);
    if(op != -1) {
        ok++;        // ok pour x_operation.csv

        char str[20];
        read(op, str, sizeof(op));

        /* Parsing pour les lignes */
        const char s[2] = "\n";
        char *token;
        token = strtok(str, s);
        token = strtok(NULL, s); //selection 2e ligne

        OPout *temp_opXout = (OPout *) malloc(sizeof(OPout));
        temp_opXout = NULL;

        OPin *temp_opXin = (OPin *) malloc(sizeof(OPin));
        temp_opXin = NULL;

        while (token != NULL) {
            int tempNum = 0;
            char *tempChar = "0";
            /* Parsing pour les ' , ' */
            sprintf(token, "CT%d , %s", tempNum, tempChar);

            if (strcmp(tempChar, "R") == 0)
            {
                OPout *opXout = (OPout *) malloc(sizeof(OPout));
                opXout->C.num = tempNum;
                if (temp_opXout != NULL)
                    temp_opXout->suiv = opXout;
                temp_opXout = opXout;
            }
            else
            {
                char *tempCharX = "0";
                char *tempCharY = "0";
                sprintf(token, "CT%d , %s , %s", tempNum, tempCharX, tempCharY);
                OPin *opXin = (OPin *) malloc(sizeof(OPin));
                opXin->C.num = tempNum;
                opXin->fin_x = (int) tempCharX;
                opXin->fin_y = (int) tempCharY;
                if (temp_opXin != NULL)
                    temp_opXin->suiv = opXin;
                temp_opXin = opXin;
            }

            token = strtok(NULL, s);
        }
    }



    if(ok == 3)
    {
        printf("init is ok!");

        /* DEBUT */

        /* FIN */
    }

}

void addContainer(Bay* bay,Cont *container,int column){
    bay->bay_array[column][bay->height_list[column]] = container;
    bay->height_list[column]++;
    bay->sum_list[column] += container->num;
}


void removeContainer(Bay* bay,int column){
    bay->sum_list[column] -= bay->bay_array[column][bay->height_list[column]]->num;
    bay->bay_array[column][bay->height_list[column]] = NULL;
    bay->height_list[column]--;

}


void moveContainer(Bay* bay, int initial_column,int final_column){
    bay->bay_array[final_column][bay->height_list[final_column]] = bay->bay_array[initial_column][bay->height_list[initial_column]];
    bay->height_list[final_column]++;
    bay->sum_list[final_column] += bay->bay_array[final_column][bay->height_list[final_column]]->num;
    bay->sum_list[initial_column] -= bay->bay_array[initial_column][bay->height_list[initial_column]]->num;
    bay->bay_array[initial_column][bay->height_list[initial_column]] = NULL;
    bay->height_list[initial_column]--;
}


int findBestColumn(Bay* bay, Cont* container){
    int max_sum_column = 0;
    bool try = true;
    for(int i = 0; i < l_baie; i++){
        if (!bay->height_list[i]){
            return i;
        }
        if (bay->sum_list[i] > max_sum_column){
            max_sum_column = i;
        }
        for(int j = 0; j < bay->height_list[i]; j++){
            try = (bay->bay_array[i][j]->num < container->num);
        }
        if(try){
            return i;



        }
    }
    return max_sum_column;
}


