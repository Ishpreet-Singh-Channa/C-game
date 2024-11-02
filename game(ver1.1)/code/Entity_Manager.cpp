#include"./Header/Entity_Manager.h"

void Entity_Manager::update_manager()
{
    //  Clear the map
    m_entitymap.clear();
    if (active_vector)
    {
        //  Active vector   = 1
        //  Unactive vector = 0
        //  Push all the newly added entities to the map (newly added entities are present in the unactive vector)
        for (auto e: m_entities_0)
        {
            m_entitymap[e->tag()].push_back(e);
        }
        //  Push all the old and ctive entities into the map as well as the unactive vector
        for (std::shared_ptr<Entity> e: m_entities_1)
        {
            if (e->isActive())
            {
                m_entities_0.push_back(e);
                m_entitymap[e->tag()].push_back(e);
            }
        }
        m_entities_1.clear();
        m_totalEntities = m_entities_0.size();
        active_vector = 0;
    }
    else 
    {
        //  Active vector  = 0
        //  Unactive vctor = 1
        //  Push all newly added entities into map (newly added entities in unactive vetor)
        for (auto e: m_entities_1)
        {
            m_entitymap[e->tag()].push_back(e);
        }
        for (auto e: m_entities_0)
        {
            //  Move all active entities in vector 0 and add them to map
            if (e->isActive())
            {
                m_entities_1.push_back(e);
                m_entitymap[e->tag()].push_back(e);
            }
        }
        m_entities_0.clear();
        m_totalEntities = m_entities_1.size();
        active_vector = 1;
    }
}

const Entity_vec Entity_Manager::get_entities()
{
    if (active_vector)
    {
        return m_entities_1;
    }
    else
    {
        return m_entities_0;
    }
}
const Entity_vec Entity_Manager::get_entities(const std::string &tag)
{
    return m_entitymap[tag];
}

const std::shared_ptr<Entity> Entity_Manager::Add_enetity (const std::string &tag)
{
    auto e = std::make_shared<Entity>(m_totalEntities++, tag);
    //  Add the newly made entity into unactive vector
    if (active_vector)
    {
        m_entities_0.push_back(e);
    }
    else
    {
        m_entities_1.push_back(e);
    }
    return e;
}