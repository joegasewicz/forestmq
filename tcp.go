package forestmq

import (
	"context"
	"io"
	"log"
	"net"
	"sync"
)

var (
	MAGIC      = []byte("FMQP")
	RESP_ERROR = []byte{0x00}
	RESP_OK    = []byte{0x01}
)

type TCP struct {
	Listener net.Listener
	WG       sync.WaitGroup
	Queue    *Queue
	FMQP     *FMQP
}

func NewTCP(q *Queue, fmqp *FMQP) *TCP {
	return &TCP{
		Queue: q,
		FMQP:  fmqp,
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
	buff := make([]byte, 6)
	for {
		select {
		case <-ctx.Done():
			return
		default:
			bytesRead, err := conn.Read(buff)
			if err != nil {
				return
			}
			log.Printf("Bytes Read: %d\n\t-Buffer: %v", bytesRead, buff[:bytesRead])

			// Get Magic - first 4 bytes
			magicBuff := make([]byte, 4)
			_, err = io.ReadFull(conn, magicBuff)
			if err != nil {
				conn.Write(RESP_ERROR)
				return
			}
			err = t.FMQP.ReadMagic(magicBuff)
			if err != nil {
				log.Printf("Client error: %v", err)
				conn.Write(RESP_ERROR)
				return
			}
			// Get Version - next 2 bytes

			// Send acknowledgement to client

			conn.Write([]byte("Message received\n"))
		}
	}
}
