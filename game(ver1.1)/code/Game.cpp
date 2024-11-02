#include<iostream>
#include<fstream>
#include"./Header/Game.h"
Game::Game (const std::string &file_name)
{
    initialize(file_name);
}
void Game::initialize (const std::string &file_name)
{
    if (!m_font.loadFromFile("./assets/font_1.ttf"))
    {
        std::cout << "Could not load fonts";
        exit(1);
    }
    m_text.setFont(m_font);
    std::ifstream file;
    file.open(file_name, std::ios::in);
    //  Window Config
    file >> window_config.width >> window_config.hight >> window_config.frame_rate;
    std::cout << "Frame Rate: " << window_config.frame_rate << std::endl;
    //  Player Config
    file >> player_config.radius >> player_config.point >> player_config.outline_thickness >> player_config.p_speed >> player_config.p_vec.x >> player_config.p_vec.y;
    //  Enemy Config
    file >> enemy_config.radius_min >> enemy_config.radius_max >> enemy_config.point_min >> enemy_config.point_max >> enemy_config.speed >> enemy_config.outline_thickness >> enemy_config.e_spawn_interval;
    //  Bullet config
    file >> bullet_config.b_radius >> bullet_config.b_point >> bullet_config.b_outline_thickness >> bullet_config.b_lifetime >> bullet_config.b_speed;
    file.close();
    std::cout << "debug w" << std::endl;
    //  Initialize Window
    m_window.create(sf::VideoMode(window_config.width, window_config.hight), "Game");
    m_window.setFramerateLimit(window_config.frame_rate);
    m_window.setVerticalSyncEnabled(true);

    //  Initialize Arrow
    m_arrow.setSize(sf::Vector2f(80,2));
    m_arrow.setOutlineThickness(1);
    m_arrow.setOutlineColor(sf::Color::Yellow);
    m_arrow.setFillColor(sf::Color::White);

    //  Initializing text
    t_score.setFont(m_font);
    t_score.setStyle(sf::Text::Bold);
    t_score.setCharacterSize(55);
    t_score.setOutlineThickness(1);
    t_score.setOutlineColor(sf::Color::Red);
    t_score.setFillColor(sf::Color::White);
    t_score.setPosition(10,75);
    t_score.setString("Score 0");

    t_score_multiplier.setFont(m_font);
    t_score_multiplier.setStyle(sf::Text::Bold);
    t_score_multiplier.setCharacterSize(55);
    t_score_multiplier.setOutlineThickness(1);
    t_score_multiplier.setOutlineColor(sf::Color::Red);
    t_score_multiplier.setFillColor(sf::Color::White);
    t_score_multiplier.setPosition(10,10);
    t_score_multiplier.setString("Score Multiplier 1.0x");

    cooldown            = 0;
    score               = 0;
    score_multiplier    = 1.0f;
    frame               = 0;
    sec                 = 0;
    min                 = 0;

}

