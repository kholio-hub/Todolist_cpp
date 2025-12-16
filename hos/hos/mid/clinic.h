#ifndef CLINIC_H
#define CLINIC_H

#include "../low/queue.h"

#define MAX_CLINICS 5
#define QUEUE_CAPACITY 50

typedef struct {
    char name[MAX_NAME];
    Queue criticalQ;
    Queue urgentQ;
    Queue normalQ;
} Clinic;

/* The array of clinics (defined in clinic.c) */
extern Clinic clinics[MAX_CLINICS];

/* History node (singly-linked) */
typedef struct HistoryNode {
    Patient data;
    char clinicName[MAX_NAME];
    struct HistoryNode* next;
} HistoryNode;

/* Clinic-level API */
void initClinics(void);
int enqueue_to_clinic(int clinicIdx, Patient p);
int call_next_from_clinic(int clinicIdx, Patient *out);
void listAllPatients(void);
void displayHistory(void);
int findPatientByID(int patientID, char *outClinicName, char *outQueueName, int *position);
void addToHistory(Patient p, const char* clinicName);

#endif /* CLINIC_H */
