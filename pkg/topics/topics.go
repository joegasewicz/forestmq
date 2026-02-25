package topics

import (
	"github.com/joegasewicz/forestmq/pkg/messages"
)

type TopicQueue struct {
	Channel chan messages.Message
}

func New(topicSize uint) *TopicQueue {
	return &TopicQueue{
		Channel: make(chan messages.Message, topicSize),
	}
}
