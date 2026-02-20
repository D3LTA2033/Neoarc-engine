cmake_minimum_required(VERSION 3.10)
project(neoarc C)

set(CMAKE_C_STANDARD 99)

set(FRECLI_INC ${CMAKE_CURRENT_SOURCE_DIR}/engine/frecli/include)
set(ENGINE_INC ${CMAKE_CURRENT_SOURCE_DIR}/engine)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${FRECLI_INC}
  ${ENGINE_INC}
  ${ENGINE_INC}/renderer
  ${ENGINE_INC}/input
  ${ENGINE_INC}/physics
  ${ENGINE_INC}/utils
  ${INCLUDE_DIR}
)

add_library(frecli STATIC engine/frecli/src/term.c)
target_include_directories(frecli PUBLIC ${FRECLI_INC})

add_executable(neoarc
  cli/main.c
  cli/commands.c
  cli/menu.c
  engine/renderer/renderer.c
  engine/renderer/sprite.c
  engine/input/input.c
  engine/input/keymap.c
  engine/physics/physics.c
  engine/physics/collision.c
  engine/utils/timer.c
  engine/utils/logger.c
  games/normal/snake/snake.c
  games/normal/tetris/tetris.c
  games/normal/platformer/platformer.c
)

target_include_directories(neoarc PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${FRECLI_INC}
  ${ENGINE_INC}/renderer
  ${ENGINE_INC}/input
  ${ENGINE_INC}/physics
  ${ENGINE_INC}/utils
  ${INCLUDE_DIR}
)

target_link_libraries(neoarc frecli)

install(TARGETS neoarc DESTINATION bin)
