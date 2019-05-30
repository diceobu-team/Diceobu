#include "lobbywindow.h"
#include "ui_lobbywindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDesktopWidget>
#include <QVector>
#include <string>
#include <QPaintEvent>
#include <QPainter>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include "mainDependancies.h"
#include "Map.h"
#include "DiceobuLibrary.h"
#include "mainuiclass.h"
#include <QObject>
#include <QKeyEvent>
#include <QStringList>
#include <QStyle>


static int window_width    =   1440;
static int window_height   =   900;

int targetMapID;
int targetCharacterID;

int directionalMovement = 0;

MainUIClass *mui = new MainUIClass;

LobbyWindow::LobbyWindow(QWidget *parent) :
    QMainWindow(),
    ui(new Ui::LobbyWindow)
{
    ui->setupUi(this);

    connect(mui, SIGNAL(refreshCurrent()),
            this, SLOT(displayCurrent()));
    connect(mui, SIGNAL(updateLog(std::string ,Map* ,int , std::string, Map* , std::string , Character* ,std::string , int ,int )),
            this, SLOT(updateSystemLog(std::string ,Map* ,int , std::string, Map* , std::string , Character* ,std::string , int ,int )));
	connect(mui, SIGNAL(errorMessage(int)),
			this, SLOT(errorHandler(int)));

    QPixmap background(":/img/bg4.jpg");
    setFixedSize(window_width,window_height);
    background = background.scaled(window_width+50,window_height+50, Qt::IgnoreAspectRatio);


    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);

    displayCurrent();
    checkLists();
}



LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::on_actionNewCharacter_triggered()
{

    if (activeMapsisEmpty())
    {
        QMessageBox::critical(this,"Error!","Cannot create character with no existing Maps!");
    }
    else
    {
        charCreateWindow = new CharCreateWindow(this);
        charCreateWindow->show();
    }
}

void LobbyWindow::on_pushButton_Character_Details_clicked()
{
    if (activeCharactersisEmpty())
    {
        errorHandler(5);
    }
    else
    {
        characterDetailsWindow = new CharacterDetailsWindow(this);
        characterDetailsWindow->show();
    }
}

void LobbyWindow::on_pushButton_Move_clicked()
{
    if (getActiveMaps().empty())
    {
        QMessageBox::critical(this,"Error!","Nothing to move!");
    }
    else
    {
        if (currWorkingMap->m_containingCharacters.empty())
        {
            QMessageBox::critical(this,"Error!","Nothing to move!");
        }
        else
        {
         targetMapID = currWorkingMap->getMapID();
            moveWindow = new MoveWindow(this);
            qDebug() << "starting move from" << targetMapID;
            moveWindow->show();
        }
    }

}


void LobbyWindow::on_pushButton_5_clicked()
{

}


void LobbyWindow::on_actionDeleteMap_triggered()
{
    diceobuSystemCore("8");
}

void LobbyWindow::on_actionNewMap_triggered()
{
    mapCreateWindow = new MapCreateWindow(this);
    mapCreateWindow->show();
    //diceobuSystemCore("1");
}

void LobbyWindow::on_pushButton_nextMap_clicked()
{
    diceobuSystemCore("10");
}

void LobbyWindow::on_actionDeleteCharacter_triggered()
{
    diceobuSystemCore("9");
}


void LobbyWindow::on_pushButton_nextCharacter_clicked()
{
    diceobuSystemCore("11");
}

void LobbyWindow::displayCurrent()
{
    //// // qDebug() << "was hereDISPLAY";
    if (activeMapsisEmpty())
    {
        ui->label_currMap->setText("None");
    }
    else
    {
        ui->label_currMap->setText(QString::number(currWorkingMap->getMapID()));
    }

    if (activeCharactersisEmpty())
    {
        ui->label_currChar->setText("None");
    }
    else
    {
        ui->label_currChar->setText(QString::number(currWorkingChar->getEntityID()));
    }
    ui->tableWidget->update();
    updateLists();
}

