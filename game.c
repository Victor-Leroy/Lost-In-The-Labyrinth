#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #define sleep(x) Sleep(x * 1000)
#else
    #include <unistd.h>
#endif


#define SIZE 20 // Size of the map
#define NUM_MONSTERS 10


// ======== MAP ========

char map[SIZE][SIZE] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#'},
    {'#', '.', '#', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '#', '.', '.', '.', '.', '#'},
    {'#', '.', '#', '#', '#', '.', '#', '#', '.', '.', '#', '.', '#', '.', '#', '.', '#', '#', '.', '#'},
    {'#', '.', '.', '.', '#', '.', '.', '#', '.', '#', '#', '.', '#', '.', '#', '.', '#', '.', '.', '#'},
    {'#', '#', '#', '.', '#', '#', '.', '#', '.', '.', '#', '#', '#', '.', '.', '.', '#', '.', '#', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '#', '#', '#', '#', '#', '.', '#', '#'},
    {'#', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '#', '#', '#', '#', '.', '#', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '#', '.', '.', '#', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '#', '#', '.', '#', '#', '.', '#', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
}; // Create a 2D array for the map

// Add a new field for the current floor
int currentFloor = 1;

// ======== PLAYER ========

int playerX = 1; // Player's X spawning position
int playerY = 1; // Player's Y spawning position

int playerMaxHP = 100; // The player starts with 100 HP
int playerHP = 100;
int playerAttack = 10;
int playerDefense = 5;
int playerLevel = 1; // The player starts at level 1
int playerExp = 0; // The player starts with 0 experience points


// ======== MONSTERS ========
typedef struct {
    int x; // Monster's X position
    int y; // Monster's Y position
    int health;
    int attack;
    int defense;
    int level;
    bool alive;
} Monster;

// Create a monster

int numMonsters = NUM_MONSTERS; // Create a new variable to store the number of monsters
Monster* monsters = NULL; // Change the type of 'monsters' to a pointer and initialize it to NULL

void generateMonsters(int floor) {
    // Resize the monsters array to accommodate the new monsters
    numMonsters *= floor;
    monsters = realloc(monsters, numMonsters * sizeof(Monster));

    // Initialize monsters with different stats
    for(int i = 0; i < numMonsters; i++) {
        do {
            monsters[i].x = rand() % (SIZE - 2) + 1;
            monsters[i].y = rand() % (SIZE - 2) + 1;
        } while(monsters[i].x == playerX && monsters[i].y == playerY); // Regenerate coordinates if they match the player's

        // Set the rest of the monster's stats
        monsters[i].health = rand() % 30 + 50; // Health between 30 and 80
        monsters[i].attack = rand() % 5 + 5; // Attack between 5 and 10
        monsters[i].defense = rand() % 3 + 3; // Defense between 3 and 6
        // Set the monster's initial state
        monsters[i].alive = true; // The monster is initially alive
        monsters[i].level = floor; // The monster's level is the same as the floor number
    }
}

// ======== FUNCTIONS ========

