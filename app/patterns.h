#ifndef PATTERNS_H_
#define PATTERNS_H_

#include <zic_seq_pattern.h>
#include <zic_note.h>

#define PATTERN_COUNT_YO 25

// NOTE
// Should there be some banks?
// selectable with the lower row of pots
Zic_Seq_Pattern patterns[PATTERN_COUNT_YO] = {
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C3, SLIDE}, {_C3}, {0}, {_C3}, {END}},
    (Pat){{_C3}, {_E3}, {_F3}, {_C3}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
};

#endif
