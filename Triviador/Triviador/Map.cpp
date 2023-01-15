#include "Map.h"

void Map::SetNumberOfPlayers(uint16_t numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;
}

void Map::SetNumberOfRounds(uint16_t numberOfRounds)
{
	m_numberOfRounds = numberOfRounds;
}

void Map::SetMapSize(uint16_t height, uint16_t width)
{
	m_mapSize.first = height;
	m_mapSize.second = width;
}

void Map::CreateMap()
{
	switch (m_numberOfPlayers)
	{
		case 2:
			m_mapSize.first = 3;
			m_mapSize.second = 3;
			m_numberOfRounds = 5;
			break;
		case 3:
			m_mapSize.first = 5;
			m_mapSize.second = 3;
			m_numberOfRounds = 4;
			break;
		case 4:
			m_mapSize.first = 6;
			m_mapSize.second = 4;
			m_numberOfRounds = 5;
			break;
	}
	
}

void Map::CreateMapCustomMode(uint16_t mapHeight, uint16_t mapWidth, uint16_t numberOfPlayers, uint16_t numberOfRounds)
{
	m_mapSize.first = mapHeight;
	m_mapSize.second = mapWidth;
	m_numberOfRounds = numberOfRounds;
}

bool Map::IsRegionAvailable(Coords coords)
{
	if (m_usedRegions.find(coords) != m_usedRegions.end())
		return false;
	return true;
}

void Map::AddBaseRegion(Coords coords, std::string owner)
{
	m_bases.insert(std::make_pair(coords, std::make_pair(owner, 300)));
}

void Map::AddRegion(Coords coords, std::string owner, int score)
{
	m_usedRegions.insert(std::make_pair(coords, std::make_pair(owner, score)));
}

void Map::ModifyRegionAttributes(Coords coords, std::string newOwner, int newScore)
{
	m_usedRegions.find(coords)->second.first = newOwner;
	m_usedRegions.find(coords)->second.second = newScore;
}

uint16_t Map::GetNumberOfPlayers() const
{
	return m_numberOfPlayers;
}

std::pair<uint16_t, uint16_t> Map::GetMapSize() const
{
	return m_mapSize;
}

int Map::GetNumberOfChosenBases() const
{
	return m_bases.size();
}

Unordered_map Map::GetUsedRegions() const
{
	return m_usedRegions;
}

Coords Map::GetBaseRegion(std::string owner) const
{
	for (auto& region : m_usedRegions)
		if (region.second.first == owner)
			return region.first;
}
