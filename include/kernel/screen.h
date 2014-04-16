#ifndef KERNEL_SCREEN_H
#define KERNEL_SCREEN_H

#include <type.h>

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define FAKE_SCREEN_HEIGHT 75
#define LEFT_UPPER    ((uword*)0xb8000)

namespace kernel {
  struct color {
    union {
      struct {
        ubyte foreground :4;
        ubyte background :4;
      };
      ubyte full;
    };
  };

  class Graphic {
    color _color;
    uword *_cursor;
  public:
    Graphic(ubyte background, ubyte foreground) :
      _cursor { LEFT_UPPER }
    {
      _color.background = background;
      _color.foreground = foreground;

      clear();
    }

    void clear() {
      for(udword x = 0; x < SCREEN_HEIGHT*SCREEN_WIDTH; ++x) {
        *_cursor++    = ' ' | (_color.full << 8);
      }

      setCursor(0, 0);
    }

    void write(const byte *c_str) {
      while(*c_str) {
        *reinterpret_cast<ubyte*>(_cursor++) = *c_str++;
      }
    }

    void write(const ubyte *window, udword length) {
      for(udword x = 0; x < length; ++x) {
        *reinterpret_cast<ubyte*>(_cursor++) = *window++;
      }
    }

    void setCursor(udword x, udword y) {
      _cursor = LEFT_UPPER + (y * SCREEN_WIDTH + x);
    }

    void moveCursor(udword x, udword y) {
      _cursor += (y * SCREEN_WIDTH + x);
    }
  };

  class Screen {
    Graphic _screen { 0x00, 0x07 };

    ubyte _map[FAKE_SCREEN_HEIGHT*SCREEN_WIDTH];
    ubyte *_window;
    ubyte *_cursor;

  public:

    Screen() :
      _window { _map },
      _cursor { _map }
    {
      for(auto &val : _map) {
        val = ' ';
      }
    }

    ubyte *operator()(udword x, udword y) {
      return _map + y*SCREEN_WIDTH + x;
    }

    void scroll(word y) {
      _window += y *SCREEN_WIDTH;
    }

    void refresh() {
      _screen.setCursor(0, 0);
      _screen.write(_window, SCREEN_HEIGHT*SCREEN_WIDTH);
    }

    Graphic &getGraphics() { return _screen; }


    void write(const byte *c_str) {
      while(*c_str) {
        switch(*c_str) {
        case '\n':
          // line break
          _cursor += SCREEN_WIDTH - (_cursor - _window) % SCREEN_WIDTH;
          ++c_str;
          break;
        default:
          *_cursor++ = *c_str++;
        }
      }

      while(_cursor > (_window + SCREEN_HEIGHT*SCREEN_WIDTH)) {
        scroll(1);
      }

      refresh();
    }
  };
  extern Screen &getScreen();
};

#endif
