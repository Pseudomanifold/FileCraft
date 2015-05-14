#ifndef FILECRAFT_BOUNDING_BOX_HH
#define FILECRAFT_BOUNDING_BOX_HH

class BoundingBox
{
public:
  BoundingBox( float xMin, float xMax,
               float yMin, float yMax,
               float zMin, float zMax );

private:
  void checkExtents() const;

  float _xMin;
  float _xMax;
  float _yMin;
  float _yMax;
  float _zMin;
  float _zMax;
};

#endif
