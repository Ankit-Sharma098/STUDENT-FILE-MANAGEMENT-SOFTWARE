#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

// Student structure
typedef struct dataManagement
{
    char name[100];
    char course[20];
    int roll;
    float fee;
} student;

// Create a student record
void createRecord(char fname[])
{
    FILE *fp;
    //char fname[100];
    student s;
    char choice;

   // printf("Enter data file name:\n");
   // scanf("%s", fname);

    printf("File name: %s\n", fname);

    do
    {
        printf("\nEnter Course:\n");
        fflush(stdin);
        fgets(s.course, sizeof(s.course), stdin);
        s.course[strcspn(s.course, "\n")] = '\0';

        // Determine next roll number for this course
        int maxRoll = 0;
        fp = fopen(fname, "r");
        if (fp != NULL)
        {
            char line[200];
            char *name, *course, *rollStr, *feeStr;

            while (fgets(line, sizeof(line), fp))
            {
                char lineCopy[200];
                strcpy(lineCopy, line);

                name = strtok(lineCopy, "|");
                course = strtok(NULL, "|");
                rollStr = strtok(NULL, "|");
                feeStr = strtok(NULL, "\n");

                if (strcmp(course, s.course) == 0)
                {
                    int r = atoi(rollStr);
                    if (r > maxRoll)
                        maxRoll = r;
                }
            }
            fclose(fp);
        }

        s.roll = maxRoll + 1;
        printf("Auto-generated Roll Number for course '%s': %d\n", s.course, s.roll);

        printf("Enter Student Name:\n");
        fflush(stdin);
        fgets(s.name, sizeof(s.name), stdin);
        s.name[strcspn(s.name, "\n")] = '\0';

        printf("Enter Fee:\n");
        scanf("%f", &s.fee);

        fp = fopen(fname, "a");
        if (fp == NULL)
        {
            printf("Error creating file! Try again later.\n");
            return;
        }

        fprintf(fp, "%s|%s|%d|%.2f\n", s.name, s.course, s.roll, s.fee);
        fclose(fp);

        printf("Record saved successfully.\n");

        printf("\nDo you want to add another student? (Y/N): ");
        fflush(stdin);
        choice = getchar();

    } while (choice == 'Y' || choice == 'y');
}

// Display all records
void disRecord(char fname[])
{
    FILE *fp;
   // char fName[100];
    char line[200];
    char *name, *course, *rollStr, *feeStr;

   // printf("Enter file name for DISPLAY:\n");
   // scanf("%s", fName);

    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    printf("\n%-20s %-10s %-5s %-10s\n", "NAME", "COURSE", "ROLL", "FEE");
    printf("-------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        name = strtok(line, "|");
        course = strtok(NULL, "|");
        rollStr = strtok(NULL, "|");
        feeStr = strtok(NULL, "\n");

        if (name && course && rollStr && feeStr)
        {
            printf("%-20s %-10s %-5s %-10s\n", name, course, rollStr, feeStr);
        }
    }

    fclose(fp);
}

