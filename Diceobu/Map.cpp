/* This is the Map class source file.
 * It contains the definition of the
 * Map class Constructor and other
 * Member Functions.
 */

//	User Libraries
#include "Map.h"
#include "Tile.h"
#include "GameData.h"
#include "GlobalVariables.h"
//	Standard Libraries
#include <string>
#include <iostream>
#include <array>

//	Constructors
Map::Map(const std::string	&mapName,	const int			&sizeX,			const int	&sizeY,
		 const int			&mapID,		const std::string	&mapEffects)
	:	m_mapName{ mapName },	m_sizeX{ sizeX },			m_sizeY{sizeY},
		m_mapID{ mapID },		m_mapEffects{ mapEffects }
{
	if (mapID == 0)
	{
		initTileDataCont();
	}
	initializeMapTiles();
}

//	Others
void Map::initializeMapTiles()
{
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			m_tileGrid[i][j] = new Tile(tileIDCounter++, tileDataContainer[i][j].tld_terrainType,
				tileDataContainer[i][j].tld_tileEffects,
				tileDataContainer[i][j].tld_open,
				tileDataContainer[i][j].tld_occupied,
				tileDataContainer[i][j].tld_occupantID);
		}
	}
}

char Map::getTileSymbol(Tile &currTile)
{
	char tileSymbol{};
	if (currTile.getOpen())
	{
		if (currTile.getOccupied())
		{
			tileSymbol = 'x';
		}
		else
		{
			if (currTile.getTileEffects() == "none")
			{
				if (currTile.getTerrainType() == "dirt")		tileSymbol = '~';
				else if (currTile.getTerrainType() == "stone")	tileSymbol = 'O';
			}
			else if (currTile.getTileEffects() == "fire")		tileSymbol = '@';
			else if (currTile.getTileEffects() == "ice")		tileSymbol = '*';
		}
	}
	else
	{
		tileSymbol = ' ';
	}
	return tileSymbol;
}

void Map::printMap()
{
	std::cout << '\n' << "Map: " << m_mapName
		<< '\t' << "MapID: " << m_mapID << '\n';
	printMapCharacters();
	for (int i = 0; i <= mapSize+1; i++) std::cout << '#';
	std::cout << '\n';
	for (int i = 0; i < mapSize; i++)
	{
		std::cout << "#";
		for (int j = 0; j < mapSize; j++)
		{
			std::cout << getTileSymbol(*m_tileGrid[i][j]);
		}
		std::cout << "#";
		std::cout << '\n';
	}
	for (int i = 0; i <= mapSize + 1; i++) std::cout << '#';
}

void Map::printMapCharacters()
{
	if (m_containingCharacters.empty())
	{
		std::cout << "No characters reside in this map\n";
	}
	else
	{
		std::cout << "Characters with IDs: ";
		std::list <int> :: iterator iter;
		for (iter = m_containingCharacters.begin(); iter != m_containingCharacters.end(); iter++)
		{
			std::cout << *iter << ' ';
		}
		std::cout << "reside in this map\n";
	}
}