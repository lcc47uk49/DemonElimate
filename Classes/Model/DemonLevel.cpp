//
//  DemonLevel.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/7/6.
//
//

#include "DemonLevel.h"

DemonLevel::DemonLevel():m_leftBottomPosX(0),m_leftBottomPosY(0)
{
    m_fruitSize = Size(0,0);
    //开辟内存
    m_tileMatrix = (DemonTile***)malloc(sizeof(DemonTile**) * __FRUIT_MATRIX_HEIGHT);//分配内存保存所有行指针
    for (int i = 0; i < __FRUIT_MATRIX_HEIGHT; i++)
    {
        //为每一行分配内存保存所有DemonTile*
        m_tileMatrix[i] = (DemonTile**)malloc(sizeof(DemonTile*) * __FRUIT_MATRIX_WIDTH);
    }
    
    m_fruitMatrix = (DemonFruit***)malloc(sizeof(DemonFruit**) * __FRUIT_MATRIX_HEIGHT);//分配内存保存所有行指针
    for (int i = 0; i < __FRUIT_MATRIX_HEIGHT; i++)
    {
        //为每一行分配内存保存所有DemonFruit*
        m_fruitMatrix[i] = (DemonFruit**)malloc(sizeof(DemonFruit*) * __FRUIT_MATRIX_WIDTH);
    }
    
    m_iMatrix = (int**)malloc(sizeof(int*) * __FRUIT_MATRIX_HEIGHT);
    for (int i = 0; i < __FRUIT_MATRIX_HEIGHT; i++)
    {
        m_iMatrix[i] = (int*)malloc(sizeof(int) * __FRUIT_MATRIX_WIDTH);
    }
    //清空
    m_allPossibleSwaps.clear();
    m_removeMatches.clear();
    m_fallFruits.clear();
}

DemonLevel::~DemonLevel()
{
    //释放内存
    if (m_tileMatrix)
    {
        for (int i = 0; i < __FRUIT_MATRIX_HEIGHT; i++)
        {
            free(m_tileMatrix[i]);
        }
        free(m_tileMatrix);
        m_tileMatrix = nullptr;
    }
    
    if (m_fruitMatrix)
    {
        for (int i = 0; i < __FRUIT_MATRIX_HEIGHT; i++)
        {
            free(m_fruitMatrix[i]);
        }
        free(m_fruitMatrix);
        m_fruitMatrix = nullptr;
    }

    if (m_iMatrix)
    {
        for (int i = 0; i < __FRUIT_MATRIX_HEIGHT; i++)
        {
            free(m_iMatrix[i]);
        }
        free(m_iMatrix);
        m_iMatrix = nullptr;
    }
}

#pragma mark - Level Loading

DemonLevel* DemonLevel::create(const string &strName)
{
    DemonLevel* level = new DemonLevel();
    if (level && level->initWithFile(strName))
    {
        level->autorelease();
        return level;
    }
    else
    {
        delete level;
        level = nullptr;
        return nullptr;
    }
}

bool DemonLevel::initWithFile(const string &strName)
{
    setZeroPoint();
    if(loadJson(strName) == false) return false;//读取后m_doc即被赋值了
    initTileMatrix();//读取地图
    //读取其他数据
    return true;
}

bool DemonLevel::loadJson(const string &strName)
{
    ssize_t size;
    string strPath = FileUtils::getInstance()->fullPathForFilename(strName);
    unsigned char* ch = FileUtils::getInstance()->getFileData(strName, "r", &size);//读取json文件并且获得其大小存入size中
    CCLOG("jsonFile size = %zd",size);//字节数
    string data = string((const char*)ch,size);//使用文件数据创建一个string
    
    //josn操作都在document中
    m_doc.Parse<0>(data.c_str());//解析json数据存入doc中
    if (m_doc.HasParseError())//检错
    {
        CCLOG("GetParaseError:  %s\n",m_doc.GetParseError());
        return false;
    }
    return true;
}

