//
//  DemonTile.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/7/7.
//
//

#include "DemonTile.h"

DemonTile::DemonTile()
{
   
}

DemonTile::~DemonTile()
{
    
}

DemonTile* DemonTile::create(const string &strName)
{
    DemonTile* tile = new DemonTile();
    if (tile)
    {
        tile->initWithFile(strName);
        tile->autorelease();
        Size contentSize = tile->getContentSize();
        //将宽度缩放到69，高等比例缩小，果实是64，留1个间距
        float scale = 69 / contentSize.width;
        tile->setScale(scale);
        return tile;
    }
    else
    {
        delete tile;
        tile = nullptr;
        return nullptr;
    }
}