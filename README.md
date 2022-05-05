# GameOfLife

Conway's Game of Life Implemented in C

## Demo

![demo](./demo.gif)

## Run

```console
make && ./main
```

Press `<Enter>` or `<Space>` to reset game.

## Rules

1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
