/**
 * Configuration Storing to SD for easy migrations
 * Author: Matthew Coon
 * Version: 1.0
 * Date: 2023/01/18
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
#include "../MarlinCore.h"
#include "../gcode/parser.h"
#include "../sd/cardreader.h"
#include "../module/temperature.h"
#include "../module/probe.h"

#include "../libs/buzzer.h"
#include "../feature/tmc_util.h"
#include "../module/stepper/indirection.h"
#include "../module/endstops.h"

#define WRITE_CFG(v) buffer +=v;



void writeSDConfig(void) {

    String buffer = {};
    card.openFileWrite("config.gcode");
      // C10                    ; Mark as a configuration file
    buffer = ";  Firmware Configuration File\n";
    WRITE_CFG( ";=====================================================\n");
    WRITE_CFG( "C10 \n");
    WRITE_CFG( "M117 Applying configuration\n");
    WRITE_CFG( ";-----------------------------------------------------\n");
    // reset settings to defaults
    WRITE_CFG( "M502\n");

    //
    // Announce current units, in case inches are being displayed
    //
    #if ENABLED(INCH_MODE_SUPPORT)
      WRITE_CFG("  G2"+ String(AS_DIGIT(parser.linear_unit_factor == 1.0))+ " ;");
    #else
      WRITE_CFG("  G21 ;");
    #endif
    WRITE_CFG(TERN_(INCH_MODE_SUPPORT, parser.linear_unit_factor != 1.0 ? " (in)\n" :) PSTR(" (mm)\n"))

    //
    // M92 Steps per Unit
    //
    WRITE_CFG("  M92 X")
    WRITE_CFG(String(LINEAR_UNIT(planner.settings.axis_steps_per_mm[X_AXIS])))
    WRITE_CFG(SP_Y_STR+ String(LINEAR_UNIT(planner.settings.axis_steps_per_mm[Y_AXIS])))
    WRITE_CFG(SP_Z_STR+ String(LINEAR_UNIT(planner.settings.axis_steps_per_mm[Z_AXIS]))+"\n")

    //
    // M203 Maximum feedrates (units/s)
    //
    WRITE_CFG(("  M203 X")+ String(LINEAR_UNIT(planner.settings.max_feedrate_mm_s[X_AXIS])));
    WRITE_CFG(SP_Y_STR+ String(LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Y_AXIS])));
    WRITE_CFG(SP_Z_STR+ String(LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Z_AXIS]))+"\n");

    //
    // M201 Maximum Acceleration (units/s2)
    //
    WRITE_CFG("  M201 X"+ String(LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[X_AXIS])));
    WRITE_CFG(SP_Y_STR+ String(LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Y_AXIS])));
    WRITE_CFG(SP_Z_STR+ String(LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Z_AXIS]))+"\n");

    //
    // M204 Acceleration (units/s2)
    //
    WRITE_CFG("  M204 P" + String(LINEAR_UNIT(planner.settings.acceleration)));
    WRITE_CFG(" R" + String(LINEAR_UNIT(planner.settings.retract_acceleration)));
    WRITE_CFG(SP_T_STR+ String(LINEAR_UNIT(planner.settings.travel_acceleration))+"\n");

    //
    // M205 "Advanced" Settings
    //
    WRITE_CFG("  M205 B"+ String(LINEAR_UNIT(planner.settings.min_segment_time_us)));
    WRITE_CFG(" S"+ String(LINEAR_UNIT(planner.settings.min_feedrate_mm_s)));
    WRITE_CFG(SP_T_STR+ String(LINEAR_UNIT(planner.settings.min_travel_feedrate_mm_s)));
    #if ENABLED(CLASSIC_JERK)
    WRITE_CFG(SP_X_STR+ String(LINEAR_UNIT(planner.max_jerk.x)));
    WRITE_CFG(SP_Y_STR+ String(LINEAR_UNIT(planner.max_jerk.y)));
    WRITE_CFG(SP_Z_STR+ String(LINEAR_UNIT(planner.max_jerk.z)));
    #endif
    WRITE_CFG("\n");
    //
    // M206 Home Offset
    //
    WRITE_CFG("  M206 X"+ String(LINEAR_UNIT(home_offset.x)));
    WRITE_CFG(SP_Y_STR+ String(LINEAR_UNIT(home_offset.y)));
    WRITE_CFG(SP_Z_STR+ String(LINEAR_UNIT(home_offset.z))+ "\n");

    //
    // M666 Endstops Adjustment
    //
    #if ANY(DELTA, HAS_EXTRA_ENDSTOPS)
    WRITE_CFG("  M666");
      #if ENABLED(X_DUAL_ENDSTOPS)
        WRITE_CFG(SP_X_STR+ String(LINEAR_UNIT(endstops.x2_endstop_adj)));
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        WRITE_CFG(SP_Y_STR+ String(LINEAR_UNIT(endstops.y2_endstop_adj)));
      #endif
      #if ENABLED(Z_MULTI_ENDSTOPS)
        WRITE_CFG(SP_Z_STR+ String(LINEAR_UNIT(endstops.z2_endstop_adj)));
      #endif
    WRITE_CFG("\n");
    #endif


    //
    // Probe Offset
    //
      WRITE_CFG("  M851 X"+ String(LINEAR_UNIT(probe.offset_xy.x)));
      WRITE_CFG(SP_Y_STR+ String(LINEAR_UNIT(probe.offset_xy.y)));
      WRITE_CFG(SP_Z_STR+ String(LINEAR_UNIT(probe.offset.z)) + "\n");

    #if HAS_TRINAMIC_CONFIG
      //
      // TMC Stepper driver current
      //
    WRITE_CFG("  M906");
    #if AXIS_IS_TMC(X)
      WRITE_CFG(SP_X_STR+ String( stepperX.getMilliamps()));
    #endif
    #if AXIS_IS_TMC(Y)
      WRITE_CFG(SP_Y_STR+ String( stepperY.getMilliamps()));
    #endif
    #if AXIS_IS_TMC(Z)
      WRITE_CFG(SP_Z_STR+ String( stepperZ.getMilliamps()));
    #endif
    SERIAL_ECHOPGM(" I1");
    #if AXIS_IS_TMC(X2)
      WRITE_CFG(SP_X_STR+ String(stepperX2.getMilliamps()));
    #endif
    #if AXIS_IS_TMC(Y2)
      WRITE_CFG(SP_Y_STR+ String(stepperY2.getMilliamps()));
    #endif
    #if AXIS_IS_TMC(Z2)
      WRITE_CFG(SP_Z_STR+ String(stepperZ2.getMilliamps()));
    #endif
    WRITE_CFG("\n");
  #endif

    //
    // TMC stepping mode
    //
    // SERIAL_ECHOPGM("  M569 S1");
    // if (etc) SERIAL_ECHO(C(' '), etc);
    // if (eol) SERIAL_EOL();

  // const bool chop_x = TERN0(X_HAS_STEALTHCHOP, stepperX.get_stored_stealthChop()),
            //  chop_y = TERN0(Y_HAS_STEALTHCHOP, stepperY.get_stored_stealthChop()),
            //  chop_z = TERN0(Z_HAS_STEALTHCHOP, stepperZ.get_stored_stealthChop()),
            //  chop_y2 = TERN0(Y2_HAS_STEALTHCHOP, stepperY2.get_stored_stealthChop()),
            //  chop_z2 = TERN0(Z2_HAS_STEALTHCHOP, stepperZ2.get_stored_stealthChop());


    // ; Save
    WRITE_CFG("M500\nG4S1\nM300P200\n");
    WRITE_CFG("M117 Configuration Applied\n");

    char charbuff[buffer.length()] = {0};
    buffer.toCharArray(charbuff,buffer.length());

    card.write(charbuff,buffer.length());
    card.closefile();
    DONE_BUZZ(true);
    ui.status_printf(0,F("Config GCode Stored"));

}

