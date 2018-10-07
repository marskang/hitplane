#include "StartScene.h"
#include "MainGameScene.h"
#include "ui\UIButton.h"

using namespace ui;

Scene * StartScene::createScene() {
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init() {
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite * spriteBg = Sprite::create("background.png");
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(spriteBg, 0);
	Button * btnStart = Button::create();
	//btnStart->setContentSize(Size(100, 100));
	btnStart->setSize(Size(100, 100));
	btnStart->setTitleFontSize(24);
	btnStart->setTitleText("Start Game");
	btnStart->setTitleColor(Color3B(125, 125, 125));
	btnStart->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2 + 20));
	btnStart->addClickEventListener(CC_CALLBACK_1(StartScene::btnStartClick, this));
	this->addChild(btnStart, 0);
}

void StartScene::btnStartClick(Ref *pSender) {
	auto director = Director::getInstance();
	auto scene = MainGame::createScene();
	director->replaceScene(scene);
}