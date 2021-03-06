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

#include "PrimitiveScene.h"

USING_NS_CC;

const float SIZE_X = 200;
const float SIZE_Y = 200;


static const char* ccPositionTextureColor_frag2 = R"(

uniform int HasTex;  

#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
	if( HasTex != 0 )
	{
		vec4 tc = texture2D(CC_Texture0, v_texCoord);	
		gl_FragColor = mix (v_fragmentColor , tc, tc.w);
	}
	else
	{
		gl_FragColor = v_fragmentColor;
	}
}
)";

PrimitiveNode::PrimitiveNode()
	:_texture(nullptr)
{
	_vertShader = ccPositionTextureColor_vert;
	_fragShader = ccPositionTextureColor_frag2;
}

PrimitiveNode::~PrimitiveNode()
{
	CC_SAFE_RELEASE(_primitive);
	CC_SAFE_RELEASE(_texture);
}

void PrimitiveNode::setTexture(cocos2d::Texture2D * texture)
{
	_texture = texture;
	CC_SAFE_RETAIN(_texture);
}

bool PrimitiveNode::init()
{
	if (!Node::init())
	{
		return false;
	}
	setContentSize(Size(SIZE_X, SIZE_Y));
	setAnchorPoint(Vec2(0.5f, 0.5f));



	V3F_C4B_T2F data[] = {
		{ { 0,    0,0 },{ 255,  0,  0,255 },{ 0,1 } },
		{ { 200,  0,0 },{ 0,  255,255,255 },{ 1,1 } },
		{ { 200,200,0 },{ 255,255,  0,255 },{ 1,0 } },
		{ { 0,  200,0 },{ 255,255,255,255 },{ 0,0 } },
	};

	uint16_t indices[] = {
		0,1,2,
		2,0,3
	};

	static const int TOTAL_VERTS = sizeof(data) / sizeof(data[0]);
	static const int TOTAL_INDICES = TOTAL_VERTS * 6 / 4;

	auto vertexBuffer = VertexBuffer::create(sizeof(V3F_C4B_T2F), TOTAL_VERTS);
	vertexBuffer->updateVertices(data, TOTAL_VERTS, 0);

	auto vertsData = VertexData::create();
	vertsData->setStream(vertexBuffer, VertexStreamAttribute(0, GLProgram::VERTEX_ATTRIB_POSITION, GL_FLOAT, 3));
	vertsData->setStream(vertexBuffer, VertexStreamAttribute(offsetof(V3F_C4B_T2F, colors), GLProgram::VERTEX_ATTRIB_COLOR, GL_UNSIGNED_BYTE, 4, true));
	vertsData->setStream(vertexBuffer, VertexStreamAttribute(offsetof(V3F_C4B_T2F, texCoords), GLProgram::VERTEX_ATTRIB_TEX_COORD, GL_FLOAT, 2));


	auto indexBuffer = IndexBuffer::create(IndexBuffer::IndexType::INDEX_TYPE_SHORT_16, TOTAL_INDICES);
	indexBuffer->updateIndices(indices, TOTAL_INDICES, 0);

	_primitive = Primitive::create(vertsData, indexBuffer, GL_TRIANGLES);
	_primitive->setCount(TOTAL_INDICES);
	_primitive->setStart(0);
	_primitive->retain();



	auto glprogram = GLProgram::createWithByteArrays(_vertShader.c_str(), _fragShader.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);
	return true;
}

void PrimitiveNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{

	auto glProgramState = getGLProgramState();
	glProgramState->setUniformInt("HasTex", _texture!=nullptr);

	GLuint tex = (_texture == nullptr) ? 0 : _texture->getName();
	_primitiveCommand.init(_globalZOrder,
		tex,
		glProgramState,
		BlendFunc::ALPHA_NON_PREMULTIPLIED,
		_primitive,
		transform,
		flags);

	renderer->addCommand(&_primitiveCommand);

}




Scene* PrimitiveScene::createScene()
{
    return PrimitiveScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PrimitiveScene::init()
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

	auto closeItem = MenuItemFont::create("Back", CC_CALLBACK_1(PrimitiveScene::menuBackCallback, this));

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
	auto node = PrimitiveNode::create();
	//node->setPosition(visibleSize / 2);
	this->addChild(node);
	node->setPosition(Vec2(node->getContentSize().width,visibleSize.height/2));

	auto node2 = PrimitiveNode::create();
	//node->setPosition(visibleSize / 2);
	this->addChild(node2);
	node2->setPosition(Vec2(node2->getContentSize().width*2 + 20,visibleSize.height/2));

	auto cache = Director::getInstance()->getTextureCache();
	auto texture = cache->addImage("HelloWorld.png");
	node2->setTexture(texture);

    return true;
}

void PrimitiveScene::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}
