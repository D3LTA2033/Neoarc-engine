CC = gcc
CFLAGS = -Wall -Wextra -I. -I engine/frecli/include -I engine/renderer -I engine/input -I engine/physics -I engine/utils -I include
LDFLAGS =
TARGET = build/neoarc

FRECLI_SRC = engine/frecli/src/term.c
FRECLI_OBJ = build/frecli_term.o
FRECLI_LIB = engine/frecli/lib/libfrecli.a

ENGINE_OBJS = build/renderer.o build/sprite.o build/input.o build/keymap.o build/physics.o build/collision.o build/timer.o build/logger.o
GAME_OBJS = build/snake.o build/tetris.o build/platformer.o
CLI_OBJS = build/main.o build/commands.o build/menu.o build/config.o build/box.o

all: $(TARGET)

$(TARGET): $(FRECLI_LIB) $(ENGINE_OBJS) $(GAME_OBJS) $(CLI_OBJS)
	$(CC) $(ENGINE_OBJS) $(GAME_OBJS) $(CLI_OBJS) -L engine/frecli/lib -lfrecli -o $(TARGET) $(LDFLAGS)

engine/frecli/lib/libfrecli.a: $(FRECLI_OBJ)
	@mkdir -p engine/frecli/lib
	ar rcs $@ $(FRECLI_OBJ)
	ranlib $@

build/frecli_term.o: engine/frecli/src/term.c
	@mkdir -p build
	$(CC) $(CFLAGS) -I engine/frecli/include -c $< -o $@

build/renderer.o: engine/renderer/renderer.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/sprite.o: engine/renderer/sprite.c
	$(CC) $(CFLAGS) -c $< -o $@

build/input.o: engine/input/input.c
	$(CC) $(CFLAGS) -c $< -o $@

build/keymap.o: engine/input/keymap.c
	$(CC) $(CFLAGS) -c $< -o $@

build/physics.o: engine/physics/physics.c
	$(CC) $(CFLAGS) -c $< -o $@

build/collision.o: engine/physics/collision.c
	$(CC) $(CFLAGS) -c $< -o $@

build/timer.o: engine/utils/timer.c
	$(CC) $(CFLAGS) -c $< -o $@

build/logger.o: engine/utils/logger.c
	$(CC) $(CFLAGS) -c $< -o $@

build/snake.o: games/normal/snake/snake.c
	$(CC) $(CFLAGS) -I engine/renderer -I engine/input -I engine/frecli/include -I engine/utils -c $< -o $@

build/tetris.o: games/normal/tetris/tetris.c
	$(CC) $(CFLAGS) -I engine/renderer -I engine/input -c $< -o $@

build/platformer.o: games/normal/platformer/platformer.c
	$(CC) $(CFLAGS) -I engine/renderer -I engine/input -c $< -o $@

build/main.o: cli/main.c
	$(CC) $(CFLAGS) -c $< -o $@

build/commands.o: cli/commands.c
	$(CC) $(CFLAGS) -c $< -o $@

build/menu.o: cli/menu.c
	$(CC) $(CFLAGS) -I include -c $< -o $@

build/config.o: cli/config.c
	$(CC) $(CFLAGS) -c $< -o $@

build/box.o: cli/box.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(FRECLI_OBJ) $(ENGINE_OBJS) $(GAME_OBJS) $(CLI_OBJS) $(TARGET) $(FRECLI_LIB)

.PHONY: all clean
