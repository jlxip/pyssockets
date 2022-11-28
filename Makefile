.PHONY: build install echo

build:
	python -m build

install: build
	sudo pip install dist/*.tar.gz

echo: install
	echo/echo.py
