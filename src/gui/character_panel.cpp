#include "gui/character_panel.h"
#include "gui/gui.h"
#include "sokol_app.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace gui {

std::vector<parameter_command> draw_character_panel(const character_panel_state& state,
                                                    const controller& character,
                                                    const character_reactive_systems& visuals,
                                                    const character::tuning_params& params) {
    std::vector<parameter_command> commands;

    if (!state.show)
        return commands;

    // Draw as collapsing sections (no window wrapper)
    // Main tuning parameters
    if (ImGui::CollapsingHeader("Character Tuning", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Local copies for slider interaction (GUI needs mutable values)
        float max_speed = params.max_speed;
        float accel = params.accel;
        float jump_height = params.jump_height;
        float gravity = params.gravity;
        float coyote_window = character.coyote_window;
        float jump_buffer_window = character.jump_buffer_window;

        // Metadata-driven tunable parameters
        if (gui::widget::tunable_param(&max_speed, character::tuning_params::max_speed_meta)) {
            commands.push_back({parameter_type::MAX_SPEED, max_speed});
        }
        if (gui::widget::tunable_param(&accel, character::tuning_params::accel_meta)) {
            commands.push_back({parameter_type::ACCEL, accel});
        }
        if (gui::widget::tunable_param(&jump_height, character::tuning_params::jump_height_meta)) {
            commands.push_back({parameter_type::JUMP_HEIGHT, jump_height});
        }
        if (gui::widget::tunable_param(&gravity, character::tuning_params::gravity_meta)) {
            commands.push_back({parameter_type::GRAVITY, gravity});
        }

        // Legacy sliders (no metadata yet)
        if (gui::widget::slider_float("Coyote Window (s)", &coyote_window, 0.0f, 0.5f)) {
            commands.push_back({parameter_type::COYOTE_WINDOW, coyote_window});
        }
        if (gui::widget::slider_float("Jump Buffer Window (s)", &jump_buffer_window, 0.0f, 0.5f)) {
            commands.push_back({parameter_type::JUMP_BUFFER_WINDOW, jump_buffer_window});
        }

        // Derived parameter: jump velocity (calculated from jump_height and gravity)
        static constexpr param_meta jump_velocity_meta = {
            "Jump Velocity", "m/s", 0.0f, 10.0f, param_type::DERIVED
        };
        gui::widget::derived_param("Jump Velocity", character.jump_velocity, jump_velocity_meta,
                                    "√(2·|g|·h)");

        // Real-time feedback: horizontal speed plot
        float horizontal_speed = glm::length(glm::vec3(character.velocity.x, 0.0f, character.velocity.z));
        gui::plot_histogram("Horizontal Speed (m/s)", horizontal_speed, 5.0f, 0.0f, params.max_speed * 1.2f);

        // Debug displays for jump timing forgiveness
        gui::widget::text("Coyote Timer: %.3f s", character.coyote_timer);
        gui::widget::text("Jump Buffer Timer: %.3f s", character.jump_buffer_timer);
        gui::widget::text(
            "Can Jump (Coyote): %s",
            (character.is_grounded || character.coyote_timer < character.coyote_window) ? "YES"
                                                                                        : "NO");
    }

    // Landing spring parameters
    if (ImGui::CollapsingHeader("Landing Spring")) {
        // Local copies for slider interaction
        float landing_stiffness = visuals.animation.landing_spring.stiffness;
        float landing_damping = visuals.animation.landing_spring.damping;
        float landing_impulse_scale = visuals.animation.landing_impulse_scale;

        if (gui::widget::slider_float("Stiffness", &landing_stiffness, 100.0f, 1000.0f)) {
            commands.push_back({parameter_type::LANDING_STIFFNESS, landing_stiffness});
        }
        if (gui::widget::slider_float("Damping", &landing_damping, 10.0f, 100.0f)) {
            commands.push_back({parameter_type::LANDING_DAMPING, landing_damping});
        }
        if (gui::widget::slider_float("Impulse Scale", &landing_impulse_scale, 0.1f, 1.5f)) {
            commands.push_back({parameter_type::LANDING_IMPULSE_SCALE, landing_impulse_scale});
        }

        // Read-only spring state display
        gui::widget::text("Spring Position: %.3f", visuals.animation.get_vertical_offset());
        gui::widget::text("Spring Velocity: %.3f", visuals.animation.landing_spring.get_velocity());
    }

    // Orientation parameters
    if (ImGui::CollapsingHeader("Orientation")) {
        // Read-only for now (no parameter commands needed for orientation spring)
        // These were being modified directly, but not part of the initial violation scope
        gui::widget::text("Spring Stiffness: %.2f", visuals.orientation.yaw_spring.stiffness);
        gui::widget::text("Spring Damping: %.2f", visuals.orientation.yaw_spring.damping);

        // Read-only orientation state display
        gui::widget::text("Current Yaw (rad): %.2f", visuals.orientation.get_yaw());
        gui::widget::text("Angular Velocity (rad/s): %.2f",
                          visuals.orientation.yaw_spring.get_velocity());
    }

    // Locomotion State (speed tiers + phase for cyclic motion)
    if (ImGui::CollapsingHeader("Locomotion State")) {
        // State name
        const char* state_name;
        switch (character.locomotion.state) {
        case controller::locomotion_speed_state::WALK:
            state_name = "WALK";
            break;
        case controller::locomotion_speed_state::RUN:
            state_name = "RUN";
            break;
        case controller::locomotion_speed_state::SPRINT:
            state_name = "SPRINT";
            break;
        default:
            state_name = "UNKNOWN";
            break;
        }
        gui::widget::text("State: %s", state_name);

        // Horizontal speed
        float speed = glm::length(glm::vec3(character.velocity.x, 0.0f, character.velocity.z));
        gui::widget::text("Speed: %.2f m/s", speed);

        // Phase (0-1)
        gui::widget::text("Phase: %.3f", character.locomotion.phase);

        // Cycle length (from locomotion_state output)
        gui::widget::text("Cycle Length: %.2f m", character.locomotion.cycle_length);

        // Distance traveled (internal state on controller)
        gui::widget::text("Distance Traveled: %.2f m", character.distance_traveled);
    }

    return commands;
}

} // namespace gui
