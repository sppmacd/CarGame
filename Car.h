#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <string>

using std::string;

class Car
{
public:
    /// \brief Constructor used to create empty cars. Do not use in spawning!
    Car()
    {
        this->typeId = NORMAL;
    }
    Car(float speed, int line);
    float getSpeed();
    int getLine();
    void setColor(sf::Color color);
    void move(float gameSpeed);
    sf::Color getColor();
    void makeDestroy();
    bool tickDestroy();
    bool isCrashedWith(Car* car);

    float getPos()
    {
        return this->pos;
    }
    bool isDestroying()
    {
        return this->destroyTick >= 0;
    }
    void setToErase()
    {
        this->canErase = true;
    }
    sf::Vector2f getScreenPos();

    void setSpeed(float speed);

    bool canErase;
    int health;
    int maxHealth;
    float carRelativeToScreen;
    float destroyTick;
    int typeId;

    enum TypeId
    {
        NORMAL,
        LORRY,
        RARE,
        BUS,
        AMBULANCE,
        COUNT
    };

    string getTextureName();

protected:
    string textureName;

private:
    float carSpeed;
    int lineIn;
    sf::Color colorMultiplier;
    float pos;
};

#endif // CAR_H
