#!/bin/bash

if [ -z "$1" ]; then
	echo "Please provide a message"
	exit1 
fi

git add .

git commit -m "$1"

git push origin main

