#include <wchar.h>
namespace utf8d {
  typedef wchar_t unicode;
  struct Result
  {
    unsigned int errors;
    unsigned int charactors;
    unsigned int read;
  };

  Result utf8_decode(const char *from, unicode *to)
  {
    Result res = { 0,0,0 };
    while (from[res.read])
    {
      if ((from[res.read] & 0xf8) == 0xf0)
      {
        if (from[res.read + 1] && from[res.read + 2] && from[res.read + 3] &&
          (from[res.read + 1] & from[res.read + 2] & from[res.read + 3] & 0xc0) == 0x80)
        {
          if (to)
          {
            unicode unic;
            unic = (unicode)(from[res.read + 0] & 0x07) << 18;
            unic |= (unicode)(from[res.read + 1] & 0x3f) << 12;
            unic |= (unicode)(from[res.read + 2] & 0x3f) << 6;
            unic |= from[res.read + 3] & 0x3f;
            to[res.charactors] = unic;
          }
          res.charactors++;
          res.read += 4;
        }
        else
        {
          res.errors++;
          res.read++;
        }
      }
      else if ((from[res.read] & 0xf0) == 0xe0)
      {
        if (from[res.read + 1] && from[res.read + 2] &&
          (from[res.read + 1] & from[res.read + 2] & 0xc0) == 0x80)
        {
          if (to)
          {
            unicode unic;
            unic = (unicode)(from[res.read + 0] & 0x0f) << 12;
            unic |= (unicode)(from[res.read + 1] & 0x3f) << 6;
            unic |= (from[res.read + 2] & 0x3f);
            to[res.charactors] = unic;
          }
          res.charactors++;
          res.read += 3;
        }
        else
        {
          res.errors++;
          res.read++;
        }
      }
      else if ((from[res.read] & 0xe0) == 0xc0)
      {
        if (from[res.read + 1] &&
          (from[res.read + 1] & 0xc0) == 0x80)
        {
          if (to)
          {
            unicode unic;
            unic = (unicode)(from[res.read + 0] & 0x1f) << 6;
            unic |= from[res.read + 1] & 0x3f;
            to[res.charactors] = unic;
          }
          res.charactors++;
          res.read += 2;
        }
        else
        {
          res.errors++;
          res.read++;
        }
      }
      else if ((from[res.read] & 0x80) == 0)
      {
        if (to)
          to[res.charactors] = from[res.read];
        res.charactors++;
        res.read++;
      }
      else
      {
        res.errors++;
        res.read++;
      }
    }
    return res;
  }

  Result utf8_unicode_charactors(const char *from)
  {
    return utf8_decode(from, 0);
  }
}