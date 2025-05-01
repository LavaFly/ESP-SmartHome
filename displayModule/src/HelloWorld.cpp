// note from me, i dont really like this code but i dislike writing a proper ui even more
// so i wont be cleaning or fixing it, maybe at some point in the future

/* Using LVGL with Arduino requires some extra steps...
 *
 * Be sure to read the docs here: https://docs.lvgl.io/master/integration/framework/arduino.html
 * but note you should use the lv_conf.h from the repo as it is pre-edited to work.
 *
 * You can always edit your own lv_conf.h later and exclude the example options once the build environment is working.
 *
 */
 #include <lvgl.h>

 #include <TFT_eSPI.h>

 #include <examples/lv_examples.h>
 #include <demos/lv_demos.h>

 #include <XPT2046_Touchscreen.h>



 #include <WiFi.h>
 #include <HTTPClient.h>
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
 
 
 #define LV_USE_LOG 1

 #if LV_USE_LOG != 0
 void my_print( lv_log_level_t level, const char * buf )
 {
     LV_UNUSED(level);
     Serial.println(buf);
     Serial.flush();
 }
 #endif



 void example_buttonmatrix_1(void);
 static void event_handler(lv_event_t * e);
 void httpGetRequestIgnoreResponse(const char* path);
 unsigned long timeOfLastTouch = 0;
 bool displayOn = false;
 void turnDisplayOff(){
  Serial.println("display off");
  digitalWrite(LCD_BACK_LIGHT_PIN, LOW);
  setCpuFrequencyMhz(80);
  displayOn = false;
 }

 void turnDisplayOn(){
  Serial.println("display on");
  digitalWrite(LCD_BACK_LIGHT_PIN, HIGH);
  setCpuFrequencyMhz(240);
  displayOn = true;
 }
 
 static const char * btnm_map[] = {"On", "PC", "Off", "L+", "L-", "\n",
                                   "1", "2", "3", "4", "5", "\n",
                                   "Show Time", "Rain", ""
                                  };


 static const char * path_map[] = {
   "http://lighting.local/on",    // On
   "http://pc.local/isLve",        // PC
   "http://lighting.local/off",   // Off
   "http://lighting.local/raiseBrightness",   // L+
   "http://lighting.local/lowerBrightness",   // L-
   "",
   "",
   "",
   "",
   "http://led.local/showTime",
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
     lv_obj_t * btnm1 = lv_buttonmatrix_create(lv_screen_active());
     lv_buttonmatrix_set_map(btnm1, btnm_map);
     lv_buttonmatrix_set_button_width(btnm1, 10, 2);        /*Make "Action1" twice as wide as "Action2"*/
     lv_buttonmatrix_set_button_ctrl(btnm1, 10, LV_BUTTONMATRIX_CTRL_CHECKABLE);
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
 
   lv_display_t * disp;
   draw_buf = new uint8_t[DRAW_BUF_SIZE];
   disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);

   //Initialize the XPT2046 input device driver
   indev = lv_indev_create();
   lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
   lv_indev_set_read_cb(indev, my_touchpad_read);

   //example_buttonmatrix_1();
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
    }
    delay(5);

    if(!displayOn && touchscreen.touched()){
      turnDisplayOn();
      example_buttonmatrix_2();
      timeOfLastTouch = millis();
    }
    if(displayOn && (millis() - timeOfLastTouch) > 5000){
      turnDisplayOff();
    }
 }


