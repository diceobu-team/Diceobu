/* This file contains the definitions
 * of functions declared in the mainDependancies
 * header file.
 */

// User Libraries
#include "GlobalVariables.h"
#include "DiceobuLibrary.h"
#include "mainDependancies.h"
//	Standard Libraries
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <iterator>

//	Global Structures
static std::list<Map*> activeMaps;

static std::list<Character*> activeCharacters;

Map* currWorkingMap;

Character* currWorkingChar;


void clearScreen()
{
	std::cout << "\033[2J\033[1;1H";
}

void displayWelcomeMessage()
{
	std::cout << "Welcome to Dfireobu also known as Diceobu Jr, or Diceobu's younger sibling.\n"
		<< "Dfireobu is not a complete game.\n"
		<< "It's just a platform that will guide you through the basic features Diceobu will have to offer.\n"
		<< "Most of the available options will be automated in the final product.\n"
		<< "Here you can take control of them to freely explore and test their behavior.\n"
		<< "Fullscreen view is recommended.\n";
}

void displayAvailableOptions()
{
	std::cout << "List of available options (select one of the numbers/symbols and hit enter):\n";
	std::cout << "1. Create new Default Map\n"
		<< "2. Create new Default Character\n"
		<< "3. Move existing character\n"
		<< "4. Start/End combat\n"
		<< "5. Display help information\n"
		<< "6. Display all active Maps\n"
		<< "7. Display all active Characters\n"
		<< "8. Delete first active Map\n"
		<< "9. Delete first active Character\n"
		<< "x. Exit\n";
	if (!activeMaps.empty())	std::cout << "Current Map: " << currWorkingMap->getMapID() << '\t';
	if (!activeCharacters.empty())	std::cout << "Current Character: " << currWorkingChar->getEntityID() << '\n';
}

void displayFeedbackMessage(std::string message)
{
	std::cout << '\n' << '\t' << '\t' << '\t'
		<< message << '\n';
}

void writeActiveMaps()
{
	std::list <Map*> ::iterator iter;
	for (iter = activeMaps.begin(); iter != activeMaps.end(); iter++)
	{
		Map* currMap = *iter;
		currMap->writeMap();
	}
}

void displayActiveMaps()
{
	std::list <Map*> ::iterator iter;
	for (iter = activeMaps.begin(); iter != activeMaps.end(); iter++)
	{
		std::cout << '\n';
		Map* currMap = *iter;
		currMap->printMap();
		std::cout << '\n';
	}
}

void displayActiveCharacters()
{
	std::list <Character*> ::iterator iter;
	for (iter = activeCharacters.begin(); iter != activeCharacters.end(); iter++)
	{
		std::cout << '\n';
		Character* currCharacter = *iter;
		currCharacter->printCharacter();
		std::cout << '\n';
	}
}

void createNewMap()
{
	activeMaps.push_back(new Map("Castle of Belithriell", mapSize, mapSize, mapIDCounter++, "none"));
	currWorkingMap = activeMaps.back();
}

void deleteFirstActiveMap()
{
	Map *tempMap{ activeMaps.front() };
	activeMaps.pop_front();
	tempMap->~Map();
}

void createNewCharacter(Map* &currMap)
{
	activeCharacters.push_back(new Character("jeff", 50, 30, 20, "large", 24, 22, characterIDCounter++, { 0, 0 }, currMap, 11, "powers", 2, "fighter",
		"lawful trash", "some vest", 20, 50000, "human", "some langs", 0, "tourash", "none", -1, -5));
	currWorkingChar = activeCharacters.back();
	currMap->m_containingCharacters.push_back(currWorkingChar->getEntityID());
}

void deleteFirstActiveCharacter()
{
	Character *tempChar{ activeCharacters.front() };
	tempChar->getCurrMap()->m_containingCharacters.remove(tempChar->getEntityID());
	activeCharacters.pop_front();
	tempChar->~Character();
}

void displayInfo()
{
	std::cout << "Information\n\n";
	std::cout << "Current Map shows the Map that your changes will affect.\n"
		<< "Current Character shows the Character that you changes will affect.\n"
		<< "You can work only with one Character/Map at a time\n";
	std::cout << '\n';
}

