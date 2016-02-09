#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>
USING_NS_CC;


class BackupDancer
{
public:
	Sprite* sprite_zombie;
	int direction;
	BackupDancer(int d){
		//sprite_zombie->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		sprite_zombie = Sprite::create("zombie.png", Rect(0, 0, 64, 64));
		direction = d;
		switch (d){
		case 1:
			sprite_zombie->setRotation(0);
			break;
		case 2:
			sprite_zombie->setRotation(90);
			break;
		case 3:
			sprite_zombie->setRotation(180);
			break;
		case 4:
			sprite_zombie->setRotation(270);
			break;
		}
	}
};

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

	std::vector<BackupDancer> bodyarray;

    virtual bool init();	
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	//sets the direction of snake
	int direction;
	bool setDirection;
	int delay;
	int score;
	//updates movement
	void update(float d);
	void grow();
	Vec2 randomizeLocation();
	bool stop;
	bool GoToGameScene();
	Label* label = Label::createWithTTF("Zombie Conga Line", "fonts/Marker Felt.ttf", 24);
	Sprite* sprite_zombie = Sprite::create("zombiehead.png");
	Sprite* sprite_food = Sprite::create("food.png");
};

#endif // __HELLOWORLD_SCENE_H__
