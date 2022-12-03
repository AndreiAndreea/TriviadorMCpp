#pragma once
#include <vector>

class Map
{
public: 
	using Coords = std::pair<uint8_t, uint8_t>;

public:
	void SetNumberOfPlayers(uint16_t numberOfPlayers);
	void CreateMap();

	bool IsRegionAvailable(Coords coords);
	void RemoveUnusedRegion(uint8_t position);
	
private:
	uint16_t m_numberOfPlayers;
	uint16_t m_numberOfRounds;

	std::vector<Coords> m_unusedRegions;
};