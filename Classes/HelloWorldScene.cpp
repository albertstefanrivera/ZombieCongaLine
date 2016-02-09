#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	direction = 1;
	delay = 5;
	stop = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	score = 0;
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...
	
	// add a label shows "Hello World"
	// create and initialize a label

	

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "HelloWorld" splash screen"
	auto sprite2 = Sprite::create("land.png");

	// position the sprite on the center of the screen
	sprite2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite2, 0);

	

	//wall
	for (int i = 0; i <= 16; i++){
		for (int j = 0; j <= 12; j++){
			if (i == 0 || i == 16 || j == 0 || j == 12){
				auto wall = Sprite::create("wall.png");
				this->addChild(wall);
				wall->setPosition(Vec2(i * 64, j * 64));
			}
		}
	}

	//Movement Code that can be found in tutorial
	
	sprite_zombie->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);

	this->addChild(sprite_zombie, 0);
	grow();
	grow();

	//food
	sprite_food->setPosition(randomizeLocation());
	this->addChild(sprite_food);

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			if (direction != 2 && setDirection){
				direction = 4;
				event->getCurrentTarget()->setRotation(270);
				setDirection = false;
			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			if (direction != 4 && setDirection){
				direction = 2;
				event->getCurrentTarget()->setRotation(90);
				setDirection = false;
			}
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			if (direction != 3 && setDirection){
				direction = 1;
				event->getCurrentTarget()->setRotation(0);
				setDirection = false;
			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			if (direction != 1 && setDirection){
				direction = 3;
				event->getCurrentTarget()->setRotation(180);
				setDirection = false;
			}
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			GoToGameScene();
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, sprite_zombie);
	//Time Ticker
	this->scheduleUpdate();
	
	return true;
}

void HelloWorld::grow(){
	auto spawndir = direction;
	if (bodyarray.size() != 0)
		spawndir = bodyarray.at(bodyarray.size() - 1).direction;

	auto temp = BackupDancer(spawndir);

	//no body yet
	Vec2 loc;
	auto dir = direction;
	if (bodyarray.size() == 0)
		loc = sprite_zombie->getPosition();
	else{
		loc = bodyarray.at(bodyarray.size() - 1).sprite_zombie->getPosition();
		dir = bodyarray.at(bodyarray.size() - 1).direction;
	}

	auto size = 64;
	switch (dir){
	case 1: //up
		
		temp.sprite_zombie->setPosition(loc.x, loc.y - size);
		break;
	case 2: //right
		temp.sprite_zombie->setPosition(loc.x - size, loc.y);
		break;
	case 3: //down
		temp.sprite_zombie->setPosition(loc.x, loc.y + size);
		break;
	case 4: //left
		temp.sprite_zombie->setPosition(loc.x + size, loc.y);
		break;
	}

	this->addChild(temp.sprite_zombie);
	bodyarray.push_back(temp);
}

