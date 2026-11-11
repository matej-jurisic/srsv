#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <time.h>

long get_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int main(int argc, char **argv)
{
    int f = open(argv[1], O_RDONLY | O_NONBLOCK);

    long last_press = 0;
    long last_release = 0;

    int long_press = 0;
    int short_press_count = 0;
    int action_completed = 0;

    int two_fast = 0;

    struct input_event ev;

    while (1)
    {
        if (read(f, &ev, sizeof(ev)) == sizeof(ev) && ev.type == EV_KEY && ev.code == KEY_LEFTCTRL)
        {
            if (ev.value == 1)
            {
                last_press = get_ms();

                if (short_press_count == 2 && last_press - last_release > 500)
                {
                    two_fast = 1;
                }

                if (action_completed)
                {
                    short_press_count = 0;
                    long_press = 0;
                    two_fast = 0;
                }
                action_completed = 0;
            }
            if (ev.value == 0)
            {
                last_release = get_ms();
                if (!action_completed)
                {
                    if (last_release - last_press > 500)
                    {
                        printf("Krivo korištenje!\n");
                        action_completed = 1;
                    }
                    else
                    {
                        short_press_count++;
                    }
                }
            }
        }

        long now = get_ms();

        if (now - last_release >= 2000 && last_press < last_release && !action_completed)
        {
            if (long_press)
                printf("Dugi pritisak!\n");
            else if (short_press_count == 1)
            {
                printf("Kratki pritisak!\n");
            }
            else if (short_press_count == 4 && two_fast)
            {
                printf("Dva brza dvostruka pritiska!\n");
            }
            else if (short_press_count > 1)
            {
                printf("%d kratkih pritisaka!\n", short_press_count);
            }
            action_completed = 1;
        }

        // if (now - last_press >= 5000 && last_press > last_release && !action_completed)
        // {
        //     printf("Akcija E!\n");
        //     action_completed = 1;
        // }
    }
}