#include "surfaceview.h"

SurfaceView::SurfaceView(QGraphicsScene *scene, Surface *surface)
{
    this->scene = scene;
    this->surface = surface;
    pen = QPen(Qt::black);
    pen.setWidth(1);
}

void SurfaceView::drawSurface(bool afterRain) {
    tiles = surface->getSurface(afterRain);
    scene->clear();
    scene->setSceneRect(scene->itemsBoundingRect());
    int density = 10;
    int heightRange = surface->tilesHeight / density;
    //qDebug() << "Start drawing!";
    rectTiles.resize(surface->width);
    for (int i = 0; i < surface->width; ++i) {
        rectTiles[i].resize(surface->height);
        for (int j = 0; j < surface->height; ++j) {
            rectTiles[i][j] = new QGraphicsRectItem(i*size, j*size, size, size);
            rectTiles[i][j]->setPen(pen);
            QString str = QString::number(tiles[i][j]->height);
            if (tiles[i][j]->heightWater > 0) {
                rectTiles[i][j]->setBrush(QBrush(Qt::cyan));
                str += "\n+ " + QString::number(tiles[i][j]->heightWater);
            }
            else if (heightRange > 0) {
                int level = (tiles[i][j]->height - surface->minHeight) / heightRange + 1;
                int brightness = 96 * (1.0 - static_cast<double>(level - 1) / density)+159;
                QColor color(brightness, brightness, brightness);
                rectTiles[i][j]->setBrush(QBrush(color));
            }
            QGraphicsTextItem *textItem = new QGraphicsTextItem(str);
            textItem->setPos(i*size, j*size); // Центрируем текст внутри item
            textItem->adjustSize();
            scene->addItem(rectTiles[i][j]);
            scene->addItem(textItem);
            //qDebug() << "Drawing tile" << i << j;
        }
    }
    //qDebug() << "End drawing!";
}

Tile* SurfaceView::getTile(QPointF point) {
    drawSurface(false);
    tiles = surface->getSurface(false);
    int x = point.x() / size;
    int y = point.y() / size;
    if (surface->width <= x || surface->height <= y) {
        return nullptr;
    }
    if (!scene->sceneRect().contains(point)) {
        return nullptr;
    }
    rectTiles[x][y]->setBrush(QBrush(Qt::red));
    return tiles[x][y];
}
