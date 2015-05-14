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

bool BoundingBox::intersects( const BoundingBox& other ) const noexcept
{
  if( _xMin > other._xMax || other._xMin > _xMax )
    return false;

  if( _yMin > other._yMax || other._yMin > _yMax )
    return false;

  if( _zMin > other._zMax || other._zMin > _zMax )
    return false;

  return true;
}

void BoundingBox::checkExtents() const
{
  bool valid =    _xMin <= _xMax
               && _yMin <= _yMax
               && _zMin <= _zMax;

  if( !valid )
    throw std::runtime_error( "Invalid bounding box extents" );
}
