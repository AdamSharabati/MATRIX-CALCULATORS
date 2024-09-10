#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>

//    ----------------------------    define   --------------------------------------

#define OR "OR"
#define AND "AND"
#define NOT "NOT"
#define TRA "TRANSPOSE"
#define MUL "MUL"
#define SUB "SUB"
#define ADD "ADD"
#define END "END"
#define INT "INT"
#define DUB "DUB"
#define COM "COM"
#define BIN "BIN"
#define BBB "BBB"
#define MAX_LINE  129

//    ----------------------------    define   --------------------------------------


//    ----------------------------    GLOBAL   --------------------------------------

char MATRIX1[MAX_LINE] = "\0";
char MATRIX2[MAX_LINE] = "\0";
char OP[MAX_LINE] = "\0";
char RESULT[MAX_LINE] = "\0";
int MATRIX_SIZE;
char type[4];       ///   ["INT": int, "DUB": double, "COM": complex, "BIN": binary, "bbb": binding]

//    ----------------------------    GLOBAL   --------------------------------------


//    ----------------------------    FUNCTION DIF   --------------------------------------

int initMatrix();
void run();
void** cast(char matrix[]);
void check_type(char matrix[]);
int checkBIN(char* BIN_MATRIX);
void freeMatrix(void** MT);
void parse_complex_number(const char* data, int* real_part, int* imag_part);
void complexToString(char* append, int rel, int img);
void transpose(void*** matrix);
void appendMatrixToString(void** matrix);
double multiply_and_round(double a, double b);

//    ----------------------------    FUNCTION DIF   --------------------------------------


//    ----------------------------    MAIN   --------------------------------------

int main() {

    while(1){

        int checkEND = initMatrix();
        if(checkEND == 0)
        {
            break;
        }
        run();
    }

    return 0;
}

//    ----------------------------    MAIN   --------------------------------------


//    ----------------------------    FUNCTIONS   --------------------------------------

int initMatrix()
{
    strcpy(MATRIX1,"\0");
    strcpy(MATRIX2,"\0");
    fgets(MATRIX1,MAX_LINE,stdin);
    MATRIX1[strlen(MATRIX1) - 1] = '\0';
    if(strcmp(MATRIX1,END) == 0)
    {
        return 0;
    }
    fgets(MATRIX2,MAX_LINE,stdin);
    MATRIX2[strlen(MATRIX2) - 1] = '\0';
    if(MATRIX2[0] != '(')
    {
        strcpy(OP,MATRIX2);
        strcpy(MATRIX2,"\0");
        return 1;
    }
    fgets(OP,MAX_LINE,stdin);
    OP[strlen(OP) - 1] = '\0';
    return 1;
}

void run()
{
    strcpy(type,BBB);
    MATRIX_SIZE = 0;
    strcpy(RESULT,"\0");


    if(strcmp(OP, OR) == 0 || strcmp(OP, AND) == 0 || strcmp(OP, NOT) == 0){

        if(!checkBIN(MATRIX1) || !checkBIN(MATRIX2))
        {
            printf("ERR\n");
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
            printf("%s\n",RESULT);
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
            printf("%s\n",RESULT);
            return;
        }
        else if(strcmp(OP,NOT)==0)
        {
            strcpy(RESULT,"\0");
            char rc;
            int i = 0;
            while(MATRIX1[i] != '\0')
            {
                rc = 'c';
                if(MATRIX1[i] == '1')
                {
                    rc = '0';
                }
                else if(MATRIX1[i] == '0')
                {
                    rc = '1';
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
            printf("%s\n",RESULT);
            return;
        }
    }


    void** MT1 = cast(MATRIX1);
    void** MT2 = cast(MATRIX2);

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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
                        sum += multiply_and_round(mtx1[i][k],mtx2[k][j]);                    //mtx1[i][k] * mtx2[k][j];
                    }
                    char append[20]="\0";
                    sprintf(append,"%.1f,",sum);
                    strcat(RESULT,append);
                }
            }
            RESULT[strlen(RESULT)-1] = '\0';
            strcat(RESULT,")");
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
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
            printf("%s\n",RESULT);
            freeMatrix(MT1);
            freeMatrix(MT2);
            return;
        }
    }

    else if(strcmp(OP,TRA)==0)
    {
        transpose(&MT1);
        appendMatrixToString(MT1);
        printf("%s\n",RESULT);
        freeMatrix(MT1);
    }
}



