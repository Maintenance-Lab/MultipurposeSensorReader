#include "IMUMeasurementStructure.h"

void resetMeasurement( MeasurementData& data, MeasurementMetadata& metadata) {
  data.iterator = 0;
  data.seconds = 0;
  metadata.hz = 0;
}