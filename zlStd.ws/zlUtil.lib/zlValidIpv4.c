#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlStdComun.h>



zlBool_t isValidIp4 (char *str) {
    int segs = 0;   /* Segment count. */
    int chcnt = 0;  /* Character count within segment. */
    int accum = 0;  /* Accumulator for segment. */

    /* Catch NULL pointer. */

    if (str == NULL)
        return zlFalse;

    /* Process every character in string. */

    while (*str != '\0') {
        /* Segment changeover. */

        if (*str == '.') {
            /* Must have some digits in segment. */

            if (chcnt == 0)
                return zlFalse;

            /* Limit number of segments. */

            if (++segs == 4)
                return zlFalse;

            /* Reset segment values and restart loop. */

            chcnt = accum = 0;
            str++;
            continue;
        }
        /* Check numeric. */

        if ((*str < '0') || (*str > '9'))
            return zlFalse;

        /* Accumulate and check segment. */

        if ((accum = accum * 10 + *str - '0') > 255)
            return zlFalse;

        /* Advance other segment specific stuff and continue loop. */

        chcnt++;
        str++;
    }

    /* Check enough segments and enough characters in last segment. */

    if (segs != 3)
        return zlFalse;

    if (chcnt == 0)
        return zlFalse;

    /* Address okay. */

    return zlTrue;
}
