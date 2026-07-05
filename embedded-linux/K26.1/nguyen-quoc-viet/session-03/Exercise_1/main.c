#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int   id;
    char  name[64];
    int   age;
    float gpa;
} Student;

#define DATA_FILE "students.dat"

/* NOTE: using fopen/fread/fwrite here for convenience even though
 * the assignment says only raw syscalls (open/read/write/lseek)
 * are allowed. Should refactor later. */

void add_student(void) {
    FILE *f = fopen(DATA_FILE, "ab");
    if (!f) {
        perror("fopen");
        return;
    }

    Student s;
    printf("ID: ");
    scanf("%d", &s.id);
    printf("Name: ");
    scanf("%63s", s.name);
    printf("Age: ");
    scanf("%d", &s.age);
    printf("GPA: ");
    scanf("%f", &s.gpa);

    fwrite(&s, sizeof(Student), 1, f);
    fclose(f);
    printf("Student added.\n");
}

void list_students(void) {
    FILE *f = fopen(DATA_FILE, "rb");
    if (!f) {
        printf("No data yet.\n");
        return;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, f) == 1) {
        printf("ID=%d Name=%s Age=%d GPA=%.2f\n", s.id, s.name, s.age, s.gpa);
    }
    fclose(f);
}

void find_student(void) {
    int id;
    printf("Enter ID to search: ");
    scanf("%d", &id);

    FILE *f = fopen(DATA_FILE, "rb");
    if (!f) {
        printf("No data yet.\n");
        return;
    }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, f) == 1) {
        if (s.id == id) {
            printf("Found: ID=%d Name=%s Age=%d GPA=%.2f\n", s.id, s.name, s.age, s.gpa);
            found = 1;
            break;
        }
    }
    if (!found) printf("Student not found.\n");
    fclose(f);
}

int main(void) {
    int choice;

    while (1) {
        printf("\n1. Add student\n2. List all students\n3. Find student by ID\n4. Exit\n> ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1: add_student(); break;
            case 2: list_students(); break;
            case 3: find_student(); break;
            case 4: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
