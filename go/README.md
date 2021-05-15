# Futures and Promises in Go

Futures und Promises und deren Umsetzung in Go

<!--<img src="assets/../../assets/go.png"
     alt="Go Logo"
     style="height:200px;display:block; margin-left:auto; margin-right:auto;"/>-->
	 
> Diese Arbeit wurde von Raphaele Salvatore Licciardo im Rahmen der Modellbasierte Softwareentwicklung Vorlesung an der Hochschule Karlruhe - Technik und Wirtschaft im SS 2021 durchgeführt.  

Für die Nebenläufige Programmiersprache Go gibt es keine offizielle Future and Promise Bibliothek wie es beispielsweise in C++ der Fall ist. Schließlich braucht man dies auch nicht, dank der Channels können Future and Promise Objekte einfach nachgebildet werden. 

## Inhaltsverzeichnis

- [Futures and Promises in Go](#futures-and-promises-in-go)
	- [Inhaltsverzeichnis](#inhaltsverzeichnis)
	- [Beispiele](#beispiele)
		- [Channels](#channels)
	- [Starten der Beispiele](#starten-der-beispiele)
## Beispiele 

### Channels

> Dieses Basis Beispiel basiert auf der Vorlesung Autonome Systeme. 

Da es in Go keine Future and Promise Bibliothek gibt, müssen diese Strukturen nachgebaut werden. Durch die Go internen Kanäle ist dies aber kein großer Aufwand. So kann Beispielsweise ein Future wie folgt aufgebaut werden.
> In diesem Beispiel wird eine Division mit dem genannten Prinzip implementiert.

```go
func future(function func() (int, bool)) Future {
	channel := make(chan futureType)
	go func() {
		value, status := function()
		fType := futureType{value, status}
		channel <- fType
	}()
	return channel
}
```

Das hier in Go implementierte Future führt eine Ansychrone Berechnung aus um einen Wert zu erzeugen. Sobald diese Berechnung abgeschlossen ist, wird dieser dank dem Kanal an den Future gebunden. Hier wird ebenso ein boolescher Rückgabeparameter angelegt, um darzustellen, ob die Berechnung erfolgreich war oder fehlgeschlagen ist. Ein Fehlschlag kann ein erkannter Rechenregel Bruch sein oder ein einfacher Timeout einer HTTP Anfrage.

```go
func (future Future) get() (int, bool) {
	fType := <-future
	return fType.value, fType.status
}
```

Die Get Methode fragt den an den Future beziehungsweise Kanal gebunden Wert ab. Falls dieser noch nichtvorhanden ist, wird entsprechend blockiert.

```go
func (future Future) onSuccess(callback func(result int)) {
	go func() {
		value, status := future.get()
		if status {
			callback(value)
		}
	}()
}
```

Im Gegensatz zu der `get()` Methode ist der `onSuccess()` Aufruf nicht blockierend. Als Argument nimmt diese Funktion eine Callback Funktion entgegegen. Hierbei wird der an den Future gebundene Wert verarbeitet. Natürlich sobald dieser verfügbar ist. Diese Methode trifft nur ein, falls die Berechnung auch erfolgreich war. 

```go
func (future Future) onFailure(callback func()) {
	go func() {
		_, status := future.get()
		if !status {
			callback()
		}
	}()
}
```

Auch der `onFailure()` Aufruf ist nicht blockierend. Dieser nimmt ebenso eine Callback Funktion entgegeben. Analog zu oben, trifft diese nur ein, wenn die Berechnung des Ergebnisses fehlgeschlagen ist. 

## Starten der Beispiele

Alle Beispielen können jeweils mit dem gleichen folgenden Kommando ausgeführt werden.

```bash
go run go/<file>.go
```