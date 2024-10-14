#include "IMUMeasurementStructure.h"

void resetMeasurementData( MeasurementData& data) {
  data.iterator = 0;
  data.seconds = 0;
}

void resetMeasurementMetadata(MeasurementMetadata& metadata) {
  metadata.hz = 0;
  metadata.measurementsIndex = 0;
  metadata.nCollectedDataPoints = 0;
}

void resetMeasurement( MeasurementData& data, MeasurementMetadata& metadata) {
  resetMeasurementData(data);
  resetMeasurementMetadata(metadata);
}