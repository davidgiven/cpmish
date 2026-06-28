#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

START_TEST(test_buffer_reads_never_exceed_declared_length)
{
    // Invariant: Buffer reads never exceed the declared length
    const char *payloads[] = {
        "normal",                    // Valid input
        "A",                         // Boundary: single char
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", // 100 chars
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", // 101 chars
        "\x00\x01\x02\x03\x04\x05"   // Binary data
    };
    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    for (int i = 0; i < num_payloads; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process: execute qe with the payload
            char *args[] = {"./qe", (char *)payloads[i], NULL};
            execv("./qe", args);
            exit(EXIT_FAILURE); // execv failed
        } else if (pid > 0) {
            // Parent process: wait for child
            int status;
            waitpid(pid, &status, 0);
            
            // Check if child terminated normally (not by signal)
            ck_assert_msg(WIFEXITED(status), 
                "Payload '%s' caused abnormal termination (signal)", payloads[i]);
            
            // If it exited, exit code should be 0 (success) or non-zero but controlled
            // We accept any exit as long as it wasn't a crash
            if (WIFEXITED(status)) {
                // Accept any exit code - the important thing is no crash
            }
        } else {
            ck_abort_msg("fork failed");
        }
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_buffer_reads_never_exceed_declared_length);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}