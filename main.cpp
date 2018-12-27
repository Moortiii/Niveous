#include <iostream>
#include <Box2D/Box2D.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    // Initialize a Box2D World with standard gravity
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    // Define the properties of our ground
    b2BodyDef groundDef;
    groundDef.position.Set(0.0f, -10.0f);

    // Create a body that represents our ground in the game
    b2Body* groundBody = world.CreateBody(&groundDef);

    // Create the box around the ground
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the properties of our dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);

    // Create a body that represents our dynamic element in the game
    b2Body* body = world.CreateBody(&bodyDef);

    // Create the box around the dynamic element
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Attach a fixture to the dynamic body
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    // Create the fixture
    body->CreateFixture(&fixtureDef);

    // Timestep defaults to 60 times per second
    float32 timeStep = 1.0f / 60.0f;

    // The iterative solver needs to know how precise it should attempt to be
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i = 0; i < 60; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float32 angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }

    return 0;
}