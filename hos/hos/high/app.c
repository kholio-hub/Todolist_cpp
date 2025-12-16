#include "app.h"
#include <stdio.h>
#include <string.h>

/* Global variable used to generate unique patient IDs.
 * Starts from 1001 and increments automatically for each new patient.
 */
int next_patient_id = 1001;

/* -------------------------------------------------------
 * Function: addPatient
 * -------------------------------------------------------
 * Description:
 *  - Registers a new patient in the system.
 *  - Assigns a unique ID.
 *  - Takes patient personal data.
 *  - Inserts the patient into the selected clinic
 *    based on priority (Critical / Urgent / Normal).
 *
 * Input: None (user input via console)
 * Output: None
 * -------------------------------------------------------
 */
void addPatient() {
    Patient p;              // Patient structure to store data
    int clinicChoice;       // Selected clinic index

    /* Assign unique patient ID */
    p.id = next_patient_id++;

    printf("\n--- Patient ID Assigned: %d ---\n", p.id);

    /* Read patient name (max 49 characters) */
    printf("Enter Name (Max 49 chars): ");
    if(scanf("%49s", p.name) != 1) {
        while(getchar() != '\n');   // Clear input buffer
        printf("Invalid Name input or format error. Patient not added.\n");
        return;
    }

    /* Read and validate patient age */
    printf("Enter Age: ");
    if(scanf("%d", &p.age) != 1 || p.age <= 0 || p.age > 120) {
        while(getchar() != '\n');
        printf("Invalid Age input. Patient not added.\n");
        return;
    }

    /* Display available clinics */
    printf("Choose Clinic:\n");
    for(int i=0; i < MAX_CLINICS; i++) {
        printf("%d. %s\n", i + 1, clinics[i].name);
    }

    /* Read clinic choice */
    printf("Enter choice: ");
    if(scanf("%d", &clinicChoice) != 1) {
        while(getchar() != '\n');
        printf("Invalid Clinic input. Patient not added.\n");
        return;
    }

    /* Convert from 1-based to 0-based index */
    clinicChoice--;

    /* Validate clinic index */
    if(clinicChoice < 0 || clinicChoice >= MAX_CLINICS) {
        printf("Invalid clinic choice (must be between 1 and %d). Patient not added.\n", MAX_CLINICS);
        return;
    }

    /* Read patient priority */
    printf("Enter Priority (1=Critical, 2=Urgent, 3=Normal): ");
    if(scanf("%d", &p.priority) != 1 || p.priority < 1 || p.priority > 3) {
        while(getchar() != '\n');
        printf("Invalid Priority input (must be 1, 2, or 3). Patient not added.\n");
        return;
    }

    /* Get selected clinic */
    Clinic *c = &clinics[clinicChoice];
    int added = 0;

    /* Insert patient into the correct priority queue */
    if(p.priority == 1) {
        if(!isFull(&c->criticalQ)) {
            enqueuePatient(&c->criticalQ, p);
            added = 1;
        }
    }
    else if(p.priority == 2) {
        if(!isFull(&c->urgentQ)) {
            enqueuePatient(&c->urgentQ, p);
            added = 1;
        }
    }
    else {  // Normal priority
        if(!isFull(&c->normalQ)) {
            enqueuePatient(&c->normalQ, p);
            added = 1;
        }
    }

    /* Display result */
    if(added) {
        printf("Patient %d (%s) Added to %s Clinic! Priority: %d\n",
               p.id, p.name, c->name, p.priority);
    } else {
        printf("Patient %d (%s) NOT Added. The selected priority queue is full!\n",
               p.id, p.name);
    }
}
/*
 * Function: doctorScreen
 * -------------------------------------------------------
 * Description:
 *  - Provides an interactive interface for doctors.
 *  - Allows calling, skipping, and viewing patients
 *    in a selected clinic.
 *
 * Input: None (user-driven)
 * Output: None
 * -------------------------------------------------------
 */
void doctorScreen() {
    int clinicChoice;

    printf("\n--- Doctor Screen ---\n");
    printf("Select Clinic:\n");

    /* Display clinic list */
    for(int i=0;i<MAX_CLINICS;i++)
        printf("%d. %s\n", i+1, clinics[i].name);

    /* Read clinic selection */
    if(scanf("%d", &clinicChoice)!=1) {
        while(getchar()!='\n');
        printf("Invalid input\n");
        return;
    }

    clinicChoice--;

    /* Validate clinic index */
    if(clinicChoice < 0 || clinicChoice >= MAX_CLINICS) {
        printf("Invalid clinic choice\n");
        return;
    }

    Clinic *c = &clinics[clinicChoice];
    int choice;

    /* Doctor menu loop */
    while(1) {
        printf("\n--- %s Clinic ---\n", c->name);
        printf("1. Call Next Patient\n");
        printf("2. Skip Patient\n");
        printf("3. Show Queue Status\n");
        printf("4. Return to Main Menu\n");
        printf("Enter choice: ");

        if(scanf("%d", &choice)!=1) {
            while(getchar()!='\n');
            printf("Invalid input\n");
            continue;
        }

        /* Call next patient */
        if(choice == 1) {
            Patient p;
            int r = call_next_from_clinic(clinicChoice, &p);

            if(r == 0) {
                printf("Calling Patient: %d - %s\n", p.id, p.name);
                addToHistory(p, c->name);
            } else {
                printf("No patients waiting!\n");
            }
        }

        /* Skip patient (move to end of same queue) */
        else if(choice == 2) {
            Patient p;
            int r = call_next_from_clinic(clinicChoice, &p);

            if(r == 0 && p.id != -1) {
                enqueue_to_clinic(clinicChoice, p);
                printf("Patient skipped: %d - %s\n", p.id, p.name);
            } else {
                printf("No patients to skip!\n");
            }
        }

        /* Display queue sizes */
        else if(choice == 3) {
            printf("Critical: %d, Urgent: %d, Normal: %d\n",
                   c->criticalQ.size,
                   c->urgentQ.size,
                   c->normalQ.size);
        }

        /* Exit doctor screen */
        else if(choice == 4) {
            break;
        }

        else {
            printf("Invalid choice!\n");
        }
    }
}

/*
 * Function: patientScreen
 * -------------------------------------------------------
 * Description:
 *  - Allows patients to check their clinic, queue type,
 *    and position using their patient ID.
 *
 * Input: Patient ID
 * Output: Patient status information
 * -------------------------------------------------------
 */
void patientScreen() {
    int patientID;

    printf("\n--- Patient Screen ---\n");
    printf("Enter your Patient ID: ");

    if(scanf("%d", &patientID)!=1) {
        while(getchar()!='\n');
        printf("Invalid input\n");
        return;
    }

    char clinicName[MAX_NAME];
    char queueName[MAX_NAME];
    int pos;

    /* Search patient in all clinics and queues */
    if(findPatientByID(patientID, clinicName, queueName, &pos)) {
        printf("Patient Found in %s Clinic - %s Queue, Position: %d\n",
               clinicName, queueName, pos);
    } else {
        printf("Patient ID not found or already treated.\n");
    }
}
