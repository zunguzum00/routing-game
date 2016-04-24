#include "Map.hpp"
#include "Renderer.hpp"
#include "Tile.hpp"
#include <cassert>

void Map::generate(unsigned w, unsigned h)
{
    m_tiles.assign(w * h, 0u);
    m_width = w;
    m_height = h;

    m_alwaysenergizedx = 2u;
    m_alwaysenergizedy = 2u;

    freshFlood();
}

void Map::render(Renderer& renderer)
{
    for(unsigned x = 0u; x < m_width; ++x)
    {
        for(unsigned y = 0u; y < m_height; ++y)
        {
            renderer.renderTile(x, y, getTile(x, y));
        }
    }
    renderer.renderPowerSource(m_alwaysenergizedx, m_alwaysenergizedy);
}

unsigned Map::getWidth() const
{
    return m_width;
}

unsigned Map::getHeight() const
{
    return m_height;
}

void Map::rotateTileRight(unsigned x, unsigned y)
{
    if(!(x < getWidth() && y < getHeight()))
        return;

    unsigned& t = getTile(x, y);
    const unsigned oldt = t;

    //clear connections
    t &= ~ETF_CONNECT_ALL;

    if(oldt & ETF_CONNECT_NORTH)
        t |= ETF_CONNECT_EAST;

    if(oldt & ETF_CONNECT_EAST)
        t |= ETF_CONNECT_SOUTH;

    if(oldt & ETF_CONNECT_SOUTH)
        t |= ETF_CONNECT_WEST;

    if(oldt & ETF_CONNECT_WEST)
        t |= ETF_CONNECT_NORTH;

    freshFlood();
}

unsigned& Map::getTile(unsigned x, unsigned y)
{
    assert(x < getWidth() && y < getHeight());
    return m_tiles[x + y * m_height];
}

void Map::freshFlood()
{
    for(unsigned i = 0u; i < m_tiles.size(); ++i)
        m_tiles[i] &= ~ETF_ENERGIZED;

    m_energized = 0u;
    flood(m_alwaysenergizedx, m_alwaysenergizedy);
}

void Map::flood(unsigned x, unsigned y)
{
    unsigned& t = getTile(x, y);
    if(t & ETF_ENERGIZED)
        return;

    t |= ETF_ENERGIZED;
    ++m_energized;

    const int off[4][4] = {
        {0, -1, ETF_CONNECT_NORTH, ETF_CONNECT_SOUTH},
        {0, 1, ETF_CONNECT_SOUTH, ETF_CONNECT_NORTH},

        {1, 0, ETF_CONNECT_EAST, ETF_CONNECT_WEST},
        {-1, 0, ETF_CONNECT_WEST, ETF_CONNECT_EAST},
    };

    for(int i = 0; i < 4; ++i)
    {
        const unsigned nx = x + off[i][0];
        const unsigned ny = y + off[i][1];
        if(nx < getWidth() && ny < getHeight())
        {
            unsigned t2 = getTile(nx, ny);
            if((t & off[i][2]) && (t2 & off[i][3]))
                flood(nx, ny);
        }
    }
}

unsigned Map::getEnergizedTiles() const
{
    return m_energized;
}

unsigned Map::getTotalTiles() const
{
    return m_width * m_height;
}
