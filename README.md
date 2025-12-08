#### Compile server & client
- compile all: `make all`
- compile server only: `make server`
- compile client only: `make client` 

#### Dependencies
- mariadb-server: [mariadb doc for Debian-base Linux distro](https://sources.debian.org/src/mariadb/1%3A11.8.3-0%2Bdeb13u1/debian/mariadb-server.README.Debian)
- mariadb Connector/C: [doc](https://mariadb.com/docs/connectors/mariadb-connector-c)

#### Note
- run `source ./load_env.sh` to set required environment variables
- if it wasn't an executable run `chmod +x ./load_env.sh`
- add this in your *** ${PROJECT_ROOT}/.vscode/settings.json *** to shut up clangd server
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
- [sha256 code source]()