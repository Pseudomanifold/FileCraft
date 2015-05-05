#ifndef FILECRAFT_NAMED_PIPE_HH
#define FILECRAFT_NAMED_PIPE_HH

#include <string>

class NamedPipe
{
public:
  NamedPipe( const std::string& name );
  ~NamedPipe();

private:
  int _fileDescriptor;
};

#endif