void LobbyWindow::updateSystemLog(std::string input,Map* this_currWorkingMap,int currWorkingCharID, std::string currWorkingCharName,
                                  Map* previousMap, std::string previousMapName, Character* previousCharacter,std::string previousCharacterName, int coordX,int coordY)
{
    if (input == "1")
    {
       ui->system_log->append(QString(">> Created Map [ID: %1] - %2.").arg(QString::number(this_currWorkingMap->getMapID()),
                                                                         QString::fromStdString(this_currWorkingMap->getMapName())));
    }
    else if (input == "2")
    {
        ui->system_log->append(QString(">> Created Character [ID: %1] - %2 on Map [ID: %3] at {%4,%5}.").arg(QString::number(currWorkingCharID),
                                                                                                             QString::fromStdString(currWorkingCharName),
                                                                                                             QString::number(currWorkingMap->getMapID()),
                                                                                                             QString::number(coordX),
                                                                                                             QString::number(coordY)));
    }
    else if (input == "3")
    {
        ui->system_log->append(QString(">> Moved Character [ID: %1] - %2 on Map [ID: %3] to {%4,%5}.").arg(QString::number(currWorkingCharID),
                                                                                                             QString::fromStdString(currWorkingCharName),
                                                                                                             QString::number(currWorkingMap->getMapID()),
                                                                                                             QString::number(coordX),
                                                                                                             QString::number(coordY)));
    }
    else if (input == "8")
    {
        qDebug() << "mD 632 " << QString::number(previousMap->getMapID()) << QString::fromStdString(previousMap->getMapName());

        ui->system_log->append(QString(">> Deleted Map [ID: %1] - %2.").arg(QString::number(previousMap->getMapID()),
                                                                            QString::fromStdString(previousMapName)));
    }
    else if (input == "9")
    {
        ui->system_log->append(QString(">> Deleted Character [ID: %1] - %2.").arg(QString::number(previousCharacter->getEntityID()),
                                                                                 QString::fromStdString(previousCharacterName)));
    }
    else if (input == "10" || input == "13")
    {
        ui->system_log->append(QString(">> Changed view from Map [ID: %1] to Map [ID: %2] - %3.").arg(QString::number(previousMap->getMapID()),
                                                                                                      QString::number(currWorkingMap->getMapID()),
                                                                                                      QString::fromStdString(currWorkingMap->getMapName())));
    }
    else if (input == "11" || input == "14")
    {
        ui->system_log->append(QString(">> Changed view from Character [ID: %1] to Character [ID: %2] - %3.").arg(QString::number(previousCharacter->getEntityID()),
                                                                                                                  QString::number(currWorkingCharID),
                                                                                                                  QString::fromStdString(currWorkingCharName)));
    }
}


void LobbyWindow::updateLists(){

    std::list<Map*> tempMaps = getActiveMaps();
    std::list<Character*> tempCharacters = getActiveCharacters();

    if (activeMapsisEmpty())
    {
        ui->comboBox_Maps->removeItem(0);
        ui->comboBox_Maps->addItem("No Maps available.");
    }
    else
    {
        for (int i = 0; i <= tempMaps.size() + 1; i++)
        {
            ui->comboBox_Maps->removeItem(0);
        }
        std::list <Map*> :: iterator iter;
        for (iter = tempMaps.begin(); iter != tempMaps.end(); iter++)
        {
            Map* tempMap = *iter;
            if (tempMap->getMapID() == currWorkingMap->getMapID())
            {
                ui->comboBox_Maps->addItem(QString::number(tempMap->getMapID()) + " : " +  QString::fromStdString(tempMap->getMapName()));
            break;
            }
        }
        for (iter = tempMaps.begin(); iter != tempMaps.end(); iter++)
        {
            Map* tempMap = *iter;
            if (tempMap->getMapID() != currWorkingMap->getMapID())
            {
                ui->comboBox_Maps->addItem(QString::number(tempMap->getMapID()) + " : " +  QString::fromStdString(tempMap->getMapName()));
            }
        }
    }


    if (activeCharactersisEmpty())
    {
        ui->comboBox_Characters->removeItem(0);
        ui->comboBox_Characters->addItem("No Characters available.");
    }
    else
    {
        for (int i = 0; i <= tempCharacters.size() + 1; i++)
        {
            ui->comboBox_Characters->removeItem(0);
        }

        std::list <Character*> :: iterator iter2;
        for (iter2 = tempCharacters.begin(); iter2 != tempCharacters.end(); iter2++)
        {
            Character* tempCharacter = *iter2;
            if (tempCharacter->getEntityID() == currWorkingChar->getEntityID())
            {
            ui->comboBox_Characters->addItem(QString::number(tempCharacter->getEntityID()) + " : " +  QString::fromStdString(tempCharacter->getName()));
            break;
            }
        }
        for (iter2 = tempCharacters.begin(); iter2 != tempCharacters.end(); iter2++)
        {
            Character* tempCharacter = *iter2;
            if (tempCharacter->getEntityID() != currWorkingChar->getEntityID())
            {
            ui->comboBox_Characters->addItem(QString::number(tempCharacter->getEntityID()) + " : " +  QString::fromStdString(tempCharacter->getName()));
            }
        }
    }
}

