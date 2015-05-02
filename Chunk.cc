#include "Chunk.hh"

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

  _buffer.create();
}

void Chunk::render()
{
  GLuint vertices[] = {
    0, 0, 0,  // front
    1, 0, 0,
    1, 1, 0,
    0, 1, 0,
    1, 0, 0,  // right
    1, 0, 1,
    1, 1, 1,
    1, 1, 0,
    0, 0, 0,  // left
    0, 0, 1,
    0, 1, 1,
    0, 1, 0,
    0, 0, 1,  // back
    1, 0, 1,
    1, 1, 1,
    0, 1, 1,
    1, 1, 0,  // top
    1, 1, 1,
    0, 1, 1,
    0, 1, 0,
    1, 0, 0,  // bottopm
    1, 0, 1,
    0, 0, 1,
    0, 0, 0,
  };

  _buffer.bind();
  _buffer.allocate( vertices, 24 * 3 * sizeof(GLfloat) );

  glDrawArrays( GL_QUADS, 0, 24 );

  _buffer.release();
}
