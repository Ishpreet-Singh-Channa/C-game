#pragma once
#include"./Components.h"
#include<string>
#include<memory>
class Entity
{
private:
    bool            m_active =  true;
    size_t          m_id     =  -1;
    std::string     m_tag    =  "Default";

public:

    std::shared_ptr<CTransform>     cTransform;
    std::shared_ptr<CCollision>     cCollision;
    std::shared_ptr<CShape>         cShape;
    std::shared_ptr<CInput>         cInput;
    std::shared_ptr<CLifespan>      cLifespan;
    std::shared_ptr<CAi>            cAi;
    std::shared_ptr<CPoints>        cPoints;

    Entity  (size_t id,const std::string &tag);

    const bool  isActive() const;
    size_t      id() const;
    const       std::string& tag() const;
    void        destroy();
};