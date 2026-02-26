package forestmq

import (
	"errors"
	"net/url"

	"github.com/joegasewicz/forestmq/pkg/schemas"
)

type FMQP struct {
	Endpoint schemas.Endpoint
}

func NewFMQP() *FMQP {
	return &FMQP{
		Endpoint: schemas.Endpoint{},
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
