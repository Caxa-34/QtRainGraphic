#include "surface.h"

Surface::Surface()
{
    width = 1;
    height = 1;
    createSurface();
}

Surface::Surface(int w, int h) {
    width = w;
    height = h;
    createSurface();
}

void Surface::setRows(int h) {
    height = h;
    createSurface();
}

void Surface::setColumns(int w) {
    width = w;
    createSurface();
}

void Surface::setSize(int w, int h) {
    width = w;
    height = h;
    createSurface();
}

void Surface::createSurface() {
    tiles.resize(width);
    for (int i = 0; i < width; i++) {
        tiles[i].resize(height);
        for (int j = 0; j < height; j++) {
            if (tiles[i][j] == nullptr) // Заполнение пустых клеток
                tiles[i][j] = new Tile(i, j, 0);
        }
    }
    createNeighbors();
}

void Surface::randSurface() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 30);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int randomHeight = dis(gen);
            tiles[i][j] = new Tile(i, j, randomHeight);
        }
    }
    createNeighbors();
}

void Surface::setTile(int x, int y, int heightTile) {
    if (x >= width || y >= height) {
        return;
    }
    tiles[x][y] = new Tile(x, y, heightTile);
    createSurface();
}

void Surface::createNeighbors() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            tiles[i][j]->neighbors.clear();

            if (i != width - 1)
                tiles[i][j]->neighbors.push_back(tiles[i+1][j]); //право

            if (i != 0)
                tiles[i][j]->neighbors.push_back(tiles[i-1][j]); //лево

            if (j != height - 1)
                tiles[i][j]->neighbors.push_back(tiles[i][j+1]); //низ

            if (j != 0)
                tiles[i][j]->neighbors.push_back(tiles[i][j-1]); //верх
        }
    }
}

void Surface::clearWater() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            tiles[i][j]->heightWater = 0;
            tiles[i][j]->heightWithWater = tiles[i][j]->height;
        }
    }
}

void Surface::getValues() {
    totalWater = 0;
    maxHeight = tiles[0][0]->height;
    minHeight = tiles[0][0]->height;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (tiles[i][j]->height > maxHeight) maxHeight = tiles[i][j]->height;
            if (tiles[i][j]->height < minHeight) minHeight = tiles[i][j]->height;
        }
    }
    tilesHeight = maxHeight - minHeight;

    //qDebug() << "tilesHeight =" << tilesHeight << "\nmaxHeight =" << maxHeight << "\nminHeight =" << minHeight;
}

bool Surface::atBorder(Tile* tile) { //Проверка на нахождение тайла с краю (где вода 100% стекает)
    if (tile->x == 0) return true;
    if (tile->y == 0) return true;
    if (tile->x == width - 1) return true;
    if (tile->y == height - 1) return true;
    return false;
}

int minInTiles(std::vector<Tile*> tiles) {
    int min = tiles[0]->heightWithWater;
    for (int i = 0; i < tiles.size(); i++) {
        if (min > tiles[i]->heightWithWater) min = tiles[i]->heightWithWater;
    }
    return min;
}

void Surface::calculate() {
    clearWater();

    int cntColumn = width / 10;
    int cntRow = height / 10;
    int lastWidth = width - 10 * cntColumn;
    int lastHeight = height - 10 * cntRow;

    std::vector<QFuture<void>> futures;

    for (int i = 0; i < cntColumn; i++)
        for (int j = 0; j < cntRow; j++) {
            futures.push_back(QtConcurrent::run(this, &Surface::calculatePart, i*10, j*10, 10, 10));
        }
    if (lastHeight > 0)
        for (int i = 0; i < cntColumn; i++) {
            futures.push_back(QtConcurrent::run(this, &Surface::calculatePart, i*10, cntRow*10, 10, lastHeight));
        }
    if (lastWidth > 0)
        for (int j = 0; j < cntRow; j++) {
            futures.push_back(QtConcurrent::run(this, &Surface::calculatePart, cntColumn*10, j*10, lastWidth, 10));
        }
    if (lastWidth > 0 && lastHeight > 0)
        futures.push_back(QtConcurrent::run(this, &Surface::calculatePart, cntColumn*10, cntRow*10, lastWidth, lastHeight));

    // Ожидаем завершения всех потоков
    for (auto& future : futures) {
        future.waitForFinished();
    }
}

void Surface::calculatePart(int x, int y, int widthPart, int heightPart) {
    for (int numWater = 0; numWater < tilesHeight; numWater++) {
        for (int xTile = x; xTile < widthPart + x; xTile++)
            for (int yTile = y; yTile < heightPart + y; yTile++) {
                qDebug() << xTile << yTile;
                std::vector<Tile*> tilesPath = std::vector<Tile*>();
                Tile *curTile = tiles[xTile][yTile];
                bool path = calculateWater(curTile, &tilesPath);
                if (!path) {
                    int minInPath = minInTiles(tilesPath);
                    for (int i = 0; i < tilesPath.size(); i++) {
                        if (tilesPath[i]->heightWithWater == minInPath) {
                            tilesPath[i]->heightWithWater++;
                            tilesPath[i]->heightWater++;
                            totalWater++;
                        }
                    }
                }
            }
    }
}



bool Surface::calculateWater(Tile* currentTile, std::vector<Tile*> *tilesPath) {
    if (atBorder(currentTile)) return true;
    int heightWater = currentTile->heightWithWater + 1;
    tilesPath->push_back(currentTile);

    bool path1 = false, path2 = false, path3 = false, path4 = false;
    if (std::find(tilesPath->begin(), tilesPath->end(), currentTile->neighbors[0]) == tilesPath->end()
            && currentTile->neighbors[0]->heightWithWater < heightWater) {
        path1 = calculateWater(currentTile->neighbors[0], tilesPath);
    }
    if (std::find(tilesPath->begin(), tilesPath->end(), currentTile->neighbors[1]) == tilesPath->end()
            && currentTile->neighbors[1]->heightWithWater < heightWater) {
        path2 = calculateWater(currentTile->neighbors[1], tilesPath);
    }
    if (std::find(tilesPath->begin(), tilesPath->end(), currentTile->neighbors[2]) == tilesPath->end()
            && currentTile->neighbors[2]->heightWithWater < heightWater) {
        path3 = calculateWater(currentTile->neighbors[2], tilesPath);
    }
    if (std::find(tilesPath->begin(), tilesPath->end(), currentTile->neighbors[3]) == tilesPath->end()
            && currentTile->neighbors[3]->heightWithWater < heightWater) {
        path4 = calculateWater(currentTile->neighbors[3], tilesPath);
    }
    if (path1 || path2 || path3 || path4) return true;
    return false;
}

std::vector<std::vector<Tile*>> Surface::getSurface(bool afterRain) {
    getValues();
    if (afterRain) calculate();
    return tiles;
}

