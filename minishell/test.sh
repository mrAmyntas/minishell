#! bin/bash

COMMAND="echo bonjour ; |"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo bonjour | |"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="|"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo bonjour |;"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo bonjour ; ls"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo bonjour > test\ 1"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="cd $HOME/Documents"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo "\s" & echo "\s""
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo >"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="cat /dev/random | head -n 1 | cat -e"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="unset var1 HOME"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="export "" et unset """
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo test > file test1"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="$"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="not_cmd bonjour > salut"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="env puis export puis env # vars aren't sorted"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="cat Makefile | grep pr | head -n 5 | cd test (mybin)"
for i in {1..10}; do echo $COMMAND | ./minishell; done
echo "echo $?" | ./minishell

COMMAND="cat Makefile | grep pr | head -n 5 | cat test (bin)"
for i in {1..10}; do echo $COMMAND | ./minishell; done
echo "echo $?" | ./minishell

COMMAND="cat Makefile | grep pr | head -n 5 | hello (NA)"
for i in {1..10}; do echo $COMMAND | ./minishell; done
echo "echo $?" | ./minishell

COMMAND="echo bonjour >>> test"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo bonjour > > out"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo 2 >> out1 > out2"
for i in {1..10}; do echo $COMMAND | ./minishell; done

COMMAND="echo 2 > out1 >> out2"
for i in {1..10}; do echo $COMMAND | ./minishell; done
