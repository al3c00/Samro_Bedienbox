# Samro_Bedienbox

##Übersicht:
Dieses Repository beinhaltet PCB-Daten (Gerber-Dateien und ein KiCAD-Projekt) sowie den Quellcode für die Programmierung des verwendeten ATMega328.

##Hintergrund:
Dies ist eine Steuerungsbox, die die bestehende Steuerung eines Samro Offsett KK Kartoffelvollernters ergänzt. Die Standardbedienung der 20 Jahre alten Maschine weicht von modernern Bedienungslayout ab. Um mir als Fahrer, der bei einem Lohnunternehmer im Herbst etliche Stunden mit neuen Maschinen fährt, jeweils den Wechsel der Maschine zu erleichtern, wurde diese Bedienung konstruiert. Die Funktionsbelegung ähnelt der Grimme GBX 800, wie sie beim Lohnunternehmen Wyss-Wyss verwendet wird.

##Funktion:
Die Kontaktschalter der Samro-Bedienbox ziehen das Signal auf Masse. Die Hardware der Box erkennt dies und sendet ein entprechendes Signal an die Hauptplatine der Maschine. Die zusätzliche Bedienbox ergänzt diese FUnktionsweise. An die Signalpins der Kontaktschalter werden Kabel gelötet welche mit der zusätzlichen Box verbunden werden. 

##Software:
Es gibt kein seperater Ordner der für den Quellcode benutzt wird. Die .ino-Datei beinhaltet alle Daten.

##Hardware:
Im Hardware-Ordner finden sich die Schemas, Gerber-Dateien und das dazugehörige KiCAD-Projekt.
