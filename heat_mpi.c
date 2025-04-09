#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COLOR_RED "\033[1;31m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

void print_local_grid(double **grid, int local_rows, int grid_size, int rank) {
  printf("=== Rank %d ===\n", rank);
  for (int i = 1; i <= local_rows; i++) {
    for (int j = 0; j < grid_size; j++) {
      double val = grid[i][j];
      if (val > 0)
        printf(COLOR_RED "%.2f " COLOR_RESET, val);
      else if (val < 0)
        printf(COLOR_BLUE "%.2f " COLOR_RESET, val);
      else
        printf(COLOR_GREEN "%.2f " COLOR_RESET, val);
    }
    printf("\n");
  }
  fflush(stdout);
}

double **alloc_grid(int rows, int cols) {
  double **grid = malloc(rows * sizeof(double *));
  for (int i = 0; i < rows; i++) {
    grid[i] = calloc(cols, sizeof(double));
  }
  return grid;
}

void free_grid(double **grid, int rows) {
  for (int i = 0; i < rows; i++)
    free(grid[i]);
  free(grid);
}

int main(int argc, char **argv) {
  int rank, size;
  int grid_size = 10;
  int max_ticks = 10;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc > 1)
    grid_size = atoi(argv[1]);
  if (argc > 2)
    max_ticks = atoi(argv[2]);
  if (grid_size <= 0 || max_ticks <= 0) {
    if (rank == 0)
      printf("Invalid grid size or max ticks\n");
    MPI_Finalize();
    return 1;
  }

  if (grid_size % size != 0) {
    if (rank == 0)
      printf("GRID_SIZE must be divisible by number of ranks.\n");
    MPI_Finalize();
    return 1;
  }

  int local_rows = grid_size / size;
  int extended_rows = local_rows + 2;

  double **grid = alloc_grid(extended_rows, grid_size);
  double **new_grid = alloc_grid(extended_rows, grid_size);

  int global_hot_i = grid_size / 2;
  int global_cold_i = 1;
  int local_start = rank * local_rows;

  if (global_hot_i >= local_start && global_hot_i < local_start + local_rows)
    grid[global_hot_i - local_start + 1][grid_size / 2] = 100.0;

  if (global_cold_i >= local_start && global_cold_i < local_start + local_rows)
    grid[global_cold_i - local_start + 1][1] = -100.0;

  for (int tick = 0; tick < max_ticks; tick++) {
    MPI_Status status;
    int up = rank - 1;
    int down = rank + 1;

    if (up >= 0)
      MPI_Sendrecv(grid[1], grid_size, MPI_DOUBLE, up, 0, grid[0], grid_size,
                   MPI_DOUBLE, up, 0, MPI_COMM_WORLD, &status);
    if (down < size)
      MPI_Sendrecv(grid[local_rows], grid_size, MPI_DOUBLE, down, 0,
                   grid[local_rows + 1], grid_size, MPI_DOUBLE, down, 0,
                   MPI_COMM_WORLD, &status);

    for (int i = 1; i <= local_rows; i++) {
      for (int j = 0; j < grid_size; j++) {
        int global_i = i - 1 + rank * local_rows;
        if ((global_i == grid_size / 2 && j == grid_size / 2) ||
            (global_i == 1 && j == 1)) {
          new_grid[i][j] = grid[i][j];
        } else if (i > 0 && i < extended_rows - 1 && j > 0 &&
                   j < grid_size - 1) {
          new_grid[i][j] = (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1] +
                            grid[i][j + 1]) /
                           4.0;
        } else {
          new_grid[i][j] = grid[i][j];
        }
      }
    }

    double **tmp = grid;
    grid = new_grid;
    new_grid = tmp;

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
      printf("Tick %d\n", tick + 1);
    MPI_Barrier(MPI_COMM_WORLD);
    print_local_grid(grid, local_rows, grid_size, rank);
    MPI_Barrier(MPI_COMM_WORLD);
    usleep(100000);
  }

  double *local_data = malloc(local_rows * grid_size * sizeof(double));
  for (int i = 0; i < local_rows; i++)
    memcpy(&local_data[i * grid_size], grid[i + 1], grid_size * sizeof(double));

  double *full_grid = NULL;
  if (rank == 0)
    full_grid = malloc(grid_size * grid_size * sizeof(double));

  MPI_Gather(local_data, local_rows * grid_size, MPI_DOUBLE, full_grid,
             local_rows * grid_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("\n=== Final Grid ===\n");
    for (int i = 0; i < grid_size; i++) {
      for (int j = 0; j < grid_size; j++) {
        double val = full_grid[i * grid_size + j];
        if (val > 0)
          printf(COLOR_RED "%.2f " COLOR_RESET, val);
        else if (val < 0)
          printf(COLOR_BLUE "%.2f " COLOR_RESET, val);
        else
          printf(COLOR_GREEN "%.2f " COLOR_RESET, val);
      }
      printf("\n");
    }
    free(full_grid);
  }

  free(local_data);
  free_grid(grid, extended_rows);
  free_grid(new_grid, extended_rows);
  MPI_Finalize();
  return 0;
}
