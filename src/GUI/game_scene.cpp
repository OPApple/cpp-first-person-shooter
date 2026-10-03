#include <iostream>
#include <sstream>
#include "render_system.hpp"
#include "movement_system.hpp"
#include "pickup_system.hpp"
#include "hit_system.hpp"
#include "attack_system.hpp"

#include "game_scene.hpp"
#include "wall.hpp"
#include "enemy_gui.hpp"
#include "pickup_gui.hpp"
#include "gui_constants.hpp"
#include "../Data/data_constants.hpp"
#include "main_menu.hpp"

GameScene* GameScene::current_instance = nullptr;

float lastX = 400, lastY = 300;
bool firstMouse = true;

float delta_time = 0;
float last_frame = 0;

std::shared_ptr<Scene> ReturnHandler(){ 
    return std::make_shared<MainMenu>();
}

GameScene::GameScene(): Scene("Wario's Battle Canyon") { }

void GameScene::Init() {
    SetAsCurrentInstance();

    game_ = Game();
    std::shared_ptr<Player> p = nullptr;
    if (player) {
        p = player->GetPlayer();
    }
    //if there is no player, create one
    if (!p) {
        p = std::make_shared<Player>();
    }
    game_.AddPlayer(p);
    game_.LoadLevel(level_name_);
    entity_handler = std::make_unique<EntityHandler>(game_);
    player = std::make_shared<PlayerGUI>(entity_handler->GetPlayer());
    //set floor and ceiling textures
    entity_handler->Init();
    if (!arms_rend) {
        arms_rend = std::make_unique<ArmsRenderer>();
        arms_rend->Init();
        //set the correct animation based on current weapon on create
        auto current_wep = player->GetPlayer()->GetCurrentWeapon();
        arms_rend->SetCurrentWeapon(current_wep->GetName());
        last_weapon_name = current_wep->GetName();
    }

    //change the bgm for the game scene
    Level level = game_.GetLevel();
    if (level.GetBGM() != "" && GetBGM()!=level.GetBGM()) {
        SetBGM(level.GetBGM());
    }

    dead_menu = Menu(GUIConstants::menu_text_width, GUIConstants::menu_height);
    dead_menu.AddButton(std::make_shared<UIButton>("Return To Menu", ReturnHandler, true));
    dead_menu.AddButton(std::make_shared<UIButton>("Illusion of choice", ReturnHandler));

    pause_menu = Menu(GUIConstants::menu_text_width, GUIConstants::menu_height);
    pause_menu.AddButton(std::make_shared<UIButton>("Continue", [&]{
        paused = false;
        return nullptr;
    }, true));
    pause_menu.AddButton(std::make_shared<UIButton>("Save Game", [&]{
        SaveGameTo(save_slot);
        paused = false;
        return nullptr;
    }, false));
    pause_menu.AddButton(std::make_shared<UIButton>("Load Save", [&]{
        LoadSave(save_slot);
        Init();
        paused = false;
        return nullptr;
    }, false));
    pause_menu.AddButton(std::make_shared<UIButton>("Return to menu", ReturnHandler));

    victory_menu = Menu(GUIConstants::menu_text_width, GUIConstants::menu_height);
    victory_menu.AddButton(std::make_shared<UIButton>("Return to menu", ReturnHandler));
}

void GameScene::SetAsCurrentInstance() {
    current_instance = this;
}

void GameScene::StaticMouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (current_instance) {
        current_instance->MouseCallback(window, xpos, ypos);
    }
}

std::shared_ptr<Scene> GameScene::ProcessInput(GLFWwindow* window, SoundHandler& sound_handler) {
    if (player) {
        player->ProcessInput(window, delta_time, sound_handler);
    }
    
    if (sound_handler.ChangeBGM(GetBGM())) sound_handler.PlayBGM();
    
    if (player->GetPlayer()->Dead()) {
        if (up_listener.IsPressed(window)) {
            dead_menu.PrevButton();
        }
        if (down_listener.IsPressed(window)) {
            dead_menu.NextButton();
        }
        if (enter_listener.IsPressed(window)) {
            return dead_menu.GetActive()->Pressed();
        }
    }
    if (paused) {
        if (up_listener.IsPressed(window)) {
            pause_menu.PrevButton();
        }
        if (down_listener.IsPressed(window)) {
            pause_menu.NextButton();
        }
        if (right_listener.IsPressed(window)) {
            if (save_slot < GUIConstants::max_save_slots) save_slot += 1;
        }
        if (left_listener.IsPressed(window)) {
           if (save_slot > 0) save_slot -= 1;
        }
        if (enter_listener.IsPressed(window)) {
            return pause_menu.GetActive()->Pressed();
        }
    } if (won) {
        if (up_listener.IsPressed(window)) {
            victory_menu.PrevButton();
        }
        if (down_listener.IsPressed(window)) {
            victory_menu.NextButton();
        }
        if (enter_listener.IsPressed(window)) {
            return victory_menu.GetActive()->Pressed();
        }
    }
    
    if(pause_listener.IsPressed(window)) {
        paused = !paused;
    }

    return nullptr;
}

