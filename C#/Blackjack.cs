using System;
using static System.Runtime.InteropServices.JavaScript.JSType;

// Config

int DeckCount = 1;

// Main

int IntArrayElementCount(int[] Array)
{
    int count = 0;

    foreach (var Element in Array)
    {
        if (Element != -1)
        {
            count++;
        }
    }

    return count;
}

int[][] Cards = new int[DeckCount * 52][];

for (int Deck = 1; Deck <= DeckCount; Deck++)
{
    for (int Suit = 0; Suit < 4; Suit++)
    {
        for (int Rank = 0; Rank < 13; Rank++)
        {
            int[] CardData = { Suit + 1, Rank + 1 };
            Cards[((Deck - 1) * 52) + (Suit * 13) + Rank] = CardData;
        }
    }
}

int PrintCard(int CardID)
{
    int[] CardData = Cards[CardID];

    string Card = "";

    switch (CardData[0])
    {
        case 1:
            Card = "♠";
            break;
        case 2:
            Console.ForegroundColor = ConsoleColor.Red;
            Card = "♥";
            break;
        case 3:
            Card = "♣";
            break;
        case 4:
            Console.ForegroundColor = ConsoleColor.Red;
            Card = "♦";
            break;
    }

    switch (CardData[1])
    {
        case 1:
            Card = Card + " A";
            break;
        case 11:
            Card = Card + " J";
            break;
        case 12:
            Card = Card + " Q";
            break;
        case 13:
            Card = Card + " K";
            break;
        default:
            Card = Card + " " + CardData[1];
            break;
    }

    Console.Write(Card);
    Console.ResetColor();

    return 0;
}

Random random = new Random();

int[] DealerCardIDs = new int[22];
int[] PlayerCardIDs = new int[22];

void ResetCards()
{
    for (int i = 0; i < DealerCardIDs.Length; i++)
    {
        DealerCardIDs[i] = -1;
    }

    for (int i = 0; i < PlayerCardIDs.Length; i++)
    {
        PlayerCardIDs[i] = -1;
    }
}

int TakeRandomCard()
{
    bool CardFound = false;
    int CardID = 0;

    while (!CardFound)
    {
        CardID = random.Next(1, DeckCount * 52);

        if (!(Array.Exists(DealerCardIDs, item => item == CardID) || Array.Exists(PlayerCardIDs, item => item == CardID)))
        {
            CardFound = true;
        }
    }

    return CardID;
}

int CalculateHighestSafeValue(int[] CardArray)
{
    int CardCount = IntArrayElementCount(CardArray);
    int AceCount = 0;

    int RealValue = 0;

    for (int i = 0; i < CardCount; i++)
    {
        int[] CardData = Cards[CardArray[i]];
        int Rank = CardData[1];

        if (Rank > 10)
        {
            RealValue = RealValue + 10;
        }
        else if (Rank == 1)
        {
            AceCount++;
        }
        else
        {
            RealValue = RealValue + Rank;
        }
    }

    for (int i = 0; i < AceCount; i++)
    {
        if ((RealValue + 11) <= 21)
        {
            RealValue = RealValue + 11;
        }
        else
        {
            RealValue = RealValue + 1;
        }
    }

    return RealValue;
}

for (int CardIndex = 0; CardIndex < 52; CardIndex++)
{
    PrintCard(CardIndex);

    Console.Write("\n");
}

Console.Write("\nRules:\n * Ace: Ace cards (A) can either be a hard 1 or a soft 11 depending on the value of the hand.\n * Initial Deal: The dealer takes 2 random cards, one is shown and one is hidden, afterwards 2 cards are given to the player.\n * Player Input: The player may hit (h) to get a random card from the deck or stand (s) to begin comparing their cards against the dealer.\n * Player Bust: If the player's hand exceeds 21, the player busts and lose their bet.\n * Dealer Input: When the player stands, the dealer will take a random card from the deck until his hand adds up to a soft 17.\n * Dealer Bust: If the dealer's hand goes over 21, the dealer busts and the player wins.\n * Winning: Player wins if their hand total is higher than the dealer's hand total without exceeding 21.\n * Push: If the player's hand total equals the dealer's hand total, it results in a push and the bet is returned without any win or loss.\n * Natural Blackjack: If the player's hand is exactly 21 with only 2 cards, it is called a blackjack and they immediately win; Unless the dealer also has a blackjack in which case it is a push.");

bool playing = true;

Console.Write("\n\nTotal Money: $");
double money = Convert.ToDouble(Console.ReadLine());

