# SYSTEM06
1.  Ctrl+C 를 세 번 눌러야 종료
기존의 got_sigint는 SIGINT가 한 번 발생했는지만 확인하는 플래그였기 때문에, Ctrl+C를 누른 횟수를 세기 위해 sigint_count라는 카운터로 변경하였다. 핸들러의 got_sigint = 1;을 sigint_count++;로 수정하여 Ctrl+C가 입력될 때마다 카운터가 1씩 증가하도록 하였다.

while (!got_sigint)
    pause();

는 SIGINT가 한 번 발생하면 반복을 종료하는 구조였다.

while (sigint_count < 3)
    pause();

로 변경하여 Ctrl+C를 세 번 누를 때까지 기다리도록 수정하였다. 카운터가 3이 되면 반복문을 빠져나와 main()에서 종료 메시지를 출력하고 프로그램을 종료한다.

원리
Ctrl+C를 누르면 SIGINT가 발생하고 handler()가 실행된다.
handler()에서는 volatile sig_atomic_t 카운터만 1 증가시킨다.
main()은 pause()로 기다리면서 카운터가 3이 될 때까지 확인한다.
3번째 Ctrl+C가 들어오면 main()에서 정리 메시지를 출력하고 종료한다.
printf()는 시그널 핸들러가 아니라 main()에서 실행한다.

사용한 프롬프트
1_sigint.c를 수정해서 Ctrl+C를 세 번 누르면 종료되게 해줘. 핸들러에서는 volatile sig_atomic_t 카운터만 증가시키고, printf()는 main()에서 하도록 해줘.
<img width="492" height="147" alt="스크린샷 2026-10-07 121338" src="https://github.com/user-attachments/assets/fad2b35f-844c-4bfd-a191-f96105d47c08" />
