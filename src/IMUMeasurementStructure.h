  #ifndef IMUMEASUREMENTSTRUCTURE_H  // Include guard start
#define IMUMEASUREMENTSTRUCTURE_H

struct MeasurementMetadata {
  int hz;
};

struct MeasurementConfig {
  int setHz;
  int saveAtNDataPoints;
};

struct MeasurementData {
  int iterator;
  int seconds;
};

void resetMeasurement(MeasurementData& data, MeasurementMetadata& metadata);

#endif  // IMUMEASUREMENTSTRUCTURE_H  // Include guard end
