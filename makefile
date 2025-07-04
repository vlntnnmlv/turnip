all: run

build:
	dotnet build

run: build
	./bin/Debug/net8.0/basic