#ifndef FILECRAFT_BOUNDING_BOX_HH
#define FILECRAFT_BOUNDING_BOX_HH

class BoundingBox
{
public:
  BoundingBox( float xMin, float xMax,
               float yMin, float yMax,
               float zMin, float zMax );

  float xMin() const noexcept { return _xMin; }
  float xMax() const noexcept { return _xMax; }
  float yMin() const noexcept { return _yMin; }
  float yMax() const noexcept { return _yMax; }
  float zMin() const noexcept { return _zMin; }
  float zMax() const noexcept { return _zMax; }

  bool intersects( const BoundingBox& other ) const noexcept;

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
