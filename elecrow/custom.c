// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "lvgl.h"
#include "custom.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
const char* weekday_text[] = {"Sun.", "Mon.", "Tue.", "Wed.", "Thur.", "Fri.", "Sat."};
const char* weather_text[] = {"Sunny", "Cloudy", "Rainy", "Showery", "Thundery"};
const char* month_text[] = {"Jan.", "Feb.", "Mar.", "Apr.", "May.", "Jun.", "Jul.", "Aug.", "Sep.", "Oct.", "Nov.", "Dec."};
static lv_timer_t * flash_weather_timer = NULL; // inclare a timer that used to get weather info 
static lv_timer_t * flash_time_timer = NULL; // inclare a timer that used to get time info 
lv_ui *c_ui = NULL;
/**
 * Create a demo application
 */

void set_weather_img(char* weather_param,lv_obj_t* obj,int big_small)
{
  if(strcmp(weather_param,"0") == 0)
  {
    lv_img_set_src(obj,&_taiyang_106x99);
    if(big_small == 0)
    {
      lv_label_set_text(c_ui->screen_label_3, weather_text[0]);
    }
    if(big_small)
    {
      lv_img_set_zoom(obj,120);
      lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
      if(big_small == 1)
      {
        lv_label_set_text(c_ui->screen_label_13, weather_text[0]);
      }
      else if(big_small == 2)
      {
        lv_label_set_text(c_ui->screen_label_14, weather_text[0]);
      }
    }
  }
  else if((strcmp(weather_param,"4") == 0) || (strcmp(weather_param,"5") == 0) || (strcmp(weather_param,"6") == 0) || (strcmp(weather_param,"7") == 0) || (strcmp(weather_param,"8") == 0) || (strcmp(weather_param,"9") == 0))             
  {
    lv_img_set_src(obj,&_5_106x99);
    if(big_small == 0)
    {
      lv_label_set_text(c_ui->screen_label_3, weather_text[1]);
    }
    if(big_small)
    {
      lv_img_set_zoom(obj,110);
      lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
      if(big_small == 1)
      {
        lv_label_set_text(c_ui->screen_label_13, weather_text[1]);
      }
      else if(big_small == 2)
      {
        lv_label_set_text(c_ui->screen_label_14, weather_text[1]);
      }
    }
  }
  else if(strcmp(weather_param,"10") == 0)
  {
    lv_img_set_src(obj,&_10_106x99);
    if(big_small == 0)
    {
      lv_label_set_text(c_ui->screen_label_3, weather_text[3]);
    }
    if(big_small)
    {
      lv_img_set_zoom(obj,110);
      lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
      if(big_small == 1)
      {
        lv_label_set_text(c_ui->screen_label_13, weather_text[3]);
      }
      else if(big_small ==2)
      {
        lv_label_set_text(c_ui->screen_label_14, weather_text[3]);
      }
    }
  }
  else if(strcmp(weather_param,"11") == 0)
  {
    lv_img_set_src(obj,&_11_106x99);
    if(big_small == 0)
    {
      lv_label_set_text(c_ui->screen_label_3, weather_text[4]);
    }
    if(big_small)
    {
      lv_img_set_zoom(obj,110);
      lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
      if(big_small == 1)
      {
        lv_label_set_text(c_ui->screen_label_13, weather_text[4]);
      }
      else if(big_small == 2)
      {
        lv_label_set_text(c_ui->screen_label_14, weather_text[4]);
      }
    }
  }
  else if((strcmp(weather_param,"13") == 0) || (strcmp(weather_param,"14") == 0) || (strcmp(weather_param,"15") == 0) || (strcmp(weather_param,"16") == 0) || (strcmp(weather_param,"17") == 0) || (strcmp(weather_param,"18") == 0))
  {
    lv_img_set_src(obj,&_14_106x99);
    if(big_small == 0)
    {
      lv_label_set_text(c_ui->screen_label_3, weather_text[2]);
    }
    if(big_small)
    {
      lv_img_set_zoom(obj,110);
      lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_REAL);
      if(big_small == 1)
      {
        lv_label_set_text(c_ui->screen_label_13, weather_text[2]);
      }
      else if(big_small == 2)
      {
        lv_label_set_text(c_ui->screen_label_14, weather_text[2]);
      }
    }
  }
}


