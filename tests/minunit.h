//
// Created by Hugo on 04/11/2017.
//

#ifndef CBDD1_MIUNIT_H
#define CBDD1_MIUNIT_H

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)

#endif //CBDD1_MIUNIT_H