void transpose(void*** matrix) {
    if (strcmp(type, INT) == 0) {
        int** int_matrix = (int**)(*matrix);
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = i + 1; j < MATRIX_SIZE; ++j) {
                int temp = int_matrix[i][j];
                int_matrix[i][j] = int_matrix[j][i];
                int_matrix[j][i] = temp;
            }
        }
    } else if (strcmp(type, DUB) == 0) {
        double** double_matrix = (double**)(*matrix);
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = i + 1; j < MATRIX_SIZE; ++j) {
                double temp = double_matrix[i][j];
                double_matrix[i][j] = double_matrix[j][i];
                double_matrix[j][i] = temp;
            }
        }
    } else if (strcmp(type, COM) == 0) {
        int complex** complex_matrix = (int complex**)(*matrix);
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = i + 1; j < MATRIX_SIZE; ++j) {
                int complex temp = complex_matrix[i][j];
                complex_matrix[i][j] = complex_matrix[j][i];
                complex_matrix[j][i] = temp;
            }
        }
    } else {
        // Handle unknown type
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


void  appendMatrixToString(void** matrix) {
    char append[50];

    if (strcmp(type, INT) == 0) {
        int** intMatrix = (int**)matrix;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                sprintf(append, "%d,", intMatrix[i][j]);
                strcat(RESULT, append);
            }
        }
    } else if (strcmp(type, DUB) == 0) {
        double** doubleMatrix = (double**)matrix;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                sprintf(append, "%.1f,", doubleMatrix[i][j]);
                strcat(RESULT, append);
            }
        }
    } else if (strcmp(type, COM) == 0) {
        int complex** complexMatrix = (int complex**)matrix;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                int realPart = (int)creal(complexMatrix[i][j]);
                int imagPart = (int)cimag(complexMatrix[i][j]);
                complexToString(append, realPart, imagPart);
                strcat(RESULT, append);
            }
        }
    }

    // Remove the last comma
    if (strlen(RESULT) > 0) {
        RESULT[strlen(RESULT) - 1] = '\0';
    }
    strcat(RESULT, ")");
}

void freeMatrix(void** MT)
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
            }
        }
    }
    free(MT);
}

int checkBIN(char* BIN_MATRIX)
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

void check_type(char matrix[]) {
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


void** cast(char matrix[]) {
    // Extract the matrix size
    if(strcmp(matrix,"\0") == 0)
    {
        return NULL;
    }

    int rows;
    sscanf(matrix, "(%d", &rows);
    MATRIX_SIZE = rows;

    // Check the matrix type based on the input string
    check_type(matrix);

    // Create a pointer to hold the matrix data
    void *matrix_data = NULL;

    // Parse and allocate memory for the matrix data based on the type
    if (strcmp(type, INT) == 0) {
        matrix_data = malloc(MATRIX_SIZE * sizeof(int*));
        if(matrix_data == NULL)
        {
            perror("MALLOC");
            exit(1);
        }
        for (int i = 0; i < MATRIX_SIZE; i++) {
            ((int**)matrix_data)[i] = (int*)malloc(MATRIX_SIZE * sizeof(int));
        }
        // Extract integer data
        char *data = strchr(matrix, ':') + 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                ((int**)matrix_data)[i][j] = (int)strtol(data, &data, 10);
                if (*data == ',') data++;
            }
        }
    } else if (strcmp(type, DUB) == 0) {
        matrix_data = malloc(MATRIX_SIZE * sizeof(double*));
        if(matrix_data == NULL)
        {
            perror("MALLOC");
            exit(1);
        }
        for (int i = 0; i < MATRIX_SIZE; i++) {
            ((double**)matrix_data)[i] = (double*)malloc(MATRIX_SIZE * sizeof(double));
        }
        // Extract double data
        char *data = strchr(matrix, ':') + 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                ((double**)matrix_data)[i][j] = strtod(data, &data);
                if (*data == ',') data++;
            }
        }
    } else if (strcmp(type, COM) == 0) {
        matrix_data = malloc(MATRIX_SIZE * sizeof(int complex*));
        if(matrix_data == NULL)
        {
            perror("MALLOC");
            exit(1);
        }
        for (int i = 0; i < MATRIX_SIZE; i++) {
            ((int complex**)matrix_data)[i] = (int complex*)malloc(MATRIX_SIZE * sizeof(int complex));
        }
        // Extract complex data
        char *data = strchr(matrix, ':') + 1;
        char *end_ptr = strchr(data, ')');
        *end_ptr = '\0';  // Temporarily null-terminate the string for parsing

        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
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

//    ----------------------------    FUNCTIONS   --------------------------------------
