IMG_NAME=bandnoticeboard/forestmq:0.6.0

build:
	mkdir build
	cmake -S . -B build
	cd build
	make

# Development requirements
# This will install all the required development libraries on a Mac.
install_deps_mac:
	brew install check
	brew install cmocka
	brew install doxygen
	brew install jansson
	brew install libevent


install_deps_linux:
	sudo apt install -y libjansson-dev
	sudo apt install -y libsystemd-dev
	sudo apt install -y libevent-dev

docs_init:
	doxygen -g Doxyfile

docker-build-mac:
	docker build --tag $(IMG_NAME) .

docker-build-linux:
	export DOCKER_DEFAULT_PLATFORM=linux/amd64 && docker build --tag $(IMG_NAME) .

docker-run:
	docker run -p 8005:8005 $(IMG_NAME)

docker-push:
	docker push $(IMG_NAME)

docker-remove:
	docker rmi $(IMG_NAME) --force

docker_img_exec:
	docker run --rm -it --entrypoint /bin/bash $(IMG_NAME)

docs_cname:
	echo "forestmq.dev" > site/CNAME

docs_build:
	cd docs && pipenv run mkdocs build
	cd docs && echo "forestmq.dev" > site/CNAME