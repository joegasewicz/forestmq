package forestmq

type Message struct {
	Content interface{}
}

func NewMessage(content interface{}) *Message {
	return &Message{
		Content: content,
	}
}
