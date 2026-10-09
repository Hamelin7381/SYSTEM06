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

<img width="792" height="177" alt="스크린샷 2026-10-09 193026" src="https://github.com/user-attachments/assets/1ff02c87-f5e6-4711-93e8-4ec2dc14c340" />


2. 타이머 만들기
기존 코드는 3초 뒤에 한 번만 SIGALRM이 발생하고, 입력 시간을 확인하는 구조였다. 간격과 반복 횟수를 입력받아 알람이 반복되도록 수정하였다.
main() 함수의 인자를 argc와 argv로 변경하여 실행할 때 간격과 반복 횟수를 입력받도록 하였다. 예를 들면 ./alarm 2 5를 실행하면 2초 간격으로 총 5번 알람이 발생한다.
기존의 timeout 플래그를 count 카운터로 변경하여 알람이 발생할 때마다 횟수를 증가시키도록 하였다. 핸들러에서 count가 반복 횟수보다 작으면 alarm(interval)을 다시 설정하여 알람이 반복되도록 하였다.

원리
프로그램을 실행하면 입력한 간격과 반복 횟수를 저장한다.
지정한 시간이 지나면 SIGALRM이 발생하고 on_alarm() 핸들러가 실행된다.
핸들러에서는 count를 증가시키고, 반복 횟수가 남아 있으면 다음 알람을 예약한다.
main()에서는 알람 횟수를 출력하고, 설정한 횟수에 도달하면 타이머 종료 메시지를 출력한다.

사용한 프롬프트
2_alarm.c를 수정해서 ./alarm <간격초> <반복횟수> 형식으로 실행할 수 있는 타이머를 만들어 줘. alarm()은 한 번만 울리므로 핸들러에서 알람 횟수를 세고, 반복 횟수가 남아 있으면 alarm(interval)을 다시 설정하도록 해 줘.

<img width="773" height="251" alt="스크린샷 2026-10-09 201455" src="https://github.com/user-attachments/assets/f1a35dd7-ff6b-4910-9cbc-7d8db62616da" />


3. 시그널 막아보기
기존 코드에 sigprocmask()를 이용하여 SIGINT 시그널을 차단하고 해제하는 기능이 구현되어 있었다. 여기에 sigaction()과 sigprocmask()의 오류를 확인하는 코드를 추가하였다.
sigprocmask(SIG_BLOCK, &block, &old)를 사용하여 SIGINT를 차단하고, sleep(5)로 5초 동안 기다리도록 하였다. 차단된 상태에서 Ctrl+C를 누르면 시그널이 바로 처리되지 않고 대기 상태로 남는다.
5초가 지나면 got 변수를 확인하고, sigprocmask(SIG_SETMASK, &old, NULL)을 사용하여 기존 시그널 마스크를 복원한다. 이때 대기 중이던 SIGINT가 전달되어 핸들러가 실행되고, got 값이 1로 변경된다.

원리
Ctrl+C를 누르면 SIGINT 시그널이 발생한다.
하지만 시그널이 차단된 동안에는 핸들러가 실행되지 않고 시그널이 대기 상태로 남는다.
차단을 해제하면 대기 중이던 시그널이 전달되어 핸들러가 실행되고, 이를 통해 시그널이 정상적으로 전달되었는지 확인할 수 있다.

사용한 프롬프트
3_signal_block.c를 이용하여 SIGINT를 5초 동안 차단한 뒤 해제하는 프로그램을 작성해 줘. 차단 중에 Ctrl+C를 누르면 시그널이 대기 상태로 남아 있다가 차단 해제 후 전달되는지 확인할 수 있도록 해 줘.

<img width="925" height="274" alt="스크린샷 2026-10-09 204807" src="https://github.com/user-attachments/assets/437c500d-5ac7-458a-b39c-e0d8717f5e16" />
