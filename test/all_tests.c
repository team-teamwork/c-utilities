#include "queue_test.h"
#include "util_test.h"
#include "regpdu_test.h"
#include "alivepdu_test.h"
#include "joinpdu_test.h"
#include "quitpdu_test.h"
#include "ackpdu_test.h"
#include "notregpdu_test.h"

int main(void) {
    util_tests();
    reg_pdu_tests();
    alive_pdu_tests();
    join_pdu_tests();
    quit_pdu_tests();
    ack_pdu_tests();
    notreg_pdu_tests();
    queue_tests();

    return 0;
}