void DemonLevel::initTileMatrix()
{
    //读取地图
    rapidjson::Value &valArray = m_doc["tiles"];
    if(valArray.IsArray())//判断是否为数组
    {
        for (int row = 0; row < valArray.Capacity(); row++)
        {
            rapidjson::Value&valInnerArray = valArray[row];//json第row行
            if(valArray.IsArray())//判断是否为数组
            {
                for (int col = 0; col < valInnerArray.Capacity(); col++)
                {
                    rapidjson::Value&valInt = valInnerArray[col];//json第col列
                    //由于json中tile(0,0)位置在左上角，而创建时(0,0)在左下角，因此要将其倒置]
                    int tileRow = __FRUIT_MATRIX_HEIGHT - row - 1;
                    int tileValue = valInt.GetInt();
                    if (1 == tileValue)
                    {
                        m_tileMatrix[tileRow][col] = DemonTile::create("Tile.png");
                        this->addChild(m_tileMatrix[tileRow][col],0);
                        m_tileMatrix[tileRow][col]->setPosition(getPosOfItem(tileRow, col));
                    }
                    else if (0 == tileValue)
                    {
                        m_tileMatrix[tileRow][col] = nullptr;
                    }
                }
            }
        }
    }
}

#pragma mark - Game Setup

void DemonLevel::shuffle()
{
    //清空向量
    m_allPossibleSwaps.clear();
    m_removeMatches.clear();
    m_fallFruits.clear();
    do {
        initIMatrix();
        initFruitMatrix();//根据m_iMatrix生成果实矩阵
    } while (detectPossibleSwaps() == 0);//如果没有可走的，则重新生成
    printSwaps();
}

void DemonLevel::initIMatrix()
{
    for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
    {
        for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
        {
            if (m_tileMatrix[row][col] != nullptr)
            {
                int fruitType = -1;
                //有地图则可以创建果实
                do
                {
                    vector<int> vecTmp = GameManager::getInstance()->getusedFruitsNo();//选择使用的果实编号向量
                    size_t length = vecTmp.size();
                    //取一个随机位置的元素值
                    int randNo = rand()%length;
                    fruitType = vecTmp[randNo];//果实类型
                }//如果有三个连续相同的则重新生成
                while ((col >= 2 &&
                        m_iMatrix[row][col-1] == fruitType &&
                        m_iMatrix[row][col-2] == fruitType)
                       ||
                       (row >= 2 &&
                        m_iMatrix[row-1][col] == fruitType &&
                        m_iMatrix[row-2][col] == fruitType));
                m_iMatrix[row][col] = fruitType;
                
            }
            else
            {
                m_iMatrix[row][col] = -1;//无果实
            }
        }
    }
}

void DemonLevel::initFruitMatrix()
{
    for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
    {
        for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
        {
            if (m_iMatrix[row][col] != -1)
            {
                m_fruitMatrix[row][col] = DemonFruit::create(row, col, m_iMatrix[row][col]);//创建
                this->addChild(m_fruitMatrix[row][col]);//加入level
                dropFruit(m_fruitMatrix[row][col]);//掉落
            }
            else if(m_iMatrix[row][col] == -1)
            {
                m_fruitMatrix[row][col] = nullptr;
            }
        }
    }
}

#pragma mark - Pos of fruit

void DemonLevel::dropFruit(DemonFruit *fruit)
{
    Size size = Director::getInstance()->getWinSize();
    Point endPos = getPosOfItem(fruit->getRow(),fruit->getCol());//最终坐标
    Point beginPos = Point(endPos.x, endPos.y + size.height / 2);//起始坐标
    fruit->setPosition(beginPos);
    float speed = beginPos.y / (2 * size.height);
    fruit->runAction(MoveTo::create(speed, endPos));
}

void DemonLevel::setZeroPoint()
{
    Size winSize = Director::getInstance()->getWinSize();
    //初始化左下角的点的坐标和果实图片大小,这里创建的果实不addChild
    DemonFruit* fruit = DemonFruit::create(1, 1, 1);
    Rect fruitRect = fruit->boundingBox();
    m_fruitSize = fruitRect.size;
    //果实和空隙总共的宽
    float wid = __FRUIT_MATRIX_WIDTH * m_fruitSize.width + (__FRUIT_MATRIX_WIDTH -1 ) * __FRUIT_GAP;
    //左下角的点的横坐标为左边间隙+图片宽度的一半
    m_leftBottomPosX = (winSize.width - wid)/2 + m_fruitSize.width/2;
    //左下角点的纵坐标可以自己设置
    m_leftBottomPosY = winSize.height*0.15;
}

Point DemonLevel::getPosOfItem(int row, int col)
{
    float x = m_leftBottomPosX + (m_fruitSize.width + __FRUIT_GAP) * col;
    float y = m_leftBottomPosY + (m_fruitSize.height+ __FRUIT_GAP) * row;
    return Point(x, y);
}

