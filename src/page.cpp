#include "./../include/page.h"







Font Page::font;




//page constructor
Page::Page() {
    if(font.texture.id == 0)
        font = LoadFont("./data/font/pacmanFont.ttf");
}