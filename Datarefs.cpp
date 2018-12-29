#include "json.hpp"
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <chrono>
#include <thread>
#include <string>

#ifdef and
#undef and
#endif

nlohmann::json aircraftJson;


void doCommandWithDelayInternal(XPLMCommandRef ref, int seconds) {
	XPLMCommandBegin(ref);
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
	XPLMCommandEnd(ref);
}

void doCommandWithDelay(XPLMCommandRef ref, int seconds) {
	std::thread delayCmdThread(doCommandWithDelayInternal, ref, seconds);
	delayCmdThread.detach();
}

namespace commands {
	XPLMCommandRef start;
	XPLMCommandRef thrust_rev_max_hold;
	XPLMCommandRef start_pushback;
	XPLMCommandRef stop_pushback;
	XPLMCommandRef start_1;
	XPLMCommandRef start_2;
	XPLMCommandRef start_3;
	XPLMCommandRef start_4;
	XPLMCommandRef shut_down;
	XPLMCommandRef shut_down_1;
	XPLMCommandRef shut_down_2;
	XPLMCommandRef shut_down_3;
	XPLMCommandRef shut_down_4;
	XPLMCommandRef carb_off;
	XPLMCommandRef carb_on;
	XPLMCommandRef apu_gen_off;
	XPLMCommandRef apu_gen_on;
	XPLMCommandRef apu_off;
	XPLMCommandRef apu_on;
	XPLMCommandRef apu_start;
	XPLMCommandRef avionics_off;
	XPLMCommandRef avionics_on;
	XPLMCommandRef gear_dn;
	XPLMCommandRef flaps_dn;
	XPLMCommandRef flaps_up;
	XPLMCommandRef gear_toggle;
	XPLMCommandRef gear_up;
	XPLMCommandRef spdbrk_dn_one;
	XPLMCommandRef spdbrk_ex_full;
	XPLMCommandRef spdbrk_rt_full;
	XPLMCommandRef spdbrk_toggle;
	XPLMCommandRef spdbrk_up_one;
	XPLMCommandRef thrust_rev;

}

namespace datarefs {
	XPLMDataRef autopilot;
	XPLMDataRef cargo_door;
	XPLMDataRef fl_door;
	XPLMDataRef rl_door;
	XPLMDataRef fr_door;
	XPLMDataRef rr_door;
	XPLMDataRef autobrake_level;
	XPLMDataRef autopilot_spd;
	XPLMDataRef ap_speed;
	XPLMDataRef autopilot_hdg_hold;
	XPLMDataRef autopilot_hdg_sel;

	XPLMDataRef head_psi;
	XPLMDataRef altimeter;
	XPLMDataRef head_the;
	XPLMDataRef pitot_heat;
	XPLMDataRef pos_x;
	XPLMDataRef pos_y;
	XPLMDataRef pos_z;
	XPLMDataRef spd_intv;
	XPLMDataRef gpu;
	XPLMDataRef v1_speed;
	XPLMDataRef v2_speed;
	XPLMDataRef vr_speed;
	XPLMDataRef flaps;
	XPLMDataRef adf1;
	XPLMDataRef adf2;
	XPLMDataRef autopilot_alt;
	XPLMDataRef autopilot_autothrottle;
	XPLMDataRef autopilot_fd;
	XPLMDataRef autopilot_fpm;
	XPLMDataRef autopilot_hdg;
	XPLMDataRef autopilot_lnav;
	XPLMDataRef autopilot_loc;
	XPLMDataRef autopilot_vnav;
	XPLMDataRef beacon_lights;
	XPLMDataRef com1;
	XPLMDataRef com2;
	XPLMDataRef gear_deploy_array;
	XPLMDataRef instrument_brightness;
	XPLMDataRef landing_lights;
	XPLMDataRef nav_lights;
	XPLMDataRef no_smoking;
	XPLMDataRef seatbelt;
	XPLMDataRef speedbrake_ratio;
	XPLMDataRef strobe_lights;
	XPLMDataRef taxi_lights;
	XPLMDataRef thrust_lever;
	XPLMDataRef transponder_code;
	XPLMDataRef transponder_mode;
}


