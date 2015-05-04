#include "Entity.hh"

Entity::~Entity()
{
}

const std::vector<GLfloat>& Entity::vertices() const
{
  return _vertices;
}

const std::vector<GLfloat>& Entity::normals() const
{
  return _normals;
}

const std::vector<GLfloat>& Entity::colours() const
{
  return _colours;
}
