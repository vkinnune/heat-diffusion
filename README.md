# Heat Diffusion Simulation

This project simulates heat diffusion on a 2D grid, either sequentially or in parallel using MPI. It shows how heat propagates from a hot spot and a cold spot over time.

## Features

- Simulates heat diffusion with a simple 2D stencil algorithm
- Color-coded terminal output (Red = hot, Blue = cold, Green = neutral)
- Configurable grid size and tick count
- Parallelized with MPI for distributed computation
- Sequential and parallel versions included

## Requirements

- `mpicc` (e.g., OpenMPI or MPICH)
- POSIX-compatible system (e.g., Linux/macOS)

## Building

To build both versions:

```bash
make
```

To build individually:

```bash
make heat        # Sequential version
make heat_mpi    # MPI version
```

To clean:

```bash
make clean
```

## Usage

Sequential version:
```bash
./heat [grid_size] [max_ticks]
```

Example:
```bash
./heat 20 50
```

MPI version:
```bash
mpirun -np [num_ranks] ./heat_mpi [grid_size] [max_ticks]
```
*grid_size must be divisible by num_ranks*

Example:
```bash
mpirun -np 2 ./heat_mpi 10 20
```

## How It Works

Each tick updates the grid based on neighbor averaging (ignoring corners). Ghost rows are used in the MPI version to exchange boundary data between ranks.

## License

This project is open source.