#pragma region Huge list of ref and command settings. Regex came to the rescue!
void initCommandsList() {
	std::string thrust_rev_max_hold_strname = aircraftJson["commands"]["thrust_rev_max_hold"];
	std::string start_pushback_strname = aircraftJson["commands"]["start_pushback"];
	std::string stop_pushback_strname = aircraftJson["commands"]["stop_pushback"];
	std::string start_strname = aircraftJson["commands"]["start"];
	std::string start_1_strname = aircraftJson["commands"]["start_1"];
	std::string start_2_strname = aircraftJson["commands"]["start_2"];
	std::string start_3_strname = aircraftJson["commands"]["start_3"];
	std::string start_4_strname = aircraftJson["commands"]["start_4"];
	std::string shut_down_strname = aircraftJson["commands"]["shut_down"];
	std::string shut_down_1_strname = aircraftJson["commands"]["shut_down_1"];
	std::string shut_down_2_strname = aircraftJson["commands"]["shut_down_2"];
	std::string shut_down_3_strname = aircraftJson["commands"]["shut_down_3"];
	std::string shut_down_4_strname = aircraftJson["commands"]["shut_down_4"];
	std::string carb_off_strname = aircraftJson["commands"]["carb_off"];
	std::string carb_on_strname = aircraftJson["commands"]["carb_on"];
	std::string apu_gen_off_strname = aircraftJson["commands"]["apu_gen_off"];
	std::string apu_gen_on_strname = aircraftJson["commands"]["apu_gen_on"];
	std::string apu_off_strname = aircraftJson["commands"]["apu_off"];
	std::string apu_on_strname = aircraftJson["commands"]["apu_on"];
	std::string apu_start_strname = aircraftJson["commands"]["apu_start"];
	std::string avionics_off_strname = aircraftJson["commands"]["avionics_off"];
	std::string avionics_on_strname = aircraftJson["commands"]["avionics_on"];
	std::string flaps_up_strname = aircraftJson["commands"]["flaps_up"];
	std::string flaps_dn_strname = aircraftJson["commands"]["flaps_dn"];
	std::string gear_dn_strname = aircraftJson["commands"]["gear_dn"];
	std::string gear_toggle_strname = aircraftJson["commands"]["gear_toggle"];
	std::string gear_up_strname = aircraftJson["commands"]["gear_up"];
	std::string spdbrk_dn_one_strname = aircraftJson["commands"]["spdbrk_dn_one"];
	std::string spdbrk_ex_full_strname = aircraftJson["commands"]["spdbrk_ex_full"];
	std::string spdbrk_rt_full_strname = aircraftJson["commands"]["spdbrk_rt_full"];
	std::string spdbrk_toggle_strname = aircraftJson["commands"]["spdbrk_toggle"];
	std::string spdbrk_up_one_strname = aircraftJson["commands"]["spdbrk_up_one"];
	std::string thrust_rev_strname = aircraftJson["commands"]["thrust_rev"];


	commands::thrust_rev_max_hold = XPLMFindCommand(thrust_rev_max_hold_strname.c_str());
	commands::start_pushback = XPLMFindCommand(start_pushback_strname.c_str());
	commands::stop_pushback = XPLMFindCommand(stop_pushback_strname.c_str());
	commands::start = XPLMFindCommand(start_strname.c_str());
	commands::start_1 = XPLMFindCommand(start_1_strname.c_str());
	commands::start_2 = XPLMFindCommand(start_2_strname.c_str());
	commands::start_3 = XPLMFindCommand(start_3_strname.c_str());
	commands::start_4 = XPLMFindCommand(start_4_strname.c_str());
	commands::shut_down = XPLMFindCommand(shut_down_strname.c_str());
	commands::shut_down_1 = XPLMFindCommand(shut_down_1_strname.c_str());
	commands::shut_down_2 = XPLMFindCommand(shut_down_2_strname.c_str());
	commands::shut_down_3 = XPLMFindCommand(shut_down_3_strname.c_str());
	commands::shut_down_4 = XPLMFindCommand(shut_down_4_strname.c_str());
	commands::carb_off = XPLMFindCommand(carb_off_strname.c_str());
	commands::carb_on = XPLMFindCommand(carb_on_strname.c_str());
	commands::apu_gen_off = XPLMFindCommand(apu_gen_off_strname.c_str());
	commands::apu_gen_on = XPLMFindCommand(apu_gen_on_strname.c_str());
	commands::apu_off = XPLMFindCommand(apu_off_strname.c_str());
	commands::apu_on = XPLMFindCommand(apu_on_strname.c_str());
	commands::apu_start = XPLMFindCommand(apu_start_strname.c_str());
	commands::avionics_off = XPLMFindCommand(avionics_off_strname.c_str());
	commands::avionics_on = XPLMFindCommand(avionics_on_strname.c_str());
	commands::gear_dn = XPLMFindCommand(gear_dn_strname.c_str());
	commands::flaps_dn = XPLMFindCommand(flaps_dn_strname.c_str());
	commands::flaps_up = XPLMFindCommand(flaps_up_strname.c_str());
	commands::gear_toggle = XPLMFindCommand(gear_toggle_strname.c_str());
	commands::gear_up = XPLMFindCommand(gear_up_strname.c_str());
	commands::spdbrk_dn_one = XPLMFindCommand(spdbrk_dn_one_strname.c_str());
	commands::spdbrk_ex_full = XPLMFindCommand(spdbrk_ex_full_strname.c_str());
	commands::spdbrk_rt_full = XPLMFindCommand(spdbrk_rt_full_strname.c_str());
	commands::spdbrk_toggle = XPLMFindCommand(spdbrk_toggle_strname.c_str());
	commands::spdbrk_up_one = XPLMFindCommand(spdbrk_up_one_strname.c_str());
	commands::thrust_rev = XPLMFindCommand(thrust_rev_strname.c_str());

	

}

