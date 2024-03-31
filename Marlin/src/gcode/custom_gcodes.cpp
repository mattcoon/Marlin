/**
 * Custom G-code implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.1.0
 * Date: 2023/10/27
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfigPre.h"

#if HAS_CGCODE

#include "../MarlinCore.h" // for wait_for_user
#include "../core/types.h"
#include "../gcode/gcode.h"
#include "../libs/buzzer.h"
#include "../lcd/marlinui.h"

#include "../lcd/e3v2/proui/dwinui.h" // mmm
#include "../lcd/e3v2/proui/dwin_lcd.h" // mmm
#include "custom_gcodes.h"
#include "../core/macros.h" // mmm
#include "../module/planner.h" // mmm

#if HAS_MEDIA
  // #include "file_header.h"
#endif

#ifdef DWIN_LCD_PROUI
  #include "../lcd/e3v2/proui/dwin.h"
  #if ENABLED(LCD_BED_TRAMMING)
    #include "../lcd/e3v2/proui/bed_tramming.h"
  #endif
#endif

#include "custom_gcodes.h"


//=============================================================================
// Extended G-CODES
//=============================================================================

void cError() {
  parser.unknown_command_warning();
}

#if DEBUG_DWIN
  void C997() {
    SERIAL_ECHOLNPGM("Simulating a printer freeze");
    TERN_(DWIN_LCD_PROUI, dwinRebootScreen());
    while (1) {};
  }
#endif

void C100() {
  // C100 - Change Physical minimums
  // X<min> Y<min> Z<min>
  // Set the minimum position for each axis
  // if (parser.seenval('X')) PRO_data.x_min_pos = parser.value_axis('X');
  // if (parser.seenval('Y')) PRO_data.y_min_pos = parser.value_axis('Y');
  // if (parser.seenval('Z')) PRO_data.z_min_pos = parser.value_axis('Z');
  C100_report();
}

void C100_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("Physical Minimums"));
  gcode.report_echo_start(forReplay);
  SERIAL_ECHOPGM("  C100");
  // SERIAL_ECHOPGM(" X", PRO_data.x_min_pos);
  // SERIAL_ECHOPGM(" Y", PRO_data.y_min_pos);
  // SERIAL_ECHOPGM(" Z", PRO_data.z_min_pos);
  SERIAL_EOL();
}

void C101() {
  // C101 - Change Physical maximums
  // X<max> Y<max> Z<max>
  // Set the maximum position for each axis
  // if (parser.seenval('X')) PRO_data.x_max_pos = parser.value_axis('X');
  // if (parser.seenval('Y')) PRO_data.y_max_pos = parser.value_axis('Y');
  // if (parser.seenval('Z')) PRO_data.z_max_pos = parser.value_axis('Z');
  C101_report();
}

void C101_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("Physical Maximums"));
  gcode.report_echo_start(forReplay);
  SERIAL_ECHOPGM("  C101");
  // SERIAL_ECHOPGM(" X", PRO_data.x_max_pos);
  // SERIAL_ECHOPGM(" Y", PRO_data.y_max_pos);
  // SERIAL_ECHOPGM(" Z", PRO_data.z_max_pos);
  SERIAL_EOL();
}

void C102() {
  // C102 - Change Bed size
  // X<max> Y<max>
  // Set the maximum size for the bed
  // if (parser.seenval('X')) PRO_data.x_bed_size = parser.value_axis('X');
  // if (parser.seenval('Y')) PRO_data.y_bed_size = parser.value_axis('Y');
  C102_report();
}

void C102_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("Bed Size"));
  gcode.report_echo_start(forReplay);
  SERIAL_ECHOPGM("  C102");
  // SERIAL_ECHOPGM(" X", PRO_data.x_bed_size);
  // SERIAL_ECHOPGM(" Y", PRO_data.y_bed_size);
  SERIAL_EOL();
}

// Special Creality DWIN GCodes
void customGcode(const int16_t codenum) {
  switch(codenum) {
    #if DEBUG_DWIN
      case 997: C997(); break;            // Simulate a printer freeze
    #endif
    case 100: C100(); break;    // Change Physical minimums
    case 101: C101(); break;    // Change Physical maximums
    case 102: C102(); break;    // Change Bed size
    #if ENABLED(NOZZLE_PARK_FEATURE)
      case 125: C125(); break;  // Set park position
    #endif
    // case 562: C562(); break;    // Invert Extruder
    #if ENABLED(LASER_FEATURE)
      case 3:   C3(); break;                // set L = laser mode, F = fan mode
    #endif
    default: cError(); break;
  }
}

void customGcodeReport(const bool forReplay/*=true*/) {
  C100_report(forReplay);
  C101_report(forReplay);
  C102_report(forReplay);
  // C562_report(forReplay);
  #if ENABLED(LASER_FEATURE)
    C3_report();
  #endif
}


#if ENABLED(LASER_FEATURE)
  void C3 () {
    /* C3 - Laser Mode enable/disable 
          L - enable Laser mode. commands G3-5 enabled and fan changes sync with movement
          F - enable Fan mode. G3-5 disabled. fan changes immediately
          fan has priority if both selected
    */
    // if (parser.seen("FHLO")) {
    //   // set to fan mode
    //   if (parser.seen("F")) setLaserMode(false);
    //   // Set to Laser mode
    //   else if (parser.seen("L")) setLaserMode(true);
    //   // Set Laser Off Limit. used in G0 moves to keep laser active but not burning.
    //   if (parser.seenval('O')) hmiData.laser_off_pwr = parser.byteval('O');
    //   if (parser.seenval('H')) hmiData.target_laser_height = parser.byteval('H');
    C3_report();
    return;
  }

  void C3_report(const bool forReplay/*=true*/) {
    gcode.report_heading(forReplay, F("Laser/Fan Configuration"));
    gcode.report_echo_start(forReplay);
    SERIAL_ECHOPGM("  C3");
    // SERIAL_ECHOPGM(" O", hmiData.laser_off_pwr);
    // SERIAL_ECHOPGM(" H", hmiData.target_laser_height);
    if (1 ) // planner.laserMode
      SERIAL_ECHOPGM(" L  ; Laser Mode enabled");
    else
      SERIAL_ECHOPGM(" F  ; Fan Mode enabled");
    SERIAL_EOL();
  }
#endif


#endif // HAS_CGCODE
