#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_ITEM_LEN 255

// Structure representing a room in the dungeon
typedef struct Room 
{
    long long id;
    char *item;
    int numPaths;
    struct Room **adjacentRooms;
} Room;

// Structure representing the dungeon map
typedef struct 
{
    Room **rooms;
    long long numRooms;
} Map;

Map dungeon;
char *playerItem = NULL;
Room *dragonRoom = NULL;
Room *playerRoom = NULL;
Room *ritualRoom = NULL;
long long **dist;
long long **next;
int health = 3;
char amulet[] = "amulet";
int ritual = 0;
bool Lich = false;

// Function prototypes
Room *chooseRandomRoom();
Room *chooseEmptyRoom();
Room *createNewRoom(long long id);
int findRoomIndex(long long id);
Room *findOrCreateRoom(long long id);
void addAdjacentRoom(long long id, long long adjId);
void initializeShortestPaths();
void floydWarshall();
void initializeDungeon();
void displayRoomInfo();
void swapItems();
void warmerColder(Room *prevRoom, Room *newRoom);
void checkDragonRoom();
void processPlayerMove(long long choice);
void handlePlayerChoice(long long choice);
void moveDragon();
void loseHealth(int damage, long long choice);
void freeDungeon(Room **rooms);


// Chooses a random room that is not the playerRoom or dragonRoom
Room *chooseRandomRoom() 
{
    int *validRoomIndices = (int *)malloc((dungeon.numRooms - 1) * sizeof(int));
    if (validRoomIndices == NULL) 
    {
        return NULL;
    }

    int count = 0;

    for (int i = 0; i < dungeon.numRooms; i++) 
    {
        if (dungeon.rooms[i] != dragonRoom && dungeon.rooms[i] != playerRoom)
        {
            validRoomIndices[count++] = i;
        }
    }

    if (count == 0)
    {
        return NULL;
    }

    int randomIndex = rand() % count;
    return dungeon.rooms[validRoomIndices[randomIndex]];
}

// Chooses and random empty room that is not the playerRoom or dragonRoom
Room *chooseEmptyRoom() 
{
    int *validRoomIndices = (int *)malloc((dungeon.numRooms - 1) * sizeof(int));
    if (validRoomIndices == NULL) 
    {
        return NULL;
    }

    int count = 0;

    for (int i = 0; i < dungeon.numRooms; i++) 
    {
        if (dungeon.rooms[i] != dragonRoom && dungeon.rooms[i] != playerRoom)
        {
            if (dungeon.rooms[i]->item == NULL)
            validRoomIndices[count++] = i;
        }
    }

    if (count == 0)
    {
        return NULL;
    }

    int randomIndex = rand() % count;
    return dungeon.rooms[validRoomIndices[randomIndex]];
}

// Create a new room
Room *createNewRoom(long long id) 
{
    Room *room = (Room *)malloc(sizeof(Room));

    room->id = id;
    room->item = NULL;
    room->numPaths = 0;
    room->adjacentRooms = NULL;

    dungeon.rooms = realloc(dungeon.rooms, (dungeon.numRooms + 1) * sizeof(Room *));
    dungeon.rooms[dungeon.numRooms] = room;
    dungeon.numRooms++;

    return room;
}

// Find the index of a room ID in the dungeon rooms array
int findRoomIndex(long long id) 
{
    for (int i = 0; i < dungeon.numRooms; i++) 
    {
        if (dungeon.rooms[i]->id == id) 
        {
            return i;
        }
    }
    return -1;
}

// Find a room and return it or if it does not exist create it and return it
Room *findOrCreateRoom(long long id) 
{
    int index = findRoomIndex(id);
    if (index != -1)
    {
        return dungeon.rooms[index];
    }

    Room *newRoom = createNewRoom(id);
    return newRoom;
}

// Add adjacent room to the adjacency list and increas numPaths assigned to the room by 1
void addAdjacentRoom(long long id, long long adjId) 
{
    if(adjId != 0)
    {
        Room *room = findOrCreateRoom(id);
        Room *adjRoom = findOrCreateRoom(adjId);

        if (room->adjacentRooms == NULL)
        {
            room->adjacentRooms = malloc(sizeof(Room *));
        }
        else
        {
            room->adjacentRooms = realloc(room->adjacentRooms, (room->numPaths + 1) * sizeof(Room *));
        }

        room->adjacentRooms[room->numPaths++] = adjRoom;
    }
}

