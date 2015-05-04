#ifndef FILECRAFT_ENTITY_HH
#define FILECRAFT_ENTITY_HH

#include <GL/gl.h>

#include <vector>

class Entity
{
public:
  Entity();
  virtual ~Entity();

  void setX( float x ) { _x = x; }
  void setY( float y ) { _y = y; }
  void setZ( float z ) { _z = z; }

  float x() const { return _x; }
  float y() const { return _y; }
  float z() const { return _z; }

  const std::vector<GLfloat>& vertices() const;
  const std::vector<GLfloat>& normals()  const;
  const std::vector<GLfloat>& colours()  const;

protected:
  std::vector<GLfloat> _vertices;
  std::vector<GLfloat> _normals;
  std::vector<GLfloat> _colours;

private:
  float _x;
  float _y;
  float _z;
};

#endif
