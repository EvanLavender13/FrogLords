#pragma once

namespace debug {
struct debug_primitive_list;
} // namespace debug

struct game_world;

namespace app {

void generate_debug_primitives(debug::debug_primitive_list& list, const game_world& world);

} // namespace app
