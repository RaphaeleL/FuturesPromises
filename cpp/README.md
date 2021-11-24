# Futures and Promises in C++

Futures und Promises und deren Umsetzung in C++

<!--<img src="assets/../../assets/cpp.png"
     alt="C++ Logo"
     style="width:200px;height:200px;display:block; margin-left:auto; margin-right:auto;"/>-->

> Diese Arbeit wurde von Raphaele Salvatore Licciardo im Rahmen der Modellbasierte Softwareentwicklung Vorlesung an der Hochschule Karlruhe - Technik und Wirtschaft im SS 2021 durchgeführt.  

C++ ist eine Programmiersprache von Bjarne Stroustrup aus 1979 als eine Erweiterung für die Programmiersprache C. C++ ermöglicht sowohl die effiziente und maschinennahe Programmierung als auch eine Programmierung auf hohem Abstraktionsniveau. Der Standard definiert auch eine Standardbibliothek, zu der verschiedene Implementierungen existieren. Eine davon ist die `<future>` Bibliothek, welche einen Mechanismus bietet, um auf das Ergebnis von asynchronen Operationen zuzugreifen.
## Inhaltsverzeichnis

- [Futures and Promises in C++](#futures-and-promises-in-c)
  - [Inhaltsverzeichnis](#inhaltsverzeichnis)
  - [Beispiele](#beispiele)
    - [Future](#future)
    - [Shared Future](#shared-future)
    - [Fire and Forget](#fire-and-forget)
  - [Starten der Beispiele](#starten-der-beispiele)
## Beispiele 
### Future

Das erste Beispiel eines Future und Promises zeigt ein mögliches Anwendungsszenario. Verschiedenste Berechnungen werden durchgeführt. Solche Berechnungen können durchaus viel Rechenleistung und somit Zeit in Anspruch nehmen. Ebenso werden meistens solche Berechnungen für weitere Rechnungen benötigt. Auf dem ersten Blick ist Nebenläufigkeit das Tool der Wahl. Dauert die Rechnung jedoch länger wie erwartet kann es zu Problemen kommen. In dieser C++ Implementierung wird dieses Problem umgangen. 

Zunächst muss man anhand folgender Zeile ein Versprechen initialisieren. Mit diesem Versprechen kann ei Wert trotz Zeitlicher Verzögerungen gesetzt werden.

```cpp
std::promise<int> promise; 
```

Aus diesem Versprechen wird nun eine Zukunft erstellt. Diese Zukunft kann mehrere Aufgaben haben. unter anderem kann daraus das Versprechen eingelöst und der Wert eingeholt werden. Ebenso wartet die Zukunft auf eine Art Benachrichtigung des Versprechens ob die Berechnung fertig ist. 

```cpp
std::future<int> result = promise.get_future();
```

Der Thread benötigt als erstes Argument ein sogenanntes `Callable`. Dies wird durch folgende Funktion möglich: 

```cpp
void add(std::promise<int>&& promise, int a, int b) {
    promise.set_value(a+b);
}
```
Es wird jeweils ein Versprechen und die zu Verrechnenden Zahlen benötigt. Das Versprechen wird hierbei durch das `std::move` der `<Future>` Bibliothek bereitgestellt. Unabhängig von der Dauer der Berechnung kann nun die Berechnung gestartet werden. Das Ergebnis wird in der Zukunft bereitgestellt. 

```cpp
std::thread thread(calc, std::move(promise), a, b);
```

## Starten der Beispiele

Alle Beispielen können jeweils mit dem gleichen folgenden Kommando ausgeführt werden.

```bash
g++ cpp/<file>.cpp -std=c++11 -o cpp/<output-name>
./cpp/<output-name>
```



