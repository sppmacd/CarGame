#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <string>

class Game;
class CarType;

using std::string;

class Car
{
public:
	enum TypeId
	{
		NORMAL,
		LORRY,
		RARE,
		BUS,
		AMBULANCE,
		RALLY,
		FIREMAN,
		TANK,
		OLD,
		BOMB,
		ARMORED,
		COUNT
	};

    /// \brief Constructor used to create empty cars. Do not use in spawning!
    Car() : animSize(1)
    {
        this->typeId = NORMAL;
    }
	Car(Car::TypeId id, float speed, int line);
    float getSpeed();
    int getLine();
    void setColor(sf::Color color);
    void move(float gameSpeed);
    sf::Color getColor();
    void makeDestroy();
    bool tickDestroy();
    bool isCrashedWith(Car* car);

	//events
	/// called when the car is created
	virtual void onCreate(Game* game);

	/// called when the player clicks on the car
	virtual void onDamage(Game* game);

	/// called when the car has 0 HP
	virtual void onDestroy(Game* game);

	/// called every tick
	virtual void onUpdate(Game* game);

	/// called when the car leaves the screen
	virtual void onLeave(Game* game);

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
    float pos;
    float destroyTick;
	int animSize;
	int frameLength;

	TypeId typeId;
	CarType* type;
    string getTextureName();

protected:
    string textureName;

private:
    float carSpeed;
    int lineIn;
    sf::Color colorMultiplier;
    //float pos;
};

#endif // CAR_H
