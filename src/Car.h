#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <string>

;
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
		TRAIN,
		COUNT
	};

    /// \brief Constructor used to create empty cars. Do not use in spawning!
    Car() : animSize(1)
    {
        this->typeId = NORMAL;
    }
	Car(Car::TypeId id, float speed, int line);
	virtual ~Car() {}
    float getSpeed();
    int getLine();
    void setColor(sf::Color color);
    void move(float gameSpeed);
    sf::Color getColor();
    void makeDestroy(float count = 1.f);
    bool tickDestroy();
    bool isCrashedWith(Car* car);
    void setMaxHealth(float maxH);

	//events
	/// Called when the car is created
	virtual void onCreate(Game* game);

	/// Called when the player clicks on the car
	virtual void onDamage(Game* game);

	/// Called when the car is destroyed (by player!)
	virtual void onDestroy(Game* game);

	/// Called every tick
	virtual void onUpdate(Game* game);

	/// called when the car leaves the screen, normally indicates that the game is over.
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
    float health;
    float maxHealth;
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
};

#endif // CAR_H
