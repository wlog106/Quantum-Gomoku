#### Compile server & client
- compile all: `make all`
- compile server only: `make server`
- compile client only: `make client` 

#### Dependencies
- mariadb-server: [mariadb doc for Debian-base Linux distro](https://sources.debian.org/src/mariadb/1%3A11.8.3-0%2Bdeb13u1/debian/mariadb-server.README.Debian)
- mariadb Connector/C: [C connector doc](https://mariadb.com/docs/connectors/mariadb-connector-c)

#### Note
- table description example ( you may want to change the some settings )
```
+-----------+-------------+------+-----+---------------------+----------------+
| Field     | Type        | Null | Key | Default             | Extra          |
+-----------+-------------+------+-----+---------------------+----------------+
| id        | int(11)     | NO   | PRI | NULL                | auto_increment |
| name      | varchar(50) | NO   | UNI | NULL                |                |
| passwd    | char(64)    | NO   |     | NULL                |                |
| create_at | timestamp   | YES  |     | current_timestamp() |                |
| elo       | int(11)     | YES  |     | NULL                |                |
+-----------+-------------+------+-----+---------------------+----------------+
```
- you have to put the following variables in `.env` file  
  `DB_HOST=${IP}`  
  `DB_ADMIN=${account name}`  
  `DB_PASSWD=${account password}`  
  `DB_NAME=${the db you want to use}`  
- put the `.env` file in `${PROJECT ROOT}`  
  then run `source ./load_env.sh` in `${PROJECT_ROOT}`  
  to set required environment variables
- you may want to add this in your `${PROJECT_ROOT}/.vscode/settings.json` to shut up clangd server
 ```json
  "clangd.fallbackFlags": [
    "-I${workspaceFolder}/server/lib/server_cmd/include",
    "-I${workspaceFolder}/server/lib/mariadb/include",
    "-I${workspaceFolder}/server/lib/objects/include",
    "-I${workspaceFolder}/server/lib/utils/include",
    "-I${workspaceFolder}/share/board/include",
    "-I${workspaceFolder}/share/cmd/include",
    "-I${workspaceFolder}/share/wrap/include"
  ]
```

#### Reference
- [sha256 code source](https://lucidar.me/en/dev-c-cpp/sha-256-in-c-cpp/)
