/*
 * 2_alarm.c — 정해진 시간 뒤에 시그널을 받는다
 *
 * [핵심 개념]
 *   alarm(n) 은 n 초 뒤에 커널이 SIGALRM 을 보내도록 예약한다.
 *   시간 제한(타임아웃)을 구현하는 가장 간단한 방법이다.
 *   출처: man 2 alarm, man 2 sigaction
 *
 * [컴파일·실행]
 *   gcc -Wall -Wextra -o 2_alarm 2_alarm.c
 *   ./2_alarm      # 3초 안에 뭔가 입력하지 않으면 시간이 끝난다
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t count = 0;
static int interval;
static int repeat_count;

static void on_alarm(int sig)
{
    (void)sig;
    count++;

    if (count < repeat_count)
        alarm(interval);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
    printf("사용법: %s <간격초> <반복횟수>\n", argv[0]);
    return 1;
}

interval = atoi(argv[1]);
repeat_count = atoi(argv[2]);

if (interval <= 0 || repeat_count <= 0) {
    printf("간격과 반복횟수는 양수여야 합니다.\n");
    return 1;
}

    struct sigaction sa;
    sa.sa_handler = on_alarm;   /* SIGALRM 이 오면 이 함수를 부르게 등록한다 */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;   /* SA_RESTART 를 안 줬으므로, 이 시그널은 fgets 같은 블로킹 호출을 중단시킨다 */
    sigaction(SIGALRM, &sa, NULL);

    alarm(interval);

while (count < repeat_count) {
    pause();
    printf("알림 %d/%d\n", (int)count, repeat_count);
}

printf("타이머 종료\n");
    return 0;
}