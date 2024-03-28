#pragma once

#include <memory>
#include <functional>
#include "composite_data_filter.hpp"
#include "simple_data_filter.hpp"

template <typename InputType, typename OutputType>
class SimpleCompositeDataFilter: public CompositeDataFilter<InputType, OutputType> {
public:
    using Base = CompositeDataFilter<InputType, OutputType>;

    SimpleCompositeDataFilter(size_t capacity_per_pipe_): CompositeDataFilter<InputType, OutputType>(capacity_per_pipe_) {
    }

    ~SimpleCompositeDataFilter() = default;

    template <typename IT, typename OT>
    SimpleCompositeDataFilter& addDataFilter(std::function<OT(IT)> func) {
        std::shared_ptr<DataFilterAny> data_filter{
            new SimpleDataFilter<IT, OT>(func)
        };
        auto next_pipe = make_pipe<OT>(this->capacity_per_pipe);
        this->Base::addDataFilterAny(data_filter, next_pipe);
        return *this;
    }

};

template <typename IT, typename OT>
std::shared_ptr<SimpleCompositeDataFilter<IT, OT>> make_simple_composite_data_filter(size_t capacity_per_pipe) {
    return std::make_shared<SimpleCompositeDataFilter<IT, OT>>(capacity_per_pipe);
}

template <typename IT, typename OT>
std::shared_ptr<CompositeDataFilter<IT, OT>> to_composite_data_filter(std::shared_ptr<SimpleCompositeDataFilter<IT, OT>> composite_data_filter) {
    return std::static_pointer_cast<CompositeDataFilter<IT, OT>>(composite_data_filter);
}
