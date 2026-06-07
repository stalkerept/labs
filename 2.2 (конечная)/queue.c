#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "func.h"

void init_queue(Queue* q, int impl) {
    q->impl = impl;
    if (impl == 0) { // список
        q->u.list.top = NULL;
        q->u.list.bottom = NULL;
    } else { // массив
        q->u.array.data = NULL;
        q->u.array.capacity = 0;
        q->u.array.front = 0;
        q->u.array.back = 0;
        q->u.array.size = 0;
    }
}

void clear_queue(Queue* q) {
    if (q->impl == 0) { // список
        struct Person* now = q->u.list.top;
        while (now != NULL) {
            struct Person* next = now->next;
            free(now);
            now = next;
        }
        q->u.list.top = NULL;
        q->u.list.bottom = NULL;
    } else { // массив
        if (q->u.array.data) {
            free(q->u.array.data);
            q->u.array.data = NULL;
        }
        q->u.array.capacity = 0;
        q->u.array.front = 0;
        q->u.array.back = 0;
        q->u.array.size = 0;
    }
}

int is_empty_queue(Queue* q) {
    if (q->impl == 0)
        return (q->u.list.top == NULL);
    else
        return (q->u.array.size == 0);
}

// Вспомогательная функция для увеличения ёмкости массива
static Error ensure_capacity_array(Queue* q, int needed) {
    Error err = {0, ERR_OK};
    if (q->u.array.capacity >= needed) return err;
    int new_capacity = (q->u.array.capacity == 0) ? 4 : q->u.array.capacity * 2;
    while (new_capacity < needed) new_capacity *= 2;
    Info* new_data = (Info*)malloc(new_capacity * sizeof(Info));
    if (!new_data) {
        err.error_char = 1;
        err.number = ERR_MEMORY_ENSURE_CAPACITY;
        return err;
    }
    // Перемещаем элементы в начало нового массива
    for (int i = 0; i < q->u.array.size; i++) {
        new_data[i] = q->u.array.data[(q->u.array.front + i) % q->u.array.capacity];
    }
    free(q->u.array.data);
    q->u.array.data = new_data;
    q->u.array.capacity = new_capacity;
    q->u.array.front = 0;
    q->u.array.back = q->u.array.size;
    return err;
}

Error push_queue(Queue* q, Info value) {
    Error err = {0, ERR_OK};
    if (q->impl == 0) { // список
        struct Person* new_person = (struct Person*)malloc(sizeof(struct Person));
        if (!new_person) {
            err.error_char = 1;
            err.number = ERR_MEMORY_PUSH;
            return err;
        }
        new_person->man = value;
        new_person->next = NULL;

        if (is_empty_queue(q)) {
            q->u.list.top = new_person;
            q->u.list.bottom = new_person;
        } else if (value.j_a == 1) { // приоритетный – в начало
            new_person->next = q->u.list.top;
            q->u.list.top = new_person;
        } else { // обычный – в конец
            q->u.list.bottom->next = new_person;
            q->u.list.bottom = new_person;
        }
    } else { // массив
        err = ensure_capacity_array(q, q->u.array.size + 1);
        if (err.error_char) return err;
        if (value.j_a == 1) { // приоритетный – в начало
            q->u.array.front = (q->u.array.front - 1 + q->u.array.capacity) % q->u.array.capacity;
            q->u.array.data[q->u.array.front] = value;
        } else { // обычный – в конец
            q->u.array.data[q->u.array.back] = value;
            q->u.array.back = (q->u.array.back + 1) % q->u.array.capacity;
        }
        q->u.array.size++;
    }
    return err;
}

Info pop_front_queue(Queue* q) {
    // Предполагается, что очередь не пуста
    Info value;
    if (q->impl == 0) { // список
        struct Person* first = q->u.list.top;
        value = first->man;
        q->u.list.top = first->next;
        if (q->u.list.top == NULL)
            q->u.list.bottom = NULL;
        free(first);
    } else { // массив
        value = q->u.array.data[q->u.array.front];
        q->u.array.front = (q->u.array.front + 1) % q->u.array.capacity;
        q->u.array.size--;
        if (q->u.array.size == 0) {
            q->u.array.front = 0;
            q->u.array.back = 0;
        }
    }
    return value;
}

void print_queue(Queue* q) {
    printf("Queue (impl=%d):\n", q->impl);
    if (q->impl == 0) { // список
        struct Person* now = q->u.list.top;
        while (now != NULL) {
            printf("  %s (ta=%d, ts=%d, j_a=%d)\n", now->man.id, now->man.ta, now->man.ts, now->man.j_a);
            now = now->next;
        }
    } else { // массив
        for (int i = 0; i < q->u.array.size; i++) {
            Info* p = &q->u.array.data[(q->u.array.front + i) % q->u.array.capacity];
            printf("  %s (ta=%d, ts=%d, j_a=%d)\n", p->id, p->ta, p->ts, p->j_a);
        }
    }
}