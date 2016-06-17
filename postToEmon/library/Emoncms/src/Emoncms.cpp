
/*
Copyright (c) 2015 Giacomo Leonzi. All right reserved.

  Emoncms - Library to pubblish data on Emoncsm.org

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


#include "Emoncms.h"

Emoncms::Emoncms()	{

}

void Emoncms::begin(String host, int port, String APIKEY,Client& _client)	{
	_host=host;
	_port=port;
	_APIKEY=APIKEY;
	_jsonString ="";
	client = &_client;
}

/*************************************************************************************
* addValue - function to build the string json format to comunicate with emoncms.org*
* you need to give a name value colled key and it's value in a int variable				 *
************************************************************************************/

void Emoncms::addValue(String key, int value)  {
	if(_jsonString!="")_jsonString +=",";//add the comma only when is not the first or only data added
	_jsonString += key;
	_jsonString += ":";
	_jsonString += value;
}

/*************************************************************************************
* pubblish - function to pubblish on emoncms.org the data measured                  *
* this function build the string to send, open the comunication through the server  *
* and send all the data.
************************************************************************************/

String Emoncms::publish()  {

	String response;

	//begin to build the string to send
	String _url="POST /input/post.json?node=1&json={";
	_url+=_jsonString;
	_url+="}&apikey=";
	_url+=String(_APIKEY);

	//ended to build the string to send
	_jsonString=""; //clear the string with data ready for the next lecture


	if (client->connect(_host.c_str(), _port)) {
		client->println(_url);
		Serial.print("Data sent");
		unsigned long timeToRead = millis();
		while (millis() - timeToRead < 1000) {
			while (client->available()) {
				response = client->readStringUntil('\r');
			}
		}
	}
	else Serial.print("Connection failed!");
	return response;
}
