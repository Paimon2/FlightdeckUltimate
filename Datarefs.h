#pragma once
#include "json.hpp"
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
extern nlohmann::json aircraftJson;

struct DatarefActionFloat {
	XPLMDataRef handle;
	float value;
};


struct DatarefActionInt {
	XPLMDataRef handle;
	int value;
};


namespace commands {
	extern XPLMCommandRef start;
	extern XPLMCommandRef thrust_rev_max_hold;
	extern XPLMCommandRef start_pushback;
	extern XPLMCommandRef stop_pushback;
	extern XPLMCommandRef start_1;
	extern XPLMCommandRef start_2;
	extern XPLMCommandRef start_3;
	extern XPLMCommandRef start_4;
	extern XPLMCommandRef shut_down;
	extern XPLMCommandRef shut_down_1;
	extern XPLMCommandRef shut_down_2;
	extern XPLMCommandRef shut_down_3;
	extern XPLMCommandRef shut_down_4;
	extern XPLMCommandRef carb_off;
	extern XPLMCommandRef carb_on;
	extern XPLMCommandRef apu_gen_off;
	extern XPLMCommandRef apu_gen_on;
	extern XPLMCommandRef apu_off;
	extern XPLMCommandRef apu_on;
	extern XPLMCommandRef apu_start;
	extern XPLMCommandRef avionics_off;
	extern XPLMCommandRef avionics_on;
	extern XPLMCommandRef gear_dn;
	extern XPLMCommandRef gear_toggle;
	extern XPLMCommandRef gear_up;
	extern XPLMCommandRef spdbrk_dn_one;
	extern XPLMCommandRef spdbrk_ex_full;
	extern XPLMCommandRef spdbrk_rt_full;
	extern XPLMCommandRef spdbrk_toggle;
	extern XPLMCommandRef spdbrk_up_one;
	extern XPLMCommandRef thrust_rev;

}

namespace datarefs {
	extern XPLMDataRef autopilot;
	extern XPLMDataRef cargo_door;
	extern XPLMDataRef fl_door;
	extern XPLMDataRef rl_door;
	extern XPLMDataRef fr_door;
	extern XPLMDataRef rr_door;
	extern XPLMDataRef autobrake_level;
	extern XPLMDataRef autopilot_spd;
	extern XPLMDataRef ap_speed;
	extern XPLMDataRef autopilot_hdg_hold;
	extern XPLMDataRef autopilot_hdg_sel;


	extern XPLMDataRef head_psi;
	extern XPLMDataRef altimeter;
	extern XPLMDataRef head_the;
	extern XPLMDataRef pitot_heat;
	extern XPLMDataRef pos_x;
	extern XPLMDataRef pos_y;
	extern XPLMDataRef pos_z;
	extern XPLMDataRef spd_intv;
	extern XPLMDataRef gpu;
	extern XPLMDataRef v1_speed;
	extern XPLMDataRef v2_speed;
	extern XPLMDataRef vr_speed;
	extern XPLMDataRef flaps;
	extern XPLMDataRef adf1;
	extern XPLMDataRef adf2;
	extern XPLMDataRef autopilot_alt;
	extern XPLMDataRef autopilot_autothrottle;
	extern XPLMDataRef autopilot_fd;
	extern XPLMDataRef autopilot_fpm;
	extern XPLMDataRef autopilot_hdg;
	extern XPLMDataRef autopilot_lnav;
	extern XPLMDataRef autopilot_loc;
	extern XPLMDataRef autopilot_vnav;
	extern XPLMDataRef beacon_lights;
	extern XPLMDataRef com1;
	extern XPLMDataRef com2;
	extern XPLMDataRef gear_deploy_array;
	extern XPLMDataRef instrument_brightness;
	extern XPLMDataRef landing_lights;
	extern XPLMDataRef nav_lights;
	extern XPLMDataRef no_smoking;
	extern XPLMDataRef seatbelt;
	extern XPLMDataRef speedbrake_ratio;
	extern XPLMDataRef strobe_lights;
	extern XPLMDataRef taxi_lights;
	extern XPLMDataRef thrust_lever;
	extern XPLMDataRef transponder_code;
	extern XPLMDataRef transponder_mode;
}


extern void initCommandsList();
extern void initDatarefsList();