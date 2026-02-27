package forestmq

import (
	"bytes"
	"errors"
	"net/url"

	"github.com/joegasewicz/forestmq/pkg/schemas"
)

type HandshakeClientReq struct {
	Magic   []byte
	Version []byte
}

type FMQP struct {
	Endpoint           schemas.Endpoint
	HandshakeClientReq *HandshakeClientReq
}

func NewFMQP() *FMQP {
	return &FMQP{
		Endpoint:           schemas.Endpoint{},
		HandshakeClientReq: &HandshakeClientReq{},
	}
}

func (f *FMQP) ParseEndpoint(fullURL string) error {
	parsedURL, err := url.Parse(fullURL)
	if err != nil {
		return errors.New("error parsing incoming url")
	}
	f.Endpoint.Schema = parsedURL.Scheme

	return nil
}

func (f *FMQP) ReadMagic(buff []byte) error {
	if !bytes.Equal(buff, MAGIC) {
		return errors.New("could not match expected MAGIC bytes")
	}
	// Let's make sure we don't store a reference to the actual magic buff:
	f.HandshakeClientReq.Magic = make([]byte, len(buff))
	copy(f.HandshakeClientReq.Magic, buff)
	return nil
}

func (f *FMQP) ReadVersion(buff []byte) error {

	return nil
}
