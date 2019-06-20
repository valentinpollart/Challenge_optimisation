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
    int x_position;
    int y_position;
}Cont;

typedef struct _OP        //Opérations (stockées dans un tableau)
{
    int container_id;     // Container (et donc numéro)
    int type;
    int ini_x;  // Position initiale du container (0 0 si exterieur)
    int ini_y;
    int fin_x;      // Position finale du container (0 si exterieur)
    int fin_y;
    struct _OP* next;
}OP;

typedef struct
{
    Cont*** bay_array;
    Cont** container_list;
    int* height_list;
    int* sum_list;

}Bay;

typedef struct _OPout//Opérations vers l'exterieur (stockées dans une liste chainee)
{
    Cont *C;         // Container (et donc numéro)
    int ini_x;      // Position initiale du container (0 0 si exterieur)
    int ini_y;
    struct _OPout * suiv;
}OPout;

typedef struct _OPin //Opérations internes (stockées dans une liste chainee)
{
    Cont *C;         // Container (et donc numéro)
    int ini_x;      // Position initiale du container (0 0 si exterieur)
    int ini_y;
    int fin_x;      // Position finale du container
    int fin_y;
    struct _OPin * suiv;
}OPin;

int nb_cont;
int l_baie;
int h_baie;
OP* op_head;


void addContainer(Bay* bay,int container_id,int column);
void removeContainer(Bay* bay,int column);
void moveContainer(Bay* bay,int initial_column,int final_column);
int findBestColumn(Bay* bay, int initial_column);
int isContainerMoveable(int container_id, Bay* bay);
int initialisation(int num);
void returnCSV(int num, OP* debut);



int main(int num, char *argv[])
{
    Bay *bay;
    int move_count = 0;
    int ok = initialisation(num);

    if(ok == 3)
    {
        printf("init is ok!");
        OP * opTot = (OP *) malloc(sizeof(OP));
        OP * debut = (OP *) malloc(sizeof(OP));
        debut->next = NULL;

        /* DEBUT DES MATHS */



        /* FIN DES MATHS */

        returnCSV(num, debut);

    }
    else
        printf("error in initialization\n");
    OP* current_op = op_head;
    while(current_op != NULL){
        if(current_op->type == 1){
            addContainer(bay,current_op->container_id,findBestColumn(bay,-1));
        }
        else{
            while(!isContainerMoveable(current_op->container_id,bay)){
                moveContainer(bay,bay->container_list[current_op->container_id]->x_position,findBestColumn(bay,bay->container_list[current_op->container_id]->x_position));
                move_count++;
            }
            removeContainer(bay,bay->container_list[current_op->container_id]->x_position);
        }
        current_op = current_op->next;
    }
    printf("Done : %d moves",move_count);

}

void addContainer(Bay* bay,int container_id,int column){
    bay->bay_array[column][bay->height_list[column]] = bay->container_list[container_id];
    bay->height_list[column]++;
    bay->sum_list[column] += bay->container_list[container_id];
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


int findBestColumn(Bay* bay, int container_id){
    int max_sum_column = 0;
    bool try = true;
    for(int i = 0; i < l_baie; i++){
        if(bay->container_list[container_id]->x_position != i){
            if (!bay->height_list[i]){
                return i;
            }
            if (bay->sum_list[i] > max_sum_column){
                max_sum_column = i;
            }
            for(int j = 0; j < bay->height_list[i]; j++){
                try = (bay->bay_array[i][j]->num < container_id);
            }
            if(try){
                return i;

            }
        }
    }
    return max_sum_column;
}

int isContainerMoveable(int container_id,Bay* bay){
    return bay->container_list[container_id]->y_position == bay->height_list[bay->container_list[container_id]->x_position];
}

int initialisation(int num)
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
    else
        printf("error opening %s\n", glob);


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
    else
        printf("error opening %s\n", posi);

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
    else
        printf("error opening %s\n", oper);

    return ok;
}


void returnCSV(int num, OP* debut)
{
    char * retCSV = "_solution.csv";
    sprintf(retCSV, "%d%s", num, retCSV);
    FILE * ret;
    ret = fopen(retCSV, "r");
    if(ret != NULL)
    {
        fprintf(ret, "FROM;TO\n");

        OP *temp_opX = (OP*) malloc(sizeof(OP));
        temp_opX = debut->next;

        while(temp_opX->next != NULL)
        {
            fprintf(ret, "%d;%d\n", temp_opX->ini_x, temp_opX->fin_x);
        }
    }
    else
        printf("error creating/opening %s\n", retCSV);
}