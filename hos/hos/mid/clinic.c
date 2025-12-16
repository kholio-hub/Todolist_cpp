#include "clinic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* -------------------------------------------------------
 * Global Clinics Database
 * -------------------------------------------------------
 * clinics     : Array holding all hospital clinics
 * historyHead : Head pointer for patient treatment history
 * -------------------------------------------------------
 */
Clinic clinics[MAX_CLINICS];
HistoryNode* historyHead = NULL;


/*
 * Function: initClinics
 * -------------------------------------------------------
 * Description:
 *  - Initializes all clinics in the hospital system.
 *  - Assigns clinic names.
 *  - Initializes three priority queues per clinic:
 *      1) Critical
 *      2) Urgent
 *      3) Normal
 *
 * Input:
 *  - None
 *
 * Output:
 *  - None
 * -------------------------------------------------------
 */
void initClinics(void) {

    /* Predefined clinic names */
    char clinicNames[MAX_CLINICS][MAX_NAME] = {
        "Cardiology",
        "Dermatology",
        "Neurology",
        "Pediatrics",
        "Orthopedics"
    };

    /* Initialize each clinic */
    for(int i = 0; i < MAX_CLINICS; i++) {

        /* Copy clinic name safely */
        strncpy(clinics[i].name, clinicNames[i], MAX_NAME - 1);
        clinics[i].name[MAX_NAME - 1] = '\0';

        /* Initialize priority queues */
        initQueue(&clinics[i].criticalQ, QUEUE_CAPACITY);
        initQueue(&clinics[i].urgentQ,   QUEUE_CAPACITY);
        initQueue(&clinics[i].normalQ,   QUEUE_CAPACITY);
    }
}

/*
 * Function: enqueue_to_clinic
 * -------------------------------------------------------
 * Description:
 *  - Adds a patient to the appropriate priority queue
 *    inside the selected clinic.
 *
 * Priority Rules:
 *  - 1 → Critical Queue
 *  - 2 → Urgent Queue
 *  - 3 → Normal Queue
 *
 * Input:
 *  - clinicIdx : Index of clinic (0 → MAX_CLINICS-1)
 *  - p         : Patient data
 *
 * Output:
 *  - Returns 0  → Patient added successfully
 *  - Returns 1  → Queue is full
 *  - Returns -1 → Invalid clinic index
 * -------------------------------------------------------
 */
int enqueue_to_clinic(int clinicIdx, Patient p) {

    /* Validate clinic index */
    if(clinicIdx < 0 || clinicIdx >= MAX_CLINICS)
        return -1;

    /* Route patient based on priority */
    if(p.priority == 1) {
        if(isFull(&clinics[clinicIdx].criticalQ)) return 1;
        enqueuePatient(&clinics[clinicIdx].criticalQ, p);
        return 0;
    }

    if(p.priority == 2) {
        if(isFull(&clinics[clinicIdx].urgentQ)) return 1;
        enqueuePatient(&clinics[clinicIdx].urgentQ, p);
        return 0;
    }

    /* Normal priority */
    if(isFull(&clinics[clinicIdx].normalQ)) return 1;
    enqueuePatient(&clinics[clinicIdx].normalQ, p);
    return 0;
}
/*
 * Function: call_next_from_clinic
 * -------------------------------------------------------
 * Description:
 *  - Calls the next patient based on priority order:
 *      1) Critical
 *      2) Urgent
 *      3) Normal
 *
 * Input:
 *  - clinicIdx : Index of clinic
 *  - out       : Pointer to store the dequeued patient
 *
 * Output:
 *  - Returns 0  → Patient successfully retrieved
 *  - Returns 1  → All queues are empty
 *  - Returns -1 → Invalid clinic index
 * -------------------------------------------------------
 */
int call_next_from_clinic(int clinicIdx, Patient *out) {

    /* Validate clinic index */
    if(clinicIdx < 0 || clinicIdx >= MAX_CLINICS)
        return -1;

    Clinic *c = &clinics[clinicIdx];
    Patient p;

    /* Priority-based dequeue */
    if(!isEmpty(&c->criticalQ)) {
        p = dequeuePatient(&c->criticalQ);
        *out = p;
        return 0;
    }

    if(!isEmpty(&c->urgentQ)) {
        p = dequeuePatient(&c->urgentQ);
        *out = p;
        return 0;
    }

    if(!isEmpty(&c->normalQ)) {
        p = dequeuePatient(&c->normalQ);
        *out = p;
        return 0;
    }

    /* No patients available */
    return 1;
}

/*
 * Function: listAllPatients
 * -------------------------------------------------------
 * Description:
 *  - Displays all waiting patients in every clinic.
 *  - Patients are shown per clinic and per priority.
 *
 * Input:
 *  - None
 *
 * Output:
 *  - Console output only
 * -------------------------------------------------------
 */
