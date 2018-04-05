#pragma once

#include "color/assert.hpp"
#include "color/math.hpp"

#include <vector>

namespace color {

class CMF;

class SPD {
public:
    explicit SPD(float start, float end, float step,
                                       float v = 0.0f)
        : _step(step) {
        color_assert(end >= start + step, "end of range is less than step");

        for (float l = start; l < end; l += step) {
            _wavelengths.push_back(l);
            _values.push_back(v);
        }
    }

    SPD(std::vector<float> w, std::vector<float> v)
        : _wavelengths(std::move(w)), _values(std::move(v)) {
        color_assert(_wavelengths.size() == _values.size(),
                     "wavelengh and value array sizes do not match");
        color_assert(_wavelengths.size() > 1,
                     "number of wavelengths must be greater than 1");

        // scan through the wavelengths and determine if it's a uniform
        // distribution or not
        _step = _wavelengths[1] - _wavelengths[0];
        for (size_t i = 2; i < _wavelengths.size(); ++i) {
            if (_wavelengths[i] - _wavelengths[i - 1] != _step) {
                _step = 0.0f;
                break;
            }
        }
    }

    SPD(float start, float end, float step,
                              std::vector<float>&& v) {
        _step = step;
        float end_inc = end - step;
        color_assert(int((start - end_inc) / step) == v.size(),
                     "range does not match size of values array");

        _values = std::move(v);

        for (float l = start; l < end; l += step) {
            _wavelengths.push_back(l);
        }

        color_assert(_wavelengths.size() == _values.size(),
                     "number of wavelengths does not match number of values");
    }

    SPD(const SPD&) = default;
    SPD(SPD&&) = default;
    SPD&
    operator=(const SPD&) = default;
    SPD& operator=(SPD&&) = default;

    bool operator==(const SPD& rhs) {
        if (start() != rhs.start() || end() != rhs.end() ||
            step() != rhs.step())
            return false;

        for (size_t i = 0; i < num_samples(); ++i) {
            if (_wavelengths[i] != rhs._wavelengths[i])
                return false;
            if (_values[i] != rhs._values[i])
                return false;
        }

        return true;
    }

    bool operator!=(const SPD& rhs) {
        return !(*this == rhs);
    }

    float start() const { return _wavelengths[0]; }
    float end() const { return _wavelengths[_wavelengths.size() - 1] + _step; }
    float step() const { return _step; }
    size_t num_samples() const { return _wavelengths.size(); }
    bool is_uniform() const { return !(_step == 0.0f); }

    /**
     * @brief Check if rhs has the same range and step size as this
     */
    bool is_equal_scale(const SPD& rhs) const {
        if (start() != rhs.start() || end() != rhs.end() ||
            step() != rhs.step())
            return false;

        for (size_t i = 0; i < _wavelengths.size(); ++i) {
            if (_wavelengths[i] != rhs._wavelengths[i])
                return false;
        }

        return true;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const SPD& spd);

    // void write(std::ostream& os) const {
    //     for (size_t i = 0; i < _wavelengths.size(); ++i) {
    //         os << _wavelengths[i] << " " << _values[i] << std::endl;
    //     }
    // }

    // interpolate this SPD from another
    // currently rhs must have a wider range than this spd (no extrapolation)
    void interpolate_from(const SPD& rhs) {
        size_t sz = _wavelengths.size();

        // if the other SPD is the same distribution as us, just copy it
        if (is_equal_scale(rhs)) {
            *this = rhs;
            return;
        }

        // first check that the range of the SPD we're interpolating is
        // at least as big as the range we're representing
        // i.e. we don't do extrapolation yet
        color_assert(_wavelengths[0] >= _wavelengths[0], "");
        color_assert(_wavelengths[sz - 1] <=
                         rhs._wavelengths[rhs._wavelengths.size() - 1],
                     "");
        color_assert(rhs._wavelengths.size() > 1, "");

        // foreach wavelength we want to interpolate onto
        for (size_t i = 0; i < sz; ++i) {
            size_t j1 = 0;
            size_t j0 = 0;
            // find the first wavelength in rhs that's past our desired
            // wavelength
            while (rhs._wavelengths[j1++] < _wavelengths[i])
                ;

            // j is now the index of the first wavelength greater than our
            // desired
            // get the preceding index
            if (j1 > 0) {
                j0 = j1 - 1;

                // get the t of where our wavelength sits between the rhs ones
                float t = (_wavelengths[i] - rhs._wavelengths[j0]) /
                          (rhs._wavelengths[j1] - rhs._wavelengths[j0]);

                // lerp to get our new value
                _values[i] = lerp(rhs._values[j0], rhs._values[j1], t);
            } else {
                _values[i] = rhs._values[j1];
            }
        }
    }

    /**
     * @brief Interpolate this SPD onto a simple float array of values
     * @details Used by the CoefficientSpectrum class to fill out its
     * precomputed
     * arrays
     */
    void interpolate_onto(float* v, float lambda_start, float lambda_end,
                          int sz) const {
        color_assert(lambda_start >= start(), "");
        color_assert(lambda_end - (lambda_end-lambda_start)/sz <= _wavelengths.back(), "");

        // if sampling is the same, just copy
        if (sz == _wavelengths.size() && lambda_start == start() &&
            lambda_end == end())
            memcpy(v, &(_wavelengths[0]), sizeof(float) * sz);

        float lambda = lambda_start;
        float lambda_step = (lambda_end - lambda_start) / float(sz);
        for (int i = 0; i < sz; ++i, lambda += lambda_step) {
            int j1 = 0;
            int j0 = 0;
            while (_wavelengths[j1++] < lambda)
                ;

            if (j1 > 0) {
                j0 = j1 - 1;
                float t = (lambda - _wavelengths[j0]) /
                          (_wavelengths[j1] - _wavelengths[j0]);

                v[i] = lerp(_values[j0], _values[j1], t);
            } else {
                v[i] = _values[0];
            }
        }
    }

    //
    float value(float lambda) const {
        color_assert(lambda >= start(),
                     "lambda ({}) is less than beginning of spd range ({})",
                     lambda, start());

        color_assert(lambda <= end(),
                     "lambda ({}) is greater than end of spd range ({})",
                     lambda, end());

        color_assert(_step != 0,
                     "cannot interpolate value from nonuniform SPD");

        float s = start();
        float e = end();
        float sz = e - s;
        int num = _wavelengths.size();
        float t = (lambda - s) / (sz);
        int i_0 = int(t * num);
        if (i_0 < _wavelengths.size() - 1) {
            int i_1 = i_0 + 1;
            float w_0 = _wavelengths[i_0];
            float w_1 = _wavelengths[i_1];
            float t_lambda = (lambda - w_0) / (w_1 - w_0);
            return lerp(_values[i_0], _values[i_1], t_lambda);
        } else {
            return _values[i_0];
        }
    }

    friend auto spd_to_xyz(const SPD& spd, const CMF& cmf, const SPD& illuminant) -> XYZ;
    friend auto spd_to_xyz(const SPD& spd, const CMF& cmf) -> XYZ;

private:
    // step size between wavelength samples. will be zero if non-uniform
    float _step;
    std::vector<float> _wavelengths;
    std::vector<float> _values;
};

inline std::ostream& operator<<(std::ostream& os,
                                const SPD& spd) {
    os << "{";
    for (size_t i = 0; i < spd._wavelengths.size(); ++i) {
        os << fmt::format("{:.2f}: {:.2f}\n", spd._wavelengths[i],
                          spd._values[i]);
    }
    os << "}";
    return os;
}



}
