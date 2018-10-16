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
	setContentSize(Size(0, 300));
	setAnchorPoint(Vec2(0.5f, 0.5f));

	auto cache = Director::getInstance()->getTextureCache();
	_texture[0] = cache->addImage("HelloWorld.png");
	_texture[0]->retain();
	_texture[1] = cache->addImage("Man.png");
	_texture[1]->retain();

	float w = 200;
	float h = 300;

	const static cocos2d::V3F_C4B_T2F_Quad quad = {
		{ { 0,  0, 0 },{ 255, 255, 255, 255 },{ 0,1 } },
		{ { 0,  h, 0 },{ 255, 255, 255, 255 },{ 0,0 } },
		{ { w,  h, 0 },{ 255, 255, 255, 255 },{ 1,0 } },
	};

	const static cocos2d::V3F_C4B_T2F_Quad quad1 = {
		{ { 20 + 0,  0, 0 },{ 255, 255, 255, 255 },{ 0,1 } },
		{ { 20 + w,  h, 0 },{ 255, 255, 255, 255 },{ 1,0 } },
		{ { 20 + w,  0, 0 },{ 255, 255, 255, 255 },{ 1,1 } },
	};

	const static cocos2d::V3F_C4B_T2F_Quad quad2 = {
		{ { w + 40 + 0,    0, 0 },{ 255, 255, 255, 255 },{ 0,1 } },
		{ { w + 40 + 0,    300, 0 },{ 255, 255, 255, 255 },{ 0,0 } },
		{ { w + 40 + 400,  300, 0 },{ 255, 255, 255, 255 },{ 1,0 } },
		{ { w + 40 + 400,  0, 0 },{ 255, 255, 255, 255 },{ 1,1 } },
	};

	static uint16_t indices[] = { 0,1,2, 2,0,3 };

	_triangles[0].indexCount = 3;
	_triangles[0].indices = indices;
	_triangles[0].vertCount = 4;
	_triangles[0].verts = (V3F_C4B_T2F*)&quad;

	_triangles[1].indexCount = 3;
	_triangles[1].indices = indices;
	_triangles[1].vertCount = 4;
	_triangles[1].verts = (V3F_C4B_T2F*)&quad1;

	_triangles[2].indexCount = 6;
	_triangles[2].indices = indices;
	_triangles[2].vertCount = 4;
	_triangles[2].verts = (V3F_C4B_T2F*)&quad2;

	auto glprogram = GLProgram::createWithByteArrays(_vertShader.c_str(), _fragShader.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);
	return true;
}

void TrianglesNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{

	auto glProgramState = getGLProgramState();

	if (!isVisible())
		return;

	for (int i = 0; i < 3; ++i)
	{
		_trianglesCommand[i].init(_globalZOrder, _texture[i/2], glProgramState,
			BlendFunc::ALPHA_PREMULTIPLIED, _triangles[i], transform, flags);
		renderer->addCommand(_trianglesCommand + i);
	}



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
	//auto sp = Sprite::create("man.png");
	//sp->setPosition(visibleSize / 2);
	//this->addChild(sp);

	auto node = TrianglesNode::create();
	node->setPosition(50, visibleSize.height / 2);
	this->addChild(node);

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