// Initialize shortest path matrices
void initializeShortestPaths() 
{
    dist = (long long **)malloc(dungeon.numRooms * sizeof(long long *));
    next = (long long **)malloc(dungeon.numRooms * sizeof(long long *));
    for (int i = 0; i < dungeon.numRooms; i++) 
    {
        dist[i] = (long long *)malloc(dungeon.numRooms * sizeof(long long));
        next[i] = (long long *)malloc(dungeon.numRooms * sizeof(long long));
        for (int j = 0; j < dungeon.numRooms; j++) 
        {
            if (i == j) 
            {
                dist[i][j] = 0;
            }
            else 
            {
                dist[i][j] = LLONG_MAX;
            }
            for (int k = 0; k < dungeon.rooms[i]->numPaths; k++)
            {
                if (dungeon.rooms[i]->adjacentRooms[k] == dungeon.rooms[j])
                {
                    dist[i][j] = 1;
                }
            }
            next[i][j] = j;
        }

        for (int k = 0; k < dungeon.rooms[i]->numPaths; k++) 
        {
            int adjacentRoomIndex = dungeon.rooms[i]->adjacentRooms[k]->id; 
            dist[i][adjacentRoomIndex] = 1;
        }
    }
}

// Floyd-Warshall algorithm to compute shortest paths
void floydWarshall() 
{
    for (int i = 0; i < dungeon.numRooms; i++) 
    {
        for (int j = 0; j < dungeon.numRooms; j++) 
        {
            for (int k = 0; k < dungeon.numRooms; k++) 
            {
                if (dist[j][i] != LLONG_MAX && dist[i][k] != LLONG_MAX &&
                    dist[j][k] > dist[j][i] + dist[i][k]) 
                {
                    dist[j][k] = dist[j][i] + dist[i][k];
                    next[j][k] = next[j][i];
                }
            }
        }
    }
}

// Initialize the dungeon and read input
void initializeDungeon() 
{
    dungeon.rooms = NULL;
    long long playerStartId, dragonStartId;
    long long id, adjId;

    while (1) 
    {
        // Create a new room for the initial digit input if it doesn't already exist
        if (scanf("%lld", &id) == EOF)
        {
            exit(0);
        }
        int endLoop = id;
        if (endLoop == 0)
        {
            while (getchar() != '\n');
            break;
        }
        else
        {
            Room *room = findOrCreateRoom(id);

            // Start reading the string after the '(' and stop at the ')'. Parse the commas out and store the digits as tokens.
            // Add an adjacent room to initial room from the input.
            char adjRoomsBuffer[MAX_ITEM_LEN];
            if(scanf(" (%[^)])", adjRoomsBuffer) == EOF)
            {
                break;
            }
            char *token = strtok(adjRoomsBuffer, ",");
            while (token != NULL) 
            {
                if(token != 0)
                {
                    adjId = atoll(token);
                    addAdjacentRoom(id, adjId);
                    token = strtok(NULL, ",");
                }
            }

            // If the next character after ')' is a space read and store in item the rest of the line, trim the '\n', replace it with '\0'
            // set room item to item
            char firstChar = getchar();
            if(firstChar == ' ')
            {
                char item[MAX_ITEM_LEN];
                fgets(item, MAX_ITEM_LEN, stdin);
                item[strcspn(item, "\n")] = '\0';
                if(strlen(item) > 0 && item[0] != '\n')
                {
                    room->item = malloc(strlen(item) + 1);
                    strcpy(room->item, item);
                }
            }
        }
    }

    // Set the starting room of the player and dragon
    scanf("%lld,%lld", &playerStartId, &dragonStartId);
    playerRoom = findOrCreateRoom(playerStartId);
    dragonRoom = findOrCreateRoom(dragonStartId);
    ritualRoom = chooseRandomRoom();
    Room *emptyRoom = chooseEmptyRoom();
    emptyRoom->item = amulet;

    initializeShortestPaths();
    floydWarshall();
}

// Display the current room information (ie room number, item, adjacent rooms)
void displayRoomInfo() 
{
    printf("You are in room %lld", playerRoom->id);
    if (playerRoom->item != NULL) 
    {
        printf(", on the ground is a %s", playerRoom->item);
    }

    if (playerRoom->numPaths > 0) 
    {
        printf(".  Nearby are rooms ");
        for (int i = 0; i < playerRoom->numPaths; i++) 
        {
            printf("%lld", playerRoom->adjacentRooms[i]->id);
            if (i < playerRoom->numPaths - 1) 
            {
                printf(", ");
            }
        }
    }
    printf(".\n");
}

// Swap items between the player and the current room
void swapItems() 
{
    if (playerItem == NULL && playerRoom->item != NULL) 
    {
        printf("You pick up the %s.\n", playerRoom->item);
    } 
    else if (playerItem != NULL && playerRoom->item == NULL) 
    {
        printf("You drop the %s.\n", playerItem);
    } 
    else if (playerItem != NULL && playerRoom->item != NULL) 
    {
        printf("You drop the %s and pick up the %s.\n", playerItem, playerRoom->item);
    }
    else if (playerItem == NULL && playerRoom->item == NULL)
    {
        printf("Is that something on the ground?\nYou reach down... there is nothing there.\n");
    }

    char *temp = playerItem;
    playerItem = playerRoom->item;
    playerRoom->item = temp;
}