void combat(int monsterIndex) {
    char action;
    int damage;

    // Clear the screen when the combat is launched
    system("clear"); // Use system("clear") on Unix-based systems

    // Display player stats
    printf("Player Stats:\n");
    printf("Level: %d\n", playerLevel);
    printf("HP: %d\n", playerHP);
    printf("Attack: %d\n", playerAttack);
    printf("Defense: %d\n\n", playerDefense);
    printf("XP: %d/%d\n\n", playerExp, playerLevel * 100);

    // Display monster stats
    printf("Monster Stats:\n");
    printf("HP: %d\n", monsters[monsterIndex].health);
    printf("Attack: %d\n", monsters[monsterIndex].attack);
    printf("Defense: %d\n\n", monsters[monsterIndex].defense);

    while(playerHP > 0 && monsters[monsterIndex].health > 0) {
        printf("Do you want to (a)ttack or (e)scape?\n");
        scanf(" %c", &action);

        // Clear the screen between every action
        system("clear"); // Use system("clear") on Unix-based systems

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

        // Check if the monster is defeated
        if(monsters[monsterIndex].health <= 0) {
            // Replace the monster with a '.'
            map[monsters[monsterIndex].x][monsters[monsterIndex].y] = '.';
            // Give the player experience points for defeating the monster
            int expGained = 10 * monsters[monsterIndex].level;
            playerExp += expGained;

            printf("You gained %d experience points!\n", expGained);

            monsters[monsterIndex].alive = false;
            // Pause for 2 seconds to allow the player to read the message
            sleep(2);

        // Check if all monsters are dead
        bool allMonstersDead = true;
        for(int i = 0; i < numMonsters; i++) {
            if(monsters[i].alive) {
                allMonstersDead = false;
                break;
            }
        }

        // If all monsters are dead, move to the next floor
        if(allMonstersDead) {
            printf("You have cleared floor %d! Moving to floor %d.\n", currentFloor, currentFloor + 1);
            currentFloor++;
            generateMonsters(currentFloor);

            int playerX = 1; // Player's X spawning position
            int playerY = 1; // Player's Y spawning position

            playerHP = playerMaxHP;
    }
        // Check if the player has enough experience points to level up
        if(playerExp >= playerLevel * 100) {
            // Level up the player
            playerLevel++;
            playerExp = 0; // Reset the player's experience points

            // Increase the player's stats
            playerMaxHP += 10;
            playerAttack += 5;
            playerDefense += 2;

            printf("You leveled up! You are now level %d.\n", playerLevel);
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
}

void randomEvent() {
    int event = rand() % 100; // Generate a random number between 0 and 99

    if(event < 5) { // 5% chance for a rickroll
        printf("You've been rickrolled! Go to this link to enjoy the song: https://www.youtube.com/watch?v=dQw4w9WgXcQ\n");
    }
    // Add more events as needed
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

        // Set the monster's initial state
        monsters[i].alive = true; // The monster is initially alive

        // Place the monster on the map
        map[monsterX][monsterY] = 'M';
    }
}

void printMap() {
    // Clear the console
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    
    // Set the monsters' positions
    for(int i = 0; i < NUM_MONSTERS; i++) {
        if(monsters[i].alive) { // Only set the position if the monster is alive
            map[monsters[i].x][monsters[i].y] = 'M';
        }
    }
    // Print the player's HP
    printf("Player HP: %d\n", playerHP);
    printf("Level: %d\n", playerLevel);
    printf("XP: %d/%d\n", playerExp, playerLevel * 100);
    printf("Floor: %d\n", currentFloor);
}

// ======== MOVEMENT ========

void movePlayer(char* direction) {
    int newX = playerX;
    int newY = playerY;

    if(strcmp(direction, "z") == 0) {
        newX--; // move up
    } else if(strcmp(direction, "s") == 0) {
        newX++; // move down
    } else if(strcmp(direction, "q") == 0) {
        newY--; // move left
    } else if(strcmp(direction, "d") == 0) {
        newY++; // move right
    } else {
        printf("Invalid direction. Please enter 'z' for up, 's' for down, 'q' for left, or 'd' for right.\n");
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
                    monsters[i].alive = false; // Set the monster's state to defeated
                    map[monsters[i].x][monsters[i].y] = '.';
                }
            }
        }

        // Clear the player's previous position on the map
        map[playerX][playerY] = '.';

        // Update player position
        playerX = newX;
        playerY = newY;
    }

    // Clear the terminal screen
    system("clear"); // Use "cls" if you're on Windows

    // Place player at new position
    map[playerX][playerY] = 'P';
}

// ======== MAIN ========

int main() {
    srand(time(NULL)); // Seed the random number generator
    
    generateMonsters(currentFloor); // Generate monsters for the current floor

    char direction[10]; // Buffer for direction input

    initializeMap(); // Initialize the map

    // Call this function at appropriate points in your game

    while(1) {
        printMap(); // Print the map

        // Ask for direction and move player
        printf("Enter direction (z/q/s/d): ");
        fgets(direction, sizeof(direction), stdin);
        // Call this function at appropriate points in your game
        
        randomEvent();
        // Check if all monsters are dead
        bool allMonstersDead = true;
        for(int i = 0; i < numMonsters; i++) {
            if(monsters[i].alive) {
                allMonstersDead = false;
                break;
        }
    }

    if(allMonstersDead) {
        printf("You have cleared floor %d! Moving to floor %d.\n", currentFloor, currentFloor + 1);
        currentFloor++;
        generateMonsters(currentFloor);

        playerX = 1; // Player's X spawning position
        playerY = 1; // Player's Y spawning position

        playerHP = playerMaxHP;

        // Reset allMonstersDead to false for the next floor
        allMonstersDead = false;
    }
        // Remove newline character from direction
        char* newline = strchr(direction, '\n');
        if(newline) *newline = 0;

        // Move player
        movePlayer(direction);
    }

    return 0;
}