#pragma once

#include <vector>
#include <Windows.h>

namespace DGE
{
  class TextScreen
  {
  public:
    TextScreen(const size_t& x, const size_t& y);

    void Draw();

    unsigned int SetCharacter(const size_t& x, const size_t& y, const unsigned char& c);

  private:
    size_t x;
    size_t y;
    std::vector<unsigned char> buffer1;
    std::vector<unsigned char> buffer2;
    std::vector<unsigned char>* curBuffer;
    HANDLE hNewScreenBuffer;
  };
}