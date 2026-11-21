# sockets

TCP sockets exercises and a small in-memory HTTP REST API (TODO list) in C.

| File / binary | Description                                        |
|---------------|----------------------------------------------------|
| 0-server.c    | Listen on port 12345                               |
| 1-server.c    | Accept one connection, print client IP             |
| 2-client.c    | Connect to a server                                |
| 3-server.c    | Receive and print one message                      |
| todo_api_0    | HTTP request line breakdown                        |
| todo_api_1    | Path and query parameters                          |
| todo_api_2    | Headers                                            |
| todo_api_3    | Body parameters                                    |
| todo_api_4    | POST /todos                                        |
| todo_api_5    | + GET /todos                                       |
| todo_api_6    | + GET /todos?id=N                                  |
| todo_api_7    | + DELETE /todos?id=N                               |

Build with `make todo_api_N` (N from 0 to 7). The todo_api binaries share one
code base compiled with `-DLEVEL=N`.
