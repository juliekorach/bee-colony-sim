#pragma once

#include <vector>


#include <SFML/Graphics.hpp>

enum class Kind : short {Herbe,Eau,Roche};

class World

{
public:

    void reloadConfig();

    void drawOn(sf::RenderTarget& target);

    void reloadCacheStructure();

    void updateCache();

    void reset(bool regenarate);

    double getSize() const;

    void loadFromFile();



private:


    std::vector<Kind> cells_;

    int nbCells_;

    double cellsSize_;

    std::vector <sf::Vertex> grassVertexes_;

    std::vector <sf::Vertex> waterVertexes_;

    std::vector <sf::Vertex> rockVertexes_;

    sf::RenderTexture renderingCache_;

    void reset();
};

