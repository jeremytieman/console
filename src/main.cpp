#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE hNewScreenBuffer = CreateConsoleScreenBuffer(
    GENERIC_READ |           // read/write access
    GENERIC_WRITE,
    FILE_SHARE_READ |
    FILE_SHARE_WRITE,        // shared
    NULL,                    // default security attributes
    CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
    NULL);                   // reserved; must be NULL

  if (hStdout == INVALID_HANDLE_VALUE ||
    hNewScreenBuffer == INVALID_HANDLE_VALUE)
  {
    cout << "CreateConsoleScreenBuffer failed - " << GetLastError() << endl;
    return 1;
  }

  if (!SetConsoleActiveScreenBuffer(hNewScreenBuffer))
  {
    cout << "SetConsoleActiveScreenBuffer failed - " << GetLastError() << endl;
    return 1;
  }

  // Set the source rectangle. 
  SMALL_RECT srctReadRect;
  srctReadRect.Top = 0;    // top left: row 0, col 0 
  srctReadRect.Left = 0;
  srctReadRect.Bottom = 29; // bot. right: row 1, col 79 
  srctReadRect.Right = 79;

  // The temporary buffer size is 2 rows x 80 columns. 
  COORD coordBufSize;
  coordBufSize.Y = 30;
  coordBufSize.X = 80;

  // The top left destination cell of the temporary buffer is 
  // row 0, col 0. 
  COORD coordBufCoord;
  coordBufCoord.X = 0;
  coordBufCoord.Y = 0;

  // Copy the block from the screen buffer to the temp. buffer. 
  CHAR_INFO chiBuffer[30 * 80];
  BOOL fSuccess = ReadConsoleOutput(
    hStdout,        // screen buffer to read from 
    chiBuffer,      // buffer to copy into 
    coordBufSize,   // col-row size of chiBuffer 
    coordBufCoord,  // top left dest. cell in chiBuffer 
    &srctReadRect); // screen buffer source rectangle 

  if (!fSuccess)
  {
    printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
    return 1;
  }

  // Set the destination rectangle. 
  SMALL_RECT srctWriteRect;
  srctWriteRect.Top = 0;    // top lt: row 10, col 0 
  srctWriteRect.Left = 0;
  srctWriteRect.Bottom = 29; // bot. rt: row 11, col 79 
  srctWriteRect.Right = 79;

  // Copy from the temporary buffer to the new screen buffer. 
  chiBuffer[(29 * 80) + 1].Char.AsciiChar = '>';
  fSuccess = WriteConsoleOutput(
    hNewScreenBuffer, // screen buffer to write to 
    chiBuffer,        // buffer to copy from 
    coordBufSize,     // col-row size of chiBuffer 
    coordBufCoord,    // top left src cell in chiBuffer 
    &srctWriteRect);  // dest. screen buffer rectangle 

  if (!fSuccess)
  {
    printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
    return 1;
  }

  COORD cursorPos;
  cursorPos.X = 3;
  cursorPos.Y = 29;
  SetConsoleCursorPosition(hNewScreenBuffer, cursorPos);

  Sleep(5000);

  // Restore the original active screen buffer. 

  if (!SetConsoleActiveScreenBuffer(hStdout))
  {
    printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
    return 1;
  }

  return 0;
}