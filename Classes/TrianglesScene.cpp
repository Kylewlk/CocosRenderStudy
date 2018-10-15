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

#include "TrianglesScene.h"

#pragma warning(disable:4838)
#pragma warning(disable:4305)

USING_NS_CC;

const float SIZE_X = 200;
const float SIZE_Y = 200;

TrianglesNode::TrianglesNode()
	:_texture{nullptr, nullptr}
{
	_vertShader = ccPositionTextureColor_noMVP_vert;
	_fragShader = ccPositionTextureColor_noMVP_frag;
}

TrianglesNode::~TrianglesNode()
{
	CC_SAFE_RELEASE(_texture[0]);
	CC_SAFE_RELEASE(_texture[1]);
}

bool TrianglesNode::init()
{
	if (!Node::init())
	{
		return false;
	}
	setContentSize(Size(SIZE_X, SIZE_Y));
	setAnchorPoint(Vec2(0.5f, 0.5f));






	auto glprogram = GLProgram::createWithByteArrays(_vertShader.c_str(), _fragShader.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);

	GLsizei stride = sizeof(float) * 6;
	glprogramstate->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
	glprogramstate->setVertexAttribPointer("a_normal", 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float)*3));

	return true;
}

void TrianglesNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{

	auto glProgramState = getGLProgramState();

	if (!isVisible())
		return;

}

Scene* TrianglesScene::createScene()
{
    return TrianglesScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TrianglesScene::init()
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
                                           CC_CALLBACK_1(TrianglesScene::menuCloseCallback, this));

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
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    //this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	auto sp = Sprite::create("man.png");
	sp->setPosition(visibleSize / 2);
	this->addChild(sp);


    return true;
}


void TrianglesScene::menuCloseCallback(Ref* pSender)
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
