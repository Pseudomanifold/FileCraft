#include "TerrainGenerator.hh"

#include "Chunk.hh"
#include "SimplexNoise.hh"

#include <random>

namespace
{

static std::random_device rd;

} // end of anonymous namespace

Chunk makePeak()
{
  Chunk peak;

  auto xDim = peak.xNum;
  auto yDim = peak.yNum;
  auto zDim = peak.zNum;

  // Assumption: Everything is a power of two. This makes calculating the
  // coordinates of the peak easier.
  for( decltype(yDim) y = 0; y < yDim; y++ )
    for( decltype(xDim) x = y; x < xDim - y; x++ )
      for( decltype(zDim) z = y; z < zDim - y; z++ )
        peak(x,y,z) = (y % 3)+1;

  return peak;
}

Chunk makePeaks( unsigned int numPeaks )
{
  // FIXME: Not yet implemented :-)
  return Chunk();
}

Chunk makeRandom()
{
  std::mt19937 rng( rd() );
  std::uniform_int_distribution<unsigned int> distribution( 0, 3);

  Chunk chunk;

  auto xDim = chunk.xNum;
  auto yDim = chunk.yNum;
  auto zDim = chunk.zNum;

  for( decltype(xDim) x = 0; x < xDim; x++ )
    for( decltype(yDim) y = 0; y < yDim; y++ )
      for( decltype(zDim) z = y; z < zDim; z++ )
        chunk(x,y,z) = distribution( rng );

  return chunk;
}

Chunk makeRandomWithSimplexNoise()
{
  Chunk chunk;

  auto xDim = chunk.xNum;
  auto yDim = chunk.yNum;
  auto zDim = chunk.zNum;

  for( decltype(xDim) x = 0; x < xDim; x++ )
    for( decltype(yDim) y = 0; y < yDim; y++ )
      for( decltype(zDim) z = y; z < zDim; z++ )
        chunk(x,y,z) = simplexNoise( 1, x,y,z ) > 0.3 ? 1 : 0;

  std::mt19937 rng( rd() );
  std::uniform_int_distribution<unsigned int> distribution( 1, 3);

  for( decltype(xDim) x = 0; x < xDim; x++ )
    for( decltype(yDim) y = 0; y < yDim; y++ )
      for( decltype(zDim) z = y; z < zDim; z++ )
        if( chunk(x,y,z) != 0 )
          chunk(x,y,z) = distribution( rng );

  return chunk;
}
