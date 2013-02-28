#include "LayerTest.h"
#include "../testResource.h"

enum 
{
    kTagLayer = 1,
};

TESTLAYER_CREATE_FUNC(LayerTestCascadingOpacityA);
TESTLAYER_CREATE_FUNC(LayerTestCascadingOpacityB);
TESTLAYER_CREATE_FUNC(LayerTestCascadingOpacityC);
TESTLAYER_CREATE_FUNC(LayerTestCascadingColorA);
TESTLAYER_CREATE_FUNC(LayerTestCascadingColorB);
TESTLAYER_CREATE_FUNC(LayerTestCascadingColorC);
TESTLAYER_CREATE_FUNC(LayerTest1);
TESTLAYER_CREATE_FUNC(LayerTest2);
TESTLAYER_CREATE_FUNC(LayerTestBlend);
TESTLAYER_CREATE_FUNC(LayerGradient);
TESTLAYER_CREATE_FUNC(LayerIgnoreAnchorPointPos);
TESTLAYER_CREATE_FUNC(LayerIgnoreAnchorPointRot);
TESTLAYER_CREATE_FUNC(LayerIgnoreAnchorPointScale);
TESTLAYER_CREATE_FUNC(LayerExtendedBlendOpacityTest);

static NEWTESTFUNC createFunctions[] = {
    CF(LayerTestCascadingOpacityA),
    CF(LayerTestCascadingOpacityB),
    CF(LayerTestCascadingOpacityC),
    CF(LayerTestCascadingColorA),
    CF(LayerTestCascadingColorB),
    CF(LayerTestCascadingColorC),
    CF(LayerTest1),
    CF(LayerTest2),
    CF(LayerTestBlend),
    CF(LayerGradient),
    CF(LayerIgnoreAnchorPointPos),
    CF(LayerIgnoreAnchorPointRot),
    CF(LayerIgnoreAnchorPointScale),
    CF(LayerExtendedBlendOpacityTest)
};

static int sceneIdx=-1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

