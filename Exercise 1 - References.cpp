#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;



// Ex 1-1
int placeBetReturn(int& playerGold);
void placeBetVoid(int& playerGold, int& currentBet);

// Ex 1-2
void rollDice(int& die1, int& die2);

// Ex 1-3
void evaluatePayout(int& playerGold, int playerBet, int pDie1, int pDie2, int aiSum);

// Ex 1-4
void playRound(int& playerGold);


int main() {
    srand(time(0));

    // Starting moneh
    int playerGold = 1000;

    // Intro
    cout << "                                                              --------------------------\n";
    cout << "                                                               WELCOME TO THE DICE GAME!       \n";
    cout << "                                                              --------------------------\n";
    cout << "\n";
    cout << "                                                               You start with " << playerGold << " gold.\n\n";

    //Loop until player loses all his moolah
    while (playerGold > 0) {
        playRound(playerGold);
        cout << "----------------------------------------\n";
    }

    cout << "\nGame Over! You have run out of gold.\n";
    cout << "Thank you for playing!\n";

    return 0;
}

// Returns the bet value
int placeBetReturn(int& playerGold) {
    int bet = 0;
    while (true) {
        cout << "Current Gold: " << playerGold << "\n";
        cout << "Enter your bet: ";
        cin >> bet;

        if (bet > 0 && bet <= playerGold) {
            playerGold -= bet;
            return bet;
        }
        cout << "Invalid bet! You must bet between 1 and " << playerGold << " gold.\n\n";
    }
}

// Returns as void
void placeBetVoid(int& playerGold, int& currentBet) {
    while (true) {
        cout << "Current Gold: " << playerGold << "\n";
        cout << "Enter your bet: ";
        cin >> currentBet;

        if (currentBet > 0 && currentBet <= playerGold) {
            playerGold -= currentBet;
            return; // Exit function once valid bet is made
        }
        cout << "Invalid bet! You must bet between 1 and " << playerGold << " gold.\n\n";
    }
}

// Rolls 2 dice and passes the value to references
void rollDice(int& die1, int& die2) {
    die1 = (rand() % 6) + 1;
    die2 = (rand() % 6) + 1;
}

// Reviewws the outcome and modifies the player's gold
void evaluatePayout(int& playerGold, int playerBet, int pDie1, int pDie2, int aiSum) {
    int playerSum = pDie1 + pDie2;

    cout << "\nResults:\n";
    cout << "AI Total: " << aiSum << "\n";
    cout << "Player Total: " << playerSum;

    // Checks for snake eyes
    if (pDie1 == 1 && pDie2 == 1) {
        cout << " (SNAKE EYES!)";
    }
    cout << "\n\n";

    // Draw
    if (playerSum == aiSum) {
        cout << "It's a draw! Your bet is returned.\n";
        playerGold += playerBet;
    }
    // Player rols a snake eyes and AI doesn't matches
    else if (pDie1 == 1 && pDie2 == 1) {
        cout << "Amazing! Snake Eyes! You win thrice your bet!\n";
        playerGold += (playerBet * 3);
    }
    // Player roll is higher than AI
    else if (playerSum > aiSum) {
        cout << "You win! You double your bet.\n";
        playerGold += (playerBet * 2);
    }
    // Player loses
    else {
        cout << "AI wins! You lost your bet.\n";
    }
}

// starts the round
void playRound(int& playerGold) {
    int currentBet = 0;

    // get bet if valid
    placeBetVoid(playerGold, currentBet);
    cout << "Bet of " << currentBet << " accepted.\n\n";

    // Outcome of the AI roll
    int aiDie1 = 0, aiDie2 = 0;
    rollDice(aiDie1, aiDie2);
    int aiSum = aiDie1 + aiDie2;
    cout << "AI has rolled the dice...\n";

    // Press enter to see outcomes of player rolls
    int pDie1 = 0, pDie2 = 0;
    cout << "Press Enter to roll your dice...";
    cin.ignore(); 
    cin.get();    

    rollDice(pDie1, pDie2);
    cout << "You rolled a " << pDie1 << " and a " << pDie2 << ".\n";

    // Calculates and distribute payout
    evaluatePayout(playerGold, currentBet, pDie1, pDie2, aiSum);
}
