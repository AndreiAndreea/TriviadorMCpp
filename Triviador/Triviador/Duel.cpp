#include "Duel.h"

Duel::Duel(Player& player1, Player& player2)
	:m_player1(player1), m_player2(player2)
{

}

void Duel::SetWinner(Player& winner)
{
	m_winner = winner;
}

const Player& Duel::GetWinner() const
{
	return m_winner;
}

void Duel::TransferTerritoryFromPlayer2ToPlayer1(Player& winner, Player& loser)
{
	winner.AddTerritory(loser.GetTerritory());

	loser.RemoveTerritory();
}
