# What the hell is this?

Simple project ideas I wrote while being bored during lectures.

Contains:
- ant_colony.c
- conways_game_of_life.c
- dancing_lights.c
- driving_school.c (a simple console game)
- perlin_noise.h
- procedural_generation.c
- sinus_wave.c
- snake.c
- system_killer.c

# ant_colony.c

Inspired by Langton's Ant, simulates a small ant colony following simple rules for each ant:
- Each ant moves one step per turn
- If it lands on a ground field (ICON_GND), it will turn right and turn this field to a small tunnel field (ICON_TURN), as if doing pioneer work
- If it lands on a small tunnel field (ICON_TURN), it will turn left and turn this field to a large tunnel field (ICON_STRAIGHT), as if building on the foundations
- If it lands on a large tunnel field (ICON_STRAIGHT), it will go straight and turn this field to a ground field (ICON_GND), as if giving back to the earth
- If it lands on an air field (ICON_AIR), it will turn around and turn this field to a ground field (ICON_GND), as if building an ant hill

# conways_game_of_life.c

A simple, well known program that creates an immersive game field with cells living and dying following four rules:
- Cells with one or less neighbours die
- Cells with two or three neighbours live on
- Cells with four or more neighbours die
- Empty spaces with three neighbours create a new cell

# dancing_lights.c

Creates a nice visual effect using a cellular automaton following these rules:
- For each cell, the average amount from all neighbors is calculated
- The value calculated will run through a changer_function()

# driving_school.c

A little console game where you have to dodge obstacles on a road. Base game controls are:
- W/S for up/down
- A/D for left/right
- X to quit

Score points are added once an obstacle has cleared the screen. Allows for game customizations within the code via macros.

# perlin_noise.h

Creates a perlin noise according to a given perlin data and coordinates. To use, call the function perlin2d(...) for each coordinate of your game field. Returns a value between 0 and 1.

Thanks to https://gist.github.com/nowl/828013

Used by:
- procedural_generation.c

# procedural_generation.c

Creates a procedural pseudo-random landscape using given perlin data. Contains ground, grass, tall grass and trees. User has the ability to:
- Move around the landscape
- Zoom in and out
- Change the level of detail
- Change the seed

# sinus_wave.c

Produces a customizable sinus (or tangens) wave and prints it to stdout.

# snake.c

A simple snake game in the console. The player has to move a snake around and eat fruit to grow. Base game controls are:
- W/A/S/D for up/left/down/right
- X to quit

Do not crash into yourself!

# system_killer.c

Pretty reliably crashes an operating system when executed using memory allocation and cloning itself.

Acts similiar to a Fork Bomb, for more on that see https://en.wikipedia.org/wiki/Fork_bomb
