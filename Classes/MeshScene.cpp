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

#include "MeshScene.h"
#include "3d/CCBundle3D.h"

#pragma warning(disable:4838)
#pragma warning(disable:4305)

USING_NS_CC;

const float SIZE_X = 200;
const float SIZE_Y = 200;

static const char* ccPositionNormal_vert = R"(
attribute vec3 a_position;
attribute vec3 a_normal;

varying vec3 v_normal;

void main(void)
{
	v_normal = normalize(CC_NormalMatrix * a_normal);	
    gl_Position = CC_MVPMatrix * vec4(a_position, 1.0);
}
)";

static const char* ccPositionNormal_frag = R"(
uniform vec4 Color;

varying vec3 v_normal;

void main()
{
	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0) *(0.1 + 0.6*max(v_normal.z, 0)) + vec4(1.0) * pow(max(v_normal.z, 0), 70)*0.2;
}
)";

MeshNode::MeshNode()
{
	_vertShader = ccPositionNormal_vert;
	_fragShader = ccPositionNormal_frag;
	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;
	_stateBlock = nullptr;
}

MeshNode::~MeshNode()
{
	CC_SAFE_RELEASE(_vertexBuffer);
	CC_SAFE_RELEASE(_indexBuffer);
	CC_SAFE_RELEASE(_stateBlock);
}

bool MeshNode::init()
{
	if (!Node::init())
	{
		return false;
	}
	setContentSize(Size(SIZE_X, SIZE_Y));
	setAnchorPoint(Vec2(0.0f, 0.0f));

	_stateBlock = RenderState::StateBlock::create();
	_stateBlock->setBlend(false);
	_stateBlock->setCullFace(false);
	_stateBlock->setDepthTest(true); //必须同时设置setDepthWrite(true)，才能进行深度比较
	_stateBlock->setDepthWrite(true);
	_stateBlock->retain();

	//这是一个正方体,数据来自3D模型Resources\3DCube\cube1.c3t，位置：3个float, 法线：3个float
	float data[] = {
		//背面 
		120.787605,  119.807968, -119.807968,  0.000000,  0.000000, -1.000000,
		-120.787605, -119.807968, -119.807968,  0.000000,  0.000000, -1.000000,
		-120.787605,  119.807968, -119.807968,  0.000000,  0.000000, -1.000000,
		120.787605, -119.807968, -119.807968,  0.000000,  0.000000, -1.000000,

		//正面
		-120.787605,  119.807968,  119.807968,  0.000000,  0.000000,  1.000000,
		120.787605, -119.807968,  119.807968,  0.000000,  0.000000,  1.000000,
		120.787605,  119.807968,  119.807968,  0.000000,  0.000000,  1.000000,
		-120.787605, -119.807968,  119.807968,  0.000000,  0.000000,  1.000000,

		//右面
		120.787605,  119.807968,  119.807968,  1.000000,  0.000000,  0.000000,
		120.787605, -119.807968, -119.807968,  1.000000,  0.000000,  0.000000,
		120.787605,  119.807968, -119.807968,  1.000000,  0.000000,  0.000000,
		120.787605, -119.807968,  119.807968,  1.000000,  0.000000,  0.000000,

		//左面
		-120.787605,  119.807968, -119.807968, -1.000000,  0.000000,  0.000000,
		-120.787605, -119.807968,  119.807968, -1.000000,  0.000000,  0.000000,
		-120.787605,  119.807968,  119.807968, -1.000000,  0.000000,  0.000000,
		-120.787605, -119.807968, -119.807968, -1.000000,  0.000000,  0.000000,

		//上面
		-120.787605,  119.807968, -119.807968,  0.000000,  1.000000,  0.000000,
		120.787605,  119.807968,  119.807968,  0.000000,  1.000000,  0.000000,
		120.787605,  119.807968, -119.807968,  0.000000,  1.000000,  0.000000,
		-120.787605,  119.807968,  119.807968,  0.000000,  1.000000,  0.000000,

		//下面
		-120.787605, -119.807968,  119.807968,  0.000000, -1.000000,  0.000000,
		120.787605, -119.807968, -119.807968,  0.000000, -1.000000,  0.000000,
		120.787605, -119.807968,  119.807968,  0.000000, -1.000000,  0.000000,
		-120.787605, -119.807968, -119.807968,  0.000000, -1.000000,  0.000000
	};

	uint32_t indices2[] = {
		0,   1,   2,   0,   3,   1,   4,   5,   6,   4,   7,   5,
		8,   9,  10,   8,  11,   9,  12,  13,  14,  12,  15,  13,
		16,  17,  18,  16,  19,  17,  20,  21,  22,  20,  23,  21
	};

	static const int TOTAL_VERTS = sizeof(data) / sizeof(data[0])/6;
	static const int TOTAL_INDICES = sizeof(indices2) / sizeof(indices2[0]);

	_vertexBuffer = VertexBuffer::create(sizeof(float)*6, TOTAL_VERTS);
	_vertexBuffer->updateVertices(data, TOTAL_VERTS, 0);
	_vertexBuffer->retain();

	_indexBuffer = IndexBuffer::create(IndexBuffer::IndexType::INDEX_TYPE_UINT_32, TOTAL_INDICES);
	_indexBuffer->updateIndices(indices2, TOTAL_INDICES, 0);
	_indexBuffer->retain();

	auto glprogram = GLProgram::createWithByteArrays(_vertShader.c_str(), _fragShader.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);

	GLsizei stride = sizeof(float) * 6;
	glprogramstate->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
	glprogramstate->setVertexAttribPointer("a_normal", 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float)*3));

	return true;
}

