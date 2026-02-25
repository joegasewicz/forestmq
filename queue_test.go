package forestmq

import (
	"testing"

	"github.com/joegasewicz/forestmq/pkg/topics"
	"github.com/stretchr/testify/assert"
)

func TestNewQueue(t *testing.T) {
	assert.Equal(t, 1, 1, "should be equal")
	queue := NewQueue()
	assert.EqualValues(t, queue.Topics, map[string]*topics.TopicQueue{}, "expected topics to be nil")
}
