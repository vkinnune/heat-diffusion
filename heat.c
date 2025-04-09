#define GRID_SIZE 10
#define MAX_TICKS 10

#define COLOR_RED "\033[1;31m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_grid(double **grid, int grid_size) {
  int i, j;

  i = 0;
  while (i < grid_size) {
    j = 0;
    while (j < grid_size) {
      // print with colors
      if (grid[i][j] > 0) {
        printf(COLOR_RED "%.2f " COLOR_RESET, grid[i][j]);
      } else if (grid[i][j] < 0) {
        printf(COLOR_BLUE "%.2f " COLOR_RESET, grid[i][j]);
      } else {
        printf(COLOR_GREEN "%.2f " COLOR_RESET, grid[i][j]);
      }
      j++;
    }
    printf("\n");
    i++;
  }
}

void calculate_heat(double **grid, double **new_grid, int grid_size,
                    int max_ticks) {
  int i, j, tick;

  tick = 0;
  while (tick < max_ticks) {
    i = 0;
    while (i < grid_size) {
      j = 0;
      while (j < grid_size) {
        if (i == grid_size / 2 && j == grid_size / 2) {
          new_grid[i][j] = grid[i][j]; // keep the hot spot
        } else if (i == 1 && j == 1) {
          new_grid[i][j] = grid[i][j]; // keep the cold spot
        } else if (i > 0 && i < grid_size - 1 && j > 0 && j < grid_size - 1) {
          new_grid[i][j] = (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1] +
                            grid[i][j + 1]) /
                           4.0;
        } else {
          new_grid[i][j] = grid[i][j];
        }
        j++;
      }
      i++;
    }
    tick++;
    printf("Tick %d completed\n", tick);
    // print the grid
    print_grid(new_grid, grid_size);
    // Swap the grids
    double **temp = grid;
    grid = new_grid;
    new_grid = temp;
    // Sleep for a while to simulate time passing
    usleep(100000); // 0.1 seconds
  }
}

int main(int argc, char *argv[]) {
  double **grid;
  double **new_grid;

  int grid_size = 10;
  int max_ticks = 10;
  if (argc > 1) {
    grid_size = atoi(argv[1]);
  }
  if (argc > 2) {
    max_ticks = atoi(argv[2]);
  }
  if (grid_size <= 0 || max_ticks <= 0) {
    printf("Invalid grid size or max ticks\n");
    return 1;
  }
  grid = (double **)malloc(grid_size * sizeof(double *));
  new_grid = (double **)malloc(grid_size * sizeof(double *));

  int i = 0;
  while (i < grid_size) {
    grid[i] = (double *)malloc(grid_size * sizeof(double));
    new_grid[i] = (double *)malloc(grid_size * sizeof(double));
    i++;
  }

  // Initialize the grid with zeros
  i = 0;
  while (i < grid_size) {
    memset(grid[i], 0, grid_size * sizeof(double));
    memset(new_grid[i], 0, grid_size * sizeof(double));
    i++;
  }

  // Initialize the grid with a hot spot
  grid[grid_size / 2][grid_size / 2] = 100.0; // Set a hot spot
  // Cold spot
  grid[1][1] = -100.0; // Set a cold spot

  printf("Grid initialized\n");

  print_grid(grid, grid_size);
  calculate_heat(grid, new_grid, grid_size, max_ticks);

  printf("Heat calculation completed\n");

  // Free allocated memory
  i = 0;
  while (i < grid_size) {
    free(grid[i]);
    free(new_grid[i]);
    i++;
  }
  free(grid);
  free(new_grid);

  return 0;
}
