package forestmq

import (
	"time"

	"github.com/joegasewicz/forestmq/pkg/schemas"
)

type Message struct {
	CreatedAt time.Time
}

func NewMessage() *Message {

	return &Message{}
}

func (m *Message) Send(channel chan TopicQueue, message schemas.Message) {
	channel <- message
}
