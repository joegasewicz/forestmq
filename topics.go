package forestmq

type TopicQueue struct {
	Channel chan Message
}

func New(topicSize uint) *TopicQueue {
	return &TopicQueue{
		Channel: make(chan Message, topicSize),
	}
}
