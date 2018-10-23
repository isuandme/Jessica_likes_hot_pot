build:
	gcc ./server_file/server.c -o ./server_file/server
	gcc ./client_file/client.c -o ./client_file/client

clean:
	rm ./server_file/server ./client_file/client

scp:
	cd ..
	scp -r ./../jacob_pratt_project2 jrpratt@pyrite.cs.iastate.edu:CS352/projects/project2

ssh:
	ssh jrpratt@pyrite.cs.iastate.edu