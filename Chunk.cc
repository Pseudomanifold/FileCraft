#include "Chunk.hh"

Chunk::Chunk()
{
  for( data_type x = 0; x < xNum; x++ )
    for( data_type y = 0; y < yNum; y++ )
      for( data_type z = 0; z < zNum; z++ )
        data[x][y][z] = 0;

  // Not exactly random...
  data[0][0][0] = 1;
  data[1][0][0] = 1;
  data[2][0][0] = 1;
  data[0][0][3] = 2;
  data[0][1][3] = 2;
  data[0][2][3] = 2;
  data[1][0][4] = 3;
  data[2][0][4] = 3;
  data[3][0][4] = 3;
}
