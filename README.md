# What the hell is this?

Simple project ideas I wrote while being bored during lectures.

Contains:
- conways_game_of_life.c
- driving_school.c (a simple console game)
- perlin_noise.h
- procedural_generation.c
- snake.c
- system_killer.c

# conways_game_of_life.c

A simple, well known program that creates an immersive game field with cells living and dying following four rules:
- Cells with one or less neighbours die
- Cells with two or three neighbours live on
- Cells with four or more neighbours die
- Empty spaces with three neighbours create a new cell

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

# snake.c

A simple snake game in the console. The player has to move a snake around and eat fruit to grow. Base game controls are:
- W/A/S/D for up/left/down/right
- X to quit

Do not crash into yourself!

# system_killer.c

Pretty reliably crashes an operating system when executed using memory allocation and cloning itself.

Similiar to a Fork Bomb, see https://en.wikipedia.org/wiki/Fork_bomb