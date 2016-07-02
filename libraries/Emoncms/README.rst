===========================================
Arduino IDE ESP8266 - Emoncms Library
===========================================

An Arduino IDE library around the Emoncms pubblishing for ESP8266.
By `GiacomoLeonzi, giacomoleonzi@gmail.com`_

=================
Table of contents
=================

- `Introduction`_

- `Status`_

  1. `Emoncms`_

  2. `Arduino IDE support`_

- `Getting the code`_

- `Installing`_

- `Getting started with Emoncms`_

  1. `Why`_

  2. `Create an account`_

  3. `Input`_

  3. `Feeds`_

- `License`_

===============
_`Introduction`
===============

This library provides some simple function to pubblish data on `Emoncms <https://emoncms.org>`_, written using Arduino IDE for ESP8266.
For all details about how to install and use Arduino IDE with ESP8266, refer to `Arduino core for ESP8266 WiFi chip <https://github.com/esp8266/Arduino>`_.

=========
_`Status`
=========

-------------------------
_`Emoncms`
-------------------------

To pubblish on Emoncms, you just need to do a get query like:
` http://<emoncms_url>/input/post?json={power:200}&apikey=<apikey>`_

**<emoncms_url>** is the url of the cms of OpenEnergyMonitor (to substitute with emoncms.org in case of cloud use).
**<apikey>** is the code of 32 character, that is univocal for each user.
Data are passed in json format.

You can try to put the url on the address bar of your browser, substituting the <> with the right parameters.
If the pubblication works fine you get "ok" on your browser!

-------------------------
_`Arduino IDE support`
-------------------------

ESP8266 is supported into Arduino IDE since version 1.6.5

===================
_`Getting the code`
===================

You can install Arduino IDE-Emoncms for your ESP8266 downloading the code that is hosted at https://github.com/giacomoleonzi/ESP8266Emoncms.
You can check out the latest development version being informed by updateds, selecting "Watch" box in the GitHub repository

=============
_`Installing`
=============

The downloaded code can be included as a new library into the IDE selecting the menù::

     Sketch / include Library / Add .Zip library

==================
_`Getting started with Emoncms`
==================
Inspired by a tutoriar proposed for `Arduino.cc <http://playground.arduino.cc/italiano/emoncms>`_ by **Pitusso**

------
_`Why`
------

Because is an open source system for monitor and menage domestic and industrial enviroment `<http://www.openenergymonitor.org>`_
You can also install your own server on a `localhost <http://openenergymonitor.org/emon/node/125>`_
This system can collect data and informations like temperature or consuption and show they with graph and graphics.

------
_`Create an account`
------

First of all we need to create an `account here <http://emoncms.org/>`_
After login we are redirect to our profile's page. On the left side we can se our personal informations and our **APIKEY**.
We need the **APIKEY** to write and read data on, without needs of login.

To post data **Write API Key** is needed, **Read API Key** is needed to read data from and to use they in exernal services or share they with other users.

-----------
_`Input`
-----------

In the **Input** section we can find all the kind of data pubblished trought our account or with ours **APIKEY**.
But the data are no logged, so we can see only the last value	inserted for each variable.

-----------
_`Feeds`
-----------

To log the data and have an history to create statistic, we need to associated the **Input** with a **Feed**.
**Feed** are usefull also to do some math and work with other data recorded.
First we create a **Log to Feed** to record data. Just click on the wrench on the right side of the data we want to store, and select the **Log to Feed** voice.
Now our data is stored and we can start to create statistics and rule to monitor our enviroment.

==========
_`License`
==========

You may copy, distribute and modify the software provided that modifications are described and licensed for free under `LGPL-3 <http://www.gnu.org/licenses/lgpl-3.0.html>`_. Derivatives works (including modifications or anything statically linked to the library) can only be redistributed under `LGPL-3 <http://www.gnu.org/licenses/lgpl-3.0.html>`_, but applications that use the library don't have to be.


- Add commands handler.
