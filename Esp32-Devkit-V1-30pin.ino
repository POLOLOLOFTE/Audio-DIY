#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <driver/i2s.h>
#include <math.h>

// ================= PIN =================
constexpr int PIN_BCLK = 5;     // SCLK codec
constexpr int PIN_LRCK = 25;    // LRCLK codec
constexpr int PIN_DIN  = 26;    // ESP -> codec
constexpr int PIN_DOUT = 35;    // codec -> ESP
constexpr int PIN_MCLK = 1;     // GPIO1 = CLK_OUT3 
constexpr int PIN_SDA  = 18;
constexpr int PIN_SCL  = 23;

constexpr int SW_PITCH   = 32;  // SW1
constexpr int SW_SHUTTER = 27;  // SW2
constexpr int SW_FILTER  = 33;  // SW3
constexpr int SW_FLANGER = 14;  // SW4
constexpr int SW_REVERB  = 13;  // SW5
constexpr int SW_ECHO    = 16;  // SW6

// ================= AUDIO =================
constexpr float FS    = 22050.0f;
constexpr int   BLOCK = 128;
constexpr float TAU   = 6.2831853f;

// ================= POTENZIOMETERS =================
enum : uint8_t { P_PITCH, P_SHUT_DIST, P_SHUT_DUR, P_FLANGER,
                 P_LOW, P_HIGH, P_REVERB, P_ECHO, P_COUNT };

struct PotSrc { uint8_t chip; uint8_t ch; };   // chip 0 = U1 (0x48), 1 = U2 (0x49)

static const PotSrc POT_SRC[P_COUNT] = {
  {0, 0},  // P_PITCH      RV1  U1 AIN0
  {0, 1},  // P_SHUT_DIST  RV2  U1 AIN1
  {0, 2},  // P_SHUT_DUR   RV3  U1 AIN2
  {0, 3},  // P_FLANGER    RV6  U1 AIN3
  {1, 0},  // P_LOW        RV4  U2 AIN0
  {1, 1},  // P_HIGH       RV5  U2 AIN1
  {1, 2},  // P_REVERB     RV7  U2 AIN2
  {1, 3},  // P_ECHO       RV8  U2 AIN3
};

Adafruit_ADS1115 ads[2];
volatile float pot[P_COUNT];

static inline float deadzone(float v) {
  const float LO = 0.04f, HI = 0.96f;   // death zone
  if (v <= LO) return 0.0f;
  if (v >= HI) return 1.0f;
  return (v - LO) / (HI - LO);
}

