#ifndef TICKSPOS_H
#define TICKSPOS_H

#include <vector>

double getMantissa(double input);
double getMagnitude(double input);
double cleanMantissa(double input);

double pickClosest(double target, std::vector<double> &candidates);

int getFirstStep(double min, double tickStep);
int getLastStep(double max, double tickStep);

int getTickCount(double firstStep, double maxNum, double tickStep);
double getTickStep(double min, double max, int count);

int getSubTickCount(double tickStep);
double getSubTickStep(double tickStep, int subTickCount);

void createTickPosVector(double tickStep, double minMun, double maxNum,
                         std::vector<double> &tickPosVector);

void createSubTickPosVector(double tickStep, std::vector<double> &ticksPosVector,
                            double minNum, double maxNum,
                            std::vector<double> &subTickPosVector);


#endif
