#ifndef FOLDERMODE_H
#define FOLDERMODE_H

enum FolderMode {
    // 0 Kein Modus
    NO_MODE,
    // 1 Hörspielmodus: spielt eine zufällige Datei aus dem Ordner
    RANDOM_MODE,
    // 2 Album Modus: kompletten Ordner spielen
    ALBUM_MODE,
    // 3 Party Modus: Inhalt des Ordners in zufälliger Reihenfolge wiedergeben
    PARTY_MODE,
    // 4 Einzel Modus: eine Datei aus dem Ordner abspielen
    SINGLE_MODE,
    // 5 Hörbuch Modus: kompletten Ordner spielen und Fortschritt merken
    AUDIOBOOK_MODE,
    // 6 Adminfunktion: Macht aus der Karte eine Adminkarte. Der ausgewähöte
    // Ordner hat keine Bedeutung
    ADMIN_FUNCTIONS,
    // 7 Spezialmodus Von-Bin: Hörspiel: eine zufällige Datei aus dem Ordner
    SPECIAL_MODE_1,
    // 8 Spezialmodus Von-Bis: Album: alle Dateien zwischen Start und Ende
    // spielen
    SPECIAL_MODE_2,
    // 9 Spezialmodus Von-Bis: Party Ordner in zufälliger Reihenfolge
    SPECIAL_MODE_3,
    // 255 TODO check
    NO_IDEA_MODE
};

#endif