void Game::spawn_player ()
{
    auto e = m_entities.Add_enetity(std::string("player"));
    //  Player Components
    e->cShape       = std::make_shared<CShape>      (player_config.radius, player_config.point, sf::Color::Black, sf::Color::Cyan, player_config.outline_thickness, Vec2(window_config.width/2, window_config.hight/2));
    e->cTransform   = std::make_shared<CTransform>  (Vec2(player_config.p_vec), player_config.p_speed);
    e->cInput       = std::make_shared<CInput>      ();
    e->cCollision   = std::make_shared<CCollision>  (player_config.radius);

    m_player = e;
}
void Game::spawn_enemy()
{
    float radius = m_random.int_random(enemy_config.radius_min , enemy_config.radius_max);
    Vec2 position(m_random.int_random(radius +1, window_config.width - radius -1 ), m_random.int_random(radius +1, window_config.hight - radius -1) );   
    //  If enemy is too close to player, it will not spawn
    if (m_player->cShape->position.distance_sq(position) <= (player_config.radius*player_config.radius*150))
    {
        std::cout << "Enemy was too close Call spawn again" << std::endl;
        spawn_enemy();
        return;
    }
    float n_x       = m_random.int_random(0, 100);
    float n_y       = m_random.int_random(0, 100);
    sf::Color       fill_colour(m_random.int_random(0,254), m_random.int_random(0,254), m_random.int_random(0,254));
    if (n_x == 0 && n_y == 0)
    {
        n_y = 1;
    }
    if (m_random.int_random(0,2) == 1)
    {
        n_x = -n_x;
    }
    if (m_random.int_random(0,2) == 1)
    {
        n_y = -n_y;
    }
    int points  = m_random.int_random(enemy_config.point_min, enemy_config.point_max); 
    auto e = m_entities.Add_enetity("enemy");
    //  Components
    e->cShape       = std::make_shared<CShape>      (radius, points, fill_colour, sf::Color::Red, enemy_config.outline_thickness, position);
    e->cTransform   = std::make_shared<CTransform>  (Vec2(n_x,n_y), enemy_config.speed);
    e->cCollision   = std::make_shared<CCollision>  (radius);
    e->cPoints      = std::make_shared<CPoints>     (10);
}
void Game::spawn_smart_enemy()
{
    float radius = m_random.int_random(enemy_config.radius_min , enemy_config.radius_max);
    Vec2 position(m_random.int_random(radius +1, window_config.width - radius -1 ), m_random.int_random(radius +1, window_config.hight - radius -1) );   
    //  If enemy is too close to player, it will not spawn
    if (m_player->cShape->position.distance_sq(position) <= (player_config.radius*player_config.radius*150))
    {
        std::cout << "Enemy was too close Call spawn again" << std::endl;
        spawn_enemy();
        return;
    }
    sf::Color       fill_colour(m_random.int_random(0,254), m_random.int_random(0,254), m_random.int_random(0,254));
    int points  = m_random.int_random(enemy_config.point_min, enemy_config.point_max); 
    auto e = m_entities.Add_enetity("smart_enemy");
    //  Components
    e->cShape       = std::make_shared<CShape>      (radius, points, fill_colour, sf::Color::Red, enemy_config.outline_thickness, position);
    e->cTransform   = std::make_shared<CTransform>  (m_player->cShape->position - position, 3);
    e->cCollision   = std::make_shared<CCollision>  (radius);
    e->cPoints      = std::make_shared<CPoints>     (15);   
}
void Game::spawn_bullet(float x, float y)
{
    Vec2 mouse_click(x,y);
    auto e = m_entities.Add_enetity("bullet");
    //  Now set Shape, Movement and lifespan
    e->cShape       = std::make_shared<CShape>      (bullet_config.b_radius, bullet_config.b_point, sf::Color::Black, sf::Color::White, bullet_config.b_outline_thickness, m_player->cShape->position);
    e->cTransform   = std::make_shared<CTransform>  (mouse_click - m_player->cShape->position, bullet_config.b_speed);
    e->cLifespan    = std::make_shared<CLifespan>   (bullet_config.b_lifetime);
    e->cCollision   = std::make_shared<CCollision>  (bullet_config.b_radius);
}
void Game::spawn_shooting_enemy()
{
    float radius = m_random.int_random(enemy_config.radius_min , enemy_config.radius_max);
    Vec2 position(m_random.int_random(radius +1, window_config.width - radius -1 ), m_random.int_random(radius +1, window_config.hight - radius -1) );   
    //  If enemy is too close to player, it will not spawn
    if (m_player->cShape->position.distance_sq(position) <= (player_config.radius*player_config.radius*150))
    {
        std::cout << "Enemy was too close Call spawn again" << std::endl;
        spawn_enemy();
        return;
    }
    sf::Color       fill_colour(m_random.int_random(0,254), m_random.int_random(0,254), m_random.int_random(0,254));
    int points  = m_random.int_random(enemy_config.point_min, enemy_config.point_max); 
    auto e = m_entities.Add_enetity("shooting_enemy");
    //  Components
    e->cShape       = std::make_shared<CShape>      (radius, points, fill_colour, sf::Color::Red, enemy_config.outline_thickness, position);
    e->cTransform   = std::make_shared<CTransform>  (Vec2(0,0), 0);
    e->cCollision   = std::make_shared<CCollision>  (radius);
    e->cAi          = std::make_shared<CAi>         (2*window_config.frame_rate);
    e->cPoints      = std::make_shared<CPoints>     (25);
}
void Game::spawn_enemy_bullet(std::shared_ptr<Entity> enemy)
{
    auto e = m_entities.Add_enetity("enemy_bullet");
    e->cShape       = std::make_shared<CShape>      (bullet_config.b_radius, bullet_config.b_point, sf::Color::White, sf::Color::Red, bullet_config.b_outline_thickness, enemy->cShape->position);
    e->cCollision   = std::make_shared<CCollision>  (bullet_config.b_radius);
    e->cLifespan    = std::make_shared<CLifespan>   (bullet_config.b_lifetime);
    e->cTransform   = std::make_shared<CTransform>  (m_player->cShape->position - e->cShape->position, bullet_config.b_speed);
}
void Game::kill_enemy(std::shared_ptr<Entity> e)
{
    int points  = e->cShape->circle.getPointCount();
    float angle = 360/points;
    for (int i=1; i<=points ;i++)
    {
        auto s_e = m_entities.Add_enetity("small_entity");
        //  Small Entity Components
        s_e->cShape     = std::make_shared<CShape>      (e->cShape->radius/2, points, e->cShape->circle.getFillColor(), e->cShape->circle.getOutlineColor(), e->cShape->circle.getOutlineThickness()/2, e->cShape->position);
        if (e->cTransform->normal_vec == Vec2(0,0))
        {
            s_e->cTransform = std::make_shared<CTransform>  (Vec2(0,1).add_angle(angle*i), 5);
        }
        else
        {
            s_e->cTransform = std::make_shared<CTransform>  (e->cTransform->normal_vec.add_angle(angle*i), 5);
        }
        s_e->cLifespan  = std::make_shared<CLifespan>   (30);
    }
    if (e->cPoints)
    {
        score += e->cPoints->points * score_multiplier;
        t_score.setString("Score " + std::to_string(score));
    }
    e->destroy();
}

