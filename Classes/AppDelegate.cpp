#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "View/PlayLayer.h"
using namespace std;
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    
    //添加搜索路径
    vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
    searchPaths.insert(searchPaths.begin(), "DemonFruit");
    searchPaths.insert(searchPaths.begin(), "Scene");
    searchPaths.insert(searchPaths.begin(), "Levels");
    searchPaths.insert(searchPaths.begin(), "Effects");
    searchPaths.insert(searchPaths.begin(), "Sounds");
    searchPaths.insert(searchPaths.begin(), "Tile");
    searchPaths.insert(searchPaths.begin(), "fonts");
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    //加载资源
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("DemonFruit.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("stars.plist");
    // create a scene. it's an autorelease object
    auto scene = PlayLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
