# minishell
This project is about creating a simple shell.
It can use commands like echo (-n) / pwd / env / export / unset / cd. (More can be added without too much hassle)
cd works with ~and - (but not with a path after e.g. ~/Path/Path)
It can interpret some special characters: '<' '>' '<<' '>>' '|' '$' and the single and double quotes.
It keeps the environment updated.
It keeps the correct exit status ($?).
It has a working history.
It mimics bash in it's behavior.
Control-C gives a new prompt.
Control-D exits the shell.
'exit' exits the shell.
It uses readline so this has to be installed in order to compile.
