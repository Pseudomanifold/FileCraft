#include "NamedPipe.hh"

#include <sys/types.h>
#include <sys/stat.h>

#include <stdexcept>

NamedPipe::NamedPipe( const std::string& name )
{
  mknod( name.c_str(), S_IFIFO | 0666, 0 );

  _fileDescriptor = open( name.c_str(),
                          O_RDONLY | O_NDELAY );

  if( _fileDescriptor == -1 )
    throw std::runtime_error( "Unable to create FIFO" );
}

NamedPipe::~NamedPipe()
{
  close( _fileDescriptor );
}
