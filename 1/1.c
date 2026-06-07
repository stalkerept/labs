#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print_sp(double** sp, int m, int* n_i){
    for(int i = 0; i<m; i++){
        printf("\n");
        for(int j = 0; j < n_i[i]; j++){
            printf("%lf ", sp[i][j]);
        }
    }
}
double* avarage_vec(double** sp, int m, int* n_i){
    double* avarage = (double*)malloc(sizeof(double) * m);
    if(avarage == NULL){
        free(avarage);
        printf("Error memory avarage");
        return NULL;
    }
    for(int i = 0; i<m; i++){
        double c = 0;
        for(int j = 0; j<n_i[i]; j++){
            c += sp[i][j];
        }
        c = c*1./n_i[i];
        avarage[i] = c;
    }
    return avarage;
}

double* m_v_g_a(double** sp, int m, int* n_i, char* flag){
    double* avarage = avarage_vec(sp, m, n_i);
    if(avarage==NULL){
        return NULL;
    }
    double maxim = avarage[0];
    double* vec = (double*)malloc(sizeof(double)*m);
    for(int i = 0; i<m; i++){
        if(avarage[i] > maxim){
            maxim = avarage[i];
        }
    }
    if(maxim != 0){
        for(int i = 0; i<m; i++){
        vec[i] = avarage[i]*1./maxim;
        }
    }
    else{
        free(vec);
        vec = NULL;
        *flag = 1; 
    }
    free(avarage);
    return vec;
}

int main(void){
    int m = 0, c;
    double m_lf;
    printf("Type rows and columns\n");
    printf("Rows:");
    while(scanf("%lf", &m_lf)!=1 || m_lf<=0 || m_lf!=(int)m_lf){
        printf("The number of rows is > 0, there are no letters and no floating point numbers. Please try again.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Rows:");
    }
    m = (int)m_lf;
    int* n_i = (int*)malloc(sizeof(int) * m);
    if(n_i == NULL){
        printf("Error memory for n_i");
        return 1;
    }
    double** sp = (double**)malloc(sizeof(double*) * m);
    if(sp == NULL){
        printf("Error memory for sp");
        return 1;
    }
    for(int i = 0; i < m; i++){
        printf("Columns:");
        double n = 0;
        while(scanf("%lf", &n)!=1 || n<=0 || n != (int)n){
            printf("The number of columns is > 0, there are no letters and no floating point numbers. Please try again.\n");
            while ((c = getchar()) != '\n' && c!=EOF);
            printf("Columns:");
        }
        n_i[i] = (int)n;
        sp[i] = (double*)malloc(sizeof(double) * n_i[i]);
        if(sp[i] == NULL){
            printf("Error memory for sp[%d]", i);
            for(int g = 0; g<i; g++){
                free(sp[g]);
            }
            free(sp);      
            free(n_i);
            return 1;
        }
        for(int j = 0; j < n_i[i]; j++){
            printf("Row: %d; Column: %d. Number:", i+1, j+1);
            while(scanf("%lf", &sp[i][j])!=1){
                printf("Please, try again");
                printf("\nRow: %d; Column: %d. Number:", i+1, j+1);
                int c;
                while ((c = getchar()) != '\n' && c!=EOF);
            }
        }
    }
    printf("Matrix before:\n");
    print_sp(sp, m, n_i);
    printf("\nVector:\n");
    char flag = 0;
    double* vec = m_v_g_a(sp, m, n_i, &flag);
    for(int i = 0; i<m; i++){
        free(sp[i]);
    }
    free(sp);
    free(n_i);
    if(vec == NULL){
        printf("\nError memory for vec\n");
        if(flag==1){
            printf("Maximum = 0\n");
        }
        return 1;
    }
    for(int i = 0; i<m; i++){
        printf("%lf ", vec[i]);
    }
    free(vec);
    return 0;
}