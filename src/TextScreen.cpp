#include "..\inc\TextScreen.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <Windows.h>

namespace DGE
{
  TextScreen::TextScreen(const size_t& x, const size_t& y) : x(x), y(y), buffer1(x * y), buffer2(x * y), curBuffer(&buffer1)
  {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hNewScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL,
      CONSOLE_TEXTMODE_BUFFER,
      NULL);

    if (hStdout == INVALID_HANDLE_VALUE ||
      hNewScreenBuffer == INVALID_HANDLE_VALUE)
    {
      std::stringstream message;
      message << "CreateConsoleScreenBuffer failed - " << GetLastError() << std::endl;
      throw std::exception(message.str().c_str());
    }

    if (!SetConsoleActiveScreenBuffer(hNewScreenBuffer))
    {
      std::stringstream message;
      message << "SetConsoleActiveScreenBuffer failed - " << GetLastError() << std::endl;
      throw std::exception(message.str().c_str());
    }
  }

  void TextScreen::Draw()
  {
    const size_t TOTAL_SIZE = x * y;
    auto chiBuffer = std::make_unique<CHAR_INFO[]>(TOTAL_SIZE);

    for (size_t i = 0; i < TOTAL_SIZE; ++i)
    {
      chiBuffer[i].Attributes = 0;
      chiBuffer[i].Char.AsciiChar = curBuffer->at(i);
    }

    COORD coordBufSize;
    coordBufSize.Y = 30;
    coordBufSize.X = 80;

    COORD coordBufCoord;
    coordBufCoord.X = 0;
    coordBufCoord.Y = 0;

    SMALL_RECT srctWriteRect;
    srctWriteRect.Top = 0;
    srctWriteRect.Left = 0;
    srctWriteRect.Bottom = 29;
    srctWriteRect.Right = 79;
    
    BOOL fSuccess = WriteConsoleOutput(
      hNewScreenBuffer,
      chiBuffer.get(),
      coordBufSize,     // col-row size of chiBuffer 
      coordBufCoord,    // top left src cell in chiBuffer 
      &srctWriteRect);  // dest. screen buffer rectangle 

    if (!fSuccess)
    {
      std::stringstream message;
      message << "WriteConsoleOutput failed - " << GetLastError() << std::endl;
      throw std::exception(message.str().c_str());
    }
  }

  unsigned int TextScreen::SetCharacter(const size_t& x, const size_t& y, const unsigned char& c)
  {
    if (x > this->x) return -1;
    if (y > this->y) return -1;

    return 0;
  }
}