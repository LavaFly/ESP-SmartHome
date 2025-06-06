/* Using LVGL with Arduino requires some extra steps...
 *  
 * Be sure to read the docs here: https://docs.lvgl.io/master/integration/framework/arduino.html
 * but note you should use the lv_conf.h from the repo as it is pre-edited to work.
 * 
 * You can always edit your own lv_conf.h later and exclude the example options once the build environment is working.
 * 
 * Note you MUST move the 'examples' and 'demos' folders into the 'src' folder inside the lvgl library folder 
 * otherwise this will not compile, please see README.md in the repo.
 * 
 */
#include <lvgl.h>

#include <TFT_eSPI.h>

#include <examples/lv_examples.h>
#include <demos/lv_demos.h>

#include <XPT2046_Touchscreen.h>



#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "internet_settings.h"

// A library for interfacing with the touch screen
//
// Can be installed from the library manager (Search for "XPT2046")
//https://github.com/PaulStoffregen/XPT2046_Touchscreen
// ----------------------------
// Touch Screen pins
// ----------------------------

// The CYD touch uses some non default
// SPI pins

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240,touchScreenMaximumY = 3800;

/*Set to your screen resolution*/
#define TFT_HOR_RES   320
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_0


/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint8_t* draw_buf;

#define LCD_BACK_LIGHT_PIN 21
// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0
// use 12 bit precission for LEDC timer
#define LEDC_TIMER_12_BIT  12
// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000
#define LV_USE_LOG 1

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif


JsonDocument doc;
void example_buttonmatrix_1(void);
static void event_handler(lv_event_t * e);
void httpGetRequestIgnoreResponse(const char* path);

unsigned long timeOfLastTouch = 0;
unsigned long timeOfLastMessage = 0;
bool displayOn = false;
lv_display_t * disp;

float temperature = 7;
float humidity = 87;
float co2 = 578;

static char temp_text[16];
static char co2_text[16];
static char humid_text[4];

static const char * btnm_map[] = {"On", "PC", "Off", "L+", "L-", "\n",
                                  temp_text, co2_text, "\n",
                                  "Time", humid_text, "Vibe", ""
                                 };


static const char * path_map[] = {
  "http://lighting.local/on",    // On
  "http://pc.local/on",        // PC
  "http://lighting.local/off",   // Off
  "http://lighting.local/raiseBrightness",   // L+
  "http://lighting.local/lowerBrightness",   // L-
  "",
  "",
  "http://led.local/showTime",
  "",
  "http://led.local/makeItRain"
};

WiFiClient client;
HTTPClient http;

static void event_handler(lv_event_t * e){

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj =  (lv_obj_t*)lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_buttonmatrix_get_selected_button(obj);
        const char * txt = lv_buttonmatrix_get_button_text(obj, id);
        LV_UNUSED(txt);
        LV_LOG_USER("%s was pressed\n", txt);  
        httpGetRequestIgnoreResponse(path_map[id]);
    }
}



void example_buttonmatrix_2(void)
{
    snprintf(temp_text, sizeof(temp_text), "T = %2.0f°C", temperature);
    snprintf(co2_text, sizeof(co2_text), "CO2 = %4.0fppm", co2);
    snprintf(humid_text, sizeof(humid_text), "%2.0f%%", humidity);

    lv_obj_t * btnm1 = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(btnm1, btnm_map);
    //lv_obj_add_style(btnm1, &style_btn, LV_PART_ITEMS);
    lv_obj_set_size(btnm1, 300, 200);
    // rewrite this to the better version mentioned in the docs
    lv_buttonmatrix_set_button_width(btnm1, 5, 2); 
    lv_buttonmatrix_set_button_width(btnm1, 6, 2); 
    lv_buttonmatrix_set_button_width(btnm1, 7, 2);
    lv_buttonmatrix_set_button_width(btnm1, 9, 2); 

    lv_buttonmatrix_set_button_ctrl(btnm1, 11, LV_BUTTONMATRIX_CTRL_CHECKED);
    lv_obj_align(btnm1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btnm1, event_handler, LV_EVENT_ALL, NULL);
}