//#define DRAW_LINE

#ifdef DRAW_LINE
static CustomCommand beforeCommand;
static CustomCommand afterCommand;
#endif // DRAW_LINE


void MeshNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{

	auto glProgramState = getGLProgramState();

	if (!isVisible())
		return;

#ifdef DRAW_LINE
	beforeCommand.init(-1);
	beforeCommand.func = []() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); };
	renderer->addCommand(&beforeCommand);
#endif

	_meshCommand.init(getGlobalZOrder(),
		0,
		glProgramState,
		_stateBlock,
		_vertexBuffer->getVBO(),
		_indexBuffer->getVBO(),
		GL_TRIANGLES,
		GL_UNSIGNED_INT,
		_indexBuffer->getIndexNumber(),
		transform,
		flags);

	_meshCommand.setSkipBatching(true);
	_meshCommand.setTransparent(false);
	_meshCommand.set3D(true);

	renderer->addCommand(&_meshCommand);

#ifdef DRAW_LINE
	afterCommand.init(1);
	afterCommand.func = []() {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); };
	renderer->addCommand(&afterCommand);
#endif

}

Scene* MeshScene::createScene()
{
    return MeshScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MeshScene::init()
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
                                           CC_CALLBACK_1(MeshScene::menuCloseCallback, this));

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

	//auto sp = Sprite3D::create("orc.c3t");
	//sp->setScale(10, 10);
	//sp->setRotation3D(Vec3(0, 180, 0));
	//sp->setPosition(visibleSize / 2);

	//auto animation = Animation3D::create("orc.c3t", "Take 001");
	//if (animation)
	//{
	//	auto animate = Animate3D::create(animation);
	//	sp->runAction(RepeatForever::create(animate));
	//}
	//this->addChild(sp);

	auto meshNode = MeshNode::create();
	meshNode->setPosition(visibleSize/2);
	meshNode->setScale(0.8);

	auto r = RotateBy::create(5, Vec3(360, 90, 180));
	auto action = RepeatForever::create(r);
	meshNode->runAction(action);

	this->addChild(meshNode);

    return true;
}


void MeshScene::menuCloseCallback(Ref* pSender)
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
