#include <stdlib.h>
#include <ncurses.h>
#include <syslog.h>
#include <stdarg.h>

#include "utils/array.h"
#include "utils/number.h"

#include "window.h"
#include "libminesweeper/minesweeper.h"

int process_character(int ch, GameWindow *window);

int main(int argc, char **argv)
{
  int rows, cols;
  MinesweeperCtx game = {};
  GameWindow window = {};

  openlog("minesweeper", (LOG_CONS | LOG_PERROR | LOG_PID), LOG_USER);

  if (argc != 3) {
    // TODO: take all the space available
    rows = 16;
    cols = 32;
  } else {
    // TODO: Limit by available screen space
    rows = get_int_from_str(argv[1]);
    cols = get_int_from_str(argv[2]);

    if (rows < 0 || cols < 0) {
      syslog(LOG_ERR, "Invalid rows and columns.");

      return -1;
    }
  }

  syslog(LOG_INFO, "Initializing game with size: %dx%d", cols, rows);

  msw_init(&game, rows, cols);
  window_init(&window, &game);
  window_draw_game(&window);

  while(process_character(getch(), &window));

  syslog(LOG_INFO, "Exiting because the user asked to.");

  window_exit(&window);

  closelog();

  return 0;
}

int process_character(int ch, GameWindow *window)
{
  if (ch == 'q') {
    if (msw_quit(window->game) == 0)
      return 0;
  }

  switch (ch) {
  case KEY_LEFT:
  case KEY_RIGHT:
  case KEY_UP:
  case KEY_DOWN:
    window_move_cursor(window, ch);

    break;
  case 10: // Enter key
    window_reveal_current_cell(window);

    break;
  case ' ':
    window_flag_current_cell(window);

    break;
  }

  return 1;
}
