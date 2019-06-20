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
//    int ini_x;  // Position initiale du container (0 0 si exterieur)
//    int ini_y;
//    int fin_x;      // Position finale du container (0 si exterieur)
//    int fin_y;
    struct _OP* next;
}OP;

typedef struct _op_move{
    int initial_x;
    int final_x;
    struct _op_move* next;
}op_move;

typedef struct
{
    Cont* bay_array[20][20];
    Cont* container_list[100];
    int height_list[100];
    int sum_list[100];

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
op_move* op_move_head;


void addContainer(Bay* bay,int container_id,int column);
void removeContainer(Bay* bay,int column);
void moveContainer(Bay* bay,int initial_column,int final_column);
int findBestColumn(Bay* bay, int container_id);
int isContainerMoveable(int container_id, Bay* bay);
int initialisation(int num,Bay* bay);
void returnCSV(int num);



int main(int argc, char *argv[])
{
    Bay *bay;
    bay = (Bay*) malloc(sizeof(Bay));

    char *num_str = argv[1];
    int num = atoi(num_str);

    int move_count = 0;
    int ok = initialisation(num,bay);

    if(ok == 3)
    {
        printf("init is ok!");
        OP * opTot = (OP *) malloc(sizeof(OP));
        OP * debut = (OP *) malloc(sizeof(OP));
        debut->next = NULL;

        /* DEBUT DES MATHS */



        /* FIN DES MATHS */



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
    returnCSV(num);
    printf("Done : %d moves",move_count);

}

void addContainer(Bay* bay,int container_id,int column){

    bay->bay_array[column][bay->height_list[column]-1] = bay->container_list[container_id];
    bay->height_list[column]++;
    bay->sum_list[column] += container_id;
    bay->container_list[container_id]->x_position = column;
    bay->container_list[container_id]->y_position = bay->height_list[column];


}


void removeContainer(Bay* bay,int column){
    bay->sum_list[column] -= bay->bay_array[column][bay->height_list[column]-1]->num;
    bay->container_list[bay->bay_array[column][bay->height_list[column]-1]->num]->x_position = -1;
    bay->container_list[bay->bay_array[column][bay->height_list[column]-1]->num]->y_position = -1;
    bay->bay_array[column][bay->height_list[column]-1] = NULL;
    bay->height_list[column]--;


}


void moveContainer(Bay* bay, int initial_column,int final_column){
    bay->bay_array[final_column][bay->height_list[final_column]] = (Cont*)malloc(sizeof(Cont));
    bay->bay_array[final_column][bay->height_list[final_column]]->num = bay->bay_array[initial_column][bay->height_list[initial_column]-1]->num;
    bay->bay_array[final_column][bay->height_list[final_column]]->x_position = bay->bay_array[initial_column][bay->height_list[initial_column]-1]->x_position;
    bay->bay_array[final_column][bay->height_list[final_column]]->y_position = bay->bay_array[initial_column][bay->height_list[initial_column]-1]->y_position;
    bay->container_list[bay->bay_array[final_column][bay->height_list[final_column]-1]->num]->x_position = bay->bay_array[final_column][bay->height_list[final_column]-1]->x_position;
    bay->container_list[bay->bay_array[final_column][bay->height_list[final_column]-1]->num]->y_position = bay->bay_array[final_column][bay->height_list[final_column]-1]->y_position;
    bay->sum_list[final_column] += bay->bay_array[final_column][bay->height_list[final_column]-1]->num;
    bay->height_list[final_column]++;
    bay->sum_list[initial_column] -= bay->bay_array[initial_column][bay->height_list[initial_column]-1]->num;
    bay->bay_array[initial_column][bay->height_list[initial_column]-1]->num = -1;
    bay->height_list[initial_column]--;
    op_move* current_op_move = op_move_head;
    if(current_op_move == NULL){
        current_op_move = (op_move*)malloc(sizeof(op_move));
        current_op_move->initial_x = initial_column;
        current_op_move->final_x = final_column;
        current_op_move->next = NULL;
        op_move_head = current_op_move;
    }
    else{
        while(current_op_move->next != NULL) {
            current_op_move = current_op_move->next;
        }
        current_op_move->next = (op_move*)malloc(sizeof(op_move));
        current_op_move = current_op_move->next;
        current_op_move->initial_x = initial_column;
        current_op_move->final_x = final_column;
        current_op_move->next = NULL;
    }



}


int findBestColumn(Bay* bay, int container_id){
    int max_sum_column = 0;
    bool try = true;
    for(int i = 0; i < l_baie; i++){
        if(bay->container_list[container_id]->x_position != i){
            if (!bay->height_list[i]){
                return i;
            }
            if (bay->sum_list[i] < max_sum_column){
                max_sum_column = i;
            }
            for(int j = 0; j < bay->height_list[i]; j++){
                if(bay->bay_array[i][j]->num < container_id){
                    try = false;

                }
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

int initialisation(int num, Bay* bay)
{
    int ok = 0;
    op_move_head = NULL;
    char *min_glob = "_global.csv";
    char *min_oper = "_operations.csv";
    char *min_posi = "_position.csv";

    char glob[50];
    char oper[50];
    char posi[50];

    sprintf(glob, "instancesChallengeCRP/%d%s", num, min_glob);
    sprintf(oper, "instancesChallengeCRP/%d%s", num, min_oper);
    sprintf(posi, "instancesChallengeCRP/%d%s", num, min_posi);

    FILE* gl;
    gl = fopen(glob, "r");
    if(gl != NULL)
    {
        ok++;       // ok pour x_global.csv

        char str[20];
        fgets(str,sizeof(str),gl);
        fgets(str,sizeof(str),gl);

        /* Parsing pour les ' , ' */
        sscanf(str, "%d , %d , %d",&nb_cont,&l_baie,&h_baie);
    }
    else
        printf("error opening %s\n", glob);
    fclose(gl);


    /* Creation du Tableau des containers */
    Cont* tab_cont[l_baie][h_baie];


    FILE* po = fopen(posi, "r");

    if(po != -1)
    {
        ok++;        // ok pour x_position.csv

        char str[50];
        fgets(str,sizeof(str),po);
        /* Parsing pour les lignes */
        while (fgets(str,sizeof(str),po) != NULL)
        {
            int tempNum = 0;
            int tempNumX = 0;
            int tempNumY = 0;
            /* Parsing pour les ' , ' */
            sscanf(str, "CT%d , %d , %d", &tempNum, &tempNumX, &tempNumY);
            bay->bay_array[tempNumX-1][tempNumY-1] = (Cont*)malloc(sizeof(Cont));
            bay->bay_array[tempNumX-1][tempNumY-1]->num = tempNum-1;
            bay->container_list[tempNum-1] = (Cont*)malloc(sizeof(Cont));
            bay->container_list[tempNum-1]->x_position =tempNumX-1;
            bay->container_list[tempNum-1]->y_position =tempNumY-1;
            bay->height_list[tempNumX-1]++;
            bay->sum_list[tempNumX-1]+=tempNum;
        }
    }
    else
        printf("error opening %s\n", posi);
    fclose(po);
    FILE* op;
    op = fopen(oper, "r");
    OP* current_op = op_head;
    if(op != NULL) {
        ok++;        // ok pour x_operation.csv
        int tempNum = 0;
        char tempChar;
        char str[20];
        fgets(str,sizeof(str),op);




        while (fgets(str,sizeof(str),op) != NULL) {

            /* Parsing pour les ' , ' */
            sscanf(str, "CT%d , %c", &tempNum, &tempChar);
            current_op = (OP*)malloc(sizeof(OP));
            current_op->container_id = tempNum-1;
            if (strcmp(&tempChar, "R") == 0)
            {
//                OPout *opXout = (OPout *) malloc(sizeof(OPout));
//                opXout->C->num = tempNum;
//                if (temp_opXout != NULL)
//                    temp_opXout->suiv = opXout;
//                temp_opXout = opXout;
                current_op->type = 0;
            }
            else
            {
//                int tempCharX = 0;
//                int tempCharY = 0;
//                sscanf(token, "CT%d , %s\r", &tempNum, tempChar);
//                OPin *opXin = (OPin *) malloc(sizeof(OPin));
//                opXin->C->num = tempNum;
//                opXin->fin_x = tempCharX;
//                opXin->fin_y = tempCharY;
//                if (temp_opXin != NULL)
//                    temp_opXin->suiv = opXin;
//                temp_opXin = opXin;
                current_op->type = 1;
            }
            if (op_head == NULL ){
                op_head = current_op;
            }
            fgets(str,sizeof(str),op);
        }
    }
    else
        printf("error opening %s\n", oper);
    fclose(op);
    return ok;
}


void returnCSV(int num)
{
    char * min_retCSV = "_solution.csv";
    char retCSV[50];
    sprintf(retCSV, "%d%s", num, min_retCSV);
    FILE * ret;
    ret = fopen(retCSV, "w");

    if(ret != NULL)
    {
        fprintf(ret, "FROM;TO\n");

        op_move *temp_opX = op_move_head;

        while(temp_opX->next != NULL)
        {
            fprintf(ret, "%d;%d\n", temp_opX->initial_x+1, temp_opX->final_x+1);
            temp_opX = temp_opX->next;
        }
    }
    else
        printf("error creating/opening %s\n", retCSV);
}