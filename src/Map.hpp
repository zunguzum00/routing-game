/* 
 * File:   Map.hpp
 * Author: frex
 *
 * Created on April 24, 2016, 6:58 PM
 */

#ifndef MAP_HPP
#define	MAP_HPP

#include <vector>

class Renderer;

class Map
{
public:
    void generate(unsigned w, unsigned h, unsigned seed);
    void render(Renderer& renderer);
    unsigned getWidth() const;
    unsigned getHeight() const;
    void rotateTileRight(unsigned x, unsigned y);

    unsigned getEnergizedTiles() const;
    unsigned getTotalTiles() const;

    unsigned getSeed() const;

    void randomlyRotate(unsigned seed);

private:
    unsigned& getTile(unsigned x, unsigned y);
    void freshFlood();
    void flood(unsigned x, unsigned y);
    void rawRotateTileRight(unsigned x, unsigned y);

    std::vector<unsigned> m_tiles;
    unsigned m_width;
    unsigned m_height;
    unsigned m_alwaysenergizedx;
    unsigned m_alwaysenergizedy;
    unsigned m_energized;
    unsigned m_seed;

};

#endif	/* MAP_HPP */

