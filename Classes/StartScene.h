#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class StartScene : public Layer {
public :
	static Scene * createScene();
	virtual bool init();
	void btnStartClick(Ref *pSender);
	CREATE_FUNC(StartScene);
};

#endif