void listAllPatients(void) {

    for(int i = 0; i < MAX_CLINICS; i++) {

        Clinic *c = &clinics[i];
        printf("\n--- %s Clinic ---\n", c->name);

        /* Critical Queue */
        printf("Critical:\n");
        for(int j = 0; j < c->criticalQ.size; j++) {
            int idx = (c->criticalQ.front + j) % c->criticalQ.capacity;
            printf("%d - %s\n",
                   c->criticalQ.arr[idx].id,
                   c->criticalQ.arr[idx].name);
        }

        /* Urgent Queue */
        printf("Urgent:\n");
        for(int j = 0; j < c->urgentQ.size; j++) {
            int idx = (c->urgentQ.front + j) % c->urgentQ.capacity;
            printf("%d - %s\n",
                   c->urgentQ.arr[idx].id,
                   c->urgentQ.arr[idx].name);
        }

        /* Normal Queue */
        printf("Normal:\n");
        for(int j = 0; j < c->normalQ.size; j++) {
            int idx = (c->normalQ.front + j) % c->normalQ.capacity;
            printf("%d - %s\n",
                   c->normalQ.arr[idx].id,
                   c->normalQ.arr[idx].name);
        }
    }
}

/*
 * Function: addToHistory
 * -------------------------------------------------------
 * Description:
 *  - Stores treated patient information in a linked list.
 *  - Used for medical history and reporting.
 *
 * Input:
 *  - p          : Treated patient data
 *  - clinicName : Name of the clinic where patient was treated
 *
 * Output:
 *  - None
 * -------------------------------------------------------
 */
void addToHistory(Patient p, const char* clinicName) {

    /* Allocate new history node */
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if(!newNode) return;

    newNode->data = p;

    /* Copy clinic name safely */
    strncpy(newNode->clinicName, clinicName, MAX_NAME - 1);
    newNode->clinicName[MAX_NAME - 1] = '\0';

    newNode->next = NULL;

    /* Insert node at end of history list */
    if(historyHead == NULL) {
        historyHead = newNode;
    } else {
        HistoryNode* temp = historyHead;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
}
/*
 * Function: displayHistory
 * -------------------------------------------------------
 * Description:
 *  - Displays all treated patients stored in history list.
 *
 * Input:
 *  - None
 *
 * Output:
 *  - Console output only
 * -------------------------------------------------------
 */
void displayHistory(void) {

    if(historyHead == NULL) {
        printf("No patient history.\n");
        return;
    }

    HistoryNode* temp = historyHead;
    printf("\n--- Patient History ---\n");

    while(temp != NULL) {
        printf("ID: %d, Name: %s, Age: %d, Clinic: %s\n",
               temp->data.id,
               temp->data.name,
               temp->data.age,
               temp->clinicName);
        temp = temp->next;
    }
}

/*
 * Function: findPatientByID
 * -------------------------------------------------------
 * Description:
 *  - Searches for a patient in all clinics and queues.
 *  - Returns clinic name, queue name, and waiting position.
 *
 * Input:
 *  - patientID     : Unique patient ID
 *  - outClinicName : Output buffer for clinic name
 *  - outQueueName  : Output buffer for queue name
 *  - position      : Output position in queue (1-based)
 *
 * Output:
 *  - Returns 1 → Patient found
 *  - Returns 0 → Patient not found
 * -------------------------------------------------------
 */
int findPatientByID(int patientID,
                    char *outClinicName,
                    char *outQueueName,
                    int *position) {

    /* Iterate over all clinics */
    for(int i = 0; i < MAX_CLINICS; i++) {

        Clinic *c = &clinics[i];

        /* Search Critical Queue */
        for(int j = 0; j < c->criticalQ.size; j++) {
            int idx = (c->criticalQ.front + j) % c->criticalQ.capacity;
            if(c->criticalQ.arr[idx].id == patientID) {

                if(outClinicName) strncpy(outClinicName, c->name, MAX_NAME);
                if(outQueueName)  strncpy(outQueueName, "Critical", MAX_NAME);
                if(position)      *position = j + 1;

                return 1;
            }
        }

        /* Search Urgent Queue */
        for(int j = 0; j < c->urgentQ.size; j++) {
            int idx = (c->urgentQ.front + j) % c->urgentQ.capacity;
            if(c->urgentQ.arr[idx].id == patientID) {

                if(outClinicName) strncpy(outClinicName, c->name, MAX_NAME);
                if(outQueueName)  strncpy(outQueueName, "Urgent", MAX_NAME);
                if(position)      *position = j + 1;

                return 1;
            }
        }

        /* Search Normal Queue */
        for(int j = 0; j < c->normalQ.size; j++) {
            int idx = (c->normalQ.front + j) % c->normalQ.capacity;
            if(c->normalQ.arr[idx].id == patientID) {

                if(outClinicName) strncpy(outClinicName, c->name, MAX_NAME);
                if(outQueueName)  strncpy(outQueueName, "Normal", MAX_NAME);
                if(position)      *position = j + 1;

                return 1;
            }
        }
    }

    /* Patient not found */
    return 0;
}






