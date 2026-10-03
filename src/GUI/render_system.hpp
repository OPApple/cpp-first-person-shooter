// render_system.hpp
#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <vector>
#include "entity.hpp"
#include "player.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "gui_constants.hpp"

/**
 * @brief A system responsible for drawing all renderable entities in the game world.
 * * It handles view and projection matrices, updates model matrices, applies simple
 * behavioral effects (like enemy rotation and pickup animation), and manages the
 * OpenGL drawing calls.
 */
class RenderSystem {

public:
    /**
     * @brief Renders all visible entities in the scene.
     * * * Sets up the View and Projection matrices based on the camera.
     * * Iterates through entities, updates their model matrix, sets uniforms in the shader,
     * and calls the mesh's Draw method.
     * @param entities A vector of all entities in the scene to be checked for rendering.
     * @param camera The active camera, providing view and projection transformations.
     * @param player The player entity, used to determine enemy facing direction.
     * @param time The current game time, used for time-dependent visual effects (e.g., animations).
     */
    static void Render(std::vector<std::shared_ptr<Entity>> entities, Camera& camera, std::shared_ptr<PlayerGUI> player, float time);
};

#endif