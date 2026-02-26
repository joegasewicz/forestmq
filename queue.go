package forestmq

type Queue struct {
	Topics map[string]*TopicQueue
}

func NewQueue() *Queue {
	topics := make(map[string]*TopicQueue)

	return &Queue{
		Topics: topics,
	}
}

func (q *Queue) Enqueue(topicName string, message *Message) {
	q.Topics[topicName].Enqueue(message)
}

func (q *Queue) Dequeue(topicName string) Message {
	return q.Topics[topicName].Dequeue()
}

func (q *Queue) NewTopic(topicName string) *TopicQueue {
	q.Topics[topicName] = NewTopicQueue(1)
	return q.Topics[topicName]
}
