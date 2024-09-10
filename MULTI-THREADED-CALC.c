#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <complex.h>

//    ----------------------------    define   --------------------------------------

#define OR "OR"
#define AND "AND"
#define MUL "MUL"
#define SUB "SUB"
#define ADD "ADD"
#define INT "INT"
#define DUB "DUB"
#define COM "COM"
#define BIN "BIN"
#define BBB "BBB"
#define MAX_LINE  129
#define INITIAL_SIZE 50

//    ----------------------------    define   --------------------------------------

typedef struct{
    char matrix1[MAX_LINE];
    char matrix2[MAX_LINE];
    char result[MAX_LINE];
    int index;
}DATA;

char MTSTR1[MAX_LINE] = "\0";
char MTSTR2[MAX_LINE] = "\0";
char op[MAX_LINE] = "\0";
int dataCounter = 0;
DATA *dataArray = NULL;
int currentSize = 0;      ///////////   counter for initializing the data array
int printERR = 0;
int checkEND = 0;

void initializeDataArray();
void updateDataArraySize();
void addData();
int inputData();
void* thread_function(void* arg);
void THREAD();
void run(const char* MATRIX1,const char* MATRIX2, const char* OP, char RESULT[MAX_LINE]);
void** cast(const char matrix[], int* MATRIX_SIZE, char type[]);
void check_type(const char matrix[], char* type);
int checkBIN(const char* BIN_MATRIX);
void freeMatrix(void** MT,const char* type, int MATRIX_SIZE);
void parse_complex_number(const char* data, int* real_part, int* imag_part);
void complexToString(char* append, int rel, int img);
double multiply_and_round(double a, double b);

int main(void) {

    while(1)
    {
        initializeDataArray();
        while(1)
        {
            int checkOP = inputData();
            if(checkOP)
            {
                break;
            }
            addData();
        }
        if(checkEND == 1)
        {
            break;
        }

        THREAD();
        if(printERR)
        {
            printf("ERR");
        }
        else
        {
            printf("%s\n",dataArray[0].result);
        }
        free(dataArray);
        dataArray = NULL;
        dataCounter = 0;
        currentSize = 0;
        printERR = 0;
    }
    free(dataArray);
    return 0;
}

void THREAD()
{
    if(dataArray == NULL)
    {
        return;
    }
    pthread_t threads[dataCounter];
    int thread_args[dataCounter];

    for(int i = 0;i<dataCounter;i++)
    {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("THREAD");
            exit(1);
        }
    }

    for (int i = 0; i < dataCounter; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            exit(1);
        }
    }

    if(printERR || dataCounter == 1)
    {
        return;
    }
    if(dataCounter > 1 && dataCounter % 2 == 1)
    {
        for(int i = 0;i<dataCounter;i++)
        {
            strcpy(dataArray[i].matrix1,"\0");
            strcpy(dataArray[i].matrix2,"\0");
        }
        run(dataArray[0].result,dataArray[1].result,op,dataArray[0].matrix1);
        strcpy(dataArray[0].matrix2,dataArray[2].result);
        int index = 1;
        for(int i = 3; i<dataCounter;i+=2)
        {
            strcpy(dataArray[index].matrix1,dataArray[i].result);
            strcpy(dataArray[index].matrix2,dataArray[i+1].result);
            index++;
            strcpy(dataArray[i].result,"\0");
            strcpy(dataArray[i+1].result,"\0");
        }
        for(int i = index+1; i<dataCounter;i++)
        {
            strcpy(dataArray[i].matrix1,"\0");
            strcpy(dataArray[i].result,"\0");
            strcpy(dataArray[i].matrix2,"\0");
        }
        dataCounter = index;
        THREAD();
    }