DemonFruit* DemonLevel::getFruitofPos(Point pos)
{
    for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
    {
        for (int col = 0 ; col < __FRUIT_MATRIX_WIDTH; col++)
        {
            if (m_fruitMatrix[row][col] != nullptr)
            {
                if (m_fruitMatrix[row][col]->boundingBox().containsPoint(pos))
                {
                    return m_fruitMatrix[row][col];
                }
            }
        }
    }
    return nullptr;
}

DemonFruit* DemonLevel::getFruitofRC(int row, int col)
{
    return m_fruitMatrix[row][col];
}

#pragma mark - Querying the Level

DemonFruit* DemonLevel::fruitAtRC(int row, int col)
{
    if (row >= __FRUIT_MATRIX_HEIGHT || row < 0) return nullptr;
    if (col >= __FRUIT_MATRIX_WIDTH || col < 0) return nullptr;
    return m_fruitMatrix[row][col];
}

DemonTile* DemonLevel::tileAtRc(int row, int col)
{
    if (row >= __FRUIT_MATRIX_HEIGHT || row < 0) return nullptr;
    if (col >= __FRUIT_MATRIX_WIDTH || col < 0) return nullptr;
    return m_tileMatrix[row][col];
}

bool DemonLevel::isPossibleSwap(DemonSwap *swap)
{
    for (auto eachswap : m_allPossibleSwaps)
    {
        if (eachswap->isEqual(swap))
        {
            return true;
        }
    }
    return false;
}

#pragma mark - Detecting Swaps

//先交换，判断交换后是否有可消除的，若有，则加入可消除链表。最后再换回来
long DemonLevel::detectPossibleSwaps()
{
    //清空
    m_allPossibleSwaps.clear();
    //因为从左下角开始判断，所以只需要向右和向上判断就行了
    for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
    {
        for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
        {
            DemonFruit* fruitA = m_fruitMatrix[row][col];
            
            if (fruitA != nullptr)
            {
                //判断与右侧的是否可以交换，不需要检测最后一个
                if(col < __FRUIT_MATRIX_WIDTH - 1)
                {
                    DemonFruit* fruitB = m_fruitMatrix[row][col+1];
                    if (fruitB != nullptr)
                    {
                        //交换
                        m_fruitMatrix[row][col] = fruitB;
                        m_fruitMatrix[row][col+1] = fruitA;
                        //m_iMatrix也要更新，因为hasChainAtRC用的就是m_iMatrix
                        int tmpInt = m_iMatrix[row][col];
                        m_iMatrix[row][col] = m_iMatrix[row][col+1];
                        m_iMatrix[row][col+1] = tmpInt;
                        //检测是否可消除--两个链都要检测
                        if(hasChainAtRC(row, col) || hasChainAtRC(row, col+1))
                        {
                            DemonSwap* swap = DemonSwap::create(fruitA, fruitB);
                            m_allPossibleSwaps.pushBack(swap);//引用加1
                        }
                        //换回来
                        m_fruitMatrix[row][col] = fruitA;
                        m_fruitMatrix[row][col+1] = fruitB;
                        //更新m_iMatrix
                        tmpInt = m_iMatrix[row][col];
                        m_iMatrix[row][col] = m_iMatrix[row][col+1];
                        m_iMatrix[row][col+1] = tmpInt;
                        
                    }
                }
                //判断能否与上方的交换，不需要检测最上一个
                if (row < __FRUIT_MATRIX_HEIGHT - 1)
                {
                    DemonFruit* fruitB = m_fruitMatrix[row+1][col];
                    if (fruitB != nullptr)
                    {
                        //交换
                        m_fruitMatrix[row][col] = fruitB;
                        m_fruitMatrix[row+1][col] = fruitA;
                        //更新m_iMatrix
                        int tmpInt = m_iMatrix[row][col];
                        m_iMatrix[row][col] = m_iMatrix[row+1][col];
                        m_iMatrix[row+1][col] = tmpInt;
                        //检测是否可消除--两个链都要检测
                        if(hasChainAtRC(row, col) || hasChainAtRC(row+1, col))
                        {
                            DemonSwap* swap = DemonSwap::create(fruitA, fruitB);
                            m_allPossibleSwaps.pushBack(swap);
                        }
                        //换回来
                        m_fruitMatrix[row][col] = fruitA;
                        m_fruitMatrix[row+1][col] = fruitB;
                        //更新m_iMatrix
                        tmpInt = m_iMatrix[row][col];
                        m_iMatrix[row][col] = m_iMatrix[row+1][col];
                        m_iMatrix[row+1][col] = tmpInt;
                    }
                }
            }
        }
    }
    return m_allPossibleSwaps.size();
}

