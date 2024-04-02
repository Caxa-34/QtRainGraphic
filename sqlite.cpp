#include "sqlite.h"

Sqlite::Sqlite(Surface *surface, QListWidget *listSave) {
    createDatabase();
    this->surface = surface;
    this->listSave = listSave;
    updateListSave();
}


bool Sqlite::createDatabase() {
    //проверяем наличие локальной бд
    QString path = QDir::currentPath() + "/RainGraphic.db";
    QFileInfo checkDB(path);
    if (checkDB.exists()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("RainGraphic.db");
        db.open();
        qDebug() << "DB opened";
        return false;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("RainGraphic.db");
    db.open();

    QSqlQuery query;

    // Создание таблицы Surface
    if (!query.exec("CREATE TABLE IF NOT EXISTS Surface ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name NVARCHAR(50) NOT NULL,"
                    "width INTEGER NOT NULL,"
                    "height INTEGER NOT NULL,"
                    "creationDate DATETIME NOT NULL)")) {
        qDebug() << "Error creating Surface table:";
        qDebug() << query.lastError().text();
        return false;
    }

    // Создание таблицы Tile
    if (!query.exec("CREATE TABLE IF NOT EXISTS Tile ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "surfaceId INTEGER NOT NULL,"
                    "x INTEGER NOT NULL,"
                    "y INTEGER NOT NULL,"
                    "height INTEGER NOT NULL,"
                    "FOREIGN KEY (surfaceId) REFERENCES Surface(id))")) {
        qDebug() << "Error creating Tile table:";
        qDebug() << query.lastError().text();
        return false;
    }
    qDebug() << "Table created";
    return true;
}

int Sqlite::saveIntoDB(QString name) {
    QDateTime currentDateTime = QDateTime::currentDateTime(); // текущее датавремя

    QSqlQuery surfaceQuery;
    surfaceQuery.prepare("INSERT INTO Surface (name, width, height, creationDate)"
                         "VALUES (:name, :width, :height, :creationDate)");

    surfaceQuery.bindValue(":name", name);
    surfaceQuery.bindValue(":width", surface->width);
    surfaceQuery.bindValue(":height", surface->height);
    surfaceQuery.bindValue(":creationDate", currentDateTime.toString(Qt::ISODate));

    if (!surfaceQuery.exec()) {
        qDebug() << "Error inserting data into Surface table:";
        qDebug() << surfaceQuery.lastError().text();
        return 0;
    }
    int surfaceId = surfaceQuery.lastInsertId().toInt(); // получение id только что добавленной поверхности

    QSqlQuery tileQuery;
    tileQuery.prepare("INSERT INTO Tile (surfaceId, x, y, height) "
                      "VALUES (:surfaceId, :x, :y, :height)");

    tiles = surface->getSurface(false);

    for (int i = 0; i < surface->width; ++i) {
        for (int j = 0; j < surface->height; ++j) {

            tileQuery.bindValue(":surfaceId", surfaceId);
            tileQuery.bindValue(":x", i);
            tileQuery.bindValue(":y", j);
            tileQuery.bindValue(":height", tiles[i][j]->height);

            if (!tileQuery.exec()) {
                qDebug() << "Error inserting data into Tile table:";
                qDebug() << tileQuery.lastError().text();
                return 0;
            }
        }
    }
    updateListSave();
    return surfaceId;
}

bool Sqlite::getFromDB(int num) {
    if (num < 0) return false;

    // получение размеров поверхности
    QSqlQuery query;
    query.prepare("SELECT width, height FROM Surface WHERE id = " + QString::number(num));
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    int width = 1;
    int height = 1;
    if (query.next()) {
        width = query.value(0).toInt();
        height = query.value(1).toInt();
    } else {
        return false;
    }


    // отрисовка тайлов
    query.prepare("SELECT * FROM Tile WHERE surfaceId = " + QString::number(num));
    if (!query.exec()) {
        qDebug() << "Error fetching tiles from database:";
        qDebug() << query.lastError().text();
        return false;
    }
    // Устанавливаем размеры поверхности
    surface->setSize(width, height);
    // Заполняем тайлы из бд
    while (query.next()) {
        int x = query.value(2).toInt();
        int y = query.value(3).toInt();
        int height = query.value(4).toInt();

        surface->setTile(x, y, height);
    }
    return true;
}


void Sqlite::updateListSave(){
    listSave->clear();
    QSqlQuery query;
    if (!query.exec("SELECT id, name, creationDate FROM Surface")) {
        qDebug() << "Error fetching data from Surface table:";
        qDebug() << query.lastError().text();
        return;
    }
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString creationDate = query.value(2).toString();

        QString itemText = name + "  (" + creationDate + ")";

        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, id); // Сохраняем id элемента в пользовательских данных
        listSave->addItem(item);
    }
}
