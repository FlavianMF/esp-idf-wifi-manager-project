/**
 * @file main.h
 * @author Flavian Melquiades (flavian.melquiades@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __MAIN_H__
#define __MAIN_H__

#include <assert.h>
#include <sys/param.h>
#include <string.h>
#include "esp_app_desc.h"
#include "sdkconfig.h"

#include "esp_log.h"
#include "esp_private/startup_internal.h"
#include "esp_attr.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "esp_err.h"
#include "esp_assert.h"
#include "esp_attr.h"


/* Stringify firmware build data included by cmake */
#define _str(s) #s
#define _xstr(s) _str(s)

#define TIME_OF_BUILD _xstr(__TIME__)
#define DATE_OF_BUILD _xstr(__DATE__)

#define VERSION_MAJOR       1
#define VERSION_MINOR       0
#define PATCHLEVEL          0
#define EXTRAVERSION        PROJECT_VER
#define VERSION_TWEAK       0

#define TOSTRING(x) #x

#define APP_VERSION_LONG VERSION_MAJOR.VERSION_MINOR.PATCHLEVEL-PROJECT_VER-VERSION_TWEAK
#define APP_VERSION_STRING _xstr(APP_VERSION_LONG)

#endif // __MAIN_H__