void Game::special_atk()
{
    //  Generate disks around the player, the move out a little then stay in place for a little while
    //  disk number  =  8 (for 8 directions)
    //  disk size ideally should be able to update according to the size of player
    //  First set parameters for the disk, like shape and size, shape the same as player and size the same as player
    //  Will have tag of special_bullet
    if (cooldown == 0)
    {
        for (int i=0; i<8; i++)
        {
            auto e = m_entities.Add_enetity("special_bullet");
            e->cShape       = std::make_shared<CShape>      (player_config.radius, player_config.point, m_player->cShape->circle.getFillColor(), sf::Color::White, m_player->cShape->circle.getOutlineThickness(), m_player->cShape->position);
            e->cCollision   = std::make_shared<CCollision>  (player_config.radius);
            e->cTransform   = std::make_shared<CTransform>  (Vec2(1,0).add_angle(45*i), 6);
            e->cLifespan    = std::make_shared<CLifespan>   (180);
        }
        cooldown = 3*window_config.frame_rate;
    }
}

void Game::sMove ()
{
    for (auto e: m_entities.get_entities("smart_enemy"))
    {
        e->cTransform->normal_vec = m_player->cShape->position - e->cShape->position;
        e->cTransform->normal_vec.normalize();
    }
    for (auto e : m_entities.get_entities())
    {
        if (e->cShape)
        {
            e->cShape->circle.setRotation(e->cShape->circle.getRotation() + 4.0);
            if (e->cTransform)
            {
                e->cShape->position+=(e->cTransform->normal_vec*e->cTransform->speed);
                e->cShape->circle.setPosition(e->cShape->position.x, e->cShape->position.y);
            }
        }
    }
}
void Game::sWindow_bound()
{
    //  NOTE: Window bounds are checked according to the component shape, not with cCollision
    for (auto e: m_entities.get_entities())
    {
        //  CHECK COLLISION WITH WINDOW BOUNDS
        //  Contiue is Entity tag is bullet, small entitiy or special_bullet
        if (e->tag() == "small_entity" | e->tag() == "bullet" | e->tag() == "special_bullet" | e->tag() == "enemy_bullet")
        {
            continue;
        }
        //  If Entity has both Shape and Transform but not input
        if ((e->cShape && e->cTransform) && !e->cInput)
        {
            Vec2 top_left(e->cShape->position - (e->cShape->radius + e->cShape->circle.getOutlineThickness()/2));
            Vec2 bottom_right(top_left + (e->cShape->radius*2) + e->cShape->circle.getOutlineThickness());
            //  Check Top Collision
            if (e->cTransform->normal_vec.y < 0)
            {
                if (top_left.y <= 0)
                {
                    e->cTransform->normal_vec.y = -e->cTransform->normal_vec.y;
                } 
            }
            //  Check Bottom Collision  (will be skipped if top collision)
            else if (bottom_right.y >= window_config.hight)
            {    
                e->cTransform->normal_vec.y = -e->cTransform->normal_vec.y;
            }
            //  Check Left Collision
            if (e->cTransform->normal_vec.x <= 0)
            {
                if (top_left.x <= 0)
                {
                    e->cTransform->normal_vec.x = -e->cTransform->normal_vec.x;
                }
            }
            //  Check Right Collision   (will be skipped if left collision)
            else if (bottom_right.x >= window_config.width)
            {
                e->cTransform->normal_vec.x = -e->cTransform->normal_vec.x;
            }
        }
        //  If entity has Input component then we will use other logic
        else if ((e->cInput) && (e->cTransform) && (e->cShape))
        {
            Vec2 top_left(e->cShape->position - (e->cShape->radius + e->cShape->circle.getOutlineThickness()/2));
            Vec2 bottom_right(top_left + (e->cShape->radius*2) + e->cShape->circle.getOutlineThickness());
            //  Check Top Collision
            if (top_left.y <= 0)
            {
                e->cShape->circle.setPosition(e->cShape->position.x, 1 + e->cShape->radius);
                e->cShape->position.y = 1 + e->cShape->radius;
            }
            //  Check Bottom Collision  (will be skipped if top collision)
            else if (bottom_right.y >= window_config.hight)
            {    
                e->cShape->circle.setPosition(e->cShape->position.x, window_config.hight - e->cShape->radius -1);
                e->cShape->position.y = window_config.hight - e->cShape->radius - 1;
            }
            //  Check Left Collision
            if (top_left.x <= 0)
            {
                e->cShape->circle.setPosition(1 + e->cShape->radius, e->cShape->position.y);
                e->cShape->position.x = 1 + e->cShape->radius;
            }
            //  Check Right Collision   (will be skipped if left collision)
            else if (bottom_right.x >= window_config.width)
            {
                e->cShape->circle.setPosition(window_config.width - 1 - e->cShape->radius, e->cShape->position.y);
                e->cShape->position.x = window_config.width - 1 - e->cShape->radius;
            }
        }
        //  CHECK COLLISION WITH OTHER ENTITIES
        //  Pending
    }
}
void Game::sInput(char i)
{
    for (auto e: m_entities.get_entities("player"))
    {
        //  Set specific Input to true
        switch (i)
        {
        case 'W':
            e->cInput->up       = true;
            break;
        case 'A':
            e->cInput->left     = true;
            break;
        case 'S':
            e->cInput->down     = true;
            break;
        case 'D':
            e->cInput->right    = true;
            break;
        //  Now we come to releasing a key
        case 'w':
            e->cInput->up       = false;
            break;
        case 'a':
            e->cInput->left     = false;
            break;
        case 's':
            e->cInput->down     = false;
            break;
        case 'd':
            e->cInput->right    = false;
            break;
        default:
            break;
        }
    //  Now we need to check which directional inputs are on and set normal vec accordingly
    //  Clear the vector so no error while setting it
    e->cTransform->normal_vec -= e->cTransform->normal_vec;
    // set each specific input to direction
    if (e->cInput->up)
    {
        e->cTransform->normal_vec.y -= 1;
    }
    if (e->cInput->down)
    {
        e->cTransform->normal_vec.y += 1;
    }
    if (e->cInput->left)
    {
        e->cTransform->normal_vec.x -= 1;
    }
    if (e->cInput->right)
    {
        e->cTransform->normal_vec.x += 1;
    }
    //  Now normalize
    e->cTransform->normal_vec.normalize();
    }
}
void Game::sLifetime()
{
    sf::Color fill_colour;
    sf::Color outline_colour;
    for (auto e: m_entities.get_entities())
    {
        if (e->cLifespan)
        {
            if (e->cLifespan->lifespan <= 0)
            {
                e->destroy();
            }
            e->cLifespan->lifespan --;
            fill_colour     = e->cShape->circle.getFillColor();
            outline_colour  = e->cShape->circle.getOutlineColor();
            float n_a = 254 * (e->cLifespan->lifespan / e->cLifespan->original_lifespan);
            fill_colour.a   = n_a;
            outline_colour.a= n_a;
            if (n_a <= 25)
            {
                outline_colour.a = 25;
            }
            e->cShape->circle.setFillColor(fill_colour);
            e->cShape->circle.setOutlineColor(outline_colour);
            if (e->tag() == "small_entity")
            {
                if (e->cTransform->speed > 2)
                {
                    e->cTransform->speed *= (e->cLifespan->lifespan / e->cLifespan->original_lifespan);
                }
            }
            else if (e->tag() == "special_bullet")
            {
                if (e->cLifespan->lifespan / e->cLifespan->original_lifespan <= 0.9)
                {
                    e->cTransform->speed = 0;
                }
            }
        }
    }
}
void Game::sBullet_hit()
{
    //  NOTE: Bullet hit is dected according to cCollision
    for (auto b: m_entities.get_entities("bullet"))
    {
        for (auto e: m_entities.get_entities("enemy"))
        {
            float radius = b->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (b->cShape->position.distance_sq(e->cShape->position)) )
            {
                b->destroy();
                kill_enemy(e);
            }
        }
        for (auto e: m_entities.get_entities("smart_enemy"))
        {
            float radius = b->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (b->cShape->position.distance_sq(e->cShape->position)) )
            {
                b->destroy();
                kill_enemy(e);
            }
        }
        for (auto e: m_entities.get_entities("shooting_enemy"))
        {
            float radius = b->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (b->cShape->position.distance_sq(e->cShape->position)) )
            {
                b->destroy();
                kill_enemy(e);
            }
        }
    }
    for (auto b: m_entities.get_entities("special_bullet"))
    {
        for (auto e: m_entities.get_entities("enemy"))
        {
            float radius = b->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (b->cShape->position.distance_sq(e->cShape->position)) )
            {
                b->destroy();
                kill_enemy(e);
            }
        }
        for (auto e: m_entities.get_entities("smart_enemy"))
        {
            float radius = b->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (b->cShape->position.distance_sq(e->cShape->position)) )
            {
                b->destroy();
                kill_enemy(e);
            }
        }
        for (auto e: m_entities.get_entities("shooting_enemy"))
        {
            float radius = b->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (b->cShape->position.distance_sq(e->cShape->position)) )
            {
                b->destroy();
                kill_enemy(e);
            }
        }
    }
}
void Game::sPlayer_hit()
{
    for (auto p: m_entities.get_entities("player"))
    {
        for (auto e: m_entities.get_entities("enemy"))
        {
            float radius = p->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (p->cShape->position.distance_sq(e->cShape->position)) )
            {
                sqGame_end();
                break;
            }
        }
        for (auto e: m_entities.get_entities("smart_enemy"))
        {
            float radius = p->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (p->cShape->position.distance_sq(e->cShape->position)) )
            {
                sqGame_end();
                break;
            }
        }
        for (auto e: m_entities.get_entities("shooting_enemy"))
        {
            float radius = p->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (p->cShape->position.distance_sq(e->cShape->position)) )
            {
                sqGame_end();
                break;
            }
        }
        for (auto e: m_entities.get_entities("enemy_bullet"))
        {
            float radius = p->cCollision->radius + e->cCollision->radius;
            if ( (radius*radius) >= (p->cShape->position.distance_sq(e->cShape->position)) )
            {
                sqGame_end();
                break;
            }
        }
    }
}
void Game::sEnemy_shoot()
{
    for (auto e: m_entities.get_entities("shooting_enemy"))
    {
        if (--e->cAi->ticks == 0)
        {
            spawn_enemy_bullet(e);
            e->cAi->ticks = e->cAi->shooting_interval;
        }
    }
}

