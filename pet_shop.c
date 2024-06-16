#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_CAGES 10
#define MAX_PETS_PER_CAGE 5
#define NAME_LENGTH 50
#define SPECIES_LENGTH 50
#define DATA_FILE "pets_data.txt"

typedef struct {
    char name[NAME_LENGTH];
    char species[SPECIES_LENGTH];
    int age;
    struct tm dateAdded;
} Pet;

typedef struct {
    char cageName[NAME_LENGTH];
    Pet pets[MAX_PETS_PER_CAGE];
    int petCount;
} Cage;

Cage cages[NUM_CAGES];

void initialize_cages() {
    for (int i = 0; i < NUM_CAGES; i++) {
        sprintf(cages[i].cageName, "Cage %d", i + 1);
        cages[i].petCount = 0;
    }
}

void load_data() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("No previous data found. Starting with empty cages.\n");
        return;
    }

    if (fread(&cages, sizeof(Cage), NUM_CAGES, file) != NUM_CAGES) {
        printf("Error reading data. Starting with empty cages.\n");
    }
    fclose(file);
}

void save_data() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Error saving data.\n");
        return;
    }

    fwrite(&cages, sizeof(Cage), NUM_CAGES, file);
    fflush(file); // Ensure data is written to disk
    fclose(file);
    printf("Data saved successfully.\n");
}

void input_date(struct tm *date) {
    char input[20];
    int day, month, year;

    printf("Enter date added (yyyy mm dd) or press Enter to use current date : ");
    // fgets will get the entire line including whitespace
    getchar(); // Consume the leftover newline character
    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%d %d %d", &year, &month, &day) == 3){
      date->tm_year = year - 1900;
      date->tm_mon = month - 1;
      date->tm_mday = day;
    } else {
      // Get current date if no valid date is entered
      time_t now = time(NULL);
      *date = *localtime(&now);

      date->tm_hour = 0;
      date->tm_min = 0;
      date->tm_sec = 0;
    }
}

void add_pet() {
    int cageIndex;
    printf("Enter cage number (1-%d) : ", NUM_CAGES);
    scanf("%d", &cageIndex);
    cageIndex--; // Convert to 0-based index

    if (cageIndex < 0 || cageIndex >= NUM_CAGES) {
        printf("Invalid cage number.\n");
        return;
    }

    Cage *cage = &cages[cageIndex];

    if (cage->petCount >= MAX_PETS_PER_CAGE) {
        printf("This cage is full. Cannot add more pets.\n");
        return;
    }

    Pet newPet;
    getchar(); // Consume leftover newline character

    printf("Enter pet name : ");
    fgets(newPet.name, NAME_LENGTH, stdin);
    newPet.name[strcspn(newPet.name, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter pet species : ");
    fgets(newPet.species, SPECIES_LENGTH, stdin);
    newPet.species[strcspn(newPet.species, "\n")] = '\0';

    printf("Enter pet age (year) : ");
    scanf("%d", &newPet.age);

    input_date(&newPet.dateAdded);

    cage->pets[cage->petCount++] = newPet;
    printf("Pet added successfully to %s!\n", cage->cageName);
    save_data();
}

void print_line(int length) {
    for (int i = 0; i < length; i++) {
        printf("-");
    }
    printf("\n");
}

void print_date(struct tm date) {
    printf("%04d-%02d-%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
}

void view_pets() {
    const int cageNameWidth = 15;
    const int nameWidth = 20;
    const int speciesWidth = 20;
    const int ageWidth = 10;
    const int dateWidth = 15;
    const int totalWidth = cageNameWidth + nameWidth + speciesWidth + ageWidth + dateWidth + 8;

    // Print table header
    print_line(totalWidth);
    printf("| %-13s | %-18s | %-18s | %-10s | %-13s |\n", "Cage", "Name", "Species", "Age (year)", "Date Added");
    print_line(totalWidth);

    int petsFound = 0;

    // Search pet's data in every cages
    for (int i = 0; i < NUM_CAGES; i++) {
        Cage *cage = &cages[i];

        for (int j = 0; j < cage->petCount; j++) {
            Pet *pet = &cage->pets[j];
            printf("| %-13s | %-18s | %-18s | %-10d | ", cage->cageName, pet->name, pet->species, pet->age);
            print_date(pet->dateAdded);
            printf("    |\n");
            petsFound = 1;
        }
    }

    if (!petsFound) {
        printf("| %-13s | %-18s | %-18s | %-10s | %-13s |\n", "No pets", "No pets", "No pets", "No pets", "No pets");
    }

    print_line(totalWidth);
}

void remove_pet() {
    int cageIndex;
    printf("Enter cage number (1-%d) : ", NUM_CAGES);
    scanf("%d", &cageIndex);
    cageIndex--; // Convert to 0-based index

    if (cageIndex < 0 || cageIndex >= NUM_CAGES) {
        printf("Invalid cage number.\n");
        return;
    }

    Cage *cage = &cages[cageIndex];

    if (cage->petCount == 0) {
        printf("No pets to remove in this cage.\n");
        return;
    }

    printf("\n%s:\n", cage->cageName);
    for (int j = 0; j < cage->petCount; j++) {
        Pet *pet = &cage->pets[j];
        printf("%d. Name : %s, Species : %s, Age (year): %d, Date Added : ", j + 1, pet->name, pet->species, pet->age);
        print_date(pet->dateAdded);
        printf("\n");
    }

    int petIndex;
    printf("Enter the number of the pet to remove : ");
    scanf("%d", &petIndex);
    petIndex--; // Convert to 0-based index

    if (petIndex < 0 || petIndex >= cage->petCount) {
        printf("Invalid number.\n");
        return;
    }

    for (int i = petIndex; i < cage->petCount - 1; i++) {
        cage->pets[i] = cage->pets[i + 1];
    }
    cage->petCount--;
    printf("Pet removed successfully from %s!\n", cage->cageName);
    save_data();
}

void menu() {
    char input[10];
    int choice;
    while (1) {
        printf("\nPet Shop Management\n");
        printf("1. Add Pet\n");
        printf("2. View Pets\n");
        printf("3. Remove Pet\n");
        printf("4. Exit\n");
        printf("Enter your choice : ");

        // Read input as a string
        fgets(input, sizeof(input), stdin);

        // Attempt to convert the input to an integer
        if (sscanf(input, "%d", &choice) != 1) {
          printf("Invalid input, please try again.\n");
          continue;
        }

        switch (choice) {
            case 1:
                add_pet();
                break;
            case 2:
                view_pets();
                break;
            case 3:
                remove_pet();
                break;
            case 4:
                save_data();
                printf("Exiting the program.\n");
                return;
            default:
                printf("Invalid input, please try again.\n");
        }
    }
}

int main() {
    initialize_cages();
    load_data();
    menu();
    return 0;
}
