/*
	Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * app.c
 *
 *  Created on: 18 apr 2014
 *      Author: benjamin
 */

#include "app.h"
#include "ch.h"
#include "hal.h"
#include "hw.h"

// Private variables
static app_configuration appconf;

void app_init(app_configuration *conf) {
	appconf = *conf;

	switch (appconf.app_to_use) {
	case APP_PPM:
		app_ppm_configure(appconf.app_ppm_ctrl_type, appconf.app_ppm_pid_max_erpm,
				appconf.app_ppm_hyst, appconf.app_ppm_pulse_start, appconf.app_ppm_pulse_width,
				appconf.app_ppm_rpm_lim_start, appconf.app_ppm_rpm_lim_end);
		app_ppm_start();
		break;

	case APP_UART:
		hw_stop_i2c_uart();
		app_uartcomm_configure(appconf.app_uart_baudrate);
		app_uartcomm_start();
		break;

	case APP_PPM_UART:
		hw_stop_i2c_uart();
		app_ppm_configure(appconf.app_ppm_ctrl_type, appconf.app_ppm_pid_max_erpm,
				appconf.app_ppm_hyst, appconf.app_ppm_pulse_start, appconf.app_ppm_pulse_width,
				appconf.app_ppm_rpm_lim_start, appconf.app_ppm_rpm_lim_end);
		app_ppm_start();
		app_uartcomm_configure(appconf.app_uart_baudrate);
		app_uartcomm_start();
		break;

	case APP_NUNCHUK:
		app_nunchuk_configure(appconf.app_chuk_ctrl_type, appconf.app_chuk_hyst,
				appconf.app_chuk_rpm_lim_start, appconf.app_chuk_rpm_lim_end);
		app_nunchuk_start();
		break;

	case APP_CUSTOM:
#ifdef USE_APP_STEN
		hw_stop_i2c_uart();
		app_sten_init();
#endif
#ifdef USE_APP_GURGALOF
		app_gurgalof_init();
#endif
		break;

	default:
		break;
	}
}

const app_configuration* app_get_configuration(void) {
	return &appconf;
}

/**
 * Reconfigure all apps. Note that this will not start apps that are not already running, that
 * should be done at boot. Some apps don't have any configuration options.
 *
 * @param conf
 * The new configuration to use.
 */
void app_set_configuration(app_configuration *conf) {
	appconf = *conf;
	app_ppm_configure(appconf.app_ppm_ctrl_type, appconf.app_ppm_pid_max_erpm,
			appconf.app_ppm_hyst, appconf.app_ppm_pulse_start, appconf.app_ppm_pulse_width,
			appconf.app_ppm_rpm_lim_start, appconf.app_ppm_rpm_lim_end);
	app_uartcomm_configure(appconf.app_uart_baudrate);
	app_nunchuk_configure(appconf.app_chuk_ctrl_type, appconf.app_chuk_hyst,
			appconf.app_chuk_rpm_lim_start, appconf.app_chuk_rpm_lim_end);
}