void DemonLevel::printSwaps()
{
    for (auto swap : m_allPossibleSwaps)
    {
        CCLOG("(%d,%d) and (%d,%d) can swap",
              swap->m_fruitA->getRow(),
              swap->m_fruitA->getCol(),
              swap->m_fruitB->getRow(),
              swap->m_fruitB->getCol());
    }
}

bool DemonLevel::hasChainAtRC(int row, int col)
{
    int fruitType = m_iMatrix[row][col];//如果没有果实，则为-1，-1与任何都不能成为链
    int horzLength = 1;//行链长度
    for (int i = col - 1; i >= 0 && m_iMatrix[row][i] == fruitType; i--, horzLength++);//向左判断
    for (int i = col + 1; i < __FRUIT_MATRIX_WIDTH && m_iMatrix[row][i] == fruitType; i++, horzLength++);//向右判断
    if (horzLength >= 3) return true;
    
    int vertLength = 1;//列链长度
    for (int i = row - 1; i >= 0 && m_iMatrix[i][col] == fruitType; i--,vertLength++);//向下判断
    for (int i = row + 1; i < __FRUIT_MATRIX_HEIGHT && m_iMatrix[i][col] == fruitType; i++,vertLength++);//向上判断
    if(vertLength >= 3) return true;
    return false;
}

#pragma mark - Swapping

//交换，更新矩阵
void DemonLevel::performSwap(DemonSwap *swap)
{
    int rowA = swap->m_fruitA->getRow();
    int colA = swap->m_fruitA->getCol();
    
    int rowB = swap->m_fruitB->getRow();
    int colB = swap->m_fruitB->getCol();
    
    m_fruitMatrix[rowA][colA] = swap->m_fruitB;
    swap->m_fruitB->setRow(rowA);
    swap->m_fruitB->setCol(colA);
    
    m_fruitMatrix[rowB][colB] = swap->m_fruitA;
    swap->m_fruitA->setRow(rowB);
    swap->m_fruitA->setCol(colB);
    //m_iMatrix也要更新，因为检测是用m_iMatrix检测的
    int tmpType = m_iMatrix[rowA][colA];
    m_iMatrix[rowA][colA] = m_iMatrix[rowB][colB];
    m_iMatrix[rowB][colB] = tmpType;
}

#pragma mark - Detecting Matches

long DemonLevel::detectHorizontalMatches()
{
    long horizontalNo = 0;
    for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
    {
        //最后两列不需要探测，因为不能与下一个形成行链
        for (int col = 0; col < __FRUIT_MATRIX_WIDTH - 2;)//注意！不自增
        {
            if (m_iMatrix[row][col] != -1)//此处有果实
            {
                int matchType = m_iMatrix[row][col];
                if (m_iMatrix[row][col+1] == matchType
                    && m_iMatrix[row][col+2] == matchType)//这里既是m_iMatrie[][]为-1也没事，如果换成m_fruitMatrix[][]->getFruitType()会出错
                {
                    //创建一个链，并且继续遍历
                    DemonChain* chain = DemonChain::create();
                    chain->m_chainType = ChainTypeHorizontal;
                    //根据m_iMatrix判断类型是否相同，若相同，则将m_fruitMatrix中对应的元素加入行链
                    do {
                        chain->addFruit(m_fruitMatrix[row][col]);
                        col += 1;
                    } while (col < __FRUIT_MATRIX_WIDTH
                             && m_iMatrix[row][col] == matchType);
                    //加入m_removeMatches
                    m_removeMatches.pushBack(chain);//引用+1
                    horizontalNo++;
                    continue;//继续检测下一个
                }
            }
            //不成链或者为空
            col += 1;
        }
    }
    return horizontalNo;
}

