#include "Map.h"

bool Map::IsRegionAvailable(Coords coords)
{
	uint8_t counter = 0;

	for (auto& pos : m_unusedRegions)
	{
		counter++;

		if (coords == pos)
		{
			RemoveRegion(counter - 1);

			return true;
		}
	}

	return false;
}

void Map::RemoveRegion(uint8_t position)
{
	m_unusedRegions.erase(m_unusedRegions.begin() + position);
}