// Update a record
void updateRecord(char fname[])
{
    FILE *fp, *temp;
    int targetRoll, choice, found = 0;
    char targetCourse[20];
    student s;

    char line[200];
    char *name, *course, *rollStr, *feeStr;

    //printf("Enter File name to update:\n");
    //scanf("%s", fname);

    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        printf("Error creating temp file.\n");
        fclose(fp);
        return;
    }

    printf("Enter Roll Number to update:\n");
    scanf("%d", &targetRoll);

    int matchCount = 0;
    while (fgets(line, sizeof(line), fp))
    {
        char lineCopy[200];
        strcpy(lineCopy, line);

        strtok(lineCopy, "|");
        strtok(NULL, "|");
        rollStr = strtok(NULL, "|");

        if (atoi(rollStr) == targetRoll)
        {
            matchCount++;
        }
    }

    rewind(fp);

    if (matchCount > 1)
    {
        printf("Multiple records found with this roll.\n");
        printf("Enter Course to identify the record:\n");
        fflush(stdin);
        fgets(targetCourse, sizeof(targetCourse), stdin);
        targetCourse[strcspn(targetCourse, "\n")] = '\0';
    }
    else
    {
        strcpy(targetCourse, "");
    }

    while (fgets(line, sizeof(line), fp))
    {
        char lineCopy[200];
        strcpy(lineCopy, line);

        name = strtok(lineCopy, "|");
        course = strtok(NULL, "|");
        rollStr = strtok(NULL, "|");
        feeStr = strtok(NULL, "\n");

        int isTarget = 0;
        if (atoi(rollStr) == targetRoll)
        {
            if (matchCount == 1 || strcmp(course, targetCourse) == 0)
                isTarget = 1;
        }

        if (isTarget)
        {
            found = 1;

            strcpy(s.name, name);
            strcpy(s.course, course);
            s.roll = atoi(rollStr);
            s.fee = atof(feeStr);

            do
            {
                printf("\nWhat do you want to update?\n");
                printf("-----------------------------\n");
                printf("1. Name\n");
                printf("2. Course (will assign new Roll Number)\n");
                printf("3. Roll Number\n");
                printf("4. Fee\n");
                printf("5. Finish Update\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("Enter new Name: ");
                    fflush(stdin);
                    fgets(s.name, sizeof(s.name), stdin);
                    s.name[strcspn(s.name, "\n")] = '\0';
                    break;

                case 2:
                    printf("Enter new Course: ");
                    fflush(stdin);
                    fgets(s.course, sizeof(s.course), stdin);
                    s.course[strcspn(s.course, "\n")] = '\0';

                    {
                        FILE *readFp = fopen(fname, "r");
                        int maxRoll = 0;
                        if (readFp != NULL)
                        {
                            char tempLine[200];
                            char *cName, *cCourse, *cRollStr, *cFeeStr;

                            while (fgets(tempLine, sizeof(tempLine), readFp))
                            {
                                char tempCopy[200];
                                strcpy(tempCopy, tempLine);

                                cName = strtok(tempCopy, "|");
                                cCourse = strtok(NULL, "|");
                                cRollStr = strtok(NULL, "|");
                                cFeeStr = strtok(NULL, "\n");

                                if (strcmp(cCourse, s.course) == 0)
                                {
                                    int r = atoi(cRollStr);
                                    if (r > maxRoll)
                                        maxRoll = r;
                                }
                            }
                            fclose(readFp);
                        }
                        s.roll = maxRoll + 1;
                        printf("New roll number for course '%s' is %d\n", s.course, s.roll);
                    }
                    break;

                case 3:
                    printf("Enter new Roll Number: ");
                    scanf("%d", &s.roll);
                    break;

                case 4:
                    printf("Enter new Fee: ");
                    scanf("%f", &s.fee);
                    break;

                case 5:
                    printf("Finishing update.\n");
                    break;

                default:
                    printf("Invalid choice.\n");
                }
            } while (choice != 5);

            fprintf(temp, "%s|%s|%d|%.2f\n", s.name, s.course, s.roll, s.fee);

            printf("\nUpdated record:\n");
            printf("%-20s %-10s %-5d %-10.2f\n", s.name, s.course, s.roll, s.fee);
        }
        else
        {
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(fname);
        rename("temp.txt", fname);
        printf("Record updated successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("Record not found.\n");
    }
}

// Search a record
void searchRecord(char fname[])
{
    FILE *fp;
   // char fname[100];
    int targetRoll, count = 0, displayed = 0;

    char line[200];
    char *name, *course, *rollStr, *feeStr;

    char matchedLines[10][200];
    char targetCourse[20];

   // printf("Enter file name to search:\n");
   // scanf("%s", fname);

    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    printf("Enter Roll Number to search:\n");
    scanf("%d", &targetRoll);

    while (fgets(line, sizeof(line), fp))
    {
        char lineCopy[200];
        strcpy(lineCopy, line);

        name = strtok(lineCopy, "|");
        course = strtok(NULL, "|");
        rollStr = strtok(NULL, "|");
        feeStr = strtok(NULL, "\n");

        if (atoi(rollStr) == targetRoll)
        {
            strcpy(matchedLines[count], line);
            count++;
        }
    }

    fclose(fp);

    if (count == 0)
    {
        printf("No records found with Roll Number %d.\n", targetRoll);
        return;
    }
    else if (count == 1)
    {
        printf("\nRecord Found:\n");
        printf("---------------------------------\n");
        strcpy(line, matchedLines[0]);
        name = strtok(line, "|");
        course = strtok(NULL, "|");
        rollStr = strtok(NULL, "|");
        feeStr = strtok(NULL, "\n");

        printf("Name   : %s\n", name);
        printf("Course : %s\n", course);
        printf("Roll   : %s\n", rollStr);
        printf("Fee    : %s\n", feeStr);
        printf("---------------------------------\n");
    }
    else
    {
        printf("\nMultiple records found with this roll.\n");
        printf("Enter Course name to identify:\n");
        fflush(stdin);
        fgets(targetCourse, sizeof(targetCourse), stdin);
        targetCourse[strcspn(targetCourse, "\n")] = '\0';

        for (int i = 0; i < count; i++)
        {
            strcpy(line, matchedLines[i]);
            name = strtok(line, "|");
            course = strtok(NULL, "|");
            rollStr = strtok(NULL, "|");
            feeStr = strtok(NULL, "\n");

            if (strcmp(course, targetCourse) == 0)
            {
                printf("\nRecord Found:\n");
                printf("---------------------------------\n");
                printf("Name   : %s\n", name);
                printf("Course : %s\n", course);
                printf("Roll   : %s\n", rollStr);
                printf("Fee    : %s\n", feeStr);
                printf("---------------------------------\n");
                displayed = 1;
                break;
            }
        }

        if (!displayed)
        {
            printf("No record found with Roll Number %d and Course '%s'.\n", targetRoll, targetCourse);
        }
    }
}

// Delete a record
void deleteRecord(char fname[])
{
    FILE *fp, *temp;
    //char fname[100];
    int targetRoll, found = 0, count = 0;

    char line[200];
    char *name, *course, *rollStr, *feeStr;

    char matchedLines[10][200];
    char targetCourse[20] = "";

   // printf("Enter file name to delete from:\n");
    //scanf("%s", fname);

    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("File not found.\n");
        return;
    }

    printf("Enter Roll Number to delete:\n");
    scanf("%d", &targetRoll);

    while (fgets(line, sizeof(line), fp))
    {
        char lineCopy[200];
        strcpy(lineCopy, line);

        name = strtok(lineCopy, "|");
        course = strtok(NULL, "|");
        rollStr = strtok(NULL, "|");
        feeStr = strtok(NULL, "\n");

        if (atoi(rollStr) == targetRoll)
        {
            strcpy(matchedLines[count], line);
            count++;
        }
    }

    fclose(fp);

    if (count == 0)
    {
        printf("No records found with Roll Number %d.\n", targetRoll);
        return;
    }

    if (count > 1)
    {
        printf("Multiple records found.\n");
        printf("Enter Course name to delete specific record:\n");
        fflush(stdin);
        fgets(targetCourse, sizeof(targetCourse), stdin);
        targetCourse[strcspn(targetCourse, "\n")] = '\0';
    }

    fp = fopen(fname, "r");
    temp = fopen("temp.txt", "w");

    if (temp == NULL)
    {
        printf("Error creating temp file.\n");
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        char lineCopy[200];
        strcpy(lineCopy, line);

        name = strtok(lineCopy, "|");
        course = strtok(NULL, "|");
        rollStr = strtok(NULL, "|");
        feeStr = strtok(NULL, "\n");

        int isTarget = 0;
        if (atoi(rollStr) == targetRoll)
        {
            if (count == 1 || strcmp(course, targetCourse) == 0)
                isTarget = 1;
        }

        if (isTarget)
        {
            found = 1;

            // Display deleted record:
            printf("\nRecord deleted:\n");
            printf("---------------------------------\n");
            printf("Name   : %s\n", name);
            printf("Course : %s\n", course);
            printf("Roll   : %s\n", rollStr);
            printf("Fee    : %s\n", feeStr);
            printf("---------------------------------\n");

            continue; // skip writing to temp file (i.e., delete)
        }

        fputs(line, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(fname);
        rename("temp.txt", fname);
        printf("Deletion completed successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("Record not found.\n");
    }
}


// Main function
int main()
{
    int choice;
    char fname[100];

    printf("\n=============================================\n");
    printf("  STUDENT FILE MANAGEMENT SOFTWARE\n");
    printf("  Developed by: Ankit Sharma\n");
    printf("  Final Year BCA Student\n");
    printf("=============================================\n\n");
      
      printf("Enter the file name:-\n\n");
      scanf("%s",fname);
    do
    {
        printf("\nMENU\n");
        printf("1. Create a Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Update a Record\n");
        printf("4. Search a Record\n");
        printf("5. Delete a Record\n");
        printf("6. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createRecord(fname);
            break;
        case 2:
            disRecord(fname);
            break;
        case 3:
            updateRecord(fname);
            break;
        case 4:
            searchRecord(fname);
            break;
        case 5:
            deleteRecord(fname);
            break;
        case 6:
            printf("Exiting program.\n");
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
        }

    } while (choice != 6);

    getch();
    return 0;
}
