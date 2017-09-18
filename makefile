#Author: spiros antonatos

all:
	+$(MAKE) -C client
	+$(MAKE) -C server

clean:
	cd client && $(MAKE) clean
	cd server && $(MAKE) clean

clean-txt:
	cd client && $(MAKE) clean-txt
