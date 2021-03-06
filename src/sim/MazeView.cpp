#include "MazeView.h"

#include "BufferInterface.h"
#include "MazeGraphic.h"
#include "Param.h"

namespace mms {

MazeView::MazeView(
        const Maze* maze,
        bool wallTruthVisible,
        bool tileColorsVisible,
        bool tileFogVisible,
        bool tileTextVisible,
        bool autopopulateTextWithDistance) :
        m_bufferInterface(
            {maze->getWidth(), maze->getHeight()},
            &m_graphicCpuBuffer,
            &m_textureCpuBuffer),
        m_mazeGraphic(
            maze,
            &m_bufferInterface,
            wallTruthVisible,
            tileColorsVisible,
            tileFogVisible,
            tileTextVisible,
            autopopulateTextWithDistance) {

    // Establish the coordinates for the tile text characters
    initText(2, 4);

    // Populate the data vectors with wall polygons and tile distance text.
    m_mazeGraphic.drawPolygons();
    m_mazeGraphic.drawTextures();
}

MazeGraphic* MazeView::getMazeGraphic() {
    return &m_mazeGraphic;
}

void MazeView::initTileGraphicText(int numRows, int numCols) {
    initText(numRows, numCols);
}

const QVector<TriangleGraphic>* MazeView::getGraphicCpuBuffer() const {
    return &m_graphicCpuBuffer;
}

const QVector<TriangleTexture>* MazeView::getTextureCpuBuffer() const {
    return &m_textureCpuBuffer;
}

void MazeView::initText(int numRows, int numCols) {

    // Initialze the tile text in the buffer class,
    // do caching for speed improvement
    m_bufferInterface.initTileGraphicText(
        Distance::Meters(P()->wallLength()),
        Distance::Meters(P()->wallWidth()),
        {numRows, numCols}
    );
        
    // TODO: MACK - this is kind of confusing
    // - I should insert and then update (get rid of draw method)
    m_textureCpuBuffer.clear();
    m_mazeGraphic.drawTextures();
}

} // namespace mms