//move forwardcode
void HelloWorld::update(float d){
	if (!stop){
		auto spd = 64;
		setDirection = true;
		Vec2 loc = sprite_zombie->getPosition();
		switch (direction){
		case 1: //up
			if (loc.y < 768)
				sprite_zombie->setPosition(loc.x, loc.y += spd);
			sprite_zombie->setRotation(0);
			break;
		case 2: //right
			if (loc.x < 1024)
				sprite_zombie->setPosition(loc.x += spd, loc.y);
			sprite_zombie->setRotation(90);
			break;
		case 3: //down
			if (loc.y > 0)
				sprite_zombie->setPosition(loc.x, loc.y -= spd);
			sprite_zombie->setRotation(180);
			break;
		case 4: //left
			if (loc.x > 0)
				sprite_zombie->setPosition(loc.x -= spd, loc.y);
			sprite_zombie->setRotation(270);
			break;
		}

		for (int i = bodyarray.size() - 1; i >= 1; i--){
			//CCLOG("Current %d", i);
			auto temp = bodyarray.at(i).sprite_zombie;

			auto dir = bodyarray.at(i).direction;

			//bodyarray.at(i).direction;

			Vec2 loc = temp->getPosition();
			switch (dir){
			case 1: //up
				if (loc.y < 768)
					temp->setPosition(loc.x, loc.y += spd);
				temp->setRotation(0);
				break;
			case 2: //right
				if (loc.x < 1024)
					temp->setPosition(loc.x += spd, loc.y);
				temp->setRotation(90);
				break;
			case 3: //down
				if (loc.y > 0)
					temp->setPosition(loc.x, loc.y -= spd);
				temp->setRotation(180);
				break;
			case 4: //left
				if (loc.x > 0)
					temp->setPosition(loc.x -= spd, loc.y);
				temp->setRotation(270);
				break;
			}
			bodyarray.at(i).direction = bodyarray.at(i - 1).direction;

		}

		if (bodyarray.size() > 0){
			auto temp = bodyarray.at(0).sprite_zombie;
			auto dir = bodyarray.at(0).direction;

			Vec2 loc = temp->getPosition();
			switch (dir){
			case 1: //up
				if (loc.y < 768)
					temp->setPosition(loc.x, loc.y += spd);
				temp->setRotation(0);
				break;
			case 2: //right
				if (loc.x < 1024)
					temp->setPosition(loc.x += spd, loc.y);
				temp->setRotation(90);
				break;
			case 3: //down
				if (loc.y > 0)
					temp->setPosition(loc.x, loc.y -= spd);
				temp->setRotation(180);
				break;
			case 4: //left
				if (loc.x > 0)
					temp->setPosition(loc.x -= spd, loc.y);
				temp->setRotation(270);
				break;
			}
			bodyarray.at(0).direction = direction;

		}

		//FOOD EATING TEST
		Vec2 locHead = sprite_zombie->getPosition();
		Vec2 locFood = sprite_food->getPosition();
		if (locHead.x == locFood.x && locHead.y == locFood.y){
			sprite_food->setPosition(randomizeLocation());
			grow();
			score += 10;
			std::string scoreStr = "Score: " + std::to_string(score);
			label->setString(scoreStr);
			//CCLOG("hitTest");
		}

		//CHECK FOR SNAKE COLLISION
		//CCLOG("FOOD X: %f Y: %f \t HEAD X: %f Y: %f", locFood.x, locFood.y, locHead.x, locHead.y);
		bool dead = false;
		if (locHead.x <= 64 || locHead.x >= 15 * 64 || locHead.y <= 64 || locHead.y >= 11 * 64){
			dead = true;
			CCLOG("WALL");
		}
		else
			for (int i = 0; i < bodyarray.size(); i++){
			Vec2 locBodyPart = bodyarray.at(i).sprite_zombie->getPosition();
				if (locHead.x == locBodyPart.x && locHead.y == locBodyPart.y){
					dead = true;
				}
			}

		if (dead)
			stop = true;
	}
	 else{
		 Size visibleSize = Director::getInstance()->getVisibleSize();
		 Vec2 origin = Director::getInstance()->getVisibleOrigin();
		 
		 auto sprite_gameover = Sprite::create("gameover.png");
		 sprite_gameover->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));
		 this->addChild(sprite_gameover);

		 auto sprite_replay = Sprite::create("replay.png");
		 sprite_replay->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 50));
		 
		 this->addChild(sprite_replay);	 
	 }
}


bool HelloWorld::GoToGameScene()
{
	if (stop)
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	return true;
}

Vec2 HelloWorld::randomizeLocation(){
	bool occupied;
	Vec2 loc, occ;

	do{
		occupied = false;
		loc.x = (std::rand() % 13 + 2) * 64;
		loc.y = (std::rand() % 9 + 2) * 64;
		for (int i = 0; i < bodyarray.size(); i++){
			occ = bodyarray.at(i).sprite_zombie->getPosition();
			if (occ.x == loc.x && occ.y == loc.y)
			{
				occupied = true;
			}
		}
		occ = sprite_zombie->getPosition();
		if (occ.x == loc.x && occ.y == loc.y)
			occupied = true;
	} while (occupied);
	return loc;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
