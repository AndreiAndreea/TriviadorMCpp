#pragma once
#include <vector>

class Map
{
public: 
	using Coords = std::pair<uint8_t, uint8_t>;

public:
	bool IsRegionAvailable(Coords coords);
	void RemoveUnusedRegion(uint8_t position);
	
private:
	std::vector<Coords> m_unusedRegions;
};