#pragma once

#include <Utility/Vec2d.hpp>
#include <ostream>

Vec2d clamp(Vec2d const& center);
/**
* @brief put the point inside the toric world
*
* @param center of the collider
*
* @return the point put inside the toric world
*/

class Collider
{

public:

  Collider(Vec2d const& center, double const& radius);
  /**
    * @brief Constructor
    *
    * @param center of collider
    * @param radius of collider
    */

  Collider(const Collider& co);
  /**
    * @brief copy constructor
    *
    * @param Collider to copy
    */

~Collider()=default;
  /**
    * @brief destructor
    */

  double getRadius()const;
  /**
    * @brief getter
    *
    * @return radius
    */

  const Vec2d& getPosition()const;
  /**
    * @brief getter
    *
    * @return position
    */

  Collider& operator=(Collider const&);
  /**
    * @brief overload of =
    *
    * @param collider
    *
    * @return new collider
    */

  Vec2d directionTo(const Vec2d to) const;
  /**
    * @brief computes the vector between the center and an other point, to
    *
    * @param other point, to
    *
    * @return the vector
    */

  Vec2d directionTo(const Collider& co) const;
  /**
    * @brief computes the vector between the center and an other center
    *
    * @param collider
    *
    * @return the vector
    */

  double distanceTo(const Vec2d to) const;
  /**
    * @brief computes the distance between the center and an other point, to
    *
    * @param vec2d to
    *
    * @return the distance
    */

  double distanceTo(const Collider& co) const;
  /**
    * @brief computes the distance betweem 2 centers
    *
    * @param collider
    *
    * @return the distance
    */

  void move(const Vec2d& dx);
  /**
    * @brief moves collider by distance dx
    *
    * @param vec2d dx
    */

  Collider& operator+=(const Vec2d& dx);
  /**
    * @brief overload operator +=
    *
    * @param dx
    *
    * @return collider moved by dx
    */

  bool isColliderInside(const Collider& other) const;
  /**
    * @brief tests if another collider is inside the collider
    *
    * @param other collider
    *
    * @return true or false
    */

  bool isColliding(const Collider& other) const;
  /**
    * @brief tests if an other collider touches the collider
    *
    * @param other collider
    *
    * @return true or false
    */

  bool isPointInside(const Vec2d& point) const;
  /**
    * @brief tests if one point is inside the collider
    *
    * @param point
    *
    * @return true or false
    */

  bool operator>(const Collider& other ) const;
  /**
    * @brief tests if there is a other collider inside the collider
    *
    * @param other collider
    *
    * @return true or false
    */


  bool operator|(const Collider& other )const;
  /**
    * @brief tests if there is a collision between two colliders
    *
    * @param other collider
    *
    * @return true or false
    */

  bool operator>(const Vec2d& point) const;
  /**
    * @brief tests if point is inside the collider
    *
    * @param vec2d point
    *
    * @return true or false
    */

  void affiche(std::ostream& o) const;
  /**
    * @brief dsiplay on the terminal
    *
    * @param ostream o
    */


private:
  Vec2d center;
  double radius;

};

std::ostream& operator<<(std::ostream& sortie, const Collider& co);
 /**
* @brief display on the terminal the position and radius of the collider in the desired format
*
* @param ostream sortie, collider co
*
* @return ostream
*/

