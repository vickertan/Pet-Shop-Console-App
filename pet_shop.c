#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_PETS_PER_STORAGE 10
#define NAME_LENGTH 50
#define SPECIES_LENGTH 50
#define NUM_STORAGES 10
#define DATA_FILE "pets_data.txt"

typedef struct {
    char name[NAME_LENGTH];
    char species[SPECIES_LENGTH];
    int age;
    struct tm dateAdded;
} Pet;

typedef struct {
    char storageName[NAME_LENGTH];
    Pet pets[MAX_PETS_PER_STORAGE];
    int petCount;
} Storage;

Storage storages[NUM_STORAGES];

void initialize_storages() {
    for (int i = 0; i < NUM_STORAGES; i++) {
        sprintf(storages[i].storageName, "Storage %d", i + 1);
        storages[i].petCount = 0;
    }
}

void load_data() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("No previous data found. Starting with empty storages.\n");
        return;
    }

    fread(&storages, sizeof(Storage), NUM_STORAGES, file);
    fclose(file);
}

void save_data() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Error saving data.\n");
        return;
    }

    fwrite(&storages, sizeof(Storage), NUM_STORAGES, file);
    fclose(file);
    printf("Data saved successfully.\n");
}

void input_date(struct tm *date) {
    int day, month, year;
    printf("Enter date added (yyyy mm dd): ");
    scanf("%d %d %d", &year, &month, &day);
    date->tm_year = year - 1900;
    date->tm_mon = month - 1;
    date->tm_mday = day;
}

void add_pet() {
    int storageIndex;
    printf("Enter storage number (1-%d): ", NUM_STORAGES);
    scanf("%d", &storageIndex);
    storageIndex--; // Convert to 0-based index

    if (storageIndex < 0 || storageIndex >= NUM_STORAGES) {
        printf("Invalid storage number.\n");
        return;
    }

    Storage *storage = &storages[storageIndex];

    if (storage->petCount >= MAX_PETS_PER_STORAGE) {
        printf("This storage is full. Cannot add more pets.\n");
        return;
    }

    Pet newPet;
    printf("Enter pet name: ");
    scanf("%s", newPet.name);
    printf("Enter pet species: ");
    scanf("%s", newPet.species);
    printf("Enter pet age: ");
    scanf("%d", &newPet.age);
    input_date(&newPet.dateAdded);

    storage->pets[storage->petCount++] = newPet;
    printf("Pet added successfully to %s!\n", storage->storageName);
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
    const int storageNameWidth = 15;
    const int nameWidth = 20;
    const int speciesWidth = 20;
    const int ageWidth = 10;
    const int dateWidth = 15;
    const int totalWidth = storageNameWidth + nameWidth + speciesWidth + ageWidth + dateWidth + 6;

    // Print table header
    print_line(totalWidth);
    printf("| %-13s | %-18s | %-18s | %-8s | %-13s |\n", "Storage", "Name", "Species", "Age", "Date Added");
    print_line(totalWidth);

    int petsFound = 0;

    for (int i = 0; i < NUM_STORAGES; i++) {
        Storage *storage = &storages[i];

        for (int j = 0; j < storage->petCount; j++) {
            Pet *pet = &storage->pets[j];
            printf("| %-13s | %-18s | %-18s | %-8d | ", storage->storageName, pet->name, pet->species, pet->age);
            print_date(pet->dateAdded);
            printf("    |\n");
            petsFound = 1;
        }
    }

    if (!petsFound) {
        printf("| %-13s | %-18s | %-18s | %-8s | %-13s |\n", "No pets", "No pets", "No pets", "No pets", "No pets");
    }

    print_line(totalWidth);
}

void delete_pet() {
    int storageIndex;
    printf("Enter storage number (1-%d): ", NUM_STORAGES);
    scanf("%d", &storageIndex);
    storageIndex--; // Convert to 0-based index

    if (storageIndex < 0 || storageIndex >= NUM_STORAGES) {
        printf("Invalid storage number.\n");
        return;
    }

    Storage *storage = &storages[storageIndex];

    if (storage->petCount == 0) {
        printf("No pets to delete in this storage.\n");
        return;
    }

    printf("\n%s:\n", storage->storageName);
    for (int j = 0; j < storage->petCount; j++) {
        Pet *pet = &storage->pets[j];
        printf("%d. Name: %s, Species: %s, Age: %d, Date Added: ", j + 1, pet->name, pet->species, pet->age);
        print_date(pet->dateAdded);
        printf("\n");
    }

    int petIndex;
    printf("Enter the number of the pet to delete: ");
    scanf("%d", &petIndex);
    petIndex--; // Convert to 0-based index

    if (petIndex < 0 || petIndex >= storage->petCount) {
        printf("Invalid number.\n");
        return;
    }

    for (int i = petIndex; i < storage->petCount - 1; i++) {
        storage->pets[i] = storage->pets[i + 1];
    }
    storage->petCount--;
    printf("Pet deleted successfully from %s!\n", storage->storageName);
}

void menu() {
    int choice;
    while (1) {
        printf("\nPet Shop Management\n");
        printf("1. Add Pet\n");
        printf("2. View Pets\n");
        printf("3. Delete Pet\n");
        printf("4. Save Data\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_pet();
                break;
            case 2:
                view_pets();
                break;
            case 3:
                delete_pet();
                break;
            case 4:
                save_data();
                break;
            case 5:
                save_data();
                printf("Exiting the program.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    initialize_storages();
    load_data();
    menu();
    return 0;
}
