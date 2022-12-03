#pragma once
#include <vector>

#include <QPainter>
#include <QPaintEvent>

class Map
{
public: 
	using Coords = std::pair<uint8_t, uint8_t>;

public:
	void SetNumberOfPlayers(uint16_t numberOfPlayers);
	void CreateMap();

	bool IsRegionAvailable(Coords coords);
	void RemoveUnusedRegion(uint8_t position);

public:
	std::pair<uint16_t, uint16_t> GetMapSize() const;
	
private:
	uint16_t m_numberOfPlayers;
	uint16_t m_numberOfRounds;
	std::pair<uint16_t, uint16_t> m_mapSize;

	std::vector<Coords> m_unusedRegions;
};