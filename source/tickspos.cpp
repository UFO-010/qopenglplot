#include "tickspos.h"
#include "math.h"


/*!
 * \brief getMantissa
 * \param input
 * \return
 */
double getMantissa(double input)
{
  double input_log = std::log10(input);
  double inpur_log_fl = std::floor(input_log);
  double mag = std::pow(10.0, inpur_log_fl);
  return input/mag;
}


/*!
 * \brief getMagnitude
 * \param input
 * \return
 */
double getMagnitude(double input)
{
  double input_log = std::log10(input);
  double inpur_log_fl = std::floor(input_log);
  double mag = std::pow(10.0, inpur_log_fl);
  return mag;
}


/*!
 * \brief pickClosest
 * \param target
 * \param candidates
 * \return
 */
double pickClosest(double target, std::vector<double> &candidates)
{
  if (candidates.size() == 1)
    //return candidates.first();
    return candidates.front();

  std::vector<double>::const_iterator it =
      std::lower_bound(candidates.begin(), candidates.end(), target);

  if (it == candidates.end())
    return *(it - 1);

  else if (it == candidates.begin())
    return *it;

  else
    return target-*(it - 1) < *it - target ? *(it - 1) : *it;
}


/*!
 * \brief cleanMantissa
 * \param input
 * \return
 */
double cleanMantissa(double input)
{
  double magnitude = getMagnitude(input);
  double mantissa = getMantissa(input);
  std::vector<double> cand = {1.0, 2.0, 2.5, 5.0, 10.0};

  return pickClosest(mantissa, cand)*magnitude;
}


/*!
 * \brief getFirstStep
 * \param min
 * \param tickStep
 * \return
 */
int getFirstStep(double min, double tickStep)
{
  return std::ceil(min/tickStep);
}


/*!
 * \brief getLastStep
 * \param max
 * \param tickStep
 * \return
 */
int getLastStep(double max, double tickStep)
{
  return std::ceil(max/tickStep);
}


/*!
 * \brief getTickCount
 * \param firstStep
 * \param maxNum
 * \param tickStep
 * \return
 */
int getTickCount(double firstStep, double maxNum, double tickStep)
{
  double lastStep = getLastStep(maxNum, tickStep);
  int tickCount = int(lastStep - firstStep);

  return tickCount;
}


/*!
 * \brief getTickStep
 * \param min
 * \param max
 * \param count
 * \return
 */
double getTickStep(double min, double max, int count)
{
  double exactStep = (max - min)/double(count + 1e-10);
  return cleanMantissa(exactStep);
}


/*!
 * \brief getSubTickCount
 * \param tickStep
 * \return
 */
int getSubTickCount(double tickStep)
{
  int result = 1; // default to 1, if no proper value can be found

  // separate integer and fractional part of mantissa:
  double epsilon = 0.01;
  double intPartf;
  int intPart;
  double mant = getMantissa(tickStep);
  double fracPart = modf(mant, &intPartf);
  intPart = int(intPartf);

  // handle cases with (almost) integer mantissa:
  if (fracPart < epsilon || 1.0 - fracPart < epsilon)
  {
    if (1.0-fracPart < epsilon)
      ++intPart;
    switch (intPart)
    {
      case 1: result = 4; break; // 1.0 -> 0.2 substep
      case 2: result = 3; break; // 2.0 -> 0.5 substep
      case 5: result = 4; break; // 5.0 -> 1.0 substep
    }
  } else
  {
    // handle cases with significantly fractional mantissa:
    if (std::abs(fracPart - 0.5) < epsilon) // *.5 mantissa
    {
      switch (intPart)
      {
        case 2: result = 4; break; // 2.5 -> 0.5 substep
      }
    }
    // if mantissa fraction isn't 0.0 or 0.5,
    // don't bother finding good sub tick marks, leave default
  }
  return result;
}


/*!
 * \brief getSubTickStep
 * \param tickStep
 * \param subTickCount
 * \return
 */
double getSubTickStep(double tickStep, int subTickCount)
{
  subTickCount++;
  return tickStep/subTickCount;
}


/*!
 * \brief createTickPosVector
 * \param tickStep
 * \param minMun
 * \param maxNum
 * \param tickPosVector
 */
void createTickPosVector(double tickStep, double minMun, double maxNum,
                         std::vector<double> &tickPosVector)
{
  double firstTickStep = getFirstStep(minMun, tickStep);
  int tickCount = getTickCount(firstTickStep, maxNum, tickStep);

  tickPosVector.resize(tickCount);
  for (int i=0; i<tickCount; ++i)
  {
    tickPosVector[i] = (firstTickStep+i)*tickStep;

  }
}


/*!
 * \brief createSubTickPosVector
 * \param tickStep
 * \param ticksPosVector
 * \param minNum
 * \param maxNum
 * \param subTickPosVector
 */
void createSubTickPosVector(double tickStep, std::vector<double> &ticksPosVector, double minNum,
                            double maxNum, std::vector<double> &subTickPosVector)
{
  subTickPosVector.clear();
  int subPerTickCount = getSubTickCount(tickStep);
  double subTickStep = getSubTickStep(tickStep, subPerTickCount);
  int addFront = 0;
  int addBack = 0;

  if(subTickStep > 0)
    {
      addFront = std::floor((ticksPosVector.front() - minNum)/subTickStep);
      addBack = std::floor((maxNum - ticksPosVector.back())/subTickStep);
    }

  int subTickCount = addFront + ((ticksPosVector.size() - 1) * subPerTickCount) + addBack;
  if(subTickCount <= 0) return;

  subTickPosVector.reserve(subTickCount);
  for(int i = addFront; i > 0; --i)
    {
      subTickPosVector.push_back(ticksPosVector[0] - i * subTickStep);
    }

  for(size_t i = 0; i < ticksPosVector.size() - 1; ++i)
    {
      for(int j = 1; j < subPerTickCount + 1; ++j)
        {
          subTickPosVector.push_back(ticksPosVector[i] + j * subTickStep);
        }
    }

  for(int i = 1; i < addBack + 1; ++i)
    {
      subTickPosVector.push_back(ticksPosVector.back() + i * subTickStep);
    }
  return;
}
