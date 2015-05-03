#include "TerrainGenerator.hh"

#include "Chunk.hh"

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
  return Chunk();
}
