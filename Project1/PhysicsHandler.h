#pragma once

class PhysicsHandler {
public:
    float x, y;         // Position
    float vx, vy;       // Velocity
    float ax, ay;       // Acceleration
    float mass;         // Mass
    float restitution;  // Restitution coefficient (bounciness)
    int width, height;  // Dimensions

    PhysicsHandler(float x, float y, float mass, int width, int height, float restitution = 0.5f)
        : x(x), y(y), vx(0), vy(0), ax(0), ay(GRAVITY), mass(mass), width(width), height(height), restitution(restitution) {}

    void update(float deltaTime);

    bool checkCollision(const PhysicsHandler& other) const;

    void resolveCollision(PhysicsHandler& other);

    bool checkCollisionWithBorders(int windowHeight, int windowWidth) const;
    void resolveCollisionWithBorders(int windowHeight, int windowWidth);

private:
    static const float GRAVITY; // Gravity constant
};

// Definition of the gravity constant
const float PhysicsHandler::GRAVITY = 9.8f;

void PhysicsHandler::update(float deltaTime) {
    vx += ax * deltaTime;
    vy += ay * deltaTime;
    x += vx * deltaTime;
    y += vy * deltaTime;
}

bool PhysicsHandler::checkCollision(const PhysicsHandler& other) const {
    return (x < other.x + other.width &&
        x + width > other.x &&
        y < other.y + other.height &&
        y + height > other.y);
}

void PhysicsHandler::resolveCollision(PhysicsHandler& other) {
    // Calculate relative velocity
    float dx = other.vx - vx;
    float dy = other.vy - vy;

    // Calculate relative velocity in terms of the normal direction
    float nx = other.x - x;
    float ny = other.y - y;

    // Prevent accidental overlap
    if (nx == 0 && ny == 0) {
        nx = 1.0f;
    }

    // Normalize the normal
    float length = sqrt(nx * nx + ny * ny);
    nx /= length;
    ny /= length;

    // Calculate relative velocity in terms of normal direction
    float vn = dx * nx + dy * ny;

    // Do not resolve if velocities are separating
    if (vn > 0.0f) {
        return;
    }

    // Calculate impulse scalar
    float e = std::min(restitution, other.restitution);
    float j = -(1.0f + e) * vn / (1.0f / mass + 1.0f / other.mass);

    // Apply impulse
    float jx = j * nx;
    float jy = j * ny;
    vx -= jx / mass;
    vy -= jy / mass;
    other.vx += jx / other.mass;
    other.vy += jy / other.mass;
}

bool PhysicsHandler::checkCollisionWithBorders(int windowHeight, int windowWidth) const {
    return (x < 0 || x + width > windowWidth || y < 0 || y + height > windowHeight);
}

void PhysicsHandler::resolveCollisionWithBorders(int windowHeight, int windowWidth) {
    // Resolve collision with window borders
    if (x < 0) {
        x = 0;
        vx = -vx * restitution; //Reverse velocity with some energy loss
    }
    if (x + width > windowWidth) {
        x = windowWidth - width;
        vx = -vx * restitution;
    }
    if (y < 0) {
        y = 0;
        vy = -vy * restitution;
    }
    if (y + height > windowHeight) {
        y = windowHeight - height;
        vy = -vy * restitution;
    }
}