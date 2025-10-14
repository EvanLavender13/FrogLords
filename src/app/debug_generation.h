#pragma once

namespace debug {
struct debug_primitive_list;
} // namespace debug

struct game_world;

namespace gui {
struct character_panel_state;
} // namespace gui

namespace app {

void generate_debug_primitives(debug::debug_primitive_list& list, const game_world& world,
                               const gui::character_panel_state& panel_state);

} // namespace app
