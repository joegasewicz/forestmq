package forestmq

type Queue struct {
	Topics map[string]*TopicQueue
}

func NewQueue() *Queue {
	return &Queue{
		Topics: make(map[string]*TopicQueue),
	}
}