void initDatarefsList() {

	datarefs::pos_x = XPLMFindDataRef("sim/aircraft/view/acf_peX");
	datarefs::pos_y = XPLMFindDataRef("sim/aircraft/view/acf_peY");
	datarefs::pos_z = XPLMFindDataRef("sim/aircraft/view/acf_peZ");
	datarefs::head_the = XPLMFindDataRef("sim/graphics/view/pilots_head_the");
	datarefs::head_psi = XPLMFindDataRef("sim/graphics/view/pilots_head_psi");

	std::string autopilot_strname = aircraftJson["datarefs"]["autopilot"];
	std::string cargo_door_strname = aircraftJson["datarefs"]["cargo_door"];
	std::string fl_door_strname = aircraftJson["datarefs"]["fl_door"];
	std::string rl_door_strname = aircraftJson["datarefs"]["rl_door"];
	std::string fr_door_strname = aircraftJson["datarefs"]["fr_door"];
	std::string rr_door_strname = aircraftJson["datarefs"]["rr_door"];
	std::string autobrake_level_strname = aircraftJson["datarefs"]["autobrake_level"];
	std::string autopilot_spd_strname = aircraftJson["datarefs"]["autopilot_spd"];
	std::string ap_speed_strname = aircraftJson["datarefs"]["ap_speed"];
	std::string autopilot_hdg_hold_strname = aircraftJson["datarefs"]["autopilot_hdg_hold"];
	std::string autopilot_hdg_sel_strname = aircraftJson["datarefs"]["autopilot_hdg_sel"];

	std::string spd_intv_strname = aircraftJson["datarefs"]["spd_intv"];
	std::string gpu_strname = aircraftJson["datarefs"]["gpu"];
	std::string flaps_strname = aircraftJson["datarefs"]["flaps"];
	std::string adf1_strname = aircraftJson["datarefs"]["adf1"];
	std::string adf2_strname = aircraftJson["datarefs"]["adf2"];
	std::string v1_speed_strname = aircraftJson["datarefs"]["V1_speed"];
	std::string v2_speed_strname = aircraftJson["datarefs"]["V2_speed"];
	std::string vr_speed_strname = aircraftJson["datarefs"]["VR_speed"];
	std::string autopilot_alt_strname = aircraftJson["datarefs"]["autopilot_alt"];
	std::string autopilot_autothrottle_strname = aircraftJson["datarefs"]["autopilot_autothrottle"];
	std::string autopilot_fd_strname = aircraftJson["datarefs"]["autopilot_fd"];
	std::string autopilot_fpm_strname = aircraftJson["datarefs"]["autopilot_fpm"];
	std::string autopilot_hdg_strname = aircraftJson["datarefs"]["autopilot_hdg"];
	std::string autopilot_lnav_strname = aircraftJson["datarefs"]["autopilot_lnav"];
	std::string autopilot_loc_strname = aircraftJson["datarefs"]["autopilot_loc"];
	std::string autopilot_vnav_strname = aircraftJson["datarefs"]["autopilot_vnav"];
	std::string beacon_lights_strname = aircraftJson["datarefs"]["beacon_lights"];
	std::string com1_strname = aircraftJson["datarefs"]["com1"];
	std::string com2_strname = aircraftJson["datarefs"]["com2"];
	std::string gear_deploy_array_strname = aircraftJson["datarefs"]["gear_deploy_array"];
	std::string instrument_brightness_strname = aircraftJson["datarefs"]["instrument_brightness"];
	std::string landing_lights_strname = aircraftJson["datarefs"]["landing_lights"];
	std::string nav_lights_strname = aircraftJson["datarefs"]["nav_lights"];
	std::string no_smoking_strname = aircraftJson["datarefs"]["no_smoking"];
	std::string seatbelt_strname = aircraftJson["datarefs"]["seatbelt"];
	std::string speedbrake_ratio_strname = aircraftJson["datarefs"]["speedbrake_ratio"];
	std::string strobe_lights_strname = aircraftJson["datarefs"]["strobe_lights"];
	std::string taxi_lights_strname = aircraftJson["datarefs"]["taxi_lights"];
	std::string thrust_lever_strname = aircraftJson["datarefs"]["thrust_lever"];
	std::string transponder_code_strname = aircraftJson["datarefs"]["transponder_code"];
	std::string transponder_mode_strname = aircraftJson["datarefs"]["transponder_mode"];

	datarefs::autopilot = XPLMFindDataRef(autopilot_strname.c_str());
	datarefs::cargo_door = XPLMFindDataRef(cargo_door_strname.c_str());
	datarefs::fl_door = XPLMFindDataRef(fl_door_strname.c_str());
	datarefs::rl_door = XPLMFindDataRef(rl_door_strname.c_str());
	datarefs::fr_door = XPLMFindDataRef(fr_door_strname.c_str());
	datarefs::rr_door = XPLMFindDataRef(rr_door_strname.c_str());
	datarefs::autobrake_level = XPLMFindDataRef(autobrake_level_strname.c_str());
	datarefs::autopilot_spd = XPLMFindDataRef(autopilot_spd_strname.c_str());
	datarefs::ap_speed = XPLMFindDataRef(ap_speed_strname.c_str());
	datarefs::autopilot_hdg_hold = XPLMFindDataRef(autopilot_hdg_hold_strname.c_str());
	datarefs::autopilot_hdg_sel = XPLMFindDataRef(autopilot_hdg_sel_strname.c_str());
	datarefs::spd_intv = XPLMFindDataRef(spd_intv_strname.c_str());
	datarefs::gpu = XPLMFindDataRef(gpu_strname.c_str());
	datarefs::v1_speed = XPLMFindDataRef(v1_speed_strname.c_str());
	datarefs::v2_speed = XPLMFindDataRef(v2_speed_strname.c_str());
	datarefs::vr_speed = XPLMFindDataRef(vr_speed_strname.c_str());
	datarefs::flaps = XPLMFindDataRef(flaps_strname.c_str());
	datarefs::adf1 = XPLMFindDataRef(adf1_strname.c_str());
	datarefs::adf2 = XPLMFindDataRef(adf2_strname.c_str());
	datarefs::autopilot_alt = XPLMFindDataRef(autopilot_alt_strname.c_str());
	datarefs::autopilot_autothrottle = XPLMFindDataRef(autopilot_autothrottle_strname.c_str());
	datarefs::autopilot_fd = XPLMFindDataRef(autopilot_fd_strname.c_str());
	datarefs::autopilot_fpm = XPLMFindDataRef(autopilot_fpm_strname.c_str());
	datarefs::autopilot_hdg = XPLMFindDataRef(autopilot_hdg_strname.c_str());
	datarefs::autopilot_lnav = XPLMFindDataRef(autopilot_lnav_strname.c_str());
	datarefs::autopilot_loc = XPLMFindDataRef(autopilot_loc_strname.c_str());
	datarefs::autopilot_vnav = XPLMFindDataRef(autopilot_vnav_strname.c_str());
	datarefs::beacon_lights = XPLMFindDataRef(beacon_lights_strname.c_str());
	datarefs::com1 = XPLMFindDataRef(com1_strname.c_str());
	datarefs::com2 = XPLMFindDataRef(com2_strname.c_str());
	datarefs::gear_deploy_array = XPLMFindDataRef(gear_deploy_array_strname.c_str());
	datarefs::instrument_brightness = XPLMFindDataRef(instrument_brightness_strname.c_str());
	datarefs::landing_lights = XPLMFindDataRef(landing_lights_strname.c_str());
	datarefs::nav_lights = XPLMFindDataRef(nav_lights_strname.c_str());
	datarefs::no_smoking = XPLMFindDataRef(no_smoking_strname.c_str());
	datarefs::seatbelt = XPLMFindDataRef(seatbelt_strname.c_str());
	datarefs::speedbrake_ratio = XPLMFindDataRef(speedbrake_ratio_strname.c_str());
	datarefs::strobe_lights = XPLMFindDataRef(strobe_lights_strname.c_str());
	datarefs::taxi_lights = XPLMFindDataRef(taxi_lights_strname.c_str());
	datarefs::thrust_lever = XPLMFindDataRef(thrust_lever_strname.c_str());
	datarefs::transponder_code = XPLMFindDataRef(transponder_code_strname.c_str());
	datarefs::transponder_mode = XPLMFindDataRef(transponder_mode_strname.c_str());
}
#pragma endregion
