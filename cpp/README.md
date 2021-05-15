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

### Shared Future
     
C++ stellt dank der `<Future>` Bibliothek nicht nur Future and Promises zur Verfügung sondern auch sogenannte `std::shared_future`. Diese Erlaubt folgende Funktinalitäten bzw. Eigenschaften:
- erlaubt es Versprechen unabhängig von anderen zugehörigen Zukünften abzufragen. 
- besitzt die gleiche Schnittstelle wie eine `std::future`.
- kann aus einer bestehenden Zukunft durch ein `future.share()` erzeugt werden.
- kann aus einem beliebigen Versprechen durch 
  ```cpp
  std::shared_future<int> result = promise.get_future()
  ```
  erzeugt werden.

Jedoch ist deren Verwaltung etwas komplizierter. Im folgenden Beispiel wird darauf näher eingegangen.

Zunächst ist der Anfang recht identisch. Man muss ein Versprechen initialisieren

```cpp
std::promise<int> promise; 
```

Die Initialisierung der Shared Future ist wie bereits angeschnitten minimal verschieden. Aus dem Versprechen wird ähnlich zu Beispiel 1 ein Future generiert, jedoch wird es hier in einer speziellen Shared Future Variable gespeichert.

```cpp
std::shared_future<int> result = promise.get_future();
```

Auch die Erstellung des Threads ist identisch: 

```cpp
std::thread thread(add, std::move(promise), a, b);
```

Besonders zu beachten sind nun die Shared Futures Threads: 

```cpp
Requestor req; 

std::thread shared_thread_1(req, result);
std::thread shared_thread_2(req, result);
// [...]
std::thread shared_thread_n(req, result);

shared_thread_1.join();
shared_thread_2.join();
// [...]
shared_thread_n.join();
```

Ähnlich wie eine herkömmliche Future arbeitet auch das Shared Future Objekt, mit dem Unterschied, dass mehrere Threads auf denselben gemeinsamen Zustand warten dürfen. Im Gegensatz zur Future, das nur verschiebbar ist (so dass nur eine Instanz auf ein bestimmtes asynchrones Ergebnis verweisen kann), ist die Shared Future kopierbar und mehrere Shared Future Objekte können auf denselben gemeinsamen Zustand verweisen. Der Zugriff auf denselben gemeinsamen Zustand von mehreren Threads aus ist sicher, wenn jeder Thread dies über seine eigene Kopie eines Shared Future Objekte erledigt.

### Fire and Forget 

Jeder Eltern Thread muss sich gewisser Maßen auch um seine Kinder Threads kümmern. Beispiele kann der Eltern Thread warten bis der Kinder Thread fertig ist oder dieser sich abgekoppelt hat. Diese Eigenschaft ist recht bekannt, gilt jedoch nicht für `std::async`. Hierbei muss ein Eltern Thread sich nicht um die Kinder Threads kümmern. Diese Eigenschaft wird ebenfalls in einer Variante des Future and Promise eingebunden. 

Die sogenannte Fire and Forget Variante erzeugt mit der genannten Asynchronität eine spezielle Zukunft. Diese warten in dem Destruktor auf das Einlösen des Versprechens. Dadurch muss sich der Eltern Thread auch nicht um die Kinder Threads kümmern. 

Somit kann man `std::future` Objekt als Fire and Forget Job ausgeführt werden. Denn das `std::async` `std::future` Objekt ist an keine Variable gebunden wie es bei der herkömmlichen Variante der Fall ist. Somit kann mit dem gleichen Future Objekt sowohl gewartet als auch ein Ergebnis geholt werden. 

In der folgenden Variante ist eine Beispielhafte Implementierung um in C++ ein solches Prinzip zu erreichen:

```cpp
void blocking() {
    std::async(std::launch::async,[]{
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Blocking> Thread 1" << std::endl;
    });
    
    std::async(std::launch::async,[]{
        std::this_thread::sleep_for(std::chrono::seconds(1));  
        std::cout << "Blocking> Thread 2" << std::endl;
    });
  
    std::cout << "Blocking> Main Thread" << std::endl;  
}
```

Beide Threads warten in dessen Destruktor auf die Beendigung der Arbeit. Ein anderes Wort für ewiges warten ist blockieren. Und genau dies passiert hier. Es werden zwei Versprechen in den eigenen Threads gestartet, die resultierenden Fire and Forget Futures. Diese blockieren den Destruktor so lange bis das entsprechende Versprechen eingelöst ist. Das Ergebnis ist hierbei der Verlust des Future and Promise Prinzip, die Ausgabe des Programmes ist wie folgt: 

```
Blocking> Thread 1
Blocking> Thread 2
Blocking> Main Thread
```

Obwohl im Main Thread zwei neue Promise Threads erzeugt wurden, die jeweils in eigenen Threads ausgeführt werden, wird dennoch jeder Thread nacheinander ausgeführt. Demnach ist der erste Thread mit der längsten Arbeitszeit auch als erstes fertig. 

Dieses grundlegende Problem der Fire and Forget Futures lässt sich nicht komplett lösen, lediglich *entschärfen*. Im folgenden Code Beispiel wird der Future an eine zugehörige Variable gebunden. Das Blockieren in dessen Destruktor findet erst statt, wenn die Variable ihre Gültigkeit verliert. 

```cpp
void not_blocking() {
    auto first = std::async(std::launch::async, [] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Not Blocking> Thread 1" << std::endl;
    });

    auto second = std::async(std::launch::async, [] {
        std::this_thread::sleep_for(std::chrono::seconds(1));  
        std::cout << "Not Blocking> Thread 2" << std::endl; 
    });

    std::cout << "Not Blocking> Thread Main" << std::endl;
}
```

Dadurch zeigt die Ausgabe des Programmes aus den gewünschten Output mit einer kleinen aber wichtigen Veränderung:

```
Not Blocking> Thread Main
Not Blocking> Thread 2
Not Blocking> Thread 1
```

Die Ausgabe entspricht dem Intuitiven Gedanken bei einer Nebenläufigen Ausführung von Threads. Schließlich besitzen die Kinder Threads (Future 1 und 2) jeweils deren Gültigkeit bis zum Ende des Eltern Threads (Main Thread). Somit ist der Thread mit der kleineren Arbeitszeit schneller fertig. 

> In diesem Beispiel werden Futures recht speziell eingesetzt. Zum einen werden diese an keine explizite Variable gebunden und zum anderen werden diese nicht dazu verwendet das Ergebnis mit einem blockierenden `get()` oder `wait()` anzufordern. Jedoch wird nur unter dieser Speziellen Rahmenbedingung erst das besagte Phänomen sichtbar. **Ein Blockierender Destruktor bei einem Asynchron erstellten Fire and Forget Future der an keine Variable gebunden ist.**

## Starten der Beispiele

Alle Beispielen können jeweils mit dem gleichen folgenden Kommando ausgeführt werden.

```bash
g++ cpp/<file>.cpp -std=c++11 -o cpp/<output-name>
./cpp/<output-name>
```



