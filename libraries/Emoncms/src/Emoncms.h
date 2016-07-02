/*
  Copyright (c) 2015 Giacomo Leonzi. All right reserved.

  ESP8266Emoncms - Library to pubblish data on Emoncsm.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
  */


#ifndef Emoncms_h
#define Emoncms_h

#include <Arduino.h>
#include <Client.h>


class Emoncms
{

  public:

  Emoncms(); //need ip address, port, APIKEY to write on emoncms
	void addValue(String, int); //need key, name of parameter measured, and its value
  void begin(String host, int port, String APIKEY,Client& _client);
  String publish();//pubblish

  private:
    String _jsonString;  // cookies forwarded by proxyssl (once per session)
    String _url;
    String _host;
    int _port;
    String _APIKEY;
    Client* client;
};

#endif
