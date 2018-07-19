#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"

USING_NS_CC;

vector<network::HttpRequest*> httpVec;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

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

    
    //Green Play button
    ui::Button *bPlay = ui::Button::create("play.png");
    bPlay->::HelloWorld::setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
    bPlay->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch(type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                
                for(int i = 0; i < 1000; i++)
                {
                    network::HttpRequest* request = new network::HttpRequest();
                    request->setUrl("http://www.cocos2d-x.org");
                    request->setRequestType(network::HttpRequest::Type::GET);
                    request->setResponseCallback([](network::HttpClient* client, network::HttpResponse* response) {
                        
                        static int i = 0;
                        log("Get cocos page %d", i);
                        i++;
                        
                    });
                    
                    network::HttpClient::getInstance()->send(request);
                    request->release();
                    
                    httpVec.push_back(request);
                }
                
                break;
            case ui::Widget::TouchEventType::MOVED: break;
            case ui::Widget::TouchEventType::ENDED: break;
            case ui::Widget::TouchEventType::CANCELED: break;
        }
    });
    
    this->addChild(bPlay);
    
    
    //Red Stop button
    ui::Button *bStop = ui::Button::create("stop.png");
    bStop->setScale(.5f);
    bStop->::HelloWorld::setPosition(bPlay->getPositionX(), bPlay->getPositionY() - bPlay->getBoundingBox().size.height/2 - bStop->getBoundingBox().size.height/2 - (visibleSize.height * 0.05f));
    bStop->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch(type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                
                for (int i = 0;i < (int)httpVec.size();i++)
                {
                    httpVec.at(i)->setResponseCallback(nullptr);
                    httpVec.at(i)->release();
                }
                
                break;
            case ui::Widget::TouchEventType::MOVED: break;
            case ui::Widget::TouchEventType::ENDED: break;
            case ui::Widget::TouchEventType::CANCELED: break;
        }
    });
    
    this->addChild(bStop);
    
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
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
