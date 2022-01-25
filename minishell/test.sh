#! bin/bash

COMMAND="export a3=43"

for i in {1..10}; do echo $COMMAND | ./minishell; done
