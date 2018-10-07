#include "MainGameScene.h"
#include "ui\UIButton.h"

USING_NS_CC;
using namespace ui;

int heroCategoryBitmask = 0x00000001;
int heroCollisionBitmask = 0x00000001;
int heroContactTestBitmask = 0x00000001;
int enemyCategoryBitmask = 0x00000003;
int enemyCollisionBitmask = 0x00000003;
int enemyContactTestBitmask = 0x00000003;
int bulletCategoryBitmask = 0x00000002;
int bulletCollisionBitmask = 0x00000002;
int bulletContactTestBitmask = 0x00000002;

void MainGame::initUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	btnRestart = Button::create("restart_nor.png", "restart_sel.png");
	btnRestart->setPosition(Vec2(origin.x + visibleSize.width / 2, 365));
	btnRestart->setVisible(false);
	btnRestart->retain();
	btnRestart->addClickEventListener(CC_CALLBACK_1(MainGame::btnRestartClickListener, this));
	this->addChild(btnRestart, 2);
}

Scene * MainGame::createScene() {
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	Vect gravity = Vect(0.0f, 0.0f);
	scene->getPhysicsWorld()->setGravity(gravity);
	auto layer = MainGame::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

Vec2 preVec;
bool isMoving = false;
bool MainGame::init() {
	if (!Layer::init()) {
		return false;
	}
	initTexture();
	//initUI();
	gameStart();
	
	/*spriteHero1 = Sprite::create("hero1.png");
	Rect rc = spriteHero1->getBoundingBox();
	Size size = spriteHero1->getBoundingBox().size;
	float hero1Height = size.height;
	spriteHero1->setPosition(Vec2(origin.x + visibleSize.width / 2, hero1Height / 2));
	spriteHero1->setScale(MainGame::COMMOM_SCALE, MainGame::COMMOM_SCALE);
	spriteHero1->setTag(TAG_HERO);
	Vec2 vec[] = { 
	{ -49.2f, -43.4f }, 
	{ -49.2f, -28.9f },
	{ -15.4f, 7.2f }, 
	{ -12.6f, 34.3f }, 
	{ -6.4f, 50.8f }, 
	{ 5.6f, 50.8f }, 
	{ 13.8f, 35.2f }, 
	{ 16.3f, 8.0f }, 
	{ 50.0f, -27.4f }, 
	{ 50.0f, -45.9f } };
	auto body = PhysicsBody::createPolygon(vec, 10);
	body->setDynamic(false);
	body->setCategoryBitmask(heroCategoryBitmask);
	body->setCollisionBitmask(heroCollisionBitmask);
	body->setContactTestBitmask(heroContactTestBitmask);
	spriteHero1->setPhysicsBody(body);
	this->addChild(spriteHero1, 1);*/
	EventListenerMouse * mouse = EventListenerMouse::create();
	mouse->onMouseDown = [=](Event * event) {
		if (isGameOver) {
			return;
		}
		EventMouse * e = (EventMoue *)event;
		isMoving = true;
		preVec = e->getLocationInView();
	};
	mouse->onMouseMove = [=](Event * event) {
		if (isGameOver) { return; }
		if (!isMoving) { return; }
		EventMouse * e = (EventMouse *)event;
		Vec2 heroVec = spriteHero1->getPosition();
		Vec2 mouseVec = e->getLocationInView();
		Rect rc = spriteHero1->getBoundingBox();
		Size sz = rc.size;
		Vec2 newVec = mouseVec - preVec + heroVec;
		Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
		if ((newVec.x - sz.width / 2) < 0) {
			newVec.x = sz.width / 2;
		}
		if ((newVec.x + sz.width / 2) > frameSize.width) {
			newVec.x = frameSize.width - sz.width / 2;
		}
		if ((newVec.y - sz.height / 2) < 0) {
			newVec.y = sz.height / 2;
		}
		if ((newVec.y + sz.height / 2) > frameSize.height) {
			newVec.y = frameSize.height - sz.height / 2;
		}
		spriteHero1->setPosition(newVec);
		preVec = mouseVec;
	};
	mouse->onMouseUp = [=](Event * event) {
		if (isGameOver) { return; }
		isMoving = false;
		EventMouse * e = (EventMouse *)event;
	};
	EventDispatcher * ed = Director::getInstance()->getEventDispatcher();
	ed->addEventListenerWithSceneGraphPriority(mouse, this);
/*	scheduleUpdate();
	schedule(schedule_selector(MainGame::createBullet), 0.2f);
	schedule(schedule_selector(MainGame::createEnemy), 0.4f);*/
	return true;
}

void MainGame::initTexture() {
	texBullet1 = Director::getInstance()->getTextureCache()->addImage("bullet1.png");
	texEnemy0 = Director::getInstance()->getTextureCache()->addImage("enemy0.png");
	texEnemy1 = Director::getInstance()->getTextureCache()->addImage("enemy1.png");
	texEnemy2 = Director::getInstance()->getTextureCache()->addImage("enemy2.png");
}

void MainGame::update(float dt) {
	//collisionDetection();
}

void MainGame::customUpdate(float dt) {
	CCLOG("%f\n", dt);
}

void MainGame::createBullet(float dt) {
	if (isGameOver) {
		return;
	}
	Sprite * bullet = Sprite::createWithTexture(texBullet1);
	bullet->setScale(MainGame::COMMOM_SCALE, MainGame::COMMOM_SCALE);
	bullet->setPosition(spriteHero1->getPosition());
	bullet->setTag(TAG_BULLET);
	addBoxBodyForSprite(bullet);
	this->addChild(bullet);
	arrBullet.pushBack(bullet);
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	Sequence * s =Sequence::create(MoveBy::create(1.0, Vec2(0, frameSize.height)), 
		CallFuncN::create(CC_CALLBACK_1(MainGame::removeSprite, this)), 
		NULL);
	bullet->runAction(s);
}



void MainGame::createEnemy(float dt) {
	if (isGameOver) {
		return;
	}
	enemy0Count++;
	if (enemy0Count % 8 == 0) {
		enemy1Count++;
		if (enemy1Count % 3 == 0) {
			enemy2Count++;
			Sprite * enemy2 = Sprite::createWithTexture(texEnemy2);
			enemy2->setScale(MainGame::COMMOM_SCALE, MainGame::COMMOM_SCALE);
			enemy2->setTag(TAG_ENEMY2);
			int * hp = new int;
			*hp = 7;
			enemy2->setUserData(hp);

			Vec2 vec[] = {
				{ -40.3f, -123.0f },
				{ -83.0f, -77.4f },
				{ -81.1f, 121.9f },
				{ 83.0f, 121.9f },
				{ 82.9f, -80.5f },
				{ 43.2f, -122.8f },
			};
			auto body = PhysicsBody::createPolygon(vec, 6);
			body->setDynamic(false);
			body->setCategoryBitmask(enemyCategoryBitmask);
			body->setCollisionBitmask(enemyCollisionBitmask);
			body->setContactTestBitmask(enemyContactTestBitmask);
			enemy2->setPhysicsBody(body);

			arrEnemy2.pushBack(enemy2);
			createEnemy(enemy2, 5);
		} else {
			Sprite * enemy1 = Sprite::createWithTexture(texEnemy1);
			enemy1->setScale(MainGame::COMMOM_SCALE, MainGame::COMMOM_SCALE);
			Vec2 vec[] = {
				{ -22.9f, -20.0f },
				{ -34.8f, -20.8f },
				{ -35.0f, 23.9f },
				{ -13.2f, 23.7f },
				{ -13.5f, 45.1f },
				{ 13.1f, 45.1f },
				{ 14.2f, 25.2f },
				{ 34.3f, 24.5f },
				{ 34.3f, 6.9f },
				{ 35.0f, -16.8f },
				{ 23.4f, -21.2f },
				{ 15.7f, -26.2f },
				{ 10.6f, -44.1f },
				{ -10.9f, -43.8f },
				{ -16.9f, -26.3f },
			};

			auto body = PhysicsBody::createPolygon(vec, 15);
			body->setDynamic(false);
			body->setCategoryBitmask(enemyCategoryBitmask);
			body->setCollisionBitmask(enemyCollisionBitmask);
			body->setContactTestBitmask(enemyContactTestBitmask);
			enemy1->setPhysicsBody(body);

			enemy1->setTag(TAG_ENEMY1);
			int speed = (random() % 2) + 4;
			int * hp = new int;
			*hp = 3;
			enemy1->setUserData(hp);
			arrEnemy1.pushBack(enemy1);
			createEnemy(enemy1, speed);
		}
	} else {
		Sprite * enemy0 = Sprite::createWithTexture(texEnemy0);
		enemy0->setScale(MainGame::COMMOM_SCALE, MainGame::COMMOM_SCALE);

		Vec2 vec[] = {
			{ -4.3f, -17.4f },
			{ -25.4f, 2.0f },
			{ -25.0f, 17.0f },
			{ 25.9f, 17.4f },
			{ 26.0f, 0.9f },
			{ 6.9f, -17.8f },
		};
		auto body = PhysicsBody::createPolygon(vec, 6);
		body->setDynamic(false);
		body->setCategoryBitmask(enemyCategoryBitmask);
		body->setCollisionBitmask(enemyCollisionBitmask);
		body->setContactTestBitmask(enemyContactTestBitmask);
		enemy0->setPhysicsBody(body);

		enemy0->setTag(TAG_ENEMY0);
		int speed = (random() % 2) + 3;
		arrEnemy0.pushBack(enemy0);
		createEnemy(enemy0, speed);
	}
}

void MainGame::createEnemy(Sprite * sprite, int speed) {
	if (isGameOver) {
		return;
	}
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	float width = frameSize.width;
	Rect rc = spriteHero1->getBoundingBox();
	Size szHero = rc.size;
	Size szEnemy = sprite->getBoundingBox().size;
	Size sz = szEnemy.width > szHero.width ? szEnemy : szHero;
	float dValue = width - sz.width;
	float x = (random() % (int)dValue) + (sz.width / 2);
	float y = frameSize.height;
	sprite->setPosition(Vec2(x, y));
	this->addChild(sprite);
	Sequence * s = Sequence::create(MoveBy::create(speed, Vec2(0, (frameSize.height + sz.height / 2) * -1)),
		CallFuncN::create(CC_CALLBACK_1(MainGame::removeSprite, this)),
		NULL);
	sprite->runAction(s);
}

/*void MainGame::initSpriteArr() {
	this->arrBullet = std::make_shared<Vector<Sprite*>>();
	this->arrEnemy0 = std::make_shared<Vector<Sprite*>>();
	this->arrEnemy1 = std::make_shared<Vector<Sprite*>>();
	this->arrEnemy2 = std::make_shared<Vector<Sprite*>>();
}*/

void MainGame::removeSprite(Ref * sender) {
	CCASSERT(sender != nullptr, "The object should not be nullptr");
	Sprite * sprite = (Sprite *)sender;
	int tag = sprite->getTag();
	if (tag == TAG_BULLET) {
		arrBullet.eraseObject(sprite);
	}
	if (tag == TAG_ENEMY0) {
		arrEnemy0.erase(arrEnemy0.find(sprite));
	}
	if (tag == TAG_ENEMY1) {
		arrEnemy1.erase(arrEnemy1.find(sprite));
	}
	if (tag == TAG_ENEMY2) {
		arrEnemy2.erase(arrEnemy2.find(sprite));
	}
	this->removeChild(sprite);
}

void MainGame::collisionDetection() {
	//����ӵ������
	Vector<Sprite *> releaseEnemy0;
	Vector<Sprite *> releaseEnemy1;
	Vector<Sprite *> releaseEnemy2;
	Vector<Sprite *> releaseBullet;
	Vector<Sprite*>::const_iterator itBullet;

	for (Sprite * bullet : arrBullet) {
		bool isRemove = false;
		Rect rcBullet = bullet->getBoundingBox();
		for (Sprite * enemy : arrEnemy0) {
			if (rcBullet.intersectsRect(enemy->getBoundingBox())) {
				releaseEnemy0.pushBack(enemy);
				isRemove = true;
			}
		}
		for (Sprite * enemy : arrEnemy1) {
			if (rcBullet.intersectsRect(enemy->getBoundingBox())) {
				releaseEnemy1.pushBack(enemy);
				isRemove = true;
			}
		}
		for (Sprite * enemy : arrEnemy2) {
			if (rcBullet.intersectsRect(enemy->getBoundingBox())) {
				releaseEnemy2.pushBack(enemy);
				isRemove = true;
			}
		}
		if(isRemove) {
			releaseBullet.pushBack(bullet);
		}
	}
	for (Sprite * s : releaseBullet) {
		arrBullet.eraseObject(s);
		this->removeChild(s);
	}
	for (Sprite * s : releaseEnemy0) {
		arrEnemy0.eraseObject(s);
		Animation * animation = Animation::create();
		for (int i = 1; i <= 4; i++) {
			char szName[25] = { 0 };
			sprintf(szName, "enemy0_down%d.png", i);
			animation->addSpriteFrameWithFile(szName);
		}
		animation->setDelayPerUnit(0.1);
		animation->setRestoreOriginalFrame(true);
		Animate * action = Animate::create(animation);
		s->setTag(TAG_DIE);
		s->runAction(Sequence::create(action, CallFuncN::create([&](Ref* sender) {
			Sprite * sp = (Sprite *)sender;
			this->removeChild(sp);
		}), NULL));
	}
	for (Sprite * s : releaseEnemy1) {
		/*std::shared_ptr<Map<std::string, int>> map =
			(std::shared_ptr<Map<std::string, int>>)s->getUserData();
		std::iterator it =  map->find("hp");
		map->*/
		int * hp = (int *)s->getUserData();
		(*hp)--;
		if (*hp <= 0) {
			arrEnemy1.eraseObject(s);
			Animation * animation = Animation::create();
			for (int i = 1; i <= 4; i++) {
				char szName[25] = { 0 };
				sprintf(szName, "enemy1_down%d.png", i);
				animation->addSpriteFrameWithFile(szName);
			}
			animation->setDelayPerUnit(0.1);
			animation->setRestoreOriginalFrame(true);
			Animate * action = Animate::create(animation);
			s->setTag(TAG_DIE);
			s->runAction(Sequence::create(action, CallFuncN::create([&](Ref* sender) {
				Sprite * sp = (Sprite *)sender;
				this->removeChild(sp);
			}), NULL));
		} else {
			Animation * animation = Animation::create();
			animation->addSpriteFrameWithFile("enemy1_hit.png");
			animation->setDelayPerUnit(0.1);
			animation->setRestoreOriginalFrame(true);
			Animate * action = Animate::create(animation);
			s->runAction(Sequence::create(action, NULL));
		}
	}
	for (Sprite * s : releaseEnemy2) {
		int * hp = (int *)s->getUserData();
		(*hp)--;
		if (*hp <= 0) {
			arrEnemy2.eraseObject(s);
			Animation * animation = Animation::create();
			for (int i = 1; i <= 6; i++) {
				char szName[25] = { 0 };
				sprintf(szName, "enemy2_down%d.png", i);
				animation->addSpriteFrameWithFile(szName);
			}
			animation->setDelayPerUnit(0.15);
			animation->setRestoreOriginalFrame(true);
			Animate * action = Animate::create(animation);
			s->setTag(TAG_DIE);
			s->runAction(Sequence::create(action, CallFuncN::create([&](Ref* sender) {
				Sprite * sp = (Sprite *)sender;
				this->removeChild(sp);
			}), NULL));
		} else {
			Animation * animation = Animation::create();
			animation->addSpriteFrameWithFile("enemy2_hit.png");
			animation->setDelayPerUnit(0.1);
			animation->setRestoreOriginalFrame(true);
			Animate * action = Animate::create(animation);
			s->runAction(Sequence::create(action, NULL));
		}
	}
	releaseEnemy0.clear();
	releaseEnemy1.clear();
	releaseEnemy2.clear();
	releaseBullet.clear();
}

void MainGame::onEnter() {
	if (isGameOver) {
		return;
	}
	Layer::onEnter();
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MainGame::onContactBegin, this);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool MainGame::onContactBegin(const PhysicsContact& contact) {
	Sprite * spriteA = (Sprite *)contact.getShapeA()->getBody()->getNode();
	Sprite * spriteB = (Sprite *)contact.getShapeB()->getBody()->getNode();
	int aTag = spriteA->getTag();
	int bTag = spriteB->getTag();

	/*const int TAG_HERO = -1;
	const int TAG_BULLET = 0;
	const int TAG_ENEMY0 = 1;
	const int TAG_ENEMY1 = 2;
	const int TAG_ENEMY2 = 3;*/

	if ((aTag == TAG_BULLET && bTag == TAG_ENEMY0)) {
		this->removeChild(spriteA);
		removeEnemy0Ani(spriteB);
	}
	if ((bTag == TAG_BULLET && aTag == TAG_ENEMY0)) {
		this->removeChild(spriteB);
		removeEnemy0Ani(spriteA);
	}

	if ((aTag == TAG_BULLET && bTag == TAG_ENEMY1)) {
		this->removeChild(spriteA);
		int * hp = (int *)spriteB->getUserData();
		(*hp)--;
		if (*hp <= 0) {
			removeEnemy1Ani(spriteB);
		} else {
			showEnemy1HitAni(spriteB);
		}
	}
	if ((bTag == TAG_BULLET && aTag == TAG_ENEMY1)) {
		this->removeChild(spriteB);
		int * hp = (int *)spriteA->getUserData();
		(*hp)--;
		if (*hp <= 0) {
			removeEnemy1Ani(spriteA);
		} else {
			showEnemy1HitAni(spriteA);
		}
	}


	if ((aTag == TAG_BULLET && bTag == TAG_ENEMY2)) {
		this->removeChild(spriteA);
		int * hp = (int *)spriteB->getUserData();
		(*hp)--;
		if (*hp <= 0) {
			removeEnemy2Ani(spriteB);
		} else {
			showEnemy2HitAni(spriteB);
		}
	}
	if ((bTag == TAG_BULLET && aTag == TAG_ENEMY2)) {
		this->removeChild(spriteB);
		int * hp = (int *)spriteA->getUserData();
		(*hp)--;
		if (*hp <= 0) {
			removeEnemy2Ani(spriteA);
		} else {
			showEnemy2HitAni(spriteA);
		}
	}
	if (bTag == TAG_HERO && aTag != TAG_BULLET && aTag != TAG_DIE) {
		isGameOver = true;
		//this->removeAllChildrenWithCleanup(true);
		spriteB->cleanup();
		heroBlowupAni(spriteB);
	}
	if (aTag == TAG_HERO && bTag != TAG_BULLET && bTag != TAG_DIE) {
		isGameOver = true;
		spriteA->cleanup();
		heroBlowupAni(spriteA);
	}
	return true;
}

void MainGame::removeEnemy0Ani(Sprite * sprite) {
	Animation * animation = Animation::create();
	for (int i = 1; i <= 4; i++) {
		char szName[25] = { 0 };
		sprintf(szName, "enemy0_down%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	animation->setDelayPerUnit(0.1);
	animation->setRestoreOriginalFrame(true);
	Animate * action = Animate::create(animation);
	sprite->setTag(TAG_DIE);
	sprite->runAction(Sequence::create(action, CallFuncN::create([&](Ref* sender) {
		Sprite * sp = (Sprite *)sender;
		this->removeChild(sp);
	}), NULL));
}

void MainGame::removeEnemy1Ani(Sprite * sprite) {
	Animation * animation = Animation::create();
	for (int i = 1; i <= 4; i++) {
		char szName[25] = { 0 };
		sprintf(szName, "enemy1_down%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	animation->setDelayPerUnit(0.1);
	animation->setRestoreOriginalFrame(true);
	Animate * action = Animate::create(animation);
	sprite->setTag(TAG_DIE);
	sprite->runAction(Sequence::create(action, CallFuncN::create([&](Ref* sender) {
		Sprite * sp = (Sprite *)sender;
		this->removeChild(sp);
	}), NULL));
}

void MainGame::showEnemy1HitAni(Sprite * sprite) {
	Animation * animation = Animation::create();
	animation->addSpriteFrameWithFile("enemy1_hit.png");
	animation->setDelayPerUnit(0.1);
	animation->setRestoreOriginalFrame(true);
	Animate * action = Animate::create(animation);
	sprite->runAction(Sequence::create(action, NULL));
}

void MainGame::removeEnemy2Ani(Sprite * sprite) {
	Animation * animation = Animation::create();
	for (int i = 1; i <= 6; i++) {
		char szName[25] = { 0 };
		sprintf(szName, "enemy2_down%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	animation->setDelayPerUnit(0.15);
	animation->setRestoreOriginalFrame(true);
	Animate * action = Animate::create(animation);
	sprite->setTag(TAG_DIE);
	sprite->runAction(Sequence::create(action, CallFuncN::create([&](Ref* sender) {
		Sprite * sp = (Sprite *)sender;
		this->removeChild(sp);
	}), NULL));
}

void MainGame::showEnemy2HitAni(Sprite * sprite) {
	Animation * animation = Animation::create();
	animation->addSpriteFrameWithFile("enemy2_hit.png");
	animation->setDelayPerUnit(0.1);
	animation->setRestoreOriginalFrame(true);
	Animate * action = Animate::create(animation);
	sprite->runAction(Sequence::create(action, NULL));
}

void MainGame::heroBlowupAni(Sprite * sprite) {
	Animation * animation = Animation::create();
	for (int i = 1; i <= 3; i++) {
		char szName[25] = { 0 };
		sprintf(szName, "hero_blowup_n%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	animation->setDelayPerUnit(0.15);
	animation->setRestoreOriginalFrame(true);
	Animate * action = Animate::create(animation);
	sprite->runAction(Sequence::create(action, CallFuncN::create([&](Ref* sender) {
		Sprite * sp = (Sprite *)sender;
		this->removeChild(sp);
	}), CallFuncN::create(CC_CALLBACK_1(MainGame::showGameOverUI, this)), NULL));
}

void MainGame::addBoxBodyForSprite(Sprite* sprite) {
	auto body = PhysicsBody::createBox(sprite->getContentSize());
	body->setDynamic(false);
	body->setCategoryBitmask(bulletCategoryBitmask);
	body->setCollisionBitmask(bulletCollisionBitmask);
	body->setContactTestBitmask(bulletContactTestBitmask);
	sprite->setPhysicsBody(body);
}

void MainGame::showGameOverUI(Ref * sender) {
	btnRestart->setVisible(true);
	//CCLOG("hello world\n");
}

void MainGame::btnRestartClickListener(Ref * sender) {
	gameStart();
}

void MainGame::gameStart() {
	isGameOver = false;
	this->removeAllChildren();
	initUI();
	//this->cleanup();
	btnRestart->setVisible(false); 
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	spriteBg = Sprite::create("background.png");
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	spriteBg->setScale(MainGame::COMMOM_SCALE, MainGame::COMMOM_SCALE);
	this->addChild(spriteBg, 0);
	spriteHero1 = Sprite::create("hero1.png");
	Rect rc = spriteHero1->getBoundingBox();
	Size size = spriteHero1->getBoundingBox().size;
	float hero1Height = size.height;
	spriteHero1->setPosition(Vec2(origin.x + visibleSize.width / 2, hero1Height / 2));
	spriteHero1->setScale(MainGame::COMMOM_SCALE, MainGame::COMMOM_SCALE);
	spriteHero1->setTag(TAG_HERO);
	Vec2 vec[] = {
		{ -49.2f, -43.4f },
		{ -49.2f, -28.9f },
		{ -15.4f, 7.2f },
		{ -12.6f, 34.3f },
		{ -6.4f, 50.8f },
		{ 5.6f, 50.8f },
		{ 13.8f, 35.2f },
		{ 16.3f, 8.0f },
		{ 50.0f, -27.4f },
		{ 50.0f, -45.9f } };
	auto body = PhysicsBody::createPolygon(vec, 10);
	body->setDynamic(false);
	body->setCategoryBitmask(heroCategoryBitmask);
	body->setCollisionBitmask(heroCollisionBitmask);
	body->setContactTestBitmask(heroContactTestBitmask);
	spriteHero1->setPhysicsBody(body);
	this->addChild(spriteHero1, 1);
	scheduleUpdate();
	schedule(schedule_selector(MainGame::createBullet), 0.2f);
	schedule(schedule_selector(MainGame::createEnemy), 0.4f);
}