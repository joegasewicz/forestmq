package forestmq

import (
	"context"
	"log"
	"net"
	"sync"
)

type TCP struct {
	Listener net.Listener
	WG       sync.WaitGroup
	Queue    *Queue
}

func NewTCP(q *Queue) *TCP {
	return &TCP{
		Queue: q,
	}
}

func (t *TCP) Start(ctx context.Context, addr string) error {
	var err error
	t.Listener, err = net.Listen("tcp", addr)
	if err != nil {
		return err
	}
	log.Printf("Server running on:%s", addr)
	go t.acceptConn(ctx)
	return nil
}

func (t *TCP) Shutdown() error {
	err := t.Listener.Close()
	t.WG.Wait()
	return err
}

func (t *TCP) acceptConn(ctx context.Context) {
	for {
		select {
		case <-ctx.Done():
			err := t.Listener.Close()
			if err != nil {
				log.Printf("Error closing listener: %v", err)
			}
			return
		default:
			conn, err := t.Listener.Accept()
			if err != nil {
				log.Printf("Error: Connection error: %v", err)
				continue
			}
			t.WG.Add(1)
			go t.handleConn(ctx, conn)
		}
	}
}

func (t *TCP) handleConn(ctx context.Context, conn net.Conn) {
	defer t.WG.Done()
	defer conn.Close()
	buff := make([]byte, 1024)
	for {
		select {
		case <-ctx.Done():
			return
		default:
			n, err := conn.Read(buff)
			log.Printf("n -----> n: %d", n)
			if err != nil {
				log.Printf("Error reading from buffer: %v", err)
				return
			}
			conn.Write([]byte("Message received\n"))
		}
	}
}
