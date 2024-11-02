#pragma once
#include"./Entity.h"
#include<vector>
typedef std::vector<std::shared_ptr<Entity>> Entity_vec;
class Entity_Manager
{
    Entity_vec  m_entities_0;
    Entity_vec  m_entities_1;
    bool        active_vector = 0;
    std::map<std::string, Entity_vec> m_entitymap;
    size_t m_totalEntities = 0;
    
public:
    void update_manager();
    
    const Entity_vec get_entities();
    const Entity_vec get_entities(const std::string &tag);
    
    const std::shared_ptr<Entity> Add_enetity (const std::string &tag);
};
