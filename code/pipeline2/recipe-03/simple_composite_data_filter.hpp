#pragma once

#include "composite_data_filter.hpp"
#include "simple_data_filter.hpp"

template <typename InputType, typename OutputType>
class SimpleCompositeDataFilter: public CompositeDataFilter<InputType, OutputType> {
public:
    using Base = CompositeDataFilter<InputType, OutputType>;

    SimpleCompositeDataFilter() = default;
    ~SimpleCompositeDataFilter() = default;

    template <typename IT, typename OT>
    SimpleCompositeDataFilter& addDataFilter(std::function<OT(IT)> func) {
        std::shared_ptr<DataFilterAny> data_filter{
            new SimpleDataFilter<IT, OT>(func)
        };
        auto next_pipe = make_pipe<OT>();
        this->Base::addDataFilterAny(data_filter, next_pipe);
        return *this;
    }

};
