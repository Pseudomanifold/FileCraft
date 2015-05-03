#ifndef FILECRAFT_CHUNK_HH
#define FILECRAFT_CHUNK_HH

#include <GL/gl.h>

#include <cstdint>
#include <vector>

class Chunk
{
public:
  using data_type = std::uint8_t;

  static constexpr data_type xNum = 8;
  static constexpr data_type yNum = 8;
  static constexpr data_type zNum = 8;

  Chunk();

  const data_type& operator()( unsigned int x, unsigned int y, unsigned int z ) const;
        data_type& operator()( unsigned int x, unsigned int y, unsigned int z );

  const std::vector<GLfloat>& vertices() const;
  const std::vector<GLfloat>& normals()  const;
  const std::vector<GLfloat>& colours()  const;

private:
  data_type _data[xNum][yNum][zNum];

  bool isOccupied( int x, int y, int z ) const;

  std::vector<GLfloat> getSubChunkVertices( GLfloat x, GLfloat y, GLfloat z ) const;
  std::vector<GLfloat> getSubChunkNormals(  unsigned int x, unsigned int y, unsigned int z ) const;
  std::vector<GLfloat> getSubChunkColours(  unsigned int x, unsigned int y, unsigned int z ) const;

  mutable bool _updateRequired;

  mutable std::vector<GLfloat> _vertices;
  mutable std::vector<GLfloat> _normals;
  mutable std::vector<GLfloat> _colours;

  void update() const;
};

#endif
