# What the hell is this?

Simple project ideas I wrote while being bored during lectures.

Contains:
- Conways game of life
- driving school (a simple console game)
- perlin noise (Header)
- procedural generation

# conways_game_of_life.c

A simple, well known program that creates an immersive gamefield with cells living and dying following four rules:
- Cells with one or less neighbours die
- Cells with two or three neighbours live on
- Cells with four or more neighbours die
- Empty spaces with three neighbours create a new cell

# driving_school.c

A little console game where you have to dodge obstacles on a road. Base game controls are:
- W/S for up/down
- A/D for left/right
- X to quit

Score points are added once an obstacle has cleared the screen.

# perlin_noise.h

Creates a perlin noise according to a given perlin data. Thanks to https://gist.github.com/nowl/828013

Used by:
- procedural_generation.c

# procedural_generation.c

Creates a procedural pseudo-random landscape using given perlin data. Contains ground, grass, tall grass and trees. User has the ability to:
- Move around the landscape
- Zoom in and out
- Change the level of detail
- Change the seed
