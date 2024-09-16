/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

//
// Prefix header to acquire configurations
//
#include "Conditionals-1-axes.h"
#include "Conditionals-2-LCD.h"
#include "Conditionals-3-etc.h"
#include "Conditionals-4-adv.h"

#ifndef __MARLIN_DEPS__
  #include HAL_PATH(.., inc/Conditionals_adv.h)
#endif

constexpr uint16_t DEF_X_BED_SIZE = X_BED_SIZE;
constexpr uint16_t DEF_Y_BED_SIZE = Y_BED_SIZE;
constexpr int16_t DEF_X_MIN_POS = X_MIN_POS;
constexpr int16_t DEF_Y_MIN_POS = Y_MIN_POS;
constexpr int16_t DEF_X_MAX_POS = X_MAX_POS;
constexpr int16_t DEF_Y_MAX_POS = Y_MAX_POS;
constexpr int16_t DEF_Z_MAX_POS = Z_MAX_POS;

typedef struct { // Do not change this data structure
  uint16_t x_bed_size = DEF_X_BED_SIZE;
  uint16_t y_bed_size = DEF_Y_BED_SIZE;
  int16_t x_min_pos  = DEF_X_MIN_POS;
  int16_t y_min_pos  = DEF_Y_MIN_POS;
  int16_t x_max_pos  = DEF_X_MAX_POS;
  int16_t y_max_pos  = DEF_Y_MAX_POS;
  int16_t z_max_pos  = DEF_Z_MAX_POS;
} CNC_data_t;
extern CNC_data_t CNC_data;

#undef X_BED_SIZE
#undef Y_BED_SIZE
#undef X_MIN_POS
#undef Y_MIN_POS
#undef X_MAX_POS
#undef Y_MAX_POS
#undef Z_MAX_POS

#define X_BED_SIZE (float)CNC_data.x_bed_size
#define Y_BED_SIZE (float)CNC_data.y_bed_size
#define X_MIN_POS  (float)CNC_data.x_min_pos
#define Y_MIN_POS  (float)CNC_data.y_min_pos
#define X_MAX_POS  (float)CNC_data.x_max_pos
#define Y_MAX_POS  (float)CNC_data.y_max_pos
#define Z_MAX_POS  (float)CNC_data.z_max_pos

