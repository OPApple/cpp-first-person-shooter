#include "sound_handler.hpp"


SoundHandler::SoundHandler() {
    std::vector<std::string> gunshot_paths;
    std::string weapon_sound_path = "resources/audio/weapons/";
    std::string footstep_path = "resources/audio/footsteps/";
    std::string menu_path = "resources/audio/menu/";
    
    for (const auto & entry : std::filesystem::directory_iterator(weapon_sound_path)) {
        gunshot_paths.push_back(entry.path().string());
    }
    
    for (auto path : gunshot_paths) {
        std::string name = path.substr(weapon_sound_path.length(), path.size());
        name.erase(name.length() - 4); // remove file extension .png
        sf::SoundBuffer sound_buffer;
        if (!sound_buffer.loadFromFile(path)) {
            std::cout << "File for player gunshot missing, using default..." << std::endl;
            sound_buffer.loadFromFile("resources/audio/weapons/gunshot.wav");
            gunshot_buffers.insert(std::make_pair(name, std::make_shared<sf::SoundBuffer>(sound_buffer)));
        } else {
            gunshot_buffers.insert(std::make_pair(name, std::make_shared<sf::SoundBuffer>(sound_buffer)));
        }
    }
    
    player_gunshot = sf::Sound(*gunshot_buffers.find("gunshot")->second);

    if(!player_steps_buffer.loadFromFile(footstep_path.append("player_footsteps.wav"))) {
        std::cout << "File for player footsteps missing, using default..." << std::endl;
    }

    player_step = sf::Sound(player_steps_buffer);

    if(!enter_buffer.loadFromFile(menu_path + "enter.wav")){
        std::cout << "Enter missing" << std::endl;
    }

    enter = sf::Sound(enter_buffer);

    if (!bgm.openFromFile("resources/audio/bgm/Totally Boneless!!!.mp3")) {
        std::cout << "bgm missing" << std::endl;
    }
    
    bgm.setVolume(25);

}

void SoundHandler::PlayerShoot() {
    player_gunshot.play();
}

void SoundHandler::SetPlayerShoot(std::string weapon_name) {
    auto gunshot_it = gunshot_buffers.find(weapon_name);
    if (gunshot_it != gunshot_buffers.end()) {
        player_gunshot = sf::Sound(*gunshot_it->second);
    } else {
        auto default_it = gunshot_buffers.find("gunshot");
        player_gunshot = sf::Sound(*default_it->second);
    }
}

void SoundHandler::PlayerStep(float delta_time) {
    player_step_timer += delta_time;
    if (player_step_timer >= 0.9f) {
        player_step.play();
        player_step_timer = 0.0f;
    }
}

void SoundHandler::Enter() {
    enter.play();
}

void SoundHandler::PlayBGM() {
    bgm.play();
}

bool SoundHandler::ChangeBGM(std::string name) {
    if (bgm_name==name) return false;
    std::string path = "resources/audio/bgm/" + name + ".mp3";
    
    if (!bgm.openFromFile(path)) {
        return false;
    } else {
        bgm_name = name;
        return true;
    }
}