//    (2,2:100101,1001010,1101010,110101)
//    (2,2:001000,0011111,0001110,000011)
//    (2,2:111111,1111111,1111111,111111)
//    (2,2:111111,1111111,1111111,111111)
//    OR
    else
    {
        int j = 0;
        int k = 1;
        for(int i = 0;i<dataCounter / 2;i++)
        {
            strcpy(dataArray[i].matrix1,dataArray[j].result);
            strcpy(dataArray[i].result,"\0");
            strcpy(dataArray[i].matrix2,dataArray[k].result);
            strcpy(dataArray[i].result,"\0");
            j+=2;
            k+=2;
        }
        for(int i = dataCounter/2; i<dataCounter;i++)
        {
            strcpy(dataArray[i].matrix1,"\0");
            strcpy(dataArray[i].result,"\0");
            strcpy(dataArray[i].matrix2,"\0");
        }
        dataCounter = dataCounter / 2;
        THREAD();
    }
}

void* thread_function(void* arg) {
    int index = *((int*)arg);
    char MT1[MAX_LINE] = "\0";
    char MT2[MAX_LINE] = "\0";
    char OP[MAX_LINE] = "\0";
    strcpy(MT1,dataArray[index].matrix1);
    strcpy(MT2,dataArray[index].matrix2);
    strcpy(OP,op);

    run(MT1,MT2,OP,dataArray[index].result);


    return NULL;
}

int inputData()
{
    strcpy(MTSTR1,"\0");
    strcpy(MTSTR2,"\0");
    fgets(MTSTR1,MAX_LINE,stdin);
    MTSTR1[strlen(MTSTR1) - 1] = '\0';
    if(MTSTR1[0] != '(')
    {
        if(strcmp(MTSTR1,"END") == 0)
        {
            checkEND = 1;
        }
        strcpy(op,MTSTR1);
        return 1;
    }
    fgets(MTSTR2,MAX_LINE,stdin);
    MTSTR2[strlen(MTSTR2) - 1] = '\0';
    return 0;
}

