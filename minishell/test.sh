#! bin/bash

COMMAND="export a3=43"

for i in {1..1000}; do echo $COMMAND | ./minishell; done
