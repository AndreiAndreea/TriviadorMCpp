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
		m_mapSize.first = 3;
		m_mapSize.second = 3;
		m_unusedRegions.resize(9);
		m_numberOfRounds = 5;
		break;
	case 3:
		m_mapSize.first = 5;
		m_mapSize.second = 3;
		m_unusedRegions.resize(15);
		m_numberOfRounds = 4;
		break;
	case 4:
		m_mapSize.first = 6;
		m_mapSize.second = 4;
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

std::pair<uint16_t, uint16_t> Map::GetMapSize() const
{
	return m_mapSize;
}
