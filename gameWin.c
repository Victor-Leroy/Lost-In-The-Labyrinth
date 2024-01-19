#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 10 // Size of the map
#define NUM_MONSTERS 3

// ======== MAP ========

char map[SIZE][SIZE] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '#', '#', '#', '#', '.', '#', '#', '#'},
    {'#', '.', '#', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '#', '#', '#', '.', '#', '#', '.', '#'},
    {'#', '.', '.', '.', '#', '.', '.', '#', '.', '#'},
    {'#', '#', '#', '.', '#', '#', '.', '#', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
    {'#', '#', '.', '#', '#', '#', '.', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
}; // Create a 2D array for the map

// ======== PLAYER ========

int playerX = 1; // Player's X spawning position
int playerY = 1; // Player's Y spawning position

int playerHP = 100;
int playerAttack = 10;
int playerDefense = 5;

// ======== MONSTERS ========
typedef struct {
    int x; // Monster's X position
    int y; // Monster's Y position
    int health;
    int attack;
    int defense;
} Monster;

// Create a monster
Monster monsters[NUM_MONSTERS];

// ======== FUNCTIONS ========

void combat(int monsterIndex) {
    char action;
    int damage;

    // cls the screen when the combat is launched
    system("cls"); // Use system("cls") on Unix-based systems

    // Display player stats
    printf("Player Stats:\n");
    printf("HP: %d\n", playerHP);
    printf("Attack: %d\n", playerAttack);
    printf("Defense: %d\n\n", playerDefense);

    // Display monster stats
    printf("Monster Stats:\n");
    printf("HP: %d\n", monsters[monsterIndex].health);
    printf("Attack: %d\n", monsters[monsterIndex].attack);
    printf("Defense: %d\n\n", monsters[monsterIndex].defense);

    while(playerHP > 0 && monsters[monsterIndex].health > 0) {
        printf("Do you want to (a)ttack or (e)scape?\n");
        scanf(" %c", &action);

        // cls the screen between every action
        system("cls"); // Use system("cls") on Unix-based systems

        if(action == 'a') {
            // Player's turn to attack
            damage = playerAttack - monsters[monsterIndex].defense;
            if(damage < 0) damage = 0; // Ensure damage is not negative
            monsters[monsterIndex].health -= damage;
            printf("You hit the monster for %d damage! Monster's health is now %d.\n", damage, monsters[monsterIndex].health);
        } else if(action == 'e') {
            // Player escapes
            printf("You escaped from the monster.\n");
            return;
        } else {
            printf("Invalid action. Please enter 'a' to attack or 'e' to escape.\n");
            continue;
        }

        // Monster's turn to attack
        if(monsters[monsterIndex].health > 0) {
            damage = monsters[monsterIndex].attack - playerDefense;
            if(damage < 0) damage = 0; // Ensure damage is not negative
            playerHP -= damage;
            printf("The monster hits you for %d damage! Your health is now %d.\n", damage, playerHP);
        }

        // If the monster is defeated, remove it from the map
        if(monsters[monsterIndex].health <= 0) {
            map[monsters[monsterIndex].x][monsters[monsterIndex].y] = '.';
        }
    }

    // Check who won
    if(playerHP > 0) {
        printf("You defeated the monster!\n");
    } else {
        printf("You were defeated by the monster...\n");
        exit(0); // End the game if the player was defeated
    }
}

void initializeMap() {
    // Set the player's initial position
    map[playerX][playerY] = 'P';

    // Set the monsters' initial positions
    for(int i = 0; i < NUM_MONSTERS; i++) {
        // Generate a random position for the monster
        int monsterX, monsterY;
        do {
            monsterX = rand() % SIZE;
            monsterY = rand() % SIZE;
        } while(map[monsterX][monsterY] == '#'); // Repeat until an empty space is found

        // Set the monster's position
        monsters[i].x = monsterX;
        monsters[i].y = monsterY;

        // Place the monster on the map
        map[monsterX][monsterY] = 'M';
    }
}

void printMap() {
    // cls the console
    #ifdef _WIN32
        system("cls");
    #else
        system("cls");
    #endif

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }

    // Print the player's HP
    printf("Player HP: %d\n", playerHP);
}

// ======== MOVEMENT ========

void movePlayer(char* direction) {
    int newX = playerX;
    int newY = playerY;

    if(strcmp(direction, "u") == 0) {
        newX--; // move up
    } else if(strcmp(direction, "d") == 0) {
        newX++; // move down
    } else if(strcmp(direction, "l") == 0) {
        newY--; // move left
    } else if(strcmp(direction, "r") == 0) {
        newY++; // move right
    } else {
        printf("Invalid direction. Please enter 'u' for up, 'd' for down, 'l' for left, or 'r' for right.\n");
        return;
    }

    // Check if the new position is within the map and not a wall
    if(newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE && map[newX][newY] != '#') {
        // Check for combat with each monster
        for(int i = 0; i < NUM_MONSTERS; i++) {
            if(newX == monsters[i].x && newY == monsters[i].y) {
                // Start combat with monster i
                combat(i);
                // If monster is defeated, remove it from the map
                if(monsters[i].health <= 0) {
                    map[monsters[i].x][monsters[i].y] = '.';
                }
            }
        }

        // cls the player's previous position on the map
        map[playerX][playerY] = '.';

        // Update player position
        playerX = newX;
        playerY = newY;
    }

    // cls the terminal screen
    system("cls"); // Use "cls" if you're on Windows

    // Place player at new position
    map[playerX][playerY] = 'P';
    for(int i = 0; i < NUM_MONSTERS; i++) {
        map[monsters[i].x][monsters[i].y] = 'M'; // Place the monster
    }
}

// ======== MAIN ========

int main() {
    srand(time(NULL)); // Seed the random number generator
    // Initialize monsters with different stats
    monsters[0] = (Monster){.x = rand() % SIZE, .y = rand() % SIZE, .health = 50, .attack = 5, .defense = 2};
    monsters[1] = (Monster){.x = rand() % SIZE, .y = rand() % SIZE, .health = 70, .attack = 7, .defense = 3};
    monsters[2] = (Monster){.x = rand() % SIZE, .y = rand() % SIZE, .health = 90, .attack = 9, .defense = 4};

    char direction[10]; // Buffer for direction input

    initializeMap(); // Initialize the map

    while(1) {
        printMap(); // Print the map

        // Ask for direction and move player
        printf("Enter direction (u/d/l/r): ");
        fgets(direction, sizeof(direction), stdin);

        // Remove newline character from direction
        char* newline = strchr(direction, '\n');
        if(newline) *newline = 0;

        // Move player
        movePlayer(direction);
    }

    return 0;
}