void initializeDataArray() {
    currentSize = INITIAL_SIZE;
    dataArray = (DATA *)malloc(currentSize * sizeof(DATA));
    if (dataArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    dataCounter = 0;
}


void updateDataArraySize() {
    int newSize = currentSize + INITIAL_SIZE;
    DATA *tempArray = (DATA *)realloc(dataArray, newSize * sizeof(DATA));
    if (tempArray == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        // Clean up existing dataArray to avoid memory leak
        free(dataArray);
        exit(1);
    }
    dataArray = tempArray;
    currentSize = newSize;
}

void addData() {
    if (dataCounter >= currentSize) {
        updateDataArraySize();
    }
    // Copy the data into the next available slot
    strncpy(dataArray[dataCounter].matrix1, MTSTR1, MAX_LINE - 1);
    dataArray[dataCounter].matrix1[MAX_LINE - 1] = '\0';  // Ensure null-termination

    strncpy(dataArray[dataCounter].matrix2, MTSTR2, MAX_LINE - 1);
    dataArray[dataCounter].matrix2[MAX_LINE - 1] = '\0';  // Ensure null-termination

    dataArray[dataCounter].index = dataCounter;

    // Increment the counter
    dataCounter++;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////

void run(const char* MATRIX1,const char* MATRIX2, const char* OP, char RESULT[MAX_LINE])
{
    if(printERR)
    {
        return;
    }
    char type[15] = "\0";
    int MATRIX_SIZE;
    strcpy(type,BBB);
    MATRIX_SIZE = 0;
    strcpy(RESULT,"\0");


    if(strcmp(OP, OR) == 0 || strcmp(OP, AND) == 0){

        if(!checkBIN(MATRIX1) || !checkBIN(MATRIX2))
        {
            printERR = 1;
            return;
        }

        if(strcmp(OP,AND)==0)
        {
            strcpy(RESULT,"\0");
            char rc;
            int i = 0;
            while(MATRIX1[i] != '\0')
            {
                rc = 'c';
                if(MATRIX1[i] == '1' || MATRIX1[i] == '0')
                {
                    if(MATRIX1[i] == '1' && MATRIX2[i] == '1')
                    {
                        rc = '1';
                    }
                    else
                    {
                        rc = '0';
                    }
                }
                if(rc != 'c')
                {
                    char append[2];
                    append[0] = rc;
                    append[1] = '\0';
                    strcat(RESULT,append);
                }
                else
                {
                    char append[2];
                    append[0] = MATRIX1[i];
                    append[1] = '\0';
                    strcat(RESULT,append);
                }
                i++;
            }
            return;
        }
        else if(strcmp(OP,OR)==0)
        {
            strcpy(RESULT,"\0");
            char rc;
            int i = 0;
            while(MATRIX1[i] != '\0')
            {
                rc = 'c';
                if(MATRIX1[i] == '1' || MATRIX1[i] == '0')
                {
                    if(MATRIX1[i] == '1' || MATRIX2[i] == '1')
                    {
                        rc = '1';
                    }
                    else
                    {
                        rc = '0';
                    }
                }
                if(rc != 'c')
                {
                    char append[2];
                    append[0] = rc;
                    append[1] = '\0';
                    strcat(RESULT,append);
                }
                else
                {
                    char append[2];
                    append[0] = MATRIX1[i];
                    append[1] = '\0';
                    strcat(RESULT,append);
                }
                i++;
            }
            return;
        }
    }


    void** MT1 = cast(MATRIX1,&MATRIX_SIZE,type);
    void** MT2 = cast(MATRIX2,&MATRIX_SIZE,type);

    sprintf(RESULT, "(%d,%d:", MATRIX_SIZE, MATRIX_SIZE);

    if(strcmp(OP,ADD)==0)
    {
        if(strcmp(type,INT) == 0)
        {
            int** mtx1 = (int**)MT1;
            int** mtx2 = (int**)MT2;
            int sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = mtx1[i][j] + mtx2[i][j];
                    char append[15]="\0";
                    sprintf(append,"%d,",sum);
                    strcat(RESULT,append);
                }
            }
            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
        else if(strcmp(type,DUB) == 0)
        {
            double** mtx1 = (double**)MT1;
            double** mtx2 = (double**)MT2;
            double sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = mtx1[i][j] + mtx2[i][j];
                    char append[20]="\0";
                    sprintf(append,"%.1f,",sum);
                    strcat(RESULT,append);
                }
            }
            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
        else if(strcmp(type,COM) == 0)
        {
            int complex** mtx1 = (int complex**)MT1;
            int complex** mtx2 = (int complex**)MT2;
            int complex sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = mtx1[i][j] + mtx2[i][j];
                    char append[40]="\0";
                    int img = (int)cimag(sum);
                    int rel = (int)creal(sum);
                    complexToString(append,rel,img);
                    strcat(RESULT,append);
                }
            }

            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
    }

    else if(strcmp(OP,SUB)==0)
    {
        if(strcmp(type,INT) == 0)
        {
            int** mtx1 = (int**)MT1;
            int** mtx2 = (int**)MT2;
            int sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = mtx1[i][j] - mtx2[i][j];
                    char append[15]="\0";
                    sprintf(append,"%d,",sum);
                    strcat(RESULT,append);
                }
            }
            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
        else if(strcmp(type,DUB) == 0)
        {
            double** mtx1 = (double**)MT1;
            double** mtx2 = (double**)MT2;
            double sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = mtx1[i][j] - mtx2[i][j];
                    char append[20]="\0";
                    sprintf(append,"%.1f,",sum);
                    strcat(RESULT,append);
                }
            }
            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
        else if(strcmp(type,COM) == 0)
        {
            int complex** mtx1 = (int complex**)MT1;
            int complex** mtx2 = (int complex**)MT2;
            int complex sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = mtx1[i][j] - mtx2[i][j];
                    char append[40]="\0";
                    int img = (int)cimag(sum);
                    int rel = (int)creal(sum);
                    complexToString(append,rel,img);
                    strcat(RESULT,append);
                }
            }

            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
    }
    else if(strcmp(OP,MUL)==0)
    {
        if(strcmp(type,INT) == 0)
        {
            int** mtx1 = (int**)MT1;
            int** mtx2 = (int**)MT2;
            int sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = 0;
                    for (int k = 0; k < MATRIX_SIZE; k++) {
                        sum += mtx1[i][k] * mtx2[k][j];
                    }
                    char append[15]="\0";
                    sprintf(append,"%d,",sum);
                    strcat(RESULT,append);
                }
            }
            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
        else if(strcmp(type,DUB) == 0)
        {
            double ** mtx1 = (double**)MT1;
            double** mtx2 = (double**)MT2;
            double sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = 0;
                    for (int k = 0; k < MATRIX_SIZE; k++) {
                        sum += multiply_and_round(mtx1[i][k] , mtx2[k][j]); //mtx1[i][k] * mtx2[k][j];
                    }
                    char append[20]="\0";
                    sprintf(append,"%.1f,",sum);
                    strcat(RESULT,append);
                }
            }
            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
        else if(strcmp(type,COM) == 0)
        {
            int complex** mtx1 = (int complex**)MT1;
            int complex** mtx2 = (int complex**)MT2;
            int complex sum;
            for(int i = 0; i<MATRIX_SIZE; i++)
            {
                for(int j = 0; j<MATRIX_SIZE; j++)
                {
                    sum = 0;
                    for (int k = 0; k < MATRIX_SIZE; k++) {
                        sum += mtx1[i][k] * mtx2[k][j];
                    }
                    char append[40]="\0";
                    int img = (int)cimag(sum);
                    int rel = (int)creal(sum);
                    complexToString(append,rel,img);
                    strcat(RESULT,append);
                }
            }

            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            freeMatrix(MT1,type,MATRIX_SIZE);
            freeMatrix(MT2,type,MATRIX_SIZE);
            return;
        }
    }

}

