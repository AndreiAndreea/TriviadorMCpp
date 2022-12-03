#include "Map.h"

void Map::SetNumberOfPlayers(uint16_t numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;
}

void Map::CreateMap()
{
	switch (m_numberOfPlayers)
	{
	case 2:
		m_unusedRegions.resize(9);
		m_numberOfRounds = 5;
		break;
	case 3:
		m_unusedRegions.resize(15);
		m_numberOfRounds = 4;
		break;
	case 4:
		m_unusedRegions.resize(24);
		m_numberOfRounds = 5;
		break;
	//default:
		//break;
	}
}

bool Map::IsRegionAvailable(Coords coords)
{
	uint8_t counter = 0;

	for (auto& pos : m_unusedRegions)
	{
		counter++;

		if (coords == pos)
		{
			RemoveUnusedRegion(counter - 1);

			return true;
		}
	}

	return false;
}

void Map::RemoveUnusedRegion(uint8_t position)
{
	m_unusedRegions.erase(m_unusedRegions.begin() + position);
}
