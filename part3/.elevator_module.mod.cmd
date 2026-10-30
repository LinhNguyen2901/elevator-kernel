savedcmd_elevator_module.mod := printf '%s\n'   src/elevator.o src/elevator_mod.o src/pet.o | awk '!x[$$0]++ { print("./"$$0) }' > elevator_module.mod
