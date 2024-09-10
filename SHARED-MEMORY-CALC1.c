//324112028
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


#define END "END"
#define MAX_LINE  129
#define MAX_STRUCTS 17
#define SHMKEY 1234     //////     shared memory key

// Structure to hold your shared data
typedef struct {
    char matrix1[MAX_LINE];
    char matrix2[MAX_LINE];
    char op[MAX_LINE];
    int ready;  // Flag to signal readiness
}SharedData ;

SharedData *shared_data = NULL;
int I;

void initStruct();
int addOP();

int main() {
    int shmid;
    key_t key;

    // Generate a key for the shared memory segment
    key = (key_t)SHMKEY;
    size_t shm_size = (size_t)(sizeof(SharedData) * MAX_STRUCTS);

    // Create the shared memory segment
    if ((shmid = shmget(key, shm_size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    if ((shared_data = (SharedData *)shmat(shmid, NULL, 0)) == (SharedData *) -1) {
        perror("shmat");
        exit(1);
    }

    initStruct();
    I = 0;
    // Main loop to read input and write to shared memory
    while (1) {
        // Read matrix1, matrix2, and operation
        int checkEND = addOP();
        if (checkEND == 0) {
            shared_data[16].ready = -2;
            break;
        }
        I++;
    }

    shmdt((void *)shared_data);
    return 0;
}


void initStruct()
{
    for(int i = 0; i<MAX_STRUCTS;i++)
    {
        strcpy(shared_data[i].matrix1,"\0");
        strcpy(shared_data[i].matrix2,"\0");
        strcpy(shared_data[i].op,"\0");
        shared_data[i].ready = 0;
    }
}

int addOP() {
    strcpy((shared_data+I)->matrix1, "\0");
    strcpy((shared_data+I)->matrix2, "\0");

    fgets((shared_data+I)->matrix1, MAX_LINE, stdin);
    (shared_data+I)->matrix1[strlen((shared_data+I)->matrix1) - 1] = '\0';

    if (strcmp((shared_data+I)->matrix1, END) == 0) {
        return 0;
    }

    fgets((shared_data+I)->matrix2, MAX_LINE, stdin);
    (shared_data+I)->matrix2[strlen((shared_data+I)->matrix2) - 1] = '\0';

    if ((shared_data+I)->matrix2[0] != '(') {
        strcpy((shared_data+I)->op, (shared_data+I)->matrix2);
        strcpy((shared_data+I)->matrix2, "\0");
    } else {
        fgets((shared_data+I)->op, MAX_LINE, stdin);
        (shared_data+I)->op[strlen((shared_data+I)->op) - 1] = '\0';
    }

    // Set the ready flag to 1 to signal the consumer process
    (shared_data+I)->ready = 1;

    return 1;
}
