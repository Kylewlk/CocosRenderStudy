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

#ifndef __COMMAND_SCENE_H__
#define __COMMAND_SCENE_H__

#include "cocos2d.h"

class CustomNode : public cocos2d::Node
{
public:
    CREATE_FUNC(CustomNode);

	virtual bool init();

	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

	void setColor(cocos2d::Vec4 &color) { this->_color = color; }

protected:
	CustomNode();
	~CustomNode();

	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

	std::string _vertShader;
	std::string _fragShader;
	cocos2d::CustomCommand _customCommand;
	cocos2d::Vec4 _color;
private:

};



class CustomScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
	void menuBackCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(CustomScene);
};

#endif // __HELLOWORLD_SCENE_H__
