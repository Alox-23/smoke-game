#ifndef CHUNK_CHUNK_H
#define CHUNK_CHUNK_H

/*
This file makes chunking possible for our game engine

Each Chunk is an entity in the ECS and has the folowing components:
	- Sprite (so that it can be renderd on the screen)
	- Position (so that it can be rendered too)
	- Velocity (it has to move left when the player moves right)
	- Is_Scrollable (if set, then the sprite must move in the oposite direction of the player)
	- ChunkData (a struct that holds the tile date etc.)

A chunk can be loaded from a tilemap at a certain chunk position.
When chunks are loaded, they are all assigned a certain chunk position,
which affects the Position component of the chunk.

The ChunkManager, handles the source of data from which chunks are loaded from.
It also renders the chunks Sprite component based on individual tiles in a tilemap.

Implementation Order:
	- The single Chunk:
		- Create the ChunkData structure
		- Create the individual Chunk Components in the ECS
		- Define a function that loads a texture to said chunk entity.
		- Declare said function obv :)
		- Create the scrollable system
		- Test that the chunk moves into oposite direction of player entity.
	- The TileMap:
		- Too be continued...
*/

#endif