void Game::sqGame_end()
{
    //  Kill player entity
    kill_enemy(m_player);
    //  Loop the window for only small enemies and also keep updating the rotation for 4 seconds
    for (int i=0; i<(3*window_config.frame_rate); i++)
    {
        m_window.clear();
        //  Update rotation of all entities and position only for small entities and bullets
        for (auto e: m_entities.get_entities())
        {
            if (e->cShape)
            {
                e->cShape->circle.setRotation(e->cShape->circle.getRotation() + 4.0);
                m_window.draw(e->cShape->circle);
            }
            if (e->tag() == "small_entity" | e->tag() == "bullet")
            {
                e->cShape->position+=(e->cTransform->normal_vec*e->cTransform->speed);
                e->cShape->circle.setPosition(e->cShape->position.x, e->cShape->position.y);
            }
        }
        m_window.draw(t_score);
        m_window.draw(t_score_multiplier);
        m_window.display();
        sLifetime();
        m_entities.update_manager();
    }

    for (auto e: m_entities.get_entities())
    {
        e->destroy();
    }
    m_entities.update_manager();

    m_text.setString("Game Over");
    m_text.setStyle(sf::Text::Bold);
    m_text.setPosition(window_config.width/2 - (m_text.getLocalBounds().width/2) ,window_config.hight/8);

    t_score.setCharacterSize(window_config.hight/6);
    t_score.setOutlineColor(sf::Color::Blue);
    t_score.setPosition(m_text.getPosition().x, ((window_config.hight/8)*2)+20);

    sf::Text t_retry;
    t_retry.setFont(m_font);
    t_retry.setString("Retry");
    t_retry.setStyle(sf::Text::Bold);
    t_retry.setCharacterSize(window_config.hight/7);
    t_retry.setOutlineThickness(1);
    t_retry.setOutlineColor(sf::Color::Red);
    t_retry.setFillColor(sf::Color::White);
    t_retry.setPosition(window_config.width/2 - (t_retry.getLocalBounds().width/2), ((window_config.hight/8)*3) + 60);

    sf::Text t_exit;
    t_exit.setFont(m_font);
    t_exit.setString("Exit");
    t_exit.setStyle(sf::Text::Bold);
    t_exit.setCharacterSize(window_config.hight/7);
    t_exit.setOutlineThickness(1);
    t_exit.setOutlineColor(sf::Color::Red);
    t_exit.setFillColor(sf::Color::White);
    t_exit.setPosition(t_retry.getPosition().x, ((window_config.hight/8)*4 + 80));

    sf::CircleShape pointer;
    pointer.setPointCount(player_config.point);
    pointer.setRadius(player_config.radius);
    pointer.setFillColor(sf::Color::Black);
    pointer.setOutlineThickness(player_config.outline_thickness);
    pointer.setOutlineColor(sf::Color::Cyan);
    pointer.setOrigin(player_config.radius, player_config.radius);
    pointer.setPosition(t_retry.getPosition().x - 100, t_retry.getPosition().y + t_retry.getLocalBounds().height/2 + player_config.radius + player_config.radius);

    m_retry = true;
    m_window.setFramerateLimit(window_config.frame_rate);
    while(m_window.isOpen())
    {
        sf::Event e;
        while(m_window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                m_window.close();
            }
            else if (e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                    case sf::Keyboard::Escape :
                        m_window.close();
                        break;
                    case sf::Keyboard::W :
                        if (!m_retry)
                        {
                            m_retry = true;
                            pointer.setPosition(pointer.getPosition().x, t_retry.getPosition().y + t_retry.getLocalBounds().height/2 + player_config.radius + player_config.radius);
                        }
                        break;
                    case sf::Keyboard::S :
                        if (m_retry)
                        {
                            m_retry = false;
                            pointer.setPosition(pointer.getPosition().x, t_exit.getPosition().y + t_exit.getLocalBounds().height/2 + player_config.radius + player_config.radius);
                        }
                        break;
                    case sf::Keyboard::Up :
                        if (!m_retry)
                        {
                            m_retry = true;
                            pointer.setPosition(pointer.getPosition().x, t_retry.getPosition().y + t_retry.getLocalBounds().height/2 + player_config.radius + player_config.radius);
                        }
                        break;
                    case sf::Keyboard::Down :
                        if (m_retry)
                        {
                            m_retry = false;
                            pointer.setPosition(pointer.getPosition().x, t_exit.getPosition().y + t_exit.getLocalBounds().height/2 + player_config.radius + player_config.radius);
                        }
                        break;
                    case sf::Keyboard::Return :
                        if (!m_retry)
                        {
                            m_window.close();
                        }
                        else
                        {
                            //  Initialize Arrow
                            m_arrow.setSize(sf::Vector2f(80,2));
                            m_arrow.setOutlineThickness(1);
                            m_arrow.setOutlineColor(sf::Color::Yellow);
                            m_arrow.setFillColor(sf::Color::White);

                            //  Initializing text
                            t_score.setFont(m_font);
                            t_score.setStyle(sf::Text::Bold);
                            t_score.setCharacterSize(55);
                            t_score.setOutlineThickness(1);
                            t_score.setOutlineColor(sf::Color::Red);
                            t_score.setFillColor(sf::Color::White);
                            t_score.setPosition(10,75);
                            t_score.setString("Score 0");
                            
                            t_score_multiplier.setString("Score Multiplier 1.0x");

                            cooldown            = 0;
                            score               = 0;
                            score_multiplier    = 1.0f;
                            frame               = 0;
                            sec                 = 0;
                            min                 = 0;

                            sqGame_start();
                        }
                        return;
                        break;
                    default:
                        break;
                }
            }
        }
        pointer.setRotation(pointer.getRotation() + 2);
        m_window.clear();
        m_window.draw(m_text);
        m_window.draw(t_score);
        m_window.draw(t_retry);
        m_window.draw(t_exit);
        m_window.draw(pointer);
        m_window.display();
    }
    std::cout << "Player Score: " << score << std::endl;
}
void Game::sqGame_start()
{
    m_text.setCharacterSize(window_config.hight/6);
    m_text.setString("Game Start");
    m_text.setStyle(sf::Text::Bold);
    m_text.setOutlineThickness(1);
    m_text.setOutlineColor(sf::Color::Blue);
    m_text.setPosition(window_config.width/2 - (m_text.getLocalBounds().width/2) ,window_config.hight/2 - (m_text.getLocalBounds().height/2));

    spawn_player();
    m_window.setFramerateLimit(5);
    for (int i=0; i<20; i++)
    {
        m_window.clear();
        m_window.draw(m_player->cShape->circle);
        if (i%5 != 0)
        {
            m_window.draw(m_text);
        }
        m_window.display();
    }
    m_window.setFramerateLimit(window_config.frame_rate);
}

