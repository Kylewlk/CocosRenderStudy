/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "CustomScene.h"
#include "PrimitiveScene.h"
#include "BatchScene.h"
#include "GroupScene.h"
#include "MeshScene.h"
#include "TrianglesScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menuExit = Menu::create(closeItem, NULL);
    menuExit->setPosition(Vec2::ZERO);
    this->addChild(menuExit, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	auto menu = Menu::create();

	auto customItem = MenuItemFont::create("Custom Command", [](Ref *sender) {
		auto scene = CustomScene::create();
		Director::getInstance()->pushScene(scene);
	});
	menu->addChild(customItem);

	auto primitiveItem = MenuItemFont::create("Primitive Command", [](Ref *sender) {
		auto scene = PrimitiveScene::create();
		Director::getInstance()->pushScene(scene);
	});
	menu->addChild(primitiveItem);

	auto batchItem = MenuItemFont::create("Batch Command", [](Ref *sender) {
		auto scene = BatchScene::create();
		Director::getInstance()->pushScene(scene);
	});
	menu->addChild(batchItem);

	auto groupItem = MenuItemFont::create("Group Command", [](Ref *sender) {
		auto scene = GroupScene::create();
		Director::getInstance()->pushScene(scene);
	});
	menu->addChild(groupItem);

	auto meshItem = MenuItemFont::create("Mesh Command", [](Ref *sender) {
		auto scene = MeshScene::create();
		Director::getInstance()->pushScene(scene);
	});
	menu->addChild(meshItem);

	auto trianglesItem = MenuItemFont::create("Triangles Command", [](Ref *sender) {
		auto scene = TrianglesScene::create();
		Director::getInstance()->pushScene(scene);
	});
	menu->addChild(trianglesItem);

	menu->alignItemsVerticallyWithPadding(20);
	this->addChild(menu);
	menu->setPosition(visibleSize / 2);
	
	return true;
}


void MainScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
