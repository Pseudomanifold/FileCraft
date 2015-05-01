#ifndef FILECRAFT_CHUNK_HH
#define FILECRAFT_CHUNK_HH

#include <cstdint>

class Chunk
{
public:
  using data_type = std::uint8_t;

  static constexpr data_type xNum = 8;
  static constexpr data_type yNum = 8;
  static constexpr data_type zNum = 8;

  Chunk();

private:
  data_type data[xNum][yNum][zNum];
};

#endif
