#include <Env/World.hpp>
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <fstream>
#include <string>



void World::reloadConfig()
{
  this->nbCells_ = getAppConfig().world_cells;
  this->cells_.assign(this->nbCells_ * this->nbCells_, Kind::Roche);
  this->cellsSize_ = getAppConfig().world_size/nbCells_;
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

//fonction update modifiée j’ai commencé à la modifier mais je ne comprends pas ce qu’il faut mettre dans les parenthèses du switch, aussi il me met des erreurs pour mes boucles for auto du coup dis moi si t arrives à régler ces erreurs


void World::updateCache()
{
  renderingCache_.clear();
  sf::RenderStates rs;
  for (int i(0); i<nbCells_; ++i) {
      for (int j(0); j<nbCells_; ++j) {
           switch(cells_[i+j*cellsSize_]) {
           case Kind::Eau :
               for (auto ind:indexesForCellVertexes(i, j, nbCells_ )) {
                  waterVertexes_[ind].color.a = 255;
                  grassVertexes_[ind].color.a = 0;
                  rockVertexes_[ind].color.a = 0;
               }
               break;
           case Kind::Roche :
               for (auto ind:indexesForCellVertexes(i, j,   nbCells_ )) {
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
      std::cout <<"Lecture fichier : "<< nom << std::endl;
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
   std::cout<< nbCells_ << " " << cellsSize_ << " " << cells_.size() << std::endl;

}