void LobbyWindow::keyPressEvent(QKeyEvent *e)
{
    if (!activeMapsisEmpty())
    {
        if (currWorkingMap->m_containingCharacters.empty())
        {
            QMessageBox::critical(this,"Error!","Nothing to move!");
        }
        else
        {
            if ((e->key() == Qt::Key_W) || (e->key() == Qt::Key_S) || (e->key() == Qt::Key_A) || (e->key() == Qt::Key_D))
            {
                int coordX = currWorkingChar->getCoordinateX();
                int coordY = currWorkingChar->getCoordinateY();

                directionalMovement = 1;

                if (e->key() == Qt::Key_W) // Up
                {
                    if (coordX > 0)
                    {
                        coordX--;
                    }
                }
                else if (e->key() == Qt::Key_S) // Down
                {
                    if (coordX < 49)
                    {
                        coordX++;
                    }
                }
                else if (e->key() == Qt::Key_A) // Left
                {
                    if (coordY > 0)
                    {
                        coordY--;
                    }
                }
                else if (e->key() == Qt::Key_D) // Right
                {
                    if (coordY < 49)
                    {
                        coordY++;
                    }
                }
                if (currWorkingMap->m_tileGrid[coordX][coordY]->getOccupied() || !currWorkingMap->m_tileGrid[coordX][coordY]->getOpen() )
                {
                    // Sound effect KAPPA
                }
                else
                {
                    //qDebug() <<  coordX << coordY;

                   diceobuSystemCore("3",coordX,coordY);
                }
            }
        }
    }
}

void LobbyWindow::on_pushButton_Grid_toggled(bool checked)
{
    grid = checked;
    displayCurrent();
}

void LobbyWindow::on_comboBox_Maps_activated(const QString &arg1)
{
    QStringList tempQSList;
    tempQSList = ui->comboBox_Maps->currentText().split(" ");
	targetMapID = tempQSList.at(0).toInt();
	diceobuSystemCore("13");
}

void LobbyWindow::on_comboBox_Characters_activated(const QString &arg1)
{
    QStringList tempQSList;
    tempQSList = ui->comboBox_Characters->currentText().split(" ");
    targetCharacterID = tempQSList.at(0).toInt();
    diceobuSystemCore("14");
}

void LobbyWindow::errorHandler(int errorCode)
{
	switch (errorCode)
	{
	case 1:
        QMessageBox::critical(this, "Error!", "Cannot create character with no existing Maps!");
        break;
	case 2:
		QMessageBox::critical(this, "Error!", "Nothing to delete!");
		break;
	case 3:
		QMessageBox::critical(this, "Error!", "Cannot delete a Map containing characters!");
		break;
	case 4:
        QMessageBox::critical(this, "Error!", "No Maps found!");
		break;
	case 5:
		QMessageBox::critical(this, "Error!", "No Characters found!");
		break;
	}
}