void complexToString(char* append, int rel, int img)
{
    if(img > 0 && rel != 0)
    {
        if(img != 1)
        {
            sprintf(append, "%d+%di,", rel, img);
        }
        else
        {
            sprintf(append, "%d+1i,", rel);
        }
    }
    else if(rel != 0 && img < 0)
    {
        if(img != -1)
        {
            sprintf(append, "%d%di,", rel, img);
        }
        else
        {
            sprintf(append, "%d-1i,", rel);
        }
    }
    else if(img == 0)
    {
        if(rel != 0)
        {
            sprintf(append, "%d+0i,", rel);
        }
        
        else
        {
            sprintf(append, "0i,");
        }
    }
    else
    {
        if(img != 1 && img != -1)
        {
            sprintf(append, "%di,", img);
        }
        else if(img == 1)
        {
            sprintf(append, "1i,");
        }
        else
        {
            sprintf(append, "-1i,");
        }
    }
}

void freeMatrix(void** MT, const char* type,int MATRIX_SIZE)
{
    if(MT == NULL)
    {
        return;
    }
    if(strcmp(type,BIN)==0)
    {
        for(int i = 0; i<MATRIX_SIZE*MATRIX_SIZE; i++)
        {
            if(MT[i] != NULL)
            {
                free(MT[i]);
                MT[i] = NULL;
            }
        }
    }
    else
    {
        for(int i = 0; i<MATRIX_SIZE; i++)
        {
            if(MT[i] != NULL)
            {
                free(MT[i]);
                MT[i] = NULL;
            }
        }
    }
    free(MT);
    MT = NULL;
}

int checkBIN(const char* BIN_MATRIX)
{
    if(strcmp(BIN_MATRIX,"\0")==0)
    {
        return 1;
    }
    char* data = strchr(BIN_MATRIX,':') + 1;
    while(*data != '\0')
    {
        if(*data != '0' && *data != '1' && *data != ',' && *data != ')')
        {
            return 0;
        }
        data++;
    }
    return 1;
}

void check_type(const char matrix[],char* type) {
    if(strcmp(type,COM) == 0 || strcmp(type,DUB) == 0)
    {
        return;
    }
    if (strchr(matrix, 'i') != NULL) {
        strcpy(type, COM);
        return;
    } else if (strchr(matrix, '.') != NULL) {
        strcpy(type, DUB);
        return;
    } else {
        strcpy(type, INT);
    }
}

