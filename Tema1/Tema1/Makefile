RPC = rpcgen
CC = gcc
CFLAGS += -g -I/usr/include/tirpc
LDLIBS += -lnsl -ltirpc
PROGRAM = oauth
SRC_SERVER = $(PROGRAM)_svc.c $(PROGRAM)_xdr.c
SRC_CLIENT = $(PROGRAM)_clnt.c $(PROGRAM)_xdr.c

.PHONY: build clean

build: server client

server: $(SRC_SERVER) $(PROGRAM)_server.c server_functions.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

client: $(SRC_CLIENT) $(PROGRAM)_client.c server_functions.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# pentru a ptea da make fara sa se suprascrie oauth_svc.c si oauth_clnt.c
# $(SRC_SERVER) $(SRC_CLIENT): $(PROGRAM).x
# 	$(RPC) -C $^

clean:
	rm -f client server $(PROGRAM).h $(SRC_SERVER) $(SRC_CLIENT)
