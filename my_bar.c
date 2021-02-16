#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>



int main() {
  int version = 1;
  int stop_signal = 19;
  int cont_signal = 12;
  char *click_events = "true";
  const int header_size = 512;
  char header[header_size];
  memset(header, '\0', header_size);
  sprintf(header, "{"
      "\"version\":%d,"
      "\"stop_signal\":%d,"
      "\"cont_signal\":%d,"
      "\"click_events\":\"%s\""
      "}", version, stop_signal, cont_signal, click_events);
  printf(header);
  printf("[");
  printf("[]");

  FILE *battery_file = fopen("/sys/class/power_supply/BAT0/capacity", "r");
  int percent = 0;

  while(1) {
    const int buffer_size = 512;
    char buffer[buffer_size];
    memset(buffer, '\0', buffer_size);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char date[14];
    char time[10];
    memset(date, '\0', 30);
    memset(time, '\0', 30);
    sprintf(date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    sprintf(time, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

    char battery_str[6];
    if (battery_file != NULL) {
      fscanf(battery_file, "%d", &percent);
      sprintf(battery_str, "%d%%", percent);
    }

    sprintf(buffer, 
      ", ["
      "{"
        "\"full_text\":\"%s\""
      "},"
      "{"
        "\"full_text\":\"<span size='large' font_family='Material Icons' foreground='green'>battery_std</span>\"," 
        "\"markup\":\"pango\","
        "\"separator\":false,"
        "\"separator_block_width\":0"
      "},"
      "{"
        "\"full_text\":\"%s\"" 
      "},"
      "{"
        "\"full_text\":\"%s\""
      "}]",
      date,
      battery_str,
      time
    );
    printf(buffer);
    memset(buffer, '\0', buffer_size);
    usleep(500);
  }

  if (battery_file != NULL) {
    fclose(battery_file);
  }

  return 0;
}
