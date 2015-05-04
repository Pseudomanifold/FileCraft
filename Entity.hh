#ifndef FILECRAFT_ENTITY_HH
#define FILECRAFT_ENTITY_HH

#include <GL/gl.h>

#include <vector>

class Entity
{
public:
  virtual ~Entity();

  const std::vector<GLfloat>& vertices() const;
  const std::vector<GLfloat>& normals()  const;
  const std::vector<GLfloat>& colours()  const;

protected:
  std::vector<GLfloat> _vertices;
  std::vector<GLfloat> _normals;
  std::vector<GLfloat> _colours;
};

#endif