void Game::run ()
{
    sqGame_start();
    while (m_window.isOpen())
    {
        //  Event loop
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            switch (event.type)
            {
                //  Case Key pressed
                case (sf::Event::KeyPressed):
                    switch (event.key.code)
                    {
                        case sf::Keyboard::W :
                            sInput('W');
                            break;
                        case sf::Keyboard::A :
                            sInput('A');
                            break;
                        case sf::Keyboard::S :
                            sInput('S');
                            break;
                        case sf::Keyboard::D :
                            sInput('D');
                            break;
                        case sf::Keyboard::Escape :
                            m_window.close();
                            break;
                        default:
                            break;
                    }
                    break;

                //  Case Key released
                case (sf::Event::KeyReleased):
                    switch (event.key.code)
                    {
                    case sf::Keyboard::W :
                        sInput('w');
                        break;
                    case sf::Keyboard::A :
                        sInput('a');
                        break;
                    case sf::Keyboard::S :
                        sInput('s');
                        break;
                    case sf::Keyboard::D :
                        sInput('d');
                        break;
                    case sf::Keyboard::Q :
                        spawn_enemy();
                        break;        
                    default:
                        break;
                    }
                    break;
                
                //  Case mouse click
                case (sf::Event::MouseButtonPressed):
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        spawn_bullet(event.mouseButton.x, event.mouseButton.y);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        special_atk();
                    }
                    break;
                
                //  Case Window Close
                case (sf::Event::Closed):
                    m_window.close();
                    break;

                //  End
                default:
                    break;
            }
        }
        
        //  Update Entity Manager
        m_entities.update_manager();
        
        //  Game logic
        sMove();
        sWindow_bound();
        sLifetime();
        sBullet_hit();
        sPlayer_hit();
        sEnemy_shoot();

        //  Global Time update
        if (++frame == 60)
        {
            frame = 0;
            if (sec%2 == 0 && frame == 0)
            {
                //  Spawn enemy every 2 seconds
                spawn_enemy();
            }
            if (sec%5 == 0 && frame == 0)
            {
                //  Spawn every 5 secs
                spawn_smart_enemy();
                spawn_shooting_enemy();
            }
            if (sec == 30)
            {
                score_multiplier += 0.5;
                t_score_multiplier.setString("Score Multiplier " + std::to_string(min+1) + ".5x");
            }
            if (++sec == 60)
            {
                sec = 0;
                min++;
                t_score_multiplier.setString("Score Multiplier " + std::to_string(min+1) + ".0x");
            }
        }
        if (cooldown-- == 0)
        {
            cooldown = 0;
        }

        //  Mouse arrow logic
        m_arrow.setPosition(m_player->cShape->position.x, m_player->cShape->position.y);
        auto mouse_pos      = sf::Mouse::getPosition(m_window);
        auto arrow_to_mouse = Vec2(mouse_pos.x, mouse_pos.y) - m_player->cShape->position;
        m_arrow.setRotation(arrow_to_mouse.angle());
        int m_arrow_length = arrow_to_mouse.magnitude()/8;
        if (m_arrow_length > 80)
        {
            m_arrow_length = 80;
        }
        else if (m_arrow_length < player_config.radius + 10)
        {
            m_arrow_length = player_config.radius + 10;
        }
        m_arrow.setSize(sf::Vector2f(m_arrow_length, 2));

        //  Window logic
        m_window.clear();
        m_window.draw(m_arrow);
        for (auto e: m_entities.get_entities())
        {
            if (e->cShape)
            {
                m_window.draw(e->cShape->circle);
            }
        }
        m_window.draw(t_score);
        m_window.draw(t_score_multiplier);
        m_window.display();
    }
    std::cout << "Session Lasted: " << min << " Minutes " << sec << " Seconds" << std::endl;
}