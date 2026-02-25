package forestmq

import "github.com/joegasewicz/forestmq/pkg/topics"

type Queue struct {
	Topics map[string]*topics.TopicQueue
}

func NewQueue() *Queue {
	return &Queue{
		Topics: make(map[string]*topics.TopicQueue, 0),
	}
}
