#ifndef LIST_H
#define LIST_H

typedef struct List List;
void List_push_front(List* list, int value); // Вставка в начало списка
void List_push_back(List* list, int value); // Вставка в конец списка

#endif