#pragma once

#include <vector>
#include "PhysicsHandler.h"

class PhysicsEngine {
public:
    PhysicsEngine() {}

    void addObject(float x, float y, float mass, int width, int height, float restitution = 0.5f);

    void update(float deltaTime, int windowHeight, int windowWidth);

    const std::vector<PhysicsHandler>& getObjects() const; // Getter for objects

private:
    std::vector<PhysicsHandler> objects;

    void handleCollisions();
};

void PhysicsEngine::addObject(float x, float y, float mass, int width, int height, float restitution) {
    objects.emplace_back(x, y, mass, width, height, restitution);
}

void PhysicsEngine::update(float deltaTime, int windowHeight, int windowWidth) {
    // Update all objects
    for (auto& obj : objects) {
        obj.update(deltaTime);

        // Check for collision with ground
        if (obj.checkCollisionWithBorders(windowHeight, windowWidth)) {
            obj.resolveCollisionWithBorders(windowHeight, windowWidth);
        }
    }

    // Handle collisions
    handleCollisions();
}

void PhysicsEngine::handleCollisions() {
    // Check collisions between all pairs of objects
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i].checkCollision(objects[j])) {
                objects[i].resolveCollision(objects[j]);
            }
        }
    }
}

const std::vector<PhysicsHandler>& PhysicsEngine::getObjects() const {
    return objects;
}