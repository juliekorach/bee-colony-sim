#include <Env/World.hpp>
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <fstream>
#include <string>
#include <Random/Random.hpp>


int World::index(int x, int y)
{
    return y * nbCells_ + x;
}

void World::reloadConfig()
{
    this->nbCells_ = getAppConfig().world_cells;
    this->cells_.assign(this->nbCells_ * this->nbCells_, Kind::Roche);
    this->cellsSize_ = getAppConfig().world_size/nbCells_;
    this->nbGrassSeeds_ = getAppConfig().world_nb_grass_seeds;
    this->nbWaterSeeds_ = getAppConfig().world_nb_water_seeds;
    std::vector<Seed> seeds(nbGrassSeeds_ + nbWaterSeeds_);
    this->seeds_=seeds;

}

void World::drawOn(sf::RenderTarget& target)
{
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}

void World::reloadCacheStructure()
{
    grassVertexes_ = generateVertexes(
                getValueConfig()["simulation"]["world"]["textures"],
            nbCells_,
            cellsSize_);

    waterVertexes_ = generateVertexes(
                getValueConfig()["simulation"]["world"]["textures"],
            nbCells_,
            cellsSize_);

    rockVertexes_ = generateVertexes(
                getValueConfig()["simulation"]["world"]["textures"],
            nbCells_,
            cellsSize_);

    renderingCache_.create(nbCells_*cellsSize_, nbCells_*cellsSize_);
}



void World::updateCache()
{
    renderingCache_.clear();
    sf::RenderStates rs;

    for (int i(0); i<nbCells_; ++i)
    {
        for (int j(0); j<nbCells_; ++j)
        {
            switch(cells_[index(i,j)])
            {
            case Kind::Eau :
                for (auto ind:indexesForCellVertexes(i, j, nbCells_ )) {
                    waterVertexes_[ind].color.a = 255;
                    grassVertexes_[ind].color.a = 0;
                    rockVertexes_[ind].color.a = 0;
                }
                break;
            case Kind::Roche :
                for (auto ind:indexesForCellVertexes(i, j, nbCells_ )) {
                    waterVertexes_[ind].color.a = 0;
                    grassVertexes_[ind].color.a = 0;
                    rockVertexes_[ind].color.a = 255;
                }
                break;
            case Kind::Herbe :
                for (auto ind:indexesForCellVertexes(i, j, nbCells_ )) {
                    waterVertexes_[ind].color.a = 0;
                    grassVertexes_[ind].color.a = 255;
                    rockVertexes_[ind].color.a = 0;
                }
                break;
            }
        }
    }

    rs.texture = &getAppTexture(getAppConfig().rock_texture); // texture liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(getAppConfig().water_texture); // texture liée à l'eau
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(getAppConfig().grass_texture); // texture liée à l'herbe
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs);
    renderingCache_.display();

}

void World::reset(bool regenerate)
{
    reloadConfig();
    reloadCacheStructure();

    for(int h(0); h < nbGrassSeeds_ + nbWaterSeeds_; ++h)
    {
        int x = uniform(0,nbCells_-1);
        int y = uniform(0,nbCells_-1) ;
        sf::Vector2i co(x,y);
        seeds_[h].coordinates = co;
        if (h < nbGrassSeeds_)
        {
            seeds_[h].type = Kind::Herbe;
            cells_[index(x, y)] = Kind::Herbe;
        }
        else
        {
            seeds_[h].type = Kind::Eau;
            cells_[index(x, y)] = Kind::Eau;
        }
    }
    updateCache();
}

double World::getSize() const
{
    return nbCells_*cellsSize_;
}

void World::loadFromFile()
{
    std::string line1;
    std::string line2;
    std::ifstream in;
    std::string nom(getApp().getResPath() + getAppConfig().world_init_file);
    in.open(nom.c_str());
    if (!in.fail()){
        std::cout << "fichier map = " << nom << std::endl;
        in >> std::ws >> line1;
        nbCells_ = std::stoi(line1);
        in >> std::ws >> line2;
        cellsSize_ = std::stod(line2);
        cells_= std::vector<Kind>(nbCells_*nbCells_);
        for(size_t i(0); i<(cells_.size)(); ++i)
        {
            short var;
            in >> std::ws >> var;
            cells_[i] = static_cast<Kind>(var);

        }
    }
    else
    {
        throw std::runtime_error("file does not exist");
    }
    in.close();
    reloadCacheStructure();
    updateCache();
}

void World::clamp_seed(Seed &seed)
{
    auto coord = seed.coordinates;
    seed.coordinates.x = std::min(nbCells_ - 1, coord.x);
    seed.coordinates.x = std::max(0, coord.x);
    seed.coordinates.y = std::min(nbCells_ - 1, coord.y);
    seed.coordinates.y = std::max(0, coord.y);
}

void World::move(Seed &seed)
{
    int x;
    int y;
    do {
        x = (uniform(-1,1));
        y = (uniform(-1,1));

    } while (not(abs(x-y)==1));

    seed.coordinates.x += x;
    seed.coordinates.y += y;
    clamp_seed(seed);
    // std::cout << "Moved seed to " << seed.coordinates.x << "," << seed.coordinates.y << std::endl;
    setCellType(seed.coordinates.x, seed.coordinates.y, seed.type);
}

void World::setCellType(int x, int y, Kind kind)
{
    int idx = index(x, y);
    if (cells_[idx] != Kind::Eau)
    {
        cells_[idx] = kind;
    }
}

void World::step()
{
    double proba = getAppConfig().water_seeds_teleport_proba;
    for (size_t  i(0); i < seeds_.size(); ++i)
    {
        if (seeds_[i].type == Kind::Herbe)
        {
            move(seeds_[i]);
        }
        else if (seeds_[i].type == Kind::Eau)
        {
            if (bernoulli(proba))
            {
                int x = uniform(0, nbCells_-1);
                int y = uniform(0, nbCells_-1) ;
                sf::Vector2i co(x, y);
                seeds_[i].coordinates = co;
                setCellType(x, y, Kind::Eau);
            }
            else {
                move(seeds_[i]);
            }
        }
    }
}



void World::steps(int nb, bool regenerate)
{
    for ( int i(0); i < nb; ++i)
    {
        //std::cout << "Running step " << i+1 << std::endl;
        step();
    }
    if (regenerate)
    {
        updateCache();
    }
}

double World::neighboring_ratio(int x,int y,std::vector<Kind> cells)
{
    int countWater(0);
    int countNeighbors(0);
    for (int j(y-1); j<=(y+1); ++j)
    {
        if ( j >= 0 and j < nbCells_-1)
        {
            for (int i(x-1); i<=(x+1); ++i)
            {
                if ( i >= 0 and i < nbCells_-1)
                   {
                    countNeighbors += 1;
                    if (cells[index(i,j)]==Kind::Eau)
                        countWater += 1;
                }
            }
        }
    }

    return (1.0*countWater/countNeighbors);
}

    void World::smooth()
    {
        auto cells_copy = cells_;
        for (int i(0); i < cellsSize_-1; ++i)
        {

            if (cells_copy[i]==Kind::Roche or cells_copy[i]==Kind::Herbe)
            {
                int y = i/nbCells_;
                int x = i%nbCells_;
                double ratio = neighboring_ratio(x, y, cells_copy);

                if(ratio < getAppConfig().smoothness_water_neighbor_ratio)
                {
                    cells_copy[i] = Kind::Eau;
                }
            }
        }
    }

