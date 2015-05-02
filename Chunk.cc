#include "Chunk.hh"

#include <array>

namespace
{

std::array<GLfloat,24*3> getSubChunkVertices( GLfloat x, GLfloat y, GLfloat z )
{
  std::array<GLfloat, 24*3> vertices
    = {
       {
        x  , y  , z  ,  // front
        x+1, y  , z  ,
        x+1, y+1, z  ,
        x  , y+1, z  ,
        x+1, y  , z+1,  // right
        x+1, y+1, z+1,
        x+1, y+1, z  ,
        x+1, y  , z  ,
        x  , y  , z  ,  // left
        x  , y  , z+1,
        x  , y+1, z+1,
        x  , y+1, z  ,
        x  , y  , z+1,  // back
        x+1, y  , z+1,
        x+1, y+1, z+1,
        x  , y+1, z+1,
        x+1, y+1, z  ,  // top
        x+1, y+1, z+1,
        x  , y+1, z+1,
        x  , y+1, z  ,
        x+1, y  , z  ,  // bottom
        x  , y  , z,
        x  , y  , z+1,
        x+1, y  , z+1
       }
  };

  return vertices;
}

std::array<GLfloat,24*3> getSubChunkNormals()
{
  std::array<GLfloat, 24*3> normals
    = {
       {
         0, 0,-1,
         0, 0,-1,
         0, 0,-1,
         0, 0,-1,
         1, 0, 0,
         1, 0, 0,
         1, 0, 0,
         1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
         0, 0, 1,
         0, 0, 1,
         0, 0, 1,
         0, 0, 1,
         0, 1, 0,
         0, 1, 0,
         0, 1, 0,
         0, 1, 0,
         0,-1, 0,
         0,-1, 0,
         0,-1, 0,
         0,-1, 0
       }
  };

  return normals;

}

} // end of anonymous namespace

Chunk::Chunk()
{
  for( data_type x = 0; x < xNum; x++ )
    for( data_type y = 0; y < yNum; y++ )
      for( data_type z = 0; z < zNum; z++ )
        _data[x][y][z] = 0;

  // Not exactly random...
  _data[0][0][0] = 1;
  _data[1][0][0] = 1;
  _data[2][0][0] = 1;
  _data[0][0][3] = 2;
  _data[0][1][3] = 2;
  _data[0][2][3] = 2;
  _data[1][0][4] = 3;
  _data[2][0][4] = 3;
  _data[3][0][4] = 3;
}

std::vector<GLfloat> Chunk::vertices() const
{
  std::vector<GLfloat> vertices;
  vertices.reserve( xNum * yNum * zNum * 3 );

  for( data_type x = 0; x < xNum; x++ )
  {
    for( data_type y = 0; y < yNum; y++ )
    {
      for( data_type z = 0; z < zNum; z++ )
      {
        if( _data[x][y][z] != 0 )
        {
          auto subChunkVertices = getSubChunkVertices( x, y, z );

          vertices.insert( std::end( vertices ),
                           std::begin( subChunkVertices ), std::end( subChunkVertices ) );
        }
      }
    }
  }

  return vertices;
}

std::vector<GLfloat> Chunk::normals() const
{
  std::vector<GLfloat> normals;
  normals.reserve( xNum * yNum * zNum * 3 );

  for( data_type x = 0; x < xNum; x++ )
  {
    for( data_type y = 0; y < yNum; y++ )
    {
      for( data_type z = 0; z < zNum; z++ )
      {
        if( _data[x][y][z] != 0 )
        {
          auto subChunkNormals = getSubChunkNormals();

          normals.insert( std::end( normals ),
                          std::begin( subChunkNormals ), std::end( subChunkNormals ) );
        }
      }
    }
  }

  return normals;
}

