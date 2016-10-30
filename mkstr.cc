#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <functional>
#include <vector>
#include "bincc.h"
#include "png.h"
#include <stdio.h>
//#include <codecvt>
#include "utf8d.h"

int main(int argc,char **argv){
  using namespace std;
  FT_Library library;
  FT_Face    face;
  FT_GlyphSlot slot;
  FT_UInt glyph_index;
  FT_Error error;

  const char *text = "/w600/h100/s50/c888/bFFF/This is /cF00/test /c888/string.";
  if (2<=argc) {
    text = argv[1];
  }


  FT_Init_FreeType( &library );
  FT_New_Memory_Face(library, static_cast<const uint8_t*>(bincc_data::font_ttf), bincc_data::font_ttf_size, 0, &face);
  slot = face->glyph;
  int w = 600;
  int h = 200;
  int s = 60;
  int u = s/16;
  int xpos = u*2;
  struct RGBA {
    float r,g,b,a;
  };
  RGBA c = { 0.6f,0.6f,0.6f };
  FT_Set_Char_Size( face, 0, 16 * s, 300, 300);

  vector<uint8_t> rgb;
  function<void(int, int,float)> put = [&](int x, int y,float d) {
    if (0 <= x && 0 <= y&&x < w&&y < h) {
      uint8_t *u = &*rgb.begin() + (y*w + x) * 3;
      u[0] = (u[0] * (1-d)) + (c.r*d) * 0xff;
      u[1] = (u[1] * (1-d)) + (c.g*d) * 0xff;
      u[2] = (u[2] * (1-d)) + (c.b*d) * 0xff;
    }
  };
#if 0
  wstring_convert<codecvt_utf8_utf16<wchar_t>> convert;
  wstring str = convert.from_bytes(text);
#else
  wstring str;
  str.resize(utf8d::utf8_unicode_charactors(text).charactors);
  utf8d::utf8_decode(text, &*str.begin());
#endif
  vector<function<void()>> f;
  for (int n = 0;n < str.length(); n++ ){
    if (L'/' == str[n]) {
      auto e = str.find(L'/', n+1);
      if (string::npos != e) {
        const std::wstring x = wstring(str.begin() + n + 1, str.begin() + e);
        bool hit = true;
        if (0 == x.size()) {

        }
        else if (L'w' == x[0]) {
          int p = stoi(x.c_str()+1);
          f.push_back([&, p] {
            w = p;
            rgb.resize(w*h * 3, 0);
          });
        }
        else if (L'h' ==x[0]) {
          int p = stoi(x.c_str() + 1);
          f.push_back([&, p] {
            h = p;
            rgb.resize(w*h * 3, 0);
          });
        }
        else if (L's' == x[0]) {
          int p = stoi(x.c_str() + 1);
          f.push_back([&, p] {
            s = p;
            FT_Set_Char_Size(face, 0, 16 * s, 300, 300);
          });
        }
        else if (L'c' == x[0]) {
          uint32_t p = stoi(x.c_str() + 1,nullptr,16);
          f.push_back([&, p] {
            c.r = ((p & 0xf00) >> 8) / 15.0f;
            c.g = ((p & 0xf0) >> 4) / 15.0f;
            c.b = (p & 0xf) / 15.0f;
          });
        }
        else if (L'b' == x[0]) {
          uint32_t p = stoi(x.c_str() + 1, nullptr, 16);
          f.push_back([&, p] {
            for (int y = 0; y < h; y++) {
              for (int x = 0; x < w; x++) {
                uint8_t *u = &*rgb.begin() + (y*w + x) * 3;
                u[0] = ((p&0xf00)>>8) * 0x11;
                u[1] = ((p & 0xf0) >> 4) * 0x11;
                u[2] = (p&0xf) * 0x11;
              }
            }
          });
        }
        else {
          hit = false;
        }
        if (hit) {
          n = e - 1;
        }
      }
    }
    else {
      wchar_t c = str[n];
      f.push_back([&,c] {
        if (L' ' == c) {
          xpos += s / 2;
        }
        else {
          FT_Load_Char(face, c, FT_LOAD_RENDER);
          auto &b = slot->bitmap;
          for (int y = 0; y < b.rows; y++)
          {
            for (int x = 0; x < b.width; x++)
            {
              if (0 < b.buffer[y*b.pitch + x]) {
                put(xpos + x - slot->bitmap_left, y + s - slot->bitmap_top, (float)b.buffer[y*b.pitch + x] / 255.0f);
              }
            }
          }
          xpos += b.width + u;
        }
      });
    }
  }
  rgb.resize(w*h * 3, 0);
  for (const auto &i : f) {
    i();
  }
  wts::Png png;
  png.image_header.width = w;
  png.image_header.height = h;
  png.image_header.color_depth = 8;
  png.image_header.color_type = wts::COLOR_TYPE_RGB;
  png.image_header.compress_type = wts::COMPRESS_TYPE_LZ;
  png.image_header.filter_type = wts::FILTER_TYPE_NONE;
  png.image_header.interlace_type = wts::INTERLACE_TYPE_NONE;
  png.image_data = &*rgb.begin();
  wts::Raw raw;
  wts::WriteFromPng(&raw, &png);
  for (int n = 0; n < raw.size; n++) {
    putchar(raw.data[n]);
  }
  wts::FreeRaw(&raw);
}
