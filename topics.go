package forestmq

import "errors"

type TopicQueue struct {
	Channel chan Message
}

func NewTopicQueue(topicSize uint) *TopicQueue {
	return &TopicQueue{
		Channel: make(chan Message, topicSize),
	}
}

func (tq *TopicQueue) Enqueue(message *Message) (error, *Message) {
	select {
	case tq.Channel <- *message:
		return nil, message
	default:
		return errors.New("queue is full"), message
	}
}

func (tq *TopicQueue) Dequeue() Message {
	message := <-tq.Channel
	return message
}