void potTask(void*) {
  for (;;) {
    for (uint8_t i = 0; i < P_COUNT; i++) {
      PotSrc s = POT_SRC[i];
      float raw = ads[s.chip].readADC_SingleEnded(s.ch) * (1.0f / 26400.0f); // GAIN_ONE, 3.3V
      float t = deadzone(raw);
      float p = pot[i] + (t - pot[i]) * 0.3f;   // smoothing
      if (t == 0.0f && p < 0.01f) p = 0.0f;
      if (t == 1.0f && p > 0.99f) p = 1.0f;
      pot[i] = p;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// ================= BUFFER =================
static inline float rd(const int16_t* b, uint16_t i) { return b[i] * (1.0f / 32768.0f); }
static inline void  wr(int16_t* b, uint16_t i, float v) {
  if (v > 0.999f) v = 0.999f; else if (v < -0.999f) v = -0.999f;
  b[i] = (int16_t)(v * 32767.0f);
}

// Echo
constexpr uint16_t ECHO_LEN = 6616;
static int16_t echoBuf[ECHO_LEN];
static uint16_t echoW = 0;

// Flanger
constexpr uint8_t FL_LEN = 128;
static int16_t flBuf[FL_LEN];
static uint8_t flW = 0;
static float flPh = 0.0f;

// Pitch shifter
static int16_t pBuf[2048];
static uint16_t pW = 0;
static float pP1 = 0.0f, pP2 = 0.5f;

// Reverb 
static int16_t cb0[558], cb1[594], cb2[638], cb3[678];
static int16_t ab0[278], ab1[112];

// ================= EFFECTS =================

static float fxEcho(float x, float wet) {
  uint16_t r = echoW + 1;               // campione di 6615 passi fa
  if (r >= ECHO_LEN) r = 0;
  float d = rd(echoBuf, r);
  wr(echoBuf, echoW, x + d * (0.25f + 0.45f * wet));
  echoW = r;
  return x + d * wet;
}

// Flanger
static float fxFlanger(float x, float depth) {
  flPh += TAU * 0.25f / FS;             // LFO a 0.25 Hz
  if (flPh > TAU) flPh -= TAU;
  float ms  = 1.0f + 1.5f * (1.0f + sinf(flPh));     // 1..4 ms
  float pos = (float)flW - ms * (FS / 1000.0f);
  if (pos < 0.0f) pos += FL_LEN;
  uint8_t i0 = (uint8_t)pos;
  float f = pos - i0;
  uint8_t i1 = (i0 + 1) & (FL_LEN - 1);
  float d = rd(flBuf, i0) * (1.0f - f) + rd(flBuf, i1) * f;
  wr(flBuf, flW, x + d * 0.5f);
  flW = (flW + 1) & (FL_LEN - 1);
  return x + d * depth;
}

// Pitch shifter
static float fxPitch(float x, float amt) {
  wr(pBuf, pW, x);
  float ratio = 1.0f + 0.6f * amt;
  float step  = (1.0f - ratio) / 1536.0f;
  pP1 += step; pP2 += step;
  if (pP1 < 0.0f) pP1 += 1.0f; else if (pP1 >= 1.0f) pP1 -= 1.0f;
  if (pP2 < 0.0f) pP2 += 1.0f; else if (pP2 >= 1.0f) pP2 -= 1.0f;
  uint16_t d1 = 256 + (uint16_t)(pP1 * 1536.0f);
  uint16_t d2 = 256 + (uint16_t)(pP2 * 1536.0f);
  float s1 = rd(pBuf, (pW - d1) & 2047);
  float s2 = rd(pBuf, (pW - d2) & 2047);
  float w1 = 1.0f - fabsf(2.0f * pP1 - 1.0f);
  float w2 = 1.0f - fabsf(2.0f * pP2 - 1.0f);
  pW = (pW + 1) & 2047;
  return s1 * w1 + s2 * w2;
}

// Filtro
struct Biquad { float b0, b1, b2, a1, a2, x1, x2, y1, y2; };
static Biquad bLow  {1,0,0,0,0,0,0,0,0};
static Biquad bHigh {1,0,0,0,0,0,0,0,0};

static inline float biq(Biquad& b, float x) {
  float y = b.b0 * x + b.b1 * b.x1 + b.b2 * b.x2 - b.a1 * b.y1 - b.a2 * b.y2;
  b.x2 = b.x1; b.x1 = x;
  b.y2 = b.y1; b.y1 = y;
  return y;
}

static void shelfCoef(Biquad& b, float fc, float dB, bool low) {
  float A  = powf(10.0f, dB / 40.0f);
  float w0 = TAU * fc / FS;
  float c  = cosf(w0);
  float al = sinf(w0) * 0.70710678f;            // S = 1
  float sA = 2.0f * sqrtf(A) * al;
  float b0, b1, b2, a0, a1, a2;
  if (low) {
    b0 =      A * ((A + 1) - (A - 1) * c + sA);
    b1 =  2 * A * ((A - 1) - (A + 1) * c);
    b2 =      A * ((A + 1) - (A - 1) * c - sA);
    a0 =          (A + 1) + (A - 1) * c + sA;
    a1 = -2 *     ((A - 1) + (A + 1) * c);
    a2 =          (A + 1) + (A - 1) * c - sA;
  } else {
    b0 =      A * ((A + 1) + (A - 1) * c + sA);
    b1 = -2 * A * ((A - 1) + (A + 1) * c);
    b2 =      A * ((A + 1) + (A - 1) * c - sA);
    a0 =          (A + 1) - (A - 1) * c + sA;
    a1 =  2 *     ((A - 1) - (A + 1) * c);
    a2 =          (A + 1) - (A - 1) * c - sA;
  }
  b.b0 = b0 / a0; b.b1 = b1 / a0; b.b2 = b2 / a0;
  b.a1 = a1 / a0; b.a2 = a2 / a0;
}

static void updateFilter(float low, float high) {
  static float lastL = -1.0f, lastH = -1.0f;
  float dbL = low * 12.0f, dbH = high * 12.0f;
  if (fabsf(dbL - lastL) > 0.1f) { shelfCoef(bLow,  200.0f,  dbL, true);  lastL = dbL; }
  if (fabsf(dbH - lastH) > 0.1f) { shelfCoef(bHigh, 4000.0f, dbH, false); lastH = dbH; }
}

// Reverb
struct Comb { int16_t* b; uint16_t n, w; float lp; };
struct Allp { int16_t* b; uint16_t n, w; };
static Comb combs[4] = {{cb0,558,0,0.0f},{cb1,594,0,0.0f},{cb2,638,0,0.0f},{cb3,678,0,0.0f}};
static Allp aps[2]   = {{ab0,278,0},{ab1,112,0}};

static float fxReverb(float x, float wet) {
  float in  = x * 0.012f;
  float sum = 0.0f;
  for (auto& c : combs) {
    float y = rd(c.b, c.w);
    c.lp = y * 0.8f + c.lp * 0.2f;
    wr(c.b, c.w, in + c.lp * 0.84f);
    if (++c.w >= c.n) c.w = 0;
    sum += y;
  }
  for (auto& a : aps) {
    float bo = rd(a.b, a.w);
    float o  = bo - sum;
    wr(a.b, a.w, sum + bo * 0.5f);
    if (++a.w >= a.n) a.w = 0;
    sum = o;
  }
  return x * (1.0f - 0.4f * wet) + sum * wet;
}

// Shutter
static float fxShutter(float x, float dist, float dur) {
  static float cnt = 0.0f, g = 1.0f;
  float period = (0.05f + 0.5f * dist) * FS;
  if (++cnt >= period) cnt = 0.0f;
  float shut = dur * 0.8f * period;
  float target = (cnt < shut) ? 0.0f : 1.0f;
  g += (target - g) * 0.02f;
  return x * g;
}

// ================= AUDIO TASK =================
static inline bool swOn(int pin) { return digitalRead(pin) == LOW; }

void audioTask(void*) {
  static int16_t inB[BLOCK * 2];
  static int16_t outB[BLOCK * 2];
  for (;;) {
    size_t br = 0, bw = 0;
    i2s_read(I2S_NUM_0, inB, sizeof(inB), &br, portMAX_DELAY);

    const bool onPitch = swOn(SW_PITCH),  onShut = swOn(SW_SHUTTER);
    const bool onFil   = swOn(SW_FILTER), onFl   = swOn(SW_FLANGER);
    const bool onRev   = swOn(SW_REVERB), onEcho = swOn(SW_ECHO);

    // Switch spento = effetto spento, pot a 0 = effetto spento
    const float pPitch = onPitch ? pot[P_PITCH]   : 0.0f;
    const float pFl    = onFl    ? pot[P_FLANGER] : 0.0f;
    const float pRev   = onRev   ? pot[P_REVERB]  : 0.0f;
    const float pEcho  = onEcho  ? pot[P_ECHO]    : 0.0f;
    const float pDist  = pot[P_SHUT_DIST];
    const float pDur   = onShut  ? pot[P_SHUT_DUR] : 0.0f;

    if (onFil) updateFilter(pot[P_LOW], pot[P_HIGH]);

    for (int i = 0; i < BLOCK; i++) {
      float x = (inB[2 * i] + inB[2 * i + 1]) * (0.5f / 32768.0f);   // mono

      if (pPitch > 0.0f) { float s = fxPitch(x, pPitch); x += (s - x) * pPitch; }
      if (onFil)         { x = biq(bHigh, biq(bLow, x)); }
      if (pFl > 0.0f)    { x = fxFlanger(x, pFl); }
      if (pRev > 0.0f)   { x = fxReverb(x, pRev); }
      if (pEcho > 0.0f)  { x = fxEcho(x, pEcho); }
      if (pDur > 0.0f)   { x = fxShutter(x, pDist, pDur); }

      int32_t v = (int32_t)(x * 32767.0f);
      if (v > 32767) v = 32767; else if (v < -32768) v = -32768;
      outB[2 * i] = outB[2 * i + 1] = (int16_t)v;
    }
    i2s_write(I2S_NUM_0, outB, sizeof(outB), &bw, portMAX_DELAY);
  }
}

// ================= CODEC ES8388 =================
static void codecWrite(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(0x10);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

// Base da confermare sul datasheet ES8388 (o sul driver ESP-ADF)
static void codecInit() {
  codecWrite(0x00, 0x80);   // reset
  delay(50);
  codecWrite(0x00, 0x06);
  codecWrite(0x02, 0x00);   // chip power ON
  codecWrite(0x08, 0x00);   // slave: il clock lo genera l'ESP32
  codecWrite(0x03, 0x00);   // ADC power ON
  codecWrite(0x04, 0x3C);   // DAC power ON, uscite LOUT1/ROUT1
  codecWrite(0x09, 0x00);   // PGA ADC 0 dB
  codecWrite(0x0A, 0x50);   // ingresso LIN2/RIN2
  codecWrite(0x0C, 0x0C);   // formato ADC I2S 16 bit
  codecWrite(0x17, 0x00);   // formato DAC I2S 16 bit (verifica)
  codecWrite(0x19, 0x00);   // DAC unmute (bit 2 = mute)
  codecWrite(0x1A, 0x00);   // volume DAC sinistro 0 dB
  codecWrite(0x1B, 0x00);   // volume DAC destro 0 dB
  codecWrite(0x2E, 0x1E);   // volume LOUT1
  codecWrite(0x2F, 0x1E);   // volume ROUT1
}

// ================= SETUP =================
void setup() {
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(100000);

  const int swPins[] = {SW_PITCH, SW_SHUTTER, SW_FILTER, SW_FLANGER, SW_REVERB, SW_ECHO};
  for (int p : swPins) pinMode(p, INPUT_PULLUP);

  ads[0].begin(0x48);
  ads[1].begin(0x49);
  for (auto& a : ads) {
    a.setGain(GAIN_ONE);
    a.setDataRate(RATE_ADS1115_860SPS);
  }

  codecInit();

  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX);
  cfg.sample_rate = 22050;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
  cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  cfg.dma_buf_count = 4;
  cfg.dma_buf_len = 256;
  cfg.use_apll = true;
  cfg.tx_desc_auto_clear = true;
  cfg.fixed_mclk = 0;
  i2s_driver_install(I2S_NUM_0, &cfg, 0, NULL);

  i2s_pin_config_t pins = {};
  pins.bck_io_num   = PIN_BCLK;
  pins.ws_io_num    = PIN_LRCK;
  pins.data_out_num = PIN_DIN;
  pins.data_in_num  = PIN_DOUT;
  pins.mck_io_num   = PIN_MCLK;
  i2s_set_pin(I2S_NUM_0, &pins);

  xTaskCreatePinnedToCore(potTask,   "pot",   4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(audioTask, "audio", 4096, NULL, 5, NULL, 1);
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}

//SORRY IF I LEFT SOME COMMENTS IN MY OWN LANGUAGE
//COULD NOT WORK UNFORTUNATELY TO MY LOW PROGRAMMING KNOWING
