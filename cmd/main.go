package main

import (
	"context"
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/joegasewicz/forestmq"
)

func main() {
	queue := forestmq.NewQueue()
	ctx, cancel := context.WithCancel(context.Background())
	tcp := forestmq.NewTCP(queue)
	go func() {
		if err := tcp.Start(ctx, ":7632"); err != nil {
			log.Printf("TCP server error: %v", err)
		}
	}()

	sig := make(chan os.Signal, 1)
	signal.Notify(sig, os.Interrupt, syscall.SIGTERM)

	<-sig // block forever until we get the above signal

	log.Println("Server shutting down...")
	cancel()
	tcp.Shutdown()
}
