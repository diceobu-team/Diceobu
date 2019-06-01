// User Libraries
#include "GlobalVariables.h"
#include "DiceobuLibrary.h"
#include "mainDependancies.h"
#include "lobbywindow.h"
//	Standard Libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <iterator>
//  Qt Libraries
#include <QDebug>
//	Global Structures
#include "charactersaves.h"

#include <sstream>
#include <string>
#include <fstream>
#include <QStringList>

int loadPlayerCharacter(std::string saveFilepath){

    std::ifstream infile(saveFilepath);
    std::string line;
    std::list <std::string> tokenList;
    std::list <std::string> lineList;
    std::string token;
    std::string previousToken = "reserved";
    QStringList tempQSL;

    int coordX = 0;
    int coordY = 0;

    int returnV = 1;

    while (std::getline(infile, line))
    {
        lineList.push_back(line);               // Get each line in lineList
    }


    std::list <std::string> :: iterator iter2;
    for(iter2 = lineList.begin(); iter2 != lineList.end(); iter2++)
    {
        token = *iter2;
        tempQSL = QString::fromStdString(token).split(" = ");               // Split each line by " = " and push tokens in tokenList
        for (int i=0; i<tempQSL.size();i++)
        {
            tokenList.push_back(tempQSL.at(i).toStdString());
        }
    }


    for(iter2 = tokenList.begin(); iter2 != tokenList.end(); iter2++) // Get spawn coords
    {
        token = *iter2;
        if (previousToken == "reserved") // token has a tag
        {
            previousToken = token;
        }
        else                            // token has a value
        {
            if (previousToken == "Coordinate X")
            {
                coordX = std::stoi(token);

            }
            else if (previousToken == "Coordinate Y")
            {
                coordY = std::stoi(token);

            }
            previousToken = "reserved";
        }
    }

    if (coordX > 49 || coordX < 0 || coordY > 49 || coordY < 0 )
    {
        emit mui->errorMessage(6);
        return 1;
    }
    else
    {
        if (currWorkingMap->m_tileGrid[coordX][coordY]->getOccupied() || !currWorkingMap->m_tileGrid[coordX][coordY]->getOpen())
        {
            emit mui->errorMessage(7);
            return 1;
        }
    }

    muteLog = 1;
    diceobuSystemCore("2",coordX,coordY);
    currWorkingChar->getPowers()->clear();
    muteLog = 0;


    std::list <std::string> :: iterator iter;                               // Parse tokenList of form Tag, Value, Tag, Value...
    for(iter = tokenList.begin(); iter != tokenList.end(); iter++)
    {
        token = *iter;
        if (previousToken == "reserved") // token has a tag
        {
            previousToken = token;
        }
        else                            // token has a value
        {
            if (activeCharactersisEmpty()){ break;}

            if (previousToken == "Name")
            {
                currWorkingChar->setName(token);
            }
			else if (previousToken == "Gender")
            {
                currWorkingChar->setGender(token);
            }
            else if (previousToken == "Class")
            {
                currWorkingChar->setCClass(token);
            }
            else if (previousToken == "Race")
            {
                currWorkingChar->setRace(token);
            }
            else if (previousToken == "Maximum Hit Points")
            {
                currWorkingChar->setmaxHitPoints(std::stoi(token));
            }
			else if (previousToken == "Current Hit Points")
            {
                currWorkingChar->setcurrHitPoints(std::stoi(token));
            }
            else if (previousToken == "Overheal")
            {
                currWorkingChar->setOverheal(std::stoi(token));
            }
            else if (previousToken == "Armor Class")
            {
                currWorkingChar->setArmorClass(std::stoi(token));
            }
            else if (previousToken == "Size")
            {
                currWorkingChar->setSize(token);
            }
            else if (previousToken == "Height")
            {
                currWorkingChar->setHeight(std::stoi(token));
            }
            else if (previousToken == "Weight")
            {
                currWorkingChar->setWeight(std::stoi(token));
            }
            else if (previousToken == "Coordinate X")
            {
                currWorkingChar->setCoordinateX(std::stoi(token));
            }
            else if (previousToken == "Coordinate Y")
            {
                currWorkingChar->setCoordinateY(std::stoi(token));
            }
            else if (previousToken == "Ability Scores")
            {
                currWorkingChar->setAbilityScores(std::stoi(token));
            }
            else if (previousToken == "Speed")
            {
                currWorkingChar->setSpeed(std::stoi(token));
            }
            else if (previousToken == "Alignment")
            {
                currWorkingChar->setAlignment(token);
            }
            else if (previousToken == "Level")
            {
                currWorkingChar->setLevel(std::stoi(token));
            }
            else if (previousToken == "EXP")
            {
                currWorkingChar->setExp(std::stoi(token));
            }
            else if (previousToken == "Balance")
            {
                currWorkingChar->setBalance(std::stoi(token));
            }
            else if (previousToken == "Background")
            {
                currWorkingChar->setBackground(token);
            }
            else if (previousToken == "Vision Range")
            {
                currWorkingChar->setVisionRange(std::stoi(token));
            }
            else if (previousToken == "Reach")
            {
                currWorkingChar->setReach(std::stoi(token));
            }
	        else if (previousToken == "Powers")
			{
				tempQSL = QString::fromStdString(token).split(", ");               // Split each line by " = " and push tokens in tokenList
				for (int i = 0; i < tempQSL.size(); i++)
                {
                    qDebug() << "load203" << tempQSL.at(i);
                    currWorkingChar->getPowers()->push_back(tempQSL.at(i).toStdString());
				}
            }
//                else if (previousToken == "Class")
//                {

//                }
            previousToken = "reserved";
            returnV = 0;
        }
    }
    if (returnV == 0)
    {
    currWorkingMap->writeMap();
    emit mui->refreshCurrent();
    emit mui->updateLog("15",currWorkingMap,currWorkingChar->getEntityID(), currWorkingChar->getName(), currWorkingMap,
                        currWorkingMap->getMapName(), currWorkingChar, currWorkingChar->getName(),
                        currWorkingChar->getCoordinateX(),currWorkingChar->getCoordinateY());
    }
    return returnV;
}


