//
// Created by matte on 20/06/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

    int op = open(glob, O_RDONLY);
    if(op != -1){}

    Cont *tab_cont[l_baie][h_baie];
    int* height_list;

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

