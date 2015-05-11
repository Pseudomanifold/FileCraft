#include "NamedPipe.hh"

#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <array>
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

std::string NamedPipe::read()
{
  std::array<char, 512> buffer;
  std::string data;

  ssize_t numBytesRead = 0;

  do
  {
    numBytesRead = ::read( _fileDescriptor,
                           reinterpret_cast<void*>( buffer.data() ),
                           buffer.size() );

    if( numBytesRead > 0 )
    {
      data += std::string( buffer.begin(),
                           buffer.begin() + numBytesRead );
    }
  }
  while( numBytesRead > 0 );

  return data;
}
