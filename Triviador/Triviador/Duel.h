#pragma once
#include "Player.h"
class Duel
{
public:
	Duel(Player player1, Player player2);

	void SetWinner(Player& winner);
	const Player GetWinner() const;

	void TransferTerritory(Player& winner, Player& loser);
	
private:
	Player m_player1;
	Player m_player2;
	Player m_winner;
};

