#pragma once

#include "Hitbox.hpp"
#include "Object.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/System/Vector2.hpp"
#include <vector>

namespace game {

class RigidBody : public Object, public sf::Transformable {
public:
  bool isStatic, isDestroyed;
  sf::Vector2f velocity;
  const float mass, inverseMass;
  std::vector<Hitbox> hitboxes;

  RigidBody(ObjectClass objClass, bool isStatic_ = false, float mass_ = 1.f,
            float bounciness_ = 1.f);

  virtual ~RigidBody() = default;

  void applyForce(const sf::Vector2f &force);

  virtual void physicsStep(float stepSize);

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

  bool intersects(const RigidBody &other) const;

  virtual void handleHitboxesCollision(RigidBody &otherRigidBody,
                                       size_t otherHitboxIdx,
                                       const sf::Vector2f &normal) = 0;

  virtual void handleTeleport(float teleportAngle) = 0;

protected:
  sf::Vector2f force;

  void addHitboxFromSprite(const sf::Sprite &sprite);

private:
  float bounciness;
};

} // namespace game
