#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//global consts
#define PLAYERSLIMIT 4
#define CARDS 13


// structure of player
typedef struct
{
	int id;
	char name[30];
	int hand[CARDS];
	int points;
} Player;

//Function signatures
void startGame();
void printMenu();
void initalizePlayers(int numPlayers);
void initializeDeck(int deck[], int deckSize);
void dealCards(Player players[], int deck[], int numPlayers);
void printHands(Player players[], int numPlayers);
void startRound(Player players[], int deck[], int numPlayers);
int calcPoints(int playedCards[], int numPlayers);
void saveGame(Player players[], int numPlayers, int currentRound);
int loadGame(Player players[], int* numPlayers, int* currentRound);

/* GLOBAL VARIABLE DECLARATION */
int numPlayers;
int deck[CARDS];
Player players[PLAYERSLIMIT];
// keep track of round
int currentRound = 1;

// starts game on load
int main()
{
	startGame();
}


void startGame()
{
	printMenu();
}

// for saving game
void saveGame(Player players[], int numPlayers, int currentRound)
{
	// looks for file with name savegame - if not get error
	// w(write mode) indicates that it will create  a new file or overwrite an existing file.
	FILE* file = fopen("savegame.txt", "w");
	if (!file)
	{
		printf("Failed to save game.\n");
		return;
	}

	// Saving data to file number of players and current round
	fprintf(file, "%d %d\n", numPlayers, currentRound);

	/*
	Loop: Iterates through each player's ID and hands.
	*/
	for (int i = 0; i < numPlayers; i++)
	{
		// fprintf: Writes formatted data to the file.
		fprintf(file, "%d %s %d\n", players[i].id, players[i].name, players[i].points);
		/*
		Iterates through each card (j) in the player’s hand.
		Writes each card value to the file in a single line, separated by spaces.
		*/
		for (int j = 0; j < CARDS; j++)
		{
			//make sure it doesnt save the 0s
			if (players[i].hand[j] != 0)
			{
				fprintf(file, "%d ", players[i].hand[j]);
			}
		}
		fprintf(file, "-1\n"); // End of player's hand
	}

	fclose(file);
	printf("Game saved successfully.\n");
}

// for loading game - anything you need to update a true value of anything - you need use a pointer

int loadGame(Player players[], int* numPlayers, int* currentRound)
{
	// open savegame
	FILE* file = fopen("savegame.txt", "r");
	if (!file)
	{
		printf("Failed to load game. Save file not found.\n");
		return 0;
	}

	// Load number of players and current round
	fscanf(file, "%d %d", numPlayers, currentRound);

	for (int i = 0; i < *numPlayers; i++)
	{
		// Read player ID, name, and points
		fscanf(file, "%d %s %d", &players[i].id, players[i].name, &players[i].points);

		// Clear hand before loading
		for (int j = 0; j < CARDS; j++)
		{
			players[i].hand[j] = 0;
		}

		// Read the hand until we hit -1 (see the load game func to see why we do this)
		int card, handIndex = 0;
		while (fscanf(file, "%d", &card) == 1 && card != -1)
		{

			players[i].hand[handIndex++] = card;
		}
	}

	fclose(file);
	printf("Game loaded successfully!\n");
	return 1;
}

void printMenu()
{

	int choice;

	printf("==== MENU ====\n");
	printf("Please enter your choice\n");
	printf("1. Start new game\n");
	printf("2. load file\n");
	printf("3. Exit without saving\n");

	scanf("%d", &choice);

	switch (choice)
	{
	case 1:
		printf("\n==== Game Start ====\n");
		printf("How many players? (2-4)\n");
		scanf("%d", &numPlayers);
		initalizePlayers(numPlayers);
		printf("Dealing cards.......");
		initializeDeck(deck, CARDS);
		dealCards(players, deck, numPlayers);
		printHands(players, numPlayers);
		startRound(players, deck, numPlayers);
		break;

	case 2:
		if (loadGame(players, &numPlayers, &currentRound))
		{
			printHands(players, numPlayers);
			startRound(players, deck, numPlayers); // Resume the game
		}
		else
		{
			printf("Failed to load the game. Starting a new game instead.\n");
			printf("1. How many players? (2-4)\n");
			scanf("%d", &numPlayers);
			initalizePlayers(numPlayers);
			printf("Dealing cards...\n");
			initializeDeck(deck, CARDS);
			dealCards(players, deck, numPlayers);
			printHands(players, numPlayers);
			startRound(players, deck, numPlayers);
		}
		break;
	case 3:
		printf("exiting game....\n");
		// exits program
		exit(0);
		break;

	default:
		printf("Error -- closing");
		break;
	}
}

