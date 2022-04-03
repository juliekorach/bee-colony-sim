#pragma once

#include <vector>


#include <SFML/Graphics.hpp>

enum class Kind : short {Herbe,Eau,Roche};


struct Seed
{
    sf::Vector2i coordinates;
    Kind type;
};


class World

{
public:

    void reloadConfig();

    void drawOn(sf::RenderTarget& target);

    void reloadCacheStructure();

    void updateCache();

    double getSize() const;

    void reset(bool regenerate);

    void loadFromFile();

    void step();

    void clamp_seed(Seed &seed);

    void move(Seed &seed);

    void steps(int nb, bool regenerate=0);

    void setCellType(int x, int y, Kind kind);

    void smooth();

private:

    std::vector<Kind> cells_;

    int nbCells_;

    double cellsSize_;

    std::vector <sf::Vertex> grassVertexes_;

    std::vector <sf::Vertex> waterVertexes_;

    std::vector <sf::Vertex> rockVertexes_;

    sf::RenderTexture renderingCache_;

    std::vector<Seed> seeds_;

    int nbWaterSeeds_;

    int nbGrassSeeds_;

    int index(int i, int y);
};

