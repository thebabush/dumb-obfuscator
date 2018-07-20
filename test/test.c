#include <stdio.h>

int main() {
  char c;

  puts("Show me your character:");
  c = getchar();

  if (c + 0x5A == 0xAA) {
    puts("You are a proper Prosecco drinker mate, come join the AA!");
  } else {
    puts("Looks like we got a non drinker here, sad");
  }

  return 0;
}

