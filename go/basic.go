package main

import (
	"fmt"
	"time"
)

type futureType struct {
	value  int
	status bool
}

type Future chan futureType

func future(function func() (int, bool)) Future {
	channel := make(chan futureType)
	go func() {
		value, status := function()
		fType := futureType{value, status}
		channel <- fType
	}()
	return channel
}

func (future Future) get() (int, bool) {
	fType := <-future
	return fType.value, fType.status
}

func (future Future) onFailure(callback func()) {
	go func() {
		_, status := future.get()
		if !status {
			callback()
		}
	}()
}

func (future Future) onSuccess(callback func(result int)) {
	go func() {
		value, status := future.get()
		if status {
			callback(value)
		}
	}()
}

func div(a int, b int) Future {
	return future(func() (int, bool) {
		if b > 0 {
			return (a / b), true
		} else {
			return 0, false
		}
	})
}

func run() {
	// start nicht blockeirend -> hintergrund
	fmt.Println("Start Calculating")

	calculation := div(10, 5)

	calculation.onSuccess(func(result int) {
		fmt.Printf("Result Received> Success (%d) \n", result)
	})

	calculation.onFailure(func() {
		fmt.Println("Result Received> Failure")
	})

	// start nicht blockeirend -> hintergrund

	// das läuft hier eigenltic h ab :D
	fmt.Println("Do stuff")
	time.Sleep(2 * time.Second)
}

func main() {
	run()
}