void startRound(Player players[], int deck[], int numPlayers)
{
	int rolledOverPoints = 0; // Points to carry over in case of a tie

	//loop each round until cards rum out
	for (; currentRound <= CARDS; currentRound++)	//semi colon current round already defined globally
	{
		printf("\n===== ROUND %d =====\n", currentRound);

		int playedCards[PLAYERSLIMIT] = { 0 };

		for (int i = 0; i < numPlayers; i++)
		{

			printf("Player %d, choose a card to play: ", players[i].id);
			char input[10];
			int choice = -1; //-1 to validate

			scanf("%s", input);

			// Map face cards to values
			if (strcmp(input, "J") == 0 || strcmp(input, "j") == 0)
				choice = 11;
			else if (strcmp(input, "Q") == 0 || strcmp(input, "q") == 0)
				choice = 12;
			else if (strcmp(input, "K") == 0 || strcmp(input, "k") == 0)
				choice = 13;
			else if (strcmp(input, "A") == 0 || strcmp(input, "a") == 0)
				choice = 14;
			else if (sscanf(input, "%d", &choice) != 1 || choice < 2 || choice > 14)
				choice = -1;

			// Validate choice
			int cardFound = 0;
			for (int j = 0; j < CARDS; j++)
			{
				if (players[i].hand[j] == choice)
				{
					cardFound = 1;
					break;
				}
			}

			if (!cardFound)
			{
				printf("Invalid choice. Try again.\n");
				i--;
				continue;
			}

			// Process valid choice
			int cardRemoved = 0;
			//
			for (int j = 0; j < CARDS; j++)
			{
				if (players[i].hand[j] == choice && !cardRemoved)
				{
					//played cards every cards for everyplayer in that round
					playedCards[i] = players[i].hand[j];
					players[i].hand[j] = 0;
					cardRemoved = 1; //stops from turning all the cards into 0 just first instance  
					break;
				}
			}

			printf("Player %d played %d\n", players[i].id, playedCards[i]);
		}


		int winnerIndex = calcPoints(playedCards, numPlayers);
		int roundPoints = 0;

		for (int i = 0; i < numPlayers; i++)
		{
			roundPoints += playedCards[i]; //adds all the values playes in the card
		}

		//Check for is done in calcpoints function (indicates a tie)
		if (winnerIndex != -1)
		{
			players[winnerIndex].points += roundPoints + rolledOverPoints;
			printf("Player %d wins this round and gets %d points!\n", players[winnerIndex].id, roundPoints + rolledOverPoints);
			rolledOverPoints = 0; // Reset rolled-over points
		}
		else
		{
			rolledOverPoints += roundPoints;
			printf("All cards tied! Points roll over to the next round.\n");
		}

		printf("\n=== Total Points ===\n");
		for (int i = 0; i < numPlayers; i++)
		{
			printf("Player %d: %d points\n", players[i].id, players[i].points);
		}

		printf("\nPlayer Hands After Round %d\n", currentRound);
		for (int i = 0; i < numPlayers; i++)
		{
			printf("Player %d's hand: ", players[i].id);
			for (int j = 0; j < CARDS; j++)
			{
				if (players[i].hand[j] != 0)
					printf("%d ", players[i].hand[j]);
			}
			printf("\n");
		}

		// Save game after each round
		printf("Do you want to save the game? (y/n): ");
		char saveInput;
		scanf(" %c", &saveInput);
		if (saveInput == 'y' || saveInput == 'Y')
		{
			saveGame(players, numPlayers, currentRound);
		}
	}// round for loop ends here after 13 it will end 

	printf("\n=== Evaulation ===\n");
	int maxPoints = 0;
	int winnerIndex = -1;
	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i].points > maxPoints)
		{
			maxPoints = players[i].points;
			winnerIndex = i;
		}
	}

	if (winnerIndex != -1)
	{
		printf("Player %d wins the game with %d points!\n", players[winnerIndex].id, maxPoints);
	}
	else
	{
		printf("It's a tie! No overall winner.\n");
	}
}

