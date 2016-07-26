# RuralHack

A place to store all the code and information for the RuralHack experimentations held during several workshops held in the summer 2016 in Italy and Mediterranean Areas by Officine Arduino and Ruralhub.

# The Kit

The list of components used in the workshop is - indeed - a kit, which hopefully will be sold on the Genuino/Arduino Store soon.

The Kit (designed to be used with a [Genuno MKR1000](https://store.arduino.cc/product/GBX00004)) features:

- Humidity Sensor (DHT22)
- Log-scale Analog Light Sensor ([Ga1a12s202 in its Adafruit's Flavour](https://www.adafruit.com/product/1384))
- [Soil Moisture Sensor](https://www.tindie.com/products/miceuz/i2c-soil-moisture-sensor) (aka the slim chirp) from [Miceuz](https://www.tindie.com/stores/miceuz/)
- [Grove Relay Module](http://seeedstudio.com/depot/Grove-Relay-p-769.html) from Seeed
- Resistors / pushbuttons / connectors

![RuralHack Kit](https://db.tt/KzGpAfxR)

# The workshop

We've done this workshop twice in a month, and I guess we'll repeat this soon. The point of a workshop is giving people access to new technologies.
Yet sometime it's not possible to access such information, or education, for logistic or economical reasons.

We have released all the teaching material for you to toy around the kit *indipendently*.

Most codes are not by us, in some cases adaptations of somebodyelse's code and projects, we tried to create diagrams of some specific setups using [Fritzing](fritzing.org/).

The basic idea behind the workshop was create some sort of automatization inside of a rural scenario, with wifi connectivity and not a large area to look after. We started little to grow big.

![RuralHack Kit](https://c1.staticflickr.com/8/7404/27509295904_5642902e38_c.jpg)
([Watch the entire Flickr Ambum on RuralHack](https://www.flickr.com/photos/142132017@N03/albums/72157670025633602) by [Ruralhub](ruralhub.it/en/))


Besides the Arduino Alphabet section, we covered some sensor-specific presentation, while we deepened the knowledge of several services, such as
- [IFTT](IFTTt.com), basically following [this tutorial](  https://create.arduino.cc/projecthub/Arduino_Genuino/if-this-then-cat-3a64b6
).
- [OpenWeatherMap](openweathermap.org)
- [Build a Telegram Bot](https://create.arduino.cc/projecthub/Arduino_Genuino/telegram-bot-library-ced4d4)
- host dashboards and data locally using [emonpi](https://github.com/openenergymonitor/emonpi) (amazing project) using [this library](https://github.com/OfficineArduinoTorino/RuralHack/tree/master/libraries/Emoncms) made by Giacomo Leonzi and edited by Lorenzo Romagnoli
