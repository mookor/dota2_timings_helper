// базовый пример инициализации и вывода текста
#include <LiquidCrystal_I2C.h>  
LiquidCrystal_I2C lcd(0x27, 16, 2);  // адрес, столбцов, строк

#include <microDS3231.h>
MicroDS3231 rtc;

#include "GyverButton.h"
int BTN_PIN = 4;
int LED_PIN = 10;
int SONG_LED_PIN = 9;
GButton butt1(BTN_PIN);

int start_game_unix = 0;
int kill_torm_unix = 0;
int kill_rosh_unix = 0;
int led_value = 0;


bool rosh_died = false;
bool start = false;
bool torm_died = false;
bool songing = false;
bool led_up = true;

const int SPEAKER=11;


void setup() {
  
  Serial.begin(9600);
  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку  
  
  lcd.setCursor(0, 0);
  lcd.print("l:");
  
  butt1.setDebounce(40);        // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(1000);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(600); 
  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SONG_LED_PIN, OUTPUT);
}

void beep()
{
  
  tone(SPEAKER, 784, 1000);

}

/**
 * Calculates the number of seconds remaining until a given moment.
 *
 * @param time_stamp the UNIX timestamp representing the starting point
 * @param to_moment_sec the number of seconds to the desired moment
 *
 * @return the number of seconds remaining until the desired moment
 *
 * @throws None
 */
int get_seconds_to_moment(int time_stamp, int to_moment_sec)
{

  int current_unix = rtc.getUnix(7);

  int elapsed_seconds = current_unix - time_stamp;
  int return_seconds = to_moment_sec - elapsed_seconds;
 
  if (return_seconds < 15)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
  if (return_seconds == 10)
    if (songing)
      beep();
  return return_seconds;
}

/**
 * Draws a lotus on the screen with the elapsed time since the start of the game.
 *
 * @param start_game_unix the Unix timestamp representing the start of the game
 *
 * @return void
 *
 * @throws None
 */
void draw_lotus()
{
  int to_lotus_sec = 3 * 60;
    
  int elapsed_seconds = get_seconds_to_moment(start_game_unix, to_lotus_sec);  
  int elapsed_minutes = elapsed_seconds / 60;
  elapsed_seconds = elapsed_seconds % 60;
  
  
  char lcd_buffer[32]; // Массив для вывода
  sprintf(lcd_buffer, "%d.%02d", elapsed_minutes,elapsed_seconds );
  lcd.setCursor(2, 0);
  lcd.print(lcd_buffer);
  Serial.println(lcd_buffer);
  if (elapsed_minutes <= 0 && elapsed_seconds <= 0)
  {
    start_game_unix =rtc.getUnix(7);

  }

}
/**
 * Updates the status of the Torm.
 *
 * @return void
 */
void torm_status()
{
  int torm_time = 599;
  int elapsed_seconds = get_seconds_to_moment(kill_torm_unix, torm_time);  


  int torm_time_minute = elapsed_seconds / 60;
  int torm_time_sec = elapsed_seconds % 60;

  char lcd_buffer[32];
  if (torm_time_minute >= 0 && torm_time_sec >= 0)
  {
    sprintf(lcd_buffer, "t:%d.%02d", torm_time_minute, torm_time_sec );
    lcd.setCursor(0, 1);
    lcd.print(lcd_buffer);
  }

  if (torm_time_minute == 0 && torm_time_sec == 0)
  {
    torm_died = false;
    lcd.print("        ");
    lcd.setCursor(0, 1);
    lcd.print("Talive");
  }
}
/**
 * Updates the status of the rosh (a game character) on the LCD display.
 *
 * This function calculates the time remaining for the maximum and minimum
 * rosh values and displays them on the LCD screen. If the maximum rosh time
 * is 9 minutes and 59 seconds, it clears the corresponding LCD position. If
 * the maximum rosh time is 0 minutes and 0 seconds, it sets the rosh_died
 * flag to false. Otherwise, it formats and displays the maximum rosh time
 * using the lcd_buffer array. Similarly, it formats and displays the minimum
 * rosh time using the lcd_buffer array if the values are non-negative.
 * If the rosh_died flag is false, it clears the LCD positions and displays
 * "Ralive" to indicate that the rosh is alive.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void rosh_status()
{
  int max_rosh = 11 * 60;
  int min_rosh = 8 * 60;

  
  int elapsed_seconds_max_rosh = get_seconds_to_moment(kill_rosh_unix, max_rosh); 
  int max_rosh_minute = elapsed_seconds_max_rosh / 60;
  int max_rosh_sec = elapsed_seconds_max_rosh % 60;

  int elapsed_seconds_min_rosh = get_seconds_to_moment(kill_rosh_unix, min_rosh); 
  int min_rosh_minute = elapsed_seconds_min_rosh / 60;
  int min_rosh_sec = elapsed_seconds_min_rosh % 60;

  char lcd_buffer[32]; // Массив для вывода
  if (max_rosh_minute == 9 && max_rosh_sec <= 59)
  {
    lcd.setCursor(15, 0);
    lcd.print(" ");
  }
  if (max_rosh_minute <= 0 && max_rosh_sec <= 0)
  {
    rosh_died = false;
  }
  else
  {
    sprintf(lcd_buffer, "rmx:%d.%02d", max_rosh_minute ,max_rosh_sec);
    lcd.setCursor(7, 0);
    lcd.print(lcd_buffer);
  }
  if (min_rosh_minute >= 0 && min_rosh_sec >= 0)
  {
    sprintf(lcd_buffer, "rmn:%d.%02d", min_rosh_minute, min_rosh_sec );
    lcd.setCursor(7, 1);
    lcd.print(lcd_buffer);
    Serial.println(lcd_buffer);
  }
  else
  {
    lcd.setCursor(7, 1);
    lcd.print("        ");
  }
  if (!rosh_died)
  {
    lcd.setCursor(7, 0);
    lcd.print("        ");
    lcd.setCursor(7, 1);
    lcd.print("        ");
    lcd.setCursor(7, 0);
    lcd.print("Ralive");
  }

}

void loop() {
  char signal = Serial.read();
  if (millis()%1000<2 && start)
  {
    draw_lotus();
    if (rosh_died)
      rosh_status();
    if (torm_died)
      torm_status();
  }
  else if (millis()%1000<2 && !start)
  {
    lcd.clear();
    rosh_died = false;
    torm_died = false;
    lcd.setCursor(0, 0);
    lcd.print("l:");
  }
  

  butt1.tick();  // обязательная функция отработки. Должна постоянно опрашиваться
  if (butt1.isClick()) Serial.println(rtc.getTimeString());         // проверка на один клик
  if (butt1.isTriple() || signal == '1')
  {
    start_game_unix =rtc.getUnix(7);
    start = !start;
  }
  if (butt1.isSingle() || signal == '2')
  {
    rosh_died = true;
    kill_rosh_unix = rtc.getUnix(7);
    lcd.setCursor(7, 0);
    lcd.print("     ");
  }       // проверка на двойной клик
  if (butt1.isDouble() || signal == '3') 
  {
    torm_died = true;
    kill_torm_unix = rtc.getUnix(7);
  }  
  if (butt1.isHolded() || signal == '4')
  {
    Serial.println("hold");
    songing = !songing;
    
  }
  // if (millis()%1000<2  && hold )
  // {
  //   Serial.println("hold");
  //   songing = !songing;
  // }
  if (songing)
  {
    digitalWrite(SONG_LED_PIN, HIGH);
  }
  else  
    digitalWrite(SONG_LED_PIN, 0);
  
}