int calcPoints(int playedCards[], int numPlayers)
{
	int maxCard = 0;
	int winnerIndex = -1;
	int tieCount = 0; // Track number of players with the max card

	for (int i = 0; i < numPlayers; i++)
	{
		if (playedCards[i] > maxCard)
		{
			maxCard = playedCards[i];
			winnerIndex = i;
			tieCount = 1; // Reset tie count, as there's a new highest card
		}
		else if (playedCards[i] == maxCard)
		{
			tieCount++;
		}
	}

	// If more than one player has the max card, it's a tie
	if (tieCount > 1)
	{
		return -1;
	}

	return winnerIndex; // Return the index of the player with the highest unique card
}

void initalizePlayers(int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		players[i].id = i + 1;			   // Set player ID
		strcpy(players[i].name, "Player"); // Set player name
		players[i].points = 0;			   // Initialize points to 0
		for (int j = 0; j < CARDS; j++)
		{
			players[i].hand[j] = 0;
		}
	}
}
void initializeDeck(int deck[], int deckSize)
{
	for (int i = 0; i < deckSize; i++)
	{
		// add 2 because cards go from 2 to 14, when i = 0, the card is "2", when i = 12, the card is "14"/A
		deck[i] = i + 2;
	}
}

void dealCards(Player players[], int deck[], int numPlayers)
{
	// Keep track of how many cards have been drawn of that value, only allow 4 of each card to be drawn
	int cardsDrawn[CARDS] = { 0 };


	//generates random seed
	srand(time(0));
	for (int playerNum = 0; playerNum < numPlayers; playerNum++)
	{
		// Each player begins with 0 cards before being dealt a hand
		int currentHandSize = 0;
		// get a number between 0 and 12. 
		while (currentHandSize < 13)
		{
			int card = rand() % CARDS;
			if (cardsDrawn[card] < 4)
			{
				// add the card to the player's hand
				players[playerNum].hand[currentHandSize] = deck[card];
				// increase hand size
				currentHandSize = currentHandSize + 1;
				// that's one more card drawn out of the 4 max that can be dealt
				cardsDrawn[card] = cardsDrawn[card] + 1;
			}
		}
	}
}

void printHands(Player players[], int numPlayers)
{
	// Print the hands of all players
	printf("\n==== Player Hands ====\n");
	for (int i = 0; i < numPlayers; i++)
	{
		printf("Player %d (%s)'s hand: ", players[i].id, players[i].name);
		for (int j = 0; j < CARDS; j++)
		{
			int cardValue = players[i].hand[j];

			// Print face cards as J, Q, K, A
			if (cardValue == 11)
			{
				printf("J ");
			}
			else if (cardValue == 12)
			{
				printf("Q ");
			}
			else if (cardValue == 13)
			{
				printf("K ");
			}
			else if (cardValue == 14)
			{
				printf("A ");
			}
			else
			{
				// Print the number value for cards 2-10
				printf("%d ", cardValue);
			}
		}
		printf("\n");
	}
}



//scipt talk
/*
* At the beginning of the program the user introduces three options, 1. start game 2, load game
* 3. exit program, When I press 1, it will prompt to enter 2 to 4 players, if I type it out of the
* integer bounds, then it will trigger an error. After I type the correct amount, we get to see the
* players cards. These cards range from 13 from each of the player and is randomized each time the
* program starts. The 13 cars from the array at the start will add 2 because 2 is the minimum value 
* while ace (14) is the highest. The fucntion will also check that each of the suit (ace, queen)
* maximum is 4. Each time player picks a card, the function will pick up and stores that number but 
* if any player has the highest card, it will check the flag winner and the player wins the match.
* If there's a player thats got 2 same cards, its a tie and the points will cancel. After it, 
* the program will prompt a save function, if hit "Y" it will save called save.txt and it shows
* the players hands in that text file and it will also load on that last match. The loop lasts around
* 13 rounds, after that the flag will check which player has the highest amount of points scored.
* For loading the game, the way its being updated is that it uses a pointer to ensure that we are getting
* its true valuefo everything
*/