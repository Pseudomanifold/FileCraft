#include "Tetrahedron.hh"

Tetrahedron::Tetrahedron()
{
  _vertices = {
    0.0f, 0.f, 0.0f,
    1.0f, 0.f, 0.0f,
    0.5f, 1.f,-0.5f,
    0.5f, 0.f,-1.0f,
    1.0f, 0.f, 0.0f,
    0.5f, 1.f,-0.5f,
    0.5f, 0.f,-1.0f,
    0.0f, 0.f, 0.0f,
    0.5f, 1.f,-0.5f,
    0.0f, 0.f, 0.0f,
    1.0f, 0.f, 0.0f,
    0.5f, 0.f,-1.0f
  };

  _normals = {
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f
  };

  _colours = {
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f,
    0.f, 0.5f, 0.7f
  };
}