// Check whether player is closer, further, or same distance after move
void warmerColder(Room *prevRoom, Room *newRoom) 
{
    floydWarshall();
    int prevPlayerIndex = findRoomIndex(prevRoom->id);
    int newPlayerIndex = findRoomIndex(newRoom->id);
    int dragonIndex = findRoomIndex(dragonRoom->id);

    long long prevDistance = dist[prevPlayerIndex][dragonIndex];
    long long newDistance = dist[newPlayerIndex][dragonIndex];

    if (newDistance < prevDistance) 
    {
        printf("You're getting warmer!\n");
        if (newDistance == 2)
        {
            printf("A metallic smell is in the air!\n");
        }
        else if (newDistance == 1)
        {
            printf("It is stiflingly hot in this room!\n");
        }
    } 
    else if (newDistance > prevDistance) 
    {
        printf("You're getting colder!\n");
    } 
    else 
    {
        printf("You're neither warmer nor colder.\n");
    }
}

// Check if the player has entered the dragon's room
void checkDragonRoom() 
{
    if (playerRoom == dragonRoom) 
    {
        printf("A large red dragon swoops down upon you from above.\n");
        if (playerItem && strcmp(playerItem, "sword") == 0) 
        {
            printf("You instinctively slash out with the sword, and mortally wound the beast!\nYou win!\n");
        } 
        else if (playerItem == NULL)
        {
            printf("The dragon chuckles in amusement as you punch its scaley hide.\n");
            printf("Growing bored watching your futile attack, it eats you.\nYou are dead.\n");
        }
        else 
        {
            printf("The %s is useless against the mighty dragon.\n", playerItem);
            printf("You are burned to a crisp by its flaming breath.\nYou are dead.\n");
        }
        exit(0);
    }
}

// Process player's movement between rooms or if the player takes an action that damages themself
// It also handles the actions of the Ritual side quest
void processPlayerMove(long long choice) 
{
    Room *prevRoom = playerRoom;
    Room *nextRoom = NULL;

    for (int i = 0; i < playerRoom->numPaths; i++) 
    {
        if (playerRoom->adjacentRooms[i]->id == choice) 
        {
            nextRoom = playerRoom->adjacentRooms[i];
            break;
        }
    }
    
    if (nextRoom == NULL)
    {
        int damage;
        if((int)choice == -1 && playerItem == NULL)
        {
            if(health == 3)
            {
                printf("You punch yourself in the face...\nAh! how clever! Sun Tzu did say 'Appear weak when you are strong'.\nThe dragon won't see you as a threat now.\n");
                if(playerRoom == ritualRoom && ritualRoom->item == amulet)
                {
                    printf("Wait!! what's this? your blood on the floor is pulled towards and absorbed by the runes\n");
                    printf("The amulet begins to glow a dull crimnson as it appears to feed on your blood\n");
                    ritual = 1;
                }
            }
            else if(health == 2)
            {
                if(playerRoom == ritualRoom && ritualRoom->item == amulet && ritual == 0)
                {
                    printf("Wait!! what's this? your blood on the floor is pulled towards and absorbed by the runes\n");
                    printf("The amulet begins to glow a dull crimnson as it appears to feed on your blood\n");
                    ritual = 1;
                }
                if(playerRoom == ritualRoom && ritualRoom->item == amulet && ritual == 1)
                {
                    printf("Your blood on the floor is once again pulled into the runes... they begin to emit a soft light\n");
                    printf("The amulet thrums with power and glows a bright ruby red as it takes in more blood\n");
                    ritual = 2;
                }
                else
                {
                printf("You might want to stop punching yourself now...\nBeing seriously injured while fighting a dragon seems like a bad idea\n");
                }
            }
            else if(health == 1)
            {
                printf("Well... I don't know what you expected to happen.\n");
            }
            else if (Lich == true)
            {
                printf("You punch yourself in the face. It is a heavy blow but you are unharmed\n");
            }

            damage = 1;
        }
        else if((int)choice == -1 && playerItem == amulet)
        {
            if(playerRoom == ritualRoom && ritual == 0)
            {
                printf("You strike youself with the amulet!\nYour blood splatters onto the amulet and floor.\n");
                printf("Wait!! what's this? your blood on the floor is pulled towards and absorbed by the runes\n");
                printf("The amulet begins to glow a dull crimnson as it appears to feed on your blood\n");
                ritual = 1;
            }
            else if (playerRoom == ritualRoom && ritual == 1)
            {
                printf("You strike youself with the amulet!\nYour blood splatters onto the amulet and floor.\n");
                printf("Your blood on the floor is once again pulled into the runes... they begin to emit a soft light\n");
                printf("The amulet thrums with power and glows a bright ruby red as it takes in more blood\n");
                ritual = 2;
            }
            else if (playerRoom == ritualRoom && health == 1)
            {
                printf("You strike yourself with your %s\nThe %s's light fades... as does your life.\n", playerItem, playerItem);
            }
            else
            {
                printf("You strike yourself with your %s\nThe blood on the %s quivers and it glows breifly but the light fades\n", playerItem, playerItem);
            }
            damage = 1;
        }
        else if ((int)choice == -1 && strcmp(playerItem, "sword") == 0 && ritualRoom->item == amulet && Lich == false)
        {
            printf("You impale yourself with your %s and fall onto the cold stone floor in a pool of your own blood\n");
            printf("As your vision begins to fade, you see the Amulet on the ground next to you.\n");
            printf("Your blood is absorbed by the amulet as it, and the runes etched into room, shine blindingly bright\n");
            printf("The room explodes with magical energy that begins to flow into your body.\n");
            printf("Feeling power surge through you, you arise, having become a Lich!!\n");
            Lich = true;
        }
        else if((int)choice == -1 && playerItem != NULL)
        {
            if(Lich == false)
            {
                printf("You strike yourself with your %s and bleed out on the floor of the dungeon\n", playerItem);
            }
            else
            {
                printf("You strike yourself with your %s... a Lich can't be harmed by something such as this.\n", playerItem);
            }
            health = 0;
            damage = 0;
        }
        else
        {
            printf("You turn and start running towards room %d", choice);
            printf("... and run face first into a wall!\nYou lose 1 health.\nRoom %d does not connect to room %d\n", playerRoom->id, choice);
            damage = 1;
        }

        if (Lich == false)
        {
            loseHealth(damage, choice);
        }
        return;
    }
    playerRoom = nextRoom;
    moveDragon();
    checkDragonRoom();
    warmerColder(prevRoom, playerRoom);
    if(playerRoom == ritualRoom)
    {
        printf("Entering the room you see faint stains on the stone floor and strange symbols etched into the walls.\n");
        printf("Upon closer inspection those stains are blood!  Something unholy once unfolded here...\n");
    }
}

