#ifndef __EVACUATION_DETECTOR_H__
#define __EVACUATION_DETECTOR_H__
#include <math.h>

#include "WindowedQueue.h"

class EvacuationDetector
{
    WindowedQueue<float> source_values_;
    WindowedQueue<float> window_;
    float filter_gain_;
    float min_;
    public:

        EvacuationDetector(size_t duration, size_t window_size = 0, float filter_gain = 0.01, float min = 0.0001)
            : source_values_(duration)
            , window_(window_size ? window_size : duration / 10)
            , filter_gain_(filter_gain)
            , min_(min)
        {}

        bool process(float value)
        {
            bool detected = false;
            window_.push(value);
            if (window_.full()) {
                if (window_.deviation() > min_) {
                    if (source_values_.empty()) {
                        source_values_.push(window_);
                    } else {
                        source_values_.push(value);
                    }
                    if (source_values_.full()) {
                        float mean = source_values_.mean();
                        WindowedQueue<float> window(window_.capacity());
                        detected = true;
                        for (size_t i = 0; i < source_values_.count(); ++i) {
                            float value = source_values_.get(i);
                            window.push(fabs(value - mean) <= filter_gain_ ? mean : value);
                            if (window.full()) {
                                if (window.deviation() < min_) {
                                    detected = false;
                                    break;
                                }
                            }
                        }
                    }
                } else {
                    source_values_.clear();
                }
            }
            return detected;
        }

        void clear()
        {
            source_values_.clear();
            window_.clear();
        }
};

#endif
