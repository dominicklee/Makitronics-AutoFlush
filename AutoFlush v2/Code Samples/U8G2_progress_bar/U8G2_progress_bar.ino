#include <U8g2lib.h>

//U8g2 Contructor
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);
// Alternative board version. Uncomment if above doesn't work.
// U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 4, /* clock=*/ 14, /* data=*/ 2);

int counter = 0;

void progressBar(U8G2 u8g2, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t percent)
{
    // can't draw it smaller than 10x8
    height = height < 8 ? 8 : height;
    width = width < 10 ? 10 : width;

    // draw percentage
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(x + width / 2 - 5 * 1, y - 1, (String(percent) + String("%")).c_str());

    // draw it
    u8g2.drawRFrame(x, y, width, height, 4);
    u8g2.drawBox(x + 2, y + 2, (width - 4) * (percent / 100.0), height - 4);
}

void draw()
{
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);

    progressBar(u8g2, 2, 12, u8g2.getDisplayWidth() - 5, 18, counter);

  } while ( u8g2.nextPage() );
}

void setup(void) {
  u8g2.begin();

}

void loop(void) {
  if (counter < 100) {
    counter++;
  }
  else {
    counter = 0;
  }
  draw();
  delay(200);
}
