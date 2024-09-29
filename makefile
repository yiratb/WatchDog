.PHONY: all clean run_client

all:
	$(MAKE) -C src all  # Calls the 'all' target in the src/Makefile to compile

clean:
	$(MAKE) -C src clean  # Calls the 'clean' target in the src/Makefile to clean up

run:
	$(MAKE) -C src run  # Calls the 'run' target in the src/Makefile to run wd_client
