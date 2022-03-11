/*
 ESP910.cpp - ESP3D command class

 Copyright (c) 2014 Luc Lebosse. All rights reserved.

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with This code; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "../../include/esp3d_config.h"
#include "../commands.h"
#include "../esp3doutput.h"
#include "../settings_esp3d.h"
#include "../../modules/authentication/authentication_service.h"
//Get state / Set state of output message clients
//[ESP920]<SERIAL / SCREEN / REMOTE_SCREEN/ WEBSOCKET / TELNET /BT / ALL>=<ON/OFF>[pwd=<admin password>]
bool Commands::ESP920(const char* cmd_params, level_authenticate_type auth_type, ESP3DOutput * output)
{
    bool response = true;
    String parameter;
#ifdef AUTHENTICATION_FEATURE
    if (auth_type == LEVEL_GUEST) {
        output->printERROR("Wrong authentication!", 401);
        return false;
    }
#else
    (void)auth_type;
#endif //AUTHENTICATION_FEATURE
    parameter = get_param (cmd_params, "");
    //get
    if (parameter.length() == 0) {
        String s = "";
#if COMMUNICATION_PROTOCOL == RAW_SERIAL || COMMUNICATION_PROTOCOL == MKS_SERIAL || COMMUNICATION_PROTOCOL == SOCKET_SERIAL
        s += "SERIAL=";
        s += ESP3DOutput::isOutput(ESP_SERIAL_CLIENT)?"ON":"OFF";
#endif //COMMUNICATION_PROTOCOL == RAW_SERIAL || COMMUNICATION_PROTOCOL == MKS_SERIAL || COMMUNICATION_PROTOCOL == SOCKET_SERIAL
#if !defined(ESP3DLIB_ENV) || (defined (ESP3DLIB_ENV) && (HAS_DISPLAY || defined (HAS_SERIAL_DISPLAY)))
        s += " REMOTE_SCREEN=";
        s += ESP3DOutput::isOutput(ESP_REMOTE_SCREEN_CLIENT)?"ON":"OFF";
#endif //!defined(ESP3DLIB_ENV) || (defined (ESP3DLIB_ENV) && HAS_DISPLAY)
#ifdef DISPLAY_DEVICE
        s += " SCREEN=";
        s += ESP3DOutput::isOutput(ESP_SCREEN_CLIENT)?"ON":"OFF";
#endif //DISPLAY_DEVICE
#ifdef WS_DATA_FEATURE
        s += " WEBSOCKET=";
        s += ESP3DOutput::isOutput(ESP_WEBSOCKET_CLIENT)?"ON":"OFF";
#endif //WS_DATA_FEATURE
#ifdef BLUETOOTH_FEATURE
        s += " BT=";
        s += ESP3DOutput::isOutput(ESP_BT_CLIENT)?"ON":"OFF";
#endif //BLUETOOTH_FEATURE
#ifdef TELNET_FEATURE
        s += " TELNET=";
        s += ESP3DOutput::isOutput(ESP_TELNET_CLIENT)?"ON":"OFF";
#endif //TELNET_FEATURE
        output->printMSG(s.c_str());
        return true;

    } else { //set
        response = false;
#if COMMUNICATION_PROTOCOL == RAW_SERIAL || COMMUNICATION_PROTOCOL == MKS_SERIAL || COMMUNICATION_PROTOCOL == SOCKET_SERIAL
        parameter = get_param (cmd_params, "SERIAL=");
        if (parameter.length() != 0) {
            if ((parameter == "ON")|| (parameter == "OFF")) {
                if (!Settings_ESP3D::write_byte (ESP_SERIAL_FLAG, (parameter == "ON")?1:0)) {
                    output->printERROR ("Set failed!");
                    return false;
                }
            } else {
                output->printERROR ("Incorrect command!");
                return false;
            }
            response = true;
        }
#endif //COMMUNICATION_PROTOCOL == RAW_SERIAL || COMMUNICATION_PROTOCOL == MKS_SERIAL || COMMUNICATION_PROTOCOL == SOCKET_SERIAL
#if !defined(ESP3DLIB_ENV) || (defined (ESP3DLIB_ENV) && (HAS_DISPLAY || defined (HAS_SERIAL_DISPLAY)))
        parameter = get_param (cmd_params, "REMOTE_SCREEN=");
        if (parameter.length() != 0) {
            if ((parameter == "ON")|| (parameter == "OFF")) {
                if (!Settings_ESP3D::write_byte (ESP_REMOTE_SCREEN_FLAG, (parameter == "ON")?1:0)) {
                    output->printERROR ("Set failed!");
                    return false;
                }
            } else {
                output->printERROR ("Incorrect command!");
                return false;
            }
            response = true;
        }
#endif //!defined(ESP3DLIB_ENV) || (defined (ESP3DLIB_ENV) && HAS_DISPLAY)
        parameter = get_param (cmd_params, "ALL=");
        if (parameter.length() != 0) {
            if ((parameter == "ON")|| (parameter == "OFF")) {
                if (
#if COMMUNICATION_PROTOCOL == RAW_SERIAL || COMMUNICATION_PROTOCOL == MKS_SERIAL || COMMUNICATION_PROTOCOL == SOCKET_SERIAL
                    !Settings_ESP3D::write_byte (ESP_SERIAL_FLAG, (parameter == "ON")?1:0)||
#endif //COMMUNICATION_PROTOCOL == RAW_SERIAL || COMMUNICATION_PROTOCOL == MKS_SERIAL || COMMUNICATION_PROTOCOL == SOCKET_SERIAL
#ifdef DISPLAY_DEVICE
                    !Settings_ESP3D::write_byte (ESP_SCREEN_FLAG, (parameter == "ON")?1:0)||
#endif //DISPLAY_DEVICE
#ifdef WS_DATA_FEATURE
                    !Settings_ESP3D::write_byte (ESP_WEBSOCKET_FLAG, (parameter == "ON")?1:0)||
#endif //WS_DATA_FEATURE
#ifdef BLUETOOTH_FEATURE
                    !Settings_ESP3D::write_byte (ESP_BT_FLAG, (parameter == "ON")?1:0)||
#endif //BLUETOOTH_FEATURE
#ifdef TELNET_FEATURE
                    !Settings_ESP3D::write_byte (ESP_TELNET_FLAG, (parameter == "ON")?1:0)||
#endif //TELNET_FEATURE
                    !Settings_ESP3D::write_byte (ESP_REMOTE_SCREEN_FLAG, (parameter == "ON")?1:0)) {
                    output->printERROR ("Set failed!");
                    return false;
                }
            } else {
                output->printERROR ("Incorrect command!");
                return false;
            }
            response = true;
        }
#ifdef DISPLAY_DEVICE
        parameter = get_param (cmd_params, "SCREEN=");
        if (parameter.length() != 0) {
            if ((parameter == "ON")|| (parameter == "OFF")) {
                if (!Settings_ESP3D::write_byte (ESP_SCREEN_FLAG, (parameter == "ON")?1:0)) {
                    output->printERROR ("Set failed!");
                    return false;
                }
            } else {
                output->printERROR ("Incorrect command!");
                return false;
            }
            response = true;
        }
#endif //DISPLAY_DEVICE
#ifdef WS_DATA_FEATURE
        parameter = get_param (cmd_params, "WEBSOCKET=");
        if (parameter.length() != 0) {
            if ((parameter == "ON")|| (parameter == "OFF")) {
                if (!Settings_ESP3D::write_byte (ESP_WEBSOCKET_FLAG, (parameter == "ON")?1:0)) {
                    output->printERROR ("Set failed!");
                    return false;
                }
            } else {
                output->printERROR ("Incorrect command!");
                return false;
            }
            response = true;
        }
#endif //WS_DATA_FEATURE
#ifdef BLUETOOTH_FEATURE
        parameter = get_param (cmd_params, "BT=");
        if (parameter.length() != 0) {
            if ((parameter == "ON")|| (parameter == "OFF")) {
                if (!Settings_ESP3D::write_byte (ESP_BT_FLAG, (parameter == "ON")?1:0)) {
                    output->printERROR ("Set failed!");
                    return false;
                }
            } else {
                output->printERROR ("Incorrect command!");
                return false;
            }
            response = true;
        }
#endif //BLUETOOTH_FEATURE
#ifdef TELNET_FEATURE
        parameter = get_param (cmd_params, "TELNET=");
        if (parameter.length() != 0) {
            if ((parameter == "ON")|| (parameter == "OFF")) {
                if (!Settings_ESP3D::write_byte (ESP_TELNET_FLAG, (parameter == "ON")?1:0)) {
                    output->printERROR ("Set failed!");
                    return false;
                }
            } else {
                output->printERROR ("Incorrect command!");
                return false;
            }
            response = true;
        }
#endif //TELNET_FEATURE
        //all ok we do the hot change
        if(response) {
            ESP3DOutput::isOutput(ESP_ALL_CLIENTS,true);
            output->printMSG ("ok");
            return true;
        }

    }
    output->printERROR ("Incorrect command!");
    return false;
}