void GameScene::MouseCallback(GLFWwindow*, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed: y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;

    if (player) {
        player->ProcessMouse(xoffset, yoffset);
    }
}

void GameScene::DrawHud(TextRenderer& text_renderer) {
    std::stringstream hp_stream;
    std::stringstream weapon_stream; 
    std::stringstream status_effect_stream;
    std::stringstream reload_stream;
    
    std::stringstream debug_stream;
    
    hp_stream << player->GetPlayer()->GetHp() << "/" << maxHP << " hp";
    int hunger =  player->GetPlayer()->GetHunger();
    if (hunger) hp_stream << " + " << player->GetPlayer()->GetHunger() << " fed";
    weapon_stream << player->GetPlayer()->GetCurrentWeapon()->GetName() << ", " << player->GetPlayer()->GetAmmo();
    debug_stream << player->GetCamera().pos.x << ", " << player->GetCamera().pos.z;
    
    if (!player->GetPlayer()->GetTimers().empty()) {
        for (auto effect : player->GetPlayer()->GetTimers()) {
            int remaining = round(effect->duration - effect->timer);
            status_effect_stream << effect->name << ": " << remaining;
        }
    }
    
    if (player->GetPlayer()->IsReloading()) {
        reload_stream << "Reloading!";
    }
    
    text_renderer.render(hp_stream.str(), GUIConstants::hp_pos.x, GUIConstants::hp_pos.y, 2.0f, GUIConstants::hp_color);
    text_renderer.render(weapon_stream.str(), GUIConstants::weapon_pos.x, GUIConstants::weapon_pos.y, 2.0f, GUIConstants::hp_color);
    text_renderer.render(status_effect_stream.str(), GUIConstants::status_pos.x, GUIConstants::status_pos.y, 1.0f, GUIConstants::hp_color);
    text_renderer.render(reload_stream.str(), GUIConstants::reload_pos.x, GUIConstants::reload_pos.y, 1.0f, GUIConstants::hp_color);
    text_renderer.render("+", GUIConstants::crosshair_pos.x, GUIConstants::crosshair_pos.y, 0.75f, GUIConstants::hp_color);
}

void GameScene::Render(TextRenderer& text_renderer, SoundHandler&) {
    if (player->GetPlayer()->Dead()) {
        text_renderer.render("Dead", GUIConstants::menu_text_width, GUIConstants::menu_text_height, 3.0f, GUIConstants::hp_color);
        dead_menu.Render(text_renderer);
    } else if (paused) {
        text_renderer.render("save slot: " + std::to_string(save_slot), GUIConstants::save_slot_width, GUIConstants::save_slot_height, 2.0f, GUIConstants::button_color);
        pause_menu.Render(text_renderer);
    } else if (won) {
        text_renderer.render("ESCAPED OTANIEMI !!", GUIConstants::menu_text_width, GUIConstants::menu_text_height, 3.0f, GUIConstants::button_color);
        victory_menu.Render(text_renderer);
    } else {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        DrawHud(text_renderer);
        auto current_wep = player->GetPlayer()->GetCurrentWeapon();
        if (current_wep && current_wep->GetName() != last_weapon_name) {
            arms_rend->SetCurrentWeapon(current_wep->GetName());
            last_weapon_name = current_wep->GetName();
        }
        
        bool is_attacking = player->shoot_signal;
        if (player->shoot_signal) {
            bool hit = HitSystem::ResolveHits(entity_handler->GetEntites(), player);
            if (hit) {
                std::cout << "HIT!!!" << std::endl;
            }

            player->shoot_signal = false;
        }
        
        if (player->space_signal) {
            PickupSystem::ResolveSwitchable(entity_handler->GetEntites(), player, this);
            player->space_signal = false;
        }

        for (auto e : entity_handler->GetEntites()) {
            if (e->HasComponent<Opposable>()) {
                auto opp = e->GetComponent<Opposable>();
                auto tran = e->GetComponent<Transform>();
                opp->Update(*tran, delta_time);
            }
        }

        auto movable = player->GetEntity().GetComponent<Movable>();
        bool is_moving = movable && glm::length(movable->velocity) > 0.1f; 
    
        arms_rend->Update(delta_time, is_moving, is_attacking);
    
        HitSystem::ResolveSight(entity_handler->GetEntites(), player);
        MovementSystem::Pathfind(game_.GetLevel(),entity_handler->GetEntites(), player);
        MovementSystem::EnemyVelocity(entity_handler->GetEntites(), player);
        MovementSystem::MoveEntities(entity_handler->GetEntites(), delta_time);
        CollisionSystem::ResolveCollisions(entity_handler->GetEntites());
        PickupSystem::ResolvePickups(entity_handler->GetEntites(), player->GetCamera(), player->GetPlayer());
        RenderSystem::Render(entity_handler->GetEntites(), player->GetCamera(), player, glfwGetTime());
        AttackSystem::AttackPlayer(player, entity_handler->GetEntites(), delta_time);
        
        if (player->hurt_signal) {
            text_renderer.render("#", 0.0f, 0.0f, 50.0f, GUIConstants::hp_color);
            player->hurt_signal = false;
        }

        arms_rend->Render();
    }

}

