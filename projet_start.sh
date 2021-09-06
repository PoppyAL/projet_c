#!/bin/bash

gcc projet_ecran.c -o projet_ecran
#./projet_ecran

gcc projet_shell.c -o projet_shell
#./projet_shell

gnome-terminal -- /bin/sh -c 'echo $$>pid.txt; ./projet_ecran; bash' 
gnome-terminal -- /bin/sh -c 'echo $$>>pid.txt; ./projet_shell; bash'
