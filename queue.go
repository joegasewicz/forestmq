package forestmq

type Queue struct {
	Topics map[string]*TopicQueue
}

func NewQueue() *Queue {

	topics := make(map[string]*TopicQueue)
	topics["system"] = NewTopicQueue(1000)
	return &Queue{
		Topics: topics,
	}
}
