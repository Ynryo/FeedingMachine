# **Feeding Machine 🐟🤖**

Un distributeur de nourriture connecté et autonome pour que tes poissons (ou ton chat) ne meurent pas de faim quand tu es en cours. Fini de culpabiliser parce que t'as oublié le repas de 19h \!

## **✨ Fonctionnalités**

* 🕰️ **Précision Atomique** : Synchronisation via NTP (Internet) pour servir le repas à **19h00 pétantes**, pas une seconde de plus.  
* 💡 **Ambiance Lumineuse & Cycle Jour/Nuit** :  
  * **Mode Jour** : Luminosité à 75% entre 7h et 22h.  
  * **Mode Nuit** : Luminosité réduite à 25% pour ne pas éblouir les poissons qui dorment.  
  * **Mode Aqua** : Animation de couleurs aléatoires quand il reste de la nourriture.  
* 🥡 **Gestion des Stocks** :  
  * Système de 16 emplacements rotatifs.  
  * Décrémentation automatique du stock.  
  * Alerte visuelle (clignotement) quand c'est vide.  
* 📡 **Connectivité Robuste** :  
  * Tentatives de reconnexion WiFi automatiques avec feedback visuel sur les LEDs.

## **🛠️ Stack Technique**

* **Framework** : Arduino sur PlatformIO.  
* **Hardware** :  
  * 🧠 **ESP8266** (Wemos D1 Mini Lite).  
  * ⚙️ **Moteur** : Stepper (28BYJ-48) pour la rotation du barillet.  
  * 🌈 **Lumière** : Ruban LED (NeoPixel/WS2812B) géré via FastLED.  
* **Bibliothèques clés** :  
  * `NTPClient` : Pour récupérer l'heure exacte.  
  * `FastLED` & `Adafruit NeoPixel` : Pour le show lumineux.  
  * `Stepper` : Pour la mécanique de distribution.  
  * `ESP8266WiFi` : Pour parler au monde extérieur.

## **🛠️ Configuration & Installation**

### **Prérequis**

* VSCode avec l'extension **PlatformIO**.  
* Un ESP8266 et un peu de câblage.

### **Fichier Credentials**

Le projet ne contient pas tes mots de passe (sécurité d'abord \!). Tu dois créer un fichier `include/credentials.h` à la racine du dossier `include` et y ajouter :

```
#ifndef CREDENTIALS_H  
#define CREDENTIALS_H

const char* ssid = "TON_NOM_WIFI";  
const char* password = "TON_MOT_DE_PASSE";

#endif
```

### **Dépendances**

Tout est géré automatiquement par platformio.ini, mais voici les stars :

`arduino-libraries/Stepper@^1.1.3`

`arduino-libraries/NTPClient@^3.2.1`

`fastled/FastLED@^3.9.4`

## **📂 Structure du Projet**

* `src/main.cpp` : Le cerveau. Gère la boucle principale, la connexion WiFi, la récupération de l'heure NTP et déclenche le moteur à l'heure du repas.  
* `src/functions.h` : Les mathématiques de la lumière. Contient bright360to255 pour gérer les conversions de couleurs et les fonctions d'affichage LED.  
* `platformio.ini` : Le chef d'orchestre de la compilation et des librairies.

## **📝 À propos**

Développé par un étudiant qui voulait s'assurer que ses animaux survivent à ses périodes de révisions intenses. 🐠