//
// Created by Hugo on 04/11/2017.
//

#ifndef CBDD1_MIUNIT_H
#define CBDD1_MIUNIT_H

#define mu_assert(message, test) do { return (test) ? 0 : (message); } while (0)
#define mu_run_test(test) do { char *message = test(); if (message) return message; } while (0)

#endif //CBDD1_MIUNIT_H
