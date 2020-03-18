**Introduction**

Ce GitHub est basé sur celui de Julien Rat : https://github.com/julienrat/captobox .
Le code a été fortement restructuré, et de nouveaux capteurs sont pris en charge.


**Capteurs pris en charge actuellement**

- M0D1023 (incluant BME280 et iAQ) :
	- MOD1023 : http://www.embeddedadventures.com/bme280_AMS-IAQ_multi_sensor_mod-1023.html
	- BME280 : https://www.bluedot.space/sensor-boards/bme280/
- BME680 : https://joy-it.net/en/products/SEN-BME680
- CCS811 : https://joy-it.net/en/products/SEN-CCS811V1


**Prise en main**

Dans le dossier "resources/prise en main", vous trouverez deux documents essentiels :
- "Description matos.pdf" : liste avec photos de l'ensemble du matériel constituant une Captobox.
- "Branchements captobox.pdf" : guide pour le branchement des différents capteurs sur le module cerveau.


**Note importante**

À l'heure actuelle, il est nécessaire de redémarrer la carte lorsqu'on change de capteur, pour que celui-ci soit détecté.