int savePlayerCharacter(){

    std::string saveFilepath = "saves/";
    saveFilepath.append(currWorkingChar->getName());
    saveFilepath.append(".pc");
    std::ofstream outFile(saveFilepath);

    outFile << "Name = "				<< currWorkingChar->getName()           << '\n';
    outFile << "Gender = "				<< currWorkingChar->getGender()         << '\n';
    outFile << "Class = "				<< currWorkingChar->getCClass()         << '\n';
    outFile << "Level = "				<< currWorkingChar->getLevel()          << '\n';
    outFile << "Race = "				<< currWorkingChar->getRace()           << '\n';
    outFile << "Background = "			<< currWorkingChar->getBackground()     << '\n';
    outFile << "Alignment = "			<< currWorkingChar->getAlignment()      << '\n';
    outFile << "Armor Class = "			<< currWorkingChar->getArmorClass()     << '\n';
    outFile << "Ability Scores = "		<< currWorkingChar->getAbilityScores()  << '\n';
    outFile << "Maximum Hit Points = "  << currWorkingChar->getmaxHitPoints()   << '\n';
    outFile << "Current Hit Points = "  << currWorkingChar->getmaxHitPoints()   << '\n';
    outFile << "Overheal = "			<< currWorkingChar->getOverheal()       << '\n';
    outFile << "EXP = "					<< currWorkingChar->getExp()            << '\n';
    outFile << "Balance = "				<< currWorkingChar->getBalance()        << '\n';
    outFile << "Size = "				<< currWorkingChar->getSize()           << '\n';
    outFile << "Height = "				<< currWorkingChar->getHeight()         << '\n';
    outFile << "Weight = "				<< currWorkingChar->getWeight()         << '\n';
    outFile << "Reach = "				<< currWorkingChar->getReach()          << '\n';
    outFile << "Speed = "				<< currWorkingChar->getSpeed()          << '\n';
    outFile << "Vision Range = "		<< currWorkingChar->getVisionRange()    << '\n';
    outFile << "Coordinate X = "		<< currWorkingChar->getCoordinateX()    << '\n';
    outFile << "Coordinate Y = "		<< currWorkingChar->getCoordinateY()    << '\n';


    outFile << "Powers = ";
    int i=0;
    std::list<std::string> :: iterator iter;
    std::string currentPower;
    std::list<std::string> *tempPowers;
    tempPowers = currWorkingChar->getPowers();
    for(iter =tempPowers->begin(); iter != tempPowers->end(); iter++)
    {
        currentPower = *iter;
        outFile << currentPower;
        i++;
        if (i < tempPowers->size()){ outFile << ", "; }
    }

//    outFile << "Name = " << currWorkingChar->getName() << '\n';
//    outFile << "Name = " << currWorkingChar->getName() << '\n';
//    outFile << "Name = " << currWorkingChar->getName() << '\n';
//    outFile << "Name = " << currWorkingChar->getName() << '\n';
    emit mui->updateLog("16",currWorkingMap,currWorkingChar->getEntityID(), currWorkingChar->getName(), currWorkingMap,
                        currWorkingMap->getMapName(), currWorkingChar, currWorkingChar->getName(),
                        currWorkingChar->getCoordinateX(),currWorkingChar->getCoordinateY());
    return 0;
}
