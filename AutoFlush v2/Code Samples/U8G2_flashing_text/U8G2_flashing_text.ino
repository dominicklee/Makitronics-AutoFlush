#include <U8g2lib.h>

//U8g2 Contructor
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);
// Alternative board version. Uncomment if above doesn't work.
// U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 4, /* clock=*/ 14, /* data=*/ 2);

int counter = 0;
int page = 0;

void button(U8G2 u8g2, uint8_t x, uint8_t y, uint8_t width, String str, bool clicked)
{
    int getMaxCharHeight = 9;
    int charWidth = 7;

    if (clicked) {
        u8g2.setDrawColor(1);
        u8g2.drawRBox(x, y+1, width,  getMaxCharHeight + 12, 2);
        u8g2.setDrawColor(0);
        u8g2.setFont(u8g2_font_profont15_mr);
        u8g2.drawStr((width/2) - (charWidth * str.length() / 2), getMaxCharHeight + y + 7, (String(str)).c_str());
    } else {
        u8g2.setDrawColor(1);
        u8g2.drawRFrame(x, y, width,  getMaxCharHeight + 14, 4);
        u8g2.setFont(u8g2_font_profont15_mr);
        u8g2.drawStr((width/2) - (charWidth * str.length() / 2), getMaxCharHeight + y + 7, (String(str)).c_str());
    }
}

void draw()
{
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_5x8_tf);

    if (page == 0)
      button(u8g2, 2, 4, 124, counter % 2 ? "Flushing..." : "Flushing...", counter % 2 ? true: false);
    else if (page == 1)
      button(u8g2, 2, 4, 124, counter % 2 ? "Refilling..." : "Refilling...", counter % 2 ? true: false);

  } while ( u8g2.nextPage() );
}

void setup(void) {
  u8g2.begin();

}

void loop(void) {
  if (counter < 12) {
    counter++;
  }
  else {
    counter = 0;
    if (page == 0) {
      page = 1;
    }
    else {
      page = 0;
    }
  }
  draw();
  delay(200);
}
