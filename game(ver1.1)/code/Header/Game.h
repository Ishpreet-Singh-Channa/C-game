#pragma once
#include"./Custom_random.h"
#include"./Entity_Manager.h"
struct window_config_struct { float width; float hight; int frame_rate; };
struct player_config_struct { float radius; int point; float outline_thickness; float p_speed; Vec2 p_vec;};
struct enemy_config_struct  { int radius_min; int radius_max; int point_min; int point_max; float speed; float outline_thickness; int e_spawn_interval;};
struct bullet_config_struct { int b_radius; int b_point; float b_outline_thickness; int b_lifetime; int b_speed;};
class Game
{
    Custom_random           m_random;
    sf::RenderWindow        m_window;
    bool                    m_paused    =   false;
    bool                    m_running   =   true;
    bool                    m_retry     =   true;
    Entity_Manager          m_entities;
    sf::Font                m_font;
    sf::Text                m_text;
    window_config_struct    window_config;
    player_config_struct    player_config;
    enemy_config_struct     enemy_config;
    bullet_config_struct    bullet_config; 
    int                     frame = 0;
    int                     sec = 0;
    int                     min = 0;
    int                     cooldown = 0;
    int                     score = 0;
    float                   score_multiplier = 1.0;
    sf::Text                t_score;
    sf::Text                t_score_multiplier;
    
    std::shared_ptr<Entity> m_player;
    sf::RectangleShape m_arrow;

    void initialize(const std::string &file_name);

    void spawn_player();
    void spawn_enemy();
    void spawn_smart_enemy();
    void spawn_bullet(float x, float y);
    void spawn_shooting_enemy();
    void spawn_enemy_bullet(std::shared_ptr<Entity> enemy);
    void kill_enemy(std::shared_ptr<Entity> e);

    void special_atk();

    void sMove();
    void sWindow_bound();
    void sInput(char i);
    void sLifetime();
    void sBullet_hit();
    void sPlayer_hit();
    void sEnemy_shoot();
    void sSpawn_in();
    
    void sqGame_start();
    void sqGame_end();
public:
    Game(const std::string &file_name);
    void run();
};