void parse_complex_number(const char* data, int* real_part, int* imag_part) {
    *real_part = 0;
    *imag_part = 0;
    char sign = '#';
    int mach = sscanf(data, "%d%c%di", real_part, &sign, imag_part);
    if (mach == 3) {
        if (sign == '-') {
            *imag_part = -1 * (*imag_part);
        }
    } else if (mach == 2) {
        if (sign == 'i') {
            *imag_part = *real_part;
            *real_part = 0;
        } else if (*real_part != 0 && sign != '#' && sign != 'i') {
            if (sign == '-') {
                *imag_part = -1;
            } else if (sign == '+') {
                *imag_part = 1;
            }
        }
    } else if (mach == 0) {
        if (strchr(data, 'i') != NULL) {
            *imag_part = 1;
        }
        if (strchr(data, '-') != NULL) {
            *imag_part = -1 * (*imag_part);
        }
    }
}


void** cast(const char matrix[], int* MATRIX_SIZE, char type[]) {
    // Extract the matrix size
    if(strcmp(matrix,"\0") == 0)
    {
        return NULL;
    }

    int rows;
    sscanf(matrix, "(%d", &rows);
    *MATRIX_SIZE = rows;

    // Check the matrix type based on the input string
    check_type(matrix,type);

    // Create a pointer to hold the matrix data
    void *matrix_data = NULL;

    // Parse and allocate memory for the matrix data based on the type
    if (strcmp(type, INT) == 0) {
        matrix_data = malloc(*MATRIX_SIZE * sizeof(int*));
        if(matrix_data == NULL)
        {
            perror("MALLOC");
            exit(1);
        }
        for (int i = 0; i < *MATRIX_SIZE; i++) {
            ((int**)matrix_data)[i] = (int*)malloc(*MATRIX_SIZE * sizeof(int));
        }
        // Extract integer data
        char *data = strchr(matrix, ':') + 1;
        for (int i = 0; i < *MATRIX_SIZE; i++) {
            for (int j = 0; j < *MATRIX_SIZE; j++) {
                ((int**)matrix_data)[i][j] = (int)strtol(data, &data, 10);
                if (*data == ',') data++;
            }
        }
    } else if (strcmp(type, DUB) == 0) {
        matrix_data = malloc(*MATRIX_SIZE * sizeof(double*));
        if(matrix_data == NULL)
        {
            perror("MALLOC");
            exit(1);
        }
        for (int i = 0; i < *MATRIX_SIZE; i++) {
            ((double**)matrix_data)[i] = (double*)malloc(*MATRIX_SIZE * sizeof(double));
        }
        // Extract double data
        char *data = strchr(matrix, ':') + 1;
        for (int i = 0; i < *MATRIX_SIZE; i++) {
            for (int j = 0; j < *MATRIX_SIZE; j++) {
                ((double**)matrix_data)[i][j] = strtod(data, &data);
                if (*data == ',') data++;
            }
        }
    } else if (strcmp(type, COM) == 0) {
        matrix_data = malloc(*MATRIX_SIZE * sizeof(int complex*));
        if(matrix_data == NULL)
        {
            perror("MALLOC");
            exit(1);
        }
        for (int i = 0; i < *MATRIX_SIZE; i++) {
            ((int complex**)matrix_data)[i] = (int complex*)malloc(*MATRIX_SIZE * sizeof(int complex));
        }
        // Extract complex data
        char *data = strchr(matrix, ':') + 1;
        char *end_ptr = strchr(data, ')');
        *end_ptr = '\0';  // Temporarily null-terminate the string for parsing

        for (int i = 0; i < *MATRIX_SIZE; i++) {
            for (int j = 0; j < *MATRIX_SIZE; j++) {
                int real_part = 0, imag_part = 0;
                parse_complex_number(data, &real_part, &imag_part);
                ((int complex**)matrix_data)[i][j] = real_part + imag_part * I;
                // Move the data pointer to the next complex number
                data = strchr(data, ',');
                if (data != NULL) {
                    data++;  // Skip the comma
                } else {
                    break;
                }
            }
        }
    }
    else {
        return NULL;
    }

    return matrix_data;
}

double multiply_and_round(double a, double b) {
    double result = a * b;
    int temp = (int)(result * 10 + (result < 0 ? -0.5 : 0.5));
    result = temp / 10.0;
    return result;
}
