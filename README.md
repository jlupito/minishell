## minishell

### minishell project for school 42

This is the first duet project at school 42 (done with @alixbbl) and one of the longest/biggest to produce.
The idea is to reproduce a shell, OK well a simple one, which includes:
- displaying a prompt waiting for a command
- having a working history
- handling simple commands (search and launch right executable)
- handling complex commands with pipes, redirections and heredocs
- handling signals
- handling single and double quotes
- handling environment variables
- re-coding a few builtins: cd, echo, exit, pwd, export, unset, env

*After discussing the general structure we would use for the project, 
we decided to split the work between parsing and execution.
I handled the main part of the parsing (using tokens and a lexer), even
though we both helped each other and put our noses in every part of the project.*
