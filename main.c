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
    int fin_x;  // Position finale du container (0 0 si exterieur)
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

void init(Bay* bay);
void addContainer(Bay* bay,int container_id,int column);
void removeContainer(Bay* bay,int column);
void moveContainer(Bay* bay,int initial_column,int final_column);
int findBestColumn(Bay* bay, int initial_column);
int isContainerMoveable(int container_id, Bay* bay);





int main(int num, char *argv[])
{
    Bay *bay;
    int move_count = 0;
    init(bay);
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