while (playing)
{
    ResetCards();

    bool InitialDeal = true;
    bool hit = false;
    bool stand = false;
    bool run = true;

    bool validBet = false;
    double bet = 0;

    if (money <= 0)
    {
        Console.Write("\n\nNew Money: $");
        money = Convert.ToDouble(Console.ReadLine());
    }

    Console.Write("\n\nMoney: $" + money);

    while (!validBet)
    {
        Console.Write("\n\nNew Bet: $");
        bet = Convert.ToDouble(Console.ReadLine());

        if (bet < 0)
        {
            Console.WriteLine("Bet can't be less than 0.");
        }
        else if (money - bet < 0)
        {
            Console.WriteLine("Can't bet more money than you have.");
        }
        else
        {
            validBet = true;
        }
    }

    while (run)
    {
        Console.Clear();
        Console.Write("\nBet: $" + bet);

        if (InitialDeal) // TakeRandomCard()
        {
            DealerCardIDs[0] = TakeRandomCard();
            DealerCardIDs[1] = TakeRandomCard();

            PlayerCardIDs[0] = TakeRandomCard();
            PlayerCardIDs[1] = TakeRandomCard();
        }

        // Dealer Data

        int DealerCardCount = IntArrayElementCount(DealerCardIDs);
        int DealerHandValue = CalculateHighestSafeValue(DealerCardIDs);

        // Player Action

        if (hit)
        {
            Console.Write("\n\nHit");
            hit = false;
            PlayerCardIDs[IntArrayElementCount(PlayerCardIDs)] = TakeRandomCard();
        }
        else if (stand)
        {
            Console.Write("\n\nStand");
        }

        // Player Data

        int PlayerCardCount = IntArrayElementCount(PlayerCardIDs);
        int PlayerHandValue = CalculateHighestSafeValue(PlayerCardIDs);

        // Player Bust

        if (PlayerHandValue > 21)
        {
            Console.Write("\n\nPlayer Bust!\n\n");
            money -= bet;
            run = false;
        }

        // Dealer Print

        Console.Write("\n\nDealer:\n");

        if (stand || !run)
        {
            for (int CardIndex = 0; CardIndex < DealerCardCount; CardIndex++)
            {
                PrintCard(DealerCardIDs[CardIndex]);

                if (!(CardIndex == DealerCardCount - 1))
                {
                    Console.Write(" + ");
                }
            }

            Console.Write(" = " + DealerHandValue);
        }
        else
        {
            PrintCard(DealerCardIDs[0]);

            Console.Write(" + ?");
        }

        // Player Print

        Console.Write("\n\nPlayer:\n");

        for (int CardIndex = 0; CardIndex < PlayerCardCount; CardIndex++)
        {
            PrintCard(PlayerCardIDs[CardIndex]);

            if (!(CardIndex == PlayerCardCount - 1))
            {
                Console.Write(" + ");
            }
        }

        Console.Write(" = " + PlayerHandValue);

        // Player Stand

        if (stand)
        {
            if (PlayerCardCount == 2 && PlayerHandValue == 21) // Natural Blackjack
            {
                if (DealerHandValue == 21)
                {
                    Console.Write("\n\nPush!\n\n");
                    run = false;
                }
                else
                {
                    Console.Write("\n\nBlackjack!\n\n");
                    money += bet * 1.5;
                    run = false;
                }
            }
            else // Normal
            {
                if (DealerHandValue < 17)
                {
                    Thread.Sleep(1000);
                    DealerCardIDs[DealerCardCount] = TakeRandomCard();
                }
                else if (DealerHandValue > 21)
                {
                    Console.Write("\n\nDealer Bust!\n\n");
                    money += bet;
                    run = false;
                }
                else if (DealerHandValue == PlayerHandValue)
                {
                    Console.Write("\n\nPush!\n\n");
                    run = false;
                }
                else if (DealerHandValue < PlayerHandValue)
                {
                    Console.Write("\n\nYou win!\n\n");
                    money += bet;
                    run = false;
                }
                else if (DealerHandValue > PlayerHandValue)
                {
                    Console.Write("\n\nYou lose!\n\n");
                    money -= bet;
                    run = false;
                }
            }
        }

        // Natural Blackjack Rule

        if (InitialDeal)
        {
            if (PlayerHandValue == 21)
            {
                stand = true;
            }

            InitialDeal = false;
        }

        // Input

        if (!stand && run)
        {
            string Key = Console.ReadKey().KeyChar.ToString().ToLower();

            if (Key == "h")
            {
                hit = true;
            }
            else if (Key == "s")
            {
                stand = true;
            }
            else if (Key == "q")
            {
                playing = false;
                run = false;
            }
        }
    }
}
