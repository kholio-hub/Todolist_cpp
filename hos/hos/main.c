#include "mid/clinic.h"
#include "high/app.h"

int main() {
    initClinics();  /* set up clinics and internal queues */
    int choice;

    while(1) {
        printf("\n===== Emergency Hospital =====\n");
        printf("1. Reception (Add Patient)\n");
        printf("2. Doctor Screen\n");
        printf("3. Patient Screen\n");
        printf("4. Display Queues\n");
        printf("5. Patient History\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        if(scanf("%d", &choice)!=1) { while(getchar()!='\n'); printf("Invalid input\n"); continue; }

        switch(choice) {
            case 1: addPatient(); break;
            case 2: doctorScreen(); break;
            case 3: patientScreen(); break;
            case 4: listAllPatients(); break;
            case 5: displayHistory(); break;
            case 7: printf("Exiting system...bye bye\n"); exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
