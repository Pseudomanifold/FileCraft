#include "Chunk.hh"
#include "SimplexNoise.hh"

#include <array>
#include <cmath>

namespace
{

std::array<GLfloat,24*3> getSubChunkVertices( GLfloat x, GLfloat y, GLfloat z )
{
  std::array<GLfloat, 24*3> vertices
    = {
       {
        x  , y  , z  , // front
        x+1, y  , z  ,
        x+1, y+1, z  ,
        x  , y+1, z  ,
        x,   y  , z-1, // back
        x,   y+1, z-1,
        x+1, y+1, z-1,
        x+1, y  , z-1,
        x,   y  , z  , // left
        x,   y+1, z  ,
        x,   y+1, z-1,
        x,   y  , z-1,
        x+1, y,   z  , // right
        x+1, y,   z-1,
        x+1, y+1, z-1,
        x+1, y+1, z  ,
        x,   y+1, z  , // top
        x+1, y+1, z  ,
        x+1, y+1, z-1,
        x  , y+1, z-1,
        x,   y,   z  , // bottom
        x,   y,   z-1,
        x+1, y,   z-1,
        x+1, y,   z
       }
  };

  return vertices;
}

std::array<GLfloat,24*3> getSubChunkNormals()
{
  std::array<GLfloat, 24*3> normals
    = {
       {
         0, 0, 1, // front
         0, 0, 1,
         0, 0, 1,
         0, 0, 1,
         0, 0,-1, // back
         0, 0,-1,
         0, 0,-1,
         0, 0,-1,
        -1, 0, 0, // left
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
         1, 0, 0, // right
         1, 0, 0,
         1, 0, 0,
         1, 0, 0,
         0, 1, 0, // top
         0, 1, 0,
         0, 1, 0,
         0, 1, 0,
         0,-1, 0, // bottom
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
  {
    for( data_type y = 0; y < yNum; y++ )
    {
      for( data_type z = 0; z < zNum; z++ )
      {
        auto noise     = std::abs( simplexNoise( 1, x,y,z ) );
        data_type type = 0;

        if( noise > 0.75f )
          type = 3;
        else if( noise > 0.40f )
          type = 2;
        else if( noise > 0.30f )
          type = 1;

        _data[x][y][z] = type;
      }
    }
  }
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

std::vector<GLfloat> Chunk::colours() const
{
  std::vector<GLfloat> colours;
  colours.reserve( xNum * yNum * zNum * 3 );

  for( data_type x = 0; x < xNum; x++ )
  {
    for( data_type y = 0; y < yNum; y++ )
    {
      for( data_type z = 0; z < zNum; z++ )
      {
        if( _data[x][y][z] == 0 )
          continue;

        GLfloat r = 1.f;
        GLfloat g = 0.f;
        GLfloat b = 0.f;

        switch( _data[x][y][z] )
        {
        // green terrain
        case 1:
          r = 0.05f; g = 0.50f; b = 0.05f;
          break;

        // brown terrain
        case 2:
          r = 0.75f; g = 0.25f; b = 0.05f;
          break;

        // blue terrain
        case 3:
          r = 0.30f; g = 0.40f; b = 1.00f;
        default:
          break;
        }

        for( unsigned int i = 0; i < 24; i++ )
        {
          colours.push_back( r );
          colours.push_back( g );
          colours.push_back( b );
        }
      }
    }
  }

  return colours;
}
