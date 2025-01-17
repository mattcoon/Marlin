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

#include "custom_gcodes.h"
#include "../core/macros.h" // mmm
#include "../module/planner.h" // mmm

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
  if (parser.seenval('X')) CNC_data.x_min_pos = parser.value_axis_units(X_AXIS);
  if (parser.seenval('Y')) CNC_data.y_min_pos = parser.value_axis_units(Y_AXIS);
  C100_report();
}

void C100_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("Physical Minimums"));
  gcode.report_echo_start(forReplay);
  SERIAL_ECHOPGM("  C100");
  SERIAL_ECHOPGM(" X", CNC_data.x_min_pos);
  SERIAL_ECHOPGM(" Y", CNC_data.y_min_pos);
  SERIAL_EOL();
}

void C101() {
  // C101 - Change Physical maximums
  // X<max> Y<max> Z<max>
  // Set the maximum position for each axis
  if (parser.seenval('X')) CNC_data.x_max_pos = parser.value_axis_units(X_AXIS);
  if (parser.seenval('Y')) CNC_data.y_max_pos = parser.value_axis_units(Y_AXIS);
  if (parser.seenval('Z')) CNC_data.z_max_pos = parser.value_axis_units(Z_AXIS);
  C101_report();
}

void C101_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("Physical Maximums"));
  gcode.report_echo_start(forReplay);
  SERIAL_ECHOPGM("  C101");
  SERIAL_ECHOPGM(" X", CNC_data.x_max_pos);
  SERIAL_ECHOPGM(" Y", CNC_data.y_max_pos);
  SERIAL_ECHOPGM(" Z", CNC_data.z_max_pos);
  SERIAL_EOL();
}

void C102() {
  // C102 - Change Bed size
  // X<max> Y<max>
  // Set the maximum size for the bed
  if (parser.seenval('X')) CNC_data.x_bed_size = parser.value_axis_units(X_AXIS);
  if (parser.seenval('Y')) CNC_data.y_bed_size = parser.value_axis_units(Y_AXIS);
  C102_report();
}

void C102_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("Bed Size"));
  gcode.report_echo_start(forReplay);
  SERIAL_ECHOPGM("  C102");
  SERIAL_ECHOPGM(" X", CNC_data.x_bed_size);
  SERIAL_ECHOPGM(" Y", CNC_data.y_bed_size);
  SERIAL_EOL();
}


#if ENABLED(LASER_FEATURE)
  void C3 () {
    /* C3 - Laser Mode enable/disable 
          L - enable Laser mode. commands G3-5 enabled and fan changes sync with movement
          F - enable Fan mode. G3-5 disabled. fan changes immediately
          fan has priority if both selected
    */
    C3_report();
    return;
  }

  void C3_report(const bool forReplay/*=true*/) {
    gcode.report_heading(forReplay, F("Laser/Fan Configuration"));
    gcode.report_echo_start(forReplay);
    SERIAL_ECHOPGM("  C3");
      SERIAL_ECHOPGM(" L1  ; Laser Mode enabled");
    SERIAL_EOL();
  }
#endif

void GRBLcode(const int16_t codenum) {
  switch(codenum) {
    #if ENABLED(LASER_FEATURE)
      case 32:   C3(); break;      // set LaserMode
    #endif
    default: cError(); break;
  }
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



#endif // HAS_CGCODE
