#include "BoundingBox.hh"

#include <stdexcept>

BoundingBox::BoundingBox( float xMin, float xMax,
                          float yMin, float yMax,
                          float zMin, float zMax )
  : _xMin( xMin )
  , _xMax( xMax )
  , _yMin( yMin )
  , _yMax( yMax )
  , _zMin( zMin )
  , _zMax( zMax )
{
}

void BoundingBox::checkExtents() const
{
  bool valid =    _xMin <= _xMax
               && _yMin <= _yMax
               && _zMin <= _zMax;

  if( !valid )
    throw std::runtime_error( "Invalid bounding box extents" );
}
