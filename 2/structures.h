#ifndef STRUCTURES_H
#define STRUCTURES_H


typedef struct 
{  
    char j_a;
    char id[100];
    int ta;
    int ts;
}Info;


struct Person{
    Info man;
    struct Person* next;
};

typedef struct
{
    struct Person* top;
    struct Person* bottom;
}Queue;

typedef struct
{
    char error_char;
    char number;
}Error;



#endif