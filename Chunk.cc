#include "Chunk.hh"

#include <array>
#include <stdexcept>

#include <cmath>

Chunk::Chunk()
  : _updateRequired( true )
{
  for( data_type x = 0; x < xNum; x++ )
    for( data_type y = 0; y < yNum; y++ )
      for( data_type z = 0; z < zNum; z++ )
        _data[x][y][z] = 0;
}

const Chunk::data_type& Chunk::operator()( unsigned int x, unsigned int y, unsigned int z ) const
{
  if( x >= xNum || y >= yNum || z >= zNum )
    throw std::runtime_error( "Attempting to access invalid index" );

  return _data[x][y][z];
}

Chunk::data_type& Chunk::operator()( unsigned int x, unsigned int y, unsigned int z )
{
  return const_cast<Chunk::data_type&>( static_cast<const Chunk&>( *this ).operator()( x, y, z ) );
}

const std::vector<GLfloat>& Chunk::vertices() const
{
  if( _updateRequired )
    this->update();

  return _vertices;
}

const std::vector<GLfloat>& Chunk::normals() const
{
  if( _updateRequired )
    this->update();

  return _normals;
}

const std::vector<GLfloat>& Chunk::colours() const
{
  if( _updateRequired )
    this->update();

  return _colours;
}

bool Chunk::isOccupied( int x, int y, int z ) const
{
  if( x < 0 || x >= xNum )
    return false;

  if( y < 0 || y >= yNum )
    return false;

  if( z < 0 || z >= zNum )
    return false;

  return _data[x][y][z] != 0;
}

std::vector<GLfloat> Chunk::getSubChunkVertices( GLfloat x, GLfloat y, GLfloat z ) const
{
  std::vector<GLfloat> vertices;
  vertices.reserve( 24*3 );

  // Front
  if( !isOccupied( x, y, z+1 ) )
  {
    std::array<GLfloat, 12> front = { {
        x  , y  , z  ,
        x+1, y  , z  ,
        x+1, y+1, z  ,
        x  , y+1, z
    } };

    vertices.insert( vertices.end(), front.begin(), front.end() );
  }

  // Back
  if( !isOccupied( x, y, z-1 ) )
  {
    std::array<GLfloat, 12> back = { {
        x,   y  , z-1,
        x,   y+1, z-1,
        x+1, y+1, z-1,
        x+1, y  , z-1
    } };

    vertices.insert( vertices.end(), back.begin(), back.end() );
  }

  // Left
  if( !isOccupied( x-1, y, z ) )
  {
    std::array<GLfloat, 12> left = { {
        x,   y  , z  ,
        x,   y+1, z  ,
        x,   y+1, z-1,
        x,   y  , z-1
    } };

    vertices.insert( vertices.end(), left.begin(), left.end() );
  }

  // Right
  if( !isOccupied( x+1, y, z ) )
  {
    std::array<GLfloat, 12> right = { {
        x+1, y,   z  ,
        x+1, y,   z-1,
        x+1, y+1, z-1,
        x+1, y+1, z
    } };

    vertices.insert( vertices.end(), right.begin(), right.end() );
  }

  // Top
  if( !isOccupied( x, y+1, z ) )
  {
    std::array<GLfloat, 12> top = { {
        x,   y+1, z  ,
        x+1, y+1, z  ,
        x+1, y+1, z-1,
        x  , y+1, z-1
    } };

    vertices.insert( vertices.end(), top.begin(), top.end() );
  }

  // Bottom
  if( !isOccupied( x, y-1, z ) )
  {
    std::array<GLfloat, 12> bottom = { {
        x,   y,   z  , // bottom
        x,   y,   z-1,
        x+1, y,   z-1,
        x+1, y,   z
    } };

    vertices.insert( vertices.end(), bottom.begin(), bottom.end() );
  }

  return vertices;
}