void httpGetRequestIgnoreResponse(const char* path){
  http.begin(client, path);
  int httpCode = http.GET();
  http.end();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
    /*Call it to tell LVGL you are ready*/
    lv_disp_flush_ready(disp);
}
/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
  if(touchscreen.touched())
  {
    TS_Point p = touchscreen.getPoint();
    //Some very basic auto calibration so it doesn't go out of range
    if(p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
    if(p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
    if(p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
    if(p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;
    //Map this to the pixel position
    data->point.x = map(p.x,touchScreenMinimumX,touchScreenMaximumX,1,TFT_HOR_RES); /* Touchscreen X calibration */
    data->point.y = map(p.y,touchScreenMinimumY,touchScreenMaximumY,1,TFT_VER_RES); /* Touchscreen Y calibration */
    data->state = LV_INDEV_STATE_PRESSED;

    timeOfLastTouch = millis();
    /*
    Serial.print("Touch x ");
    Serial.print(data->point.x);
    Serial.print(" y ");
    Serial.println(data->point.y);
    */
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void send_display_command(uint8_t cmd) {
    digitalWrite(TFT_CS, LOW);
    digitalWrite(TFT_DC, LOW); // Command mode
    touchscreenSpi.beginTransaction(SPISettings(27000000, MSBFIRST, SPI_MODE0));
    touchscreenSpi.transfer(cmd);
    touchscreenSpi.endTransaction();

    digitalWrite(TFT_CS, HIGH);
}

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  value = min(value, valueMax);
  uint32_t duty = (uint32_t)((4095.0 * value) / valueMax);
  ledcWrite(channel, duty);
}

void turnDisplayOff(){
  Serial.println("display off");
  digitalWrite(LCD_BACK_LIGHT_PIN, LOW);
  //setCpuFrequencyMhz(80);
  displayOn = false;
}

void turnDisplayOn(){
  Serial.println("display on");
  digitalWrite(LCD_BACK_LIGHT_PIN, HIGH);
  //setCpuFrequencyMhz(240);
  displayOn = true;
}
const String httpGetRequest(const char* path){
    if(http.begin(client, path)){
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
           //static String rsp = ;
           return http.getString();
        }
        http.end();
    }
    return "";
}

// the data is loaded into doc, not sure how i like this implementation, but
// i havent compared different ideas yet
uint8_t jsonDataRequest(const char *path, uint8_t sizeOfData){
   const String response = httpGetRequest(path);
   DeserializationError err = ArduinoJson::deserializeJson(doc, (response));

   if(err.Ok){
       Serial.println(err.c_str());
       Serial.println(err.code());
       return 0;
    }
    return 1;
}


lv_indev_t * indev; //Touchscreen input device
//uint8_t* draw_buf;  //draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0;  //Used to track the tick timer

void setup()
{
  //Some basic info on the Serial console
  String LVGL_Arduino = "LVGL demo ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);
  pinMode(LCD_BACK_LIGHT_PIN, OUTPUT);

    
  //Initialise the touchscreen
  touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  touchscreen.begin(touchscreenSpi); /* Touchscreen init */
  touchscreen.setRotation(3); /* Inverted landscape orientation to match screen */

  //Initialise LVGL
  lv_init();

  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);
  

  //Initialize the XPT2046 input device driver
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);  
  lv_indev_set_read_cb(indev, my_touchpad_read);

  example_buttonmatrix_2();


  WiFi.begin(APSSID, APPASS);
   while(WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }

  turnDisplayOff();

  //Done
  Serial.println( "Setup done" );
}

void loop()
{   
    if(displayOn){
      lv_tick_inc(millis() - lastTick); //Update the tick timer. Tick is new for LVGL 9
      lastTick = millis();
      lv_timer_handler();
    }                   //Update the UI
    delay(5);

    if(!displayOn && touchscreen.touched()){
      turnDisplayOn();
      lv_scr_load_anim(lv_scr_act(), LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
      example_buttonmatrix_2();
      timeOfLastTouch = millis();
    }
    if(displayOn && (millis() - timeOfLastTouch) > 5000){
      turnDisplayOff();
      timeOfLastMessage = millis();
    }

  

    if((millis() - timeOfLastMessage) > 5 * 1000 * 60){
      timeOfLastMessage = millis();
      uint8_t successful =  jsonDataRequest("http://base.local/currentReading", 0); 

      if(!successful){
        Serial.println("no response or smth");
        return;
      }

      // check if reading is valid
      if(doc["sensor"] == "base"){ // or "invalid"
        // have not decided what to do in this case
      }

      temperature = doc["temperature"].as<float>();
      humidity = doc["humidity"].as<float>();
      co2 = doc["co2"].as<float>();
    }
}

