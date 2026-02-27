package main

import (
	"fmt"
	"net"
)

// | Magic 		 (2 bytes) |
// | Version	 (2 byte)  |
// | Flags 		 (1 byte)  |
// | FrameLength (4 bytes) |
// | Body 		 (1 byte)  |
// The version is split into 2 bytes or 16 bits. The high byte represents the major & low byte the minor.
func main() {

	fmt.Println("Running Go Client")
	// Example url - fmqp://user:password@localhost:7632
	conn, err := net.Dial("tcp", "127.0.0.1:7632")
	if err != nil {
		panic(err)
	}
	magic := []byte{
		byte(0x46), // F
		byte(0x4D), // M
		byte(0x51), // Q
		byte(0x50), // P
	}
	version := []byte{0x00, 0x01}
	header := append(magic, version...)

	_, err = conn.Write(header)
	if err != nil {
		panic(err)
	}

	fmt.Println("Sent version:", version)
}
