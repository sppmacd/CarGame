#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <string>

class Game;
class CarType;

using std::string;

// The class representing one car spawned on screen.
// Car states:
// "dead" - {canErase = true} - The car can be removed from game (deallocated).
// "destroyed" - {destroyTick > -1 && !canErase} - The destroy animation is currently played.
// "alive" - {destroyTick == -1) - You can destroy the car.
class Car
{
public:
    // Constructor used to create empty cars. Do not use in spawning!
	/*Car() : animSize(1)
	{
		this->typeId = "";
	}*/

	// Don't need to replace all occurrences, just map TypeId to std::string :)
	typedef std::string TypeId;

	// %id - The car type ID.
	// %speed - The car speed. It's constant because the game speed is controlled from <Game> class.
	// %line - The car line (0-2).
	Car(Car::TypeId id, float speed, int line);

	virtual ~Car() {}

	// \returns Speed of the car (set on spawn)
    float getSpeed();

	// \returns The line the car belongs to.
    int getLine();

	// Sets the car color to %color. Car color is multiplied by texture background color (not animated).
    void setColor(sf::Color color);

	// Perform an one-tick car movement.
	// %gameSpeed - The current game speed, passed from <Game>.
    void move(float gameSpeed);

	// \returns the car color.
    sf::Color getColor();

	// Causes car damage with value of %count. If health < 0, sets car to "destroyed" state.
    void makeDestroy(float count = 1.f);

	// Performs a tick while car is in "destroyed" state. If <destroyTick> reaches 0, sets car on "dead" state.
	// \returns True if the animation has ended and the car can be removed from memory.
    bool tickDestroy();

	// \deprecated
	// \returns True if %car is collided with this car.
    bool isCrashedWith(Car* car);

	// Set car max health to %maxH.
    void setMaxHealth(float maxH);

	////////////////
	//// Events ////
	////////////////

	// Called when the car is created.
	virtual void onCreate(Game* game);

	// Called when the player clicks on the car.
	virtual void onDamage(Game* game);

	// Called when the car is destroyed (by player!).
	virtual void onDestroy(Game* game);

	// Called every tick.
	virtual void onUpdate(Game* game);

	// Called when the car leaves the screen, normally indicates that the game is over.
	virtual void onLeave(Game* game);

	// \returns Current car position (on the world).
    float getPos()
    {
        return this->pos;
    }

	// \returns True if the car is in "destroyed" state.
    bool isDestroying()
    {
        return this->destroyTick >= 0;
    }

	// Sets car in "dead" state, notifying game that the car can be removed.
    void setToErase()
    {
        this->canErase = true;
    }

	// Returns the screen position of this car.
    sf::Vector2f getScreenPos();

	// Sets the current car speed to %speed. Currently not used.
    void setSpeed(float speed);

	// <true> if in "dead" state.
    bool canErase;

	// Current car health.
    float health;

	// Car max health.
    float maxHealth;

	// Current world position.
    float pos;

	// Destroy tick (used in tickDestroy()).
    float destroyTick;

	// The size (frame count) of animation.
	int animSize;

	// The frame length, in ticks. Total animation size: animSize*frameLength.
	int frameLength;

	// The car ID.
	TypeId typeId;

	// The <CarType> which holds the static car info (rarity etc.)
	CarType* type;

	// Gets the texture name of car. It will be added to "/res/car/".
    string getTextureName();

protected:
	// Texture name, it will be added to "/res/car/".
    string textureName;

private:
	// Current car speed.
    float carSpeed;

	// Car line.
    int lineIn;

	// Car color multiplier.
    sf::Color colorMultiplier;
};

#endif // CAR_H
