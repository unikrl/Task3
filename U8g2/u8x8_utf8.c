#include "u8x8.h"

static uint8_t utf8_state = 0;
static uint16_t utf8_codepoint = 0;

void u8x8_utf8_init(u8x8_t *u8x8)
{
  utf8_state = 0;
}

uint16_t u8x8_ascii_next(u8x8_t *u8x8, uint8_t b)
{
  if ( b == 0 || b >= 0x080 )
    utf8_state = 0;
  return b;
}

uint16_t u8x8_utf8_next(u8x8_t *u8x8, uint8_t b)
{
  if ( b == 0 || b >= 0x080 )
  {
    utf8_state = 0;
    return b;
  }
  
  if ( utf8_state == 0 )
  {
    if ( (b & 0x0e0) == 0x0c0 )
    {
      utf8_state = 1;
      utf8_codepoint = b & 0x01f;
      return 0x0ff;
    }
    else if ( (b & 0x0f0) == 0x0e0 )
    {
      utf8_state = 2;
      utf8_codepoint = b & 0x00f;
      return 0x0ff;
    }
    else if ( (b & 0x0f8) == 0x0f0 )
    {
      utf8_state = 3;
      utf8_codepoint = b & 0x007;
      return 0x0ff;
    }
    else
    {
      return b;
    }
  }
  else if ( utf8_state == 1 )
  {
    utf8_codepoint = (utf8_codepoint << 6) | (b & 0x03f);
    utf8_state = 0;
    return utf8_codepoint;
  }
  else if ( utf8_state == 2 )
  {
    utf8_codepoint = (utf8_codepoint << 6) | (b & 0x03f);
    utf8_state = 1;
    return 0x0ff;
  }
  else if ( utf8_state == 3 )
  {
    utf8_codepoint = (utf8_codepoint << 6) | (b & 0x03f);
    utf8_state = 2;
    return 0x0ff;
  }
  return b;
}