long DemonLevel::detectVerticalMatches()
{
    long verticalNo = 0;
    for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
    {
        for (int row = 0; row < __FRUIT_MATRIX_HEIGHT - 2; )//不自知
        {
            if(m_iMatrix[row][col] != -1)//此处有果实
            {
                int matchType = m_iMatrix[row][col];
                if (m_iMatrix[row+1][col] == matchType
                    && m_iMatrix[row+2][col] == matchType)
                {
                    DemonChain* chain = DemonChain::create();
                    chain->m_chainType = ChainTypeVertical;
                    do {
                        chain->addFruit(m_fruitMatrix[row][col]);
                        row += 1;
                    } while (row < __FRUIT_MATRIX_HEIGHT
                             && m_iMatrix[row][col] == matchType);
                    m_removeMatches.pushBack(chain);
                    verticalNo++;
//                    CCLOG("removechains size = %zd, chain refcount = %d",
//                          m_removeMatches.size(),chain->getReferenceCount());
                    continue;
                }
            }
            row += 1;
        }
    }
    return verticalNo;
}

void DemonLevel::removeChians()
{
    for (auto chain : m_removeMatches)//遍历Vector
    {
        //遍历vector
        vector<DemonFruit*>::iterator it = chain->m_fruitsChain.begin();
        for (; it != chain->m_fruitsChain.end(); it++)
        {
            DemonFruit* fruit = *it;
            int row = fruit->getRow();
            int col = fruit->getCol();
//            fruit->explode();//2.消除动画放在PlayLayer中
            m_fruitMatrix[row][col] = nullptr;
            m_iMatrix[row][col] = -1;
        }
    }
}

#pragma mark - Detecting Holes

void DemonLevel::fillHoles()
{
    m_fallFruits.clear();
    int *colEmptyInfo = (int *)malloc(sizeof(int) * __FRUIT_MATRIX_WIDTH);//记录每一列的空位个数
    memset((void*)colEmptyInfo, 0, sizeof(int) * __FRUIT_MATRIX_WIDTH);
    Size size = Director::getInstance()->getWinSize();
    
    //1.掉落已存在的
    for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
    {
        for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
        {//从下向上挨个判断
            if (m_tileMatrix[row][col] != nullptr && m_fruitMatrix[row][col] == nullptr)//找到第一个空果实，但是非空地图
            {
                for (int lookUp = row + 1; lookUp < __FRUIT_MATRIX_HEIGHT; lookUp++)//从此果实上一个继续往上找，找到第一个非空果实落到此空果实处
                {
                    DemonFruit* fruit = m_fruitMatrix[lookUp][col];
                    if (fruit != nullptr)
                    {
                        //下落
                        m_fruitMatrix[lookUp][col] = nullptr;
                        m_fruitMatrix[row][col] = fruit;
                        fruit->setRow(row);
                        //更新iMatrix
                        m_iMatrix[lookUp][col] = -1;
                        m_iMatrix[row][col] = fruit->getFruitType();
                        m_fallFruits.pushBack(fruit);
                        //动画在PlayLayer中
                        break;//跳出循环查找下一个空
                    }
                }
                
            }
        }
    }
    //2.生成新的，填补空果实
    fillTop();
}

void DemonLevel::fillTop()
{
    for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
    {
        for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
        {
            if (m_tileMatrix[row][col] != nullptr && m_fruitMatrix[row][col] == nullptr)
            {
                vector<int> vecTmp = GameManager::getInstance()->getusedFruitsNo();//选择使用的果实编号向量
                size_t length = vecTmp.size();
                //取一个随机位置的元素值
                int randNo = rand()%length;
                int fruitType = vecTmp[randNo];//果实类型
                DemonFruit* fruit = DemonFruit::create(row, col, fruitType);
                m_fruitMatrix[row][col] = fruit;
                m_iMatrix[row][col] = fruitType;
                this->addChild(fruit);
                m_fallFruits.pushBack(fruit);
                
                Size size = Director::getInstance()->getWinSize();
                Point endPos = getPosOfItem(fruit->getRow(),fruit->getCol());//最终坐标
                Point beginPos = Point(endPos.x, endPos.y + size.height / 2);//起始坐标
                fruit->setPosition(beginPos);
                //动画--PlayLayer中
            }
        }
    }
}

#pragma mark - Give Hints
void DemonLevel::giveHints()
{
    size_t size = m_allPossibleSwaps.size();
    int randIndex = rand()%size;//随机下标
    DemonSwap* swap = m_allPossibleSwaps.at(randIndex);//随机一个交换组合
    swap->m_fruitA->Hints();
    swap->m_fruitB->Hints();
}

#pragma mark - Give Score
long DemonLevel::giveScore(long basicScore,long number)
{
    long score = basicScore;
    for (long i = number - 3; i > 0; i--)
    {
        score *= 2;
    }
    return score;
}
