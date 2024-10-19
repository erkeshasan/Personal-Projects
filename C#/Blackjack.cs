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

for (int i = 0; i < DealerCardIDs.Length; i++)
{
    DealerCardIDs[i] = -1;
}

for (int i = 0; i < PlayerCardIDs.Length; i++)
{
    PlayerCardIDs[i] = -1;
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

bool dealt = false;
bool hit = false;
bool natural = false;
bool stand = false;
bool run = true;

while (run)
{
    if (!dealt)
    {
        Console.Clear();

        PlayerCardIDs[0] = TakeRandomCard();
        PlayerCardIDs[1] = TakeRandomCard();

        DealerCardIDs[0] = TakeRandomCard();
        DealerCardIDs[1] = TakeRandomCard();
    }

    // Dealer Data

    int DealerCardCount = IntArrayElementCount(DealerCardIDs);
    int DealerHandValue = CalculateHighestSafeValue(DealerCardIDs);

    // Player Data

    int PlayerCardCount = IntArrayElementCount(PlayerCardIDs);
    int PlayerHandValue = CalculateHighestSafeValue(PlayerCardIDs);

    // Natural Blackjack

    if (!dealt)
    {
        if (PlayerHandValue == 21)
        {
            natural = true;
        }

        dealt = true;
    }

    // Dealer Print

    Console.Write("Dealer:\n");

    if (stand || natural)
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

    // Player Action

    if (hit)
    {
        Console.Write("\n\nHit");
        hit = false;
        PlayerCardIDs[PlayerCardCount] = TakeRandomCard();
    }
    else if (stand)
    {
        Console.Write("\n\nStand");
    }
    else if (natural)
    {
        Console.Write("\n\nNatural");
    }

    Console.Write("\n\n");

    // Player Print

    Console.Write("Player:\n");

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
        DealerCardIDs[DealerCardCount] = TakeRandomCard();

        int DealerValue = CalculateHighestSafeValue(DealerCardIDs);

        if (DealerValue > 21)
        {
            Console.Write("\n\nDealer Bust!\n\n");
            run = false;
        } else if (DealerValue == PlayerHandValue)
        {
            Console.Write("\n\nPush!\n\n");
            run = false;
        }
        else if (DealerValue >= 17 && DealerValue < PlayerHandValue)
        {
            Console.Write("\n\nwin\n\n");
            run = false;
        }
    }

    // Player Bust

    if (PlayerHandValue > 21)
    {
        Console.Write("\n\nBust!\n\n");
        run = false;
    }

    // Natural

    if (natural)
    {
        if (DealerHandValue == 21)
        {
            Console.Write("\n\nPush!\n\n");
            run = false;
        }
        else
        {
            Console.Write("\n\nBlackjack!\n\n");
            run = false;
        }
    }

    // Input

    if (!stand && run && !natural)
    {
        string Key = Console.ReadKey().KeyChar.ToString().ToLower();
        Console.Clear();

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
            run = false;
        }
    }
}