std::string getUserOption()
{
	std::cout << "Awaiting input (whitespace characters are ignored): ";
	std::string input{};
	std::cin >> input;
	return input;
}

void simLaunch()
{
	clearScreen();
	displayAvailableOptions();
	std::string input{};
	while (1)
	{
		input = getUserOption();
		clearScreen();
		displayAvailableOptions();
		if (input == "1")
		{
			displayFeedbackMessage("Creating map");
			createNewMap();
			clearScreen();
			displayAvailableOptions();
			displayFeedbackMessage("New map created");
		}
		else if (input == "2")
		{
			if (activeMaps.empty())
			{
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("Cannot create character without a map");
			}
			else
			{
				displayFeedbackMessage("Creating new character");
				createNewCharacter(activeMaps.back());
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("New character created");
			}
		}
		else if (input == "3")
		{
			if (activeCharacters.empty())
			{
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("No character found");
			}
			else
			{
				//	Create new function for this
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("Type new coordinates (format: x,y) or press x to cancel");
				input = getUserOption();
				if (input == "x")
				{
					displayFeedbackMessage("Canceling character movement");
				}
				else
				{
					displayFeedbackMessage("Moving character");
					std::string delimiter = ",";
					size_t pos = 0;
					std::string token;
					while ((pos = input.find(delimiter)) != std::string::npos) {
						token = input.substr(0, pos);
						input.erase(0, pos + delimiter.length());
					}
					std::stringstream coordX{ token }, coordY{ input };
					std::pair<int, int> coords{};
					coordX >> coords.first;
					coordY >> coords.second;
					Character* tempChar{ activeCharacters.back() };
					tempChar->changeEntityPosition(activeMaps.back(), coords);
				}
				//	Create new function for this

				clearScreen();
				displayAvailableOptions();
			}
		}
		else if (input == "4")
		{
			if (inCombat)
			{
				displayFeedbackMessage("Ending combat");
				inCombat = false;
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("Combat ended");
			}
			else
			{
				displayFeedbackMessage("Starting combat");
				inCombat = true;
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("Combat started");
			}
		}
		else if (input == "5")
		{
			displayFeedbackMessage("Displaying additional information");
			clearScreen();
			displayInfo();
			displayAvailableOptions();
		}
		else if (input == "6")
		{
			displayFeedbackMessage("Displaying all active maps");
			clearScreen();
			if (activeMaps.empty())
			{
				displayAvailableOptions();
				displayFeedbackMessage("Nothing to display");
			}
			else
			{
				displayActiveMaps();
				writeActiveMaps();
				displayAvailableOptions();
			}
		}
		else if (input == "7")
		{
			displayFeedbackMessage("Displaying all active characters");
			clearScreen();
			if (activeCharacters.empty())
			{
				displayAvailableOptions();
				displayFeedbackMessage("Nothing to display");
			}
			else
			{
				displayActiveCharacters();
				displayAvailableOptions();
			}
		}
		else if (input == "8")
		{
			if (activeMaps.empty())
			{
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("Nothing to delete");
			}
			else
			{
				displayFeedbackMessage("Deleting first active map");
				if (activeMaps.front()->m_containingCharacters.empty())
				{
					deleteFirstActiveMap();
					clearScreen();
					displayAvailableOptions();
					displayFeedbackMessage("First active map deleted");
				}
				else
				{
					clearScreen();
					displayAvailableOptions();
					displayFeedbackMessage("Cannot delete map containing characters");
				}
			}
		}
		else if (input == "9")
		{
			if (activeCharacters.empty())
			{
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("Nothing to delete");
			}
			else
			{
				displayFeedbackMessage("Deleting first active character");
				deleteFirstActiveCharacter();
				clearScreen();
				displayAvailableOptions();
				displayFeedbackMessage("First active character deleted");
			}
		}
		else if (input == "x")
		{
			displayFeedbackMessage("Exiting");
			activeMaps.clear();
			activeCharacters.clear();
			clearScreen();
			displayFeedbackMessage("Application exited");
			break;
		}
		else
		{
			displayFeedbackMessage("Unrecognized command");
		}
	}
}