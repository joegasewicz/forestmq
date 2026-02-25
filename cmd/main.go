package main

import (
	"fmt"

	"github.com/joegasewicz/forestmq"
)

func main() {
	queue := forestmq.NewQueue()
	fmt.Println("Created admin topic: ", queue.Topics["system"])
}