static CCLayer* nextAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* backAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if( sceneIdx < 0 )
        sceneIdx += total;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* restartAction()
{
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

//------------------------------------------------------------------
//
// LayerTest
//
//------------------------------------------------------------------

LayerTest::LayerTest(void)
{
}

LayerTest::~LayerTest(void)
{
}

string LayerTest::subtitle()
{
    return "";
}

std::string LayerTest::title()
{
    return "No title";
}

void LayerTest::onEnter()
{
    CCLayer::onEnter();

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 32);
    addChild(label, 1);
    label->setPosition( ccp(s.width/2, s.height-50) );

    string subtitle_ = subtitle();
    if (subtitle_.size() > 0)
    {
        CCLabelTTF *l = CCLabelTTF::create(subtitle_.c_str(), "Thonburi", 16);
        addChild(l, 1);
        l->setPosition(ccp(s.width / 2, s.height - 80));
    }

    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(LayerTest::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(LayerTest::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(LayerTest::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition( CCPointZero );
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    
    addChild(menu, 1);    
}

void LayerTest::restartCallback(CCObject* pSender)
{
    CCScene* s = new LayerTestScene();
    s->addChild(restartAction());

    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void LayerTest::nextCallback(CCObject* pSender)
{
    CCScene* s = new LayerTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void LayerTest::backCallback(CCObject* pSender)
{
    CCScene* s = new LayerTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
} 

#pragma mark - Cascading support extensions

@interface CCNode (cascading)
@end
@implementation CCNode (cascading)
-(void) setEnableRecursiveCascading:(BOOL)enable
{
	if( [self conformsToProtocol:@protocol(CCRGBAProtocol)] ) {
		[(id<CCRGBAProtocol>)self setCascadeColorEnabled:enable];
		[(id<CCRGBAProtocol>)self setCascadeOpacityEnabled:enable];
		
	}
	for( CCNode* child in self.children)
		[child setEnableRecursiveCascading:enable];
}
@end

#pragma mark - Example LayerTestCascadingOpacity

@implementation LayerTestCascadingOpacityA
-(id) init
{
	if( (self=[super init] )) {
        
		CGSize s = [[CCDirector sharedDirector] winSize];
		CCLayerRGBA* layer1 = [CCLayerRGBA node];
        
		CCSprite *sister1 = [CCSprite spriteWithFile:@"grossinis_sister1.png"];
		CCSprite *sister2 = [CCSprite spriteWithFile:@"grossinis_sister2.png"];
		CCLabelBMFont *label = [CCLabelBMFont labelWithString:@"Test" fntFile:@"bitmapFontTest.fnt"];
        
		[layer1 addChild:sister1];
		[layer1 addChild:sister2];
		[layer1 addChild:label];
		[self addChild: layer1 z:0 tag:kTagLayer];
        
		sister1.position = ccp( s.width*1/3, s.height/2);
		sister2.position = ccp( s.width*2/3, s.height/2);
		label.position = ccp( s.width/2, s.height/2);
        
        [layer1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCFadeTo actionWithDuration:4 opacity:0],
           [CCFadeTo actionWithDuration:4 opacity:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
        [sister1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCFadeTo actionWithDuration:2 opacity:0],
           [CCFadeTo actionWithDuration:2 opacity:255],
           [CCFadeTo actionWithDuration:2 opacity:0],
           [CCFadeTo actionWithDuration:2 opacity:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
        
		// Enable cascading in scene
		[self setEnableRecursiveCascading:YES];
    }
	return self;
}

-(NSString *) title
{
	return @"LayerRGBA: cascading opacity";
}
@end

@implementation LayerTestCascadingOpacityB
-(id) init
{
	if( (self=[super init] )) {
        
		CGSize s = [[CCDirector sharedDirector] winSize];
		CCLayerColor* layer1 = [CCLayerColor layerWithColor:ccc4(192, 0, 0, 255) width:s.width height:s.height/2];
        layer1.cascadeColorEnabled = NO;
        
        layer1.position = ccp(0, s.height/2);
        
		CCSprite *sister1 = [CCSprite spriteWithFile:@"grossinis_sister1.png"];
		CCSprite *sister2 = [CCSprite spriteWithFile:@"grossinis_sister2.png"];
		CCLabelBMFont *label = [CCLabelBMFont labelWithString:@"Test" fntFile:@"bitmapFontTest.fnt"];
		
		[layer1 addChild:sister1];
		[layer1 addChild:sister2];
		[layer1 addChild:label];
		[self addChild: layer1 z:0 tag:kTagLayer];
		
		sister1.position = ccp( s.width*1/3, 0);
		sister2.position = ccp( s.width*2/3, 0);
		label.position = ccp( s.width/2, 0);
        
        [layer1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCFadeTo actionWithDuration:4 opacity:0],
           [CCFadeTo actionWithDuration:4 opacity:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
        [sister1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCFadeTo actionWithDuration:2 opacity:0],
           [CCFadeTo actionWithDuration:2 opacity:255],
           [CCFadeTo actionWithDuration:2 opacity:0],
           [CCFadeTo actionWithDuration:2 opacity:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
		
		// Enable cascading in scene
		[self setEnableRecursiveCascading:YES];
	}
	return self;
}

-(NSString *) title
{
	return @"CCLayerColor: cascading opacity";
}
@end

@implementation LayerTestCascadingOpacityC
-(id) init
{
	if( (self=[super init] )) {
        
		CGSize s = [[CCDirector sharedDirector] winSize];
		CCLayerColor* layer1 = [CCLayerColor layerWithColor:ccc4(192, 0, 0, 255) width:s.width height:s.height/2];
        layer1.cascadeColorEnabled = NO;
        layer1.cascadeOpacityEnabled = NO;
        
        layer1.position = ccp(0, s.height/2);
        
		CCSprite *sister1 = [CCSprite spriteWithFile:@"grossinis_sister1.png"];
		CCSprite *sister2 = [CCSprite spriteWithFile:@"grossinis_sister2.png"];
		CCLabelBMFont *label = [CCLabelBMFont labelWithString:@"Test" fntFile:@"bitmapFontTest.fnt"];
		
		[layer1 addChild:sister1];
		[layer1 addChild:sister2];
		[layer1 addChild:label];
		[self addChild: layer1 z:0 tag:kTagLayer];
		
		sister1.position = ccp( s.width*1/3, 0);
		sister2.position = ccp( s.width*2/3, 0);
		label.position = ccp( s.width/2, 0);
        
        [layer1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCFadeTo actionWithDuration:4 opacity:0],
           [CCFadeTo actionWithDuration:4 opacity:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
        [sister1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCFadeTo actionWithDuration:2 opacity:0],
           [CCFadeTo actionWithDuration:2 opacity:255],
           [CCFadeTo actionWithDuration:2 opacity:0],
           [CCFadeTo actionWithDuration:2 opacity:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
	}
	return self;
}

-(NSString *) title
{
	return @"CCLayerColor: non-cascading opacity";
}
@end

#pragma mark -
#pragma mark Example LayerTestCascadingColor

@implementation LayerTestCascadingColorA
-(id) init
{
	if( (self=[super init] )) {
        
		CGSize s = [[CCDirector sharedDirector] winSize];
		CCLayerRGBA* layer1 = [CCLayerRGBA node];
        
		CCSprite *sister1 = [CCSprite spriteWithFile:@"grossinis_sister1.png"];
		CCSprite *sister2 = [CCSprite spriteWithFile:@"grossinis_sister2.png"];
		CCLabelBMFont *label = [CCLabelBMFont labelWithString:@"Test" fntFile:@"bitmapFontTest.fnt"];
		
		[layer1 addChild:sister1];
		[layer1 addChild:sister2];
		[layer1 addChild:label];
		[self addChild: layer1 z:0 tag:kTagLayer];
		
		sister1.position = ccp( s.width*1/3, s.height/2);
		sister2.position = ccp( s.width*2/3, s.height/2);
		label.position = ccp( s.width/2, s.height/2);
        
        [layer1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCTintTo actionWithDuration:6 red:255 green:0 blue:255],
           [CCTintTo actionWithDuration:6 red:255 green:255 blue:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
        [sister1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:0],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:0 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:0 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
		// Enable cascading in scene
		[self setEnableRecursiveCascading:YES];
        
    }
	return self;
}

-(NSString *) title
{
	return @"LayerRGBA: cascading color";
}
@end

@implementation LayerTestCascadingColorB
-(id) init
{
	if( (self=[super init] )) {
        
		CGSize s = [[CCDirector sharedDirector] winSize];
		CCLayerColor* layer1 = [CCLayerColor layerWithColor:ccc4(255, 255, 255, 255) width:s.width height:s.height/2];
        
        layer1.position = ccp(0, s.height/2);
        
		CCSprite *sister1 = [CCSprite spriteWithFile:@"grossinis_sister1.png"];
		CCSprite *sister2 = [CCSprite spriteWithFile:@"grossinis_sister2.png"];
		CCLabelBMFont *label = [CCLabelBMFont labelWithString:@"Test" fntFile:@"bitmapFontTest.fnt"];
		
		[layer1 addChild:sister1];
		[layer1 addChild:sister2];
		[layer1 addChild:label];
		[self addChild: layer1 z:0 tag:kTagLayer];
		
		sister1.position = ccp( s.width*1/3, 0);
		sister2.position = ccp( s.width*2/3, 0);
		label.position = ccp( s.width/2, 0);
        
        [layer1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCTintTo actionWithDuration:6 red:255 green:0 blue:255],
           [CCTintTo actionWithDuration:6 red:255 green:255 blue:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
        [sister1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:0],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:0 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:0 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
		
		// Enable cascading in scene
		[self setEnableRecursiveCascading:YES];
	}
	return self;
}

-(NSString *) title
{
	return @"CCLayerColor: cascading color";
}
@end

@implementation LayerTestCascadingColorC
-(id) init
{
	if( (self=[super init] )) {
        
		CGSize s = [[CCDirector sharedDirector] winSize];
		CCLayerColor* layer1 = [CCLayerColor layerWithColor:ccc4(255, 255, 255, 255) width:s.width height:s.height/2];
        layer1.cascadeColorEnabled = NO;
        layer1.position = ccp(0, s.height/2);
        
		CCSprite *sister1 = [CCSprite spriteWithFile:@"grossinis_sister1.png"];
		CCSprite *sister2 = [CCSprite spriteWithFile:@"grossinis_sister2.png"];
		CCLabelBMFont *label = [CCLabelBMFont labelWithString:@"Test" fntFile:@"bitmapFontTest.fnt"];
		
		[layer1 addChild:sister1];
		[layer1 addChild:sister2];
		[layer1 addChild:label];
		[self addChild: layer1 z:0 tag:kTagLayer];
		
		sister1.position = ccp( s.width*1/3, 0);
		sister2.position = ccp( s.width*2/3, 0);
		label.position = ccp( s.width/2, 0);
        
        [layer1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCTintTo actionWithDuration:6 red:255 green:0 blue:255],
           [CCTintTo actionWithDuration:6 red:255 green:255 blue:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
        
        [sister1 runAction:
         [CCRepeatForever actionWithAction:
          [CCSequence actions:
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:0],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:0 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:0 blue:255],
           [CCTintTo actionWithDuration:2 red:255 green:255 blue:255],
           [CCDelayTime actionWithDuration:1],
           nil]]];
	}
	return self;
}

-(NSString *) title
{
	return @"CCLayerColor: non-cascading color";
}
@end

//------------------------------------------------------------------
//
// LayerTest1
//
//------------------------------------------------------------------
void LayerTest1::onEnter()
{
    LayerTest::onEnter();

    setTouchEnabled(true);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* layer = CCLayerColor::create( ccc4(0xFF, 0x00, 0x00, 0x80), 200, 200); 
    
    layer->ignoreAnchorPointForPosition(false);
    layer->setPosition( ccp(s.width/2, s.height/2) );
    addChild(layer, 1, kTagLayer);
}

void LayerTest1::updateSize(CCPoint &touchLocation)
{    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCSize newSize = CCSizeMake( fabs(touchLocation.x - s.width/2)*2, fabs(touchLocation.y - s.height/2)*2);
    
    CCLayerColor* l = (CCLayerColor*) getChildByTag(kTagLayer);

    l->setContentSize( newSize );
}

void LayerTest1::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesMoved(pTouches, pEvent);
}

void LayerTest1::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint touchLocation = touch->getLocation();

    updateSize(touchLocation);
}

void LayerTest1::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesMoved(pTouches, pEvent);
}

std::string LayerTest1::title()
{
    return "ColorLayer resize (tap & move)";
}

//------------------------------------------------------------------
//
// LayerTest2
//
//------------------------------------------------------------------
void LayerTest2::onEnter()
{
    LayerTest::onEnter();

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* layer1 = CCLayerColor::create( ccc4(255, 255, 0, 80), 100, 300);
    layer1->setPosition(ccp(s.width/3, s.height/2));
    layer1->ignoreAnchorPointForPosition(false);
    addChild(layer1, 1);
    
    CCLayerColor* layer2 = CCLayerColor::create( ccc4(0, 0, 255, 255), 100, 300);
    layer2->setPosition(ccp((s.width/3)*2, s.height/2));
    layer2->ignoreAnchorPointForPosition(false);
    addChild(layer2, 1);
    
    CCActionInterval* actionTint = CCTintBy::create(2, -255, -127, 0);
    CCActionInterval* actionTintBack = actionTint->reverse();
    CCActionInterval* seq1 = CCSequence::create( actionTint, actionTintBack, NULL);
    layer1->runAction(seq1);

    CCActionInterval* actionFade = CCFadeOut::create(2.0f);
    CCActionInterval* actionFadeBack = actionFade->reverse();
    CCActionInterval* seq2 = CCSequence::create(actionFade, actionFadeBack, NULL);        
    layer2->runAction(seq2);
}

std::string LayerTest2::title()
{
    return "ColorLayer: fade and tint";
}

//------------------------------------------------------------------
//
// LayerTestBlend
//
//------------------------------------------------------------------

LayerTestBlend::LayerTestBlend()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* layer1 = CCLayerColor::create( ccc4(255, 255, 255, 80) );
    
    CCSprite* sister1 = CCSprite::create(s_pPathSister1);
    CCSprite* sister2 = CCSprite::create(s_pPathSister2);
    
    addChild(sister1);
    addChild(sister2);
    addChild(layer1, 100, kTagLayer);
    
    sister1->setPosition( ccp( s.width*1/3, s.height/2) );
    sister2->setPosition( ccp( s.width*2/3, s.height/2) );

    schedule( schedule_selector(LayerTestBlend::newBlend), 1.0f);
}

void LayerTestBlend::newBlend(float dt)
{
     CCLayerColor *layer = (CCLayerColor*)getChildByTag(kTagLayer);

    GLenum src;
    GLenum dst;

    if( layer->getBlendFunc().dst == GL_ZERO )
    {
        src = GL_SRC_ALPHA;
        dst = GL_ONE_MINUS_SRC_ALPHA;
    }
    else
    {
        src = GL_ONE_MINUS_DST_COLOR;
        dst = GL_ZERO;
    }

    ccBlendFunc bf = {src, dst};
    layer->setBlendFunc( bf );
}


std::string LayerTestBlend::title()
{
    return "ColorLayer: blend";
}

//------------------------------------------------------------------
//
// LayerGradient
//
//------------------------------------------------------------------
LayerGradient::LayerGradient()
{
    CCLayerGradient* layer1 = CCLayerGradient::create(ccc4(255,0,0,255), ccc4(0,255,0,255), ccp(0.9f, 0.9f));
    addChild(layer1, 0, kTagLayer);

    setTouchEnabled(true);

    CCLabelTTF *label1 = CCLabelTTF::create("Compressed Interpolation: Enabled", "Marker Felt", 26);
    CCLabelTTF *label2 = CCLabelTTF::create("Compressed Interpolation: Disabled", "Marker Felt", 26);
    CCMenuItemLabel *item1 = CCMenuItemLabel::create(label1);
    CCMenuItemLabel *item2 = CCMenuItemLabel::create(label2);
    CCMenuItemToggle *item = CCMenuItemToggle::createWithTarget(this, menu_selector(LayerGradient::toggleItem), item1, item2, NULL);

    CCMenu *menu = CCMenu::create(item, NULL);
    addChild(menu);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    menu->setPosition(ccp(s.width / 2, 100));
}

void LayerGradient::toggleItem(CCObject *sender)
{
    CCLayerGradient *gradient = (CCLayerGradient*)getChildByTag(kTagLayer);
    gradient->setCompressedInterpolation(! gradient->isCompressedInterpolation());
}

void LayerGradient::ccTouchesMoved(CCSet * touches, CCEvent *event)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCSetIterator it = touches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    CCPoint start = touch->getLocation();    

    CCPoint diff = ccpSub( ccp(s.width/2,s.height/2), start);    
    diff = ccpNormalize(diff);

    CCLayerGradient *gradient = (CCLayerGradient*) getChildByTag(1);
    gradient->setVector(diff);
}

std::string LayerGradient::title()
{
    return "LayerGradient";
}

string LayerGradient::subtitle()
{
    return "Touch the screen and move your finger";
}

// LayerIgnoreAnchorPointPos

#define kLayerIgnoreAnchorPoint  1000

void LayerIgnoreAnchorPointPos::onEnter()
{
    LayerTest::onEnter();

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCLayerColor *l = CCLayerColor::create(ccc4(255, 0, 0, 255), 150, 150);

    l->setAnchorPoint(ccp(0.5f, 0.5f));
    l->setPosition(ccp( s.width/2, s.height/2));

    CCMoveBy *move = CCMoveBy::create(2, ccp(100,2));
    CCMoveBy * back = (CCMoveBy *)move->reverse();
    CCSequence *seq = CCSequence::create(move, back, NULL);
    l->runAction(CCRepeatForever::create(seq));
    this->addChild(l, 0, kLayerIgnoreAnchorPoint);

    CCSprite *child = CCSprite::create("Images/grossini.png");
    l->addChild(child);
    CCSize lsize = l->getContentSize();
    child->setPosition(ccp(lsize.width/2, lsize.height/2));

    CCMenuItemFont *item = CCMenuItemFont::create("Toggle ignore anchor point", this, menu_selector(LayerIgnoreAnchorPointPos::onToggle));

    CCMenu *menu = CCMenu::create(item, NULL);
    this->addChild(menu);

    menu->setPosition(ccp(s.width/2, s.height/2));
}

void LayerIgnoreAnchorPointPos::onToggle(CCObject* pObject)
{
    CCNode* pLayer = this->getChildByTag(kLayerIgnoreAnchorPoint);
    bool ignore = pLayer->isIgnoreAnchorPointForPosition();
    pLayer->ignoreAnchorPointForPosition(! ignore);
}

std::string LayerIgnoreAnchorPointPos::title()
{
    return "IgnoreAnchorPoint - Position";
}

std::string LayerIgnoreAnchorPointPos::subtitle()
{
    return "Ignoring Anchor Point for position";
}

// LayerIgnoreAnchorPointRot

void LayerIgnoreAnchorPointRot::onEnter()
{
    LayerTest::onEnter();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCLayerColor *l = CCLayerColor::create(ccc4(255, 0, 0, 255), 200, 200);

    l->setAnchorPoint(ccp(0.5f, 0.5f));
    l->setPosition(ccp( s.width/2, s.height/2));

    this->addChild(l, 0, kLayerIgnoreAnchorPoint);

    CCRotateBy *rot = CCRotateBy::create(2, 360);
    l->runAction(CCRepeatForever::create(rot));


    CCSprite *child = CCSprite::create("Images/grossini.png");
    l->addChild(child);
    CCSize lsize = l->getContentSize();
    child->setPosition(ccp(lsize.width/2, lsize.height/2));

    CCMenuItemFont *item = CCMenuItemFont::create("Toogle ignore anchor point", this, menu_selector(LayerIgnoreAnchorPointRot::onToggle));

    CCMenu *menu = CCMenu::create(item, NULL);
    this->addChild(menu);

    menu->setPosition(ccp(s.width/2, s.height/2));
}

void LayerIgnoreAnchorPointRot::onToggle(CCObject* pObject)
{
    CCNode* pLayer = this->getChildByTag(kLayerIgnoreAnchorPoint);
    bool ignore = pLayer->isIgnoreAnchorPointForPosition();
    pLayer->ignoreAnchorPointForPosition(! ignore);
}

std::string LayerIgnoreAnchorPointRot::title()
{
    return "IgnoreAnchorPoint - Rotation";
}

std::string LayerIgnoreAnchorPointRot::subtitle()
{
    return "Ignoring Anchor Point for rotations";
}

// LayerIgnoreAnchorPointScale
void LayerIgnoreAnchorPointScale::onEnter()
{
    LayerTest::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCLayerColor *l = CCLayerColor::create(ccc4(255, 0, 0, 255), 200, 200);

    l->setAnchorPoint(ccp(0.5f, 1.0f));
    l->setPosition(ccp( s.width/2, s.height/2));


    CCScaleBy *scale = CCScaleBy::create(2, 2);
    CCScaleBy* back = (CCScaleBy*)scale->reverse();
    CCSequence *seq = CCSequence::create(scale, back, NULL);

    l->runAction(CCRepeatForever::create(seq));

    this->addChild(l, 0, kLayerIgnoreAnchorPoint);

    CCSprite *child = CCSprite::create("Images/grossini.png");
    l->addChild(child);
    CCSize lsize = l->getContentSize();
    child->setPosition(ccp(lsize.width/2, lsize.height/2));

    CCMenuItemFont *item = CCMenuItemFont::create("Toogle ignore anchor point", this, menu_selector(LayerIgnoreAnchorPointScale::onToggle));

    CCMenu *menu = CCMenu::create(item, NULL);
    this->addChild(menu);

    menu->setPosition(ccp(s.width/2, s.height/2));
}

void LayerIgnoreAnchorPointScale::onToggle(CCObject* pObject)
{
    CCNode* pLayer = this->getChildByTag(kLayerIgnoreAnchorPoint);
    bool ignore = pLayer->isIgnoreAnchorPointForPosition();
    pLayer->ignoreAnchorPointForPosition(! ignore);
}

std::string LayerIgnoreAnchorPointScale::title()
{
    return "IgnoreAnchorPoint - Scale";
}

std::string LayerIgnoreAnchorPointScale::subtitle()
{
    return "Ignoring Anchor Point for scale";
}

void LayerTestScene::runThisTest()
{
    sceneIdx = -1;
    CCLayer* pLayer = nextAction();
    addChild(pLayer);

    CCDirector::sharedDirector()->replaceScene(this);
}

LayerExtendedBlendOpacityTest::LayerExtendedBlendOpacityTest()
{
    CCLayerGradient* layer1 = CCLayerGradient::create(ccc4(255, 0, 0, 255), ccc4(255, 0, 255, 255));
    layer1->setContentSize(CCSizeMake(80, 80));
    layer1->setPosition(ccp(50,50));
    addChild(layer1);
    
    CCLayerGradient* layer2 = CCLayerGradient::create(ccc4(0, 0, 0, 127), ccc4(255, 255, 255, 127));
    layer2->setContentSize(CCSizeMake(80, 80));
    layer2->setPosition(ccp(100,90));
    addChild(layer2);
    
    CCLayerGradient* layer3 = CCLayerGradient::create();
    layer3->setContentSize(CCSizeMake(80, 80));
    layer3->setPosition(ccp(150,140));
    layer3->setStartColor(ccc3(255, 0, 0));
    layer3->setEndColor(ccc3(255, 0, 255));
    layer3->setStartOpacity(255);
    layer3->setEndOpacity(255);
    ccBlendFunc blend;
    blend.src = GL_SRC_ALPHA;
    blend.dst = GL_ONE_MINUS_SRC_ALPHA;
    layer3->setBlendFunc(blend);
    addChild(layer3);
}

string LayerExtendedBlendOpacityTest::title()
{
    return "Extended Blend & Opacity";
}

string LayerExtendedBlendOpacityTest::subtitle()
{
    return "You should see 3 layers";
}

