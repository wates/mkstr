#ifndef BINCC_H_INCLUDED
#define BINCC_H_INCLUDED
#include <stdint.h>
namespace bincc_data{
extern const uint8_t font_ttf[];
static const int font_ttf_size=1716320;
static const uint8_t *data[]={
  font_ttf,
};
static const int data_size[]={
  1716320,
};
static const char* data_names[]={
  "font.ttf",
};
}
#endif