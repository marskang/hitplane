#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui\UIButton.h"
USING_NS_CC;
using namespace ui;
class MainGame : public cocos2d::Layer {
public:
	const float COMMOM_SCALE = 2.0 / 3;
	const int TAG_HERO = -1;
	const int TAG_BULLET = 0;
	const int TAG_ENEMY0 = 1;
	const int TAG_ENEMY1 = 2;
	const int TAG_ENEMY2 = 3;
	const int TAG_DIE = 4;

	PhysicsWorld * m_world;
	bool isGameOver = false;

	void setPhyWorld(PhysicsWorld* world) { m_world = world; };
	void addBoxBodyForSprite(Sprite* sprite);
	virtual void onEnter();
	bool onContactBegin(const PhysicsContact& contact);

	static Scene * createScene();
	virtual bool init();
	void update(float dt) override;
	void customUpdate(float dt);
	void createBullet(float dt);
	void removeSprite(Ref * sender);
	void createEnemy(float dt);
	void MainGame::createEnemy(Sprite * sprite, int speed);
	void initTexture();
	void initSpriteArr();
	void initUI();
	//void initEnemyDieAction();
	void collisionDetection();
	void removeEnemy0Ani(Sprite * sprite);
	void removeEnemy1Ani(Sprite * sprite);
	void showEnemy1HitAni(Sprite * sprite);
	void removeEnemy2Ani(Sprite * sprite);
	void showEnemy2HitAni(Sprite * sprite);
	void heroBlowupAni(Sprite * sprite);
	void showGameOverUI(Ref * sender);
	void btnRestartClickListener(Ref * sender);
	void gameStart();
	CREATE_FUNC(MainGame);
private:
	Sprite * spriteBg;
	Sprite * spriteHero1;
	Button * btnRestart;
	//Sprite * spriteBullet;
	//SpriteBatchNode * bulletNode;
	Texture2D * texBullet1 = NULL;
	Texture2D * texEnemy0 = NULL;
	Texture2D * texEnemy1 = NULL;
	Texture2D * texEnemy2 = NULL;
	Animate * aniEnemy0Die = NULL;
	long enemy0Count = 0;
	long enemy1Count = 0;
	long enemy2Count = 0;
	/*std::shared_ptr<Vector<Sprite*>>  arrBullet;
	std::shared_ptr<Vector<Sprite*>>  arrEnemy0;
	std::shared_ptr<Vector<Sprite*>>  arrEnemy1;
	std::shared_ptr<Vector<Sprite*>>  arrEnemy2;*/

	Vector<Sprite*>  arrBullet;
	Vector<Sprite*>  arrEnemy0;
	Vector<Sprite*>  arrEnemy1;
	Vector<Sprite*>  arrEnemy2;
};

#endif