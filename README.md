# Skribbl

Skribbl is a C++ multiplayer drawing-and-guessing game with:
- a **Qt desktop client** (`/home/runner/work/Skribbl/Skribbl/Client`)
- a **Crow HTTP server** (`/home/runner/work/Skribbl/Skribbl/Server`)
- a shared **PasswordHashing** library (`/home/runner/work/Skribbl/Skribbl/PasswordHashing`)

Presentation video:  
https://drive.google.com/file/d/12xymZgyJs11AEa2ckRiU7voXYyB4keOr/view?usp=drive_link

## Repository structure

- `/home/runner/work/Skribbl/Skribbl/Client.sln` – solution for the Qt client + PasswordHashing
- `/home/runner/work/Skribbl/Skribbl/Server.sln` – solution for the server + PasswordHashing
- `/home/runner/work/Skribbl/Skribbl/Client` – UI, networking, drawing tools, lobby/game flows
- `/home/runner/work/Skribbl/Skribbl/Server` – routing, game logic, player/game management, SQLite storage
- `/home/runner/work/Skribbl/Skribbl/PasswordHashing` – hashing utilities used by server/client projects

## Tech stack

- C++ (MSVC / Visual Studio projects)
- Qt 6 Widgets (client UI)
- Crow (HTTP server)
- CPR (HTTP client requests from Qt client)
- sqlite_orm + SQLite (persistence in `skribbl.sqlite`)

## Prerequisites

- Windows
- Visual Studio 2022 with C++ workload
- Qt 6 configured for Visual Studio (`Qt VS Tools` / `QtMsBuild`)
- Crow, CPR, and sqlite_orm available to the Visual Studio toolchain (for include/lib resolution)

## Build and run

1. **Start server**
   - Open `/home/runner/work/Skribbl/Skribbl/Server.sln` in Visual Studio.
   - Build and run the `Server` project (Debug/Release, x64).
   - The server binds to `127.0.0.1:18080`.

2. **Start client**
   - Open `/home/runner/work/Skribbl/Skribbl/Client.sln` in Visual Studio.
   - Build and run the `Client` project.
   - In the connection form, use the server IP (for local run: `127.0.0.1`).

3. **Play**
   - Register/login users.
   - Create or join rooms.
   - Start rounds, draw, guess words, and track scores in-game.

## Notes

- The words list used for rounds is stored in: `/home/runner/work/Skribbl/Skribbl/Server/words.txt`.
- The database file is: `/home/runner/work/Skribbl/Skribbl/Server/skribbl.sqlite`.
