#ifndef FILECRAFT_TERRAIN_GENERATOR_HH
#define FILECRAFT_TERRAIN_GENERATOR_HH

class Chunk;

Chunk makePeak();
Chunk makePeaks( unsigned int numPeaks = 0 );

Chunk makeRandom();
Chunk makeRandomWithSimplexNoise( float xStart = 0.f, float yStart = 0.f, float zStart = 0.f );

#endif
