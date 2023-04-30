#include <Arduino.h>
#include <Button2.h>

#include <gfx_cpp14.hpp>
#include <lilygot54in7.hpp>

#include "Bm437_HP_100LX_16x12.h"

#define SERIAL_BAUD 115200

#define GRAPH_INSET_PX 32

#define GFX_ROTATION 1

#define BTN_LEFT_PIN 35
#define BTN_CENTER_PIN 34
#define BTN_RIGHT_PIN 39

using namespace arduino;
using namespace gfx;

lilygot54in7 epd;
using epd_color = color<decltype(epd)::pixel_type>;

Button2 leftButton(BTN_LEFT_PIN);
Button2 centerButton(BTN_CENTER_PIN);
Button2 rightButton(BTN_RIGHT_PIN);

void drawGraphData() {}

void drawGraph(const rect16 bounds, const srect16 ranges,
               const spoint16 tickIntervals) {
  // prepare area
  draw::filled_rectangle(epd, bounds, epd_color::white);
  draw::rectangle(epd, bounds, epd_color::black);

  // calculate pixel size for graph
  auto graphSize = point16(min(bounds.height(), epd.dimensions().height),
                           min(bounds.width(), epd.dimensions().width));
  auto dataSize =
      point16(graphSize.x - GRAPH_INSET_PX, graphSize.y - GRAPH_INSET_PX);

  // draw axis lines
  auto xLine = srect16(GRAPH_INSET_PX, GRAPH_INSET_PX, GRAPH_INSET_PX, graphSize.x);
  draw::line(epd, xLine, epd_color::black);
  auto yLine = srect16(GRAPH_INSET_PX, GRAPH_INSET_PX, graphSize.y, GRAPH_INSET_PX);
  draw::line(epd, yLine, epd_color::black);

  // draw x-axis ticks and labels
  uint32_t xRange = abs(ranges.x2 - ranges.x1);
  uint32_t xTicks = floor(max(0.0, (xRange / tickIntervals.x) - 1.0));
  uint32_t xPixelsPerTick = dataSize.x / xTicks;
  for (uint8_t i = 1; i <= xTicks; i++) {
    uint32_t y = GRAPH_INSET_PX + (i * xPixelsPerTick);
    auto tickLine = srect16(GRAPH_INSET_PX / 2, y, GRAPH_INSET_PX, y);

    draw::line(epd, tickLine, epd_color::black);

    // char buffer[8];
    // sprintf(buffer, "%d", (uint32_t)round(i * tickIntervals.x));

    // Serial.println(buffer);

    // auto textScale = Montserrat.scale(GRAPH_INSET_PX / 2);
    // auto textBounds = srect16(spoint16::zero(), Montserrat.measure_text(ssize16::max(), spoint16::zero(), buffer, textScale));
    // open_text_info oti(buffer, Montserrat, textScale);
    // draw::text(epd, textBounds.center(srect16(0, 0, GRAPH_INSET_PX / 2, graphSize.y)), oti, epd_color::black);
  }

  // draw y-axis ticks and labels
  uint32_t yRange = abs(ranges.y2 - ranges.y1);
  uint32_t yTicks = floor(max(0.0, (yRange / tickIntervals.y) - 1.0));
  uint32_t yPixelsPerTick = dataSize.y / yTicks;
  for (uint8_t i = 1; i <= yTicks; i++) {
    uint32_t x = GRAPH_INSET_PX + (i * yPixelsPerTick);
    auto tickLine = srect16(x, GRAPH_INSET_PX / 2, x, GRAPH_INSET_PX);

    draw::line(epd, tickLine, epd_color::black);

    char buffer[8];
    sprintf(buffer, "%d", (uint32_t)round(i * tickIntervals.y));

    Serial.println(buffer);

    // auto textScale = Bm437_HP_100LX_16x12_FON.scale(GRAPH_INSET_PX / 2);
    // auto textBounds = srect16(spoint16::zero(), Montserrat.measure_text(ssize16::max(), spoint16::zero(), buffer, textScale));
    text_info ti(buffer, Bm437_HP_100LX_16x12_FON, 16);
    // draw::text(epd, textBounds.center(srect16(0, 0, GRAPH_INSET_PX / 2, graphSize.y)), oti, epd_color::black);
    auto textBounds = srect16(spoint16::zero(), Bm437_HP_100LX_16x12_FON.measure_text(ssize16::max(), buffer, 16));
    auto centeredBounds = textBounds.center(srect16(0, 0, GRAPH_INSET_PX / 2, graphSize.y));
    draw::text(epd, centeredBounds, ti, epd_color::black);
  }
  // draw data series
}

void leftButtonClick(Button2 &btn) {
  // todo: page up
}

void centerButtonClick(Button2 &btn) {
  // todo: refresh?
}

void rightButtonClick(Button2 &btn) {
  // todo: page down
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  leftButton.setClickHandler(leftButtonClick);
  centerButton.setClickHandler(centerButtonClick);
  rightButton.setClickHandler(rightButtonClick);

  epd.rotation(GFX_ROTATION);
  epd.initialize();
  epd.clear(epd.bounds());
}

void loop() {
  draw::suspend(epd);
  auto graphBounds =
      rect16(0, 0, epd.bounds().width(), epd.bounds().height() - 96);
  auto graphRanges = srect16(0, 0, 100, 100);
  auto graphTicks = spoint16(5, 5);
  drawGraph(graphBounds, graphRanges, graphTicks);
  drawGraphData();
  draw::resume(epd);
  epd.sleep(true);
  delay(30000);
}
