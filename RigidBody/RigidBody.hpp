#pragma once

#include "../Hitbox/Hitbox.hpp"
#include "../Sprite/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include <vector>

namespace game {

enum class objectClass { wall, player, portal, cube };

class RigidBody : public Sprite {
public:
  objectClass objClass;
  bool isStatic, isDestroyed;
  sf::Vector2f velocity;
  const float mass, inverseMass;
  std::vector<Hitbox> hitboxes;

  RigidBody(objectClass objClass_, const std::string &textureName,
            const Textures &textures, bool isStatic_ = false, float mass_ = 1.f,
            float bounciness_ = 1.f);

  virtual ~RigidBody() = default;

  void applyForce(const sf::Vector2f &force);

  virtual void step(float stepSize);

  void setBounciness(float bounciness_);

  float getBounciness() const;

  void setPosition(const sf::Vector2f &position);

  void setPosition(float x, float y);

  void move(const sf::Vector2f &offset);

  void move(float x, float y);

  void setSize(const sf::Vector2f &size);

  void setSize(float x, float y);

  void setScale(const sf::Vector2f &scale);

  void setScale(float x, float y);

  bool intersects(const RigidBody &other);

  virtual void handleHitboxesCollision(RigidBody &otherRigidBody,
                                       const Hitbox &otherHitbox,
                                       const sf::Vector2f &normal);

private:
  sf::Vector2f force;
  float bounciness;
};

} // namespace game
