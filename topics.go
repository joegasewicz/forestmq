package forestmq

type TopicQueue struct {
	Channel chan Message
}

func NewTopicQueue(topicSize uint) *TopicQueue {
	return &TopicQueue{
		Channel: make(chan Message, topicSize),
	}
}

func (tq *TopicQueue) Enqueue() {

}
