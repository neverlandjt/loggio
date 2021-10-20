# loggio

### Build and Setup

1. Build base image with all the dependencies installed
```asm
docker build  --no-cache -t boost-grpc-base ./docker/
```
2. Build the server and client images
```asm
docker build --build-arg target=client --no-cache -t client .
docker build --build-arg target=server --no-cache -t server .
```
3. Run server containers as much as needed, use the port to expose web-server to local machine
```asm
docker run  -p <local_port>:8080 --name <server_name> -d server
```
4. Extract server's IP for inter-node communication
```asm
docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' <server_name>
```
5. After all servers are set up, run client container.
   Set up env variable `ADDRESSES` with extracted ips with port 50051 separated by ';'
   
```asm
docker run -p <local_port>:8080  -e ADDRESSES=<server1_ip:50051;server2_ip:50051> --name <client_name> -d client

```