// Copyright 2021 Pinezhanin Evgeny
#ifndef MODULES_TASK_1_PINEZHANIN_E_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
#define MODULES_TASK_1_PINEZHANIN_E_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_

#include <functional>

double getIntegralTrapezoidalRuleParallel(const std::function<double(double)>& f, double a, double b, int n);
double getIntegralTrapezoidalRuleSequential(const std::function<double(double)>& f, double a, double b, int n);

#endif  // MODULES_TASK_1_PINEZHANIN_E_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
