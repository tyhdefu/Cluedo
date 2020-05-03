// 0 - None - including final output
// 1 - Only final output
// 2 - Moderate - Recommended with human players
// 3 - ALL

#include "pch.h"
#include <iostream>
#include <time.h>
#include "GameClass.h"
#include "SmartPlayer.h"
#include "RealPlayer.h"

int main()
{	
	clock_t start = clock();
	
	// previously used srand(3)
	srand(0);

	
	// Initialise game with loglevel
	Game myGame(0);

	SmartPlayer* p1 = new SmartPlayer();
	//SmartPlayer* p2 = new SmartPlayer();
	//SmartPlayer* p3 = new SmartPlayer();

	std::shared_ptr<Player> player1(p1);
	std::shared_ptr<Player> player2(new Player);
	std::shared_ptr<Player> player3(new Player);

	std::vector<std::shared_ptr<Player>> players;
	players.push_back(player1);
	players.push_back(player2);
	players.push_back(player3);
	players.resize(5);

	std::cout << std::endl;

	myGame.SetPlayers(players);

	std::cout << myGame.players << " players" << std::endl;

	for (int i = 0; i < 1000; i++)
	{
		myGame.DealCards();
		
		/*std::cout << "player 1" << std::endl;
		BasicNotes::printnote(player1->notes);
		std::cout << "player 2" << std::endl;
		BasicNotes::printnote(p2->notes);
		std::cout << "player 3" << std::endl;
		BasicNotes::printnote(player3->notes);*/
		
		myGame.StartGame();

		/*p2->smartnotes.printShownCards();
		p2->smartnotes.printKnownCards();
		p2->smartnotes.printSuspectedCards();
		p2->smartnotes.printNotOwnedCards();*/

		myGame.ResetGame();
	}
	std::cout << std::endl << std::endl << std::endl;

	std::cout << "Player 1 win count: " << myGame.Players[0]->wins << std::endl;
	std::cout << "Player 2 win count: " << myGame.Players[1]->wins << std::endl;
	std::cout << "Player 3 win count: " << myGame.Players[2]->wins << std::endl;




	clock_t endtime = clock();

	clock_t elapsedtime = (endtime - start);

	std::cout << elapsedtime << " miliseconds" << std::endl;

	exit(0);
}