// Handle player's actions
void handlePlayerChoice(long long choice) 
{
    if (choice == 0) 
    {
        swapItems();
    } 
    else 
    {
        processPlayerMove(choice);
    }
}

// Move the dragon towards the player using the shortest path
void moveDragon() 
{
    floydWarshall();
    int dragonIndex = findRoomIndex(dragonRoom->id);
    int playerIndex = findRoomIndex(playerRoom->id);

    if (dragonRoom->numPaths == 0)
    {
        return;
    }

    int nextRoomIndex = next[dragonIndex][playerIndex];
    for (int i = 0; i < dragonRoom->numPaths; i++) 
    {
        if (dragonRoom->adjacentRooms[i]->id == dungeon.rooms[nextRoomIndex]->id) 
        {
            dragonRoom = dragonRoom->adjacentRooms[i]; 
            break;
        }
    }
}

// Tracking the players health and damage they recieve
void loseHealth(int damage, long long choice)
{
    health -= damage;
    if(health == 0 && damage == 1 && choice != -1)
    {
        printf("Congratulations!\nYou are the first person in this dungeon to have died by repeatedly running into a wall.\nYou lose.\n");
        exit(0);
    }
    else if (health < 1)
    {
        printf("Why... just why?\nYou lose.\n");
        exit(0);
    }
}

// Free the dungeon memory
void freeDungeon(Room **rooms) 
{
    for (int i = 0; i < dungeon.numRooms; i++)
    {
        free(dist[i]);
        free(next[i]);
    }
    free(dist);
    free(next);

    if(playerItem != NULL)
    {
        free(playerItem);
    }

    for (int i = 0; i < dungeon.numRooms; i++) 
    {
        for (int j = 0; j < dungeon.rooms[i]->numPaths; j++)
        {
            free(dungeon.rooms[i]->item);
            free(dungeon.rooms[i]->adjacentRooms[j]);
            free(dungeon.rooms[i]);
        }
    }

    free(dungeon.rooms);
}

// Main game loop
int main() 
{
    initializeDungeon();
    printf("\n\n\nWelcome to the dungeon.\nIt is dank and musty down here, large cobwebs adorn corners of this forgotten place.\n");
    printf("Despite how warm it is, a shiver runs down your spine.\n");
    long long choice;
    while (1) 
    {
        displayRoomInfo();
        scanf("%lld", &choice);
        printf("> %d\n", choice);
        handlePlayerChoice(choice);
    }

    freeDungeon(dungeon.rooms);
    return 0;
}