std::vector<GLfloat> Chunk::getSubChunkNormals( unsigned int x, unsigned int y, unsigned int z ) const
{
  std::vector<GLfloat> normals;
  normals.reserve( 24*3 );

  // Front
  if( !isOccupied( x, y, z+1 ) )
  {
    std::array<GLfloat,12> front = { {
         0, 0, 1,
         0, 0, 1,
         0, 0, 1,
         0, 0, 1
    } };

    normals.insert( normals.end(), front.begin(), front.end() );
  }

  // Back
  if( !isOccupied( x, y, z-1 ) )
  {
    std::array<GLfloat,12> back = { {
         0, 0, -1,
         0, 0, -1,
         0, 0, -1,
         0, 0, -1
    } };

    normals.insert( normals.end(), back.begin(), back.end() );
  }

  // Left
  if( !isOccupied( x-1, y, z ) )
  {
    std::array<GLfloat,12> left = { {
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0
    } };

    normals.insert( normals.end(), left.begin(), left.end() );
  }

  // Right
  if( !isOccupied( x+1, y, z ) )
  {
    std::array<GLfloat,12> right = { {
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0
    } };

    normals.insert( normals.end(), right.begin(), right.end() );
  }

  // Top
  if( !isOccupied( x, y+1, z ) )
  {
    std::array<GLfloat,12> top = { {
         0, 1, 0,
         0, 1, 0,
         0, 1, 0,
         0, 1, 0
    } };

    normals.insert( normals.end(), top.begin(), top.end() );
  }

  // Bottom
  if( !isOccupied( x, y-1, z ) )
  {
    std::array<GLfloat,12> bottom = { {
         0, 1, 0,
         0, 1, 0,
         0, 1, 0,
         0, 1, 0
    } };

    normals.insert( normals.end(), bottom.begin(), bottom.end() );
  }

  return normals;
}

std::vector<GLfloat> Chunk::getSubChunkColours( unsigned int x, unsigned int y, unsigned int z ) const
{
  std::vector<GLfloat> colours;
  colours.reserve( 24*3 );

  GLfloat r = 0.f;
  GLfloat g = 0.f;
  GLfloat b = 0.f;

  switch( this->operator()(x,y,z) )
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

  unsigned int numEmptyFaces =   !isOccupied(x,   y  , z-1)
                               + !isOccupied(x,   y  , z+1)
                               + !isOccupied(x-1, y  , z  )
                               + !isOccupied(x+1, y  , z  )
                               + !isOccupied(x  , y+1, z  )
                               + !isOccupied(x  , y-1, z  );

  for( unsigned int i = 0; i < numEmptyFaces*4; i++ )
  {
    colours.push_back(r);
    colours.push_back(g);
    colours.push_back(b);
  }

  return colours;
}


void Chunk::update() const
{
  _vertices.clear();
  _vertices.reserve( xNum * yNum * zNum * 3 );

  _normals.clear();
  _normals.reserve( xNum * yNum * zNum * 3 );

  _colours.clear();
  _colours.reserve( xNum * yNum * zNum * 3 );

  for( data_type x = 0; x < xNum; x++ )
  {
    for( data_type y = 0; y < yNum; y++ )
    {
      for( data_type z = 0; z < zNum; z++ )
      {
        if( _data[x][y][z] != 0 )
        {
          auto&& subChunkVertices = getSubChunkVertices( x, y, z );

          _vertices.insert( std::end( _vertices ),
                            std::begin( subChunkVertices ), std::end( subChunkVertices ) );

          auto&& subChunkNormals = getSubChunkNormals( x, y, z );

          _normals.insert( std::end( _normals ),
                           std::begin( subChunkNormals ), std::end( subChunkNormals ) );

          auto&& subChunkColours = getSubChunkColours( x, y, z );

          _colours.insert( std::end( _colours ),
                           std::begin( subChunkColours ), std::end( subChunkColours ) );
        }
      }
    }
  }

  _updateRequired = false;
}
