#include "Tile.h"

#include "Param.h"

namespace mms{

Tile::Tile() : m_x(-1), m_y(-1), m_distance(-1) {
    for (Direction direction : DIRECTIONS()) {
        m_walls.insert(direction, false);
    }
}

int Tile::getX() const {
    return m_x;
}

int Tile::getY() const {
    return m_y;
}

void Tile::setPos(int x, int y) {
    m_x = x;
    m_y = y;
}

bool Tile::isWall(Direction direction) const {
    return m_walls.value(direction);
}

void Tile::setWall(Direction direction, bool isWall) {
    m_walls[direction] = isWall;
}

int Tile::getDistance() const {
    return m_distance;
}

void Tile::setDistance(int distance) {
    m_distance= distance;
}

Polygon Tile::getFullPolygon() const {
    return m_fullPolygon;
}

Polygon Tile::getInteriorPolygon() const {
    return m_interiorPolygon;
}

Polygon Tile::getWallPolygon(Direction direction) const {
    return m_wallPolygons.value(direction);
}

QVector<Polygon> Tile::getCornerPolygons() const {
    return m_cornerPolygons;
}

void Tile::initPolygons(int mazeWidth, int mazeHeight) {

    //  The polygons associated with each tile are as follows:
    //
    //      full: 05af
    //
    //      interior: 278d
    //
    //      northWall: 7698
    //      eastWall: d8be
    //      southWall: 32dc
    //      westWall: 1472
    //
    //      lowerLeftCorner: 0123
    //      upperLeftCorner: 4567
    //      upperRightCorner: 89ab
    //      lowerRightCorner: cdef
    //
    //      5---6-------------9---a
    //      |   |             |   |
    //      4---7-------------8---b
    //      |   |             |   |
    //      |   |             |   |
    //      |   |             |   |
    //      |   |             |   |
    //      |   |             |   |
    //      1---2-------------d---e
    //      |   |             |   |
    //      0---3-------------c---f

    // Order is important
    initFullPolygon(mazeWidth, mazeHeight);
    initInteriorPolygon(mazeWidth, mazeHeight);
    initWallPolygons(mazeWidth, mazeHeight);
    initCornerPolygons(mazeWidth, mazeHeight);
}


void Tile::initFullPolygon(int mazeWidth, int mazeHeight) {
    Distance halfWallWidth = Distance::Meters(P()->wallWidth()) / 2.0;
    Distance tileLength = Distance::Meters(P()->wallLength() + P()->wallWidth());
    Coordinate lowerLeftPoint = Coordinate::Cartesian(
        tileLength * getX() - halfWallWidth * (getX() == 0 ? 1 : 0),
        tileLength * getY() - halfWallWidth * (getY() == 0 ? 1 : 0)
    );
    Coordinate upperRightPoint = Coordinate::Cartesian(
        tileLength * (getX() + 1) + halfWallWidth * (getX() == mazeWidth - 1 ? 1 : 0),
        tileLength * (getY() + 1) + halfWallWidth * (getY() == mazeHeight - 1 ? 1 : 0)
    );
    Coordinate lowerRightPoint = Coordinate::Cartesian(
        upperRightPoint.getX(),
        lowerLeftPoint.getY()
    );
    Coordinate upperLeftPoint = Coordinate::Cartesian(
        lowerLeftPoint.getX(),
        upperRightPoint.getY()
    );
    m_fullPolygon = Polygon({
        lowerLeftPoint,
        upperLeftPoint,
        upperRightPoint,
        lowerRightPoint,
    });
}

void Tile::initInteriorPolygon(int mazeWidth, int mazeHeight) {

    Distance halfWallWidth = Distance::Meters(P()->wallWidth()) / 2.0;
    Coordinate lowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Coordinate upperLeftPoint = m_fullPolygon.getVertices().at(1);
    Coordinate upperRightPoint = m_fullPolygon.getVertices().at(2);
    Coordinate lowerRightPoint = m_fullPolygon.getVertices().at(3);

    m_interiorPolygon = Polygon({
        lowerLeftPoint + Coordinate::Cartesian(
            halfWallWidth * (getX() == 0 ? 2 : 1),
            halfWallWidth * (getY() == 0 ? 2 : 1)
        ),
        upperLeftPoint + Coordinate::Cartesian(
            halfWallWidth * (getX() == 0 ? 2 : 1),
            halfWallWidth * (getY() == mazeHeight - 1 ? -2 : -1)
        ),
        upperRightPoint + Coordinate::Cartesian(
            halfWallWidth * (getX() == mazeWidth - 1 ? -2 : -1),
            halfWallWidth * (getY() == mazeHeight - 1 ? -2 : -1)
        ),
        lowerRightPoint + Coordinate::Cartesian(
            halfWallWidth * (getX() == mazeWidth - 1 ? -2 : -1),
            halfWallWidth * (getY() == 0 ? 2 : 1)
        ),
    });
}

void Tile::initWallPolygons(int mazeWidth, int mazeHeight) {

    Coordinate outerLowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Coordinate outerUpperLeftPoint = m_fullPolygon.getVertices().at(1);
    Coordinate outerUpperRightPoint = m_fullPolygon.getVertices().at(2);
    Coordinate outerLowerRightPoint = m_fullPolygon.getVertices().at(3);

    Coordinate innerLowerLeftPoint = m_interiorPolygon.getVertices().at(0);
    Coordinate innerUpperLeftPoint = m_interiorPolygon.getVertices().at(1);
    Coordinate innerUpperRightPoint = m_interiorPolygon.getVertices().at(2);
    Coordinate innerLowerRightPoint = m_interiorPolygon.getVertices().at(3);

    QVector<Coordinate> northWall;
    northWall.push_back(innerUpperLeftPoint);
    northWall.push_back(Coordinate::Cartesian(
        innerUpperLeftPoint.getX(),
        outerUpperLeftPoint.getY()
    ));
    northWall.push_back(Coordinate::Cartesian(
        innerUpperRightPoint.getX(),
        outerUpperRightPoint.getY()
    ));
    northWall.push_back(innerUpperRightPoint);
    m_wallPolygons.insert(Direction::NORTH, Polygon(northWall));

    QVector<Coordinate> eastWall;
    eastWall.push_back(innerLowerRightPoint);
    eastWall.push_back(innerUpperRightPoint);
    eastWall.push_back(Coordinate::Cartesian(
        outerUpperRightPoint.getX(),
        innerUpperRightPoint.getY()
    ));
    eastWall.push_back(Coordinate::Cartesian(
        outerLowerRightPoint.getX(),
        innerLowerRightPoint.getY()
    ));
    m_wallPolygons.insert(Direction::EAST, Polygon(eastWall));

    QVector<Coordinate> southWall;
    southWall.push_back(Coordinate::Cartesian(
        innerLowerLeftPoint.getX(),
        outerLowerLeftPoint.getY()
    ));
    southWall.push_back(innerLowerLeftPoint);
    southWall.push_back(innerLowerRightPoint);
    southWall.push_back(Coordinate::Cartesian(
        innerLowerRightPoint.getX(),
        outerLowerRightPoint.getY()
    ));
    m_wallPolygons.insert(Direction::SOUTH, Polygon(southWall));

    QVector<Coordinate> westWall;
    westWall.push_back(Coordinate::Cartesian(
        outerLowerLeftPoint.getX(),
        innerLowerLeftPoint.getY()
    ));
    westWall.push_back(Coordinate::Cartesian(
        outerUpperLeftPoint.getX(),
        innerUpperLeftPoint.getY()
    ));
    westWall.push_back(innerUpperLeftPoint);
    westWall.push_back(innerLowerLeftPoint);
    m_wallPolygons.insert(Direction::WEST, Polygon(westWall));
}

void Tile::initCornerPolygons(int mazeWidth, int mazeHeight) {

    Coordinate outerLowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Coordinate outerUpperLeftPoint = m_fullPolygon.getVertices().at(1);
    Coordinate outerUpperRightPoint = m_fullPolygon.getVertices().at(2);
    Coordinate outerLowerRightPoint = m_fullPolygon.getVertices().at(3);

    Coordinate innerLowerLeftPoint = m_interiorPolygon.getVertices().at(0);
    Coordinate innerUpperLeftPoint = m_interiorPolygon.getVertices().at(1);
    Coordinate innerUpperRightPoint = m_interiorPolygon.getVertices().at(2);
    Coordinate innerLowerRightPoint = m_interiorPolygon.getVertices().at(3);

    QVector<Coordinate> lowerLeftCorner;
    lowerLeftCorner.push_back(outerLowerLeftPoint);
    lowerLeftCorner.push_back(Coordinate::Cartesian(
        outerLowerLeftPoint.getX(),
        innerLowerLeftPoint.getY()
    ));
    lowerLeftCorner.push_back(innerLowerLeftPoint);
    lowerLeftCorner.push_back(Coordinate::Cartesian(
        innerLowerLeftPoint.getX(),
        outerLowerLeftPoint.getY()
    ));
    m_cornerPolygons.push_back(Polygon(lowerLeftCorner));

    QVector<Coordinate> upperLeftCorner;
    upperLeftCorner.push_back(Coordinate::Cartesian(
        outerUpperLeftPoint.getX(),
        innerUpperLeftPoint.getY()
    ));
    upperLeftCorner.push_back(outerUpperLeftPoint);
    upperLeftCorner.push_back(Coordinate::Cartesian(
        innerUpperLeftPoint.getX(),
        outerUpperLeftPoint.getY()
    ));
    upperLeftCorner.push_back(innerUpperLeftPoint);
    m_cornerPolygons.push_back(Polygon(upperLeftCorner));

    QVector<Coordinate> upperRightCorner;
    upperRightCorner.push_back(innerUpperRightPoint);
    upperRightCorner.push_back(Coordinate::Cartesian(
        innerUpperRightPoint.getX(),
        outerUpperRightPoint.getY()
    ));
    upperRightCorner.push_back(outerUpperRightPoint);
    upperRightCorner.push_back(Coordinate::Cartesian(
        outerUpperRightPoint.getX(),
        innerUpperRightPoint.getY()
    ));
    m_cornerPolygons.push_back(Polygon(upperRightCorner));

    QVector<Coordinate> lowerRightCorner;
    lowerRightCorner.push_back(Coordinate::Cartesian(
        innerLowerRightPoint.getX(),
        outerLowerRightPoint.getY()
    ));
    lowerRightCorner.push_back(innerLowerRightPoint);
    lowerRightCorner.push_back(Coordinate::Cartesian(
        outerLowerRightPoint.getX(),
        innerLowerRightPoint.getY()
    ));
    lowerRightCorner.push_back(outerLowerRightPoint);
    m_cornerPolygons.push_back(Polygon(lowerRightCorner));
}

} // namespace mms
