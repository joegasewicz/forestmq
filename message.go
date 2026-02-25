package forestmq

import "time"

type Message struct {
	CreatedAt time.Time
}

func NewMessage() *Message {

	return &Message{}
}
