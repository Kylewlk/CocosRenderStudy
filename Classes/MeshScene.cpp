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

USING_NS_CC;

const float SIZE_X = 200;
const float SIZE_Y = 200;


static const char* ccPositionTextureColor_frag2 = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
	vec4 tc = texture2D(CC_Texture0, v_texCoord);	
	gl_FragColor = mix (v_fragmentColor , tc, tc.w);
}
)";

MeshNode::MeshNode()
{
	_vertShader = ccPositionTextureColor_vert;
	_fragShader = ccPositionTextureColor_frag2;
}

MeshNode::~MeshNode()
{
}


bool MeshNode::init()
{
	if (!Node::init())
	{
		return false;
	}
	setContentSize(Size(SIZE_X, SIZE_Y));
	setAnchorPoint(Vec2(0.5f, 0.5f));

	auto cache = Director::getInstance()->getTextureCache();

	std::string fullPath = FileUtils::getInstance()->fullPathForFilename("1.c3t");

	MeshDatas* meshdatas = new (std::nothrow) MeshDatas();
	MaterialDatas* materialdatas = new (std::nothrow) MaterialDatas();
	NodeDatas* nodeDatas = new (std::nothrow) NodeDatas();
	auto bundle = Bundle3D::createBundle();
	if (!bundle->load(fullPath))
	{
		Bundle3D::destroyBundle(bundle);
		return false;
	}
	bundle->loadMeshDatas(*meshdatas);
	bundle->loadMaterials(*materialdatas);
	bundle->loadNodes(*nodeDatas);
	Bundle3D::destroyBundle(bundle);

	auto VertexData = MeshVertexData::create(*(meshdatas->meshDatas.front()));



	auto glprogram = GLProgram::createWithByteArrays(_vertShader.c_str(), _fragShader.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);
	//setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR, tex));
	return true;
}

void MeshNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{

	auto glProgramState = getGLProgramState();


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

    // add a label shows "Hello World"
    // create and initialize a label
	//auto node = MeshNode::create();
	//this->addChild(node);
	//node->setPosition(Vec2(node->getContentSize().width,visibleSize.height/2));

	auto sp = Sprite3D::create("orc.c3t");
	sp->setScale(10, 10);
	sp->setRotation3D(Vec3(0, 180, 0));
	sp->setPosition(visibleSize / 2);

	auto animation = Animation3D::create("orc.c3t", "Take 001");
	if (animation)
	{
		auto animate = Animate3D::create(animation);
		sp->runAction(RepeatForever::create(animate));
	}
	this->addChild(sp);

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
