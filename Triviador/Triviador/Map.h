#pragma once
#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include "ui_Game.h"

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);
		return h1 ^ h2;
	}
};

using Coords = std::pair<qreal, qreal>;
using Unordered_map = std::unordered_map<Coords, std::pair<std::string, int>, pair_hash>;

class Map
{
public:
	//using Coords = std::pair<uint8_t, uint8_t>;
	//using Coords = std::pair<qreal, qreal>;
	//using Unordered_map = std::unordered_map<Coords, std::pair<std::string, int>, pair_hash>;
	
public:
	void SetNumberOfPlayers(uint16_t numberOfPlayers);
	void SetNumberOfRounds(uint16_t numberOfRounds);
	void SetMapSize(uint16_t height, uint16_t width);

	void CreateMap();
	void CreateMapCustomMode(uint16_t mapHeight, uint16_t mapWidth, uint16_t numberOfPlayers, uint16_t numberOfRounds);

	bool IsRegionAvailable(Coords coords);
	//void RemoveUnusedRegion(uint8_t position);
	
	void AddBaseRegion(Coords coords, std::string owner);
	void AddRegion(Coords coords, std::string owner, int score);

	void ModifyRegionAttributes(Coords coords, std::string newOwner, int newScore);
	
public:
	uint16_t GetNumberOfPlayers() const;
	std::pair<uint16_t, uint16_t> GetMapSize() const;
	
	int GetNumberOfChosenBases() const;
	Unordered_map GetUsedRegions() const;
	Coords GetBaseRegion(std::string owner) const;
	
private:
	uint16_t m_numberOfPlayers;
	uint16_t m_numberOfRounds;
	std::pair<uint16_t, uint16_t> m_mapSize;

	//std::vector<Coords> m_unusedRegions; - in loc sa cautam in regiunile nefolosite
	Unordered_map m_usedRegions; //avem o mapa cu regiunile utilizate, cand se alege o baza/teritoriu se va insera
	Unordered_map m_bases;
};