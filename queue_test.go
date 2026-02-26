package forestmq

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewQueue(t *testing.T) {
	assert.Equal(t, 1, 1, "should be equal")
	queue := NewQueue()
	assert.EqualValues(t, queue.Topics, map[string]*TopicQueue{}, "expected topics to be nil")

	q := NewQueue()
	systemTopic := q.NewTopic("system")

	// Messages
	msg1 := NewMessage("Message #1")
	msg2 := NewMessage("Message #2")
	msg3 := NewMessage("Message #3")

	err1, resp1 := systemTopic.Enqueue(msg1)
	err2, resp2 := systemTopic.Enqueue(msg2)
	err3, resp3 := systemTopic.Enqueue(msg3)
	fmt.Println("err1 ---> ", err1)
	fmt.Println("resp1 ---> ", resp1)
	fmt.Println("err2 ---> ", err2)
	fmt.Println("resp2 ---> ", resp2)
	fmt.Println("err3 ---> ", err3)
	fmt.Println("resp3 ---> ", resp3) // TODO

	fmt.Println("here -----> ", systemTopic.Dequeue()) // TODO
	//fmt.Println("here -----> ", systemTopic.Dequeue())
	//fmt.Println("here -----> ", systemTopic.Dequeue())
}