static void format_day_offset(int offset, char *buffer, size_t buffer_len)
{
  if (buffer == NULL || buffer_len == 0) {
    return;
  }

  struct tm temp = timeinfo;
  if (temp.tm_year == 0 && temp.tm_mon == 0 && temp.tm_mday == 0) {
    temp.tm_year = 123;  /* default to 2023 */
    temp.tm_mon = 0;
    temp.tm_mday = 1;
  }

  temp.tm_mday = (temp.tm_mday > 0) ? temp.tm_mday : 1;
  temp.tm_mday += offset;

  time_t normalized = mktime(&temp);
  if (normalized != (time_t)-1) {
    localtime_r(&normalized, &temp);
  }

  snprintf(buffer, buffer_len, "%02d/%02d", temp.tm_mon + 1, temp.tm_mday);
}

void flash_weather_timer_callback(lv_timer_t * timer)
{
  static int counter = 0;
  counter++;
  if(counter >= 360)
  {
    get_weather();
    lv_label_set_text(c_ui->screen_label_7, weatherdata.city);
    lv_label_set_text(c_ui->screen_label_4, weatherdata.high);
    lv_label_set_text(c_ui->screen_label_5, weatherdata.humi);

    set_weather_img(weatherdata.weather, c_ui->screen_img_1, 0);
    set_weather_img(weatherdata.weather_1, c_ui->screen_img_5, 1);
    set_weather_img(weatherdata.weather_2, c_ui->screen_img_6, 2);
    
    counter = 0;
  }
  
}

void flash_time_timer_callback(lv_timer_t * timer)
{
  char current_time[32];
  get_time();
  memset(current_time, 0, sizeof(current_time));
  snprintf(current_time, sizeof(current_time), "%02d:%02d",
           timeinfo.tm_hour % 24, timeinfo.tm_min % 60);
  lv_label_set_text(c_ui->screen_label_6, current_time);

  int month_index = timeinfo.tm_mon;
  if (month_index < 0 || month_index >= 12) {
    month_index = 0;
  }
  int wday_index = timeinfo.tm_wday;
  if (wday_index < 0 || wday_index >= 7) {
    wday_index = 0;
  }

  memset(current_time, 0, sizeof(current_time));
  snprintf(current_time, sizeof(current_time), "%s %02d %04d",
           month_text[month_index],
           (timeinfo.tm_mday > 0 && timeinfo.tm_mday <= 31) ? timeinfo.tm_mday : 1,
           timeinfo.tm_year + 1900);
  lv_label_set_text(c_ui->screen_label_9, current_time);

  memset(current_time, 0, sizeof(current_time));
  format_day_offset(1, current_time, sizeof(current_time));
  lv_label_set_text(c_ui->screen_label_10, current_time);

  memset(current_time, 0, sizeof(current_time));
  format_day_offset(2, current_time, sizeof(current_time));
  lv_label_set_text(c_ui->screen_label_11, current_time);
    
  lv_label_set_text(c_ui->screen_label_8, weekday_text[wday_index]);
}


void custom_init(lv_ui *ui)
{
  /* Add your codes here */
  c_ui = ui;

  if(WIFI_try <= 8)
  {
    get_weather();
    lv_label_set_text(c_ui->screen_label_7, weatherdata.city);
    lv_label_set_text(c_ui->screen_label_4, weatherdata.high);
    lv_label_set_text(c_ui->screen_label_5, weatherdata.humi);
  
    set_weather_img(weatherdata.weather, c_ui->screen_img_1, 0);
    set_weather_img(weatherdata.weather_1, c_ui->screen_img_5, 1);
    set_weather_img(weatherdata.weather_2, c_ui->screen_img_6, 2);
    
    flash_weather_timer = lv_timer_create(flash_weather_timer_callback, 10000, NULL);
    flash_time_timer    = lv_timer_create(flash_time_timer_callback, 371, NULL);
  }
}