/**
 * @brief Loads a save file from the saves folder. Loads player WEAPONS, player STATS, and current MAP
 * from a saves[index].txt file.
 * @param index The index for a given save file. Used for file reading.
 */
void GameScene::LoadSave(int index) {
    std::string save_path = GUIConstants::save_folder_path + "save" + std::to_string(index) + ".txt";
    std::ifstream map_file(save_path);

    std::cout << "save index: " << index;

    auto loaded_player = std::make_shared<Player>();
    std::string current;

    //start parse
    std::getline(map_file, current);

    if (current == "#### WEAPONS ####") {
        
        while (std::getline(map_file, current)) {
            // Check for ledger start
            if (current == "#### STATS ####") break;
            std::string weapon_name = current;
            std::string current_ammo;
            std::string stocked_ammo;
            
            std::getline(map_file, current_ammo);
            std::getline(map_file, stocked_ammo);
            Weapon weapon = Weapon(weapon_name);
            weapon.SetCurrentAmmunition(std::stoi(current_ammo));
            weapon.SetStoredAmmunition(std::stoi(stocked_ammo));
            loaded_player->AddWeapon(weapon);
            std::cout << current << "\n added " << weapon_name << std::endl;
        } 
    }
    
    if (current == "#### STATS ####") {
        std::getline(map_file, current);
        int current_health = std::stoi(current);
        loaded_player->SetHp(current_health);
        std::getline(map_file, current);
        int current_hunger = std::stoi(current);
        loaded_player->SetHunger(current_hunger);
        std::getline(map_file, current);
    }
    
    if (current == "#### MAP ####") {
        std::getline(map_file, current);
        level_name_ = GUIConstants::map_folder_path + current;
    }
    //set the player of the scene based on loaded
    player = std::make_shared<PlayerGUI>(loaded_player, glm::vec3(0.0f));
}

/** @brief Saves player WEAPONS, player STATS, and current MAP into a save[index].txt file.
 *  @param index The index for a given save file. Used for file reading.
*/
void GameScene::SaveGameTo(int index) {
    std::string save_path = GUIConstants::save_folder_path + "save" + std::to_string(index) + ".txt";
    std::ofstream save_file(save_path);
    std::shared_ptr<Player> player_to_save = player->GetPlayer();
    std::cout << "\nsaving to " << index << std::endl;

    //WEAPONS
    save_file << "#### WEAPONS ####" << std::endl;
    
    for (auto weapon : player_to_save->GetWeapons()) {
        save_file << weapon->GetName() << std::endl;
        save_file << weapon->GetCurrentAmmo() << std::endl;
        save_file << weapon->GetStoredAmmo() << std::endl;
    }

    //STATS
    save_file << "#### STATS ####" << std::endl;
    save_file << player_to_save->GetHp() << std::endl;
    save_file << player_to_save->GetHunger() << std::endl;
    
    //MAP
    save_file << "#### MAP ####" << std::endl;
    std::string rm = GUIConstants::map_folder_path;
    std::string current_level = level_name_.substr(rm.length(), level_name_.size());
    save_file << current_level << std::endl;
    save_file.close();
}