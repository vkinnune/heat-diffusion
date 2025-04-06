# Heat Diffusion Simulation

This project simulates heat diffusion on a 2D grid. It demonstrates how heat spreads from a hot spot to surrounding areas over time.

## Features

- Simulates heat diffusion using a simple averaging algorithm
- Visualizes the heat distribution with color-coded output in the terminal
- Configurable grid size and simulation duration
- Uses MPI for potential parallel processing capabilities

## Requirements

- C compiler (gcc/clang)
- MPI library (for parallel processing)

## Building

To build the project, simply run:

```bash
make
```

This will compile the `heat.c` file and create an executable named `heat`.

## Usage

Run the simulation with:

```bash
./heat [grid_size] [max_ticks]
```

Where:
- `grid_size`: Size of the grid (default: 10)
- `max_ticks`: Number of simulation steps (default: 10)

Example:
```bash
./heat 20 50
```

This will run the simulation on a 20x20 grid for 50 time steps.

## How It Works

The simulation places a hot spot in the center of the grid. In each time step, the temperature of each cell is updated to be the average of its four neighboring cells (up, down, left, right). The hot spot maintains its temperature throughout the simulation.

The output uses color coding:
- Red: Hot areas (positive values)
- Blue: Cold areas (negative values)
- Green: Neutral areas (zero)

## License

This project is open source and available under the MIT License.
