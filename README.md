# blokus-duo-cli
Online Blockus Duo game in command line interface.

## Installation
Latest prebuilt version could be find [here](https://github.com/yuyuranium/blokus-duo-tui/releases/tag/v0.0.1)
### Manual buid
1. Install `ncurses` library
   ```bash
   sudo apt update
   sudo apt install libncurses5-dev libncursesw5-dev
   ```
2. Build
   ```bash
   make
   ```
   The binaries will be placed under `build` directory.

## Usage
1. Start server first
   ```bash
   ./server -p [port]
   ```
2. Client connect to server
   ```bash
   ./client -i [server ip] -p [port]
   ```
   Whenever two clients connected to the server, the match will be started.

## How to play?
- [Rule of Blokus Duo](https://www.ultraboardgames.com/blokus/blokus-duo.php)
- Use `h`, `j`, `k`, `l` or `<up>`, `<down>`, `<left>`, `<right>` to navigate. Including tile selection and placement.
- Press `<space>` to select tile you want to use.
- Navigate to the position that you want to place the tile.
  - Press `q` to reselect the tile.
  - Press `r` to rotate the selected tile.
  - Press `m` to mirror the selected tile.
  - Press `c` to see opponent's remaining tiles.
  - Press `<space>` to choose the position.
- Press `y`, `<enter>` to confirm placement. Press `n` to cancel placement.
- The match will be continued until no tile could be placed onto the board.
  - Press 'h' to get a hint if you don't know which tile you can use.
