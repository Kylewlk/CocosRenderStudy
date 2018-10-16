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

#include "CustomScene.h"

USING_NS_CC;

const float SIZE_X = 240;
const float SIZE_Y = 240;


CustomNode::CustomNode()
	:_color(1.0, 0.0, 0.0, 1.0)
{
	_vertShader = ccPositionTextureColor_vert;
	_fragShader = "uniform vec4 Color;  void main(void) { gl_FragColor = Color; }";
}

CustomNode::~CustomNode()
{

}


bool CustomNode::init()
{
	if (!Node::init())
	{
		return false;
	}

	auto glprogram = GLProgram::createWithByteArrays(_vertShader.c_str(), _fragShader.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);
	setContentSize(Size(SIZE_X, SIZE_Y));
	setAnchorPoint(Vec2(0.5f, 0.5f));
	return true;
}

void CustomNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(CustomNode::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);

}

void CustomNode::onDraw(const cocos2d::Mat4& transform, uint32_t flags)
{
	float w = SIZE_X, h = SIZE_Y;
	GLfloat vertices[12] = { 0,0, w,0, w,h, 0,0, 0,h, w,h };

	auto glProgramState = getGLProgramState();
	glProgramState->setUniformVec4("Color", _color);
	glProgramState->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glProgramState->apply(transform);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
}


Scene* CustomScene::createScene()
{
    return CustomScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool CustomScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemFont::create("Back", CC_CALLBACK_1(CustomScene::menuBackCallback, this));

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
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	auto node = CustomNode::create();
	//node->setPosition(visibleSize / 2);
	this->addChild(node);
	node->setPosition(Vec2(node->getContentSize().width,visibleSize.height/2));



	auto c2 = Camera::create();
	c2->initDefault();
	c2->setCameraFlag(CameraFlag::USER1);
	c2->setPosition(0, 0);
	addChild(c2);

	auto n2 = CustomNode::create();
	this->addChild(n2);
	n2->setPosition(Vec2(n2->getContentSize().width, 0));

	n2->setColor(Vec4(1.0, 1.0, 0.0, 1.0));
	n2->setCameraMask((uint32_t)CameraFlag::USER1);

    return true;
}

void CustomScene::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}
